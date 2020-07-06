#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <iostream>


class Player;
typedef std::shared_ptr<Player> player_ptr_t;

enum DICE {FAIR, LOADED};

class Player {
private:
  int victoryPoints;
  std::string col;
  DICE dice = DICE::FAIR;
  int lastLoadedRoll;

  // amount owned of each resource type
  std::map<std::string, int> resources;
  // map of gained resources in the last turn
  bool gainedRes = false;
  std::map<std::string, int> gainedResources;

  // owned criteria and goal numbers
  std::vector<int> ownedCriteria;
  std::vector<int> ownedGoals;

  std::map<std::string, std::vector<int>> costs;

public:
  // initialize owned maps
  Player(const std::string &col) : col(col) {
    victoryPoints = 0;

    resources["CAFFEINE"] = resources["LAB"] = resources["LECTURE"]
    = resources["STUDY"] = resources["TUTORIAL"] = 0;

    gainedResources["CAFFEINE"] = gainedResources["LAB"] = gainedResources["LECTURE"]
    = gainedResources["STUDY"] = gainedResources["TUTORIAL"] = 0;

    costs["GOAL"] = {0, 0, 0, 1, 1};
    costs["ASSIGNMENT"] = {1, 1, 1, 0, 1};
    costs["MIDTERM"] = {0, 0, 2, 3, 0};
    costs["FINAL"] = {3, 2, 2, 2, 1};

    costs["CAFFEINE"] = {1, 0, 0, 0, 0};
    costs["LAB"] = {0, 1, 0, 0, 0};
    costs["LECTURE"] = {0, 0, 1, 0, 0};
    costs["STUDY"] = {0, 0, 0, 1, 0};
    costs["TUTORIAL"] = {0, 0, 0, 0, 1};
  }

  // get colour
  const std::string &getColour() const { return col; }

  // get/set dice
  void setDice(DICE d, int val = 0) {
    dice = d;
    if (d == DICE::LOADED) lastLoadedRoll = val;
  }
  DICE getDice() const { return dice; }
  int getLastLoadedRoll() const { return lastLoadedRoll; }

  // increments 'resource' resource by 'by' and records 'by'
  void incrementResources(const std::string &resource, int by) {
    resources.at(resource) += by;
    gainedResources.at(resource) += by;
    gainedRes = true;
  }

  void setResource(const std::string &resource, int amt) {
    resources.at(resource) = amt;
  }

  bool canAfford(const std::string &type) {
    int i = 0;
    for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
      if (resources.at(r) < costs[type].at(i)) return false;
      i++;
    }
    return true;
  }

  void buyGoal(int num, bool init = false) {
    if (!init) {
      int i = 0;
      for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
        resources.at(r) -= costs["GOAL"].at(i);
        i++;
      }
    }
    ownedGoals.push_back(num);
  }

  void buyCriterion(int num, bool init = false) {
    if (!init) {
      int i = 0;
      for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
        resources.at(r) -= costs["ASSIGNMENT"].at(i);
        i++;
      }
    }
    ownedCriteria.push_back(num);
    victoryPoints++;
  }

  void buyImprovement(const std::string &type) {
    int i = 0;
    for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
      resources.at(r) -= costs[type].at(i);
      i++;
    }
    victoryPoints++;
  }
  void incrementVP() { victoryPoints++; }

  void trade(const std::string &lose, const std::string &gain) {
    resources.at(lose) -= 1;
    resources.at(gain) += 1;
  }

  const std::vector<int> &getOwnedCriteria() const { return ownedCriteria; }
  const std::vector<int> &getOwnedGoals() const { return ownedGoals; }


  int numberOf(const std::string &resource) const {
    return resources.at(resource);
  }

  int totalResources() {
    int ret = 0;
    for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
      ret += resources.at(r);
    }
    return ret;
  }

  std::string selectRand(std::vector<std::string> &things) {
    int index = rand() % things.size();
    std::string ret = things.at(index);
    things.erase(things.begin() + index);
    return ret;
  }

  std::map<std::string, int> loseResources(int amt = -1) {
    std::vector<std::string> toLose;
    // flatten resources
    for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
      for (int i = 0; i < resources.at(r); i++) {
        toLose.emplace_back(r);
      }
    }
    std::map<std::string, int> ret;

    amt = amt == -1 ? totalResources() / 2 : amt;

    for (int i = 0; i < amt; i++) {
      ret[selectRand(toLose)] += 1;
    }
    for (auto i : ret) {
      incrementResources(i.first, i.second * -1);
    }
    return ret;
  }

  int getVictoryPoints() {
    return victoryPoints;
  }

  std::string costOf(std::string type) {
    std::string ret = type;
    int vp = 0;
    if (type == "ASSIGNMENT") {
      vp += 1;
      ret = "Assignment (1 VP)\n   ";
    } else if (type == "MIDTERM") {
      vp += 2;
      ret = "Midterm (2 VP)\n   ";
    } else if (type == "FINAL") {
      vp += 3;
      ret = "Final (3 VP)\n   ";
    } else {
      ret = "Goal (0 VP)\n   ";
    }
    int i = 0;
    for (auto &r : {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"}) {
      ret += std::string(" ") + std::to_string(costs[type].at(i)) + r;
      i++;
    }
    return ret;
  }

  // Querying last turn's gained resources
  const std::map<std::string, int> &getGainedResources() const {
    return gainedResources;
  }
  bool gainedResource() const { return gainedRes; }
  void resetGains() {
    for (auto kv : gainedResources) gainedResources.at(kv.first) = 0;
    gainedRes = false;
  }

  void reset() {
    victoryPoints = 0;

    resources["CAFFEINE"] = resources["LAB"] = resources["LECTURE"]
    = resources["STUDY"] = resources["TUTORIAL"] = 0;

    gainedResources["CAFFEINE"] = gainedResources["LAB"] = gainedResources["LECTURE"]
    = gainedResources["STUDY"] = gainedResources["TUTORIAL"] = 0;

    gainedRes = false;

    dice = DICE::FAIR;

    ownedCriteria.clear();
    ownedGoals.clear();
  }

};

#endif
