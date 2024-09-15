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

	bool whiteKingMoved = false;
	bool blackKingMoved = false;
	bool whiteRooksMoved[2] = { false, false };
	bool blackRooksMoved[2] = { false, false };
	
	ChessBoard() {
		initializeBoard();		
		turn = WHITE; // White always goes first in chess
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

	bool isValidKingsideCastle(COLOR color) {
		cout << "ASFDSF";
		if (color == WHITE) {
			if (whiteKingMoved || whiteRooksMoved[1]) { // Ensure white king/rook haven't moved
				return false;
			}
			cout << "TEST";
			if (board[0][5].piece != NONE || board[0][6] != NONE) {
				return false;
			}
			if (kingInCheck(WHITE)) {  // Can't castle in check
				return false;
			}
			if (squareIsAttacked(0, 5, BLACK) || squareIsAttacked(0, 6, BLACK)) {
				return false;
			}
			
			return true;
		}
		else if (color == BLACK) {
			if (blackKingMoved || blackRooksMoved[1]) { // Ensure black king/rook haven't moved
				return false;
			}
			if (kingInCheck(BLACK)) {  // Can't castle in check
				return false;
			}
		}
		return true;
		
	}

	bool isValidQueensideCastle(COLOR color) {

	}

	bool movePiece(int startX, int startY, int endX, int endY) {
		cout << "movePiece called with startX=" << startX << ", startY=" << startY << ", endX=" << endX << ", endY=" << endY << endl;
		if (startX == -1 && startY == -1 && endX == -1 && endY == -1) { // Kingside castle
			if (isValidKingsideCastle(turn)) {
				if (turn == WHITE) {
					whiteKingMoved = true;
					whiteRooksMoved[1] = true;
					board[0][4] = { NONE,NOCOLOR };
					board[0][7] = { NONE, NOCOLOR };
					board[0][5] = { ROOK, WHITE };
					board[0][6] = { KING, WHITE };
				}
				else {
					blackKingMoved = true;
					blackRooksMoved[1] = true;
					// black moves
				}
				return true;
			}
		}
		else if (startX == -2 && startY == -2 && endX == -2 && endY == -2) { // Queenside Castle
			if (isValidQueensideCastle(turn)) {
				// move pieces

		
			}
		}

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

		int pawnStart = (color == WHITE) ? 1 : 6; // Takes the value of the starting rank of pawns (1 for White, 6 for Black)

		if (startY == endY && board[endX][endY].piece == NONE) { // Ensure pawn isn't moving to a piece occupied square (of either color, that is) 
			if (endX == startX + direction) { // Move 1 square
				return true;
			}

			if (startX == pawnStart && endX == startX + 2 * direction) { // Moving 2 squares (ensuring its the pawn's first move of the game)
				if (board[endX][endY].piece == NONE && board[startX + direction][startY].piece == NONE) {
					return true;
				}
			}
		}

		if (abs(startY - endY) == 1 && endX == startX + direction && board[endX][endY].piece != NONE) {
			if (board[endX][endY].color != color) {
				return true;
			}
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
		if (board[endX][endY].color == turn) { // Can't capture own piece
			return false;
		}
		if (abs(startX - endX) != abs(startY - endY)) {	// Diagonal movement -- dx must = dy 
			return false;
		}
		
		int xDirection = (endX > startX) ? 1 : -1;
		int yDirection = (endY > startY) ? 1 : -1;

		int x = xDirection + startX;
		int y = yDirection + startY;

		while (x != endX && y != endY) {
			if (board[x][y].piece != NONE) {
				return false;
			}
			x += xDirection, 
			y += yDirection;
		}
		return true;
	}

	bool isValidRookMove(int startX, int startY, int endX, int endY) {
		int dx = abs(startX - endX);
		int dy = abs(startY - endY);

		if (board[endX][endY].color == turn) { // Can't capture own piece
			return false;
		}

		if (dx != 0 && dy != 0) { // Can only move horizontally OR vertically
			return false;
		}

		if (dx == 0) { // Vertical rook move
			int yDirection = (endY > startY) ? 1 : -1;
			int y = startY + yDirection;

			while (y != endY) {
				if (board[startX][y].piece != NONE) {
					return false;
				}
				y += yDirection;
			}
		}
		else if (dy == 0) { // Horizontal rook move
			int xDirection = (endX > startX) ? 1 : -1;
			int x = startX + xDirection;

			while (x != endX) {
				if (board[x][startY].piece != NONE) {
					return false;
				}
				x += xDirection;
			}

		}
		if (turn == WHITE) {
			if (startX == 0 && startY == 0) {
				whiteRooksMoved[0] == true; // Queenside White Rook
			}
			else if (startX == 0 && startY == 7) {		
				whiteRooksMoved[1] = true; // Kingside White Rook
			}
		}
		else if (turn == BLACK) {
			if (startX == 7 && startY == 0) { // Queenside Black Rook
				blackRooksMoved[0] = true;
			}
			else if (startX == 7 && startY == 7) { // Kingside Black Rook
				blackRooksMoved[1] = true;
			}
		}
		return true;
	}

	bool isValidQueenMove(int startX, int startY, int endX, int endY) {
		return (isValidRookMove(startX, startY, endX, endY) || isValidBishopMove(startX, startY, endX, endY));
	}

	bool isValidKingMove(int startX, int startY, int endX, int endY) {
		int dx = abs(endX - startX);
		int dy = abs(endY - startY);

		if (board[endX][endY].color == turn) {
			return false;
		}
		
		if ((dx <= 1 && dy <= 1) && (dx != 0 || dy != 0)) {
			if (turn == WHITE) {
				whiteKingMoved = true;
			}
			else if (turn == BLACK) {
				blackKingMoved = true;
			}
			return true;
		}
		return false;
	}


	void getMove(int& startX, int& startY, int& endX, int& endY) {
		cout << endl << endl << "Enter move(e2 e4) ";
		string move;

		getline(cin, move);
		cout.flush();

		if (move.length() != 5 || move[2] != ' ') {
			if (move != "O-O" && move != "O-O-O") {
				cout << "Invalid move format!!";
				return;
			}
		}
			
		if (move == "O-O") {   // Kingside Castle
			startX = -1, startY = -1, endX = -1, endY = -1;
			return;
		}
		else if (move == "O-O-O") {
			startX = -2, startY = -2, endX = -2, endY = -2;
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
			cout << endl;

			if (turn == WHITE) {
				cout << "White's ";
			}
			else {
				cout << "Black's ";
			}
			cout << "turn";

			getMove(startX, startY, endX, endY);

			if (movePiece(startX, startY, endX, endY)) {
				turn = (turn == WHITE) ? BLACK : WHITE;
			}
		}
	}	

	pair<int, int> locateKing(COLOR color) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (board[i][j].piece == KING && board[i][j].color == color) {
					return { i, j };
				}
			}
		}
		return { -1, -1 };
	}

	bool squareIsAttacked(int x, int y, COLOR enemyColor) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (board[i][j].color == enemyColor && board[i][j].piece != NONE) {
					if (isValidMove(i, j, x, y)) {
						return true;
					}
				}
			}
		}
		return false;
	}
	
	bool kingInCheck(COLOR color) {
		pair<int, int> kingLocation = locateKing(color);
		COLOR enemyColor = (color == WHITE ? BLACK : WHITE); 
		return(squareIsAttacked(kingLocation.first, kingLocation.second, enemyColor));
	}

	bool hasLegalMoves(COLOR color) {
		for (int i = 0; i < SIZE; i++) {					// Checks all possible moves a color can make and determines if any are possible
			for (int j = 0; j < SIZE; j++) {
				if (board[i][j].color == color) {
					for (int x = 0; x < SIZE; x++) {
						for (int y = 0; y < SIZE; y++) {
							if (isValidMove(i, j, x, y)) {
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	bool isStalemate(COLOR color) {
		if (!kingInCheck(color) && !hasLegalMoves(color)) {
			return true;
		}
		return false;
	}

	bool isCheckmate(COLOR color) {
		if (kingInCheck(color) && !hasLegalMoves(color)) {
			return true;
		}
		return false;
	}
};

int main() {
	ChessBoard board;
	board.playGame();
}