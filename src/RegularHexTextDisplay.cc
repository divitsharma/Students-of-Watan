#include "../include/RegularHexTextDisplay.h"
#include <iostream>

using namespace std;





RegularHexTextDisplay::RegularHexTextDisplay() {
    // Initialize an empty character grid by adding spaces to all values
    // determined by the vertical and horizontal size of the grid.
    for (int i = 0; i < sizeHorizontal; ++i) {
        vector<char> c;
        theGrid.emplace_back(c);
    }
    for (int i = 0; i < sizeHorizontal; ++i) {
        for (int j = 0; j < sizeVertical; ++j) {
            theGrid.at(i).emplace_back(' ');
        }
    }
}

// Print second
void RegularHexTextDisplay::insertEmptyVertex(vector<int> coordinate, int index) {
    theGrid.at(coordinate[0]).at(coordinate[1]) = (((index / 10) + '0') != '0')? (index / 10) + '0' : ' ';
    theGrid.at(coordinate[0] + 1).at(coordinate[1]) = (index % 10) + '0';
    // cout << index << endl;
}

// Print third
void RegularHexTextDisplay::insertEmptyTile(vector<int> coordinate, int index) {
    theGrid.at(coordinate[0] + 6).at(coordinate[1] - 6) = (((index / 10) + '0') != '0')? (index / 10) + '0' : ' ';
    theGrid.at(coordinate[0] + 7).at(coordinate[1] - 6) = (index % 10) + '0';
}

// Print fourth
void RegularHexTextDisplay::insertEmptyEdge(vector<int> coordinate, int index) {
    theGrid.at(coordinate[0]).at(coordinate[1]) = (((index / 10) + '0') != '0')? (index / 10) + '0' : ' ';
    theGrid.at(coordinate[0] + 1).at(coordinate[1]) = (index % 10) + '0';
}

// Print first
void RegularHexTextDisplay::insertTileShape(int x, int y) {
    // Top layer
    theGrid.at(x).at(y) = '|'; theGrid.at(x + 3).at(y) = '|';
    theGrid.at(x + 4).at(y) = '-'; theGrid.at(x + 5).at(y) = '-';
    theGrid.at(x + 8).at(y) = '-'; theGrid.at(x + 9).at(y) = '-';
    theGrid.at(x + 10).at(y) = '|'; theGrid.at(x + 13).at(y) = '|';
    // Second layer
    theGrid.at(x).at(y - 1) = '\\';
    for (int i = 1; i < 13; ++i) {
        theGrid.at(x + i).at(y - 1) = ' ';
    }
    theGrid.at(x + 13).at(y - 1) = '/';
    // Third layer

    // Fourth layer
    theGrid.at(x - 2).at(y - 3) = '\\';
    theGrid.at(x + 15).at(y - 3) = '/';
    // Fifth layer
    theGrid.at(x - 5).at(y - 4) = '|';
    theGrid.at(x - 2).at(y - 4) = '|';
    theGrid.at(x + 15).at(y - 4) = '|';
    theGrid.at(x + 18).at(y - 4) = '|';
    // Sixth layer
    theGrid.at(x - 2).at(y - 5) = '/';
    theGrid.at(x + 15).at(y - 5) = '\\';
    // Seventh layer

    // Eighth layer
    theGrid.at(x).at(y - 7) = '/';
    theGrid.at(x + 13).at(y - 7) = '\\';
    // Ninth layer
    theGrid.at(x).at(y - 8) = '|'; theGrid.at(x + 3).at(y - 8) = '|';
    theGrid.at(x + 4).at(y - 8) = '-'; theGrid.at(x + 5).at(y - 8) = '-';
    theGrid.at(x + 8).at(y - 8) = '-'; theGrid.at(x + 9).at(y - 8) = '-';
    theGrid.at(x + 10).at(y - 8) = '|'; theGrid.at(x + 13).at(y - 8) = '|';
}

void RegularHexTextDisplay::insertVertex(VertexInfo info, int x) {
    char b = ' ';
    if (info.type == CriteriaType::NONE) {
        return;
    } else if (info.type == CriteriaType::ASSIGNMENT) {
        b = 'A';
    } else if (info.type == CriteriaType::MIDTERM) {
        b = 'M';
    } else if (info.type == CriteriaType::FINAL) {
        b = 'F';
    }
    theGrid.at(theVertexCoordinates.at(x).at(0)).at(theVertexCoordinates.at(x).at(1)) = info.colour;
    theGrid.at(theVertexCoordinates.at(x).at(0) + 1).at(theVertexCoordinates.at(x).at(1)) = b;
}

void RegularHexTextDisplay::insertTile(TileInfo info, int x) {
    string s = info.type;
    int newIndex = (theCoordinates.size() - x) - 1;
    for (size_t i = 0; i < s.size(); ++i) {
        theGrid.at(theCoordinates.at(newIndex).at(0) + 4 + i).at(theCoordinates.at(newIndex).at(1) - 5) = s[i];
    }
    theGrid.at(theCoordinates.at(newIndex).at(0) + 6).at(theCoordinates.at(newIndex).at(1) - 4) = (((info.value / 10) + '0') != '0')? (info.value / 10) + '0' : ' ';
    theGrid.at(theCoordinates.at(newIndex).at(0) + 7).at(theCoordinates.at(newIndex).at(1) - 4) = (info.value % 10) + '0';
}

void RegularHexTextDisplay::insertEdge(EdgeInfo info, int x) {
    if (info.set) {
        theGrid.at(theEdgeCoordinates.at(x).at(0)).at(theEdgeCoordinates.at(x).at(1)) = info.colour;
        theGrid.at(theEdgeCoordinates.at(x).at(0) + 1).at(theEdgeCoordinates.at(x).at(1)) = 'A';
    } else return;
}

void RegularHexTextDisplay::insertGeese(int x, int y) {
    string s = "GEESE";
    for (int i = 4; i < 9; i++) {
        theGrid.at(x + i).at(y - 3) = s[i - 4];
    }
}


void RegularHexTextDisplay::draw(const Board &b, int geese) {

    // Construct all the tiles by iterating through all 19 starting
    // coordinates and inserting them into the grid.
    for (size_t i = 0; i < theCoordinates.size(); ++i) {
        // cout << "drawing base shape" << endl;
        insertTileShape(theCoordinates.at(i).at(0), theCoordinates.at(i).at(1));
    }
    //cout << theGrid.size() << endl;

    // Print all basic vertex coordinates
    for (size_t i = 0; i < theVertexCoordinates.size(); ++i) {
        // cout << "vertexCoor" << endl;
        insertEmptyVertex(theVertexCoordinates[i], i);
    }
    // Print all basic tile coordinates
    int w = 0;
    for (auto it = theCoordinates.rbegin(); it != theCoordinates.rend(); ++it) {
        // cout << "tileCoor" << endl;
        insertEmptyTile(*it, w);
        ++w;
    }
    // Print all basic edge coordinates
    for (size_t i = 0; i < theEdgeCoordinates.size(); ++i) {
        // cout << "edgeCoor" << endl;
        insertEmptyEdge(theEdgeCoordinates.at(i), i);
    }

    // Insert informative vertices
    for (size_t i = 0; i < theVertexCoordinates.size(); ++i) {
        // cout << "infovert" << endl;
        insertVertex(b.getVertexAt(i), i);
    }

     // Insert informative edges
    for (size_t i = 0; i < theEdgeCoordinates.size(); ++i) {
        //cout << "infoedge" << endl;
        insertEdge(b.getEdgeAt(i), i);
    }

     // Insert informative tiles
    for (size_t i = 0; i < theCoordinates.size(); ++i) {
        //cout << i << endl;
        insertTile(b.getTileAt(i), i);
    }

    // because tile 18 is index 0 in theCoordinates
    int loc = 18 - geese;
    insertGeese(theCoordinates.at(loc).at(0), theCoordinates.at(loc).at(1));

    // Output the entire grid to cout
    for (int i = 0; i < sizeVertical; ++i) {
        for (int j = 0; j < sizeHorizontal; ++j) {
            //cout << i << "," << j << endl;
            cout << theGrid.at(j).at(i);
            }
        cout << endl;
    }
}
