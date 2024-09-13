#include <iostream>
#include <string>
#include <cmath>

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
			board[6][i] = { PAWN, BLACK };
			board[1][i] = { PAWN, WHITE };
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
		SQUARE start = board[startX][startY];
		SQUARE end = board[endX][endY];

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
			return isValidPawnMove(startX, startY, endX, endY, start.color);
		case KNIGHT:
			return isValidKnightMove(startX, startY, endX, endY);
		case BISHOP:
			return isValidBishopMove(startX, startY, endX, endY);
		case ROOK:
			return isValidRookMove(startX, startY, endX, endY);
		case QUEEN:
			return isValidQueenMove(startX, startY, endX, endY);
		case KING:
			return isValidKingMove(startX, startY, endX, endY);
		}
	}

	bool isValidPawnMove(int startX, int startY, int endX, int endY, COLOR color) {
		int direction = (color == WHITE) ? 1 : -1; // push up/down board

		int pawnStart = (color == WHITE) ? 1 : 6; // to check for first move (2 squares)

		if (startY == endY && board[endX][endY].piece == NONE) { // vertical pawn push
			if (endX == startX + direction) {
				return true;
			}

			if (startX == pawnStart && endX == startX + 2 * direction) {
				if (board[endX][endY].piece == NONE && board[startX + direction][startY].piece == NONE) {
					return true;
				}
			}
		}

		if (abs(startY - endY) == 1 && endX == startX + direction && board[endX][endY].piece != NONE) {
			return true;
		}
		return false;

	}

	bool isValidKnightMove(int startX, int startY, int endX, int endY) {
		int dx = abs(endX - startX);
		int dy = abs(endY - startY);

		if(board[endX][endY].color == turn){   // can't capture own piece
			return false;
		}
		return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
	}

	bool isValidBishopMove(int startX, int startY, int endX, int endY) {
		return true;
		// add - can't capture own piece
		// for diagonal mvoe dx and dy must be the same 
		// cant go through pieces
		// check all diagonals
	}

	bool isValidRookMove(int startX, int startY, int endX, int endY) {
		return true;
		// cant capture own piece 
		// cant move through piece 
		// dx and dy cant both change 
	}

	bool isValidQueenMove(int startX, int startY, int endX, int endY) {
		return true;
		//valid rook or valid bishop??
	}

	bool isValidKingMove(int startX, int startY, int endX, int endY) {
		return true; 
		// is borderign move
		// can put self in check for the purposes of this program 
	}

	// add check logic?? and checkmate

	void getMove(int& startX, int& startY, int& endX, int& endY) {
		cout << endl << endl << "Enter move(e2 e4) ";
		string move;

		getline(cin, move);
		cout.flush();

		if (move.length() != 5 || move[2] != ' ') {
			cout << "Invalid move format  !! ";										  
			return;
		}
																				 
		startY = move[0] - 'a'; 
		startX = (move[1] - '0') - 1;

		endY = move[3] - 'a';   
		endX = (move[4] - '0') - 1;

	}


	void playGame() {
		int startX, startY, endX, endY;

		while (true) {
			cout << endl;
			printBoard(); 
			if (turn == WHITE) {
				cout << "White's ";
			}
			else {
				cout << "Black's ";
			}
			cout << "turn";

			getMove(startX, startY, endX, endY);

			if (movePiece(startX, startY, endX, endY)) {
				if (turn == WHITE) {
					turn = BLACK;
				}
				else {
					turn = WHITE;
				}
			}
		}
	}

	

};

int main() {
	ChessBoard board;
	board.playGame();
}