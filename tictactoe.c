#include <stdio.h>
#include <stdlib.h>

char availablePositions[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; 

int checkWin(char board[3][3], char letter, char positions[9]);
void drawBoard(char board [3][3]);
void makeMove(char board[3][3], char playerLetter);

void main(){
    char gameBoard[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
    };

    drawBoard(gameBoard);
    makeMove(gameBoard, 'X');

    return 0;
}

void drawBoard(char board [3][3]){

    for(int i=0; i<3; i++){

        for(int j=0; j<3; j++){
            printf("| %c ", board[i][j]);
        }

        printf("|\n");

    }

    return 0;

}

void makeMove(char board[3][3], char playerLetter){
    system("cls");
    printf("Tic Tac Toe\n\n");
    drawBoard(board);


    char position;
    printf("\n%c's turn. Enter a value from 1-9: ", playerLetter);
    scanf(" %c", &position);
    int validMove=0;

    while(validMove != 1){
        for(int i=0; i<9; i++){
            if(availablePositions[i] == position){
                validMove = 1;
                for(int i=0; i<3; i++){
                    for(int j=0; j<3; j++){
                        if(position == board[i][j]){
                            board[i][j] = playerLetter; //places player's letter into corresponding position
                        }
                    }
                }
                
                availablePositions[i] = '0'; //acts as removal of available position
                break;
            }
        }
        
        if(validMove == 1){
            break;
        }

        else if(validMove == 0){
            printf("validMove = 0\n");
            validMove = 0;
            position = '\0';
            printf("Invalid Move. Enter a valid number: ");
            scanf(" %c", &position);
        }

    }

    switch(checkWin(board, playerLetter, availablePositions)){
        case 0:
            switch(playerLetter){
                case 'X':
                    makeMove(board, 'O');
                case 'O':
                    makeMove(board, 'X');
            }
        
        case 1:
            system("cls");
            drawBoard(board);
            printf("%c wins.\n", playerLetter);
            exit(0);
        
        case 2:
            system("cls");
            drawBoard(board);
            printf("Draw.\n");
            exit(0);
    }

    return 0;
}

int checkWin(char board[3][3], char letter, char positions[9]){
    int gameWon=0;
    int gameDrawn=2;

    for(int i=0; i<3; i++){ // horizontal
        if(board[i][0] == letter && board[i][1] == letter && board[i][2] == letter){
            gameWon=1;
            gameDrawn=0;
        }
    }

    for(int j=0; j<3; j++){ //vertical
        if(board[0][j] == letter && board[1][j] == letter && board[2][j] == letter){
            gameWon=1;
            gameDrawn=0;
        }
    }

    if((board[0][0] == letter && board[1][1] == letter && board[2][2] == letter) || (board[2][0] == letter && board[1][1] == letter && board[0][2] == letter)){
        gameWon=1;
        gameDrawn=0;
    }

    for(int i=0; i<9; i++){//checking for draw
        if(positions[i] != '0'){
            gameDrawn=0;
            break;
        }
    }

    switch(gameDrawn){
        case 0:
            return gameWon;

        case 2:
        return gameDrawn;
    }
}
