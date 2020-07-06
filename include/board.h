#ifndef BOARD_H
#define BOARD_H

#include "gameboard.h"
#include "criterion.h"
#include "tile.h"
#include "goal.h"
#include "VertexInfo.h"
#include "EdgeInfo.h"
#include "TileInfo.h"

#include <vector>
#include <memory>


// this is kind of a problem -- using typedefs requires that i include the
// entire file, whereas i could just forward-declare them
class Board;
typedef std::shared_ptr<Board> board_ptr_t;

class Board {
private:
  // Tiles, goals and criteria are initialized using data in GameBoard
  gameboard_ptr_t gameboard;

  // All the tiles, criteria and goals on the board
  std::vector<tile_ptr_t> tiles;
  std::vector<criterion_ptr_t> criteria;
  std::vector<goal_ptr_t> goals;

	//Geese geese;



public:
	// Default ctor - initializes default tiles, criteria and goals
	Board(gameboard_ptr_t gb);

  // Initializes board arrangement based on data
  void init(const std::vector<std::pair<int,int>> &data);

  bool isValidCriteria(size_t index, player_ptr_t owner);
  // Used only in the first round
  bool isPartlyValidCriteria(size_t index, player_ptr_t owner);
  bool isValidGoal(size_t index, const std::string &colour);
  bool isValidImprovement(size_t index, player_ptr_t p);

  void achieveGoal(int index, const std::string &colour);
  void completeCriterion(int index, player_ptr_t owner);
  // Only for first turn
  void completeInitialCriterion(int index, player_ptr_t owner);
  void improveCriterion(size_t index);
  void moveGeese(int index);
  void removeGeese(int index);

  VertexInfo getVertexAt(int index) const;
  EdgeInfo getEdgeAt(int index) const;
  TileInfo getTileAt(int index) const;
  std::vector<VertexInfo> criteriaOnTile(size_t index) const;
  const std::vector<tile_ptr_t> &getTiles() const {return tiles;};


};

#endif
