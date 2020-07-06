#ifndef GOAL_H
#define GOAL_H

#include "EdgeInfo.h"
#include <memory>
#include <string>

class Goal;
typedef std::shared_ptr<Goal> goal_ptr_t;

class Goal {
private:
  bool set = false;
  std::string colour;
public:
  void achieve(const std::string &colour) {
    set = true;
    this->colour = colour;
  }
  EdgeInfo getInfo() const { return {colour[0], set}; }
};



#endif
