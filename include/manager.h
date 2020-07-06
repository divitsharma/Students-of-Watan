#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "board.h"
#include "display.h"
#include "player.h"
#include "dice.h"
#include "LastRollInfo.h"

#include <string>
#include <vector>
#include <iostream>
#include <locale> // for making strings uppercase


class Manager {
private:
  board_ptr_t mainBoard;
  display_ptr_t mainDisplay;
  std::vector<player_ptr_t> players;
  size_t currPlayer;

  dice_ptr_t dice;

  std::string getStatus(const player_ptr_t p) const;
  // generates the vector to initialize board from a stream
  std::vector<std::pair<int,int>> generateBoardData(std::istream &in);

  // given by load functions or created random
  std::vector<std::pair<int,int>> boardInfo;

  player_ptr_t playerByColour(char col) const;

  int geese=0;

public:
  // Initialization
  Manager(board_ptr_t mainBoard,
          const std::vector<player_ptr_t> &players,
          display_ptr_t mainDisplay = nullptr);

  // Loads order and values of tiles from file
  void loadBoard(const std::string &file);
  void loadGame(const std::string &file);
  void createRandom();

  // Commands
  void setLoadedDie(int value);
  void setFairDie();
  void roll();
  std::string getRollStatus() const;

  void printBoard();

  void achieveGoal(int loc, player_ptr_t p = nullptr, bool init = false);
  void completeCriterion(int loc, player_ptr_t p = nullptr, bool init = false);
  void improveCriterion(int loc, player_ptr_t p = nullptr, bool init = false);
  void trade(
    const std::string &p2, std::string &give, std::string &take);
  bool seven();
  std::string doGeese();
  void moveGeese(int loc);
  std::vector<player_ptr_t> playersOnTile(int loc) const;
  std::string steal(const std::string &fromCol);

  void next(const int direction = 1);

  std::string getCurPlayerStatus() const;
  std::string getAllStatuses() const;
  std::string getCriteria() const;

  void saveGame(const std::string &filename);

  std::string costs();
  std::string help();

  bool winner();

  // Setters/getters
  void setDisplay(display_ptr_t display);
  void setBoard(board_ptr_t mainBoard);
  const player_ptr_t currentPlayer() const;

  void reset();
};


#endif
