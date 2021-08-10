/*
  Copyright 2020 Simon Meaden

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  @author: Simon Meaden

*/
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <QList>
#include <QVector>

#include "circularbuffer.h"
#include "sm_widgets_global.h"
#include <list>
#include <stddef.h>
#include <vector>

/*!
  \file circularbuffer.h
  \class CircularBuffer circularbuffer.h
  \brief A template class that implements a circular or ring buffer.
  \since 5.14.0

  CircularBuffer works basically like a queue with items pushed to the head
  of the buffer and pulled from the tail. Unlike a queue however, when the
  buffer is full the items overwrite the oldest data and the CircularBuffer
  is NOT resized.

  The primary constructor, CircularBuffer(int) which initialises a buffer with
  a supplied capacity but a size of 0.  Alternatively you can initialise the
  buffer using either a QList<value_type> or a QVector<value_type>, either
  directly which creates a buffer with a capacity the same size as the supplied
  item, or with a supplied int capacity which will will set the capacity
  accordingly. Please note that if the capcity set is smaller than the supplied
  list/vector then only the last N items will be retained.

  It is also possible to push() a QList or a QVector of items onto the buffer,
  or alternatively use CircularBuffer::iterator which are basically a typedef of
  std::iterator. CircularBuffer has a begin() and end() iterator, also cbegin(),
  cend(), constBegin(), constEnd(), rbegin() and rend() in exactly the same way
  as QVector, which makes sense as the underlying class to CircularBuffer is
  a QVector(). A further option is to push items using the operator<<() which
  allows the pushing of a single item of type T, or alternatively a
  QVector<value_type> or a QList<value_type>.

  The earliest item on the buffer can be accessed and removed using the pull()
  method. Alternatively you can pull more than one item by calling pull(count)
  which pulls either 'count' items or size() items whichever is smaller. The
  toVector(), toList(), toStdVector() and toStdList() methods can be used to
  access the entire contents as a QVector<value_type>, a QList<value_type>, a
  std::vector<value_type> or a std::list<value_type> respectively.

  As in most Qt collection classes, there are alternatives to most of the
  methods: for example push(), append() or operator<<() or front(), first().
  These are mostly supplied to match either historical Qt or STL methods.

  You can call fill() at any time to fill the vector with a particular value.

  This is basically just a wrapper around the stl style circular_buffer calls
  by Peter Goodliffe.
*/

template<typename T,
         bool always_accept_data_when_full = true,
         typename Alloc = std::allocator<T>>
class CircularBuffer
{

public:
  /// \cond DO_NOT_DOCUMENT
  typedef circular_buffer<T> self_type;
  typedef typename circular_buffer<T>::value_type value_type;
  typedef typename circular_buffer<T>::pointer pointer;
  typedef typename circular_buffer<T>::const_pointer const_pointer;
  typedef typename circular_buffer<T>::reference reference;
  typedef typename circular_buffer<T>::const_reference const_reference;
  typedef typename circular_buffer<T>::difference_type difference_type;
  typedef typename circular_buffer<T>::iterator iterator;
  typedef typename circular_buffer<T>::const_iterator const_iterator;
  typedef typename circular_buffer<T>::reverse_iterator reverse_iterator;
  typedef
    typename circular_buffer<T>::const_reverse_iterator const_reverse_iterator;
  /// \endcond

  /*!
    \brief Constructor of the CircularBuffer

    Creates a new circular buffer with a capacity of capacity,
    and fills it with default values of type T.

    \param capacity - an int value specifying how many items may be stored in
     the buffer.
  */
  explicit CircularBuffer(int capacity)
    : m_buffer(new circular_buffer<value_type>(capacity))
  {}

  /*!
    \brief Constructor of the CircularBuffer

    Creates a new circular buffer, using the size and contents of the list,
    unless the optional capacity is used in which this value is used. If the
    capcity is less than the size of the supplied QList, only the last N values
    are stored. Primitive types like int and double, as well as for pointer
    types, the C++ language doesn't specify any initialization; in those cases,
    Qt's containers automatically initialize the value to 0.

    \param items - a QList<value_type> containig the items to push.
    \param capacity - an optional capacity for the buffer.
  */
  CircularBuffer(const QList<value_type>& items, int capacity = -1)
    : m_buffer(new circular_buffer<value_type>(capacity == -1 ? items.size()
                                                              : capacity))
  {
    push(items);
  }

  /*!
    \brief Constructor of the CircularBuffer

    Creates a new circular buffer, using the size and contents of the vector,
    unless the optional capacity is used in which this value is used. If the
    capacity is less than the size of the supplied QVector, only the last N
    values are stored.

    \param items - a QVector<value_type> containing the items to push.
    \param capacity - an optional capacity for the buffer.
  */
  CircularBuffer(const QVector<value_type>& items, int capacity = -1)
    : m_buffer(new circular_buffer<value_type>(capacity == -1 ? items.size()
                                                              : capacity))
  {
    push(items);
  }

  /*!
    \brief Constructs a copy of other.

    \param other - another CircularBuffer.
  */
  CircularBuffer(const CircularBuffer<value_type>& other)
    : m_buffer(other.m_buffer)
  {}

  /*!
     \brief Returns an STL-style iterator pointing to the first item in the
     vector.
  */
  iterator begin() { return m_buffer->begin(); }

  /*!
     \brief This is an overloaded function.
  */
  const_iterator begin() const { return m_buffer->begin(); }

  /*!
     \brief Returns an constant STL-style iterator pointing to the first item in
     the vector.
  */
  const_iterator cbegin() const { return m_buffer->begin(); }

  /*!
     \brief Returns an constant STL-style iterator pointing to the first item in
     the vector.
  */
  const_iterator constBegin() const { return m_buffer->begin(); }

  /*!
     \brief Returns an STL-style iterator pointing to the last item in the
     vector.
  */
  iterator end() { return m_buffer->end(); }
  /*!

     \brief Returns an constant STL-style iterator pointing to the last item in
     the vector.
  */
  const_iterator end() const noexcept { return m_buffer->end(); }

  /*!
     \brief Returns an constant STL-style iterator pointing to the last item in
     the vector.
  */

  const_iterator cend() const noexcept { return m_buffer->end(); }

  /*!
     \brief Returns a const STL-style iterator pointing to the imaginary item
     after the last item in the CircularBuffer.
  */
  const_iterator constEnd() const noexcept { return m_buffer->end(); }

  /*!
     \brief Returns a STL-style reverse iterator pointing to the first item in
     the vector, in reverse order.
  */
  reverse_iterator rbegin() { return m_buffer->rbegin(); }

  /*!
     \brief Returns a STL-style reverse iterator pointing to the last item in
     the vector, in reverse order.
  */
  reverse_iterator rend() { return m_buffer->rend(); }

  /*!
     \brief Returns a const STL-style reverse iterator pointing to the first
     item in the vector, in reverse order.
  */
  const_reverse_iterator rbegin() const { return m_buffer->rbegin(); }

  /*!
     \brief Returns a constant STL-style reverse iterator pointing to the last
     item in the vector, in reverse order.
  */
  const_reverse_iterator rend() const { return m_buffer->rend(); }

  /*!
     \brief Returns a const STL-style reverse iterator pointing to the first
     item in the vector, in reverse order.
  */
  const_reverse_iterator crbegin() const { return m_buffer->rbegin(); }

  /*!
     \brief Returns a const STL-style reverse iterator pointing to the last item
     in the vector, in reverse order.
  */
  const_reverse_iterator crend() const { return m_buffer->rend(); }

  /*!
     \brief Deletes the first element.

     To access the first element use CircularBuffer::front() or
     CircularBuffer::first().
  */
  void pop() { m_buffer->pop_front(); }

  /*!
     \brief Deletes the first element.

    This is an overloaded function. It is the equivalent of pop().
  */
  inline void removeFirst() { pop(); }

  /*!
     \brief Deletes the first element.

    This is an overloaded function. It is the equivalent of pop().
  */
  inline void pop_front() { pop(); }

  /*!
     \brief Deletes the first element.

    This is an overloaded function. It is the equivalent of pop().
  */
  inline void popFront() { pop(); }

  /*!
     \brief Deletes the first element.

    This is an overloaded function. It is the equivalent of pop().
  */
  inline void popFirst() { pop(); }

  /*!
    \brief Pushes a new item at the head position.

    Note: You should check if there is space to enter more data before adding
           more as it will overwrite the oldest data if there is insufficient
          space available.

    \param value - an object of type value_type.
  */
  void push(const value_type& value) { m_buffer->push_back(value); }

  /*!
    \brief Pushes a new item at the head position.

    This is an overloaded function. It is the equivalent of push().

    Note: You should check if there is space to enter more data before adding
           more as it will overwrite the oldest data if there is insufficient
             space available.

    \param value - an object of type value_type.
  */
  inline void push_back(const value_type& value) { push(value); }

  /*!
    \brief Pushes a new item at the head position.

    This is an overloaded function. It is the equivalent of push().

    Note: You should check if there is space to enter more data before adding
             more as it will overwrite the oldest data if there is insufficient
               space available.

    \param value - an object of type value_type.
  */
  inline void append(const value_type& value) { push(value); }

  /*!
     \brief Pushes a list of items to the buffer.

    This will push the items to the buffer but does not alter the size of
    the buffer so if the list is too large onle the last N items will be
      stored where N = the capacity of the buffer.

     \param values - a QList of type value_type.
  */
  void push(const QList<value_type> values)
  {
    for (const_reference value : values) {
      push(value);
    }
  }

  /*!
    \brief Pushes a new item at the head position.

    This is an overloaded function. It is the equivalent of push().


    Note: You should check if there is space to enter more data before adding
             more as it will overwrite the oldest data if there is insufficient
               space available.

    \param values - an object of type value_type.
  */
  inline void append(const QList<value_type> values) { push(values); }

  /*!
     \brief Pushes a vector of items to the buffer.

     This will push the items to the buffer but does not alter the size of
     the buffer so if the vector is too large onle the last N items will be
     stored where N = the capacity of the buffer.

     \param values - a QVector of type value_type.
     \sa push()
  */
  void push(const QVector<value_type>& values)
  {
    for (const value_type value : values) {
      push(value);
    }
  }

  /*!
    \brief Pushes a vector of items to the buffer.

    This is an overloaded function.

    This will push the items to the buffer but does not alter the size of
    the buffer so if the vector is too large onle the last N items will be
    stored where N = the capacity of the buffer.

    \param values - a QVector of type value_type.
    \sa push()
  */
  inline void append(const QVector<value_type>& values) { push(values); }

  /*!
    \brief pulls an item from the front position and returns it.

    The item is removed and the back position is incremented by one position.

    \return - an object of type value_type.
  */
  const value_type get()
  {
    const value_type value = front();
    pop();
    return value;
  }

  /*!
    \brief pulls an item from the front position and returns it.

    This is an overloaded function.

    The item is removed and the back position is incremented by one position.

    \return - an object of type value_type.
    \sa pull()
  */
  inline const value_type takeFirst() { return get(); }

  /*!
    \brief pulls an item from the front position and returns it.

    This is an overloaded function.

    The item is removed and the back position is incremented by one position.

    \return - an object of type value_type.
    \sa pull()
  */
  inline const value_type takeFront() { return get(); }

  /*!
    \brief pulls an count items from the front position.

    The items are removed and the front position is incremented by count
    positions, or size() positions if the buffer size is less than count.

    \return - an object of type value_type.
  */
  const QVector<value_type> get(int count)
  {
    QVector<value_type> vec;

    for (int i = 0; i < (count < size() ? count : size()); i++) {
      vec.append(get());
    }

    return vec;
  }

  /*!
     \brief Assigns value to all, or size, items in the vector.

    If size is -1 (the default), or size is greater than the available items
                                 then all of the available items are
                                 set to 'value', however if size is less than
    the available items only that number of items is set to 'value'.

      Generally you will not need to use this method as default
      values are automatically entered.

     \param value - the value_type value to fill the buffer with.
    \param size - the number of slots to fill.
  */
  void fill(value_type& value, int size = -1)
  {

    int s = (size == -1 ? count() : (size > count() ? count() : size));

    for (int i = 0; i < s; i++) {
      m_buffer[i] = value;
    }
  }

  /*!
     \brief Access the element at index, relative to the tail of the buffer.

    index must be a valid index position in the buffer (i.e., 0 <= i < size()).

     \param index - the index within the buffer.
    \return an object of type value_type.
  */
  reference at(int index)
  {
    Q_ASSERT(index >= 0 && index < size());
    return m_buffer->at(index);
  }

  /*!
    \brief Access the element at index, relative to the tail of the buffer.

    index must be a valid index position in the buffer (i.e., 0 <= i < size()).

    \param index - an int offset value.
    \return an object of type value_type.
  */
  const_reference at(int index) const
  {
    Q_ASSERT(index >= 0 && index < size());
    return m_buffer->at(index);
  }

  /*!
    \brief Access the element at index, relative to the tail of the buffer.

    If the index i is out of bounds, the function returns a default-constructed
    value. If you are certain that i is within bounds, you can use at() instead
    which is slightly faster.

    \param index - the index within the buffer.
    \return an object of type value_type.
  */
  value_type value(int index)
  {
    if (index >= 0 && index < size()) {
      return m_buffer->at(index);

    } else {
      return value_type();
    }
  }

  /*!
    \brief Access the element at index, relative to the tail of the buffer or
    a supplied default value.

    This is an overloaded function.

    If the index i is out of bounds, the function returns a default-constructed
    value. If you are certain that i is within bounds, you can use at() instead
    which is slightly faster.


    \param index - the index within the buffer.
    \param defaultValue - the default item to use to fill the buffer.
    \return an object of type value_type.
  */
  value_type value(int index, const value_type defaultValue)
  {
    if (index >= 0 && index < size()) {
      return m_buffer->at(index);

    } else {
      return defaultValue;
    }
  }

  /*!
     \brief Access the first element.

    This function assumes that the vector isn't empty.

    \return a reference to the first item in the vector.
  */
  reference first() { return m_buffer->front(); }

  /*!
     \brief Access the first element.

     This is an overloaded function.
    This function assumes that the vector isn't empty.

    \return a const reference to the first item in the vector.
  */
  const_reference& first() const { return m_buffer->front(); }

  /*!
     \brief Access the first element.

    This is an overloaded function.

    This function is provided for STL compatibility. It is equivalent to
    first().

    \return a reference to the first item in the vector.
  */
  inline reference front() { return first(); }

  /*!
     \brief Access the first element.

    This is an overloaded function.

    This function is provided for STL compatibility. It is equivalent to
    first().

    \return a const reference to the first item in the vector.
  */
  inline const_reference front() const { return first(); }

  /*!
     \brief Access the first element.

    This is an overloaded function.
      It is equivalent to first().

    \return a const reference to the first item in the vector.
  */
  inline const_reference& constFirst() const { return first(); }

  /*!
     \brief Access the first element.

    This is an overloaded function.
      It is equivalent to first().

    \return a const reference to the first item in the vector.
  */
  inline const_reference& constFront() const { return first(); }

  /*!
     \brief Access the last element.

    This is an overloaded function.

    This function is provided for STL compatibility. It is equivalent to last().

    \return a reference to the last item in the vector.
  */
  inline reference back() { return last(); }

  /*!
     \brief Access the last element.

    This is an overloaded function.

    This function is provided for STL compatibility. It is equivalent to last().

    \return a const reference to the last item in the vector.
  */
  inline const_reference back() const { return last(); }

  /*!
     \brief Access the last element.

    This function is provided for STL compatibility.

    \return a const reference to the last item in the vector.
  */
  reference last() { return m_buffer->back(); }

  /*!
     \brief Access the last element.

    This is an overloaded function.

    This function is provided for STL compatibility.

    \return a const reference to the last item in the vector.
  */
  inline const_reference last() const { return m_buffer->back(); }

  /*!
     \brief Access the last element.

    This is an overloaded function.
    It is equivalent to last().

    \return a const reference to the last item in the vector.
  */
  inline const_reference constLast() const { return back(); }

  /*!
    \brief Access the data as a QVector.

    If you want to remove the data, you can use pullVector().

    \return a QVector of values.
  */
  const QVector<value_type> toVector()
  {
    QVector<value_type> vec(begin(), end());
    return vec;
  }

  /*!
    \brief Access the data as a QVector, emptying the buffer.

    \return a QVector of values.
    \sa toVector()
  */
  const QVector<value_type> pullVector()
  {
    QVector<value_type> vec = toVector();
    clear();
    return vec;
  }

  /*!
     \brief Access the data as a std::vector.

    If you want to remove the data, you can use pullStdVector().

    \return a vector of values.
  */
  const std::vector<value_type> toStdVector()
  {
    return std::vector<value_type>(begin(), end());
  }

  /*!
     \brief Access the data as a std::vector.

    \return a vector of values.
    \sa toStdVector()
  */
  const std::vector<value_type> pullStdVector()
  {
    std::vector<value_type> svec = toStdVector();
    clear();
    return svec;
  }

  /*!
    \brief Access the data as a QList.

    If you want to remove the data, you can use pullList().

    \return a vector of values.
  */
  const QList<value_type> toList() { return QList<value_type>(begin(), end()); }

  /*!
    \brief Access the data as a QList.

    The buffer is empty and the end of this.

    \return a vector of values.
    \sa toList()
  */
  const QList<value_type> pullList()
  {
    QList<value_type> list = toList();
    clear();
    return list;
  }

  /*!
    \brief Access the data as a std::list.

    If you want to remove the data, you can use pullStdList().

    \return a vector of values.
  */
  const std::list<value_type> toStdList()
  {
    return std::list<value_type>(begin(), end());
  }

  /*!
    \brief Access the data as a QList.

    The buffer is empty and the end of this.

    \return a vector of values.
    \sa toStdList()
  */
  const std::list<value_type> pullStdList()
  {
    std::list<value_type> list = toStdList();
    m_buffer->clear();
    return list;
  }

  /*!
    \brief Returns the number of items stored in the buffer.
  */
  int size() const { return m_buffer->size(); }

  /*!
    \brief Returns the number of items stored in the buffer.

    This is an overloaded function, the equivalent of size().
  */
  inline int count() const { return size(); }

  /*!
    \brief Returns the number of items stored in the buffer.

    This is an overloaded function, the equivalent of size().
  */
  inline int length() const { return size(); }

  /*!
     \brief Gets the total capacity of the buffer.

     \return - an int value.
  */
  int capacity() const { return m_buffer->capacity(); }

  /*!
     \brief Gets the total free space still available to the buffer.
  */
  int available() const { return m_buffer->available(); }

  /*!
     \brief Checks if the buffer is empty.
  */
  bool empty() const { return m_buffer->empty(); }

  /*!
     \brief Checks if the buffer is empty.

    This is an overloaded function, the equivalent of empty().
  */
  inline bool isEmpty() const { return empty(); }

  /*!
     \brief Checks if the buffer is full.

     \return true if full, otherwise false.
  */
  bool isFull() const { return (m_buffer->size() == m_buffer->capacity()); }

  /*!
     \brief Removes all the elements from the vector.
  */
  void clear() { m_buffer->clear(); }

  /*!
    \brief Resizes the buffer to a new capacity.

    Resizes the buffer to the supplied size. If the new capacity is larger
    then all of the old will be retained, otherwise only the most recent
    additions will be retained.

    \param capacity - the capacity value.
  */
  void reserve(const int capacity) { m_buffer->reserve(capacity); }

  /*!
    \brief Gets the value at the index, relative to the tail of the buffer.

    A similar operator at() can also be used. The difference is
    that at() is bounds checked and throws an std::out_of_bounds exception
    if index is outside the size of the buffer. In this case this causes
    undefined behaviour.

    \param index - an int offset value.
    \return a reference to the item at index.
  */
  reference operator[](int index) { return reference((*m_buffer)[index]); }

  /*!
    \brief Gets the value at the index, relative to the tail of the buffer.

    A similar operator at() can also be used. The difference is
    that at() is bounds checked and throws an std::out_of_bounds exception
    if index is outside the size of the buffer. In this case this causes
    undefined behaviour.

    \param index - an int offset value.
    \return a const reference to the item at index.
  */
  const_reference operator[](int index) const
  {
    return const_reference((*m_buffer)[index]);
  }

  /*!
     \brief Assigns other to this buffer and returns a reference to this buffer.
  */
  CircularBuffer& operator=(self_type& other)
  {
    m_buffer = other.m_buffer;
    return *this;
  }

  /*!
     \brief Assigns other to this buffer and returns a reference to this buffer.
  */
  CircularBuffer& operator=(const self_type& other)
  {
    m_buffer = other.m_buffer;
    return *this;
  }

  /*!
    \brief Pushes a new item at the head position.

    Note: You should check if there is space to enter more data before adding
    more as it will overwrite the oldest data if there is insufficient space
    available.

    \param value - an constant object of type value_type.
    \sa CircularBuffer::push()
  */
  CircularBuffer<value_type>& operator<<(const value_type value)
  {
    m_buffer->push_back(value);
    return *this;
  }

  /*!
     \brief Pushes a vector of items to the buffer.

     This will push the items to the buffer but does not alter the size of
     the buffer so if the list is too large onle the last N items will be
     stored where N = the capacity of the buffer.

     \param other - a const QVector of type value_type.
     \sa CircularBuffer::push(QVector<value_type>)
  */
  CircularBuffer<value_type>& operator<<(const QVector<value_type>& other)
  {
    push(other);
    return *this;
  }

  /*!
     \brief Pushes a list of items to the buffer.

     This will push the items to the buffer but does not alter the size of
     the buffer so if the list is too large onle the last N items will be
     stored where N = the capacity of the buffer.

     \param other - a const QList of type value_type.
     \sa CircularBuffer::push(QVector<value_type>)
  */
  CircularBuffer<value_type>& operator<<(const QList<value_type>& other)
  {
    push(other);
    return *this;
  }

private:
  circular_buffer<value_type>* m_buffer;

}; // end of CircularBuffer.

#endif // _CIRCULAR_BUFFER_H_
