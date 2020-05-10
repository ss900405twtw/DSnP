/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data.front(); }
   void insert(const Data& d) 
   {
    
      int arr_size = _data.size();
      _data.push_back(d);
      while(arr_size > 0)
      {
        int p = (arr_size-1) / 2;
        if(d.getLoad() >= _data[p].getLoad())
          break;
        _data[arr_size] = _data[p];
        arr_size = p;
      }
      _data[arr_size] = d;
  

    }
   void delMin() 
   {

    delData(0);

    }

   void delData(size_t i) { 
      size_t move = i;
      int n = _data.size();
      move++;
      int t; 
      t = 2*move;
      while( t < n ) {
         if((_data[t-1] < _data[t]) == 0)   ++t;
         if((_data[n-1] < _data[t-1]) != 0) { break; }
         _data[move-1] = _data[t-1];
         move = t;
         t = move*2;
      }
      _data[move-1] = _data[n-1];
      _data.pop_back();
   }



private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
