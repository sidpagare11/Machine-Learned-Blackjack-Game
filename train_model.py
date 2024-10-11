import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
import joblib

def generate_data(num_samples=10000):
    data = []
    for _ in range(num_samples):
        player_total = np.random.randint(4, 22)
        dealer_card = np.random.randint(2, 12)
        action = np.random.choice(['Hit', 'Stand'])

        if player_total <= 11:
            outcome = 'Hit'
        elif player_total >= 17:
            outcome = 'Stand'
        else:
            outcome = action
        
        data.append([player_total, dealer_card, outcome])
    
    df = pd.DataFrame(data, columns=['PlayerTotal', 'DealerCard', 'Outcome'])
    return df

data = generate_data()
data.to_csv('blackjack_data.csv', index=False)

df = pd.read_csv('blackjack_data.csv')
df['Label'] = df['Outcome'].apply(lambda x: 1 if x == 'Hit' else 0)

X = df[['PlayerTotal', 'DealerCard']]
y = df['Label']

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

model = DecisionTreeClassifier()
model.fit(X_train, y_train)

y_pred = model.predict(X_test)
print(f"Model Accuracy: {accuracy_score(y_test, y_pred) * 100:.2f}%")

joblib.dump(model, 'blackjack_model.pkl')
print("Model saved to blackjack_model.pkl")
