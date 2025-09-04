#include "game.h"
#include "random_agent.cpp"
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <map>

namespace SixNimmt {

class Contest {
private:
    std::vector<std::unique_ptr<Player>> players;
    std::map<std::string, int> wins;
    std::map<std::string, int> totalScore;
    std::map<std::string, int> gamesPlayed;

public:
    void addPlayer(std::unique_ptr<Player> player) {
        players.push_back(std::move(player));
    }

    void runTournament(int gamesPerMatchup = 100) {
        std::cout << "Starting \"6 nimmt!\" Tournament" << std::endl;
        std::cout << "Players: " << players.size() << std::endl;
        std::cout << "Games per matchup: " << gamesPerMatchup << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        // Initialize statistics
        for (const auto& player : players) {
            wins[player->getName()] = 0;
            totalScore[player->getName()] = 0;
            gamesPlayed[player->getName()] = 0;
        }

        // Play round-robin tournament
        for (int i = 0; i < players.size(); ++i) {
            for (int j = i + 1; j < players.size(); ++j) {
                std::cout << "\n" << players[i]->getName() << " vs " << players[j]->getName() << std::endl;

                int player1Wins = 0;
                int player2Wins = 0;

                for (int gameNum = 0; gameNum < gamesPerMatchup; ++gameNum) {
                    std::vector<std::unique_ptr<Player>> gamePlayers;

                    // Might need a clone method
                    // For now, let's create new instances
                    if (players[i]->getName() == "RandomAgent") {
                        gamePlayers.push_back(std::make_unique<RandomAgent>());
                    }

                    if (players[j]->getName() == "RandomAgent") {
                        gamePlayers.push_back(std::make_unique<RandomAgent>());
                    }

                    // Initialize players (hands will be set by game)
                    gamePlayers[0]->initialize(0, 2, {});
                    gamePlayers[1]->initialize(1, 2, {});

                    // Play game
                    Game game(std::move(gamePlayers));
                    std::vector<int> scores = game.playGame(false);

                    // Update statistics
                    totalScore[players[i]->getName()] += scores[0];
                    totalScore[players[j]->getName()] += scores[1];
                    gamesPlayed[players[i]->getName()]++;
                    gamesPlayed[players[j]->getName()]++;

                    if (scores[0] < scores[1]) {
                        wins[players[i]->getName()]++;
                        player1Wins++;
                    } else if (scores[1] < scores[0]) {
                        wins[players[j]->getName()]++;
                        player2Wins++;
                    }
                    // Tie - no winner
                }

                std::cout << "  " << players[i]->getName() << ": " << player1Wins << " wins" << std::endl;
                std::cout << "  " << players[j]->getName() << ": " << player2Wins << " wins" << std::endl;
            }
        }

        printResults();
    }

    void runSingleGame(bool verbose = true) {
        if (players.empty()) {
            std::cout << "No players added!" << std::endl;
            return;
        }

        // Initialize all players (hands will be set by game)
        for (int i = 0; i < players.size(); ++i) {
            players[i]->initialize(i, players.size(), {});
        }

        // Play the game
        Game game(std::move(players));
        std::vector<int> scores = game.playGame(verbose);

        // Show results
        std::cout << "\n=== Final Results ===" << std::endl;
        std::vector<std::pair<std::string, int>> results;
        for (int i = 0; i < scores.size(); ++i) {
            results.emplace_back("Player " + std::to_string(i), scores[i]);
        }

        std::sort(results.begin(), results.end(), 
                 [](const auto& a, const auto& b) { return a.second < b.second; });
        
        for (int i = 0; i < results.size(); ++i) {
            std::cout << (i + 1) << ". " << results[i].first 
                     << ": " << results[i].second << " points" << std::endl;
        }
    }
    
private:
    void printResults() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "TOURNAMENT RESULTS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        // Sort by win rate
        std::vector<std::pair<std::string, double>> standings;
        for (const auto& player : players) {
            const std::string& name = player->getName();
            double winRate = (double)wins[name] / gamesPlayed[name] * 100.0;
            standings.emplace_back(name, winRate);
        }

        std::sort(standings.begin(), standings.end(), 
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::cout << std::left << std::setw(15) << "Player" 
                  << std::setw(10) << "Wins" 
                  << std::setw(12) << "Win Rate" 
                  << std::setw(12) << "Avg Score" << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        for (const auto& standing : standings) {
            const std::string& name = standing.first;
            std::cout << std::left << std::setw(15) << name
                      << std::setw(10) << wins[name]
                      << std::setw(12) << std::fixed << std::setprecision(1) 
                      << (double)wins[name] / gamesPlayed[name] * 100.0 << "%"
                      << std::setw(12) << std::fixed << std::setprecision(1)
                      << (double)totalScore[name] / gamesPlayed[name] << std::endl;
        }
    }
};

} // namespace SixNimmt

int main() {
    using namespace SixNimmt;

    Contest contest;

    // Add players
    contest.addPlayer(std::make_unique<RandomAgent>());

    std::cout << "\"6 nimmt!\" Contest Framework" << std::endl;
    std::cout << "1. Single game" << std::endl;
    std::cout << "2. Tournament" << std::endl;
    std::cout << "Choose (1 or 2): ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        contest.runSingleGame(true);
    } else if (choice == 2) {
        contest.runTournament(50); // 50 games per matchup
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }

    return 0;
}
