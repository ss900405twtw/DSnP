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

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {*(reinterpret_cast<int*>(const0)) = 0;}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const 
   {
      CirGate* ans = findgate(gid);
      if(!ans) return 0; 
      else return ans;

    }
    int gatesize() const {return M;}

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

   //my function
   void readHeader();
   void readInput();
   void readOutput();
   void readAig();
   void readsymbol();
   static int* const0;
   void PoConnection();
   void AigConnection();
   void DFSPo();
   void DFSAig(CirGate*);
   CirGate* findgate(unsigned) const;


private:
  int M, I, L, O, A;
  int line_count = 0;
  fstream newfile;
  vector<CirGate*> _piList;
  vector<CirGate*> _poList;
  vector<CirGate*> _paigList;
  vector<CirGate*> _gateList;
  vector<CirGate*> _dfsList;
  vector<CirGate*> _nullList;

};

#endif // CIR_MGR_H
