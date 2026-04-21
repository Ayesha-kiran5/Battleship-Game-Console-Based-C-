#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;
char player1[10][10]; 
char player2[10][10];
char player1_view[10][10]; 
char player2_view[10][10]; 

const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;

string shipnames[5] = {
    "AirCraft Carrier (5 Spaces)",
    "BattleShip  (4 Spaces)",
    "Destroyer   (3 Spaces)",
    "Submarine   (3 Spaces)",
    "Patrol Boat (2 Spaces)"
};

int ship_size[5] = {5, 4, 3, 3, 2};

void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void setRandomShips(char board[10][10]);
void vsComputer(char playerBoard[10][10], char computerBoard[10][10]);
void computerAttack(char playerBoard[10][10], int &computerScore);
void clearBoard(char board[10][10]);
void updateScore(int &score, bool isHit);

void clearScreen() {
    system("CLS"); 
}

void boardprinting(char board[10][10], bool showShips) {
    cout << "     0    1    2    3    4    5    6    7    8    9  " << endl;
    cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << endl;

    for (int row = 0; row < 10; row++) {
        cout << " " << row << " ";
        
        for (int col = 0; col < 10; col++) {
            char cell = board[row][col];
            
            if (cell == 'X') { 
                setColor(15, 12); 
                cout << "  X  "; 
                setColor(7, 0);
            }
            else if (cell == 'O') { 
                setColor(0, 8); 
                cout << "  O  "; 
                setColor(7, 0);
            }
            else if (cell == 'S' && showShips) { 
                setColor(0, 14); 
                cout << "  S  "; 
                setColor(7, 0);
            }
            else { 
                setColor(7, 1); 
                cout << "  ~  "; 
                setColor(7, 0);
            }
            cout << "|";
        }
        cout << endl;
        
        if (row < 9) {
            cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----�" << endl;
        }
    }
    
    cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
}

bool canplace(char board[10][10], int r, int c, int size, bool vert) {
    if (vert && r + size > 10) return false;
    if (!vert && c + size > 10) return false;

    for (int i = 0; i < size; i++) {
        int r_r = r;   
        int c_c = c;   

        if (vert)
            r_r = r + i;   
        else
            c_c = c + i;   

        if (r_r < 0 || r_r >= 10 || c_c < 0 || c_c >= 10) return false;

        if (board[r_r][c_c] != '~')
            return false;
    }
    return true;
}
//placementfunction
void place(char board[10][10], int r, int c, int size, bool vert) {
    for (int i = 0; i < size; i++) {
        int r_r = r;
        int c_c = c;

        if (vert)
            r_r = r + i;
        else
            c_c = c + i;

        board[r_r][c_c] = 'S';
    }
}
//manaul ships
void setManualShips(char board[10][10], string playerName) {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            board[i][j] = '~';

    for (int s = 0; s < 5; s++) {
        int shipSize = ship_size[s];
        bool placed = false;
       //ship size and placement explanation 
        while (!placed) {
            system("cls");
            cout << playerName << ", you are placing a ship of length: " << shipSize << endl;
            cout << "Ship: " << shipnames[s] << endl ;
            boardprinting(board, true);
            cout << "Select Placement Method 1=Manual, 2=Random: ";
            int choice;
            cin >> choice;
          //randomize them  
        if (choice == 2) {
                bool done = false;
                while (!done) {
                    int r = rand() % 10;
                    int c = rand() % 10;
                    bool vert = rand() % 2;
                    
        if (canplace(board, r, c, shipSize, vert)) {
                        place(board, r, c, shipSize, vert);
                        placed = true;
                        done = true;
                    }
                }
            }
       else if (choice == 1) {
                int r, c;
        cout << "Enter starting row (0-9): ";
        cin >> r;
        cout << "Enter starting column (0-9): ";
	    cin >> c;
       cout << "Do You Want To Place Vertically?(Y/N)else ships is placed horizontally: ";
       char rotate;//rotation check
                cin >> rotate;
                bool vert = (rotate == 'Y' || rotate == 'y');
                
                if (!canplace(board, r, c, shipSize, vert)) {
                    cout << "Invalid Placement! Press any key to try again...";
                    getch();
                }
                else {
                    place(board, r, c, shipSize, vert);
                    placed = true;
                }
            }
            else {
                cout << "Invalid choice! Press any key...";
                getch();
            }
        }
    }
    
    system("cls");
    cout << "All Ships Successfully Placed for " << playerName << "!" << endl;
    boardprinting(board, true);
    cout << "Press any key to continue...";
    getch();
}
// ship hit checkk 
bool allShipsSunk(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (board[i][j] == 'S') {
                return false;
            }
        }
    }
    return true;
}
//board printing
void displayBothBoards(char ownBoard[10][10], char enemyView[10][10], string ownName, string enemyName) {
    cout << "ENEMY BOARD";
    for(int i=0; i<45; i++) cout << " ";
    cout << "Opponent";
    for(int i=0; i<55; i++) cout << " ";
    cout << "PLAYER BOARD" << endl;
    
    cout << "                                   ";
    cout << "0     1     2     3     4     5    6     7     8     9  ";
    cout << endl;
    
    cout << "      0 1 2 3 4 5 6 7 8 9       ";
    cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+";
    cout << "      0 1 2 3 4 5 6 7 8 9" << endl;
    
    cout << "0  ";
    for (int col = 0; col < 10; col++) {
        char cell = enemyView[0][col];
        if (cell == 'X') {
            cout << "X  ";
        }
        else if (cell == 'O') {
            cout << "O  ";
        }
        else {
            cout << ".  ";
        }
    }
    cout << " ";
    cout << "0 �";
    for (int col = 0; col < 10; col++) {
        char cell = enemyView[0][col];
        if (cell == 'X') {
            setColor(15, 12);
            cout << "  X  ";
            setColor(7, 0);
        }
        else if (cell == 'O') {
            setColor(0, 8);
            cout << "  O  ";
            setColor(7, 0);
        }
        else {
            setColor(7, 1);
            cout << "  ~  ";
            setColor(7, 0);
        }
        cout << "�";
    }
    cout << "  0  ";
    for (int col = 0; col < 10; col++) {
        char cell = ownBoard[0][col];
        if (cell == 'X') {
            cout << "X  ";
        }
        else if (cell == 'O') {
            cout << "O  ";
        }
        else {
            cout << ".  ";
        }
    }
    cout << endl;
    
    for (int row = 1; row <= 9; row++) {
        cout << "                                   ";
        cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----" << endl;
        
        cout << row << "  ";
        for (int col = 0; col < 10; col++) {
            char cell = enemyView[row][col];
            if (cell == 'X') {
                cout << "X  ";
            }
            else if (cell == 'O') {
                cout << "O  ";
            }
            else {
                cout << ".  ";
            }
        }
        cout << " ";
        
        cout << row << " �";
        for (int col = 0; col < 10; col++) {
            char cell = enemyView[row][col];
            if (cell == 'X') {
                setColor(15, 12);
                cout << "  X  ";
                setColor(7, 0);
            }
            else if (cell == 'O') {
                setColor(0, 8);
                cout << "  O  ";
                setColor(7, 0);
            }
            else {
                setColor(7, 1);
                cout << "  ~  ";
                setColor(7, 0);
            }
            cout << "�";
        }
        
        cout << "  " << row << "  ";
        for (int col = 0; col < 10; col++) {
            char cell = ownBoard[row][col];
            if (cell == 'X') {
                cout << "X  ";
            }
            else if (cell == 'O') {
                cout << "O  ";
            }
            else {
                cout << ".  ";
            }
        }
        cout << endl;
    }
    
    cout << "                                   ";
    cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
}
/*
void vsplayer() {
    cout<<"Ahmed's part'"<<endl;}*/
void vsplayer() {
 
    for (int i = 0; i < 10; i++)
	 {
        for (int j = 0; j < 10; j++) 
		{
            player1[i][j]='~';
            player2[i][j]='~';
        	player1_view[i][j]='~';
            player2_view[i][j]='~';
            
        }
    }
    
    setManualShips(player1,"Player 1");
    system("cls");
    cout << "Player 2's turn to place ships. Player 1, look away!"<<endl;
    cout << "Press any key when ready";
    getch();
    setManualShips(player2,"Player 2");
    
    
   
    bool gameOver=false;
    bool player1Turn = true;
    
    while (!gameOver) 
	{
        system("cls");
        
        if (player1Turn) {
            cout << "========== PLAYER 1's TURN ==========" << endl ;
            displayBothBoards(player1,player2_view,"Player 1","Player 2");
            
            int r, c;
            cout << "Enter row to attack (0-9): "<<endl;
            cin >> r;
            cout << "Enter column to attack (0-9): ";
            cin >> c;
            
            if (r < 0 || r >= 10 || c < 0 || c >= 10)
			 {
                cout << "Invalid coordinates! Press any key";
                getch();
                continue;
            }
            
            if (player2_view[r][c]=='X'||player2_view[r][c]=='O')
            
              // Check if cell was already attacked and dont allow to attack again
			
			 {
                cout << "Already attacked this position! Press any key...";
                getch();
                continue;
            }
            
            if (player2[r][c] == 'S')
			
			 {
                player2[r][c] = 'X';
                
                player2_view[r][c] = 'X';
                
                cout << " HIT!   " << endl; // Hit a ship!
            }
            
            else if (player2[r][c] == '~')
			
			 {
                player2[r][c] = 'O';
                player2_view[r][c] = 'O';
                cout << " MISS! " << endl;//Missed  A Ship
                
            }
            else 
			{
                cout << "Already attacked! This shouldn't happen." << endl;
            }
            
            if (allShipsSunk(player2)) 
			
			{
                system("cls");
                cout << "======================================" << endl;
                cout << "    PLAYER 1 WINS! ALL SHIPS SUNK!" << endl;
                cout << "======================================" << endl;
                gameOver = true;
            }
            
            cout << "Press any key to continue";
            getch();
            player1Turn = false;
        }
        else {
            system("cls");
            cout << "Player 1, look away! Player 2's turn" << endl;
            cout << "Press any key when Player 2 is ready";
            getch();
            
            system("cls");
            cout << "PLAYER 2's TURN "  << endl;
            displayBothBoards(player2, player1_view, "Player 2", "Player 1");
            
            int r, c;
            cout << "Enter row to attack (0-9): ";
            cin >> r;
            cout << "Enter column to attack (0-9): ";
            cin >> c;
            
            if (r < 0 || r >= 10 || c < 0 || c >= 10) 
			{
                cout << "Invalid coordinates! Press any key";
                getch();
                continue;
            }
            
            if (player1_view[r][c]=='X'||player1_view[r][c]=='O')
            
              // Check if cell was already attacked and dont allow to attack again
            
			 {
                cout << "Already attacked this position! Press any key";
                getch();
                continue;
            }
            
            if (player1[r][c] == 'S') 
			{
                player1[r][c] = 'X';// Mark player view board
                player1_view[r][c] = 'X';// Mark player view board
                cout << " HIT!  " << endl;  // Hit a ship!
            }
            
            else if (player1[r][c] == '~')
            
			 {
                player1[r][c] = 'O';
                player1_view[r][c] = 'O';
                cout << "MISS! " << endl;//Missed A Ship
            }
            
            else
            
			 {
                cout << "Already Attacked!!!" << endl;
            }
            
            if (allShipsSunk(player1)) 
			
			{
                system("cls");
                cout << "======================================" << endl;
                cout << "    PLAYER 2 WINS! ALL SHIPS SUNK!" << endl;
                cout << "======================================" << endl;
                gameOver = true;
            }
            
            cout << "Press any key to continue...";
            getch();
            player1Turn = true;
        }
    }
}
void clearBoard(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = '~';
        }
    }
}
//scoring function
void updateScore(int &score, bool isHit) {
    if (isHit) {
        score += 10; 
    } else {
        score -= 1;   
    }
}
// Randomly places all 5 ships on the board
void setRandomShips(char board[10][10]) {
clearBoard(board);  // Start with empty board
 
    // Loop through all 5 ships
    for (int s = 0; s < 5; s++) {
    int shipSize = ship_size[s];
    bool placed = false;
        
    // Keep trying random positions until ship is placed successfully
    while (!placed) {
            int r = rand() % 10;      // Random row (0-9)
            int c = rand() % 10;      // Random column (0-9)
            bool vert = rand() % 2;   // Random orientation (0=horizontal, 1=vertical)
            
            // Check if ship can be placed at this position
            if (canplace(board, r, c, shipSize, vert)) {
                place(board, r, c, shipSize, vert);
                placed = true;
     }
        }
    }
}

void computerAttack(char playerBoard[10][10], int &computerScore) {
    int r, c;
    bool validAttack = false;
    while (!validAttack) {
        r = rand() % 10;  // Random row
        c = rand() % 10;  // Random column
	    if (playerBoard[r][c] == 'S' || playerBoard[r][c] == '~') {
            validAttack = true;
      }
    }
    cout << "Computer attacks position (" << r << ", " << c << ")... ";
    // Process the attack
    if (playerBoard[r][c] == 'S') {
        // Hit a ship!
        playerBoard[r][c] = 'X';  // Mark as hit
        cout << "HIT!" << endl;
        updateScore(computerScore, true);  // Add 10 points
    }
    else if (playerBoard[r][c] == '~') {
        playerBoard[r][c] = 'O';  // Mark as miss
        cout << "MISS!" << endl;
        updateScore(computerScore, false);  // Subtract 1 point
   }
}

void vsComputer(char playerBoard[10][10], char computerBoard[10][10]) {
char player_view[10][10];
    // Score tracking
    int playerScore = 0;
    int computerScore = 0;
//at first the scores are 0 intialized     
    // Initialize all boards to water
    clearBoard(playerBoard);
    clearBoard(computerBoard);
   clearBoard(player_view);
    // Player manually places their ships on the board
	setManualShips(playerBoard, "Player");
    
    // the Computer randomly places its ships now while hiding the shipss
system("cls");
  cout << "Computer is placing ships" << endl;
    setRandomShips(computerBoard);
    cout << "Computer has placed all ships!" << endl;
    cout << "Press any key to start the battle...";
    getch();
    // BATTLE PHASE
    bool gameOver = false;
    
    while (!gameOver) {
        system("cls");
        cout << "========== YOUR TURN (6 SHOTS) ==========" << endl;
        cout << "Your Score: " << playerScore << " | Computer Score: " << computerScore << endl << endl;
        //shotting turns 6 eachh
        for (int shot = 1; shot <= 6; shot++) {
            // Display current state of both boards
            displayBothBoards(playerBoard, player_view, "Player", "Computer");
            cout << "\n--- SHOT " << shot << " of 6 ---" << endl;
            int r, c;
            cout << "Enter row to attack (0-9): ";
            cin >> r;
            cout << "Enter column to attack (0-9): ";
            cin >> c;
            // Validate coordinates are within thebound to place them easily
            if (r < 0 || r >= 10 || c < 0 || c >= 10) {
                cout << "Invalid coordinates! Try again." << endl;
                shot--;  // Don't count this as a shot in scoring as well as shot count
                cout << "Press any key to continue...";
                getch();
                system("cls");
                cout << "========== YOUR TURN (6 SHOTS) ==========" << endl;
                cout << "Your Score: " << playerScore << " | Computer Score: " << computerScore << endl << endl;
                continue;
            }
            // Check if cell was already attacked and dont allow to attack again
            if (player_view[r][c] == 'X' || player_view[r][c] == 'O') {
           cout << "Already attacked this position! Try again." << endl;
            shot--;  // Don't count this as a shot
                cout << "Press any key to continue...";
             getch();
                system("cls");
              cout << "========== YOUR TURN (6 SHOTS) ==========" << endl;
         cout << "Your Score: " << playerScore << " | Computer Score: " << computerScore << endl << endl;
                continue;
            }
            // Process the attack on computer board in this condition
            if (computerBoard[r][c] == 'S') {
                // HIT ke liye s
                computerBoard[r][c] = 'X';  // Mark computer actual board as well
           player_view[r][c] = 'X';     // Mark player view board
                cout << "\n GOT IT HIT! " << endl;
                updateScore(playerScore, true);  // +10 points
        }
            else if (computerBoard[r][c] == '~') {
                // MISS!
                computerBoard[r][c] = 'O';  // Mark computer's actual board
             player_view[r][c] = 'O';     // Mark player's view
                cout << "\n  YOU MISS! " << endl;
                updateScore(playerScore, false);  // -1 point
       }
            
            if (allShipsSunk(computerBoard)) {
	    system("cls");
                cout << "======================================" << endl;    
	      cout << "    YOU WIN! ALL ENEMY SHIPS SUNK!" << endl;
           cout << "    Final Score: " << playerScore << endl;
                gameOver = true;
                break;  // Exit shot loop
            }       
            if (shot < 6) {
                cout << "\nPress any key for next shot...";
                getch();
	   system("cls");
                cout << "========== YOUR TURN (6 SHOTS) ==========" << endl;
             cout << "Your Score: " << playerScore << " | Computer Score: " << computerScore << endl << endl;
            }
        }  
        if (gameOver) break;
        
        cout << "\nYour turn complete! Press any key for computer's turn...";
        getch();
                
        system("cls");
        cout << "========== COMPUTER'S TURN (6 SHOTS) ==========" << endl;
        cout << "Your Score: " << playerScore << " | Computer Score: " << computerScore << endl << endl;
        
        // Computer gets RANDOM SHOTS
        for (int shot = 1; shot <= 6; shot++) {
            cout << "Computer Shot " << shot << "of 6";
            // Computer attacks a random unattacked cell on player's board
     computerAttack(playerBoard, computerScore);
            
            // Check if computer destroyed all player ships (lose condition)
            if (allShipsSunk(playerBoard)) {
                system("cls");
                cout << "======================================" << endl;
                cout << "    COMPUTER WINS! ALL YOUR SHIPS SUNK!" << endl;
                cout << "    Your Score: " << playerScore << endl;
                cout << "    Computer Score: " << computerScore << endl;
                cout << "======================================" << endl;
                gameOver = true;
                break;  // Exit shot loop
            }
        
    if (shot < 6) {
                cout << endl;
            }
        }   
        //exit main loop
		  if (gameOver) break;  
        // Show updated scores after computer's turn
    cout << "\nComputer's turn complete!" << endl;
   cout << "Current Scores - You: " << playerScore << "  Computer: " << computerScore << endl;
        cout << "Press any key to continue...";
        getch();
    }
}

// Wrapper function to call vsComputer with proper board setup for vs computer parttt
void vscomputer() {
    char playerBoard[10][10];
    char computerBoard[10][10];
    vsComputer(playerBoard, computerBoard);
    getch();
}

void gamelogic() {
	string name;
    cout << "1. PLAYER VS PLAYER" << endl;
    cout << "2. PLAYER VS COMPUTER" << endl;
    cout << "-----------------------------" << endl;
    cout <<"enter your name"<<endl;
    cin>>name;
    cout << "Enter your choice (1-2): ";
    int input;
    cin >> input;
    
    if (input == 1) {
        system("cls");
        vsplayer();
    }
    else { system("cls");
        vscomputer();
    }
}
void instructions() {
    cout << "----------------------------" << endl;
    cout << "      Instructions" << endl;
    cout << "----------------------------" << endl << endl;
    cout << "1. Place Your Ships On The Board" << endl;
    cout << "2. You And The Opponent Will Take Turns Firing Shots" << endl;
    cout << "3. 'O' means it's a miss, 'X' means it's a hit" << endl;
    cout << "4. Sink all enemy boats to win" << endl;
    cout << "5. Your Score is based on accuracy and speed" << endl << endl;
    cout << "----------------------------" << endl;
    cout << "(Press Any Key To Go To The Main Menu)";
}
void leaderboards() {
   // cout << "Highest Score is By " << highestname << " With a score of " << highest << endl;
    cout << "(Press any key to go to the main menu)";
}
void mainmenu() {
    while (true) {
        cout << "==========================================" << endl << endl;
        cout << "      WELCOME TO THE BATTLESHIP GAME" << endl << endl;
        cout << "==========================================" << endl << endl;
        cout << "              MAIN MENU" << endl << endl;
        cout << "-------------------------------------------" << endl;
        setColor(4, 0);
        cout << "1.";
        setColor(7, 0);
        cout << " Start New Game" << endl;
        setColor(4,0);
        cout << "2.";
        setColor(7,0);
        cout << " Instructions" << endl;
        setColor(4,0);
        cout << "3.";
        setColor(7,0);
        cout << " View LeaderBoard" << endl;
        setColor(4,0);
        cout << "4.";
        setColor(7,0);
        cout << " Exit" << endl << endl;
        cout << "Enter your choice: (1-4) ";

        int input;
        while (true) {
            cin >> input;
            
            if ((input == 1) || (input == 2) || (input == 3) || (input == 4)) {
                system("cls");
                break;
            }
            else {
                cout << "Input A Valid Input: ";
            }
        }

        switch (input) {
            case 1:
                gamelogic();
                getch();
                system("cls");
                break;
            case 2:
                instructions();
                cout << endl;
                getch();
                system("cls");
                break;
            case 3:
                leaderboards();
                cout << endl;
                getch();
                system("cls");
                break;
            case 4:
                exit(0);
        }
    }
}

int main() {
    srand(time(0)); 
    mainmenu();
    return 0;
}