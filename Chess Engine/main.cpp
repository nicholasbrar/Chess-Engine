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

	ChessBoard() {
		initializeBoard();											
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

};

int main() {
	ChessBoard board;
	board.printBoard();
}