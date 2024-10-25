/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:
   
   //
   // Construct
   //

   list(const A& a = A()) : alloc(a), numElements(0), pHead(nullptr), pTail(nullptr){ }
   list(list <T, A> & rhs, const A& a = A()): alloc(a), numElements(0), pHead(nullptr), pTail(nullptr)
   {
      // Check if the source list is empty
      if (rhs.pHead == nullptr)
      {
         // If it's empty, we do nothing, as we've initialized numElements to 0 and pHead/pTail to nullptr.
         return;
      }
      // Create a temporary pointer for traversing the source list
      Node* current = rhs.pHead;
       
      // Loop through the source list and copy each node
      while (current != nullptr)
      {
         // Create a new node using the copy constructor of T
         Node* newNode = new Node(current->data);
         
         // If the destination list is empty, set head and tail to the new node
         if (pHead == nullptr)
         {
            pHead = newNode;
            pTail = newNode;
         }
         else
         {
            // Link the new node to the end of the list
            pTail->pNext = newNode;
            newNode->pPrev = pTail;
            pTail = newNode; // Update the tail to the new node
         }

         // Move to the next node in the source list
            current = current->pNext;
      }
         // Set the number of elements in the destination list
         numElements = rhs.numElements;
   }
   list(list <T, A>&& rhs, const A& a = A());
   list(size_t num, const T & t, const A& a = A());
   list(size_t num, const A& a = A());
   list(const std::initializer_list<T>& il, const A& a = A()) : alloc(a), numElements(0), pHead(nullptr), pTail(nullptr)
   {
      for (const T& item : il)
      {
         push_back(item); // Use push_back to handle copying and linking
      }
   }
   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A()) : alloc(a), numElements(0), pHead(nullptr), pTail(nullptr)
   {
      // Iterate through the range from first to last
      for (Iterator it = first; it != last; ++it)
      {
         push_back(*it); // Use push_back to add elements to the list
      }
   }

   ~list()
   {
      clear();
   }
   
   //
   // Assign
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);
   list <T, A> & operator = (list <T, A> && rhs);
   list <T, A> & operator = (const std::initializer_list<T>& il);

   void swap(list<T, A>& rhs)
   {
      // Step 1: Check for self-swap
      if (this == &rhs) return; // No action needed if both lists are the same

      // Step 2: Swap the internal states
      std::swap(this->pHead, rhs.pHead);
      std::swap(this->pTail, rhs.pTail);
      std::swap(this->numElements, rhs.numElements);

      // Step 3: (Optional) If you have any additional state to swap, do it here
   }


   //
   // Iterator
   //
   
   class iterator;
   iterator begin()  { return iterator (pHead);   }
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   
   //
   // Access
   //
   
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return size() == 0; }
   size_t size() const { return numElements;   }

private:
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list<T, A>::Node {
public:
   T data;              // user data of type T
   Node* pNext;       // pointer to next node
   Node* pPrev;       // pointer to previous node

   Node() : pNext(nullptr), pPrev(nullptr) {}
   Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}
   Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}
};




/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list<T, A>::iterator {
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT, typename AA>
   friend class custom::list;

public:
   // Constructors
   iterator() : p(nullptr) {}
   explicit iterator(Node* pRHS) : p(pRHS) {}

   // Copy constructor
   iterator(const iterator& rhs) : p(rhs.p) {}

   // Assignment operator
   iterator& operator=(const iterator& rhs)
   {
      if (this != &rhs)
      {
         p = rhs.p;
      }
      return *this;
   }

   // Equality operators
   bool operator==(const iterator& rhs) const { return p == rhs.p; }
   bool operator!=(const iterator& rhs) const { return p != rhs.p; }

   // Dereference operator
   T& operator*()
   {
      return p->data; // Return the data stored in the current node
   }

   // Postfix increment
   iterator operator++(int)
   {
      iterator temp = *this; // Copy the current iterator
      if (p) p = p->pNext;   // Move to the next node
      return temp;           // Return the original iterator
   }

   // Prefix increment
   iterator& operator++()
   {
      if (p) p = p->pNext;   // Move to the next node
      return *this;          // Return the updated iterator
   }

   // Postfix decrement
   iterator operator--(int)
   {
      iterator temp = *this; // Copy the current iterator
      if (p) p = p->pPrev;   // Move to the previous node
      return temp;           // Return the original iterator
   }

   // Prefix decrement
   iterator& operator--()
   {
      if (p) p = p->pPrev;   // Move to the previous node
      return *this;          // Return the updated iterator
   }

private:
   Node* p; // Pointer to the current node
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> :: list(size_t num, const T& t, const A& a): alloc(a), numElements(0), pHead(nullptr), pTail(nullptr)
{
   for (size_t i = 0; i < num; ++i)
   {
      push_back(t); // Ensure copy constructor is used
   }
}


/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list<T, A>::list(size_t num, const A& a) : alloc(a), numElements(0), pHead(nullptr), pTail(nullptr)
{
   for (size_t i = 0; i < num; ++i)
   {
      push_back(T()); // Calls the default constructor for T (in this case, Spy)
   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
list<T, A>& list<T, A>::operator=(list<T, A>&& rhs)
{
   if (this != &rhs)
   { // Avoid self-assignment
      clear(); // Clear current elements
      swap(rhs);
   }
   return *this;
}



/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list<T, A>& list<T, A>::operator=( list<T, A>& rhs)
{
   if (this != &rhs)
   { // Avoid self-assignment
      // Step 1: Handle empty case
      if (rhs.empty())
      {
         clear(); // Clear the current list if rhs is empty
         return *this;
      }

      // Step 2: Create an iterator for the current list
      auto itLHS = begin();
      auto itRHS = rhs.begin();

      // Step 3: Copy elements from rhs to the current list
      while (itRHS != rhs.end() && itLHS != end())
      {
         *itLHS = *itRHS; // Assign existing nodes
         ++itRHS;
         ++itLHS;
      }

      // Step 4: If there are more elements in rhs, push them back
      while (itRHS != rhs.end())
      {
         push_back(*itRHS); // Use the copy constructor for new nodes
         ++itRHS;
      }

      // Step 5: If there are remaining elements in LHS, erase them
      while (itLHS != end())
      {
         itLHS = erase(itLHS); // Erase nodes
      }
   }
    return *this; // Return the current object for chaining
}



/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list<T, A>& list<T, A>::operator=(const std::initializer_list<T>& il)
{
   // Step 2: Assign to existing nodes
   auto it = begin();
   auto itIl = il.begin();
   
   // Copy elements from the initializer list to existing nodes
   while (it != end() && itIl != il.end())
   {
      *it = *itIl; // Use T's assignment operator
      ++it;
      ++itIl;
   }
   
   // Step 3: If the initializer list is larger, push new nodes
   while (itIl != il.end())
   {
      push_back(*itIl); // Add new elements to the end
      ++itIl;
   }

   // Step 4: If the current list is larger, erase excess nodes
   while (it != end())
   {
      it = erase(it); // Remove excess nodes
   }

   return *this; // Return this for chaining
}







/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
void list <T, A> :: clear()
{
   while (pHead != nullptr)
   {
      pop_front();
   }
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list<T, A>::push_back(const T& data)
{
   Node* newNode = new Node(data); // Copy constructor
   if (pTail)
   {
      pTail->pNext = newNode; // Link new node after tail
      newNode->pPrev = pTail;  // Link tail before new node
      pTail = newNode;         // Update tail to new node
   }
   else
   {
      pHead = pTail = newNode; // New node is both head and tail
   }
   numElements++;
}

template <typename T, typename A>
void list<T, A>::push_back(T&& data)
{
   Node* newNode = new Node(std::move(data)); // Move constructor
   if (pTail)
   {
      pTail->pNext = newNode; // Link new node after tail
      newNode->pPrev = pTail;  // Link tail before new node
      pTail = newNode;         // Update tail to new node
   }
   else
   {
      pHead = pTail = newNode; // New node is both head and tail
   }
   numElements++;
}




/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list<T, A>::push_front(const T& data)
{
   Node* newNode = new Node(data); // Use the copy constructor to create a new node
   
   if (pHead)
   {
      newNode->pNext = pHead; // Link the new node to the current head
      pHead->pPrev = newNode; // Link the current head back to the new node
      pHead = newNode;        // Update head to the new node
   }
   else
   {
      pHead = pTail = newNode; // New node is both head and tail if the list was empty
   }
   
   numElements++; // Increment the number of elements
}

template <typename T, typename A>
void list<T, A>::push_front(T&& data)
{
   Node* newNode = new Node(std::move(data)); // Use the move constructor to create a new node

    if (pHead)
   {
      newNode->pNext = pHead; // Link the new node to the current head
      pHead->pPrev = newNode; // Link the current head back to the new node
      pHead = newNode;        // Update head to the new node
   }
   else
   {
      pHead = pTail = newNode; // New node is both head and tail if the list was empty
   }

   numElements++; // Increment the number of elements
}



/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{
   if (pTail)
   {
      Node* temp = pTail;
      pTail = pTail->pPrev;
      if (pTail) pTail->pNext = nullptr;
      else pHead = nullptr;
      delete temp;
      --numElements;
   }
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{
   if (pHead)
   {
      Node* temp = pHead;
      pHead = pHead->pNext;
      if (pHead) pHead->pPrev = nullptr;
      else pTail = nullptr;
      delete temp;
      --numElements;
   }
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T& list<T, A>::front()
{
   if (empty())
   {
      throw "ERROR: unable to access data from an empty list"; // Use const char* to match the test
   }
   return pHead->data; // Return the data from the head node
}



/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: back()
{
   if (empty())
   {
       throw "ERROR: unable to access data from an empty list"; // Use const char* to match the test
   }
   return pTail->data; // Return the data from the head node
}



/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
   Node* nodeToDelete = it.p; // Node to be deleted
   
   // Ensure the iterator is valid
   if (nodeToDelete == nullptr)
   {
      return end(); // If the iterator is null, return end
   }
   
   // Save the pointer to the next node
   Node* nextNode = nodeToDelete->pNext;
   
   // Adjust the pointers of the neighboring nodes
   if (nodeToDelete->pPrev)
   {
      nodeToDelete->pPrev->pNext = nextNode; // Link previous node to next node
   }
   else
   {
      pHead = nextNode; // If deleting the head, update pHead
   }
   
   if (nextNode)
   {
      nextNode->pPrev = nodeToDelete->pPrev; // Link next node to previous node
   }
   else
   {
      pTail = nodeToDelete->pPrev; // If deleting the tail, update pTail
   }
   
   // Update numElements
   numElements--;
   
   // Delete the node
   delete nodeToDelete;
   
   // Return an iterator to the next node, or end if there was none
   return (nextNode != nullptr) ? iterator(nextNode) : end();
}


/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *               an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
                                                const T & data)
{
   // Step 1: Create a new node
   Node* newNode = new Node(data);      // Construct the node with data

   // Step 2: Handle insertion
   if (it.p == nullptr)
   { // Inserting at the end (or empty list)
      if (empty())
      { // If the list is empty
         pHead = pTail = newNode; // New node becomes both head and tail
         newNode->pNext = nullptr;
         newNode->pPrev = nullptr;
      }
      else
      { // Inserting at the end of a non-empty list
         pTail->pNext = newNode; // Link new node after the tail
         newNode->pPrev = pTail;  // Set new node's previous to current tail
         newNode->pNext = nullptr; // New node's next is nullptr
         pTail = newNode;          // Update tail to new node
      }
   }
   else
   {
      Node* current = it.p; // The node where the iterator points
      Node* prevNode = current->pPrev; // Get the previous node
      
      // Step 3: Link new node in the list
      if (prevNode)
      {
         prevNode->pNext = newNode; // Link previous node to new node
         newNode->pPrev = prevNode;  // Link new node back to previous node
      }
      else
      {
         pHead = newNode; // If inserting before the first element
      }
      newNode->pNext = current; // Link new node to current node
      current->pPrev = newNode; // Link current node back to new node
   }

   // Step 4: Increment element count
   numElements++;
   // Step 5: Return an iterator to the newly inserted node
   return iterator(newNode);
}


/******************************************
 * LIST :: INSERT
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list<T, A>::iterator list<T, A>::insert(iterator it, T&& data)
{
    // Step 1: Allocate memory for the new node
   Node* newNode = new Node(std::move(data)); // Allocate space for one Node


    // Step 3: Insert the new node into the list
    if (it.p == nullptr)
    { // Inserting at the end
        if (empty())
        {
            pHead = pTail = newNode; // New node becomes both head and tail
            newNode->pNext = nullptr;
            newNode->pPrev = nullptr;
        }
        else
        {
            pTail->pNext = newNode; // Link new node after tail
            newNode->pPrev = pTail;  // Link new node to current tail
            newNode->pNext = nullptr; // New node's next is nullptr
            pTail = newNode;          // Update tail
        }
    }
    else
    {
        Node* current = it.p; // The node where the iterator points
        Node* prevNode = current->pPrev; // Get the previous node

        newNode->pNext = current; // Link new node to current
        current->pPrev = newNode; // Link current back to new node

        if (prevNode)
        {
            prevNode->pNext = newNode; // Link previous node to new node
            newNode->pPrev = prevNode;  // Link new node back to previous
        }
        else
        {
            pHead = newNode; // Update head if inserting before the first element
        }
    }

    // Step 4: Increment element count
    numElements++;

    // Step 5: Return an iterator to the newly inserted node
    return iterator(newNode);
}


/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
   lhs.swap(rhs);
}

}; // namespace custom

