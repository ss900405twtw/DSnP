/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;
   public:   
      iterator() {};
      
      iterator(vector<Data>* b, size_t numBuckets, int flag) 
      {
         buckets = b;
         num_buckets = numBuckets;
         num_pos = 0;
         num_index = 0;

         if(flag == 1)
         {
         for(size_t i = 0; num_index < num_buckets; num_index++)
         {
            if(buckets[num_index].size())
            {
               node = &buckets[num_index][num_pos];
               // num_index = i;
               break;
            }
         }
         }
         else if(flag == 2)
            num_index = num_buckets;
      }





      ~iterator() {};
      const Data& operator * () const { return *node; }
      Data& operator * () { return *node; }
    
    /*      iterator& operator ++ () {
         if(num_index == num_buckets) return (*this);
         else if(num_pos < buckets[num_index].size() - 1) ++num_pos;
         else {
            while(++num_index < num_buckets) {
               if(buckets[num_index].size()) {
                  break;
               }
            }
            num_pos = 0;
         }
         if(num_index != num_buckets) node = &buckets[num_index][num_pos];
         return (*this);
      }  */

     iterator& operator ++ () {
         if(num_index == num_buckets) return (*this);
         if(num_pos < buckets[num_index].size() - 1) num_pos += 1;
         else {
            while(++num_index < num_buckets) {
               if(buckets[num_index].size()) {
                  break;
               }
            }
            num_pos = 0;
         }
         if(num_index != num_buckets) node = &buckets[num_index][num_pos];
         return (*this);
      }


      iterator operator ++ (int) {
      iterator cur = (*this);
      ++(*this);
      return cur;
      }
      iterator& operator -- () {
         if(num_pos <= 0) 
         {
            size_t n = num_index;
            while(n > 0) {
               n--;
               if(buckets[n].size()) {
                  num_pos = buckets[n].size() - 1;
                  num_index = n;
                  break;
               }
            }
         }
         else {

            num_pos = num_pos-1;

         }
         node = &buckets[num_index][num_pos];
         return (*this);
      }
      iterator operator -- (int) {
         iterator cur = (*this);
         --(*this);
         return cur;
      }

      iterator& operator = (const iterator& i) {
         this->num_index = i.num_index;
         this->node = i.node;
         this->num_buckets = i._num_buckets;
         this->num_pos = i.num_pos;
         this->buckets = i.buckets;
      }
      bool operator == (const iterator& i) const {
         if(i.num_pos == this->num_pos)
         {
            if(i.num_index == this->num_index)
               return true;
            return false;
         }
         else return false;

      }
      bool operator != (const iterator& i) const {
         if(i == (*this)) return false;
         else return true;
      }



   private:
      vector<Data>* buckets;
      Data* node;
      size_t num_index;
      size_t num_pos;
      size_t num_buckets;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const 
   {
      iterator it(_buckets, _numBuckets, 1); 
      return it; 
   }
   // Pass the end
   iterator end() const 
   {
      iterator it(_buckets, _numBuckets, 2); 
      return it; 
   }
   // return true if no valid data
   bool empty() const 
   {
      if(begin() == end()) 
         return true; 
      else
         return false;
   }
   // number of valid data
   size_t size() const 
   {
      size_t count = 0;
      iterator itbegin = begin();
      iterator itend = end();

      for(size_t i = 0; itbegin != itend; i++)
      {
         count ++;
      }
      size_t s = count; 
      return s; 
}


   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const 
   {
      size_t init_pos = bucketNum(d);
      size_t pos_size = _buckets[init_pos].size();
      for(size_t pos = 0; pos < pos_size; pos++ )
      {
         if(d == _buckets[init_pos][pos])
            return true;
      }
      return false; 
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const 
   { 
      size_t init_pos = bucketNum(d);
      size_t pos_size = _buckets[init_pos].size();
      size_t record = 0;
      for(size_t pos = 0; pos < pos_size; pos++ )
      {
         if(d == _buckets[init_pos][pos])
         { 
            record = pos; 
            break;
          }
      }
      if(record != _buckets[init_pos].size())
      {
         d = _buckets[init_pos][record];
         return true;

      }
      else {
      return false; 
      }
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) 
   {

      size_t init_pos = bucketNum(d);
      size_t pos_size = _buckets[init_pos].size();
      size_t record = 0;
      for(size_t pos = 0; pos < pos_size; pos++ )
      {
         if(d == _buckets[init_pos][pos])
         { 
            record = pos; 
            break;
          }
      }
      if(record != _buckets[init_pos].size())
      {
         _buckets[init_pos][record] = d;
         return true;

      }
      else {
         _buckets[init_pos].push_back(d);
         return false;

      }


   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) 
   { 


      size_t init_pos = bucketNum(d);
      size_t pos_size = _buckets[init_pos].size();
      size_t record = 0;
      for(size_t pos = 0; pos < pos_size; pos++ )
      {
         if(d == _buckets[init_pos][pos])
         { 
            record = pos; 
            break;
          }
      }
      if(record != _buckets[init_pos].size())
      {
         return false;

      }
      else {
         _buckets[init_pos].push_back(d);
         return true;

      }








   }

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
  bool remove(const Data& d) 
   { 



      size_t init_pos = bucketNum(d);
      size_t pos_size = _buckets[init_pos].size();
      size_t record = 0;
      for(size_t pos = 0; pos < pos_size; pos++ )
      {
         if(d == _buckets[init_pos][pos])
         { 
            record = pos; 
            break;
          }
      }
      if(record != _buckets[init_pos].size())
      {
         _buckets[init_pos].erase(_buckets[init_pos].begin() + record);
         return true;

      }
      else {
         return false;

      }

   }




private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
