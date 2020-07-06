#include "../include/RegularHex.h"
#include <iostream>
using namespace std;

// vector<vector<int> > VertexToVertex {{1,3}, {0,4}, {3,7}, {2,8,0}, {1,9,5},
//                                     {4,10}, {7,12}, {6,2,13}, {3,14,9}, {8,4,15},
//                                     {5,16,11}, {10,17}, {6,18}, {7,19,14}, {13,8,20},
//                                     {9,21,16}, {15,10,22}, {11,23}, {12,24,19}, {18,13,25},
//                                     {14,26,21}, {20,15,27}, {16,28,23}, {22,17,29}, {18,30},
//                                     {19,31,26}, {25,20,32}, {21,33,28}, {27,23,34}, {23,35},
//                                     {24,36,31}, {30,25,37}, {26,38,33}, {32,27,39}, {28,40,35},
//                                     {34,29,41}, {30,42}, {31,43,38}, {37,32,44}, {33,45,40},
//                                     {39,34,46}, {35,47}, {36,43}, {42,37,48}, {38,49,45},
//                                     {44,39,50}, {40,51,47}, {46,41}, {43,49}, {48,44,52},
//                                     {45,53,51}, {50,46}, {49,53}, {52,40}};
vector<vector<int> > TileToVertex {{0,1,3,4,8,9}, {2,3,7,8,13,14}, {4,5,9,10,15,16},
                                  {6,7,12,13,18,19}, {8,9,14,15,20,21}, {10,11,16,17,22,23},
                                  {13,14,19,20,25,26}, {15,16,21,22,27,28}, {18,19,24,25,30,31},
                                  {20,21,26,27,32,33}, {22,23,28,29,34,35}, {25,26,31,32,37,38},
                                  {27,28,33,34,39,40}, {30,31,36,37,42,43}, {32,33,38,39,44,45},
                                  {34,35,40,41,46,47}, {37,38,43,44,48,49}, {39,40,45,46,50,51},
                                  {44,45,49,50,52,53}};
vector<vector<int> > EdgeToVertex {{0,1}, {0,3}, {1,4}, {2,3}, {4,5}, {2,7}, {3,8}, {4,9}, {5,10},
                                   {6,7}, {8,9}, {10,11}, {6,12}, {7,13}, {14,8}, {9,15}, {10,16},
                                   {11,17}, {13,14}, {15,16}, {12,18}, {13,19}, {14,20}, {15,21},
                                   {16,22}, {17,23}, {18,19}, {20,21}, {22,23}, {18,24}, {19,25},
                                   {20,26}, {21,27}, {22,28}, {23,29}, {25,26}, {27,28}, {24,30},
                                   {25,31}, {26,32}, {27,33}, {28,34}, {29,35}, {30,31}, {32,33},
                                   {34,35}, {30,36}, {31,37}, {32,38}, {33,39}, {34,40}, {35,41},
                                   {37,38}, {39,40}, {36,42}, {37,43}, {38,44}, {39,45}, {40,46},
                                   {41,47}, {42,43}, {44,45}, {46,47}, {43,48}, {44,49}, {45,50},
                                   {46,51}, {48,49}, {50,51}, {49,52}, {50,53}, {52,53}};
vector<vector<int> > VertexToEdge
  { {1,0}, {0,2}, {5,3}, {3,6,1}, {2,7,4},
    {4,8}, {12,9}, {9,5,13}, {6,14,10}, {10,7,15},
    {8,16,11}, {11,17}, {12,20}, {13,21,18}, {18,14,22},
    {15,23,19}, {19,16,24}, {17,25}, {20,29,26}, {21,26,30},
    {22,31,27}, {23,27,32}, {24,33,28}, {25,28,34}, {29,37},
    {30,38,35}, {31,35,39}, {32,40,36}, {33,36,41}, {34,42},
    {37,46,43}, {38,43,47}, {39,48,44}, {40,44,49}, {41,50,45},
    {42,45,51}, {46,54}, {47,55,52}, {48,52,56}, {49,57,53},
    {50,53,58}, {51,59}, {54,60}, {55,60,63}, {56,64,61},
    {57,61,65}, {58,66,62}, {59,62}, {63,67}, {64,67,69},
    {65,70,68}, {66,68}, {69,71}, {70,71} };

vector<vector<int>> EdgeToEdge;
vector<vector<int>> VertexToVertex;

// vector<vector<int> > EdgeToEdge {{1,2}, {0,3,6}, {0,7,4}, {5,6,1}, {2,7,8}, {3,9,13}, {3,1,14,10}, {2,4,10,15}, {4,16,11}, {}};


RegularHex::RegularHex() : GameBoard(19, 54, 72) {
  intializeEEAndVV();

  // std::cout << VertexToVertex.size() << '\n';

  // for (size_t i = 0; i < NUM_EDGES; i++) {
  //   std::cout << i << ": ";
  //   for (auto &es : EdgeToEdge.at(i)){
  //     std::cout << es << " ";
  //   }
  //
  //     std::cout << '\n';
  // }
};



/* =========== Initializing V to V and E to E ===========  */

vector<int> adjVertices(int vertex) {
  vector<int> adjVertices;

  for (int adjE : VertexToEdge.at(vertex)) {
    for (int adjV : EdgeToVertex.at(adjE)) {
      if (adjV != vertex) adjVertices.push_back(adjV);
    }
  }

  return adjVertices;
}

vector<int> adjEdges(int edge) {
  /* Strategy:
   * for each vertex the edge is attached to,
   * find all edges that vertex is attached to,
   * and for each of those edges that is not the current edge,
   * attach it to the list of adjacent edges.
   */
   vector<int> adjEdges;

   for (int adjV : EdgeToVertex.at(edge)) {
     for (int adjE : VertexToEdge.at(adjV)) {
       if (adjE != edge) adjEdges.push_back(adjE);
     }
   }

   return adjEdges;
}

void RegularHex::intializeEEAndVV() {
  // for each edge/vertex, find its adjacent edges/vertices
  for (size_t edge = 0; edge < NUM_EDGES; edge++) {
    EdgeToEdge.push_back(adjEdges(edge));
  }
  for (size_t vertex = 0; vertex < NUM_VERTICES; vertex++) {
    VertexToVertex.push_back(adjVertices(vertex));
  }
}



/* =========== Getting Adjacencies ===========  */

const std::vector<int> &RegularHex::verticesByTile(int tileNum) const {
    return TileToVertex.at(tileNum);
}

const std::vector<int> &RegularHex::adjacentVerticesToVertex(int vertexNum) const {
  return VertexToVertex.at(vertexNum);
}

const std::vector<int> &RegularHex::adjacentVerticesToEdge(int edgeNum) const {
    return EdgeToVertex.at(edgeNum);
}

const std::vector<int> &RegularHex::adjacentEdgesToVertex(int vertexNum) const {
    return VertexToEdge.at(vertexNum);
}

const std::vector<int> &RegularHex::adjacentEdgesToEdge(int edgeNum) const {
    // Do some magic here to make this happen
    return EdgeToEdge.at(edgeNum);
}
