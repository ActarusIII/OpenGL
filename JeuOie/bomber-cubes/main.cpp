#define _SCL_SECURE_NO_WARNINGS

#include <string>

#include "window.h"
#include "input.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"
#include "camera.h"
#include "quaternion.h"
#include "maths_3d.h"
#include "Game.h"
#include <random>
#include <cmath>



// Setting up various global variables
//jeu de l'oie
Game game = Game(25);
int currentPlayer = 1;
int de;
Player player1(game);
Player player2(game);
std::string displayString = "";
char* displayRules = "Player 1 plays with TAB, Player 2 plays with SPACE.";
char* specialDisplay = "";
char* displayPlayer1 = "Player 1";
char* displayPlayer2 = "Player 2";
bool isGameOver = false;

Model3D  *geometrie_raptor = NULL;
Texture  *texture_raptor = NULL;
Texture  *texture_raptor2 = NULL;

float sizeSquareX = 50.0f;
float sizeSquareY = 30.0f;
float sizeSquareZ = 50.0f;

float colorSquareDestroyableR = 0.7f;
float colorSquareDestroyableG = 0.7f;
float colorSquareDestroyableB = 0.7f;

float colorSquareR = 0.3f;
float colorSquareG = 0.3f;
float colorSquareB = 0.3f;

int nbSquareX = game.nombreCases;
int nbSquareZ = 11;

int sprite1PositionX = 0;
int sprite1PositionZ = 0;
int sprite2PositionX = 0;
int sprite2PositionZ = 0;

// Defining the size of the window
WINDOW *win = NULL;
int sizeScreenX = 1600;
int sizeScreenY = 900;

// Handling inputs and timer
MY_INPUT *inp = NULL;
TIMER *tim = NULL;

// Initializing the camera
CAMERA *cam;
float posXCam = nbSquareX * sizeSquareX / 2 ;
float posYCam = 350.0f;
float posZCam = -150.0f;
float targetPosX = nbSquareX * sizeSquareX / 2;
float targetPosY = -150.0f;
float targetPosZ = nbSquareZ * sizeSquareZ / 2;



bool start() {
	// Setting up the window
	win = new WINDOW();
	win->create(sizeScreenX, sizeScreenY, 32, 60, false);
	
	// Setting up the camera
	cam = new CAMERA();

	// Creating the timer
	tim = new TIMER();

	// Handling keyboard and mouse inputs
	inp = new MY_INPUT(win);


	// Finishing the basic setup
	create_context(*win);								// Creating OpenGL context in the window
	init_font(*win, "Courier");							// Initializing text management using OpenGL

  
	// OpenGL setup
	glClearColor(0.0f,0.0f,0.0f,1.0f);					// Set clear color for color buffer (RGBA, black)

	glViewport(0,0,win->Xres,win->Yres);				// Render zone (screen wide)

 	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Best mapping quality
	glFrontFace(GL_CCW);								// Front of face is defined counter clock wise
	glPolygonMode(GL_FRONT, GL_FILL);					// Front of a face is filled
	glPolygonMode(GL_BACK,  GL_FILL);					// Back of a face is filled
	glCullFace(GL_BACK);								// Cull back face only
	glDisable(GL_CULL_FACE);						    // Disable back face culling

	geometrie_raptor = new Model3D();
	geometrie_raptor->load_3d_model("data/RAPTOR.off");
	texture_raptor = new Texture();
	texture_raptor->load_texture("data/RAPTOR3.tga", NULL);
	glGenTextures(1, texture_raptor->OpenGL_ID);
	texture_raptor2 = new Texture();
	texture_raptor2->load_texture("data/RAPTOR2.tga", NULL);
	glGenTextures(1, texture_raptor2->OpenGL_ID);

	// Naming the window
	win->set_title("Jeu du raptor");

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Repeating texture in case U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			// or if < 0.0

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			// Mixing up the texture and the color

	return true;
}





//fonction retournant un entier entre les deux bornes en argument
int d(int const borne1, int const borne2)
{
	static std::random_device rd;
	static std::default_random_engine engine(rd());
	std::uniform_int_distribution<> distribution(borne1, borne2);
	return distribution(engine);
}
void displaySquare(float posX, float posY, float posZ, float lengthX, float lengthZ, float colorR, float colorG, float colorB) {
	// Defining a quad in the asked size
	glBegin(GL_QUADS);

	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX, posY, posZ);

	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX + lengthX, posY, posZ);

	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX + lengthX, posY, posZ + lengthZ);

	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX, posY, posZ + lengthZ);

	glEnd();
}


void displayCube(float posX, float posY, float posZ, float lengthX, float lengthY, float lengthZ, float colorR, float colorG, float colorB) {
	// Front side
	glBegin(GL_QUADS);
	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX, posY + lengthY, posZ);
	glVertex3f(posX, posY, posZ);
	glVertex3f(posX + lengthX, posY, posZ);
	glVertex3f(posX + lengthX, posY + lengthY, posZ);
	glEnd();

	// Right side
	glBegin(GL_QUADS);
	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX + lengthX, posY, posZ);
	glVertex3f(posX + lengthX, posY + lengthY, posZ);
	glVertex3f(posX + lengthX, posY + lengthY, posZ + lengthZ);
	glVertex3f(posX + lengthX, posY, posZ + lengthZ);
	glEnd();

	// Left side
	glBegin(GL_QUADS);
	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX, posY, posZ + lengthZ);
	glVertex3f(posX, posY + lengthY, posZ + lengthZ);
	glVertex3f(posX, posY + lengthY, posZ);
	glVertex3f(posX, posY, posZ);
	glEnd();

	// Top side
	glBegin(GL_QUADS);
	glColor3f(colorR, colorG, colorB);
	glVertex3f(posX + lengthX, posY + lengthY, posZ + lengthZ);
	glVertex3f(posX + lengthX, posY + lengthY, posZ);
	glVertex3f(posX, posY + lengthY, posZ);
	glVertex3f(posX, posY + lengthY, posZ + lengthZ);
	glEnd();
}

//ne marche que pour le cas particulier de nbCases=25
void updateSpritePosition(int numero) {
	if (numero == 1) {
		if (player1.currentCase.numero < 5) {
			sprite1PositionX = 240*player1.currentCase.numero;
			sprite1PositionZ = 0;
		}
		else if (player1.currentCase.numero >= 20) {
			sprite1PositionX = 240 * (player1.currentCase.numero-10);
			sprite1PositionZ = 0;
		}
		else if ((player1.currentCase.numero >= 10 && player1.currentCase.numero < 15)) {
			sprite1PositionX = 240 * (player1.currentCase.numero-5);
			sprite1PositionZ = -250 * 5;
		}
		else if (player1.currentCase.numero < 10) {
			int i= player1.currentCase.numero-4;
			sprite1PositionZ = -i * 250;
			sprite1PositionX = 4*250;
		}
		else {
			int i = 19 - player1.currentCase.numero;
			sprite1PositionZ = -i * 250;
			sprite1PositionX = 240 * 9;
			}
	}
	else {
		if (player2.currentCase.numero < 5) {
			sprite2PositionX = 240 * player2.currentCase.numero;
			sprite2PositionZ = 0;
		}
		else if (player2.currentCase.numero >= 20) {
			sprite2PositionX = 240 * (player2.currentCase.numero - 10);
			sprite2PositionZ = 0;
		}
		else if ((player2.currentCase.numero >= 10 && player2.currentCase.numero < 15)) {
			sprite2PositionX = 240 * (player2.currentCase.numero - 5);
			sprite2PositionZ = 250 * 5;
		}
		else if (player2.currentCase.numero < 10) {
			int i = player2.currentCase.numero - 4;
			sprite2PositionZ = i * 250;
			sprite2PositionX = 4 * 250;
		}
		else {
			int i = 19 - player2.currentCase.numero;
			sprite2PositionZ = i * 250;
			sprite2PositionX = 240 * 9;
		}
	}
}
void displayRaptor(int numero, int currentCase)
{
	float offset = 0;
	float zmove = 0;
	float xmove = 0;
	if (numero == 1) {
		zmove = sprite1PositionZ;
		xmove = sprite1PositionX;
	}
	else {
		zmove = sprite2PositionZ;
		xmove = sprite2PositionX;
	}
	glEnable(GL_TEXTURE_2D);
	if (numero == 2) {
		glBindTexture(GL_TEXTURE_2D, texture_raptor2->OpenGL_ID[0]);
		offset = 950;
	}
	else {
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_raptor->OpenGL_ID[0]);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap

	if (numero == 2) {
		if (texture_raptor2->isRGBA)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_raptor2->img_color->lenx, texture_raptor2->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_raptor2->img_all);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_raptor2->img_color->lenx, texture_raptor2->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_raptor2->img_color->data);
	}
	else {
		if (texture_raptor->isRGBA)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_raptor->img_color->lenx, texture_raptor->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_raptor->img_all);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_raptor->img_color->lenx, texture_raptor->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_raptor->img_color->data);
	}

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();


	glRotatef(270, 0,0, 1);
	glRotatef(-90, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);

	glTranslatef(850+offset+zmove, 8600-xmove, 250); //TODO

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < geometrie_raptor->nb_triangles; i++) {
		glTexCoord2f(geometrie_raptor->points[geometrie_raptor->faces[i].a].ucol, geometrie_raptor->points[geometrie_raptor->faces[i].a].vcol);
		glVertex3f(geometrie_raptor->points[geometrie_raptor->faces[i].a].x, geometrie_raptor->points[geometrie_raptor->faces[i].a].y, geometrie_raptor->points[geometrie_raptor->faces[i].a].z);

		glTexCoord2f(geometrie_raptor->points[geometrie_raptor->faces[i].b].ucol, geometrie_raptor->points[geometrie_raptor->faces[i].b].vcol);
		glVertex3f(geometrie_raptor->points[geometrie_raptor->faces[i].b].x, geometrie_raptor->points[geometrie_raptor->faces[i].b].y, geometrie_raptor->points[geometrie_raptor->faces[i].b].z);

		glTexCoord2f(geometrie_raptor->points[geometrie_raptor->faces[i].c].ucol, geometrie_raptor->points[geometrie_raptor->faces[i].c].vcol);
		glVertex3f(geometrie_raptor->points[geometrie_raptor->faces[i].c].x, geometrie_raptor->points[geometrie_raptor->faces[i].c].y, geometrie_raptor->points[geometrie_raptor->faces[i].c].z);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void reset() {
	player1.currentCase = game.cases[0];
	player2.currentCase = game.cases[0];
	player1.hasWon = false;
	player2.hasWon = false;
	isGameOver = false;
	updateSpritePosition(1);
	updateSpritePosition(2);
}
void handleInput() {
	if (inp->RBmouse) {
		// Handling mouse movements
		inp->get_mouse_movement();
	}
	if (inp->keys[KEY_CODE_ESCAPE]) {
		// Stop the message flow by sending QUIT message
		PostMessage(win->handle, WM_CLOSE, 0, 0);
	}
}

void displayFloor() {
	
	


	int hauteur = 0;
	int largeur = 0;
	float xoffset = 500;
	float zoffset = 100;
		for (int i = 1; i < nbSquareX+1; i++) {
			if (game.cases[i].type == MEAN) {
				colorSquareB = 0;
				colorSquareG = 0;
				colorSquareR = 1;
			}
			else if (i % 2 == 0) {
				colorSquareB = 1;
				colorSquareG = 1;
				colorSquareR = 1;
			}
			else {
				colorSquareB = 0.5;
				colorSquareG = 0.5;
				colorSquareR = 0.5;
			}
			if (i == 1 || i==25) {
				colorSquareB = 0;
				colorSquareG = 1;
				colorSquareR = 0;
			}

			if (i <= nbSquareX / 5) {
				displayCube(xoffset+largeur* sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				displayCube(xoffset + largeur * sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 - zoffset, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				largeur++;
			}
			else if (i <= 2 * nbSquareX / 5) {
				displayCube(xoffset + largeur*sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 + hauteur * sizeSquareZ / 2, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				displayCube(xoffset + largeur * sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 - hauteur * sizeSquareZ / 2 - zoffset, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				hauteur++;
			}
			else if (i <= 3 * nbSquareX / 5) {
				displayCube(xoffset + largeur*sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 + hauteur * sizeSquareZ / 2, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				displayCube(xoffset + largeur * sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 - hauteur * sizeSquareZ / 2 - zoffset, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				largeur++;
			}
			else if (i <= 4 * nbSquareX / 5) {
				displayCube(xoffset + largeur*sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 + hauteur * sizeSquareZ / 2, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				displayCube(xoffset + largeur * sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 - hauteur * sizeSquareZ / 2 - zoffset, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				hauteur--;
			}
			else {
				displayCube(xoffset + largeur* sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				displayCube(xoffset + largeur * sizeSquareX / 2 + 5, 0.0f, sizeSquareZ / 2 + 150 - zoffset, sizeSquareX / 2, sizeSquareY / 2, sizeSquareZ / 2, colorSquareR, colorSquareG, colorSquareB);
				largeur++;
			}

		
		}
	
}

void main_loop() {

	glClearColor(0.1f, 0.1f, 0.25f, 1.0f);
	// Time update
	tim->update_horloge();
	// Input update
	inp->refresh();
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Handling the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Setting up camera projection with a 60° view angle, near 10 and far 30000
	gluPerspective(60, (double)win->Xres / (double)win->Yres, 10, 30000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Defining the point to look at with the camera
	// (arguments : posX, posY, posZ, targetPosX, targetPosY, targetPosZ, and the up vector)
	gluLookAt(posXCam, posYCam, posZCam, targetPosX, targetPosY, targetPosZ, 0.0f, 1.0f, 0.0f);


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//									Entering main game loop										//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	
	displayFloor();
		if (currentPlayer == 1) {
			if (inp->keys[KEY_CODE_TAB]) {
				de = d(1, 4);
				displayString = "Player 1 throws a " + std::to_string(de)+ ".";

				for (int i = 0; i < de; i++) {
					player1.move(1);
					player1.checkForWins();
					if (player1.hasWon) {
						break;
					}
					updateSpritePosition(1);
				}
				if (player1.currentCase.type == MEAN) {
					for (int i = 0; i < 2; i++) {
						player1.move(-1);
						updateSpritePosition(1);
					}
					specialDisplay = "The player hit a red case and went back 2 cases !";
				}
				else {
					specialDisplay = "";
				}
				player1.checkForWins();
				if (player1.hasWon) {
					specialDisplay = "Player 1 wins.";
					isGameOver = true;
				}
				currentPlayer = 2;
				glColor3f(0, 0, 1);
			}
		}
		else if (!player1.hasWon){
			if (inp->keys[KEY_CODE_SPACE]) {
				de = d(1, 4);
				displayString = "Player 2 throws a " + std::to_string(de) + ".";
	
				for (int i = 0; i < de; i++) {
					player2.move(1);
					player2.checkForWins();
					if (player2.hasWon) {
						break;
					}
					updateSpritePosition(2);
				}
				if (player2.currentCase.type == MEAN) {
					for (int i = 0; i < 3; i++) {
						player2.move(-1);
						updateSpritePosition(2);
					}
					specialDisplay = "The player hit a red case and went back 2 cases !";
				}
				else {
					specialDisplay = "";
				}
				player2.checkForWins();
				if (player2.hasWon) {
					specialDisplay = "Player 2 wins.";
					isGameOver = true;
				}
				currentPlayer = 1;
				glColor3f(0, 1, 1);
			}
		}
		
		displayRaptor(1, player1.currentCase.numero);
		displayRaptor(2, player2.currentCase.numero);
		
	
	//gestion du reset (non fonctionnel)
	if (isGameOver){
		reset();

	}
	char *displayChar = new char[displayString.size() + 1];
	std::copy(displayString.begin(), displayString.end(), displayChar);
	displayChar[displayString.size()] = '\0';
	glColor3f(1, 1, 1);
	write_2_screen(displayRules, 600, 100, 200);
	write_2_screen(displayChar, 500, -500, 200);
	write_2_screen(displayPlayer1, 1090, -190, 200);
	write_2_screen(displayPlayer2, 950, 0, 200);
	glColor3f(1, 0, 0);
	write_2_screen(specialDisplay, 500, -550, 200);

	// Displaying the created image to the screen
	swap_buffer(win);
}


void stop() {
	delete inp;
	delete tim;

	if (win) {
		kill_font();
		kill_context(*win);
		delete win;
	}
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow) {
	MSG  msg;

	win	= NULL;
	inp	= NULL;
	tim = NULL;

	// Calling the scene setup function
	if (start() == false) {
		debug("Setting up of the scene encountered an error");
		stop();

		return 1;
	}

	// Initiating the loop
	while (true) {
		// When there is a message, calling WndProc() to handle it
		if (PeekMessage(&msg, NULL,0,0,PM_NOREMOVE)) {
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		// Else, continuing the main loop
		else {
			main_loop();
		}
	}

	// Stopping OpenGL and closing the window
	stop();

	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_MOVE:
			win->Xpos = (dword) LOWORD(lParam);
			win->Ypos = (dword) HIWORD(lParam);
			return 0;
	
		case WM_CLOSE:
			PostQuitMessage(0); // Tell to GetMessage() to send back 0
			return 0;
	
		case WM_SYSCOMMAND:  // Prevent energy saving screen
			switch (wParam) {
				case SC_SCREENSAVE:
		
				case SC_MONITORPOWER:
					return 0;
				}
				break;
	
		case WM_KEYDOWN:
			inp->set_key_down ((dword)wParam);
			return 0;
	
		case WM_KEYUP:
			inp->set_key_up((dword)wParam);
			return 0;

		case WM_LBUTTONDOWN:
			inp->set_mouse_left_down();
			return 0;

		case WM_LBUTTONUP:
			inp->set_mouse_left_up();
			return 0;
	
		case WM_RBUTTONDOWN: inp->set_mouse_right_down();
			return 0;
	
		case WM_RBUTTONUP:
			inp->set_mouse_right_up();
			return 0;
	
		case WM_MBUTTONDOWN: inp->set_mouse_middle_down();
			return 0;
	
		case WM_MBUTTONUP:
			inp->set_mouse_middle_up();
			return 0;
	}
	
	return DefWindowProc(hwnd,msg,wParam,lParam);
}
