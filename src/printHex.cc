#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Declare the size of the grid
int sizeHorizontal = 86;
int sizeVertical = 41;

int startx = 35;
int starty = 40;

// Using hexagonal coordinates
vector<vector<int> > theCoordinates {{startx,starty},  
                                    {startx + 15, starty - 4}, 
                                    {startx + 30, starty - 8}, 
                                    {startx - 15, starty - 4},
                                    {startx - 30, starty - 8},
                                    {startx, starty - 8},
                                    {startx + 15, starty - 12},
                                    {startx + 30, starty - 16},
                                    {startx + 15, starty - 20},
                                    {startx, starty - 16},
                                    {startx - 15, starty - 12},
                                    {startx - 30, starty - 16},
                                    {startx - 30, starty - 16},
                                    {startx - 15, starty - 20},
                                    {startx - 30, starty - 24},
                                    {startx - 15, starty - 28},
                                    {startx, starty - 24},
                                    {startx, starty - 32},
                                    {startx + 15, starty - 28},
                                    {startx + 30, starty - 24},};

vector<vector<char> > theGrid;

// Coordinates are given representing the bottom left of each hexagonal tile. Starting 
// from the first |. It then builds the layers above it one by one. Layers 3 and 7 aren't printed
// at this stage. 

void insertTileShape(int x, int y) {
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
    theGrid.at(x - 2).at(y - 2) = '2'; theGrid.at(x - 1).at(y - 2) = '4';
    theGrid.at(x + 13).at(y - 2) = '4'; theGrid.at(x + 14).at(y - 2) = '2';
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
    // Name of title
    string word = "WATAN";
    for (int i = 0; i < word.length(); i++) {
    	theGrid.at(x + 4 + i).at(y - 5) = word[i];
    }
    theGrid.at(x + 15).at(y - 5) = '\\';
    // Seventh layer
    theGrid.at(x - 2).at(y - 6) = '3'; theGrid.at(x - 1).at(y - 6) = '6';
    // Center number
    theGrid.at(x + 6).at(y - 6) = '9'; theGrid.at(x + 7).at(y - 6) = '9';
    theGrid.at(x + 13).at(y - 6) = '6'; theGrid.at(x + 14).at(y - 6) = '3';
    // Eighth layer
    theGrid.at(x).at(y - 7) = '/'; 
    theGrid.at(x + 13).at(y - 7) = '\\';
    // Ninth layer
    theGrid.at(x).at(y - 8) = '|'; theGrid.at(x + 3).at(y - 8) = '|'; 
    theGrid.at(x + 4).at(y - 8) = '-'; theGrid.at(x + 5).at(y - 8) = '-'; 
    theGrid.at(x + 8).at(y - 8) = '-'; theGrid.at(x + 9).at(y - 8) = '-'; 
    theGrid.at(x + 10).at(y - 8) = '|'; theGrid.at(x + 13).at(y - 8) = '|';
}

int main() {
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

    while(true) {
        // Construct all the tiles by iterating through all 19 starting 
        // coordinates and inserting them into the grid.
        for (auto p : theCoordinates) {
            insertTileShape(p[0], p[1]);
        }

        // Output the entire grid to cout
        for (int i = 0; i < sizeVertical; ++i) {
            for (int j = 0; j < sizeHorizontal; ++j) {
                cout << theGrid.at(j).at(i);
            } 
            cout << endl;
        } 
        break;   
    }
} 
