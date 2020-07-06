#include "../include/manager.h"
#include "../include/exceptions.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <ctype.h>

using namespace std;

Manager::Manager(
  board_ptr_t mainBoard, const vector<player_ptr_t> &players, display_ptr_t mainDisplay)
  : mainBoard(mainBoard), mainDisplay(mainDisplay),
    players(players), currPlayer(0), dice{make_shared<Dice>()} {

  // set every tile to be an observer of dice
  for (auto t : mainBoard->getTiles())
    dice->attach(t);
}


/* ================== Initializing the Game ================== */

// returns false if eof or parse to num fails
bool readInt(istream &in, int &out) {
  string s;
  if (!(in >> s)) return false;
  stringstream ss{s};
  if (!(ss >> out)) {
    cout << "Corrupted board file." << endl;
    return false;
  }

  return true;
}

// reads board data from a stream
std::vector<pair<int,int>> Manager::generateBoardData(istream &in) {
  vector<pair<int,int>> data;
  // read one line
  string s;
  getline(in, s);
  istringstream iss{s};

  // populate data from in until you can no longer read ints
  while (true) {
    pair<int, int> p;
    // read in the two numbers
    if (!readInt(iss, p.first)) break;
    if (!readInt(iss, p.second)) break;

    data.push_back(p);

    if (p.first == 5) {
      geese = data.size();
    }
  }

  return data;
}

// converts board save string to vector of pairs and initializes board
void Manager::loadBoard(const std::string &file) {
  std::cout << "Loading board from file: " << file << '\n';
  // open file
  ifstream boardFile{file};

  // if file was found, init board with contents of boardFile
  if (boardFile) {
    // initialize board
    boardInfo = generateBoardData(boardFile);
    mainBoard->init(boardInfo);
  } else {
    // TODO: make this an error later
    std::cout << "can't find board file" << '\n';
  }
}

// Loads the state of players, manager and board from save file
void Manager::loadGame(const std::string &file) {
  std::cout << "Loading previously saved game." << '\n';
  // open file
  ifstream saveFile{file};

  // load game if file was found, otherwise abort
  if (saveFile) {
    string s;

    /* First line is current turn, [0,3]
     * next four lines are each player data (resources, goals, and criteria)
     * next line is board string
     * last line is the geese tile,, [0,18]
     */

     // current turn
    saveFile >> s;
    currPlayer = stoi(s);

    // each player data
    for (player_ptr_t p : players) {
      // populating resources
      for (auto &resource : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
        saveFile >> s;
        p->setResource(resource, stoi(s));
      }

      // goals
      saveFile >> s;
      assert(s == "g"); // make sure reading correctly
      // read numbers until a "c"
      while (true) {
        saveFile >> s;
        if (s == "c") break;

        // add goal for each number
        achieveGoal(stoi(s), p, true);
      }

      // criteria - read until end of line
      getline(saveFile, s);
      istringstream ss{s};
      int n;
      while (ss >> n) {
        int m;
        ss >> m;
        completeCriterion(n, p, true);
        for (; m > 1; m--) { // improve the critera m times
          improveCriterion(n, p, true);
        }
      }
    }

    // read in the board and initialize board
    boardInfo = generateBoardData(saveFile);
    mainBoard->init(boardInfo);

    // geese
    saveFile >> s;
    geese = stoi(s);

  } else {
    std::cout << "can't find save file" << '\n';
  }
}

int selectRand(vector<int> &nums) {
  int index = rand() % nums.size();
  int ret = nums.at(index);
  nums.erase(nums.begin() + index);
  return ret;
}

// Creates the board data randomly
void Manager::createRandom() {
  /*
  LEGEND
  Resources:
  3 tutorial (id 4)
  3 study (id 3)
  4 caffeine (id 0)
  4 lab (id 1)
  4 lecture (id 2)
  1 netflix (id 5)
  Dice values:
  one 2 one 12
  two 3,4,5,6 two 8,9,10,11*/

  // vector that stores all the possible resources, see legend
  // emplace four 0s, four 1s, four 2s
  vector<int> resources{};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      resources.emplace_back(i);
    }
  }
  // emplace three 3s, three 4s
  for (int i = 3; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      resources.emplace_back(i);
    }
  }

  vector<int> values{};
  values.emplace_back(2);
  values.emplace_back(12);
  for (int i = 3; i < 12; i++) {
    if (i == 7) continue;
    values.emplace_back(i);
    values.emplace_back(i);
  }

  for (int i = 0; i < 18; i++) {
    boardInfo.emplace_back(selectRand(resources), selectRand(values));
  }

  pair<int, int> netflix(5, 7);
  // random number between 0 and 18
  int index = rand() % 18;
  geese = index;
  // insert netflix at a random index
  boardInfo.insert(boardInfo.begin() + index, netflix);

  mainBoard->init(boardInfo);
}






/* ================== Rolling the Dice ================== */

// updates player's die to be loaded, and updates dice's roll value
void Manager::setLoadedDie(int value) {
  players.at(currPlayer)->setDice(DICE::LOADED, value);
  dice->setLoaded(value);
}
// updates player's die to be fair
void Manager::setFairDie() {
  players.at(currPlayer)->setDice(DICE::FAIR);
}
// rolls a die based on what the player's dice is
void Manager::roll() {
  if (currentPlayer()->getDice() == DICE::FAIR)
    dice->rollFair();
  else if (currentPlayer()->getDice() == DICE::LOADED) {
    dice->setLoaded(currentPlayer()->getLastLoadedRoll());
    dice->rollLoaded();
  }
}

string Manager::getRollStatus() const {
  // check if any player earned resource in this roll
  bool playerGainedRes = false;
  for (auto p : players) {
    if (p->gainedResource()) {
      playerGainedRes = true;
      break;
    }
  }

  stringstream ss;

  // if someone did, print out recently gained resources of players
  // that gained resources
  if (playerGainedRes) {
    for (auto p : players)
      if (p->gainedResource()) {
        ss << "Student " << p->getColour() << " gained:" << '\n';
        for (auto &kv : p->getGainedResources())
          if (kv.second)
            ss << kv.second << " " << kv.first << endl;
      }
  } else {
    ss << "No students gained resources." << '\n';
  }

  return ss.str();
}








/* ================== Interacting With Board ================== */

// calls draw on display with the board
void Manager::printBoard() {
  mainDisplay->draw(*mainBoard, geese);
}

void Manager::achieveGoal(int loc, player_ptr_t p, bool init) {
  if (!p) p = players[currPlayer];

  // if not init, check if player can afford and if location is valid
  if (!init) {
    // if player can't afford, throw
    if (!(p->canAfford("GOAL"))) throw NoResources();

    // if building location is invalid, throw
    if (loc < 0 || !mainBoard->isValidGoal(loc, p->getColour()))
      throw InvalidBuild();
  }

  mainBoard->achieveGoal(loc, p->getColour());
  p->buyGoal(loc, init);

}


void Manager::completeCriterion(int loc, player_ptr_t p, bool init) {
  if (!p) p = players[currPlayer];

  // if init only check partial validity (adj vertices, out of bounds, occupied)
  if (init) {
    if (!(mainBoard->isPartlyValidCriteria(loc, p)))
      throw InvalidBuild();
  } else {
    // otherwise check completely valid (adj edges) and affordability
    if (loc < 0 || (!mainBoard->isValidCriteria(loc, p)))
      throw InvalidBuild();

    if (!(p->canAfford("ASSIGNMENT"))) throw NoResources();
  }

  mainBoard->completeCriterion(loc, p);
  p->buyCriterion(loc, init);
}


void Manager::improveCriterion(int loc, player_ptr_t p, bool init) {
  if (!p) p = players[currPlayer];

  // If location is not built on, or a final, you can't improve
  if (loc < 0 ||!(mainBoard->isValidImprovement(loc, p)))
    throw InvalidBuild();

  // if not init, check if player can afford and buy the improvement
  if (!init) {
    string type = mainBoard->getVertexAt(loc).type == ASSIGNMENT ?
    "MIDTERM" : "FINAL";

    if (!(p->canAfford(type))) throw NoResources();
    p->buyImprovement(type);
  } else {
    p->incrementVP();
  }

  mainBoard->improveCriterion(loc);
}

void Manager::trade(const string &p2, string &give, string &take) {
  //first check that colour p2 refers to a valid student
  //check that give and take (uppercase) are valid resources
  //and both players can afford
  size_t p = 0;
  if (p2 == "Red") {
    p = 1;
  } else if (p2 == "Orange") {
    p = 2;
  } else if (p2 == "Yellow") {
    p = 3;
  } else if (p2 != "Blue") {
    throw InvalidInput();
  }
  for (auto & c: give) c = toupper(c);
  for (auto & c: take) c = toupper(c);
  int valid = 0;
  for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
    if (r == give || r == take) valid++;
  }
  if (valid == 2) {
    if (currentPlayer()->canAfford(give) && players.at(p)->canAfford(take)) {
      currentPlayer()->trade(give, take);
      players.at(p)->trade(take, give);
    } else {
      throw NoResources();
    }
  } else {
    throw InvalidInput();
  }
}

bool Manager::seven() {
  return dice->getInfo() == 7;
}

void Manager::moveGeese(int loc) {
  // try moving the geese to loc
  if (loc >= 0 && loc <= 18 && loc != geese) {
    geese = loc;
  } else {
    throw InvalidBuild();
  }
}

player_ptr_t Manager::playerByColour(char col) const {
  for (auto p : players)
    if (p->getColour()[0] == col) return p;

  return nullptr;
}

vector<player_ptr_t> Manager::playersOnTile(int loc) const {
  vector<player_ptr_t> players;

  // return all players with criteria on the tile, that arent the current
  // player, and have some resources
  for (VertexInfo &v : mainBoard->criteriaOnTile(loc)) {
    if (find(players.begin(), players.end(), playerByColour(v.colour)) == players.end()
        && v.colour != currentPlayer()->getColour()[0]
        && playerByColour(v.colour)->totalResources() > 0)
          players.push_back(playerByColour(v.colour));
  }

  return players;
}

string Manager::steal(const string &fromCol) {
  auto from = playerByColour(fromCol[0]);
  string res = (*(from->loseResources(1).begin())).first;
  players[currPlayer]->incrementResources(res, 1);
  return res;
}

string Manager::doGeese() {
  stringstream ss;
  for (auto p : players) {
    if (p->totalResources() >= 10) {
      int lost = p->totalResources() / 2;
      ss << "Student " << p->getColour() << " loses "
      << to_string(lost) << " resources to the geese. They lose:" << endl;
      for (auto &j : p->loseResources()) {
        if (j.second) {
          ss << j.second << " " << j.first << endl;
        }
      }
    }
  }
  return ss.str();
}

void Manager::next(const int direction) {
  currPlayer += direction;
  currPlayer %= players.size();
  // reset all players for the next turn
  for (auto p : players) p->resetGains();
}

void Manager::saveGame(const std::string &filename) {
  std::cout << "Saving game..." << '\n';

  ofstream saveFile{filename};

  // current turn
  saveFile << currPlayer << '\n';

  // each player data
  for (player_ptr_t p : players) {

    // writing resources
    for (auto &resource : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
      saveFile << p->numberOf(resource) << " ";
    }

    // goals
    saveFile << "g ";

    // write goal numbers
    for (int goal : p->getOwnedGoals())
      saveFile << goal << " ";

    saveFile << "c";

    // write each criteria number followed by level of improvements
    for (int c : p->getOwnedCriteria())
      saveFile << " " << c << " " << mainBoard->getVertexAt(c).type;

    saveFile << '\n';
  }

  // board data
  for (size_t i = 0; i < boardInfo.size(); i++) {
    if (i != 0) saveFile << " ";
    saveFile << boardInfo[i].first << " " << boardInfo[i].second;
  }

  // geese
  saveFile << '\n' << geese << '\n';
}

bool Manager::winner() {
  if (currentPlayer()->getVictoryPoints() >= 10) return true;
  return false;
}

void Manager::setDisplay(display_ptr_t display) {
  mainDisplay = display;
}

void Manager::setBoard(board_ptr_t board) {
  mainBoard = board;
}

const player_ptr_t Manager::currentPlayer() const {
  return players.at(currPlayer);
}

// returns status of player
string Manager::getStatus(const player_ptr_t p) const {
  return p->getColour() + " has " + to_string(p->getVictoryPoints())
  + " victory points, "
  + to_string(p->numberOf("CAFFEINE")) + " caffeines, "
  + to_string(p->numberOf("LAB")) + " labs, "
  + to_string(p->numberOf("LECTURE")) + " lectures, "
  + to_string(p->numberOf("STUDY")) + " studies, and "
  + to_string(p->numberOf("TUTORIAL")) + " tutorials.";
}
// returns status of current player
string Manager::getCurPlayerStatus() const {
  return getStatus(players[currPlayer]);
}
// returns status of all players
std::string Manager::getAllStatuses() const {
  string ret = "";
  for (player_ptr_t p : players) {
    ret += getStatus(p) + "\n";
  }
  return ret;
}

string Manager::costs() {
  stringstream ss;
  ss << "Building Costs:\n" << endl;
  for (auto i : {"GOAL", "ASSIGNMENT", "MIDTERM", "FINAL"}) {
    ss << currentPlayer()->costOf(i) << endl;
  }
  return ss.str();
}

string Manager::help() {
  return string("Valid commands:\nboard\nstatus\ncriteria\nachieve <goal>\n") +
  "complete <criterion>\nimprove <criterion>\ntrade <colour> <give> <take>\n" +
  "next\nsave <file>\ncosts\nhelp";
}

string Manager::getCriteria() const {
  const auto p = currentPlayer();
  string ret = p->getColour() + " has completed:\n";
  for (int critNum : p->getOwnedCriteria()) {
    ret += to_string(critNum) + " " + to_string(mainBoard->getVertexAt(critNum).type) + '\n';
  }
  return ret;
}

void Manager::reset() {
  for (size_t i = 0; i < players.size(); i++) {
    players.at(i).reset();
  }
  boardInfo.clear();
}
