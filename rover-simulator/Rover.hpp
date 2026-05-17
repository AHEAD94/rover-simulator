//
//  Rover.hpp
//  rover-simulator
//
//  Created by Ryu Dae-ha on 5/17/26.
//

enum DIRECTIONS {
    NS = 0,
    NORTH,
    SOUTH,
    WEST,
    EAST
};

struct POSITION {
    std::size_t row;
    std::size_t col;
};

class Rover {
public:
    Rover() : row(0), col(0) {}
    
    Rover(std::size_t r, std::size_t c) : row(r), col(c) {}
    
    POSITION position() const { return {row, col}; }
    
    void move(const int dir) {
        switch (dir) {
            case NORTH: row -= 1; break;
            case SOUTH: row += 1; break;
            case WEST:  col -= 1; break;
            case EAST:  col += 1; break;
            default:              break;
        }
    }
    
private:
    std::size_t row;
    std::size_t col;
};
