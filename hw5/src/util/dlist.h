/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () 
      {
         // cout << "die here\n";
         _node = _node->_next; 
         // cout << "die here2\n";
         return *(this); 
      }
      iterator operator ++ (int) 
      {
         iterator _tmp(_node);
         _node = _node->_next;   
         return _tmp;
      }
      iterator& operator -- () 
      {
         _node = _node->_prev; 
         return *(this); 
      }
      iterator operator -- (int) 
      {
         // _node = _node->_prev;   
         // return *(this);
         iterator _tmp(_node);
         _node = _node->_prev;   
         return _tmp;

      }

      iterator& operator = (const iterator& i) 
      {
         (*this)._node = i._node;
         return *(this); 
      }

      bool operator != (const iterator& i) const 
      {
         if((*this)._node != i._node)
            return true;
         else return false; 
      }
      bool operator == (const iterator& i) const 
      { 
         if((*this)._node == i._node)
            return true;
         else return false; 
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const 
   {
       return iterator(_head); 
   }
   iterator end() const 
   {
       return iterator(_head->_prev); 
   }
   bool empty() const 
   { 
      if(_head->_prev == _head) return true;
      else return false; 
   }
   size_t size() const 
   {
      int count = 0;

      DList<T>::iterator li(_head);
      while(li._node->_next != _head)
      {
         li++;
         count++;
      }
      return (size_t)count; 
   }

   void push_back(const T& x) 
   {
      // DListNode<T> _tmp(_head);
      if(_head->_prev == _head)
      {
         // cout << "here?\n";
         DListNode<T>* new_node = new DListNode<T>(x,_head,_head);
         // _head = new_node;
         _head->_prev = new_node;
         _head->_next = new_node;
         _head = new_node;
         // DListNode<T>* _dummy = new DListNode<T>(T(),_head,_head);
         // _head->_next = _dummy;
         // _head->_prev = _dummy;
      }
      else
      {
         // cout << "here2?\n";
         DListNode<T>* new_node = new DListNode<T>(x,_head->_prev->_prev, _head->_prev);
         _head->_prev->_prev->_next = new_node;
         _head->_prev->_prev = new_node;
      }


   }
   void pop_front() 
   {
      if(this->empty()) 
      {

         return;
      }
      else if(_head->_prev->_prev == _head)
      {
         this->pop_back();
      }
      else
      {
         DListNode<T>*t = _head->_next;
         t->_prev = _head->_prev;
         delete _head;
         _head = t; 
      }
   }
   void pop_back() 
   {
      if(this->empty()) 
      {
         return;
      }
      else if(_head->_prev->_prev == _head)
      {
         DListNode<T>*t = _head->_next;
         t->_prev = _head->_prev;
         delete _head;
         _head = t;
      }
      else
      {

         DListNode<T>*t = _head->_prev->_prev->_prev;
         t->_next = _head->_prev;
         _head->_prev->_prev = t;
      }

    }

   // return false if nothing to erase
   bool erase(iterator pos) 
   {
      if(this->empty())
       return false;
      else if(pos._node == _head)
      {
         // pos._node->_prev->_next = pos._node->_next;
         // pos._node->_next->_prev = pos._node->_prev;
         // _head = pos._node->_next;
         this->pop_back();
         return true;
      }
      else if(pos._node == _head->_prev)
      {
         this->pop_back();
         return true;
      }
      else
      {
         // iterator tmp(pos);
         pos._node->_prev->_next = pos._node->_next;
         pos._node->_next->_prev = pos._node->_prev;
         // pos._node = 0;
         // delete pos._node;
         return true;
      }
    }
   bool erase(const T& x) 
   {
      if(this->empty())
         return false;
      else
      {
         iterator pos = this->find(x);
         if(pos != end())
         {
            // cout << "clean here\n";
            if(pos._node == _head)
               this->pop_front();
            else this->erase(pos);
            return true;
         }
         else return false;
      } 
    }

   iterator find(const T& x) 
   {
       iterator it(_head);
       while(it != end())
       {
         if(it._node->_data == x) return it;
         it++;
       }
      return end(); 

   }

   void clear() 
   {
      if(_head->_prev == _head) return;
      iterator it(_head);
      while(it != end() )
      {
        iterator tmp(it);
        this->erase(tmp);
        it++;


      }
      this->pop_back();
   }  // delete all nodes except for the dummy node

   void sort() const 
   {
      size_t list_size = this->size();
      if(list_size == 0 || list_size == 1) return;
      iterator itj = this->begin();
      for(;itj != this->end(); ++itj)
      {
         T key = itj._node->_data;
         iterator iti = itj._node->_prev;
         while(iti._node != _head->_prev && iti._node->_data > key )
         {
            swap(iti._node->_next->_data, iti._node->_data);
            iti--;
         }
         iti._node->_next->_data = key;
      }



    }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
