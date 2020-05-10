/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"

using namespace std;

int main()
{
   Json json;

   // Read in the csv file. Do NOT change this part of code.
   string jsonFile;
   cout << "Please enter the file name: ";
   cin >> jsonFile;
   if (json.read(jsonFile))
      cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
      exit(-1); // jsonFile does not exist.
   }

   // TODO read and execute commands
   // cout << "Enter command: ";
   while (true) {
      cout << "Enter command: ";
      string cmd;
      cin >> cmd;
      if(cmd == "PRINT") json.print();
      else if (cmd == "SUM") json.sum();
      else if (cmd == "AVE") json.ave();
      else if (cmd == "MAX") json.max();
      else if (cmd == "MIN") json.min();
      else if (cmd.substr(0,3) == "ADD")
      {
        string key;
        int value;
        cin >> key >> value;
        json.add(key,value);
      }
      else if (cmd == "EXIT") exit(-1);
      else
      {
        cerr << "Error: unknown command: " << "\"" << cmd <<"\"\n";

        // cout<<"Error: No element found!!"<<endl;
      }
   }
}
