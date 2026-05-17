//
//  RoverController.cpp
//  rover-simulator
//
//  Created by Ryu Dae-ha on 5/17/26.
//

#include "RoverController.hpp"

RoverController::RoverController(Matrix2D& map, Rover& rover)
: map_(map), rover_(rover) {}

bool RoverController::canMoveTo(std::size_t r, std::size_t c) const {
    bool result = false;
    if (r < map_.rows() && c < map_.cols()) {
        result = true;
    }
    return result;
}

bool RoverController::canMove(Direction dir) const {
    auto pos = rover_.position();
    std::size_t r = pos.row;
    std::size_t c = pos.col;
    switch (dir) {
        case Direction::North: r -= 1; break;
        case Direction::South: r += 1; break;
        case Direction::West:  c -= 1; break;
        case Direction::East:  c += 1; break;
        default:                       break;
    }
    return canMoveTo(r, c);
}

void RoverController::applyMove(Direction dir) {
    rover_.move(dir);
}

bool RoverController::tryMove(Direction dir) {
    bool result = false;
    if (canMove(dir)) {
        applyMove(dir);
        result = true;
    }
    return result;
}
