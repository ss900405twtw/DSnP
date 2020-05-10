/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
  // for(int i =0; i < 8; i++)
  // {
  //   int ran = rnGen(16);
  //   cout << "ran number is: "<< ran << endl;
  // }
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO

  // Use try-catch to catch the bad_alloc exception
  string opt1, opt2, opt3;
  int end, end2, int1, int2;
  end = myStrGetTok(option, opt1);
  end2 = myStrGetTok(option, opt2, end);
  //new object
  if(opt2.size()==0) 
    {
      myStr2Int(opt1, int1);
      mtest.newObjs(int1);
    }
      // if(!myStrNCmp("-Array",opt1, 2))
  //new array
  else
      if(!myStr2Int(opt1, int1)) // mtn (-a 2) 3
      {
        if( myStrNCmp("-Array", opt1, 2)) errorOption(CMD_OPT_ILLEGAL, opt1);
        myStrGetTok(option, opt3, end2);
        myStr2Int(opt2, int1); myStr2Int(opt3, int2);
        try
        {
          mtest.newArrs(int2, int1);
        }
        catch(const bad_alloc &err)
        {
          return CMD_EXEC_ERROR;
        }
        catch(...)
        {
          errorOption(CMD_OPT_ILLEGAL, to_string(int1));
        }


      }
      else // mtn 3 (-a 2)
      {
        myStrGetTok(option, opt3, end2);
        myStr2Int(opt3, int2);
        try
        {
          mtest.newArrs(int1, int2);
        }
        catch(const bad_alloc &err)
        {
          return CMD_EXEC_ERROR;
        }
        catch(...)
        {
          errorOption(CMD_OPT_ILLEGAL, to_string(int2));
        }

      }
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
  //normal mtd -i 20 -a
  string opt1, opt2, opt3;
  int end, end2, int1;
  end = myStrGetTok(option, opt1);
  end2 = myStrGetTok(option, opt2, end);
  if(!opt1.size() | !opt2.size()) 
    {
      errorOption(CMD_OPT_MISSING, "");
      return CMD_EXEC_ERROR;
    }
  // cout <<"rea?\n"
  // mtd -a -i 20
  if(!myStrNCmp("-Array", opt1, 2))
  {
    myStrGetTok(option, opt3, end2);
    string tmp = opt1;
    opt1 = opt2;
    opt2 = opt3;
  } 

  //by ineex

  if(!myStrNCmp("-Index", opt1, 2)) 
  {
    if(!myStr2Int(opt2, int1)) errorOption(CMD_OPT_ILLEGAL, opt2);
    myStrGetTok(option, opt3, end2);
    //delete array
    if(opt3.size())
    {

      if(int1 >= (int)mtest.getArrListSize())
      {
        cerr << "Size of array list (" << mtest.getArrListSize() 
        << ") is <= " << to_string(int1) <<"!!\n";
        return errorOption(CMD_OPT_ILLEGAL, to_string(int1));
      }
      mtest.deleteArr(int1);

    }
    //delete object
    else
    {

      if(int1 >= (int)mtest.getObjListSize())
      {
        cerr << "Size of object list (" << mtest.getObjListSize() 
        << ") is <= " << to_string(int1) <<"!!\n";
        return errorOption(CMD_OPT_ILLEGAL, to_string(int1));
      }
      mtest.deleteObj(int1);
    }



  }
  //by randon
  else if(!myStrNCmp("-Random", opt1, 2))
  {
    int gen;
    if(!myStr2Int(opt2, int1)) errorOption(CMD_OPT_ILLEGAL, opt2);
    myStrGetTok(option, opt3, end2);
    //delete array
    if(opt3.size())
    {
        if(mtest.getArrListSize() == 0) {
            cerr << "Size of array list is 0!!\n";
            return errorOption(CMD_OPT_ILLEGAL, opt1);     
          }
        for(int i =0; i < int1; i++)
        {
          gen = rnGen(mtest.getArrListSize());
          // if(mtest.getArrListSize() == 0) {
          //   cerr << "Size of array list is 0!!\n";
          //   return errorOption(CMD_OPT_ILLEGAL, opt1);     
          // }
          mtest.deleteArr(gen);
          // cout << "ran number is: "<< ran << endl;
        }

    }
    //delete object
    else
    {
      if(mtest.getObjListSize() == 0) {
            cerr << "Size of object list is 0!!\n";
            return errorOption(CMD_OPT_ILLEGAL, opt1);     
          }
      for(int i =0; i < int1; i++)
      {
        gen = rnGen(mtest.getObjListSize());
        // if(mtest.getObjListSize() == 0) {
        //     cerr << "Size of object list is 0!!\n";
        //     return errorOption(CMD_OPT_ILLEGAL, opt1);     
        //   }
        mtest.deleteObj(gen);
      }
    }


  } 
  else errorOption(CMD_OPT_ILLEGAL, opt1);


  return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


