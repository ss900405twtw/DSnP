/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"
#include <sstream>

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
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


ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
    assert(j._obj.empty());
    string line;
    while (getline(is, line))
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


      // find value in string
        string s;
        char c;
        int value;
        stringstream ss(pos_value);
        ss >> s >> c >> value;
        // cout << key << " : " << value << endl;
        DBJsonElem object(key,value);
        j._obj.push_back(object);
        // return is;

      }
     if ((line.find('}') != string::npos) && !(j._obj.empty())) {return is;}

    // count_line ++ ;
    // if(count_line == line_len-1) return is;


    

    }
    return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
  int obj_len = j.size();
  cout << "{\n";
  for(int i = 0; i < obj_len; ++i)
  {
      if(i == obj_len-1) {cout << "  \"" << j[i].key() << "\" : " << j[i].value() <<"\n"; break;}
      cout << "  \"" << j[i].key() << "\" : " << j[i].value() << ",\n";
  } 
  cout << "}";

   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
  _obj.clear();
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
  int vec_len = _obj.size();
  for(int i = 0; i < vec_len; i++)
  {
    if(_obj[i].key() == elm.key()) return false;
  }
  _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
  int vec_len = _obj.size();
  if(vec_len == 0) return NAN;
  else
  {

    float sum = 0;
    for(int i = 0; i < vec_len; ++i)
    {
      sum += _obj[i].value();
    }
    // cout << "The average of the values is: "<< fixed << setprecision(1) << (float)sum/vec_len <<"."<<endl;
    // cout.unsetf( ios::fixed );
    return sum/vec_len;
  }
   return 0.0;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   if(_obj.empty())
   {
      idx = _obj.size();
      return  maxN;
    }
    else
    {
      int vec_len = _obj.size();
      int flag = 0;
      for(int i = 0; i < vec_len; ++i)
      {
        if(_obj[i].value() > maxN)
        {
          maxN = _obj[i].value();
          flag = i;
        }

      }
      idx = flag;
      // cout << "The maxumum value in the json vector is: " << maxN << endl;
      return maxN;
    }
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;
   if(_obj.empty())
   {
      idx = _obj.size();
      return  minN;
    }
    else
    {
      int vec_len = _obj.size();
      int flag = 0;
      for(int i = 0; i < vec_len; ++i)
      {
        if(_obj[i].value() < minN)
        {
          minN = _obj[i].value();
          flag = i;
        }

      }
      idx = flag;
      // cout << "The minimum value in the json vector is: " << minN << endl;
      return minN;
    } 

}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int s = 0;
   if(_obj.empty()) return 0;
   int vec_len = _obj.size();
   for(int i = 0; i < vec_len; ++i)
   {
      s += _obj[i].value();
   }
   // cout << "The summation of the values is: "<< s <<".\n";
   return s;
}
