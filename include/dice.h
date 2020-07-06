#ifndef DICE_H
#define DICE_H

#include "subject.h"
#include <stdlib.h> // would rand use the seed as set in main????
#include <memory>
#include <iostream>

class Dice;
typedef std::shared_ptr<Dice> dice_ptr_t;

class Dice : public Subject<int> {
private:
  int currentRoll;

public:
  void rollFair() {
    currentRoll = ((rand() % 6) + 1) + ((rand() % 6) + 1);
    // notify tiles of the current roll
    notifyObservers();
    std::cout << "Rolled fair: " << currentRoll << '\n';
  }

  void rollLoaded() {
    notifyObservers();
    std::cout << "Rolled loaded: " << currentRoll << '\n';
  }

  int getInfo() const override {
    return currentRoll;
  }

  void setLoaded(int value) {
    currentRoll = value; 
  }
};

#endif
