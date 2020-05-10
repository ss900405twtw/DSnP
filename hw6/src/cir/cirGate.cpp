/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"
#include <algorithm>

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
	cout << "==================================================" << endl;
	if(this->getID() == -100)
	{ 
		cout << "= CONST(0), line 0 " << endl;
	}
	else
	{
		if(this->getTypeStr() == "Pi")
		{
			cout << "= PI(" << this->getID() << ")";
			if(this->getsymbol() != "") cout <<"\"" << this->getsymbol() << "\"";
			cout << ", line " << this->getLineNo() << endl; 
		 	// cout << this->getID() << " is found\n";
		}
		else if(this->getTypeStr() == "Po")
		{
			cout << "= PI(" << this->getID() << ")";
			if(this->getsymbol() != "") cout <<"\"" << this->getsymbol() << "\"";
			cout << ", line " << this->getLineNo() << endl; 
		 	// cout << this->getID() << " is found\n";
		}
		else
		{
			cout << "= AIG(" << this->getID() << ")";
			cout << ", line " << this->getLineNo() << endl; 

		}
	}
	cout << "==================================================" << endl; 

}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   vector<bool> vis(cirMgr->gatesize()+1, 0);
   dfsFanin(level, 0, vis);
}

void
CirGate::dfsFanin(int depth, int cur, vector<bool>& vis) const
{
	if(!vis[this->getID()])
	{
		string up = this->getTypeStr();
		transform(up.begin(), up.end(), up.begin(), ::toupper);
		cout << up << " " << this->getID() << endl;
	}
	else if (this->getTypeStr() == "Pi" || cur == depth)
	{
		string up = this->getTypeStr();
		transform(up.begin(), up.end(), up.begin(), ::toupper);
		cout << up << " " << this->getID() << endl;
		return;
	}
	else
	{
		string up = this->getTypeStr();
		transform(up.begin(), up.end(), up.begin(), ::toupper);
		cout << up << " " << this->getID() << " (*) " << endl;
		return;
	}
	vis[this->getID()] = 1;

	if(cur == depth) return;

	string ind;
	for(int i = 0; i <= cur ; i++) ind+= "  ";
	for(size_t i = 0; i < Lfanin.size(); i++)
	{
		cout << ind;
		if(Lfanin[i] % 2) cout << "!";
		CirGate* gate = cirMgr->getGate(Lfanin[i] / 2);
		if(gate) gate->dfsFanin(depth, cur+1, vis);
		else cout << "UNDEF " << Lfanin[i] / 2 << endl;
	}



}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   vector<bool> vis(cirMgr->gatesize()+1, 0);
   dfsFanout(level, 0, vis);
}

void
CirGate::dfsFanout(int depth, int cur, vector<bool>& vis) const
{
	if(!vis[this->getID()])
	{
		string up = this->getTypeStr();
		transform(up.begin(), up.end(), up.begin(), ::toupper);
		cout << up << " " << this->getID() << endl;
	}
	else if (this->getTypeStr() == "Pi" || cur == depth)
	{
		string up = this->getTypeStr();
		transform(up.begin(), up.end(), up.begin(), ::toupper);
		cout << up << " " << this->getID() << endl;
		return;
	}
	else
	{
		string up = this->getTypeStr();
		transform(up.begin(), up.end(), up.begin(), ::toupper);
		cout << up << " " << this->getID() << " (*) " << endl;
		return;
	}
	vis[this->getID()] = 1;

	if(cur == depth) return;

	string ind;
	for(int i = 0; i <= cur ; i++) ind+= "  ";
	for(size_t i = 0; i < Lfanout.size(); i++)
	{
		cout << ind;
		if(Lfanout[i] % 2) cout << "!";
		CirGate* gate = cirMgr->getGate(Lfanout[i] / 2);
		if(gate) gate->dfsFanout(depth, cur+1, vis);
		else cout << "UNDEF " << Lfanout[i] / 2 << endl;
	}



}