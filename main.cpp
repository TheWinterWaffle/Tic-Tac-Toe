#include <iostream>
#include "GameState.h"
#include "ArrayList.h"
#include <string>

using namespace std;

const int MIN = -1000;
const int MAX = 1000;

int alphaBeta(GameState game, int depth, int player, int alpha, int beta){
    ArrayList<GameState> nextMoves;
    if(!game.done){
        for (int i = 0; i < game.size; i++){
            for (int j = 0; j < game.size; j++){
                GameState temp = game;
                bool valid = temp.play(i, j);
                if (valid) {
                    nextMoves.append(temp); 
                }
            }
        }
    }

    if (depth == 0 || game.done){
        if (game.hasWon(player)) return 100;
        else if (game.hasWon(!player)) return -100;
        else return 0;
    }
    else {
        int reward = alphaBeta(nextMoves.removeFirst(), depth - 1, player, alpha, beta);
        while (nextMoves.size() > 0){
            int curr = alphaBeta(nextMoves.removeFirst(), depth - 1, player, alpha, beta);
            if (game.currentTurn == player){
                reward = max(reward, curr);
                alpha = max(alpha, reward);
                if (beta <= alpha) break;
            } else {
                reward = min(reward, curr);
                beta = min(beta, reward);
                if (beta <= alpha) break;
            }
        }
        return reward;
    }
}

int main(){
    bool play = true;
    while (play) {
        system("clear");
        int size = 0;
        while (size < 3 || size > 5){
            cout << "Choose a board size (3 to 5): ";
            cin >> size;
            if (size < 3 || size > 5) {
                system("clear");
                cout << "Invalid size, please enter a valid board size." << endl;
            }
        }
        GameState game = GameState(size);

        int depth;
        if (size == 3) depth = 9;
        if (size == 4) depth = 4;
        if (size == 5) depth = 2;

        int playerTurn;
        string doesPlayerGoFirst;
        while (doesPlayerGoFirst != "y" && doesPlayerGoFirst != "n"){
            cout << "Do you want to go first? (y/n): "; 
            cin >> doesPlayerGoFirst;
            if (doesPlayerGoFirst != "y" && doesPlayerGoFirst != "n"){
                system("clear");
                cout << "Please enter a valid response." << endl;
            }
        }
        if (doesPlayerGoFirst == "y") playerTurn = 0;
        if (doesPlayerGoFirst == "n") playerTurn = 1;

        int x, y;

        if (!playerTurn){
            system("clear");
            cout << game << endl;
            cout << "Enter coordinates (X): ";
            cin >> x >> y;
            game.play(x, y);
        } else {
            game.play(0, 0);
        }

        while (!game.done){
            ArrayList<GameState> nextMoves;
            for (int i = 0; i < game.size; i++){
                for (int j = 0; j < game.size; j++){
                    GameState temp = game;
                    bool valid = temp.play(i, j);
                    if (valid) nextMoves.append(temp);
                }
            }

            system("clear");
            cout << game << endl;

            if (game.currentTurn == playerTurn) {
                (!playerTurn) ? cout << "Enter coordinates (X): " : cout << "Enter coordinates (O): ";
                cin >> x >> y;
                game.play(x, y);
            } else {
                GameState toPlay = nextMoves.removeFirst();
                int reward = alphaBeta(toPlay, depth, !playerTurn, MIN, MAX);
                while (nextMoves.size() > 0){
                    GameState next = nextMoves.removeFirst();
                    int curr = alphaBeta(next, depth, !playerTurn, MIN, MAX);
                    if (curr > reward){
                        reward = curr;
                        toPlay = next;
                    }
                }
                game.play(toPlay.lastMove.x, toPlay.lastMove.y);
            }
        }

        system("clear");
        cout << game << endl;

        if (game.hasWon(0)){
            cout << "X wins" << endl;
        }
        else if (game.hasWon(1)){
            cout << "O wins" << endl;
        }
        else {
            cout << "It's a tie" << endl;
        }
        cout << endl;

        string replay;
        while (replay != "y" && replay != "n") {
            cout << "Would you like to play again? (y/n): ";
            cin >> replay;
            if (replay != "y" && replay != "n") {
                system("clear");
                cout << "Please enter a valid response." << endl;
            }
        }
        (replay == "y") ? play = true : play = false;
    }
    return 0;
}
