/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef BST_H
#define BST_H

#include <cassert>

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//

template<class T> class BSTree;

template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode(const T& _data, BSTreeNode *_parent, BSTreeNode *_left = NULL, BSTreeNode* _right = NULL)
      : data(_data), parent(_parent), left(_left), right(_right)
   {
   }
   ~BSTreeNode()
   {
   }

   // order are to fit parameters of constructors
   T data;
   BSTreeNode *parent;
   BSTreeNode *left;
   BSTreeNode *right;
};


template <class T>
class BSTree
{
public:
   // TODO: design your own class!!
   class iterator
   {
   public:
      iterator(BSTreeNode<T>* data, BSTreeNode<T>* hidden = NULL): node(data), hiddenNode(hidden)
      {
      }
      ~iterator()
      {
      }

      const T& operator*() const
      {
         return this->node->data;
      }
      T operator*()
      {
         return this->node->data;
      }
      const iterator& operator++() // prefix
      {
         return *this;
      }
      iterator operator++(int)
      {
         iterator temp = *this;
         return temp;
      }
      const iterator& operator--() // prefix
      {
         return *this;
      }
      iterator operator--(int)
      {
         iterator temp = *this;
         return temp;
      }
      iterator& operator=(const iterator& i)
      {
         return *this;
      }
      bool operator!=(const iterator& i) const
      {
         return false;
      }
      bool operator==(const iterator& i) const
      {
         return false;
      }
   private:
      BSTreeNode<T>* node;
      BSTreeNode<T>* hiddenNode; // only for end()--

      friend class BSTree<T>;
   };
   void print() const { cout << "hi\n";}

   iterator begin() const { return 0; }
   iterator end() const { return 0; }
   bool empty() const { return false; }
   size_t size() const {  return 0; }

   void push_back(const T& x) { }
   void insert(const T& x) { }
   void pop_front() { }
   void pop_back() { }

   // return false if nothing to erase
   bool erase(iterator pos) { return false; }
   bool erase(const T& x) { return false; }

   iterator find(const T& x) { return end(); }

   void clear() { }  // delete all nodes except for the dummy node

   void sort() const { }
   
   // data members
   BSTreeNode<T>* root;
   BSTreeNode<T>* minNode; // there are min and max functions
   BSTreeNode<T>* maxNode;
};

#endif // BST_H
