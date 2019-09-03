// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grid.h"
#include "vector.h"
#include "marbletypes.h"
#include "compression.h"
#include "marbleutil.h"
#include "set.h"
using namespace std;

bool solver(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory, Set<BoardEncoding>& boardHistory);

bool solvePuzzle(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory) {
    Set<BoardEncoding> boardHistory;
    return solver(board, marbleCount, moveHistory, boardHistory);
}

bool solver(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory, Set<BoardEncoding>& boardHistory){
    if(marbleCount == 1) //Base Case / Victory State
        return true;

    BoardEncoding encoded = compressBoard(board);

    if(boardHistory.contains(encoded)) //prevents going down path that has already been assessed
        return false;

    Vector<Move> possibleMoves = findPossibleMoves(board);
    for(int i = 0; i < possibleMoves.size(); i++){
        makeMove(possibleMoves.get(i), board);
        moveHistory.add(possibleMoves.get(i));
        if(solvePuzzle(board, marbleCount-1, moveHistory))
            return true;

        moveHistory.pop_back(); //resets moveHistory
        undoMove(possibleMoves.get(i), board);
        boardHistory.add(encoded); //stores configuration to prevent redundancy
    }
    return false;

}
