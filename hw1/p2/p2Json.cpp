/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"
#include <fstream>
#include <sstream>

using namespace std;

// Implement member functions of class Row and Table here

bool 
check_obj(const string& s)
{
	int line_len=s.length();
	for(int i=0;i<line_len;i++)
	{
		if (s[i] == ':')
			return true;
	}
	
	return false;
	
}



bool
Json::read(const string& jsonFile)
{
	fstream file;
	string line;
	file.open(jsonFile);
	if (!file.is_open())
		return false;
	while (std::getline(file, line))
	{
		
		if(check_obj(line))
		{
			// find key in string
			int line_len=line.length();
			vector<int> pos;
			for(int i =0;i<line_len;i++)
			{
				if(line[i] == '"')
				{
					pos.push_back(i);
					// cout << i << endl;

				}
			}
			string key = line.substr(pos[0]+1,pos[1]-pos[0]-1);
			string pos_value = line.substr(pos[1],line_len-pos[1]);
			// cout << "pos_value is: " << pos_value << endl; 
			// cout<<"the key is "<< key <<endl;


			// find value in string
			string s;
    		char c;
    		int value;
    		std::stringstream ss(pos_value);
    		ss >> s >> c >> value;
    		// cout<<"the value is "<< value <<endl;
    		JsonElem object(key,value);
    		_obj.push_back(object);

		}
		

  	}
  	// cout<<_obj.size()<<endl;	

   return true; // TODO
}
 
ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "  " <<"\"" << j._key << "\" : " << j._value);
}

void
Json::print()
{
	int vec_len=_obj.size();
	int i=0;
	cout << "{\n";
	if(vec_len != 0)
	{
		for(i=0; i < vec_len-1 ; i++)
		{
			cout << _obj[i] << "," <<endl;
		}
		cout << _obj[i] << endl;
	}
	cout << "}\n"; 
}

int
operator + (int& sum, const JsonElem& i)
{
   return sum + i._value;
}

void
Json::sum()
{
	int vec_len=_obj.size();
	int sum=0;
	if(vec_len == 0)
	{
		cerr << "Error: No element found!!\n";
	}
	else
	{	
		for(int i=0; i < vec_len ; i++)
		{
			sum = sum + _obj[i];
		}
	
	cout << "The summation of the values is: "<< sum <<"."<<endl;
	}
}

void
Json::ave()
{
	int vec_len=_obj.size();
	int sum=0;
	if(vec_len == 0)
	{
		cerr << "Error: No element found!!\n";
	}
	else
	{	for(int i=0; i < vec_len ; i++)
		{
			sum = sum + _obj[i];
		}
	cout << "The average of the values is: "<< fixed << setprecision(1) << (float)sum/vec_len <<"."<<endl;
	cout.unsetf( ios::fixed );
	}
}

void
Json::max()
{
	int vec_len=_obj.size();
	if(vec_len == 0)
	{
		cerr << "Error: No element found!!\n";
	}

	else
	{
		int max = _obj[0]._value;
		int max_pos = 0;		
		for(int i=1; i < vec_len ; i++)
		{
			if(_obj[i]._value > max)
			{
				max = _obj[i]._value;
				max_pos = i;
			}
		}
	cout << "The maximum element is: { \""<< _obj[max_pos]._key <<"\" : " << _obj[max_pos]._value <<" }." <<endl;
	}	
}

void
Json::min()
{
	int vec_len=_obj.size();
	if(vec_len == 0)
	{
		cerr << "Error: No element found!!\n";
	}

	else
	{
		int min = _obj[0]._value;
		int min_pos = 0;	
		for(int i=1; i < vec_len ; i++)
		{
			if(_obj[i]._value < min)
			{
				min = _obj[i]._value;
				min_pos = i;
			}
		}

	cout << "The minimum element is: { \""<< _obj[min_pos]._key <<"\" : " << _obj[min_pos]._value <<" }." <<endl;
	}
}

void
Json::add(string key, int value)
{
	JsonElem object(key,value);
	_obj.push_back(object);
}