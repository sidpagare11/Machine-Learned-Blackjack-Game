import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import joblib

def retrain_model():
    # Load the historical game data
    df = pd.read_csv('game_data.csv')
    df['Label'] = df['Outcome'].apply(lambda x: 1 if x == 'Win' else 0)

    X = df[['PlayerTotal', 'DealerCard']]
    y = df['Label']

    model = DecisionTreeClassifier()
    model.fit(X, y)

    # Save the updated model
    joblib.dump(model, 'blackjack_model.pkl')
    print("Model retrained and saved.")

if __name__ == "__main__":
    retrain_model()
