#include <iostream>
#include <sstream>
#include <string>
#include <memory>   // for shared_ptr
#include <stdlib.h> // for rand, srand
#include <time.h>   // for time

//All .h files here:
#include "../include/manager.h"
#include "../include/player.h"
#include "../include/board.h"
#include "../include/RegularHexTextDisplay.h"
#include "../include/ColourHexTextDisplay.h"
#include "../include/RegularHex.h"
#include "../include/exceptions.h"


using namespace std;


bool colour;

string playerToColour(const string &playerCol) {
  if (playerCol == "Red") return "\033[31m";
  else if (playerCol == "Blue") return "\033[34m";
  else if (playerCol == "Orange") return "\033[38;5;202m";
  else if (playerCol == "Yellow") return "\033[33m";
  else return "";
}

// Handles how input is recieved
bool input(string &in, Manager &manager) {
  if (colour)
    cout << playerToColour(manager.currentPlayer()->getColour());
  cout << "> " << "\033[0m";
  cin >> in;
  if (cin.eof()) {
    manager.saveGame("/backups/backup.sv");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {

  /* ====== Initializing game elements ====== */

  vector<player_ptr_t> students = {make_shared<Player>("Blue"), make_shared<Player>("Red"),
                                   make_shared<Player>("Orange"), make_shared<Player>("Yellow")};

  // create a manager for a new board and display,
  // initializing the board with a RegularHex
  Manager manager{make_shared<Board>(make_shared<RegularHex>()), students};
  manager.setDisplay(make_shared<RegularHexTextDisplay>());

  // read in command-line options
  bool load = false;
  bool board = false;
  for (int i = 1; i < argc; i++) {
    if (string{argv[i]} == "-seed") {
      if (i + 1 == argc) {
        cout << "Usage: -seed ###" << endl;
      }
      else {
        srand(stoi(argv[i + 1]));
      }
    }
    else if (string{argv[i]} == "-load") {
      // read in and store save file name
      if (i + 1 == argc) {
        cout << "Usage: [add usage]" << endl;
        return 1;
      }
      else {
        // laod board with next argument
        manager.loadGame(argv[i + 1]);
      }
      load = true;
    }
    else if (string{argv[i]} == "-board") {
      // read in and store board file name
      if (i + 1 == argc) {
        cout << "Usage: [add usage]" << endl;
        return 1;
      }
      else {
        // load board with next argument
        manager.loadBoard(argv[i + 1]);
      }
      board = true;
    }
    else if (string{argv[i]} == "-colour") {
      colour = true;
      manager.setDisplay(make_shared<ColourHexTextDisplay>());
    }
  }


  /* ================== Main Game Loop ================== */

  while (true) {

    // if no other method of board generation was given, create random board
    if (!load && !board) {
      manager.createRandom();
    }

    // input string
    string command;

    bool game = true;

    cout <<
    string(
    "------------------------------------------------------------------------------------\n")
    +
    "----------------------------------Welcome to Watan!---------------------------------\n"
    +
    "------------------------------------------------------------------------------------\n"
    << endl;
    manager.printBoard();


    // Choose initial criteria
    // if not generating board from load file, input initial criterion/goal placements
    if (!load) {
      int direction = 1;
      bool done = false;
      while (true) {
        cout << "Student " << manager.currentPlayer()->getColour()
          << ", where do you want to complete an Assignment?" << endl;

        // exit on eof()
        if (!input(command, manager)) return 1;
        stringstream ss{command};
        int n;
        ss >> n; // TODO: do something if this fails - general readint fn
        try {
          manager.completeCriterion(n, manager.currentPlayer(), true);
          manager.printBoard();
        } catch (WatanException &e) {
          cout << e.what() << endl;
          continue;
        }
        if (done) break;

        manager.next(direction);
        if (manager.currentPlayer()->getColour() == "Blue") {
          if (direction == 1) {
            manager.next(3);
            direction = -1;
          } else {
            done = true;
          }
        }
      }
    }

    /* ====== Main Game Loop ====== */
    while (game) {

      // do player turn by taking in input, passing the action inputted
      // into manager with current player
      string currCol = manager.currentPlayer()->getColour();
      cout << "Student " << currCol << "'s turn." << endl;
      cout << manager.getCurPlayerStatus() << endl;

      // Beginning of Turn
      while (true) {
        if (!input(command, manager))
          return 1;
        if (command == "load") {
          int n;
          while (true) {
            cout << "Input a roll between 2 and 12" << endl; // TODO: put in loop
            if (!input(command, manager)) return 1;

            stringstream ss{command};
            ss >> n;
            if (!ss || n < 2 || n > 12) cout << "Invalid command." << endl;
            else break;
          }
          manager.setLoadedDie(n);
        } else if (command == "fair") {
          manager.setFairDie();
        }
        else if (command == "roll") {
          manager.roll();
          if (manager.seven()) {
            cout << "The geese are attacking!" << endl;
            cout << manager.doGeese(); // losing of resources
            cout << "Choose where to place the GEESE." << endl;
            int n;
            while (true) {
              if (!input(command, manager)) return 1;
              stringstream ss{command};
              if (!(ss >> n)) {
                cout << "Invalid command." << endl;
              } else {
                try {
                  manager.moveGeese(n);
                  break;
                }
                catch (WatanException &e) {
                  cout << e.what() << endl;
                }
              }
            }
            //do steal
            auto playersToSteal = manager.playersOnTile(n);
            if (playersToSteal.empty()) {
              cout << "Student " << currCol << " has no students to steal from.\n";
            } else {
              cout << "Student " << currCol << " can choose to steal from";
              for (auto &p : playersToSteal) cout << " " << p->getColour();
              cout << "." << endl;

              // enter student to steal from
              while (true) {
                cout << "Choose a student to steal from." << endl;
                if (!input(command, manager)) return 1;
                bool isValid = true;
                for (auto p : playersToSteal) {
                  if (p->getColour() == command) {
                    isValid = true;
                  }
                }
                if (isValid) {
                  cout << "Student " << currCol << " steals " <<
                  manager.steal(command) << " from student " << command << ".\n";
                  break;
                } else {
                  cout << "Invalid player name" << endl;
                }
              }
            }


          } else {
            cout << manager.getRollStatus() << endl;
          }
          break;
        } else {
          cout << "Invalid command." << endl;
        }
      }

      // End of Turn - loop until 'next' entered
      while (true) {
        if (!input(command, manager)) {
          return 1;
        }
        if (command == "board") {
          // draw board
          manager.printBoard();
        } else if (command == "status") {
          // print status of all students in order
          cout << manager.getAllStatuses() << endl;
        } else if (command == "criteria") {
          // print current player criteria
          cout << manager.getCriteria();
        } else if (command == "achieve") {
          // read in index to achieve
          cin >> command;
          stringstream ss{command};
          int n;
          if (!(ss >> n)) {
            cout << "Invalid command." << endl;
          } else {
            try {
              manager.achieveGoal(n);
              cout << "Goal achieved!" << endl;
            }
            catch (WatanException &e) {
              cout << e.what() << endl;
            }
          }
        }
        else if (command == "complete") {
          // read in index to complete TODO: repeated code
          cin >> command;
          stringstream ss{command};
          int n;
          if (!(ss >> n)) {
            cout << "Invalid command." << endl;
          } else {
            try {
              manager.completeCriterion(n);
              cout << "Criteria completed!" << endl;
              if (manager.winner()) {
                cout << "Student " << currCol <<
                " has won the game!" << endl;
                game = false;
                break;
              }
            } catch (WatanException &e) {
              cout << e.what() << endl;
            }
          }
        } else if (command == "improve") {
          // read in index to complete TODO: repeated code
          cin >> command;
          stringstream ss{command};
          int n;
          if (!(ss >> n)) {
            cout << "Invalid command." << endl;
          } else {
            try {
              manager.improveCriterion(n);
              cout << "Criteria improved!" << endl;
              if (manager.winner()) {
                cout << "Student " << currCol <<
                " has won the game!" << endl;
                game = false;
                break;
              }
            } catch (WatanException &e) {
              cout << e.what() << endl;
            }
          }
        } else if (command == "trade") {
          string p2;
          string give;
          string take;
          cin >> p2;
          cin >> give;
          cin >> take;
          p2[0] = toupper(p2[0]);
          cout << currCol << " offers " <<
          p2 << " one " << give << " for one " << take << "." << endl;
          cout << "Does " << p2 << " accept this offer?" << endl;
          do {
            if (!input(command, manager)) return 1;
          } while (command != "yes" && command != "no");
          if (command == "yes") {
            try {
              manager.trade(p2, give, take);
              cout << "Trade successful!" << endl;
            } catch (WatanException &e) {
              cout << e.what() << endl;
            }
          } else {
            cout << "Trade declined!" << endl;
          }
        } else if (command == "save") {
            cin >> command;
            manager.saveGame(command);
        } else if (command == "costs") {
          cout << manager.costs() << endl;
        } else if (command == "help") {
          cout << manager.help() << endl;
        } else if (command == "next") {
          manager.next();
          break;
        } else {
          cout << "Invalid Command." << endl;
        }
      }
    }


    cout << "Would you like to play again?" << endl;
    do {
      if (!input(command, manager)) return 1;
    } while (command != "yes" && command != "no");
    if (command == "yes") {
      cout << "This feature is not supported yet." << endl;
      //please help im tired and i dont know how to reset shit
      //see reset() in manager and player (at bottom of both files)
      // load = false;
      // board = false;
      // manager.reset();
      // auto newDisplay = make_shared<ColourHexTextDisplay>();
      // auto newHex = make_shared<RegularHex>();
      // auto newBoard = make_shared<Board>(newHex);
      // manager.setDisplay(newDisplay);
      // manager.setBoard(newBoard);
    } else {
      break;
    }
  }
  cout << "Thank you for playing Students of Watan!" << endl;
}
