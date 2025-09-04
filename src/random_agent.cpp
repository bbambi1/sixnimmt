#include "game.h"
#include <random>
#include <chrono>

namespace SixNimmt {

class RandomAgent : public Player {
private:
    std::mt19937 rng;
    int playerId;
    int numPlayers;

public:
    RandomAgent() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    void initialize(int playerId, int numPlayers, const std::vector<Card>& initialHand) override {
        this->playerId = playerId;
        this->numPlayers = numPlayers;
        this->hand = initialHand;
    }

    int chooseCard(const GameState& state) override {
        // Randomly choose a card from our own hand
        std::uniform_int_distribution<int> dist(0, hand.size() - 1);
        return dist(rng);
    }

    std::string getName() const override {
        return "RandomAgent";
    }
};

} // namespace SixNimmt
