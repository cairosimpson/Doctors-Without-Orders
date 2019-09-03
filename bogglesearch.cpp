// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "vector.h"
#include "bogglegui.h"
#include "console.h"
using namespace std;

struct Coordinate{
    int x;
    int y;
};

bool validateWord(Grid<char>& board, Lexicon& dictionary, Vector<char>& wordVec, Grid<int>& checkBoard, Coordinate xy);
Vector<Coordinate> findCoordinatesOfFirstLetter(Grid<char>& board, char firstLetter);
Vector<char> strToVector(string word);
int scoreWord(string word);
bool findWords(Grid<char>& board, Lexicon& dictionary, Set<string> &foundWords, Grid<int>& checkBoard, string partial, int x, int y);

//Validates and score human word entries, main purpose is to initalize parameters for validateWord
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    toLowerCaseInPlace(word); //dictionary characters are lowercase
    if(word.length() < BoggleGUI::MIN_WORD_LENGTH || !(dictionary.contains(word)) ) return false;
    toUpperCaseInPlace(word); //grid characters are uppercase

    Vector<Coordinate> firstLetterXY = findCoordinatesOfFirstLetter(board, word.at(0)); //returns vector that contains coordinates of first letter
    Grid<int> checkBoard;
    checkBoard.resize(board.numRows(), board.numCols()); //initalizing board to keep track of checked locations (visited = 1, non-visited = 0)

    for(int k = 0; k < firstLetterXY.size(); k++){
        Vector<char> wordVec = strToVector(word);
        wordVec.removeFront();
        checkBoard.fill(0); //sets every element as "non-visited"
        checkBoard[firstLetterXY.get(k).x][firstLetterXY.get(k).y] = 1; //sets location of first letter as checked
        if(validateWord(board, dictionary, wordVec, checkBoard, firstLetterXY.get(k))){
            BoggleGUI::scorePointsHuman(scoreWord(word));
            return true;
        }
    }
    return false;
}

//This function returns all words not found by the human, it also scores these words
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> foundWords;
    Grid<int> checkBoard;
    checkBoard.resize(board.numRows(), board.numCols());

    for(int i = 0; i < board.numRows(); i++){
        for(int j = 0; j < board.numCols(); j++){
            checkBoard.fill(0);
            checkBoard[i][j] = 1;
            findWords(board, dictionary, foundWords,checkBoard, "", i, j);

        }
    }
    Set<string> computerWords = foundWords - humanWords;

    Set<string>::iterator iter = computerWords.begin();
    while(iter != computerWords.end()){
        string word = *iter;
        BoggleGUI::scorePointsComputer(scoreWord(word));
        iter++;
    }
    return computerWords;
}

//This function finds every possible valid word in the game and stores them in foundWords
bool findWords(Grid<char>& board, Lexicon& dictionary, Set<string>&foundWords,Grid<int>& checkBoard, string partial, int x, int y){
    if(dictionary.contains(partial) && partial.size() >= BoggleGUI::MIN_WORD_LENGTH){
        foundWords.add(partial);
        if(!dictionary.containsPrefix(partial))//prevents it from stopping path at a valid word that could possibly be the start to another (ie. straw and strawberry)
            return true;
    }

    for(int i = x-1; i <= x+1; i++){
        for(int j = y-1; j <= y+1; j++){
            int x0 = x;//stores coordinates to "unmake" choice at bottom of loop
            int y0 = y;

            if(i < 0 || i==board.numRows() || j < 0|| j == board.numCols()) continue; //bounds checking

            if((dictionary.containsPrefix(partial+board[i][j]) || dictionary.contains(partial+board[i][j])) && checkBoard[i][j] == 0){
                partial+=board[i][j]; //appends valid character to partial
                checkBoard[i][j] = 1; //marks spot in grid as visited
                x = i; //updates coordinates
                y = j;
            } else continue;

            if(findWords(board, dictionary, foundWords, checkBoard, partial, x, y)) return true;

            checkBoard[i][j] = 0; //reset checkBoard
            partial.pop_back(); //reset partial
            x = x0;//reset coordinates
            y = y0;

        }
    }
    return false;
}

//This function validates a human's words entry and returns true if it's valid, false if it's not
bool validateWord(Grid<char>& board, Lexicon& dictionary, Vector<char>& wordVec, Grid<int>& checkBoard, Coordinate xy){
    BoggleGUI::clearHighlighting();
    if(wordVec.size() == 0) return true;  //Means all letters have been successfully found (base case)

    for(int i = xy.x-1; i <= xy.x+1; i++){
        for(int j = xy.y-1; j <= xy.y+1; j++){
            char front = wordVec.get(0); //stores first letter of word to unmake choice at bottom of loop
            int x = xy.x; //stores coordinates to unmake choice at bottom of loop
            int y = xy.y;

            if(i < 0 || i==board.numRows() || j < 0|| j == board.numCols()) continue; //bounds checking
            BoggleGUI::setHighlighted(i, j, true);
            pause(100);

            if(board[i][j] == wordVec.get(0) && checkBoard[i][j] == 0){ //if its the letter we're looking for at an unvisited location
                wordVec.removeFront();
                checkBoard[i][j] = 1;//marks location in grid as visited
                xy.x = i; //updates coordinates
                xy.y = j;
            } else {
                BoggleGUI::setHighlighted(i, j, false);
                continue;
            }

            if(validateWord(board, dictionary, wordVec, checkBoard, xy)) return true;

            checkBoard[i][j] = 0; //unmake choices
            wordVec.insert(0, front);
            xy.x = x;
            xy.y = y;
        }
    }
    return false;

}

//This function finds the coordinates of a letter of word in a grid and returns these coordinates as a Vector of Coordinates (struct)
Vector<Coordinate> findCoordinatesOfFirstLetter(Grid<char>& board, char firstLetter){
    Vector<Coordinate> coordinatesVec;
    for(int i = 0; i < board.numRows(); i++){
        for(int j = 0; j < board.numCols(); j++){
            if(board[i][j] == firstLetter){
                Coordinate xy;
                xy.x = i;
                xy.y = j;
                coordinatesVec.add(xy);
            }
        }
    }
    return coordinatesVec;
}

//This vector accepts a string as an input and returns the string as a vector of characters
Vector<char> strToVector(string word){
    Vector<char> wordVec;
    for(int i = 0; i < word.length(); i++){
        wordVec.add(word[i]);
    }
    return wordVec;
}

//This function accepts a string as an input and returns its corresponding score (per boggle rules) as an int
int scoreWord(string word){
    int size = word.length();
    int score;
    if(size <= 4) score = 1;
    else if(size == 5) score = 2;
    else if(size == 6) score = 3;
    else if(size == 7 ) score = 5;
    else score = 11;
    return score;
}
