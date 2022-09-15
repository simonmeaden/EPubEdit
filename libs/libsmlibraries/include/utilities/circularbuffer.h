/******************************************************************************
   $Id:  $
   $Name: $

   Author: Pete Goodliffe

   ----------------------------------------------------------------------------
   Copyright 2002 Pete Goodliffe All rights reserved.

   ----------------------------------------------------------------------------
   Purpose: STL-style circular buffer

   ----------------------------------------------------------------------------
   History: See source control system log.

 *****************************************************************************/

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <exception>
#include <iterator>
#include <memory>
#include <stdexcept>

#if __cplusplus > 201703L
  #define CPP17
#elif __cplusplus == 201402L
  #define CPP14
#elif __cplusplus < 201103L
  #define CPP11
#else
  #define CPP
#endif

/*! \cond INTERNAL */ // Blocks this class from documentation
/******************************************************************************
   Iterators
 *****************************************************************************/

/**
   Iterator type for the circular_buffer class.

   This one template class provides all variants of forward/reverse
   const/non const iterators through plentiful template magic.

   You don't need to instantiate it directly, use the good public functions
   availble in circular_buffer.
*/
template<typename T, // circular_buffer type
         //(incl const)
         typename T_nonconst,                         // with any consts
         typename elem_type = typename T::value_type> //+ const for const iter
class circular_buffer_iterator
{
public:
  typedef circular_buffer_iterator<T, T_nonconst, elem_type> self_type;

  typedef T cbuf_type;

  typedef std::random_access_iterator_tag iterator_category;
  typedef typename cbuf_type::value_type value_type;
  typedef typename cbuf_type::size_type size_type;
  typedef typename cbuf_type::pointer pointer;
  typedef typename cbuf_type::const_pointer const_pointer;
  typedef typename cbuf_type::reference reference;
  typedef typename cbuf_type::const_reference const_reference;
  typedef typename cbuf_type::difference_type difference_type;

  circular_buffer_iterator(cbuf_type* b, size_type p)
    : buf_(b)
    , pos_(p)
  {}

  // Converting a non-const iterator to a const iterator
  circular_buffer_iterator(
    const circular_buffer_iterator<T_nonconst,
    T_nonconst,
    typename T_nonconst::value_type>& other)
    : buf_(other.buf_)
    , pos_(other.pos_)
  {}
  friend class circular_buffer_iterator<const T, T, const elem_type>;

  // Use compiler generated copy ctor, copy assignment operator and dtor

  elem_type& operator*() {
    return (*buf_)[pos_];
  }
  elem_type* operator->() {
    return &(operator*());
  }

  self_type& operator++() {
    pos_ += 1;
    return *this;
  }
  self_type operator++(int) {
    self_type tmp(*this);
    ++(*this);
    return tmp;
  }

  self_type& operator--() {
    pos_ -= 1;
    return *this;
  }
  self_type operator--(int) {
    self_type tmp(*this);
    --(*this);
    return tmp;
  }

  self_type operator+(difference_type n) const {
    self_type tmp(*this);
    tmp.pos_ += n;
    return tmp;
  }
  self_type& operator+=(difference_type n) {
    pos_ += n;
    return *this;
  }

  self_type operator-(difference_type n) const {
    self_type tmp(*this);
    tmp.pos_ -= n;
    return tmp;
  }
  self_type& operator-=(difference_type n) {
    pos_ -= n;
    return *this;
  }

  difference_type operator-(const self_type& c) const {
    return pos_ - c.pos_;
  }

  bool operator==(const self_type& other) const {
    return pos_ == other.pos_ && buf_ == other.buf_;
  }
  bool operator!=(const self_type& other) const {
    return pos_ != other.pos_ && buf_ == other.buf_;
  }
  bool operator>(const self_type& other) const {
    return pos_ > other.pos_;
  }
  bool operator>=(const self_type& other) const {
    return pos_ >= other.pos_;
  }
  bool operator<(const self_type& other) const {
    return pos_ < other.pos_;
  }
  bool operator<=(const self_type& other) const {
    return pos_ <= other.pos_;
  }

private:
  cbuf_type* buf_;
  size_type pos_;
};

template<typename circular_buffer_iterator_t>
circular_buffer_iterator_t operator+(const typename circular_buffer_iterator_t::difference_type& a,
                                     const circular_buffer_iterator_t& b)
{
  return circular_buffer_iterator_t(a) + b;
}

template<typename circular_buffer_iterator_t>
circular_buffer_iterator_t operator-(const typename circular_buffer_iterator_t::difference_type& a,
                                     const circular_buffer_iterator_t& b)
{
  return circular_buffer_iterator_t(a) - b;
}

/******************************************************************************
   circular_buffer
 *****************************************************************************/

/**
   This class provides a circular buffer in the STL style.

   You can add data to the end using the @ref push_back function, read data
   using @ref front() and remove data using @ref pop_front().

   The class also provides random access through the @ref operator[]()
   function and its random access iterator. Subscripting the array with
   an invalid (out of range) index number leads to undefined results, both
   for reading and writing.

   This class template accepts three template parameters:
     <li> T                            The type of object contained</li>
     <li> always_accept_data_when_full Determines the behaviour of
                                       @ref push_back when the buffer is full.
                                       Set to true new data is always added, the
                                       old "end" data is thrown away.
                                       Set to false, the new data is not added.
                                       No error is returned neither is an
                                       exception raised.</li>
     <li> Alloc                        Allocator type to use (in line with other
                                       STL containers).</li>

   @short   STL style circular buffer
   @author  Pete Goodliffe
   @version 1.00
*/
template<typename T,
         bool always_accept_data_when_full = true,
         typename Alloc = std::allocator<T>>
class circular_buffer
{
public:
  enum
  {
    version_major = 1,
    version_minor = 0
  };

  // Typedefs
  typedef circular_buffer<T, always_accept_data_when_full, Alloc> self_type;

  typedef Alloc allocator_type;
  typedef typename Alloc::value_type value_type;
  //#if defined(CPP11) || defined(CPP14) || defined(CPP17)
  //  typedef T* pointer;
  //  typedef const T* const_pointer;
  //  typedef T& reference;
  //  typedef const T& const_reference;
  //#else
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::const_pointer const_pointer;
  typedef typename Alloc::reference reference;
  typedef typename Alloc::const_reference const_reference;
  //#endif
  typedef typename Alloc::size_type size_type;
  typedef typename Alloc::difference_type difference_type;

  typedef circular_buffer_iterator<self_type, self_type> iterator;
  typedef circular_buffer_iterator<const self_type, self_type, const value_type>
  const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // Lifetime
  enum
  {
    default_capacity = 100
  };

  /*!
     \brief Constructor for circular buffer.

     \param capacity - the total capacity of the buffer.
  */
  explicit circular_buffer(size_type capacity = default_capacity)
    : m_data(m_allocator.allocate(capacity))
    , m_capacity(capacity)
    , m_head(1)
    , m_tail(0)
    , m_size(0)
  {}

  circular_buffer(const circular_buffer& other)
    : m_data(m_allocator.allocate(other.m_capacity))
    , m_capacity(other.m_capacity)
    , m_head(other.m_head)
    , m_tail(other.m_tail)
    , m_size(other.m_size) {
    try {
      assign_into(other.begin(), other.end());

    } catch (...) {
      destroy_all_elements();
      m_allocator.deallocate(m_data, m_capacity);
      throw;
    }
  }

  template<class InputIterator> circular_buffer(InputIterator from, InputIterator to)
    : m_data(m_allocator.allocate(1))
    , m_capacity(1)
    , m_head(1)
    , m_tail(0)
    , m_size(0) {
    circular_buffer tmp;
    tmp.assign_into_reserving(from, to);
    swap(tmp);
  }

  ~circular_buffer() {
    destroy_all_elements();
    m_allocator.deallocate(m_data, m_capacity);
  }

  /*!
     \brief Assign content

    Assigns new contents to the container, replacing its current contents,
    and modifying its size accordingly.

    \param other  -   another container to use as data source.
  */
  circular_buffer& operator=(const self_type& other) {
    circular_buffer tmp(other);
    swap(tmp);
    return *this;
  }

  /*!
     \brief Swap content.

    Exchanges the content of the container by the content of x, which is
    another buffer object of the same type. Sizes may differ.

    After the call to this member function, the elements in this container
    are those which were in x before the call, and the elements of x are those
    which were in this. All iterators, references and pointers remain valid
    for the swapped objects.

    Notice that a non-member function exists with the same name, swap,
    overloading that algorithm with an optimization that behaves like
    this member function.

    \param other  -   another container to use as data source.
  */
  void swap(circular_buffer& other) {
    std::swap(m_data, other.m_data);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_head, other.m_head);
    std::swap(m_tail, other.m_tail);
    std::swap(m_size, other.m_size);
  }

  /*!
     \brief Get allocator.

    Returns a copy of the allocator object associated with the buffer.
  */
  allocator_type get_allocator() const {
    return m_allocator;
  }

  // Iterators
  /*!
     \brief Return iterator to beginning.

    Returns an iterator pointing to the first element in the buffer.

    Notice that, unlike member circular_buffer::front, which returns a reference
    to the first element, this function returns a random access iterator
    pointing to it.

    If the container is empty, the returned iterator value shall not be
    dereferenced.
  */
  iterator begin() {
    return iterator(this, 0);
  }

  /*!
     \brief Return iterator to end.

    Returns an iterator pointing to the first element in the buffer.

    Notice that, unlike member circular_buffer::back, which returns a reference
    to the first element, this function returns a random access iterator
    pointing to it.

    If the container is empty, the returned iterator value shall not be
    dereferenced.
  */
  iterator end() {
    return iterator(this, size());
  }

  /*!
     \brief Return const iterator to beginning.

    Returns an iterator pointing to the first element in the buffer.

    Notice that, unlike member circular_buffer::front, which returns a reference
                                            to the first element, this function
    returns a random access iterator pointing to it.

    If the container is empty, the returned iterator value shall not be
    dereferenced.
  */
  const_iterator begin() const {
    return const_iterator(this, 0);
  }

  /*!
     \brief Return const iterator to end.

    Returns an iterator pointing to the first element in the buffer.

    Notice that, unlike member circular_buffer::back, which returns a reference
                                           to the first element, this function
    returns a random access iterator pointing to it.

    If the container is empty, the returned iterator value shall not be
    dereferenced.
  */
  const_iterator end() const {
    return const_iterator(this, size());
  }

  /*!
     \brief Return reverse iterator to reverse beginning

    Returns a reverse iterator pointing to the last element in the vector (i.e.,
    its reverse beginning).

    Reverse iterators iterate backwards: increasing them moves them
    towards the beginning of the container.

    rbegin points to the element right before the one that would be pointed to
    by member end.

    Notice that unlike member circular_buffer::back, which returns a reference
    to this same element, this function returns a reverse random access
    iterator.

  */
  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  /*!
     \brief Return reverse iterator to reverse end

    Returns a reverse iterator pointing to the theoretical element preceding the
    first element in the buffer (which is considered its reverse end).

    The range between circular_buffer::rbegin and circular_buffer::rend contains
    all the elements of the buffer (in reverse order).
  */
  reverse_iterator rend() {
    return reverse_iterator(begin());
  }

  /*!
     \brief Return reverse iterator to reverse beginning

    Returns a reverse iterator pointing to the last element in the vector (i.e.,
                                                                        its
    reverse beginning).

    Reverse iterators iterate backwards: increasing them moves them
                                          towards the beginning of the
    container.

                                          rbegin points to the element right
    before the one that would be pointed to by member end.

                                          Notice that unlike member
    circular_buffer::back, which returns a reference to this same element, this
    function returns a reverse random access iterator.

  */
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  /*!
     \brief Return reverse iterator to reverse end

    Returns a reverse iterator pointing to the theoretical element preceding the
      first element in the buffer (which is considered its reverse end).

    The range between circular_buffer::rbegin and circular_buffer::rend contains
    all the elements of the buffer (in reverse order).
  */
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  // Size
  /*!
     \brief Return size.

    Returns the number of elements in the buffer.

    This is the number of actual objects held in the buffer, which is not
    necessarily equal to its storage capacity.
  */
  size_type size() const {
    return m_size;
  }

  /*!
     \brief Return size of allocated storage capacity.

    Returns the size of the storage space currently allocated for the buffer,
    expressed in terms of elements.

    This capacity is not necessarily equal to the buffer size. It can be equal
    or greater, with the extra space allowing to accommodate for growth.
    When this capacity is exhausted the earliest members are overwritten.

    The capacity of a circular_buffer can be explicitly altered by calling
    member circular_buffer::reserve.
  */
  size_type capacity() const {
    return m_capacity;
  }

  /*!
     \brief Return the number of member elements that are unused.

    Returns the difference between the used size and the total capacity.
  */
  size_type available() const {
    return (m_capacity - m_size);
  }

  /*!
     \brief Test whether buffer is empty.

    Returns whether the BUFFER is empty (i.e. whether its size is 0).

    This function does not modify the container in any way. To clear
    the content of a vector, see circular_buffer::clear.
  */
  bool empty() const {
    return !m_size;
  }

  /*!
     \brief Return maximum size.

    Returns the maximum number of elements that the buffer can hold.

    This is the maximum potential size the container can reach due to
    known system or library implementation limitations, but the container
    is by no means guaranteed to be able to reach that size: it can still
    fail to allocate storage at any point before that size is reached.
  */
  size_type max_size() const {
    return m_allocator.max_size();
  }

  /*!
     \brief Request a change in capacity.

    Requests that the buffer capacity be at least enough to contain new_size
    elements.

    If new_size is greater than the current buffer capacity, the function causes
    the container to reallocate its storage increasing its capacity to new_size
    (or greater).

    In all other cases, the function call does not cause a reallocation and the
    buffer capacity is not affected.

    This function has no effect on the buffer size and cannot alter its
    elements.

    \param new_size - new capacity of the vector .
  */
  void reserve(size_type new_size) {
    if (capacity() < new_size) {
      circular_buffer tmp(new_size);
      tmp.assign_into(begin(), end());
      swap(tmp);
    }
  }

  // Accessing
  /*!
     \brief Access first element

      Returns a reference to the first element in the vector.

      Unlike member vector::begin, which returns an iterator to this same
      element, this function returns a direct reference.

      Calling this function on an empty container causes undefined behavior.
  */
  reference front() {
    return m_data[m_head];
  }

  /*!
     \brief Access last element

     Returns a reference to the last element in the vector.

     Unlike member vector::begin, which returns an iterator to this same
     element, this function returns a direct reference.

     Calling this function on an empty container causes undefined behavior.
  */
  reference back() {
    return m_data[m_tail];
  }

  /*!
    \brief Access first element

    Returns a reference to the first element in the vector.

    Unlike member vector::begin, which returns an iterator to this same
    element, this function returns a direct reference.

    Calling this function on an empty container causes undefined behavior.
  */
  const_reference front() const {
    return m_data[m_head];
  }

  /*!
     \brief Access last element

    Returns a reference to the last element in the vector.

    Unlike member vector::begin, which returns an iterator to this same
      element, this function returns a direct reference.

      Calling this function on an empty container causes undefined behavior.
  */
  const_reference back() const {
    return m_data[m_tail];
  }

  /*!
     \brief Add element at the end

    Adds a new element at the end of the buffer, after its current last element.
    The content of val is copied (or moved) to the new element.

    This effectively increases the container size by one if and only if
    the size doesn't exceed the capacity.

    \param item - the new item to push onto the buffer.

  */
  void push_back(const value_type& item) {
    size_type next = next_tail();

    if (m_size == m_capacity) {
      if (always_accept_data_when_full) {
        m_data[next] = item;
        increment_head();
      }

    } else {
      m_allocator.construct(m_data + next, item);
    }

    increment_tail();
  }

  /*!
     \brief Delete first element

    Removes the first (earliest) element in the buffer, effectively reducing the
    container size by one.

    This destroys the removed element.
  */
  void pop_front() {
    size_type destroy_pos = m_head;
    increment_head();
    m_allocator.destroy(m_data + destroy_pos);
  }

  /*!
     \brief Clear content

    Removes all elements from the buffer (which are destroyed), leaving the
    container with a size of 0.
  */
  void clear() {
    for (size_type n = 0; n < m_size; ++n) {
      m_allocator.destroy(m_data + index_to_subscript(n));
    }

    m_head = 1;
    m_tail = m_size = 0;
  }

  /*!
     \brief Access element.

    Returns a reference to the element at position n in the buffer container.

    A similar member function, buffer::at, has the same behavior as this
    operator function, except that buffer::at is bound - checked and signals if
    the requested position is out of range by throwing an out_of_range
    exception.

    Portable programs should never call this function with an argument n that is
    out of range, since this causes undefined behavior.

    \param n - the index position to access.
  */
  reference operator[](size_type n) {
    return at_unchecked(n);
  }

  /*!
     \brief Access element.

    Returns a reference to the element at position n in the buffer container.

    A similar member function, buffer::at, has the same behavior as this
    operator function, except that buffer::at is bound - checked and signals if
    the requested position is out of range by throwing an out_of_range
    exception.

    Portable programs should never call this function with an argument n that is
    out of range, since this causes undefined behavior.

    \param n - the index position to access.
  */
  const_reference operator[](size_type n) const {
    return at_unchecked(n);
  }

  /*!
     \brief Access element.

    Returns a reference to the element at position n in the buffer.

    The function automatically checks whether n is within the bounds
    of valid elements in the vector, throwing an out_of_range exception
    if it is not (i.e., if n is greater than, or equal to, its size).
    This is in contrast with member operator[], that does not check against
    bounds.

    \param n - the index position to access.
  */
  reference at(size_type n) {
    return at_checked(n);
  }

  /*!
     \brief Access const element.

    Returns a reference to the element at position n in the buffer.

    The function automatically checks whether n is within the bounds
      of valid elements in the vector, throwing an out_of_range exception
    if it is not (i.e., if n is greater than, or equal to, its size).
    This is in contrast with member operator[], that does not check against
    bounds.

    \param n - the index position to access.
  */
  const_reference at(size_type n) const {
    return at_checked(n);
  }

private:
  reference at_unchecked(size_type index) const {
    return m_data[index_to_subscript(index)];
  }

  reference at_checked(size_type index) const {
    if (index >= m_size) {
      throw std::out_of_range("");
    }

    return at_unchecked(index);
  }

  // Rounds an unbounded to an index into array_
  size_type normalise(size_type n) const {
    return n % m_capacity;
  }

  // Converts external index to an array subscript
  size_type index_to_subscript(size_type index) const {
    return normalise(index + m_head);
  }

  void increment_tail() {
    ++m_size;
    m_tail = next_tail();
  }

  size_type next_tail() {
    return (m_tail + 1 == m_capacity) ? 0 : m_tail + 1;
  }

  void increment_head() {
    // precondition: !empty()
    ++m_head;
    --m_size;

    if (m_head == m_capacity) {
      m_head = 0;
    }
  }

  template<typename f_iter>
  void assign_into(f_iter from, f_iter to) {
    if (m_size) {
      clear();
    }

    while (from != to) {
      push_back(*from);
      ++from;
    }
  }

  template<typename f_iter>
  void assign_into_reserving(f_iter from, f_iter to) {
    if (m_size) {
      clear();
    }

    while (from != to) {
      if (m_size == m_capacity) {
        reserve(static_cast<size_type>(m_capacity * 1.5));
      }

      push_back(*from);
      ++from;
    }
  }

  void destroy_all_elements() {
    for (size_type n = 0; n < m_size; ++n) {
      m_allocator.destroy(m_data + index_to_subscript(n));
    }
  }

  allocator_type m_allocator;
  value_type* m_data;
  size_type m_capacity;
  size_type m_head;
  size_type m_tail;
  size_type m_size;
};

template<typename T, bool consume_policy, typename Alloc>
bool operator==(const circular_buffer<T, consume_policy, Alloc>& a,
                const circular_buffer<T, consume_policy, Alloc>& b)
{
  return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

template<typename T, bool consume_policy, typename Alloc>
bool operator!=(const circular_buffer<T, consume_policy, Alloc>& a,
                const circular_buffer<T, consume_policy, Alloc>& b)
{
  return a.size() != b.size() || !std::equal(a.begin(), a.end(), b.begin());
}

template<typename T, bool consume_policy, typename Alloc>
bool operator<(const circular_buffer<T, consume_policy, Alloc>& a,
               const circular_buffer<T, consume_policy, Alloc>& b)
{
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

/*! \endcond INTERNAL */


#endif
