/*****************************************************************************/
/*!
@file   Containers.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/2/2016
@brief  Custom containers used by the engine.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

namespace DCEngine {

  template <typename Type>
  class PointerIterator {

  };

  /**************************************************************************/
  /*!
  @brief A range class that manipulates two given iterators (begin/end)  
  */
  /**************************************************************************/
  //template <typename Type>
  //class Range {
  //protected:
  //  using Iterator = Range::iterator;
  //  Iterator Begin;
  //  Iterator End;

  //public:
  //  Range(const Iterator& begin, const Iterator& end) : Begin(begin), End(end)
  //  bool IsRange() { return ContainerInstance.begin() != ContainerInstance.end(); }
  //  typename const Iterator begin() const { return Begin; }
  //  typename const Iterator begin() const { return End; }
  //  typename Iterator begin() { return Begin; }
  //  typename Iterator end() { return End; }
  //  size_t size() const { return End - Begin; }

  //};

  /**************************************************************************/
  /*!
  @brief A range-based container.
  */
  /**************************************************************************/
  //template <typename Type>
  //class RangeContainer : public std::vector<Type> {
  //protected:
  //  
  //  std::vector<Type>::iterator Begin;
  //  std::vector<Type>::iterator End;
  //  RangeContainer() : Begin(this->begin()), End(this->end()) {}
  //  //using Iterator = std::vector<Type>::iterator;
  //  //typedef std::vector<Type>::iterator Iterator;

  //public:
  //  bool IsRange() { return Begin != End }
  //  typename const RangeContainer::iterator beginRange() const { return Begin; }
  //  typename const RangeContainer::iterator endRange() const { return End; }
  //  typename RangeContainer::iterator beginRange() { return Begin; }
  //  typename RangeContainer::iterator endRange() { return End; }
  //  size_t sizeRange() const { return End - Begin; }

  //private:

  //};

}