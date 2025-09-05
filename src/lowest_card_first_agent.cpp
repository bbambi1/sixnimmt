#include "game.h"
#include <algorithm>

namespace SixNimmt {

class LowestCardFirstAgent : public Player {
private:
    int playerId;
    int numPlayers;

public:
    void initialize(int playerId, int numPlayers, const std::vector<Card>& initialHand) override {
        this->playerId = playerId;
        this->numPlayers = numPlayers;
        this->hand = initialHand;
    }

    int chooseCard(const GameState& state) override {
        // Find the card with the lowest number in our hand
        int lowestCardIndex = 0;
        int lowestNumber = hand[0].number;

        for (int i = 1; i < hand.size(); ++i) {
            if (hand[i].number < lowestNumber) {
                lowestNumber = hand[i].number;
                lowestCardIndex = i;
            }
        }

        return lowestCardIndex;
    }

    std::string getName() const override {
        return "LowestCardFirstAgent";
    }
};

} // namespace SixNimmt
