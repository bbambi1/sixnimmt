# 6 nimmt! C++ Contest Framework

A complete C++ framework for implementing and competing with AI agents in the card game "6 nimmt!" (Take 6!).

## Game Rules

**6 nimmt!** is a strategic card game where players try to avoid collecting penalty cards:

- **Cards**: 104 cards numbered 1-104, each with 1-7 bull heads (penalty points)
- **Goal**: Have the fewest penalty points at the end
- **Setup**: Each player gets 10 cards, 4 cards start on the table in separate rows
- **Gameplay**: 
  - Players simultaneously choose and reveal one card
  - Cards are placed in ascending order after the closest lower card in any row
  - If a card can't be placed, the player must take a row (collecting all its penalty points)
  - When a row reaches 6 cards, the player who placed the 6th card takes the row
  - Game ends after 10 rounds

**Bull Head Distribution:**
- Card 55: 7 bull heads
- Multiples of 11: 5 bull heads  
- Multiples of 10: 3 bull heads
- Multiples of 5: 2 bull heads
- All others: 1 bull head

## Quick Start

### Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

### Running the Contest

```bash
./sixnimmt_contest
```

Choose between:
1. **Single Game**: Watch one game with verbose output
2. **Tournament**: Run multiple games and see statistics

### Testing

```bash
./test_game
```

## Creating Your Own Agent

To create a new AI agent, implement the `Player` interface:

```cpp
#include "game.h"

class MyAgent : public SixNimmt::Player {
public:
    void initialize(int playerId, int numPlayers) override {
        // Called once at the start of each game
    }
    
    int chooseCard(const SixNimmt::GameState& state) override {
        // Return the index (0-9) of the card to play from your hand
        // state.hand contains your 10 cards
        // state.rows contains the 4 rows on the table
        // state.scores contains current scores for all players
        
        return 0; // Play first card (example)
    }
    
    std::string getName() const override {
        return "MyAgent";
    }
};
```

### GameState Information

The `GameState` object provides all visible information:

```cpp
struct GameState {
    std::vector<std::vector<Card>> rows;  // 4 rows of cards on table
    std::vector<Card> hand;               // Your current hand (10 cards)
    int playerId;                         // Your player ID (0-based)
    int roundNumber;                      // Current round (1-10)
    std::vector<int> scores;              // Current scores for all players
    std::vector<Card> playedCards;        // Cards played this round
};
```

### Card Information

```cpp
struct Card {
    int number;      // Card number (1-104)
    int bullHeads;   // Penalty points (1-7)
};
```

## Example Strategies

### Random Agent
Plays cards randomly - useful as a baseline.

### Smart Agent
- Avoids cards that would force taking a row
- Prefers cards with fewer bull heads
- Includes 10% randomness to avoid predictability

### Advanced Strategies to Try
- **Risk Assessment**: Calculate the probability of taking a row
- **Opponent Modeling**: Track what cards other players have played
- **Endgame Strategy**: Different approach in later rounds
- **Card Counting**: Track which cards have been played
- **Row Management**: Try to force opponents to take high-penalty rows

## Contest Framework Features

### Tournament System
- Round-robin play between all agents
- Configurable games per matchup
- Win rate and average score statistics
- Detailed results display

### Game Engine
- Complete 6 nimmt! rule implementation
- Fair card dealing and shuffling
- Proper turn order and card placement
- Accurate scoring system

### Extensibility
- Easy to add new agents
- Modular design
- Clean separation of game logic and AI strategies

## File Structure

```
sixnimmt/
├── include/
│   └── game.h              # Core game definitions
├── src/
│   ├── game.cpp            # Game engine implementation
│   ├── random_agent.cpp    # Random strategy example
│   ├── smart_agent.cpp     # Basic strategy example
│   ├── contest.cpp         # Tournament framework
│   └── test_game.cpp       # Simple test program
├── CMakeLists.txt          # Build configuration
└── README.md              # This file
```

## Building Your Agent

1. Create a new `.cpp` file in the `src/` directory
2. Implement the `Player` interface
3. Add your agent to the contest in `contest.cpp`
4. Rebuild and test

Example addition to `contest.cpp`:
```cpp
// Add your agent
contest.addPlayer(std::make_unique<MyAgent>());
```

## Tips for Success

1. **Start Simple**: Begin with basic heuristics
2. **Analyze the Game**: Understand when you're forced to take rows
3. **Track Information**: Use the game state effectively
4. **Test Thoroughly**: Run many games to validate your strategy
5. **Learn from Losses**: Analyze what went wrong in bad games

## Advanced Features

The framework supports:
- Custom game lengths
- Verbose output for debugging
- Statistical analysis
- Multiple tournament formats
- Easy integration of new agents

Good luck with your 6 nimmt! AI strategy!
