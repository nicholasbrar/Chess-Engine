#include "main.h"

using namespace std;

ChessBoard::ChessBoard() {
	initializeBoard();
	turn = WHITE; 
}

void ChessBoard::initializeBoard() {
	for (int i = 0; i < SIZE; i++) {	 // Initialize pawns
		board[6][i] = { PAWN, BLACK };
		board[1][i] = { PAWN, WHITE };
	}

	board[0][0] = board[0][7] = { ROOK, WHITE }; // Initialize rooks
	board[7][0] = board[7][7] = { ROOK, BLACK };

	board[0][1] = board[0][6] = { KNIGHT, WHITE }; // Initialize knights
	board[7][1] = board[7][6] = { KNIGHT, BLACK };

	board[0][2] = board[0][5] = { BISHOP, WHITE }; // Initialize bishops
	board[7][2] = board[7][5] = { BISHOP, BLACK };

	board[0][3] = { QUEEN, WHITE }; // Initialize kings / queens
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

char ChessBoard::getPieceCharacter(PIECE piece, COLOR color) {
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

void ChessBoard::printBoard() {
	for (int i = SIZE - 1; i >= 0; i--) {
		for (int j = 0; j < SIZE; j++) {
			cout << getPieceCharacter(board[i][j].piece, board[i][j].color) << " ";
		}
		cout << endl;
	}
	}

bool ChessBoard::isValidKingsideCastle(COLOR color) {
	if (color == WHITE) {
		if (whiteKingMoved || whiteRooksMoved[1]) { // Ensure white king/rook haven't moved
			return false;
		}
		if (board[0][5].piece != NONE || board[0][6].piece != NONE) {
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
		if (board[7][5].piece != NONE || board[7][6].piece != NONE) {
			return false;
		}
		if (kingInCheck(BLACK)) {  // Can't castle in check
			return false;
		}
		if (squareIsAttacked(7, 5, WHITE) || squareIsAttacked(7, 6, WHITE)) {
			return false;
		}
	}
	return true;

}

	bool ChessBoard::isValidQueensideCastle(COLOR color) {
		if (color == WHITE) {
			if (whiteKingMoved || whiteRooksMoved[0]) { // Ensure white king/rook haven't moved
				return false;
			}
			if (board[0][1].piece != NONE || board[0][2].piece != NONE || board[0][3].piece != NONE) {
				return false;
			}
			if (kingInCheck(WHITE)) {  // Can't castle in check
				return false;
			}
			if (squareIsAttacked(0, 2, BLACK) || squareIsAttacked(0, 3, BLACK)) {
				return false;
			}

			return true;
		}
		else if (color == BLACK) {
			if (blackKingMoved || blackRooksMoved[0]) { // Ensure black king/rook haven't moved
				return false;
			}
			if (board[7][1].piece != NONE || board[7][2].piece != NONE || board[7][3].piece != NONE) {
				return false;
			}
			if (kingInCheck(BLACK)) {  // Can't castle in check
				return false;
			}
			if (squareIsAttacked(7, 2, WHITE) || squareIsAttacked(7, 3, WHITE)) {
				return false;
			}
		}
		return true;
	}

bool ChessBoard::movePiece(int startX, int startY, int endX, int endY) {
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
				board[7][4] = { NONE, NOCOLOR };								  
				board[7][7] = { NONE, NOCOLOR };
				board[7][5] = { ROOK, BLACK };
				board[7][6] = { KING, BLACK };
			}
			return true;
		}
	}
	else if (startX == -2 && startY == -2 && endX == -2 && endY == -2) { // Queenside Castle
		if (isValidQueensideCastle(turn)) {
			if (turn == WHITE) {
				whiteKingMoved = true;
				whiteRooksMoved[0] = true;
				board[0][4] = { NONE, NOCOLOR };
				board[0][0] = { NONE, NOCOLOR };
				board[0][3] = { ROOK, WHITE };
				board[0][2] = { KING, WHITE };
			}
			else {
				blackKingMoved = true;
				blackRooksMoved[0] = true;
				board[7][4] = { NONE, NOCOLOR };
				board[7][0] = { NONE, NOCOLOR };
				board[7][3] = { ROOK, BLACK };
				board[7][2] = { KING, BLACK };
			}


		}
		return true;
	}

	if (isValidMove(startX, startY, endX, endY)) {
		SQUARE start = board[startX][startY];
		SQUARE end = board[endX][endY];
		board[endX][endY] = board[startX][startY];
		board[startX][startY] = { NONE, NOCOLOR };

		if ((turn == WHITE && endX == 7 && board[endX][endY].piece == PAWN) || (turn == BLACK && endX == 0 && board[endX][endY].piece == PAWN)) {  // Piece promotion
			promotePawn(endX, endY);
		}

		return true;
	}
	else {
		cout << "INVALID MOVE" << endl;
		return false;
	}

}

void ChessBoard::promotePawn(int x, int y) {
	char input;
	cout << "Choose piece to promote to (Q/B/N/B): ";
	cin >> input;

	PIECE promotionPiece;
	switch (input) {
	case 'Q':
	case 'q': promotionPiece = QUEEN; break;
	case 'R':
	case 'r': promotionPiece = ROOK; break;
	case 'B':
	case 'b': promotionPiece = BISHOP; break;
	case 'N':
	case 'n': promotionPiece = KNIGHT; break;
	default:
		cout << "Invalid choice. Will promote to Queen by default." << endl;
		promotionPiece = QUEEN;
	}

	board[x][y].piece = promotionPiece;
}

bool ChessBoard::isValidMove(int startX, int startY, int endX, int endY) {
	SQUARE start = board[startX][startY];
	SQUARE end = board[endX][endY];

	if (start.piece == NONE) return false; // Can't move a nonexistent piece
	if (start.color != turn) return false; // Can't move other color's piece
	if (end.color == turn) return false; // Can't capture your own piece

	bool valid = false;

	switch (start.piece) {
	case PAWN: valid = isValidPawnMove(startX, startY, endX, endY, start.color); break;
	case KNIGHT: valid = isValidKnightMove(startX, startY, endX, endY); break;
	case BISHOP: valid = isValidBishopMove(startX, startY, endX, endY);	break;
	case ROOK: valid = isValidRookMove(startX, startY, endX, endY); break;
	case QUEEN: valid = isValidQueenMove(startX, startY, endX, endY); break;
	case KING: valid = isValidKingMove(startX, startY, endX, endY); break;
	default: return false;
	}

	if (!valid) return false;

	return !moveLeavesKingInCheck(startX, startY, endX, endY);
}

bool ChessBoard::isValidPawnMove(int startX, int startY, int endX, int endY, COLOR color) {
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

bool ChessBoard::isValidKnightMove(int startX, int startY, int endX, int endY) {
	int dx = abs(endX - startX);
	int dy = abs(endY - startY);

	if (board[endX][endY].color == turn) {   // can't capture own piece
		return false;
	}
	return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

bool ChessBoard::isValidBishopMove(int startX, int startY, int endX, int endY) {
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

bool ChessBoard::isValidRookMove(int startX, int startY, int endX, int endY) {
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
			whiteRooksMoved[0] = true; // Queenside White Rook
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

bool ChessBoard::isValidQueenMove(int startX, int startY, int endX, int endY) {
	return (isValidRookMove(startX, startY, endX, endY) || isValidBishopMove(startX, startY, endX, endY));
}

bool ChessBoard::isValidKingMove(int startX, int startY, int endX, int endY) {
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


void ChessBoard::getMove(int& startX, int& startY, int& endX, int& endY) {
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


void ChessBoard::playGame() {
	while (true) {
		cout << endl;
		printBoard();

		COLOR currentPlayer = turn;

		bool inCheck = false;

		if (kingInCheck(currentPlayer)) {
			if (isCheckmate(currentPlayer)) {
				cout << "Checkmate " << (currentPlayer == WHITE ? "Black" : "White") << " wins!" << endl;
				break;
			}
			else {
				cout << (currentPlayer == WHITE ? "\nWhite" : "\nBlack") << " is in check!" << endl;
				inCheck = true;
			}
		}
		else if (isStalemate(currentPlayer)) {
			cout << "Draw by stalemate" << endl;
		}

		if (!inCheck) {
			cout << endl << (turn == WHITE ? "White's turn" : "Black's turn");
		}
	
		int startX, startY, endX, endY;
		getMove(startX, startY, endX, endY);

		if (movePiece(startX, startY, endX, endY)) {
			turn = (turn == WHITE) ? BLACK : WHITE;
		}

	}
}

pair<int, int> ChessBoard::locateKing(COLOR color, const SQUARE(&tempBoard)[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (tempBoard[i][j].piece == KING && tempBoard[i][j].color == color) {
				return { i, j };
			}
		}
	}
	return { -1, -1 };
}

pair<int, int> ChessBoard::locateKing(COLOR color) {
	return locateKing(color, board);
}

bool ChessBoard::squareIsAttacked(int x, int y, COLOR enemyColor, const SQUARE(&tempBoard)[SIZE][SIZE]) {
	bool attacked = false;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (tempBoard[i][j].color == enemyColor && tempBoard[i][j].piece != NONE) {
				switch (tempBoard[i][j].piece) {
				case BISHOP:
					if (canBishopAttack(i, j, x, y)) return true;
					break;
				case KNIGHT:
					if (canKnightAttack(i, j, x, y)) return true;
					break;
				case ROOK:
					if (canRookAttack(i, j, x, y)) return true;
					break;
				case PAWN:
					if (canPawnAttack(i, j, x, y, enemyColor)) return true;
					break;
				case QUEEN:
					if (canQueenAttack(i, j, x, y)) return true;
					break;
				case KING:
					if (canKingAttack(i, j, x, y)) return true;
					break;
				default: break;
				}
			}
		}
	}
	return false;
}

bool ChessBoard::squareIsAttacked(int x, int y, COLOR enemyColor) {
	return squareIsAttacked(x, y, enemyColor, board);
}

bool ChessBoard::kingInCheck(COLOR color) {
	pair<int, int> kingLocation = locateKing(color);
	COLOR enemyColor = (color == WHITE ? BLACK : WHITE);
	return(squareIsAttacked(kingLocation.first, kingLocation.second, enemyColor));
}

bool ChessBoard::moveLeavesKingInCheck(int startX, int startY, int endX, int endY) {
	SQUARE tempBoard[SIZE][SIZE];
	memcpy(tempBoard, board, sizeof(SQUARE) * SIZE * SIZE);

	SQUARE startPiece = tempBoard[startX][startY];
	tempBoard[startX][startY] = { NONE, NOCOLOR };
	tempBoard[endX][endY] = startPiece;

	COLOR currentColor = turn;
	pair<int, int> kingLocation;

	if (startPiece.piece == KING && startPiece.color == currentColor) {
		kingLocation = { endX, endY };
	}
	else {
		kingLocation = locateKing(currentColor, tempBoard);
	}

	COLOR enemyColor = (currentColor == WHITE) ? BLACK : WHITE;
	return squareIsAttacked(kingLocation.first, kingLocation.second, enemyColor, tempBoard);
}

bool ChessBoard::hasLegalMoves(COLOR color) {
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

bool ChessBoard::isStalemate(COLOR color) {
	return !kingInCheck(color) && !hasLegalMoves(color);
}

bool ChessBoard::isCheckmate(COLOR color) {
	return kingInCheck(color) && !hasLegalMoves(color);
}

bool ChessBoard::canBishopAttack(int startX, int startY, int endX, int endY) {
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

bool ChessBoard::canKnightAttack(int startX, int startY, int endX, int endY) {
	int dx = abs(endX - startX);
	int dy = abs(endY - startY);

	return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

bool ChessBoard::canRookAttack(int startX, int startY, int endX, int endY) {
	int dx = abs(startX - endX);
	int dy = abs(startY - endY);

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
	return true;
}

bool ChessBoard::canPawnAttack(int startX, int startY, int endX, int endY, COLOR color) {
	int direction = (color == WHITE) ? 1 : -1;

	if (abs(startY - endY) == 1 && endX == startX + direction) {
		return true;
	}
	return false;
}

bool ChessBoard::canQueenAttack(int startX, int startY, int endX, int endY) {
	return canRookAttack(startX, startY, endX, endY) || canBishopAttack(startX, startY, endX, endY);
}

bool ChessBoard::canKingAttack(int startX, int startY, int endX, int endY) {
	int dx = abs(startX - endX);
	int dy = abs(startY - endY);

	return((dx <= 1 && dy <= 1) && (dx != 0 || dy != 0));
}


int main() {
	ChessBoard board;
	board.playGame();
	return 0;

}