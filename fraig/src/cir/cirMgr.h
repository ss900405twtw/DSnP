/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <fstream>


using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"

extern CirMgr *cirMgr;
class CirIOGate;
class CirAndGate;
class CirConstGate;

class CirMgr
{
public:
   CirMgr();
   ~CirMgr();

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const;

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void optimize();


   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*) const;
   void buildDFSwrapper();
   void buildFloatingFanin();
   void buildDefinedButNotUsed();

   operator int()
   {
      return hasCircuit?1:0;
   }

private:
   ofstream           *_simLog;
   fstream* fCir;

   bool hasCircuit;
   unsigned int M; // maximum variable index
   unsigned int I; // number of inputs
   unsigned int L; // number of latches
   unsigned int O; // number of outputs
   unsigned int A; // number of AND gates
   CirGate** gates;
   vector<unsigned int> PI;
   vector<unsigned int> PO;
   vector<unsigned int> undefs;
   vector<unsigned int> dfsOrder;
   vector<unsigned int> AIGinDFSOrder;
   vector<unsigned int> dfsOrder2;
   vector<unsigned int> AIGinDFSOrder2;
   vector<unsigned int> notInDFS;
   vector<unsigned int> floatingFanin;

   /* helper functions */
   unsigned int buildDFSOrder(CirGate*, unsigned int);
   unsigned int buildDFSOrder2(CirGate*, unsigned int);

};

#endif // CIR_MGR_H
