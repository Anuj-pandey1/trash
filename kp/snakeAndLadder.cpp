#include<iostream>
#include<queue>
#include<vector>
#include<unordered_map>

using namespace std;
int ID = 0;

class SnakeAndLadder{
public:
    // static int ID;
    SnakeAndLadder(){
        // ID = 0;
    }

    class Player{
    public:
        int playerId;
        string name;

        Player(string name){
            playerId = ID++;
            this->name = name;
        }
    };

    class Dice{
    public:
        int noOfDice;
        Dice(int noOfDice){
            this->noOfDice = noOfDice;
        }
        int rollDice(){
            // should return no in between (n, 6n)
            return rand()%(5*noOfDice+1) + noOfDice;
        }
    };


    class GameBoard{
    public:
        Dice* dice;
        queue<Player*> nextPlayer;
        unordered_map<int, int> jumpers;
        unordered_map<int, int> currentPositions;
        int boardSize;

        GameBoard(Dice* dice, queue<Player*> nextPlayer, unordered_map<int, int> jumpers, unordered_map<int, int> currentPositions, int boardSize){
            
            this->dice = dice;
            this->nextPlayer = nextPlayer;
            this->jumpers = jumpers;
            this->currentPositions = currentPositions;
            this->boardSize =  boardSize;

        }

        void PlayGame(){
            cout << "Total no of players are: " << nextPlayer.size() << endl;
            while(nextPlayer.size() > 1){
                Player* currentPlayer = nextPlayer.front();
                nextPlayer.pop();
                cout << "Player: " << currentPlayer->name << ", ";
                int diceValue = dice->rollDice();
                cout << "DiceValue: " << diceValue << ", ";

                int currentPosition = currentPositions[currentPlayer->playerId];
                cout << "Current Position: " << currentPosition << ", ";
                //can't move
                if(currentPosition + diceValue > boardSize){
                    nextPlayer.push(currentPlayer);
                    continue;
                }
                int nextPosition = currentPosition + diceValue;
                while(jumpers.find(nextPosition) != jumpers.end()){
                    cout << "\n hit by jumper\n";
                    nextPosition = jumpers.find(nextPosition)->second;
                }

                // winner
                if(nextPosition == 100){
                    cout << "\nPlayer: " << currentPlayer->name << " got eliminated\n";
                    continue;
                }
                cout << "Next Position: " << nextPosition << endl;
                currentPositions[currentPlayer->playerId] = nextPosition;
                nextPlayer.push(currentPlayer);
            }
        }

    };
    void start(){
        cout << "Starting the Game.....\n";
        Dice* dice = new Dice(1);
        Player* player1 = new Player("KP");
        Player* player2 = new Player("JJ");
        queue<Player*> nextPlayer;
        nextPlayer.push(player1);
        nextPlayer.push(player2);
        unordered_map<int, int> jumpers;
        jumpers[4] = 10;
        jumpers[6] = 90;
        jumpers[95] = 5;
        unordered_map<int, int> currentPosition;
        currentPosition[player1->playerId] = 1;
        currentPosition[player2->playerId] = 1;
        int boardSize = 100;

        GameBoard* gameBoard = new GameBoard(dice, nextPlayer, jumpers, currentPosition, boardSize);
        cout << "Here we go......\n";
        gameBoard->PlayGame();
    }
};

int main(){
    cout << "Lets play Snake and Ladder game-----\n";


    SnakeAndLadder* snakeAndLadder= new SnakeAndLadder();
    snakeAndLadder->start();
    return 0;
}