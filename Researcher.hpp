#pragma once
#include "City.hpp"
#include "Color.hpp"
#include "Player.hpp"
#include "Board.hpp"

namespace pandemic{

    class Researcher: public Player {

        public:
        Player& fly_direct(City city) override;
    };

}