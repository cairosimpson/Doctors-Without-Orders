#include <iostream>
#include <fstream>
#include "simpio.h"
#include "filelib.h"
#include "console.h"
#include "strlib.h"
#include "grid.h"
#include "lifegui.h"
using namespace std;


void printInstructions();
void openFile(ifstream& file);
void initializeGrid(ifstream& file, Grid<char>& grid);
void printGrid(Grid<char>& grid);
void nextGen(Grid<char>& grid);
int countNeighbors(Grid<char>& grid, int row, int col);
string getUserChoice();
void animate(Grid<char>&grid);
void tick(Grid<char>&grid);
string quitProcedure();
void updateGUI(Grid<char>& grid);

const int pauseTime = 300;

int main() {
    LifeGUI::initialize;
    string sentinel = "";
    printInstructions();
    while(sentinel != "SENTINEL"){
        ifstream file;
        openFile(file);
        Grid<char> grid(0,0);
        initializeGrid(file,grid);
        LifeGUI::resize(grid.height(), grid.width());
        updateGUI(grid);
        printGrid(grid);
        string userInput = "";

        while(userInput != "Q"){
            cout<<endl<<"a)nimate, t)ick, q)uit?";
            userInput = getUserChoice();

            if(userInput == "A")
                animate(grid);
            else if(userInput == "T"){
                tick(grid);
            }
            else{
                sentinel = quitProcedure();
            }
        }
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

//prints out game instructions to the console
void printInstructions(){
    cout<<"Welcome to the CS 106B/X Game of Life!"<<endl;
    cout<<"This program simulates the lifecycle of a bacterial colony."<<endl;
    cout<<"Cells (X) live and die by the following rules:"<<endl;
    cout<<"* A cell with 1 or fewer neighbors dies."<<endl;
    cout<<"* Locations with 2 neighbors remain stable."<<endl;
    cout<<"* Locations with 3 neighbors will create life."<<endl;
    cout<<"* A cell with 4 or more neighbors dies."<<endl;

}

//Opens .txt file and verifies that the file exists. If it doesnt, user is reprompted.
void openFile(ifstream &file){
    string fileName;
    cout<<"Grid Input File Name? ";
    fileName = getLine();
    while(!fileExists(fileName)){
        cout<<"Unable to open that file.  Try again."<<endl;
        cout<<"Grid Input File Name? ";
        fileName = getLine();
    }
    file.open(fileName.c_str());
}

//Reads the .txt file and maps the initial layout to a grid object
void initializeGrid(ifstream &file, Grid<char>& grid){
    string rows,cols,line;
    getline(file, rows);
    getline(file,cols);
    const int nRows = stringToInteger(rows);
    const int nCols = stringToInteger(cols);
    grid.resize(nRows,nCols);

    for(int i = 0; i < nRows; i++){ //This for loop sets every element of grid to a dash
        for(int j = 0; j < nCols; j++){
            grid.set(i,j,'-');
        }
    }
    int rowIndex = 0;
    int pos = 0;
    while(getline(file,line)){
        while(line.find_first_of('X',pos) != string::npos ){
            pos = line.find_first_of('X', pos); //finds position of X's
            grid.set(rowIndex, pos, 'X');
            pos++; //moves starting index one to the right to find next X
        }
        pos = 0;
        rowIndex++; //moves down a row
    }
    file.close();
}

//prints a grid to the console
void printGrid(Grid<char>& grid){
    const int nRows = grid.height();
    const int nCols = grid.width();
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            cout<<grid.get(i,j);
        }
        cout<<endl;
    }
}

//updates the grid by one generation
void nextGen(Grid<char> &grid){
    Grid<char> newGrid = grid;
    const int nRows = grid.height();
    const int nCols = grid.width();
    int neighbors;

    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            neighbors = countNeighbors(grid,i,j);

            if(neighbors <= 1)
                newGrid.set(i, j, '-');

            if(neighbors == 3)
                newGrid.set(i,j,'X');

            if(neighbors > 3)
                newGrid.set(i,j,'-');
        }
    }
    grid = newGrid;
}

//counts and returns the amount of neighbors for a single specified grid element
int countNeighbors(Grid<char>& grid, int row, int col){
    int neighbors = 0;
    int iTemp, jTemp; //will hold i and j values in case of edge cases to preserve the for loop
    const int nRows = grid.height();
    const int nCols = grid.width();

    for(int i = row-1; i <= row + 1; i++){
        for(int j = col-1; j <= col + 1; j++){
            iTemp = i;
            jTemp = j;
            if( i == -1) //very left side of grid
                i = nRows - 1; //sets i to check on other side of grid (deals with wrapping)

            else if(i == nRows) // right side of grid
                i = 0;

            else if(j == -1) //top of grid
                j = nCols - 1;

            else if(j == nCols) //bottom of grid
                j = 0;

            else if(grid.get(i,j) == 'X')
                neighbors++;

            i = iTemp; //resets i and j vales so for loop continues through
            j = jTemp;
        }
    }
    if(grid.get(row,col) == 'X') //Can't be a neighbor to yourself
        neighbors--;

    return neighbors;
}

//verifies that user enters valid menu choice and returns that choice
string getUserChoice(){
    string userInput = getLine();
    toUpperCaseInPlace(userInput);//makes user input case insensisitve
    while(userInput != "A" && userInput != "T" && userInput != "Q"){
        cout<<"Please type either a, q, or t. Try again ";
        userInput = getLine();
        toUpperCaseInPlace(userInput);

    }
    return userInput;
}

//takes care of the animation portion
void animate(Grid<char> &grid){
    cout<<"How many frames? ";
    int frames = getInteger();

    while(!(frames > 0)){
        cout<<"Try again. ";
        cout<<"How many frames? ";
        frames = getInteger();
    }

    for(int i = 0; i < frames; i++){
        clearConsole();
        tick(grid);
        pause(pauseTime);

    }
}

//calculates and prints next generation
void tick(Grid<char>&grid){
    nextGen(grid);
    printGrid(grid);
    updateGUI(grid);
}

//called when user enters q to quit, if user doesn't want to load another file, it returns "SENTINEL" which will end the program
string quitProcedure(){
    cout<<"Load another file? (y/n)";
    string userInput = getLine();
    toUpperCaseInPlace(userInput);
    while(userInput.at(0) != 'Y' && userInput.at(0) != 'N'){
        cout<<"Please type a word that starts with 'Y' or 'N'.";
        userInput = getLine();
        toUpperCaseInPlace(userInput);
    }

    if(userInput.at(0) == 'N'){
        return "SENTINEL"; //breaks out of outer loop in main
    }
    else{
        return "ARBITRARY"; //program will continue, return value could be anything
    }

}

//Redraws GUI, called everytime nextGen is called (ie. animate() and tick())
void updateGUI(Grid<char>& grid){
    LifeGUI::clear();
    const int nRows = grid.height();
    const int nCols = grid.width();

    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            if(grid.get(i,j) == 'X')
                LifeGUI::fillCell(i, j);
        }
    }
    LifeGUI::repaint();
}

