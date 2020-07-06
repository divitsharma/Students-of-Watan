#ifndef COLOURHEXTEXTDISPLAY_H
#define COLOURHEXTEXTDISPLAY_H

#include "display.h"
#include "VertexInfo.h"
#include "TileInfo.h"
#include "EdgeInfo.h"
#include "board.h"
#include "colour.h"

#include <vector>

class ColourHexTextDisplay : public Display {
//   int numVertex = 54;
//   int numEdge = 72;
//   int numTile = 19;

  int sizeHorizontal = 84;
  int sizeVertical = 42;
  int startx = 35;
  int starty = 40;

  std::vector<std::vector<char> > theGrid;

  // Using hexagonal coordinates
  std::vector<std::vector<int> > theCoordinates {{startx,starty},  // 18
                                      {startx + 15, starty - 4}, // 17
                                      {startx - 15, starty - 4}, // 16
                                      {startx + 30, starty - 8}, // 15
                                      {startx, starty - 8}, // 14
                                      {startx - 30, starty - 8}, // 13
                                      {startx + 15, starty - 12}, // 12
                                      {startx - 15, starty - 12}, // 11
                                      {startx + 30, starty - 16}, // 10
                                      {startx, starty - 16}, // 9
                                      {startx - 30, starty - 16}, // 8
                                      {startx + 15, starty - 20}, // 7
                                      {startx - 15, starty - 20}, // 6
                                      {startx + 30, starty - 24}, // 5
                                      {startx, starty - 24}, // 4
                                      {startx - 30, starty - 24}, // 3
                                      {startx + 15, starty - 28}, // 2
                                      {startx - 15, starty - 28}, // 1
                                      {startx, starty - 32}}; // 0

  std::vector<std::vector<int> > theVertexCoordinates {{36, 0}, {46, 0}, {21,4},
                                              {31, 4}, {51, 4}, {61, 4},
                                              {6, 8}, {16, 8}, {36, 8},
                                              {46, 8}, {66, 8}, {76, 8},
                                              {1, 12}, {21, 12}, {31, 12},
                                              {51, 12}, {61, 12}, {81, 12},
                                              {6, 16}, {16, 16}, {36, 16},
                                              {46, 16}, {66, 16}, {76, 16},
                                              {1, 20}, {21, 20}, {31, 20},
                                              {51, 20}, {61, 20}, {81, 20},
                                              {6, 24}, {16, 24}, {36, 24},
                                              {46, 24}, {66, 24}, {76, 24},
                                              {1, 28}, {21, 28}, {31, 28},
                                              {51, 28}, {61, 28}, {81, 28},
                                              {6, 32}, {16, 32}, {36, 32},
                                              {46, 32}, {66, 32}, {76, 32},
                                              {21, 36}, {31, 36}, {51, 36},
                                              {61, 36}, {36, 40}, {46, 40}};

  std::vector<std::vector<int> > theEdgeCoordinates {{41, 0}, {33, 2}, {48, 2}, {26, 4}, {56, 4},
                                          {18, 6}, {33, 6}, {48, 6}, {63, 6}, {11, 8},
                                          {41, 8}, {71, 8}, {3, 10}, {18, 10}, {33, 10},
                                          {48, 10}, {63, 10}, {78, 10}, {26, 12}, {56, 12},
                                          {3, 14}, {18, 14}, {33, 14}, {48, 14}, {63, 14},
                                          {78, 14}, {11, 16}, {41, 16}, {71, 16}, {3, 18},
                                          {18, 18}, {33, 18}, {48, 18}, {63, 18}, {78, 18},
                                          {26, 20}, {56, 20}, {3, 22}, {18, 22}, {33, 22},
                                          {48, 22}, {63, 22}, {78, 22}, {11, 24}, {41, 24},
                                          {71, 24}, {3, 26}, {18, 26}, {33, 26}, {48, 26},
                                          {63, 26}, {78, 26}, {26, 28}, {56, 28}, {3, 30},
                                          {18, 30}, {33, 30}, {48, 30}, {63, 30}, {78, 30},
                                          {11, 32}, {41, 32}, {71, 32}, {18, 34}, {33, 34},
                                          {48, 34}, {63, 34}, {26, 36}, {56, 36}, {33, 38},
                                          {48, 38}, {41, 40}};


    // std::vector<std::vector<int> > theCoordinates;
    // std::vector<std::vector<char> > theGrid;

    void insertTileShape(int x, int y);
    void insertVertex(VertexInfo info, int x);
    void insertTile(TileInfo info, int x);
    void insertEdge(EdgeInfo info, int x);
    void insertEmptyTile(std::vector<int>, int);
    void insertEmptyVertex(std::vector<int> coordinate, int index);
    void insertEmptyEdge(std::vector<int>, int);
    void insertGeese(int x, int y);

public:

    ColourHexTextDisplay();
    void draw(const Board &b, int geese) override;

};


#endif
