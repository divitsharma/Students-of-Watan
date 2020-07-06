#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <memory>

class GameBoard;
typedef std::shared_ptr<GameBoard> gameboard_ptr_t;

class GameBoard {

public:
  const size_t numTiles;
  const size_t numVertices;
  const size_t numEdges;

  GameBoard(size_t nt, size_t nv, size_t ne) : numTiles(nt), numVertices(nv), numEdges(ne) {};
  virtual ~GameBoard() {}

  // Following return numbers of all adjacent Vertices/Edges
  // to a given Vertex/Edge number
  virtual const std::vector<int> &adjacentVerticesToVertex(int vertexNum) const = 0;
  virtual const std::vector<int> &adjacentVerticesToEdge(int edgeNum) const = 0;
  virtual const std::vector<int> &adjacentEdgesToVertex(int vertexNum) const = 0;
  virtual const std::vector<int> &adjacentEdgesToEdge(int edgeNum) const = 0;
  virtual const std::vector<int> &verticesByTile(int tileNum) const = 0;

};



#endif
