#ifndef TILE_H
#define TILE_H

#include "observer.h"
#include "subject.h"
#include "TileInfo.h"
#include <memory>
#include <string>
#include <iostream>

class Tile;
typedef std::shared_ptr<Tile> tile_ptr_t;

class Tile : public Observer<int>, public Subject<TileInfo> {
private:
  std::string type = "NONE";
  int value;
  bool geese = false;

public:
  void setValue(int value) { this->value = value; }
  void setType(std::string type) { this->type = type; }

  void moveGeese() {
    geese = true;
  }
  void removeGeese() {
    geese = false;
  }

  TileInfo getInfo() const override { return {type, value}; }

  // Dice notifies each tile of the current roll
  // notify all vertices if roll matched value and this is not the netflix tile
  void notify(Subject<int> &whoFrom) override {
    // std::cout << "notified me with roll " << whoFrom.getInfo() << '\n';
    if (whoFrom.getInfo() == value && type != "NETFLIX") {
      if (!geese) {
        notifyObservers();
      }
    }
  }
};



#endif
