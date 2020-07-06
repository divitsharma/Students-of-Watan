#ifndef CRITERION_H
#define CRITERION_H

#include "subject.h"
#include "VertexInfo.h"
#include "TileInfo.h"
#include "observer.h"
#include "player.h" // can forward-declare here later
#include <memory>
#include <iostream>


struct TileInfo; // idk if i can get away with this here
class Criterion;
typedef std::shared_ptr<Criterion> criterion_ptr_t;

class Criterion : public Observer<TileInfo> {
private:
  CriteriaType type = CriteriaType::NONE;
  player_ptr_t owner = nullptr;

public:
  void place(player_ptr_t player) {
    owner = player;
    improve();
  }
  // only call if player is set
  void improve() {
    switch (type) {
      case NONE: type = CriteriaType::ASSIGNMENT; break;
      case ASSIGNMENT: type = CriteriaType::MIDTERM; break;
      case MIDTERM: type = CriteriaType::FINAL; break;
      default: break;
    }
  }

  VertexInfo getInfo() const {
    VertexInfo v;
     if(owner) {
      v.colour = owner->getColour()[0];
    } else {
      v.colour = '\0';
    }
    v.type = type;
    return v;
  }

  // when notified by a tile, if this criteria has an owner,
  // give the owner appropriate resources
  void notify(Subject<TileInfo> &whoFrom) override {
    // std::cout << "crit notted" << '\n';
    if (owner) {
      int by = 0;
      // choose increment amount based on type
      switch (type) {
        case ASSIGNMENT: by += 1; break;
        case MIDTERM: by += 2; break;
        case FINAL: by += 3; break;
        default: break;
      }
      owner->incrementResources(whoFrom.getInfo().type, by);
    }
  }
};



#endif
