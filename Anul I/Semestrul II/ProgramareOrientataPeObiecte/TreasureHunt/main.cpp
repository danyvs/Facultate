#include <iostream>
#include <vector>
#include "game.h"

using namespace std;

int main() {
    int lines, columns;
    do {
        cout << "Input the number of lines (>= 15) and columns (>= 15): ";
        cin >> lines >> columns;
    } while (lines < 15 || columns < 15);

    // initialize the game
    Game game(lines, columns);
    game.generateTreasures();

    string answer;
    int cntRounds, step = 1;
    cout << "Do you want to play 'til the end? (YES / NO)\n";
    cin >> answer;
    if (answer == "YES")
        game.play();
    else {
        if (answer == "NO") {
            while (!game.isGameFinished()) {
                cout << "Do you want to play a given number of rounds? (YES / NO)\n";
                cin >> answer;
                if (answer == "YES") {
                    cout << "Input the number of rounds you want to play: ";
                    cin >> cntRounds;
                    game.playNumberOfRounds(cntRounds, step);
                    step += cntRounds;
                }
                else {
                    if (answer == "NO") {
                        game.playOneRound(step);
                        ++step;
                    }
                    else
                        cout << "Invalid answer!\n";
                }
            }
        }
        else
            cout << "Invalid answer!\n";
    }

    game.printLeaderboard();

    return 0;
}