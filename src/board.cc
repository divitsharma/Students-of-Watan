#include "../include/board.h"
#include <iostream>
#include <assert.h>

using namespace std;

// Default ctor - initializes default tiles, criteria and goals
Board::Board(gameboard_ptr_t gb)
  : gameboard(gb),
    tiles(std::vector<tile_ptr_t>(gb->numTiles)),
    criteria(std::vector<criterion_ptr_t>(gb->numVertices)),
    goals(std::vector<goal_ptr_t>(gb->numEdges)) {

      // std::cout << tiles.size() << '\n';
      // std::cout << criteria.size() << '\n';
      // std::cout << goals.size() << '\n';

  // Fill vectors with default tile/criteria/goal values
  for (size_t i = 0; i < tiles.size(); i++)
    tiles[i] = make_shared<Tile>();

  for (size_t i = 0; i < criteria.size(); i++)
    criteria[i] = make_shared<Criterion>();

  for (size_t i = 0; i < goals.size(); i++)
    goals[i] = make_shared<Goal>();


  // initialize observers (vertices) of each tile
  for (size_t i = 0; i < tiles.size(); i++) {
    for (int v : gameboard->verticesByTile(i)) {
      tiles[i]->attach(criteria[v]);
    }
  }

  // std::cout << "made board" << '\n';
  // std::cout << criteria.size() << '\n';
  // std::cout << criteria[18]->getInfo().colour << '\n';
}

// Initializes board arrangement based on data
// data is a list of pairs of <tile type, value>
void Board::init(const std::vector<std::pair<int, int>> &data) {
  //std::cout << "Board.cc: initializing board" << '\n';
  assert(tiles.size() == data.size()); // this must be the case
  // auto curTile = tiles.begin();
  int curr = 0;

  for (const auto &pair : data) {
    // fill in type and value TODO: remove magic numbers
    switch (pair.first) {
      case 0: tiles[curr]->setType("CAFFEINE"); break;
      case 1: tiles[curr]->setType("LAB"); break;
      case 2: tiles[curr]->setType("LECTURE"); break;
      case 3: tiles[curr]->setType("STUDY"); break;
      case 4: tiles[curr]->setType("TUTORIAL"); break;
      case 5: tiles[curr]->setType("NETFLIX"); break;
    }
    tiles[curr]->setValue(pair.second);
    curr++;
  }

  // for (auto tile : tiles)
  //   std::cout << tile->getInfo().type << '\n';
}


// First check partly correct (not beside any criteria)
// Then iterate through all the edges to determine if a road of player type exists.
bool Board::isValidCriteria(size_t index, player_ptr_t pl) {
  if (!isPartlyValidCriteria(index, pl)) return false;

  for (auto p : gameboard->adjacentEdgesToVertex(index)) {
    if(goals.at(p)->getInfo().colour == pl->getColour()[0]) {
      return true;
    }
  }
  return false;
}

// Check if index is already occupied neighbouring vertices are occupied
bool Board::isPartlyValidCriteria(size_t index, player_ptr_t pl) {
  //check out of bounds
  if (index >= criteria.size()) return false;
  // check if index already taken
  if (criteria.at(index)->getInfo().colour != '\0') return false;

  // check if any adj criteria is already taken
  for (int p : gameboard->adjacentVerticesToVertex(index))
    if(criteria.at(p)->getInfo().colour != '\0')
      return false;

  return true;
}


// Checks all neighbouring vertices for player colour. If that fails it checks all
// neighbouring edges for goals of same type.
bool Board::isValidGoal(size_t index, const std::string &colour) {
  // check out of bounds
  if (index >= goals.size()) return false;
  // check if index already taken
  if (goals.at(index)->getInfo().set) return false;

  // location valid if some adj vertex is of same colour
  for (int p : gameboard->adjacentVerticesToEdge(index)) {
    if(criteria.at(p)->getInfo().colour == colour[0]) {
      return true;
    }
  }

  // or, some adjacent edge is of same colour
  for (int e : gameboard->adjacentEdgesToEdge(index))
    if(goals.at(e)->getInfo().colour == colour[0])
      return true;

  return false;
}

bool Board::isValidImprovement(size_t index, player_ptr_t p) {
  // out of bounds
  if (index >= criteria.size()) return false;
  // there is no criteria there or its already max improvement
  if (criteria.at(index)->getInfo().type == NONE ||
      criteria.at(index)->getInfo().type == FINAL)
      return false;
  // if someone else owns the criteria
  if (criteria.at(index)->getInfo().colour != p->getColour()[0]) return false;


  return true;
}


void Board::achieveGoal(int index, const std::string &colour) {
  goals.at(index)->achieve(colour);
}

// checks for validity in vertices and edges
void Board::completeCriterion(int index, player_ptr_t owner) {
  criteria.at(index)->place(owner);
}

// only checks nearby vertices for validity
void Board::completeInitialCriterion(int index, player_ptr_t owner) {
  criteria.at(index)->place(owner);
}

void Board::improveCriterion(size_t index) {
  criteria.at(index)->improve();
}

void Board::moveGeese(int index) {
  // std::cout << "moving geese to: " << index << '\n';
  tiles.at(index)->moveGeese();
}

void Board::removeGeese(int index) {
  // std::cout << "moving geese to: " << index << '\n';
  tiles.at(index)->removeGeese();
}

VertexInfo Board::getVertexAt(int index) const {
  return criteria.at(index)->getInfo();
}

EdgeInfo Board::getEdgeAt(int index) const {
  return goals.at(index)->getInfo();
}

TileInfo Board::getTileAt(int index) const {
  return tiles.at(index)->getInfo();
}

vector<VertexInfo> Board::criteriaOnTile(size_t index) const {
  vector<VertexInfo> criteria;
  for (int v : gameboard->verticesByTile(index)) {
    auto vInfo = this->criteria.at(v)->getInfo();
    if (vInfo.type != NONE) criteria.push_back(vInfo);
  }

  return criteria;
}
