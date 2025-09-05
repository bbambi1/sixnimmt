#include "game.h"
#include "random_agent.cpp"
#include "lowest_card_first_agent.cpp"
#include "highest_card_first_agent.cpp"
#include "bulls_heads_first_agent.cpp"
#include <iostream>

int main() {
    using namespace SixNimmt;

    std::cout << "Testing \"6 nimmt!\" game engine" << std::endl;

    // Create a 3-player game with different strategies
    std::vector<std::unique_ptr<Player>> players;
    players.push_back(std::make_unique<RandomAgent>());
    players.push_back(std::make_unique<LowestCardFirstAgent>());
    players.push_back(std::make_unique<HighestCardFirstAgent>());
    players.push_back(std::make_unique<BullsHeadsFirstAgent>());

    // Initialize players (hands will be set by game)
    players[0]->initialize(0, 4, {});
    players[1]->initialize(1, 4, {});
    players[2]->initialize(2, 4, {});
    players[3]->initialize(3, 4, {});

    // Create and play game
    Game game(std::move(players));
    std::vector<int> scores = game.playGame(true);

    std::cout << "\nTest completed successfully!" << std::endl;
    return 0;
}
