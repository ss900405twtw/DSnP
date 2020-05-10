/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;
int b = 0;
int *CirMgr::const0 = &b;


enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
    newfile.open(fileName, ios::in);
    if(!newfile.is_open())
    {
      cout << "Cannot open design \""<< fileName <<"\"!!\n";
      return false;
    }
    readHeader();
    // cout << M << " " << I << " " << L << " " << O << " " 
    // << A << " " << line_count << endl;
    readInput();
    // cout << "input: " << line_count << endl; 
    readOutput();
    // cout << "output: " << line_count << endl; 
    readAig();
    // cout << "Aig: " << line_count << endl; 
    readsymbol();
    PoConnection();
    // cout << "aig is slow\n";
    AigConnection();
    //DFS
    // cout << "dfs is slow\n";
    DFSPo();

   return true;
}

void
CirMgr::readHeader()
{
  if(newfile.is_open())
  {
    string tp;
    getline(newfile, tp);
    istringstream iss(tp);
    string aag;
    iss >> aag >> M >> I >> L >> O >> A;
    line_count++;
  }
}

void
CirMgr::readInput()
{
  if(newfile.is_open())
  {
    string tp;
    for(int i = 0; i < I; i++)
    {
      getline(newfile, tp);
      line_count++;
      istringstream iss(tp);
      int gateID;
      iss >> gateID;
      // CirGate *pi = new CirPiGate(gateID,line_count);
      CirGate *pi = new CirPiGate(int(gateID/2), line_count);
      _piList.push_back(pi);
      _gateList.push_back(pi);


    }
  }
}

void
CirMgr::readOutput()
{
  if(newfile.is_open())
  {
    string tp;
    int newM = M;
    for(int i = 0; i < O; i++)
    {
      getline(newfile, tp);
      line_count++;
      newM++;
      istringstream iss(tp);
      int fanin0;
      bool isinv;
      iss >> fanin0;
      isinv = (fanin0 % 2 == 0)? 0 : 1;
      fanin0 = int(floor(fanin0/2));
      CirGate *po = new CirPoGate(fanin0, newM, line_count,isinv);
      _poList.push_back(po);
      _gateList.push_back(po);

      //addfanin
      fanin0 = (isinv == 0)? fanin0*2: fanin0*2+1;
      po->addFanin(fanin0);
      po->addFanout(fanin0);

    }
  }
}

void
CirMgr::readAig()
{
  if(newfile.is_open())
  {
    string tp;
    for(int i = 0; i < A; i++)
    {
      getline(newfile, tp);
      line_count++;
      istringstream iss(tp);
      int gateID, fanin0, fanin1;
      bool is0inv, is1inv;
      iss >> gateID >> fanin0 >> fanin1;
      is0inv = (fanin0 % 2 == 0)? 0 : 1;
      is1inv = (fanin1 % 2 == 0)? 0 : 1;
      fanin0 = int(floor(fanin0/2));
      fanin1 = int(floor(fanin1/2));
      CirGate *paig = new CirAigGate(fanin0, fanin1, int(gateID/2), line_count, is0inv, is1inv);
      _paigList.push_back(paig);
      _gateList.push_back(paig);


      //addfanin
      fanin0 = (is0inv == 0)? fanin0*2: fanin0*2+1;
      paig->addFanin(fanin0);
      paig->addFanout(fanin0);

      fanin1 = (is1inv == 0)? fanin1*2: fanin1*2+1;
      paig->addFanin(fanin1);
      paig->addFanout(fanin1);
      



    }
  }




}
void
CirMgr::readsymbol()
{
  if(newfile.is_open())
  {
    string tp;
    getline(newfile, tp);
    if(tp[0] == 'c') return;
    while( (tp[0] == 'i') || (tp[0] == 'o'))
    {
      // cout << "here2?\n";
      if(tp[0] == 'i')
      {
      // cout << "here2?\n";
        istringstream iss(tp);
        string p1, p2;
        iss >> p1 >> p2;
        p1 = p1.substr(1,p1.size()-1);
        int num = 0;
        stringstream geek(p1);
        geek >> num;
        _piList[num]->setsymbol(p2);
        // cout << _piList[num]->getsymbol() << endl;

      }

      if(tp[0] == 'o')
      {
        istringstream iss(tp);
        string p1, p2;
        iss >> p1 >> p2;
        p1 = p1.substr(1,p1.size()-1);
        int num = 0;
        stringstream geek(p1);
        geek >> num;
        _poList[num]->setsymbol(p2);
        // cout << _poList[num]->getsymbol() << endl;
        
      }


      getline(newfile, tp);
    }
  }
}



void
CirMgr::PoConnection()
{
  for(int i = 0; i < (int)_gateList.size(); i++)
  {
    // cout << "find for me" << endl;
  //conect output
    if(_gateList[i]->getTypeStr() == "Po")
    {
      if((_gateList[i]->getf0() == 0))
      {
          _gateList[i]->setf0(reinterpret_cast<CirGate*>(const0));
          continue;
      }
      int j;
      for(j = 0; j < (int)_gateList.size(); j++)
      {
        if(_gateList[i]->getf0() == _gateList[j]->getID())
        {
          _gateList[i]->setf0(_gateList[j]);
          break;
        }
      }

      if(j == (int)_gateList.size())
      {
        CirGate *nullgate = new CirPiGate(_gateList[i]->getf0(),-1);
        nullgate->gateexist = 0;
        _gateList[i]->setf0(nullgate);
        _nullList.push_back(nullgate);
      }

    }

  }

}

void
CirMgr::AigConnection()
{
  for(int i = 0; i < (int)_paigList.size(); i++)
  {
      if(_paigList[i]->getf0() == 0)
      {
        _paigList[i]->setf0(reinterpret_cast<CirGate*>(const0));
      }
      else
      {
        int j;
        //set f0;
        for(j = 0; j < (int)_gateList.size(); j++)
        {
          /*
          if((_gateList[j]->getf0() == 0))
          {
            _gateList[i]->setf0(reinterpret_cast<CirGate*>(const0));
            cout << "success\n";
            break;
          }*/

          if(_paigList[i]->getf0() == _gateList[j]->getID())
          {
            _paigList[i]->setf0(_gateList[j]);
            break;
          }
        }

        if(j == (int)_gateList.size())
        {
           // cout << _paigList[i]->getf0() << " is null" << endl; 
          // _paigList[i]->setf0(NULL);
           CirGate *nullgate = new CirPiGate(_paigList[i]->getf0(),-1);
           nullgate->gateexist = 0;
           _paigList[i]->setf0(nullgate);
          _nullList.push_back(nullgate);
        }

      }
      //set f1;
      if(_paigList[i]->getf1() == 0)
      {
        _paigList[i]->setf1(reinterpret_cast<CirGate*>(const0));
      }
      else
      {
        int j;
        for(j = 0; j < (int)_gateList.size(); j++)
        {
         /* 
          if((_gateList[j]->getf1() == 0))
          {
            _gateList[i]->setf1(reinterpret_cast<CirGate*>(const0));
            break;
          }*/
          if(_paigList[i]->getf1() == _gateList[j]->getID())
          {
            _paigList[i]->setf1(_gateList[j]);
            break;
          }
        }

        if(j == (int)_gateList.size())
        {
          _paigList[i]->setf1(NULL);
           CirGate *nullgate = new CirPiGate(_paigList[i]->getf1(),-1);
           nullgate->gateexist = 0;
           _paigList[i]->setf1(nullgate);
          _nullList.push_back(nullgate);
      }

      }

    }

}

void 
CirMgr::DFSPo()
{
  for(int i = 0; i < (int)_poList.size(); i++)
  {
    if(_poList[i]->getf0add() == reinterpret_cast<CirGate*>(const0))
    {
      _dfsList.push_back(_poList[i]->getf0add());
    }

    if(_poList[i]->getf0add()->gateexist)
    {
      DFSAig(_poList[i]->getf0add());
      // cout << "finish: " << _poList[i] << endl; 
      _dfsList.push_back(_poList[i]);
    }
    else
      _dfsList.push_back(_poList[i]);
  }
/*
  //reset dfs
  *(reinterpret_cast<int*>(const0)) = 0;
  for(size_t i = 0; i < _dfsList.size(); i++)
  {
    if(_dfsList[i] != (reinterpret_cast<CirGate*>(const0)))
    {
     _dfsList[i]->isvisited = 0; 
    }
  }*/
}

int count = 0;
void 
CirMgr::DFSAig(CirGate* node)
{
  if(node == reinterpret_cast<CirGate*>(const0))
  { 
    // const0 = reinterpret_cast<int*>(const0);
    // cout << "original " << *(reinterpret_cast<int*>(const0)) << endl;
    if(*(reinterpret_cast<int*>(const0)) == 0)
    {
      *(reinterpret_cast<int*>(const0)) = 1;
      _dfsList.push_back(reinterpret_cast<CirGate*>(const0));
    }
    return;
  }
  // cout << node->getID() << endl;
  // cout << (node->getTypeStr() == "Pi") << endl;
  // if(count == 10)
    // cin >> count;
  // count ++;
  while(node->gateexist/* && node->getTypeStr() != "Pi" */&& (!node->isvisited))
  {
    // cout << "here\n";
    // cout << node->getTypeStr() << " " << node->getID() << endl;
    if(node->getTypeStr() != "Pi")
    {
    if(!node->getf0add()->isvisited)
      DFSAig(node->getf0add());
    if(!node->getf1add()->isvisited)
      DFSAig(node->getf1add());
    }
    node->isvisited = 1;
   _dfsList.push_back(node);
  }
  // cout << node->getID() << " node is visited?\n";
  // node->isvisited = 1;

  // _dfsList.push_back(node);

}

CirGate*
CirMgr::findgate(unsigned s) const
{
  //find null gate
  if(s == 0) 
  {
      CirGate* gate0 = new CirPiGate(-100,-100);
      return gate0;
  }
  for(size_t i = 0; i < _gateList.size();i++)
  {
    if(_gateList[i] == reinterpret_cast<CirGate*>(const0))
    {
      if(s == 0) 
      {
        return _gateList[i];
      }

    }
    else if(_gateList[i]->getID() == (int)s) return _gateList[i];
  }

  return 0;
      
}


/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
  cout << endl;
  cout << "Circuit Statistics\n" << "==================\n"
  << "  "  << left << setw(5) <<"PI" << right <<setw(9) << I 
  << "\n  "<< left << setw(5) <<"PO" << right <<setw(9) << O 
  << "\n  "<< left << setw(5) <<"AIG" << right <<setw(9) << A 
  << endl <<  "------------------" 
  << "\n  "<< left << setw(5) <<"Total" << right <<setw(9) << I+O+A << endl;
/*
  for(size_t i = 0; i < _paigList.size(); i++)
  {
    // _poList[i]->printf0() ;
    // cout << endl;
    if(_paigList[i]->getf0() == 0)
    {
      cout << "0 0 0\n";
      cout << _paigList[i]->getf0add() << endl;
      cout << _paigList[i]->getf0inv() << endl;
      continue;
    }
    if(_paigList[i]->getf1() == 0)
    {
      cout << "0 0 0\n";
      continue;
    }
    _paigList[i]->printf0() ;
    cout << " f0inv?: " <<  _paigList[i]->getf0inv() 
    << ", fain0 exist?" << _paigList[i]->getf0add()->gateexist << endl;
    
    _paigList[i]->printf1() ;
    cout << " f1inv?: " <<  _paigList[i]->getf1inv()
    << ", fain1 exist?" << _paigList[i]->getf1add()->gateexist << endl;
    // cout << _paigList[i]->getID() << endl;
    cout << endl;
  }*/
/*
  for(int i = 0; i < _poList.size(); i++)
  {
    _poList[i]->printf0() ;
    cout << endl;
    cout << " finv?: " <<  _poList[i]->getf0inv() 
    << ", fain exist?: " << _poList[i]->getf0add()->gateexist << endl;
  }*/
  /*
  for(size_t i = 0; i < _dfsList.size();i++)
  {
    if(_dfsList[i] == reinterpret_cast<CirGate*>(const0))
      cout << *(reinterpret_cast<int*>(_dfsList[i])) << " i am zero\n";
    else
      cout << _dfsList[i]->getID() << " is visited " << _dfsList[i]->isvisited <<endl;
  }*/
}

void
CirMgr::printNetlist() const
{
  cout << endl;
  for(size_t i = 0; i < _dfsList.size(); i++)
  {

    cout << "[" << i << "] ";
    if(_dfsList[i] == reinterpret_cast<CirGate*>(const0)) cout << "CONST0" << endl;
    else if(_dfsList[i]->getTypeStr() == "Pi")
    { 
      cout << "PI  " << _dfsList[i]->getID();
      if(_dfsList[i]->getsymbol() != "") cout << " (" << _dfsList[i]->getsymbol() << ")";
      cout << endl;
    }
    else if(_dfsList[i]->getTypeStr() == "Aig")
    {
      cout << "AIG  " << _dfsList[i]->getID() << "  ";
      //handle f0
      if(_dfsList[i]->getf0add() == reinterpret_cast<CirGate*>(const0))
      {
        if(_dfsList[i]->getf0inv()) cout << "!0  ";
        else cout << "0  ";
      }
      else
      {
        if(!_dfsList[i]->getf0add()->gateexist) cout << "*";
        if(_dfsList[i]->getf0inv()) cout << "!";
        cout << _dfsList[i]->getf0add()->getID() << "  ";

      }

      //handle f1
      if(_dfsList[i]->getf1add() == reinterpret_cast<CirGate*>(const0))
      {
        if(_dfsList[i]->getf1inv())
          cout << "!0" << endl;
        else cout << "0" << endl;
      }
      else
      {
        if(!_dfsList[i]->getf1add()->gateexist) cout << "*";
        if(_dfsList[i]->getf1inv()) cout << "!";
        cout << _dfsList[i]->getf1add()->getID() << endl;

      }
    }
    else if(_dfsList[i]->getTypeStr() == "Po")
    {
      cout << "PO  " << _dfsList[i]->getID() << "  ";
      if(_dfsList[i]->getf0add() == reinterpret_cast<CirGate*>(const0))
      {
        if(_dfsList[i]->getf0inv()) cout << "!0" << endl;
        else cout << "0" << endl;
      }
      else
      {
        if(!_dfsList[i]->getf0add()->gateexist) cout << "*";
        if(_dfsList[i]->getf0inv()) cout << "!";
        cout << _dfsList[i]->getf0add()->getID();
        if(_dfsList[i]->getsymbol() != "") cout<< " (" << _dfsList[i]->getsymbol() << ")"; 
        cout << endl;

      }  

    }

  }

 /* 
  for(size_t i = 0; i < _dfsList.size();i++)
  {
    if(_dfsList[i] == reinterpret_cast<CirGate*>(const0))
      cout << *(reinterpret_cast<int*>(_dfsList[i])) << " i am zero\n";
    else
      cout << _dfsList[i]->getID() << " is visited " << _dfsList[i]->isvisited <<endl;
  }*/
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit: ";
   for(size_t i = 0; i < _piList.size(); i++)
   {
    cout << _piList[i]->getID() << " ";
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit: ";
   for(size_t i = 0; i < _poList.size(); i++)
   {
    cout << _poList[i]->getID() << " ";
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
  // floating fanin
  vector<int> floating_fanin;
  //floating output
  for(size_t i = 0; i < _poList.size(); i++)
  {
      // cout << _poList[i]->getf0add()->gateexist << " ";
    if(_poList[i]->getf0add() != reinterpret_cast<CirGate*>(const0))
    {
      if(!_poList[i]->getf0add()->gateexist)
        floating_fanin.push_back(_poList[i]->getID());
     } // cout << _poList[i]->getID() << " ";
  }
  //floatinf aigGate
  for(size_t i = 0; i < _paigList.size(); i++)
  {
    if(_paigList[i]->getf0add() != reinterpret_cast<CirGate*>(const0))
    {
    if(_paigList[i]->getf1add() != reinterpret_cast<CirGate*>(const0))
  {
    if((!_paigList[i]->getf0add()->gateexist) || (!_paigList[i]->getf1add()->gateexist))
      floating_fanin.push_back(_paigList[i]->getID());
   }   // cout << _paigList[i]->getID() << " ";
    }
  }
  sort(floating_fanin.begin(),floating_fanin.end());
  if(floating_fanin.size()) 
  {
    cout << "Gates with floating fanin(s) : ";
    for(size_t i = 0; i < floating_fanin.size();i++) cout << floating_fanin[i] << " ";
    cout << endl;
  }



  //Undefined Gate
  vector<int> undefined_tmp;
  for(size_t i = 0; i < _gateList.size(); i++)
  {
    if((_gateList[i]->getTypeStr()!="Po") && (_gateList[i]->isvisited == 0))
    {
      undefined_tmp.push_back(_gateList[i]->getID());
    }
  }
  sort(undefined_tmp.begin(),undefined_tmp.end());
  if(undefined_tmp.size()) 
  {
    cout <<"Gates defined but not used : ";
    for(size_t i = 0; i < undefined_tmp.size();i++) cout << undefined_tmp[i] << " ";
    cout << endl;
  }
}

void
CirMgr::writeAag(ostream& outfile) const
{
  vector<CirGate*> newAig;
  for(size_t i = 0; i < _dfsList.size(); i++)
  {
    if(_dfsList[i] != reinterpret_cast<CirGate*>(const0))
    {
      if((_dfsList[i]->getTypeStr() != "Pi") && (_dfsList[i]->getTypeStr() != "Po"))
        newAig.push_back(_dfsList[i]);
    }
  }
  outfile << "aag " << M << " " <<I << " " << L << " "<< O << " "<< newAig.size() << endl;
  for(size_t i = 0; i < _piList.size(); i++) outfile << _piList[i]->getID()*2 << endl;
  for(size_t i = 0; i < _poList.size(); i++) 
  {
      if(_poList[i]->getf0inv())    
        outfile << _poList[i]->getf0()*2+1 << endl;
      else outfile << _poList[i]->getf0()*2 << endl;
  }
  int f0id = 0;
  int f1id = 0;
  for(size_t i = 0; i < newAig.size(); i++) 
  {
    outfile <<newAig[i]->getID()*2 << " " ;

    // f0
    if(newAig[i]->getf0add() == reinterpret_cast<CirGate*>(const0))
    {
      if(newAig[i]->getf0inv() == 0)
        f0id = 0;
      else f0id = 1;
    }
    else 
    {
      if(newAig[i]->getf0inv() == 0)
        f0id = newAig[i]->getf0add()->getID()*2;
      else
        f0id = newAig[i]->getf0add()->getID()*2+1;

    }
    //f1
    if(newAig[i]->getf1add() == reinterpret_cast<CirGate*>(const0))
    {
      if(newAig[i]->getf1inv() == 0)
        f1id = 0;
      else f1id = 1;
    }
    else 
    {
      if(newAig[i]->getf1inv() == 0)
        f1id = newAig[i]->getf1add()->getID()*2;
      else
        f1id = newAig[i]->getf1add()->getID()*2+1;
    }

    outfile<< f0id << " " << f1id << endl;
  }
  //ptint symbol
  for(size_t i = 0; i < _piList.size(); i++)
  {
    if(_piList[i]->getsymbol()!="")
      outfile << "i" << i << " " << _piList[i]->getsymbol() << endl;
  }
  for(size_t i = 0; i < _poList.size(); i++)
  {
    if(_poList[i]->getsymbol()!="")
      outfile << "o" << i << " " << _poList[i]->getsymbol() << endl;
  }
  outfile << "c" << endl << "OMG!!! The code is so long!!! Jui-Che (Chris) Wu" << endl;


}
