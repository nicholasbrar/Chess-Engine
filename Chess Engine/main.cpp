#include <iostream>

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

	ChessBoard() {
		initializeBoard();		
		turn = WHITE;
	}

	void initializeBoard() {
		for (int i = 0; i < SIZE; i++) {	 // initialize pawns
			board[1][i] = { PAWN, WHITE };
			board[6][i] = { PAWN, BLACK };
		}

		board[0][0] = board[0][7] = { ROOK, WHITE }; // initialize rooks
		board[7][0] = board[7][7] = { ROOK, BLACK };

		board[0][1] = board[0][6] = { KNIGHT, WHITE }; // initialize knights
		board[7][1] = board[7][6] = { KNIGHT, BLACK };

		board[0][2] = board[0][5] = { BISHOP, WHITE }; // initialize bishops
		board[7][2] = board[7][5] = { BISHOP, BLACK };

		board[0][3] = { QUEEN, WHITE }; // Kings and Queens
		board[0][4] = { KING, WHITE };

		board[7][3] = { QUEEN, BLACK };
		board[7][4] = { KING, BLACK };

		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (board[i][j].color != WHITE && board[i][j].color != BLACK) {
					board[i][j] = { NONE, NOCOLOR };
				}
			}
		}
	}

	char getPieceCharacter(PIECE piece, COLOR color) {
		switch (piece) {
		case PAWN: return color == WHITE ? 'P' : 'p';
		case KNIGHT: return color == WHITE ? 'N' : 'n';
		case BISHOP: return color == WHITE ? 'B' : 'b';
		case ROOK: return color == WHITE ? 'R' : 'r';
		case QUEEN: return color == WHITE ? 'Q' : 'q';
		case KING: return color == WHITE ? 'K' : 'k';
		case NONE: return '.';
		}
	}

	void printBoard() {
		for (int i = SIZE - 1; i >= 0; i--) {  
			for (int j = 0; j < SIZE; j++) {
				cout << getPieceCharacter(board[i][j].piece, board[i][j].color) << " ";
			}
			cout << endl;
		}
	}

	bool movePiece(int startX, int startY, int endX, int endY) {
		if (isValidMove(startX, startY, endX, endY)) {
			board[endX][endY] = board[startX][startY];
			board[startX][startY] = { NONE, NOCOLOR };
			return true;
		}
		else {
			cout << "INVALID MOVE" << endl;
			return false;
		}

	}	   

	bool isValidMove(int startX, int startY, int endX, int endY) {
		SQUARE start = board[startX][startX];
		SQUARE end = board[endX][endX];

		if (start.piece == NONE) { // can't move a piece with no square
			return false;
		}
		if (start.color != turn) {	  // can't move the other color's piece
			return false;
		}
		if (end.color == turn) {   // can't capture your own piece
			return false;
		}

		switch (start.piece) {
		case PAWN:
			return isValidPawnMove(startX, startY, endX, endY);
		}
	}

	bool isValidPawnMove(int startX, int startY, int endX, int endY) {
		return true;
	}

};

int main() {
	ChessBoard board;
	board.printBoard();
}