import numpy as np
import sys

# Load the Q-table
Q = np.load('blackjack_q_table.npy')

def get_action(player_total, dealer_card):
    action = np.argmax(Q[player_total][dealer_card])
    return "Hit" if action == 1 else "Stand"

if __name__ == "__main__":
    player_total = int(sys.argv[1])
    dealer_card = int(sys.argv[2])
    action = get_action(player_total, dealer_card)
    print(action)
