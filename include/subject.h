#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>

template <typename InfoType> class Observer;

template <typename InfoType> class Subject {
  std::vector< std::shared_ptr<Observer<InfoType>> > observers;
 protected:

 public:
  void attach(std::shared_ptr<Observer<InfoType>> o);
  void notifyObservers();
  virtual InfoType getInfo() const = 0;
};

template <typename InfoType>
void Subject<InfoType>::attach(std::shared_ptr<Observer<InfoType>> o) {
  observers.emplace_back(o);
}

template <typename InfoType>
void Subject<InfoType>::notifyObservers() {
  for (auto &ob : observers) ob->notify(*this);
}

#endif
