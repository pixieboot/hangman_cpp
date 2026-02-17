#include "random.h"
#include <algorithm>
#include <iostream>

namespace WordList {
/**
 * Vector of words, words can be added or deleted as desired.
 */
std::vector<std::string_view> words{"mystery", "broccoli",  "account",
                                    "almost",  "spaghetti", "opinion",
                                    "moon",    "something", "galaxy"};

/**
 * Randomizes which word will be picked from the array of words,
 * randomizer taken from learncpp.com "Random.h" header
 *
 * @param: none;
 * @return: std::string_view; string view of the randomized word
 */
std::string_view randomizeWord() {
    return words[Random::get<std::size_t>(0, words.size() - 1)];
};
} // namespace WordList

/**
 * Class Session for game initial game variables.
 *
 * private: std::string_view m_word; randomized word given at the start of the
 * game
 *
 * public: std::string_view getWord(); func for getting the private member
 * "m_word"; public: std::vector<char> temp{}; temporary array to be worked on
 * with placeholders '_' and letters as user guesses it correctly public:
 * std::vector<char> guesses{}; also temp array to be changed worked on, will be
 * filled with '+' at first and then letters if the user fails to guess public:
 * char lastInput{}; last inserted character from the user, used to check if the
 * user has inserted the same letter twice public: int attempts{6}; amount of
 * attempts that user have to corrently guess the letter, if changed, the
 * program will adjust to the number public: bool match; used to check if the
 * user has guessed the current letter
 */
class Session {
  private:
    std::string_view m_word{WordList::randomizeWord()};

  public:
    std::string_view getWord() const { return m_word; }
    std::vector<char> temp{};
    std::vector<char> guesses{};
    char lastInput{};
    int attempts{6};
    bool match;
};

/**
 * Checks if the words array is empty.
 *
 * @param: none
 * @return: bool
 */
bool checkIfWordArrIsEmpty() {
    if (WordList::words.empty()) {
        std::cout << "Error: No elements in the array!\n";
        return true;
    }
    return false;
}

/**
 * Sets the welcoming text on game start, iterates through
 * arrays to display current word as '_' and guesses as '+'
 *
 * @param: none
 * @return: none
 */
void setGameStartText(Session &gameVars) {
    std::cout << "Welcome to the C++man (a variant of Hangman)\n"
              << "To win: guess the word. To lose: run out of pluses.\n\n";

    for ([[maybe_unused]] const auto &x : gameVars.getWord()) {
        gameVars.temp.push_back('_');
    };
    for (int i = 1; i <= gameVars.attempts; i++) {
        gameVars.guesses.push_back('+');
    }
}

/**
 * Updates the game state after user input.
 *
 * @param: Session &gameVars; initial game variables passed
 * @param: char c; user input
 * @return: void;
 */
void updateGameState(Session &gameVars, char c) {
    for (std::size_t i = 0; i < gameVars.getWord().size(); i++) {
        if (c < 'a' || c > 'z') {
            std::cout << "-------------------------------------\n";
            std::cout << "\nInvalid input. Try again.\n";
            break;
        }
        if (c == gameVars.lastInput) {
            std::cout << "You already guessed that. Try again.\n";
            break;
        }
        if (c == gameVars.getWord()[i]) {
            gameVars.temp[i] = c;
            gameVars.match = true;
        }
    }

    if (!gameVars.match) {
        for (std::size_t k = 0; k < gameVars.guesses.size() - 1; k++) {
            gameVars.guesses[k] = gameVars.guesses[k + 1];
        }
        gameVars.guesses[gameVars.guesses.size() - 1] = c;
        gameVars.attempts--;
    } else {
        gameVars.lastInput = c;
    }
}

/**
 * std::cin checker for extraction, if it fails it will return the cin to the
 * normal mode and also remove bad previously given bad input.
 *
 * If std::cin has not failed, it will only take 1 char and remove the excessive
 * input if there is one
 *
 * @param: none;
 * @return: none;
 */
void inputChecker() {
    if (!std::cin) {      // check if the previous extraction failed
        std::cin.clear(); // put back 'normal' operation mode for cin
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                        '\n'); // remove bad input
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Game over loop logic, asks if the player wants to play new game, if not the
 * game is terminated
 *
 * @param: none;
 * @return: bool; returns true if the player has pressed 'n', else false to
 * continue game loop
 */
bool endGameState(bool isGameOver) {
    if (isGameOver == true) {
        std::cout << "\nGame over! All attempts have been used.\n";
    } else {
        std::cout << "\nCongratulations, you won!\n";
    }
    while (true) {
        std::cout << "\nStart new game? [y/n]: ";
        char c;
        std::cin >> c;

        inputChecker();

        if (c == 'n') {
            return true;
        } else if (c == 'y') {
            return false;
        } else {
            std::cout << "Invalid input.\n";
        }
    }
}

/**
 * Checks how many letters have been guessed by counting how many '_'
 * placeholders are left, if its 0 the user has won and end game state will be
 * triggered, otherwise the game continues
 *
 * @param: const Session &gameVars; inital game variables passed
 * @return: bool; returns true if the player has guessed all the letters
 */
bool checkGuessedLetters(const Session &gameVars) {
    // static int counter {static_cast<int>(gameVars.temp.size() - 1)};
    const int counter =
        std::count(gameVars.temp.begin(), gameVars.temp.end(), '_');
    if (counter == 0) {
        return true;
    } else {
        return false;
    }
}

/*
 * Game loop logic and function calls
 *
 * @param: Session &gameVars; inital game vars passed from main func
 * @return: bool; if the game finishes it will return false otherwise its
 * endless loop
 */
bool gameState(Session &gameVars) {
    if (checkIfWordArrIsEmpty()) {
        return false;
    };

    setGameStartText(gameVars);

    while (true) {
        if (gameVars.attempts < 0) {
            if (endGameState(true)) {
                return true;
            } else
                return false;
        }

        gameVars.match = false; // resets the letter matching on each loop

        std::cout << "\nThe word: ";

        for (const auto &x : gameVars.temp) {
            std::cout << x;
        }

        std::cout << "\tWrong guesses: ";

        for (const auto &x : gameVars.guesses) {
            std::cout << x;
        }

        if (checkGuessedLetters(gameVars)) {
            if (endGameState(false)) {
                return true;
            }
            return false;
        }

        std::cout << "\n\nEnter your next letter: ";

        char c{};
        std::cin >> c;

        inputChecker();

        updateGameState(gameVars, c);
    }
    return true;
}

int main() {
    while (true) {
        Session gameVars{};
        if (gameState(gameVars))
            break;
    }
    std::cout << "\nGame exited.\n";
    return 0;
}
