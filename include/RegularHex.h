#ifndef REGULAR_H
#define REGULAR_H

#include "gameboard.h"
#include <vector>

class RegularHex : public GameBoard {
  //const size_t NUM_TILES = 19;
  const size_t NUM_VERTICES = 54;
  const size_t NUM_EDGES = 72;

void intializeEEAndVV();

public:

  RegularHex();;

  const std::vector<int> &adjacentVerticesToVertex(int vertexNum) const override;
  const std::vector<int> &adjacentVerticesToEdge(int edgeNum) const override;
  const std::vector<int> &adjacentEdgesToVertex(int vertexNum) const override;
  const std::vector<int> &adjacentEdgesToEdge(int edgeNum) const override;
  const std::vector<int> &verticesByTile(int tileNum) const override;

};


#endif
