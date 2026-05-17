//
//  RoverController.hpp
//  rover-simulator
//
//  Created by Ryu Dae-ha on 5/17/26.
//

#pragma once

#include "Matrix2D.hpp"
#include "Rover.hpp"

class RoverController {
public:
    RoverController(Matrix2D& map, Rover& rover);

    bool tryMove(Direction dir);
    bool canMove(Direction dir) const;

private:
    Matrix2D& map_;
    Rover& rover_;

    bool canMoveTo(std::size_t r, std::size_t c) const;
    void applyMove(Direction dir);
};

