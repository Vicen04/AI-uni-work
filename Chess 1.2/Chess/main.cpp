#include <iostream>
using namespace std;

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
    #include <GL/glut.h>
#endif // GLUT Library
//Model
#include "Model.h"
//Chess Game
#include "Chess\Game.h"
#include "ChessPlayer.h"
//Window size and position
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_POS_X 50
#define WINDOW_POS_Y 50

#define BUTTON_X -100
#define BUTTON_Y -100
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 75

#define SLEEP_TIME_MILLISECONDS 200 /// 1000 = 1 second

/** Prototype **/
void keyFunction(unsigned char key, int x, int y);
void specialFunction(int key, int x, int y);

void newAITurn();


/**
    Variables for look at function
    eye     - position of eye
    center  - position of the center of view
    up      - up vertex
*/
GLfloat     eyeX = 2.0, eyeY = 0.0, eyeZ = -5.0,
            centerX = 0.0, centerY = 0.0, centerZ =0.0,
            upX = 0.0, upY = 0.0, upZ = -1.0;

/**
    Variables for perspective function
*/
GLfloat     fovy = 50.0f, zNear = 0.1f, zFar = 20.0f;

/**
    Variables for light
*/
GLfloat     position[] = {0.0f, 0.0f, 100.0f, 0.0f};
GLfloat     diffusion[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat     normal_board[] = {0.0f, 0.0f, 1.0f};
GLfloat     normal_valid_move[] = {0.0f, 0.0f, -1.0f};
float       ang = 0;
GLfloat mat_diffusion[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat mat_specular[] = {0.1f, 0.1f, 0.1f, 1.0f};

/**
    Variables for managing view
*/
GLfloat     screen_ratio, zoomOut = 2;

/**
    Model Loading
*/
Model   Pawn("model/cube.obj");
Model   Rook("model/cube.obj");
Model   Knight("model/cube.obj");
Model   Bishop("model/cube.obj");
Model   King("model/cube.obj");
Model   Queen("model/cube.obj");

/**
    Pre-start
*/
bool pressed = false;

/**
    Game Loading
*/
Game *chess;
Board* boardCopy = nullptr;
void newGame();

/**
    Real-time variables
*/
bool    inGame = false, verify = false;
int     selectedRow = 1, selectedCol = 1;
int     moveToRow = 1, moveToCol = 1;
bool    selected = false;
bool    board_rotating = true;
bool    ai_moving = false;
int     rotation = 0;
bool    check = false, checkMate = false, draw = false;
bool    closeGame = false;
bool	needPromote = false;

/**
    Chess board vertices
*/
GLfloat     chessBoard[12][3] = {{-4.0, -4.0, 0.5},
                                {-4.0,  4.0, 0.5},
                                { 4.0,  4.0, 0.5},
                                { 4.0, -4.0, 0.5},

                                {-4.5, -4.5, 0.5},
                                {-4.5,  4.5, 0.5},
                                { 4.5,  4.5, 0.5},
                                { 4.5, -4.5, 0.5},

                                {-5.0, -5.0, 0.0},
                                {-5.0,  5.0, 0.0},
                                { 5.0,  5.0, 0.0},
                                { 5.0, -5.0, 0.0}};


/**
*/
void showWord( int x, int y, string word)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WINDOW_WIDTH/2, WINDOW_WIDTH/2, -WINDOW_HEIGHT/2, WINDOW_HEIGHT/2, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    int l,i;

    l=word.length(); // see how many characters are in text string.
    glRasterPos2i(x, y); // location to start printing text
    glColor3f(1,1,0);
    for( i=0; i < l; i++) // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, word[i]); // Print a character on the screen
    }
}

/**
*/
void drawMoveToSquare()
{
    float r = 1.0f*(moveToRow-5), c = 1.0f*(moveToCol-5);
    if(selected)
    {
        glPushMatrix();
            glColor3f(0.5f, 1.0f, 0.0f);
            glTranslatef(r, c, 0.502f);
            glScalef(0.98f, 0.98f, 1.0f);
                glBegin(GL_TRIANGLES);
                    glNormal3fv(normal_valid_move);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(1.0f, 1.0f, 0.0f);
                    glVertex3f(0.0f, 1.0f, 0.0f);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(1.0f, 1.0f, 0.0f);
                    glVertex3f(1.0f, 0.0f, 0.0f);
                glEnd();
        glPopMatrix();
    }
    glColor3f(0, 0, 0);
}

/**
    Drawing a chess board using points from array "chessBoard"
*/
void drawChessBoard()
{
    glPushMatrix();
        /**Drawing bottom of the chess board*/
        glNormal3fv(normal_valid_move);
        glBegin(GL_QUADS);
            glColor3f(1.0, 0.0, 0.0);
            for(int i=8; i<12; i++) glVertex3fv(chessBoard[i]);
        glEnd();
        /**Drawing top of the chess board*/
        glBegin(GL_QUADS);
            glColor3f(0.55f, 0.24f, 0.09f);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[0]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[4]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[5]);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[1]);
        glEnd();
        glBegin(GL_QUADS);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[1]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[5]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[6]);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[2]);
        glEnd();
        glBegin(GL_QUADS);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[2]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[6]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[7]);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[3]);
        glEnd();
        glBegin(GL_QUADS);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[3]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[7]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[4]);
            glColor3f(0.803f, 0.522f, 0.247f);
            glVertex3fv(chessBoard[0]);
        glEnd();
        /**Drawing side of the chess board*/
        glBegin(GL_QUADS);
            glColor3f(1.0f, 0.95f, 0.9f);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[4]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[8]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[9]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[5]);
        glEnd();
        glBegin(GL_QUADS);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[5]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[9]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[10]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[6]);
        glEnd();
        glBegin(GL_QUADS);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[6]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[10]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[11]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[7]);
        glEnd();
        glBegin(GL_QUADS);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[7]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[11]);
            glColor3f(1.000f, 1.000f, 1.000f);
            glVertex3fv(chessBoard[8]);
            glColor3f(0.545f, 0.271f, 0.075f);
            glVertex3fv(chessBoard[4]);
        glEnd();
    glPopMatrix();
    glColor3f(0, 0, 0);
}

void drawBoardSquares()
{
	if (chess == nullptr)
		return;

    float r, c;
    for(int row = 1 ; row <= 8 ; row++)
    {
        for(int col = 1 ; col <= 8 ; col++)
        {
            r = 1.0f*(row-5);
            c = 1.0f*(col-5);
            if(row == selectedRow && col == selectedCol)
            {
                if(selected) glColor3f(0.33f, 0.420f, 0.184f);
                else if(chess->isSquareOccupied(selectedRow, selectedCol))
                    if(chess->getPieceColor(selectedRow, selectedCol) == chess->getTurnColor())
                        glColor3f(0.0f, 0.5f, 0.0f);
                    else glColor3f(1.0f, 0.0f, 0.0f);
                else glColor3f(0.3f, 0.7f, 0.5f);
            }
            else if((row + col) & 1) glColor3f(1.0, 1.0, 1.0);
            else glColor3f(0.0, 0.0, 0.0);
            glPushMatrix();
                glTranslatef(r, c, 0.5f);
                glBegin(GL_TRIANGLES);
                    glNormal3fv(normal_board);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(1.0f, 1.0f, 0.0f);
                    glVertex3f(0.0f, 1.0f, 0.0f);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(1.0f, 1.0f, 0.0f);
                    glVertex3f(1.0f, 0.0f, 0.0f);
                glEnd();
            glPopMatrix();
        }
    }
    glColor3f(0, 0, 0);
}

/**
    Draw Valid Moves of Selected Piece
*/
void drawValidMoves()
{
	if (chess == nullptr)
		return;


    if(selected)
    {
        std::vector<Move> validMoves = chess->getValidMoves(selectedRow, selectedCol);
        int vec_size = validMoves.size(), row, col;
        for(int id = 0; id < vec_size; id++)
        {
            row = validMoves[id].getDestinationPosition().first;
            col = validMoves[id].getDestinationPosition().second;
            switch(validMoves[id].getType())
            {
                case MoveType::NORMAL:
                    glColor3f(0.8f, 1.0f, 0.6f);
                    break;
                case MoveType::CAPTURE:
                    glColor3f(1.0f, 0.0f, 0.0f);
                    break;
                case MoveType::EN_PASSANT:
                    glColor3f(0.8f, 1.0f, 0.6f);
                    break;
                case MoveType::CASTLING:
                    glColor3f(0.196f, 0.804f, 0.196f);
                    break;
            }
            float r = 1.0f*(row-5), c = 1.0f*(col-5);
            glPushMatrix();
                    glTranslatef(r, c, 0.501f);
                    glScalef(0.99f, 0.99f, 1.0f);
                    glBegin(GL_TRIANGLES);
                        glNormal3fv(normal_valid_move);
                        glVertex3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(1.0f, 1.0f, 0.0f);
                        glVertex3f(0.0f, 1.0f, 0.0f);
                        glVertex3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(1.0f, 1.0f, 0.0f);
                        glVertex3f(1.0f, 0.0f, 0.0f);
                    glEnd();
            glPopMatrix();
        }
    }
    glColor3f(0, 0, 0);
}

/**
    Drawing Chess Pieces in Board
*/
void drawChessPieces()
{
	if (chess == nullptr)
		return;

    float z;
    for(int row = 1; row <= 8; row++)
    {
        for(int col = 1; col <= 8; col++)
        {
            if(chess->isSquareOccupied(row, col))
            {
                glPushMatrix();
                    if(selected && row == selectedRow && col == selectedCol) z = 1.0;
                    else z = 0.5;
                    glTranslatef((row - 5) * 1.0f + 0.5f, (col - 5) * 1.0f + 0.5f, z);
                    glScalef(0.01f, 0.01f, 0.01f);
                    switch(chess->getPieceColor(row, col))
                    {
                        case PieceColor::WHITE:
                            glRotatef(90, 0.0f, 0.0f, 1.0f);
							glColor3f(1.0f, 1.0f, 0.5f);

                            break;
                        case PieceColor::BLACK:
                            glRotatef(-90, 0.0f, 0.0f, 1.0f); 
							glColor3f(0.5f, 0.5f, 0.5f);
							
                            break;
                    }
                    switch(chess->getPiece(row, col)->getType())
                    {
					case PieceType::PAWN: Pawn.Draw(chess->getPieceColor(row, col) == PieceColor::WHITE); break;
                        case PieceType::ROOK: Rook.Draw(chess->getPieceColor(row, col) == PieceColor::WHITE); break;
                        case PieceType::KNIGHT: Knight.Draw(chess->getPieceColor(row, col) == PieceColor::WHITE); break;
                        case PieceType::BISHOP: Bishop.Draw(chess->getPieceColor(row, col) == PieceColor::WHITE); break;
                        case PieceType::QUEEN: Queen.Draw(chess->getPieceColor(row, col) == PieceColor::WHITE); break;
                        case PieceType::KING: King.Draw(chess->getPieceColor(row, col) == PieceColor::WHITE); break;
                    }
                glPopMatrix();
            }
        }
    }
    glColor3f(0, 0, 0);
}

void key_W_pressed(PieceColor color)
{
    switch(color)
    {
        case PieceColor::WHITE:
            if(!selected && selectedRow < 8) selectedRow++;
            if(selected && moveToRow < 8) moveToRow++;
            break;
        case PieceColor::BLACK:
            if(!selected && selectedRow > 1) selectedRow--;
            if(selected && moveToRow > 1) moveToRow--;
            break;
    }
}

void key_D_pressed(PieceColor color)
{
    switch(color)
    {
        case PieceColor::WHITE:
            if(!selected && selectedCol < 8) selectedCol++;
            if(selected && moveToCol < 8) moveToCol++;
            break;
        case PieceColor::BLACK:
            if(!selected && selectedCol > 1) selectedCol--;
            if(selected && moveToCol > 1) moveToCol--;
            break;
    }
}

void key_S_pressed(PieceColor color)
{
    switch(color)
    {
        case PieceColor::WHITE:
            if(!selected && selectedRow > 1) selectedRow--;
            if(selected && moveToRow > 1) moveToRow--;
            break;
        case PieceColor::BLACK:
            if(!selected && selectedRow < 8) selectedRow++;
            if(selected && moveToRow < 8) moveToRow++;
            break;
    }
}

void key_A_pressed(PieceColor color)
{
    switch(color)
    {
        case PieceColor::WHITE:
            if(!selected && selectedCol > 1) selectedCol--;
            if(selected && moveToCol > 1) moveToCol--;
            break;
        case PieceColor::BLACK:
            if(!selected && selectedCol < 8) selectedCol++;
            if(selected && moveToCol < 8) moveToCol++;
            break;
    }
}

void updateTurn(PieceColor color)
{
    switch(color)
    {
        case PieceColor::WHITE:
            selectedRow = 1;
            selectedCol = 8;
            break;
        case PieceColor::BLACK:
            selectedRow = 8;
            selectedCol = 1;
            break;
    }
}

#define BOARD_ROTATION 0 //4

void doRotationBoard(PieceColor color)
{
	board_rotating = false;
    switch(color)
    {
        case PieceColor::WHITE:
			if (rotation < 180) {
				rotation += BOARD_ROTATION;
			}
			else {
				board_rotating = false;
				
			}
            break;
        case PieceColor::BLACK:
			if (rotation < 360) {
				rotation += BOARD_ROTATION;
			}
            else {
                rotation = 0;
				board_rotating = false;
                
            }
            break;
    }
}

void endOfTurn()
{
    selected = false;
    needPromote = false;
	check = false;
	chess->nextTurn();
	vecPieces temp;
	if (chess->getBlackPlayer()->getAllLivePieces(temp) == 1 && chess->getWhitePlayer()->getAllLivePieces(temp) == 1)
		draw = true;
	
	
	if(chess->inCheckMateState())
	{
		checkMate = true;
	}
	else if(chess->inCheckState())
	{
		check = true;
	}
	board_rotating = true;
	updateTurn(chess->getTurnColor());
}

void newAITurn()
{
	PieceColor colour = chess->getTurnColor();
	ChessPlayer* player = nullptr, *rival = nullptr;
	if (colour == PieceColor::BLACK) {
		player = chess->getBlackPlayer();
		rival = chess->getWhitePlayer();
	}
	else {
		player = chess->getWhitePlayer();
		rival = chess->getBlackPlayer();
	}
	
	if (!player->isAI() || chess->inCheckMateState()) 
		return;

		// decide and make the move
		Move move;
		bool moveMade = player->chooseAIMove(&move);
		if (!moveMade)
		{
			return; // TODO: if this is the case - what next? 
		}

		Sleep(SLEEP_TIME_MILLISECONDS);

		ai_moving = true;

		selectedRow = move.getOriginPosition().first;
		selectedCol = move.getOriginPosition().second;
		keyFunction(' ', 0, 0);

		moveToRow = move.getDestinationPosition().first;
		moveToCol = move.getDestinationPosition().second;
		keyFunction(' ', 0, 0);
}

void displayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(inGame)
    {
        /**
            Changing view perspective
        */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, screen_ratio, zNear, zoomOut * zFar);
        /**
            Drawing model mode
        */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(zoomOut * eyeX, zoomOut * eyeY, zoomOut * eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

        /**
            Draw code here
        */
		if (board_rotating) {

			doRotationBoard(chess->getTurnColor());

			// take this as a signal to update the AI (if any)
			if(board_rotating == false) // a change of state - do the AI
				newAITurn();
		}
			

        GLfloat ambient_model[] = {0.5, 0.5, 0.5, 1.0};

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_model);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffusion);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusion);

        glScalef(1.0f, 1.0f, -1.0f);

        glLightfv(GL_LIGHT0, GL_POSITION, position);

        glRotatef((float)rotation, 0, 0, 1);

		
        drawChessBoard();

        drawBoardSquares();

		if(chess != nullptr) {
			drawChessPieces();

			drawMoveToSquare();

			drawValidMoves();

			if(needPromote)
			{
				showWord(-200, WINDOW_HEIGHT/2-24, "Promote to: (Q) Queen | (R) Rook | (B) Bishop | (K) Knight");
			}
			else if(verify) 
				showWord(-200, WINDOW_HEIGHT/2-24, "Are you sure to retry? Yes (O)  or  No (X)");
			else
			{
				if(check)
				{
					string s = chess->getTurnColor() == PieceColor::BLACK?"BLACK PIECE" : "WHITE PIECE";
					showWord(-150, WINDOW_HEIGHT/2-24, s+" CHECKED!");
				}
				if(checkMate || draw)
				{
					if (checkMate)
					{
						string s = chess->getTurnColor() == PieceColor::BLACK ? "WHITE PLAYER" : "BLACK PLAYER";
						showWord(-100, WINDOW_HEIGHT / 2 - 24, "CHECK MATE!");
						showWord(-140, WINDOW_HEIGHT / 2 - 50, s + " WIN!");
					}
					else
					showWord(-140, WINDOW_HEIGHT / 2 - 50, "DRAW!");
					showWord(-150, -WINDOW_HEIGHT/2+50, "Do you want to play again?");
					showWord(-120, -WINDOW_HEIGHT/2+25, "Yes (O)  or  No (X)");
				}
			}
		}
    }
    else
    {
        showWord(-150, 0, "- - Press N to Start The Game - -");
    }

	if(closeGame)
		glutExit();

    glutSwapBuffers();

    glutPostRedisplay();

	if (ai_moving) {
		ai_moving = false;
		Sleep(SLEEP_TIME_MILLISECONDS);
	}
}

void reshapeFunction(int width, int height)
{
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    screen_ratio = (GLfloat) width / (GLfloat) height;
}

void specialFunction(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            zoomOut += 0.1f;
            break;
        case GLUT_KEY_DOWN:
            zoomOut -= 0.1f;
            break;
        case GLUT_KEY_LEFT:
            ang += 5;
            break;
        case GLUT_KEY_RIGHT:
            ang -= 5;
            break;
        default: break;
    }
}

void keyFunction(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
        case 'W':
            if(!needPromote && !checkMate && !draw && !verify && inGame && !board_rotating) key_W_pressed(chess->getTurnColor());
            break;
        case 'a':
        case 'A':
            if(!needPromote && !checkMate && !draw && !verify && inGame && !board_rotating) key_A_pressed(chess->getTurnColor());
            break;
        case 's':
        case 'S':
            if(!needPromote && !checkMate && !draw && !verify && inGame && !board_rotating) key_S_pressed(chess->getTurnColor());
            break;
        case 'd':
        case 'D':
            if(!needPromote && !checkMate && !draw && !verify && inGame && !board_rotating) key_D_pressed(chess->getTurnColor());
            break;
        case ' ':
            if(!needPromote && !checkMate && !draw && !verify && inGame && !board_rotating)
            {
                if(selected)
                {
					// the selected piece is being moved
                    if(chess->move(selectedRow, selectedCol, moveToRow, moveToCol))
                    {
						Piece* movedPiece = chess->getPiece(moveToRow, moveToCol);
						if(movedPiece->getType() == PieceType::PAWN &&
							((movedPiece->getColor() == PieceColor::BLACK && moveToRow == chess->getBoard()->MIN_ROW_INDEX)
							|| moveToRow == chess->getBoard()->MAX_ROW_INDEX))
						{
							needPromote = true;
						}
						if(needPromote)
							if (ai_moving)
							{
								chess->promote(moveToRow, moveToCol, PieceType::QUEEN);
								if (chess->getTurnColor() == PieceColor::BLACK) {
									vecPieces vpiecesBlack;
									unsigned int numPiecesBlack = chess->getBlackPlayer()->getAllLivePieces(vpiecesBlack);
									chess->getWhitePlayer()->SetAllRivalPieces(vpiecesBlack, numPiecesBlack);
								}
								else {
									vecPieces vpiecesWhite;
									unsigned int numPiecesWhite = chess->getWhitePlayer()->getAllLivePieces(vpiecesWhite);
									chess->getBlackPlayer()->SetAllRivalPieces(vpiecesWhite, numPiecesWhite);
									
								}
							}
							else
							break;
                        endOfTurn();
                    }
                    selected = false;
                }
                else if(chess->isSquareOccupied(selectedRow, selectedCol) && chess->getPieceColor(selectedRow, selectedCol) == chess->getTurnColor())
                {
					// a piece is being selected 
                    selected = !selected;
                    if(selected)
                    {
                        moveToRow = selectedRow;
                        moveToCol = selectedCol;
                    }
                }
            }
            break;
        case 'n':
        case 'N':
            if(!inGame)newGame();
            else verify = true;
            break;
        case 'o': case 'O':
            if(checkMate || verify || draw) {delete chess; newGame(); verify = false;}
            break;
        case 'x': case 'X':
			if(checkMate || draw) 
			{ 
				closeGame = true; 
				delete chess;
				chess = nullptr;
			}
            if(verify) 
			{
				verify = false;
			}
            break;
		case 'q': case 'Q':
			if(needPromote)
			{
				chess->promote(moveToRow, moveToCol, PieceType::QUEEN);
				endOfTurn();
				break;
			}
			else break;
		case 'r': case 'R':
			if(needPromote)
			{
				chess->promote(moveToRow, moveToCol, PieceType::ROOK);
				endOfTurn();
				break;
			}
			else break;
		case 'b': case 'B':
			if(needPromote)
			{
				chess->promote(moveToRow, moveToCol, PieceType::BISHOP);
				endOfTurn();
				break;
			}
			else break;
		case 'k': case 'K':
			if(needPromote)
			{
				chess->promote(moveToRow, moveToCol, PieceType::KNIGHT);
				endOfTurn();
				break;
			}
			else break;
		case 'c': case 'C':
			boardCopy = chess->getCopyOfBoard();
			break;
		case 'v': case 'V':
			if(boardCopy != nullptr)
				chess->replaceBoard(boardCopy);
			break;

		default: break;
    }
}

void initialize()
{
	Pawn.Initialise("model/pawn_black.jpg", "model/pawn_white.jpg");
	Rook.Initialise("model/rook_black.jpg", "model/rook_white.jpg");
	Knight.Initialise("model/knight_black.jpg", "model/knight_white.jpg");
	Bishop.Initialise("model/bishop_black.jpg", "model/bishop_white.jpg");
	King.Initialise("model/king_black.jpg", "model/king_white.jpg");
	Queen.Initialise("model/queen_black.jpg", "model/queen_white.jpg");

    glClearColor(0.2f, 0.6f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void newGame()
{
    chess = new Game();
    selectedRow = 1; selectedCol = 1;
    moveToRow = 1; moveToCol = 1;
    selected = false;
    board_rotating = true;
    rotation = 0;
    inGame = true;
    check = false;
    checkMate = false;
	draw = false;
    updateTurn(chess->getTurnColor());
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("Chess");

    initialize();
    glutDisplayFunc(displayFunction);
    glutReshapeFunc(reshapeFunction);
    glutKeyboardFunc(keyFunction);
    glutSpecialFunc(specialFunction);
    glutMainLoop();
	return 0;
}
