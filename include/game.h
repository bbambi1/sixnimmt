#pragma once

#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <algorithm>
#include <cassert>

namespace SixNimmt {

// Card representation: number (1-104) with bull heads (1-7)
struct Card {
    int number;
    int bullHeads;

    Card(int num) : number(num) {
        // Calculate bull heads based on game rules
        if (num == 55) bullHeads = 7;
        else if (num % 11 == 0) bullHeads = 5;
        else if (num % 10 == 0) bullHeads = 3;
        else if (num % 5 == 0) bullHeads = 2;
        else bullHeads = 1;
    }

    bool operator<(const Card& other) const {
        return number < other.number;
    }
};

struct GameState {
    std::vector<std::vector<Card>> rows;  // 4 rows of cards on table
    int roundNumber;                      // Current round (1-10)
    std::vector<int> scores;              // Current scores for all players
};

// Abstract base class for all player agents
class Player {
protected:
    std::vector<Card> hand;
    int playerId;
    int numPlayers;

    std::mt19937 rng;

    int choseRandowRowToTake(const GameState& state) {
        std::uniform_int_distribution<int> dist(0, 3);
        return dist(rng);
    }

    int choseLowestPenaltyRowToTake(const GameState& state) {
        int bestRow = 0;
        int minPenalty = calculateRowPenalty(state.rows[0]);

        for (int i = 1; i < 4; ++i) {
            int penalty = calculateRowPenalty(state.rows[i]);
            if (penalty < minPenalty) {
                minPenalty = penalty;
                bestRow = i;
            }
        }

        return bestRow;
    }

    int choseHighestPenaltyRowToTake(const GameState& state) {
        int bestRow = 0;
        int maxPenalty = calculateRowPenalty(state.rows[0]);

        for (int i = 1; i < 4; ++i) {
            int penalty = calculateRowPenalty(state.rows[i]);
            if (penalty > maxPenalty) {
                maxPenalty = penalty;
                bestRow = i;
            }
        }

        return bestRow;
    }

    int choseLowestLastCardRowToTake(const GameState& state) {
        int bestRow = 0;
        int minLastCard = state.rows[0].back().number;

        for (int i = 1; i < 4; ++i) {
            if (!state.rows[i].empty() && state.rows[i].back().number < minLastCard) {
                minLastCard = state.rows[i].back().number;
                bestRow = i;
            }
        }

        return bestRow;
    }

    int choseHighestLastCardRowToTake(const GameState& state) {
        int bestRow = 0;
        int maxLastCard = state.rows[0].back().number;
        
        for (int i = 1; i < 4; ++i) {
            if (!state.rows[i].empty() && state.rows[i].back().number > maxLastCard) {
                maxLastCard = state.rows[i].back().number;
                bestRow = i;
            }
        }

        return bestRow;
    }

    int choseFewestCardsRowToTake(const GameState& state) {
        int bestRow = 0;
        int minCards = state.rows[0].size();

        for (int i = 1; i < 4; ++i) {
            if (state.rows[i].size() < minCards) {
                minCards = state.rows[i].size();
                bestRow = i;
            }
        }

        return bestRow;
    }

    int choseMostCardsRowToTake(const GameState& state) {
        int bestRow = 0;
        int maxCards = state.rows[0].size();

        for (int i = 1; i < 4; ++i) {
            if (state.rows[i].size() > maxCards) {
                maxCards = state.rows[i].size();
                bestRow = i;
            }
        }

        return bestRow;
    }

    int calculateRowPenalty(const std::vector<Card>& row) {
        int penalty = 0;
        for (const Card& card : row) {
            penalty += card.bullHeads;
        }
        return penalty;
    }

public:
    virtual ~Player() = default;

    // Called at the start of each game - player receives their hand
    virtual void initialize(int playerId, int numPlayers, const std::vector<Card>& initialHand) = 0;

    // Called each round to choose a card to play
    // Return the index of the card to play from your hand
    virtual int chooseCard(const GameState& state) = 0;

    // Called when player must take a row (when their card can't be placed)
    // Return the index (0-3) of the row to take
    // Default implementation chooses the row with highest last card
    virtual int chooseRowToTake(const GameState& state) {
        return choseLowestPenaltyRowToTake(state);
    }

    virtual std::string getName() const = 0;

    const std::vector<Card>& getHand() const { return hand; }

    void removeCard(int index) {
        hand.erase(hand.begin() + index);
    }
};

// Main game engine
class Game {
private:
    std::vector<std::unique_ptr<Player>> players;
    std::vector<Card> deck;
    std::vector<std::vector<Card>> rows;
    std::vector<int> scores;
    int currentRound;
    std::mt19937 rng;

    void initializeDeck();
    void dealCards();
    void initializeRows();
    void playRound();
    void processCard(const Card& card, int playerId);
    int findBestRow(const Card& card) const;
    void takeRow(int playerId, int rowIndex);
    void printGameState() const;

public:
    Game(std::vector<std::unique_ptr<Player>>&& players);

    // Run a complete game and return final scores
    std::vector<int> playGame(bool verbose = false);

    GameState getGameState() const;

    // Public access to scores for debugging
    const std::vector<int>& getScores() const { return scores; }
};

} // namespace SixNimmt
