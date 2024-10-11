#include <iostream>

// Function prototypes (from blackjack.cpp)
void initializeDeck();
void shuffleDeck();
void blackjack();

int main() {
    std::cout << "Welcome to Blackjack!" << std::endl;
    initializeDeck();
    shuffleDeck();
    blackjack();
    return 0;
}
