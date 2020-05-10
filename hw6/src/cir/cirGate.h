/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;
class CirPiGate;
class CirPoGate;
class CirAigGate;
//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
   CirGate() {}
   virtual ~CirGate() {}

   // Basic access methods
   virtual string getTypeStr() const { return ""; }
   virtual unsigned getLineNo() const { return 0; }
   virtual int getf0(){return -1;}
   virtual void setf0(CirGate* f0){}
   virtual int getf1(){return -1;}
   virtual void setf1(CirGate* f1){}
   virtual int getID() const {return -1;}
   virtual void printf0() {cout << "you fail\n";}
   virtual void printf1() {cout << "you fail\n";}
   virtual CirGate* getf0add(){return NULL;}
   virtual CirGate* getf1add(){return NULL;}
   virtual bool getf0inv() {return NULL;}
   virtual bool getf1inv() {return NULL;}
   bool gateexist = 1;
   bool isvisited = 0;
   virtual void setsymbol(string sym) {}
   virtual string getsymbol() const {return "-1";}

   virtual void addFanin(int id) {Lfanin.push_back(id);}
   virtual void addFanout(int id) {Lfanout.push_back(id);}




   CirGate* f0_addr;


   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;
   void dfsFanin(int depth, int cur, vector<bool>& vis) const;
   void dfsFanout(int depth, int cur, vector<bool>& vis) const;

private:


protected:
    vector<int> Lfanin;
    vector<int> Lfanout;


};


class CirPiGate : public CirGate
{
public:
    CirPiGate() {}
    CirPiGate(int ID, int No) :gateID(ID),lineNo(No) {}
    virtual ~CirPiGate() {}
    void printGate() const {}
    int getID() const {return gateID;}
    int getf0() {return -1;}
    void setf0(CirGate* f0){}
    int getf1(){return -1;}
    void setf1(CirGate* f1){}
    bool gateexist = 1;
    bool isvisited = 0;
  //Basic access methods    
   string getTypeStr() const { return "Pi"; }
   virtual unsigned getLineNo() const { return lineNo; }
    void setsymbol(string sym) {symbol = sym;}
    string getsymbol() const{return symbol;}

   void addFanin(int id) {Lfanin.push_back(id);}
   void addFanout(int id) {Lfanout.push_back(id);}



private:
  int gateID;
  int lineNo;
  string symbol;

};

class CirPoGate : public CirGate
{
public:
    CirPoGate() {}
    CirPoGate(int in0, int ID, int No, bool is0inv) :fainin0(in0),gateID(ID), lineNo(No), isinv(is0inv) {}
    virtual ~CirPoGate() {}
    void printGate() const {}
    
    // Basic access methods
    string getTypeStr() const { return "Po"; }
    unsigned getLineNo() const { return lineNo; }
    void setf0(CirGate* f0) {f0_addr = f0;}
    int getf0(){return fainin0;}
    int getf1(){return -1;}
    void setf1(CirGate* f1){}
    int getID() const{return gateID;}
    void printf0() {cout << (f0_addr->getID());}
    CirGate* getf0add(){return f0_addr;}
    bool getf0inv() {return isinv;}
    bool gateexist = 1;
    bool isvisited = 0;
    void setsymbol(string sym) {symbol = sym;}
    string getsymbol() const{return symbol;}
   void addFanin(int id) {Lfanin.push_back(id);}
   void addFanout(int id) {Lfanout.push_back(id);}



private:
  int fainin0;
  int gateID;
  int lineNo;
  bool isinv;
  CirGate* f0_addr;
  string symbol;


};

class CirAigGate : public CirGate
{
public:
    CirAigGate() {}
    CirAigGate(int in0, int in1, int ID, int No, bool is0inv, bool is1inv) :fainin0(in0),fainin1(in1),gateID(ID), lineNo(No), is0inv(is0inv), is1inv(is1inv) {}
    virtual ~CirAigGate() {}
    void printGate() const {}
    int getID() const {return gateID;}
    int getf0(){return fainin0;}
    int getf1(){return fainin1;}
    void setf0(CirGate* f0){ f0_addr = f0; }
    void setf1(CirGate* f1){ f1_addr = f1;}
    CirGate* getf0add(){return f0_addr;}
    CirGate* getf1add(){return f1_addr;}
    bool getf0inv() {return is0inv;}
    bool getf1inv() {return is1inv;}
    bool gateexist = 1;
    bool isvisited = 0;
   void printf0() 
    {
      if(f0_addr == NULL)
        cout << "f0 is null\n";
      else
        cout << (f0_addr->getID());
    }
    void printf1() 
    {
      if(f1_addr == NULL)
        cout << "f1 is null\n";
      else
        cout << (f1_addr->getID());
    }



    // Basic access methods
    virtual string getTypeStr() const { return "Aig"; }
    virtual unsigned getLineNo() const { return lineNo; }

   void addFanin(int id) {Lfanin.push_back(id);}
   void addFanout(int id) {Lfanout.push_back(id);}





private:
  int fainin0;
  int fainin1;
  int gateID;
  int lineNo;
  bool is0inv, is1inv;
  CirGate* f0_addr;
  CirGate* f1_addr;
  


};


#endif // CIR_GATE_H
