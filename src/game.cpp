#include "game.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace SixNimmt {

void Game::initializeDeck() {
    deck.clear();
    for (int i = 1; i <= 104; ++i) {
        deck.emplace_back(i);
    }

    // Shuffle the deck
    std::shuffle(deck.begin(), deck.end(), rng);
}

void Game::dealCards() {
    // Each player gets 10 cards
    int cardsPerPlayer = 10;
    int cardIndex = 0;
    for (int player = 0; player < players.size(); ++player) {
        std::vector<Card> playerHand;
        for (int card = 0; card < cardsPerPlayer; ++card) {
            playerHand.push_back(deck[cardIndex]);
            cardIndex++;
        }
        players[player]->initialize(player, players.size(), playerHand);
    }

    // Remove dealt cards from deck
    deck.erase(deck.begin(), deck.begin() + cardIndex);
}

void Game::initializeRows() {
    rows.clear();
    rows.resize(4);

    // Deal 4 starting cards, one to each row
    for (int i = 0; i < 4; ++i) {
        rows[i].push_back(deck[i]);
    }

    // Remove the 4 starting cards from deck
    deck.erase(deck.begin(), deck.begin() + 4);
}

void Game::playRound() {
    std::vector<std::pair<Card, int>> playedCards; // (card, playerId)

    GameState state = getGameState();
    for (int playerId = 0; playerId < players.size(); ++playerId) {
        int cardIndex = players[playerId]->chooseCard(state);
        assert(cardIndex >= 0 && cardIndex < static_cast<int>(players[playerId]->getHand().size()));

        // Store card and player ID, then remove from hand
        playedCards.emplace_back(players[playerId]->getHand()[cardIndex], playerId);
        players[playerId]->removeCard(cardIndex);
    }

    // Sort by card number and process
    std::sort(playedCards.begin(), playedCards.end());

    for (const auto& pair : playedCards) {
        processCard(pair.first, pair.second);
    }

    currentRound++;
}

void Game::processCard(const Card& card, int playerId) {
    int bestRow = findBestRow(card);

    if (bestRow == -1) {
        GameState state = getGameState();
        int rowToTake = players[playerId]->chooseRowToTake(state);

        assert(rowToTake >= 0 && rowToTake < 4);

        takeRow(playerId, rowToTake);
        rows[rowToTake] = {card};
    } else {
        rows[bestRow].push_back(card);

        if (rows[bestRow].size() == 6) {
            takeRow(playerId, bestRow);
            rows[bestRow] = {card};
        }
    }
}

int Game::findBestRow(const Card& card) const {
    int bestRow = -1;
    int bestDifference = 1000; // Large number

    for (int i = 0; i < 4; ++i) {
        if (!rows[i].empty()) {
            int lastCardNumber = rows[i].back().number;
            if (lastCardNumber < card.number) {
                int difference = card.number - lastCardNumber;
                if (difference < bestDifference) {
                    bestDifference = difference;
                    bestRow = i;
                }
            }
        }
    }

    return bestRow;
}

void Game::takeRow(int playerId, int rowIndex) {
    // Add penalty points for all cards in the row

    int penalty = 0;
    for (const Card& card : rows[rowIndex]) {
        penalty += card.bullHeads;
    }
    scores[playerId] += penalty;
}

void Game::printGameState() const {
    std::cout << "\n=== Game State ===" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "Row " << i << ": ";
        for (const Card& card : rows[i]) {
            std::cout << card.number << "(" << card.bullHeads << ") ";
        }
        std::cout << std::endl;
    }
    std::cout << "Scores: ";
    for (int i = 0; i < scores.size(); ++i) {
        std::cout << "P" << i << ":" << scores[i] << " ";
    }
    std::cout << std::endl;
}

Game::Game(std::vector<std::unique_ptr<Player>>&& players) 
    : players(std::move(players)), scores(this->players.size(), 0), currentRound(1), rng(std::random_device{}()) {
    assert(players.size() >= 2 && "Must have at least 2 players");
    assert(players.size() <= 10 && "Cannot have more than 10 players");

    initializeDeck();
    dealCards();
    initializeRows();
}

GameState Game::getGameState() const {
    GameState state;
    state.rows = rows;
    state.roundNumber = currentRound;
    state.scores = scores;
    return state;
}

std::vector<int> Game::playGame(bool verbose) {
    if (verbose) {
        std::cout << "Starting \"6 nimmt!\" game with " << players.size() << " players" << std::endl;
        printGameState();
    }

    // Play 10 rounds
    for (int round = 1; round <= 10; ++round) {
        if (verbose) {
            std::cout << "\n--- Round " << round << " ---" << std::endl;
        }
        playRound();
        if (verbose) {
            printGameState();
        }
    }

    if (verbose) {
        std::cout << "\n=== Final Scores ===" << std::endl;
        for (int i = 0; i < players.size(); ++i) {
            std::cout << players[i]->getName() << ": " << scores[i] << " points" << std::endl;
        }
    }

    return scores;
}


} // namespace SixNimmt
