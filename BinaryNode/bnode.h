/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Ashlee Hart and Spencer Lamoreaux
 * Source:
 *    ChatGPT for methods on lines 148 & 185.
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   // 
   // Default constructor
   BNode() : pParent(nullptr), pLeft(nullptr), pRight(nullptr), data(T()) {}

   // Copy Constructor
   BNode(const BNode& t) : data(t.data), pParent(nullptr), pLeft(nullptr), pRight(nullptr) {}

   // Move Constructor.
   BNode(BNode&& t) noexcept : data(std::move(t.data)), pParent(nullptr), pLeft(nullptr), pRight(nullptr)
   {
      t.pParent = t.pLeft = t.pRight = nullptr;
   }

   // Value Constructor. Copies provided data
   BNode(const T& t) : data(t), pParent(nullptr), pLeft(nullptr), pRight(nullptr) {}

   // Move Constructor with Data Only. Moves data
   BNode(T&& t) : data(std::move(t)), pParent(nullptr), pLeft(nullptr), pRight(nullptr) {}


   //
   // Data
   //
   BNode <T> * pLeft;
   BNode <T> * pRight;
   BNode <T> * pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T>* p)
{
   if (!p)
   {
      return 0;
   }
   // Use recursion to return 1 for each node found. 
   return 1 + size(p->pLeft) + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pNode && pAdd) 
   {
      // Set left of the parent
      pNode->pLeft = pAdd;    
      // Set parent of the left child
      pAdd->pParent = pNode;      
   }
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pNode && pAdd) 
   {
      // Set right of the parent
      pNode->pRight = pAdd;   
      // Set parent of the right child
      pAdd->pParent = pNode;     
   }
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t) 
{
   if (pNode == nullptr)
      return;

   // Create a new node
   BNode<T>* newNode = new BNode<T>(t);

   // Set up the new node
   newNode->pParent = pNode;
   newNode->pLeft = nullptr;
   newNode->pRight = nullptr;
   pNode->pLeft = newNode;
}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{
   if (pNode == nullptr)
      return;

   // Create a new node
   BNode<T>* newNode = new BNode<T>(std::forward<T>(t));

   // Set the parent of the new node
   newNode->pParent = pNode;
   newNode->pLeft = nullptr;
   newNode->pRight = nullptr;
   pNode->pLeft = newNode;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{

   if (pNode == nullptr) 
      return;

   // Create a new node
   BNode<T>* newNode = new BNode<T>(t); 

   // Set up the new node
   newNode->pParent = pNode;
   newNode->pLeft = nullptr;
   newNode->pRight = nullptr;
   pNode->pRight = newNode;
}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{
   if (pNode == nullptr)
      return;

   // Create a new node
   BNode<T>* newNode = new BNode<T>(std::forward<T>(t));

   // Set the parent of the new node
   newNode->pParent = pNode;
   newNode->pLeft = nullptr;
   newNode->pRight = nullptr;
   pNode->pRight = newNode;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{
   if (!pThis)
      return;
   // Go down the left all the way
   clear(pThis->pLeft);
   // Go down the right all the way
   clear(pThis->pRight);

   // Delete the nodes off the stack. 
   delete pThis;
   // So the pointer isn't still pointing to something.
   pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
   if (pLHS == pRHS)
      return;
   // Swap
   BNode<T>* temp = pLHS;
   pLHS = pRHS;
   pRHS = temp;
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc) 
{
   // If cource is empty
   if (!pSrc)
      return nullptr;
   // If src not empty, create a new node.
   BNode<T>* pDest = new BNode<T>(pSrc->data);
   pDest->pParent = nullptr;
   // Copy the left and right subtrees using recursion
   pDest->pLeft = copy(pSrc->pLeft);
   pDest->pRight = copy(pSrc->pRight);

   // If left or right children exist, update their parent pointers
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
   // Return the new tree
   return pDest;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest.
 * reuse as many nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
   // case 1: source empty
   // If source is empty, clear the destination. 
   if (pSrc == nullptr)
   {
      clear(pDest);
      return;
   }
   // case 2: destination empty
   // If the destination is empty. create a new node. 
   if (pDest == nullptr && pSrc != nullptr)
   {
      pDest = new BNode<T>(pSrc->data);
   }
   // case 3: both have values. 
   // If both the source and dest nodes exist, copy the data. 
   else
   {
      pDest->data = pSrc->data;
   }

   // Recursively assign left and right children
   assign(pDest->pLeft, pSrc->pLeft);
   assign(pDest->pRight, pSrc->pRight);

   // Update parent pointers for children
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
}
