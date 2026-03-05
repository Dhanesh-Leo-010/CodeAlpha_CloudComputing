from flask import Flask, render_template, request, jsonify
import json
import random
import nltk
nltk.download('punkt')
from nltk.tokenize import word_tokenize

app = Flask(__name__)

# Load intents
with open('intents.json', encoding='utf-8') as file:
    data = json.load(file)

# Function to get response
def chatbot_response(user_input):
    user_words = word_tokenize(user_input.lower())
    best_match = None
    max_matches = 0

    for intent in data["intents"]:
        for pattern in intent["patterns"]:
            pattern_words = word_tokenize(pattern.lower())

            # Count meaningful matches
            matches = len(set(user_words) & set(pattern_words))

            if matches > max_matches:
                max_matches = matches
                best_match = intent

    if best_match and max_matches > 0:
        return random.choice(best_match["responses"])

    return random.choice(
        next(intent["responses"] for intent in data["intents"] if intent["tag"] == "fallback")
    )
@app.route("/")
def home():
    return render_template("index.html")

@app.route("/get", methods=["POST"])
def chatbot_response():
    sentence = request.form.get("msg")

    if not sentence:
        return "Please enter a message."

    sentence = sentence.lower()

    response = chatbot_response(sentence)
    return response
if __name__ == "__main__":
    app.run(debug=True)
    