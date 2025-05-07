#pragma once

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <cmath>
#include <utility>

using namespace std;

enum PIECE { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE };

enum COLOR { WHITE, BLACK, NOCOLOR };

struct SQUARE {
	PIECE piece;
	COLOR color;
};

class ChessBoard {
public: 
    static const int SIZE = 8;
    SQUARE board[SIZE][SIZE];
    COLOR turn;

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRooksMoved[2] = { false, false };
    bool blackRooksMoved[2] = { false, false };

    ChessBoard();
    void initializeBoard();
    char getPieceCharacter(PIECE piece, COLOR color);
    void printBoard();
    bool isValidKingsideCastle(COLOR color);
    bool isValidQueensideCastle(COLOR color);
    bool movePiece(int startX, int startY, int endX, int endY);
    void promotePawn(int x, int y);
    bool isValidMove(int startX, int startY, int endX, int endY);
    bool isValidPawnMove(int startX, int startY, int endX, int endY, COLOR color);
    bool isValidKnightMove(int startX, int startY, int endX, int endY);
    bool isValidBishopMove(int startX, int startY, int endX, int endY);
    bool isValidRookMove(int startX, int startY, int endX, int endY);
    bool isValidQueenMove(int startX, int startY, int endX, int endY);
    bool isValidKingMove(int startX, int startY, int endX, int endY);
    void getMove(int& startX, int& startY, int& endX, int& endY);
    void playGame();
    pair<int, int> locateKing(COLOR color);
    pair<int, int> locateKing(COLOR color, const SQUARE(&tempBoard)[SIZE][SIZE]);
    bool squareIsAttacked(int x, int y, COLOR enemyColor);
    bool squareIsAttacked(int x, int y, COLOR enemyColor, const SQUARE(&tempBoard)[SIZE][SIZE]);
    bool kingInCheck(COLOR color);
    bool moveLeavesKingInCheck(int startX, int startY, int endX, int endY);
    bool hasLegalMoves(COLOR color);
    bool isStalemate(COLOR color);
    bool isCheckmate(COLOR color);
    bool canBishopAttack(int startX, int startY, int endX, int endY);
    bool canKnightAttack(int startX, int startY, int endX, int endY);
    bool canRookAttack(int startX, int startY, int endX, int endY);
    bool canPawnAttack(int startX, int startY, int endX, int endY, COLOR color);
    bool canQueenAttack(int startX, int startY, int endX, int endY);
    bool canKingAttack(int startX, int startY, int endX, int endY);
};

#endif