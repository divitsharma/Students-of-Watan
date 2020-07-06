#ifndef DISPLAY_H
#define DISPLAY_H

#include "board.h"
#include <memory>

class Display;
typedef std::shared_ptr<Display> display_ptr_t;

class Display {
public:
    virtual void draw(const Board& b, int geese) = 0;
};


#endif
