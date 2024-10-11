import numpy as np
import random

# Initialize Q-table with zeros (assuming player sum ranges from 4 to 21, and dealer card from 2 to 11)
Q = np.zeros((22, 12, 2))  # Q[player_total][dealer_card][action] -> action: 0 = Stand, 1 = Hit

# Hyperparameters
alpha = 0.1  # Learning rate
gamma = 0.9  # Discount factor
epsilon = 0.1  # Exploration rate

def choose_action(player_total, dealer_card):
    if random.uniform(0, 1) < epsilon:
        return random.choice([0, 1])  # Explore: randomly choose an action
    else:
        return np.argmax(Q[player_total - 4][dealer_card - 2])  # Exploit: choose the best-known action

def update_q_table(state, action, reward, next_state):
    player_total, dealer_card = state
    next_player_total, next_dealer_card = next_state
    
    # Ensure the indices are within the bounds of the Q-table
    player_total = min(max(player_total, 4), 21)
    dealer_card = min(max(dealer_card, 2), 11)
    next_player_total = min(max(next_player_total, 4), 21)
    next_dealer_card = min(max(next_dealer_card, 2), 11)
    
    # Adjust indices to match Q-table dimensions
    player_total_index = player_total - 4
    dealer_card_index = dealer_card - 2
    next_player_index = next_player_total - 4
    next_dealer_index = next_dealer_card - 2

    old_value = Q[player_total_index][dealer_card_index][action]
    next_max = np.max(Q[next_player_index][next_dealer_index])
    
    # Q-learning formula
    Q[player_total_index][dealer_card_index][action] = old_value + alpha * (reward + gamma * next_max - old_value)

def simulate_game():
    # Initialize state
    player_total = random.randint(4, 21)
    dealer_card = random.randint(2, 11)
    
    state = (player_total, dealer_card)
    done = False
    while not done:
        action = choose_action(player_total, dealer_card)
        
        if action == 1:  # Hit
            new_card = random.randint(2, 11)  # In reality, this should come from the game logic
            player_total += new_card
            
            if player_total > 21:  # Bust
                reward = -1
                done = True
            else:
                reward = 0  # Neutral, continue playing
        else:  # Stand
            dealer_total = random.randint(17, 21)  # Simulated dealer behavior
            if dealer_total > 21 or player_total > dealer_total:
                reward = 1  # Win
            elif player_total == dealer_total:
                reward = 0  # Draw
            else:
                reward = -1  # Loss
            done = True
        
        # Update Q-table
        next_state = (player_total, dealer_card)
        update_q_table(state, action, reward, next_state)
        state = next_state

# Train the model by simulating multiple games
for _ in range(10000):
    simulate_game()

# Save the Q-table for later use
np.save('blackjack_q_table.npy', Q)
