/*****************************************************************************/
/*!
\file   DaisyVector.h
\author Gabriel Neumann
\par    email: g.neumann\@digipen.edu
\date   3/26/2016
\brief  Wrapping Vector for std::vector so we can use this in Zilch
*/
/******************************************************************************/
#pragma once

template <typename T>
class DaisyVector
{
  std::vector<T> list;
public:
  DaisyVector& operator= (const DaisyVector<T>& ref);
  unsigned size();
  void resize(unsigned n, unsigned val = value_type());
  unsigned capacity() const;
  bool empty() const;
  void reserve(unsigned n);
  void shrink_to_fit();
  T* operator[] (unsigned n);
  T* at(unsigned n);
  T* front();
  T* back();
  T* data();
  void push_back(const T& val);
  void pop_back();
  void insert(unsigned position, const T& val);
  void erase(unsigned position);
  void swap(DaisyVector<T>& copy);
  void clear();
};

template <typename T>
DaisyVector<T>& DaisyVector<T>::operator= (const DaisyVector<T>& ref)
{
  list = ref.list;
}

template <typename T>
unsigned DaisyVector<T>::size()
{
  return list.size();
}

template <typename T>
void DaisyVector<T>::resize(unsigned n, unsigned val)
{
  list.resize(n, val);
}

template <typename T>
unsigned DaisyVector<T>::capacity() const
{
  return list.capacity();
}

template <typename T>
bool DaisyVector<T>::empty() const
{
  return list.empty();
}
template <typename T>
void DaisyVector<T>::reserve(unsigned n)
{
  list.reserve(n);
}
template <typename T>
void DaisyVector<T>::shrink_to_fit()
{
  list.shrink_to_fit();
}
template <typename T>
T* DaisyVector<T>::operator[] (unsigned n)
{
  return list[n]
}
template <typename T>
T* DaisyVector<T>::at(unsigned n)
{
  return list.at(n);
}
template <typename T>
T* DaisyVector<T>::front()
{
  return list.front();
}
template <typename T>
T* DaisyVector<T>::back()
{
  return list.back();
}
template <typename T>
T* DaisyVector<T>::data()
{
  return list.data();
}
template <typename T>
void DaisyVector<T>::push_back(const T& val)
{
  list.push_back(val);
}
template <typename T>
void DaisyVector<T>::pop_back()
{
  list.pop_back();
}
template <typename T>
void DaisyVector<T>::insert(unsigned position, const T& val)
{
  list[position] = val;
}
template <typename T>
void DaisyVector<T>::erase(unsigned position)
{
  list.erase(position);
}
template <typename T>
void DaisyVector<T>::swap(DaisyVector<T>& copy)
{
  list.swap(copy);
}
template <typename T>
void DaisyVector<T>::clear()
{
  list.clear();
}