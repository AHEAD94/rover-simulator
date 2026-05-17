//
//  Rover.hpp
//  rover-simulator
//
//  Created by Ryu Dae-ha on 5/17/26.
//

#pragma once

#include "Direction.hpp"

struct POSITION {
    std::size_t row;
    std::size_t col;
};

class Rover {
public:
    Rover() : row(0), col(0) {}
    
    Rover(std::size_t r, std::size_t c) : row(r), col(c) {}
    
    POSITION position() const { return {row, col}; }
    
    void move(Direction dir) {
        switch (dir) {
            case Direction::North: row -= 1; break;
            case Direction::South: row += 1; break;
            case Direction::West:  col -= 1; break;
            case Direction::East:  col += 1; break;
            default:                         break;
        }
    }
    
private:
    std::size_t row;
    std::size_t col;
};
