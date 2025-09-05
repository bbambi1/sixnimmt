#include "game.h"
#include <algorithm>

namespace SixNimmt {

class BullsHeadsFirstAgent : public Player {
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
        // Find the card with the most bull heads in our hand
        int maxBullsIndex = 0;
        int maxBulls = hand[0].bullHeads;

        for (int i = 1; i < hand.size(); ++i) {
            if (hand[i].bullHeads > maxBulls) {
                maxBulls = hand[i].bullHeads;
                maxBullsIndex = i;
            }
        }

        return maxBullsIndex;
    }

    std::string getName() const override {
        return "BullsHeadsFirstAgent";
    }
};

} // namespace SixNimmt
