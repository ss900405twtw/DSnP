/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <algorithm>

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
	// CirMgr::printFloatGates: floatingFaninin, notInDFS
	// CirMgr::printSummary: gates
	// CirGate:: a lot...

	//sweep for printsummary
	int flag = 0;
	for(unsigned int i = 0; i <= M+O; i++)
	{
		if(gates[i])
		{
			if(gates[i]->gateType == UNDEF_GATE)
			{
				cout << "Sweeping: UNDEF(" << gates[i]->getID() << ") removed...\n";
				gates[i] = NULL;
			} 
			else if(gates[i]->gateType == AIG_GATE)
			{
				for(size_t j = 0; j < dfsOrder.size(); j++)
				{
					if(dfsOrder[j] == gates[i]->getID())
					{
						// cout <<  gates[i]->getID() << endl;
						// cout << dfsOrder[j] << endl;
						flag = -1;
						break;
					}
				}
				if(flag != -1)
				{
					// for(size_t k = 0; k < gates[i]->fanin.size() ;k++ )
					/*
					for(vector<unsigned int>::iterator it = gates[i]->fanin.begin();it != gates[i]->fanin.end();it++)
					{
						// if(gates[i]->fanin[i]/2)
						if(gates[(*it)/2]->gateType == PI_GATE)
							cout << gates[(*it)/2]->getID() << endl;
					}
					*/
					cout << "Sweeping: AIG(" << gates[i]->getID() << ") removed...\n";
					gates[i] = NULL;
				} 
					flag = 0;
				
			}
		}
	}
	// sweep for printfloat gate
	//Gatedefined but not used
	vector<unsigned int> notInDFS2;
	for(size_t i = 0; i < PI.size(); i++)
	{
		// cout << PI[i]/2 << endl;
		if(!gates[gates[PI[i]/2]->fanout[0]])
			notInDFS2.push_back(PI[i]/2);
			// cout << "hi\n";
		// cout << gates[PI[i]/2]->fanout.size() << endl;
	}
   sort(notInDFS2.begin(), notInDFS2.end());
   // http://stackoverflow.com/questions/1041620/most-efficient-way-to-erase-duplicates-and-sort-a-c-vector
   notInDFS2.erase(unique(notInDFS2.begin(), notInDFS2.end()), notInDFS2.end());
   notInDFS = notInDFS2;

   // Gate with floating fanin(s)
   vector<unsigned int> fl;
   floatingFanin = fl;



	/*
	// sweep for printfloat gate
   vector<unsigned int> notInDFS2;

	for(unsigned int i = 1;i <= M;i++)
   {
      if(gates[i])
      {
         if(gates[i]->gateType != UNDEF_GATE && gates[i]->fanout.empty())
         {
         	cout << "fuck\n";
            notInDFS2.push_back(i);
         }
      }
   }
   sort(notInDFS2.begin(), notInDFS2.end());
   // http://stackoverflow.com/questions/1041620/most-efficient-way-to-erase-duplicates-and-sort-a-c-vector
   notInDFS2.erase(unique(notInDFS2.begin(), notInDFS2.end()), notInDFS2.end());
   notInDFS = notInDFS2;

*/
	// cout << "it is too hard" << endl;

}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...

// chaged functions
// CirMgr::printSummary: gates
// CirMgr::printNetlist: dfsOrder
//CirMgr::printFloatGates: floatingFanin, notInDFS



void
CirMgr::optimize()
{


   for(vector<unsigned int>::const_iterator it = PO.begin();it != PO.end();it++)
   {
   		unsigned record;
   		unsigned record2;
   		if(gates[(*it)])
   		{
   			// if(gates[(*it)/2]->gateType == AIG_GATE)
   				if(gates[(gates[(*it)]->fanin[0])/2]->fanin[0] == 0 || gates[(gates[(*it)]->fanin[0])/2]->fanin[1] == 0)
				{
					if(gates[(gates[(*it)]->fanin[0])/2]->fanin[0] == 0)	
   					{
	   					// gates[(gates[(*it)]->fanin[0])/2] = NULL;
	   					unsigned int tmp_id = gates[(*it)]->fanin[0]/2; 
	   					gates[(*it)]->fanin[0] = gates[(gates[(*it)]->fanin[0])/2]->fanin[0];		
	   					// gates[(gates[(*it)]->fanin[0])/2]->fanout[0] = *it;
	   					CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[*it]);
	   					tmp->id = 0;
	   					gates[tmp_id] = NULL;
	   					record = tmp_id;

   					}
   					else
   					{
	   					unsigned int tmp_id = gates[(*it)]->fanin[0]/2; 
	   					gates[(*it)]->fanin[0] = gates[(gates[(*it)]->fanin[0])/2]->fanin[1];		
	   					// gates[(gates[(*it)]->fanin[0])/2]->fanout[0] = *it;
	   					CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[*it]);
	   					tmp->id = 0;
	   					gates[tmp_id] = NULL;
	   					record = tmp_id;

   					}
   					cout << "Simplifying: 0 merging " << record << "..." << endl;
   				}
   				else if(gates[(gates[(*it)]->fanin[0])/2]->fanin[0] == 1 || gates[(gates[(*it)]->fanin[0])/2]->fanin[1] == 1)
   				{
   					if(gates[(gates[(*it)]->fanin[0])/2]->fanin[0] == 1)	
   					{
   						unsigned int simp = gates[(gates[(*it)]->fanin[0])/2]->fanin[1]; 
	   					unsigned int tmp_id = gates[(*it)]->fanin[0]/2; 
	   					CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[*it]);
	   					tmp->id = gates[(gates[(*it)]->fanin[0])/2]->fanin[1]/2;
	   					gates[(*it)]->fanin[0] = gates[(gates[(*it)]->fanin[0])/2]->fanin[1];		
	   					gates[tmp_id] = NULL;
	   					record = tmp_id;
	   					record2	= simp;
	   					// cout << "hi\n";
   					}
   					else
   					{
   					    unsigned int tmp_id = gates[(*it)]->fanin[0]/2; 
   					    unsigned int simp = gates[(gates[(*it)]->fanin[0])/2]->fanin[0];
	   					CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[*it]);
	   					tmp->id = gates[(gates[(*it)]->fanin[0])/2]->fanin[0]/2;
	   					gates[(*it)]->fanin[0] = gates[(gates[(*it)]->fanin[0])/2]->fanin[0];		
	   					gates[tmp_id] = NULL;
	   					record = tmp_id;
	   					record2	= simp;
   					}
   						cout << "Simplifying: " << record2/2 <<" merging " << record << "..." << endl;

   				}
   				else if(gates[(gates[(*it)]->fanin[0])/2]->fanin[0] == gates[(gates[(*it)]->fanin[0])/2]->fanin[1] == 1)
   				{

   					    unsigned int tmp_id = gates[(*it)]->fanin[0]/2;
   					    unsigned int simp = gates[(gates[(*it)]->fanin[0])/2]->fanin[0]; 
	   					CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[*it]);
	   					cout << "my name is "<< tmp->id<<endl;
	   					tmp->id = gates[(gates[(*it)]->fanin[0])/2]->fanin[1]/2;
	   					gates[(*it)]->fanin[0] = gates[(gates[(*it)]->fanin[0])/2]->fanin[0];		
	   					gates[tmp_id] = NULL;
   						cout << "Simplifying: " << simp/2 <<" merging " << tmp_id << "..." << endl;


   				}
   				else if((gates[(gates[(*it)]->fanin[0])/2]->fanin[0]/2) == (gates[(gates[(*it)]->fanin[0])/2]->fanin[1]/2))
   				{

   					    unsigned int tmp_id = gates[(*it)]->fanin[0]/2; 
	   					CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[*it]);
	   					tmp->id = 0;
	   					gates[(*it)]->fanin[0] = 0;		
	   					gates[tmp_id] = NULL;

   						cout << "Simplifying: 0 merging " << tmp_id << "..." << endl;
	   					
   				}
   				// cout << gates[(gates[(*it)]->fanin[0])/2]->fanin[1] << endl;
   		}
   		// gates[(*it)] = NULL;
   }
   // gates[PI[0]] = NULL;
   for(unsigned int i = 0;i <= M+O;i++)
   {
      if(gates[i])
      {
         gates[i]->dfsOrder = -1;
      }
   }
   unsigned int lastID = 0;
   for(vector<unsigned int>::iterator it = PO.begin();it != PO.end();it++)
   {
      lastID = buildDFSOrder2(gates[*it], lastID);
   }
   this->dfsOrder = this->dfsOrder2;
   this->AIGinDFSOrder = this->AIGinDFSOrder2;
   // for(size_t i = 0; i < dfsOrder.size(); i++)
   // {
   // 	    // CirIOGate* tmp = reinterpret_cast<CirIOGate*>(gates[i]);

   // 		cout << dfsOrder[i] << " ";
   // }

}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
// void
// CirMgr::buildnewDFS()
// {

// }
unsigned int CirMgr::buildDFSOrder2(CirGate* g, unsigned int curID)
{
   for(vector<unsigned int>::iterator it = g->fanin.begin();it != g->fanin.end();it++)
   {
      if(gates[(*it)/2]->dfsOrder == -1 &&        // not visited
         gates[(*it)/2]->gateType != UNDEF_GATE)  // undefined gates are not counted
      {
         curID = buildDFSOrder2(gates[(*it)/2], curID);
      }
   }
   g->dfsOrder = curID;
   unsigned int idOfG = g->getID();
   this->dfsOrder2.push_back(idOfG);
   if(g->gateType == AIG_GATE) // for cirwrite
   {
      AIGinDFSOrder2.push_back(idOfG);
   }
   curID++;
   return curID; // return maximum ID
}