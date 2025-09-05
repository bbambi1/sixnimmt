#include "game.h"
#include <algorithm>

namespace SixNimmt {

class HighestCardFirstAgent : public Player {
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
        // Find the card with the highest number in our hand
        int highestCardIndex = 0;
        int highestNumber = hand[0].number;

        for (int i = 1; i < hand.size(); ++i) {
            if (hand[i].number > highestNumber) {
                highestNumber = hand[i].number;
                highestCardIndex = i;
            }
        }

        return highestCardIndex;
    }

    std::string getName() const override {
        return "HighestCardFirstAgent";
    }
};

} // namespace SixNimmt
