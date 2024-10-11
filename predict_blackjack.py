import sys
import joblib

def predict_action(player_total, dealer_card):
    model = joblib.load('blackjack_model.pkl')
    prediction = model.predict([[player_total, dealer_card]])
    action = 'Hit' if prediction[0] == 1 else 'Stand'
    print(action)

if __name__ == "__main__":
    player_total = int(sys.argv[1])
    dealer_card = int(sys.argv[2])
    predict_action(player_total, dealer_card)
