#include "Delegate.h"

namespace DCEngine {
  Delegate::Delegate(Component * component, DelegateFn function) 
                    : _subject(0), _observer(0), _function(function) {

    //_subjectList.push_front(0);
    //_subjectList.back = 0;
    //_observerList.front = 0;
    //_observerList.back = 0;


  }
}