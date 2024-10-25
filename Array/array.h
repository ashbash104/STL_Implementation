/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Ashlee Hart
 * Sources: 
 *    Chat GPT (To assist with an error on lines 89-102)
 *    https://cplusplus.com/reference/array/array/begin/
 ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray; // forward declaration for unit tests

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <typename T, int N>
struct array
{
public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   // Return an iterator referring to the first element in the array
   iterator begin() 
   { 
      return iterator(__elems_);
   }
   // Return an iterator referring to the first element off the end of the array
   iterator end()   
   { 
      return iterator(__elems_ + N); 
   }

   // 
   // Access
   //

   // Subscript
   // Return the i-th element in the array
   T& operator [] (size_t index)       
   { 
      return __elems_[index]; 
   }

   // Return the i-th element in the array
   const T& operator [] (size_t index) const 
   { 
      return __elems_[index];
   }

   // At
   T& at(size_t index)  
   {
      if (index >= N) {
         throw std::out_of_range("Index out of range.");
      }
      return __elems_[index];
   }
   const T& at(size_t index) const 
   {
      if (index >= N) {
         throw std::out_of_range("Index out of range.");
      }
      return __elems_[index];
   }

   // Front and Back
   // Return the first element in the array
   T& front()       
   { 
      return __elems_[0]; 
   }
   // Return the last element in the array
   T& back()        
   { 
      return __elems_[N - 1]; 
   }
   // Return the first element in the array
   const T& front() const 
   { 
      return __elems_[0]; 
   }
   // Return the last element in the array
   const T& back()  const 
   { 
      return __elems_[N -1]; 
   }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // Number of elements in the array
   size_t size()  const 
   { 
      return N;           
   }
   bool empty() const 
   { 
      return N == 0;
   }
   
   //
   // Member Variable
   //

   // statically allocated array of T
   T __elems_[N]; 
};



/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <typename T, int N>
class array <T, N> :: iterator
{
   friend class ::TestArray; // give unit tests access to the privates
public:
   // Constructors, destructors, and assignment operator
   // Initialize the iterator
   iterator() : p(nullptr)
   { 
     
   }
   iterator(T * p)  
   {
      this->p = p;
   }

   // Copy an iterator
   iterator(const iterator & rhs) 
   {
      p = rhs.p;
   }

   // Copy the pointer
   iterator & operator = (const iterator & rhs)
   {
      if (this != &rhs) {
         p = rhs.p; 
      }
      return *this;
   }
   
   // Equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return p != rhs.p; 
   }
   bool operator == (const iterator & rhs) const 
   { 
      return p == rhs.p;
   }
   
   // Dereference operator
   // Fetch the element referred to by p
   T & operator * ()       
   { 
      return *p; 
   }
   const T & operator * () const 
   { 
      return *p; 
   }
   
   // Prefix increment
   iterator & operator ++ ()
   {
      ++p;
      return *this;
   }
   
   // Postfix increment
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++p;
      return temp;
   }
   
private:
   T * p;
};



}; // namespace custom
