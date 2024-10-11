#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

// Structure cards with rank and suite
struct Card {
    string rank;
    string suit;
    int value;
};

// Vector card object
vector<Card> deck;

// Functions
void initializeDeck();
void shuffleDeck();
Card dealCard();
void displayHand(const vector<Card>& hand, string name);
int calculateHand(const vector<Card>& hand);
string getQTableSuggestion(int playerTotal, int dealerCard);
void logGameData(int playerTotal, int dealerCard, string action, string outcome);
void blackjack();



// Function that initalizes deck with 52 cards
void initializeDeck() {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 0; rank < 13; rank++) {
            Card card;
            card.rank = ranks[rank];
            card.suit = suits[suit];
            card.value = values[rank];
            deck.push_back(card);
        }
    }
}

// Function to shuffle the deck
void shuffleDeck() {
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < deck.size(); i++) {
        int randomIndex = rand() % deck.size();
        swap(deck[i], deck[randomIndex]);
    }
}

// Function to deal card from deck
Card dealCard() {
    Card card = deck.back();
    deck.pop_back();
    return card;
}

// Function to display current hand
void displayHand(const vector<Card>& hand, string name) {
    cout << name << "'s Hand: ";
    for (const Card& card : hand) {
        cout << card.rank << " of " << card.suit << " ";
    }
    cout << endl;
}

// Function to calculate the current value of hand
int calculateHand(const vector<Card>& hand) {
    int sum = 0;
    int aceCount = 0;

    for (const Card& card : hand) {
        sum += card.value;
        if (card.rank == "Ace") aceCount++;
    }

    // Adjust for Aces (if sum is greater than 21, make Ace worth 1 instead of 11)
    while (sum > 21 && aceCount > 0) {
        sum -= 10;
        aceCount--;
    }

    return sum;
}

// Function to call the Python script for Q-table suggestions
string getQTableSuggestion(int playerTotal, int dealerCard) {
    string command = "python3 python/get_qtable_suggestion.py " + to_string(playerTotal) + " " + to_string(dealerCard) + " > result.txt";
    system(command.c_str());

    // Read the results from the file
    string result;
    ifstream resultFile("result.txt");
    if (resultFile.is_open()) {
        getline(resultFile, result);
        resultFile.close();
    } else {
        cerr << "Error: Unable to open result file." << endl;
    }

    return result;
}

// Function to log the game data
void logGameData(int playerTotal, int dealerCard, string action, string outcome) {
    ofstream logFile("python/game_data.csv", ios::app);
    if (logFile.is_open()) {
        logFile << playerTotal << "," << dealerCard << "," << action << "," << outcome << "\n";
        logFile.close();
    } else {
        cerr << "Error: Unable to open log file." << endl;
    }
}

// Game logic for player vs dealer
void blackjack() {
    vector<Card> playerHand;
    vector<Card> dealerHand;

    // Deal two cards each to player and dealer
    playerHand.push_back(dealCard());
    playerHand.push_back(dealCard());
    dealerHand.push_back(dealCard());
    dealerHand.push_back(dealCard());

    displayHand(playerHand, "Player");
    cout << "Dealer's Hand: " << dealerHand[0].rank << " of " << dealerHand[0].suit << " ?" << endl;

    // Get the dealer's visible card
    int dealerVisible = dealerHand[0].value;
    char choice;

    // Declare playerSum outside the loop so it's accessible throughout the function
    int playerSum;
    bool playerBusted = false;

    // Player's turn
    while (true) {
      
        // Calculate player's total
        playerSum = calculateHand(playerHand);

        // Get model's recommendation based on the current state of the player's hand
        string modelSuggestion = getQTableSuggestion(playerSum, dealerVisible);
        cout << "Model suggests: " << modelSuggestion << endl;

        // Display the player's hand and current total
        displayHand(playerHand, "Player");
        cout << "Your current total is: " << playerSum << endl;

        // Check if player already busted
        if (playerSum > 21) {
            cout << "You busted with " << playerSum << "! Dealer wins!" << endl;
            logGameData(playerSum, dealerVisible, "Bust", "Loss");
            playerBusted = true;
            return;
        }

        // Prompt the player for action
        cout << "Do you want to (H)it or (S)tand? ";
        cin >> choice;

        if (choice == 'H' || choice == 'h') {
            playerHand.push_back(dealCard());
            logGameData(playerSum, dealerVisible, "Hit", "In Progress");
        } else if (choice == 'S' || choice == 's') {
            // Break the loop if the player chooses to stand
            logGameData(playerSum, dealerVisible, "Stand", "In Progress");
            break;
        } else {
            cout << "Invalid choice. Please enter 'H' for Hit or 'S' for Stand." << endl;
        }
    }

    // Dealer's turn (dealer hits until their hand is 17 or higher, most effective yet safe strategy)
    while (calculateHand(dealerHand) < 17) {
        dealerHand.push_back(dealCard());
    }

    int dealerSum = calculateHand(dealerHand);
    displayHand(dealerHand, "Dealer");
    cout << "Dealer's total is: " << dealerSum << endl;

    // Outcomes to determine the winner
    string outcome;
    if (dealerSum > 21) {
        cout << "Dealer busted! You win!" << endl;
        outcome = "Win";
    } else if (dealerSum > playerSum) {
        cout << "Dealer wins!" << endl;
        outcome = "Loss";
    } else if (playerSum > dealerSum) {
        cout << "You win!" << endl;
        outcome = "Win";
    } else {
        cout << "It's a tie!" << endl;
        outcome = "Draw";
    }

    if (!playerBusted) {
        logGameData(playerSum, dealerVisible, "Stand", outcome);
    }
}
