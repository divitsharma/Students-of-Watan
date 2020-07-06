#ifndef _OBSERVER_H_
#define _OBSERVER_H_

template <typename InfoType> class Subject;

template <typename InfoType> class Observer {
 public:
   // pass the Subject that called the notify method
  virtual void notify(Subject<InfoType> &whoFrom) = 0;
  virtual ~Observer() = default;
};

#endif
