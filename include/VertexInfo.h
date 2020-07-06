#ifndef VERTEX_INFO_H
#define VERTEX_INFO_H

#include <string>

enum CriteriaType {NONE, ASSIGNMENT, MIDTERM, FINAL};

struct VertexInfo {
    char colour;
    CriteriaType type;
};


#endif
