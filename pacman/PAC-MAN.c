/*

Autores deste código:

Daniel Palma  Nº24664

Santiago Molina Sanchez   Nº24151

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include "pm-maps.h"
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLfloat vertices[][3] = { {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},
{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0} };

GLfloat normals[][3] = { {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},
{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0} };

GLfloat colors[][3] = {
	{0.0, 0.0, 0.0}, // Preto
	{1.0, 0.0, 0.0}, // Vermelho
	{1.0, 1.0, 0.0}, // Amarelo
	{0.0, 1.0, 0.0}, // Verde
	{0.0, 0.0, 1.0}, // Azul
	{1.0, 0.0, 1.0}, // Magenta
	{1.0, 1.0, 1.0}, // Branco
	{0.0, 1.0, 1.0}, // Ciano
	{0.5, 0.0, 0.0}, // Bordô
	{0.647, 0.165, 0.165}, //Castanho
	{0.0, 0.5, 0.0}, // Verde escuro
	{0.0, 0.0, 0.5}, // Azul escuro
	{0.5, 0.0, 0.5}, // Roxo
	{0.5, 0.5, 0.5}, // Cinza médio
	{0.3, 0.3, 0.3}, // Cinza escuro
	{1.0, 0.5, 0.0}, // Laranja
	{0.0, 0.5, 1.0}, // Azul claro
	{0.8, 0.8, 0.8}  // Cinza claro
};

#define BLACK 0
#define RED 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4
#define MAGENTA 5
#define WHITE 6
#define CIANE 7
#define PURPLE 8
#define BROWN 9

#define ZOOM_MIN 0.2
#define ZOOM_MAX 2.0


#define FRONT 180.0
#define BACK  0.0
#define LEFT 90.0
#define RIGHT 270.0

#define ROTATION 5.0

#define TIME 16

#define RESET 2000


typedef struct pacman {

	float theta[3];
	float scale;
	float xPacman;
	float yPacman;
	int xSquare;
	int ySquare;

}pacman;

typedef pacman* PACMAN;



typedef struct cube {

	float theta[3];
	float scale;
	float xCube, yCube;
	int xCubeSquare, yCubeSquare;
	float PositionFinalXghost, PositionFinalYghost;
	float PreviousXghost, PreviousYghost;
	int isMoving;
	float v;
	float color;

}cube;

typedef cube* Cube;


struct statusBar {
	int x, y;
	char buffer[1024];
} status;

char BigBuffer[2048];

PACMAN player;

Cube* ghosts;

Cube* smartGhosts;


float v0 = 0.3;

int gamePaused = 0;
int gamePausePending = 0;
int ActualKey = 0;

int numSquareInvalid = 0;


float zoom;

int lastX = 0;
int lastY = 0;
float CameraX = 0.0;
float CameraZ = 0.0;

float internalPos = 0.0;
float internalVel = 2.0;

int xSquareInitial = -1;
int ySquareInitial = -1;
int PreviousY, PreviousX;
int isMoving = 0;

float PositionFinalX, PositionFinalY;

int directions[4][2] = {
			{0, 1},   // Cima
			{0, -1},  // Baixo
			{1, 0},   // Direita
			{-1, 0}   // Esquerda
};

int ActualAngle = 0.0, LastAngle = 180.0;
int point = 0;
int** visitedMap;
static GLdouble viewer[] = { 0.0, -2.0, 3.0 };
char** map = NULL;


// Cria um jogador com os parâmetros iniciais fornecidos
PACMAN CriatePlayer(float theta1, float theta2, float theta3, float scale, float x, float y) {

	PACMAN player = malloc(sizeof(struct pacman));

	player->theta[0] = theta1;
	player->theta[1] = theta2;
	player->theta[2] = theta3;

	player->scale = scale;
	player->xPacman = x;
	player->yPacman = y;

	return player;
}


// Cria um fantasma (cubo) com os parâmetros iniciais fornecidos
Cube CriateCube(float theta1, float theta2, float theta3, float escala, float x, float y, float v) {

	Cube cubo = malloc(sizeof(struct cube));

	cubo->theta[0] = theta1;
	cubo->theta[1] = theta2;
	cubo->theta[2] = theta3;

	cubo->scale = escala;
	cubo->xCube = x;
	cubo->yCube = y;
	cubo->v = v;

	return cubo;

}

void polygon(int a, int b, int c, int d)
{

	glBegin(GL_POLYGON);
	glNormal3fv(normals[a]);
	glVertex3fv(vertices[a]);
	glNormal3fv(normals[b]);
	glVertex3fv(vertices[b]);
	glNormal3fv(normals[c]);
	glVertex3fv(vertices[c]);
	glNormal3fv(normals[d]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube(int i)
{

	glColor3fv(colors[i]);
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}

void cuboMagico(int n, int l)
{
	int i, j, k;

	glPushMatrix();

	glScalef(0.2, 0.2, 0.2);

	glTranslatef(-2.2, -2.2, -2.2);

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++) {
				glPushMatrix();
				glTranslatef(2.2 * i, 2.2 * j, 2.2 * k);
				colorcube(i + l);
				glPopMatrix();
			}

	glPopMatrix();
}


void desenhaBoneco() {


	glPushMatrix();
	glTranslatef(0.0, 1.26, 0.0);
	glScalef(0.25, 0.15, 0.25);
	colorcube(BLACK);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0, 1.1, 0.0);
	glRotatef(0, 0, 90, 1);
	glScalef(0.05, 0.5, 0.4);
	colorcube(YELLOW);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, 0.8, 0.0);
	glRotatef(7 * internalPos, 0, 0, 1);
	glScalef(0.10, 0.2, 0.10);
	glRotatef(90, 0, 1, 0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(3, 100, 40.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.1, 0.0);
	glScalef(0.4, 0.5, 0.4);
	colorcube(RED);
	glPopMatrix();

	// Left Leg
	glPushMatrix();
	glTranslatef(-0.23, -0.4, 0.0);
	glRotatef(40 * internalPos, 1, 0, 0);
	glTranslatef(0, -0.3, 0.0);
	glScalef(0.15, 0.3, 0.15);
	glRotatef(-90, 0, 1, 0);
	colorcube(PURPLE);
	glPopMatrix();

	// right leg
	glPushMatrix();
	glTranslatef(0.23, -0.4, 0.0);
	glRotatef(-45 * internalPos, 1, 0, 0);
	glTranslatef(0, -0.3, 0.0);
	glScalef(0.15, 0.3, 0.15);
	glRotatef(90, 0, 1, 0);
	colorcube(PURPLE);
	glPopMatrix();

	// Braço esquerdo
	glPushMatrix();
	glTranslatef(-0.5, 0.4, 0.0);
	glRotatef(45 * internalPos, 1, 0, 0);
	glTranslatef(0, -0.1, 0.0);
	glScalef(0.15, 0.3, 0.15);
	glRotatef(-90, 0, 1, 0);
	colorcube(BLACK);
	glPopMatrix();

	// Braço direito
	glPushMatrix();
	glTranslatef(0.5, 0.4, 0.0);
	glRotatef(-45 * internalPos, 1, 0, 0);
	glTranslatef(0, -0.1, 0.0);
	glScalef(0.15, 0.3, 0.15);
	glRotatef(90, 0, 1, 0);
	colorcube(BLACK);
	glPopMatrix();
}

void DrawGhostArray(Cube* ghosts, int i, int l) {
	glPushMatrix();

	glTranslatef(ghosts[i]->xCube, ghosts[i]->yCube, 0.15);

	glRotatef(ghosts[i]->theta[0], 1.0, 0.0, 0.0);
	glRotatef(ghosts[i]->theta[1], 0.0, 1.0, 0.0);
	glRotatef(ghosts[i]->theta[2], 0.0, 0.0, 1.0);

	glScalef(ghosts[i]->scale, ghosts[i]->scale, ghosts[i]->scale);

	cuboMagico(3, l);
	glPopMatrix();
}


void DrawAllGhost() {
	for (int i = 0; i < numGhosts(); i++)
	{
		DrawGhostArray(ghosts, i, 0);
		glutPostRedisplay();
	}
	for (int i = 0; i < numSmartGhosts(); i++)
	{
		DrawGhostArray(smartGhosts, i, 5);
		glutPostRedisplay();
	}
}


void eixos() {
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0); //Eixo x amarelo
	glVertex3f(-2, 0, 0);
	glVertex3f(2, 0, 0);

	glVertex3f(2, 0, 0);
	glVertex3f(1.7, 0.3, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(1.7, -0.3, 0);


	glColor3f(0.0, 1.0, 0.0); //Eixo y verde
	glVertex3f(0, -2, 0);
	glVertex3f(0, 2, 0);

	glVertex3f(0, 2, 0);
	glVertex3f(0.3, 1.7, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(-0.3, 1.7, 0);


	glColor3f(0.0, 0.0, 1.0); //Eixo z azul
	glVertex3f(0, 0, -2);
	glVertex3f(0, 0, 2);

	glVertex3f(0, 0, 2);
	glVertex3f(0.3, 0, 1.7);
	glVertex3f(0, 0, 2);
	glVertex3f(-0.3, 0, 1.7);

	glEnd();
}


void DrawPACMAN() {

	glPushMatrix();

	glTranslatef(player->xPacman, player->yPacman, 0.20);
	glRotatef(player->theta[0], 1.0, 0.0, 0.0);
	glRotatef(player->theta[1], 0.0, 1.0, 0.0);
	glRotatef(player->theta[2], 0.0, 0.0, 1.0);

	glScalef(player->scale, player->scale, player->scale);

	desenhaBoneco();

	glPopMatrix();
}

// Posiciona o jogador em uma casa específica no tabuleiro
void PlacePlayerOnSquare(PACMAN player, int i, int j) {

	int maxSize = xTabSize() > yTabSize() ? xTabSize() : yTabSize();
	float translatex = -(xTabSize() - 1);
	float translatey = -(yTabSize() - 1);

	// Calcular a posição do boneco
	player->xPacman = (translatex + i * 2.0) * 1.0 / maxSize;
	player->yPacman = (translatey + j * 2.0) * 1.0 / maxSize;

	printf("Player Position: x = %f, y = %f\n", player->xPacman, player->yPacman);

	player->xSquare = i;
	player->ySquare = j;

	visitedMap[i][j] = 1;

}

// Posiciona um fantasma aleatoriamente no tabuleiro, garantindo que não haja conflitos
void PlaceGhostRandomly(Cube ghost, char** map, PACMAN player, Cube* dumbGhosts, int numberDumGhosts, Cube* smartGhosts, int numberSmartGhosts) {

	int valid = 0;
	int i, j;

	while (!valid) {
		i = rand() % xTabSize();
		j = rand() % yTabSize();

		if (map[i][j] == 1) { // Verifica se a casa é válida no mapa
			valid = 1;

			if (player->xSquare == i && player->ySquare == j) {
				valid = 0;

			}

			for (int k = 0; k < numberDumGhosts; k++) {
				if (dumbGhosts[k] != NULL && dumbGhosts[k]->xCubeSquare == i && dumbGhosts[k]->yCubeSquare == j) {
					valid = 0;
					break;
				}
			}

			for (int k = 0; k < numberSmartGhosts; k++) {
				if (smartGhosts[k] != NULL && smartGhosts[k]->xCubeSquare == i && smartGhosts[k]->yCubeSquare == j) {
					valid = 0;
					break;
				}
			}
		}
	}

	// Atualiza as coordenadas do fantasma
	ghost->xCubeSquare = i;
	ghost->yCubeSquare = j;

	float scale = 1.0 / (xTabSize() > yTabSize() ? xTabSize() : yTabSize());
	float translatex = -(xTabSize() - 1);
	float translatey = -(yTabSize() - 1);

	ghost->xCube = (translatex + i * 2.0) * scale;
	ghost->yCube = (translatey + j * 2.0) * scale;

	printf("Ghost positioned at (%d, %d)\n", i, j);
}

// Desenha o mapa do jogo com as casas (cubos) e bordas
void drawRawMap(char** m) {
	int i = 0, j = 0;
	int maxSize = xTabSize() > yTabSize() ? xTabSize() : yTabSize();
	int numInvalid = 0;
	int color = rand() % 18;
	glPushMatrix();

	glScalef(1.0 / maxSize, 1.0 / maxSize, 1.0 / maxSize);

	glTranslatef(-(xTabSize() - 1), -(yTabSize() - 1), 0.0);


	for (i = 0; i < xTabSize(); i++) {
		for (j = 0; j < yTabSize(); j++) {

			// Desenha o cubo principal
			glPushMatrix();
			glTranslatef(i * 2.0, j * 2.0, 0.0);
			glScalef(0.9, 0.9, 0.4);

			if (m[i][j] == 1) {
				if (visitedMap[i][j] == 1) {
					colorcube(GREEN);												// Pinta de verde as casas visitadas

				}
				else {
					colorcube(rand() % 18);											// Pinta de azul ou amarelo
				}
				color = (color == rand() % 18) ? rand() % 18 : rand() % 18;			// Alterna cores

				if (xSquareInitial == -1 && ySquareInitial == -1) {

					PlacePlayerOnSquare(player, i, j);
					colorcube(GREEN);
					xSquareInitial = 0;
					xSquareInitial = 0;
				}
			}
			else
			{
				numInvalid++;
			}

			numSquareInvalid = numInvalid;


			glPopMatrix();


			// Desenha a borda horizontal
			if (j < yTabSize() - 1) {
				glPushMatrix();
				glTranslatef(i * 2.0, j * 2.0 + 0.95, 0.0);
				glScalef(0.99, 0.08, 0.4);
				colorcube(BLACK);
				glPopMatrix();
			}

			// Desenha a borda vertical
			if (i < xTabSize() - 1) {
				glPushMatrix();
				glTranslatef(i * 2.0 + 0.95, j * 2.0, 0.0);
				glScalef(0.08, 0.99, 0.4);
				colorcube(BLACK);
				glPopMatrix();
			}
		}

	}

	glPopMatrix();
}

// Inicializa o mapa de casas visitadas, marcando todas como não visitadas
int initVisitedMap() {
	int xSize = xTabSize();
	int ySize = yTabSize();

	visitedMap = malloc(xSize * sizeof(int*));
	for (int i = 0; i < xSize; i++) {
		visitedMap[i] = malloc(ySize * sizeof(int));
		for (int j = 0; j < ySize; j++) {
			visitedMap[i][j] = 0;
		}
	}
}

// Verifica se uma posição no tabuleiro está ocupada por outros fantasmas
int isPositionOccupied(Cube* ghosts, int numberGhosts, int x, int y, Cube currentGhost) {

	for (int i = 0; i < numberGhosts; i++) {
		if (ghosts[i] != currentGhost &&
			ghosts[i]->xCubeSquare == x &&
			ghosts[i]->yCubeSquare == y) {
			return 1;
		}
	}
	return 0;
}

// Movimenta os fantasmas burros aleatoriamente no tabuleiro
void MovingDumGhost() {


	for (int i = 0; i < numGhosts(); i++) {

		Cube ghost = ghosts[i];

		if (!ghost->isMoving) {

			int x = ghost->xCubeSquare;
			int y = ghost->yCubeSquare;

			int ValidDirections[4][2];
			int ValidDiretion = 0;

			// Avaliar todas as direções para encontrar movimentos válidos
			for (int j = 0; j < 4; j++) {
				int GhostSquareX = x + directions[j][0];
				int GhostSquareY = y + directions[j][1];

				if (GhostSquareX >= 0 && GhostSquareX < xTabSize() && GhostSquareY >= 0 && GhostSquareY < yTabSize() && map[GhostSquareX][GhostSquareY] == 1 && !isPositionOccupied(ghosts, numGhosts(), GhostSquareX, GhostSquareY, ghost) && !isPositionOccupied(smartGhosts, numSmartGhosts(), GhostSquareX, GhostSquareY, ghost)) { // Não está ocupada

					ValidDirections[ValidDiretion][0] = GhostSquareX;
					ValidDirections[ValidDiretion][1] = GhostSquareY;
					ValidDiretion++;
				}
			}

			// Se encontrou um movimento válido
			if (ValidDiretion > 0) {
				int RandomDirection = rand() % ValidDiretion;
				int ValidMoveX = ValidDirections[RandomDirection][0];
				int ValidMoveY = ValidDirections[RandomDirection][1];

				ghost->PositionFinalXghost = ghost->xCube + (ValidMoveX - x) * 0.25;
				ghost->PositionFinalYghost = ghost->yCube + (ValidMoveY - y) * 0.25;

				ghost->xCubeSquare = ValidMoveX;
				ghost->yCubeSquare = ValidMoveY;

				ghost->PreviousXghost = ghost->xCube;
				ghost->PreviousYghost = ghost->yCube;

				ghost->isMoving = 1;
			}
		}
		else {

			if (ghost->PreviousXghost < ghost->PositionFinalXghost) {
				ghost->xCube += v0 * TIME / 1000.0;
				if (ghost->xCube >= ghost->PositionFinalXghost) {
					ghost->xCube = ghost->PositionFinalXghost;
				}
			}
			else if (ghost->PreviousXghost > ghost->PositionFinalXghost) {
				ghost->xCube -= v0 * TIME / 1000.0;
				if (ghost->xCube <= ghost->PositionFinalXghost) {
					ghost->xCube = ghost->PositionFinalXghost;
				}
			}

			if (ghost->PreviousYghost < ghost->PositionFinalYghost) {
				ghost->yCube += v0 * TIME / 1000.0;
				if (ghost->yCube >= ghost->PositionFinalYghost) {
					ghost->yCube = ghost->PositionFinalYghost;
				}
			}
			else if (ghost->PreviousYghost > ghost->PositionFinalYghost) {
				ghost->yCube -= v0 * TIME / 1000.0;
				if (ghost->yCube <= ghost->PositionFinalYghost) {
					ghost->yCube = ghost->PositionFinalYghost;
				}
			}

			// Verifica se o movimento foi concluído
			if (ghost->xCube == ghost->PositionFinalXghost && ghost->yCube == ghost->PositionFinalYghost) {
				ghost->isMoving = 0;
			}
		}
	}
}

// Movimenta os fantasmas inteligentes em direção ao jogador
void moveTowardPlayer() {

	for (int i = 0; i < numSmartGhosts(); i++) {

		Cube ghost = smartGhosts[i];


		if (!ghost->isMoving) {
			int x = ghost->xCubeSquare;
			int y = ghost->yCubeSquare;

			int BestXSquare = x, BestYSquare = y;

			float BestDistance = 100.0;

			// Avalia todas as direções para encontrar a melhor
			for (int j = 0; j < 4; j++) {
				int GhostSquareX = x + directions[j][0];
				int GhostSquareY = y + directions[j][1];

				if (GhostSquareX >= 0 && GhostSquareX < xTabSize() && GhostSquareY >= 0 && GhostSquareY < yTabSize() && map[GhostSquareX][GhostSquareY] == 1 && !isPositionOccupied(smartGhosts, numSmartGhosts(), GhostSquareX, GhostSquareY, ghost) && !isPositionOccupied(ghosts, numGhosts(), GhostSquareX, GhostSquareY, ghost)) {  // Não está ocupada


					int DeltaX = player->xSquare - GhostSquareX;
					int DeltaY = player->ySquare - GhostSquareY;
					float distance = (DeltaX * DeltaX) + (DeltaY * DeltaY);

					if (distance < BestDistance) {
						BestDistance = distance;
						BestXSquare = GhostSquareX;
						BestYSquare = GhostSquareY;
					}
				}
			}

			// Se encontrou um movimento válido
			if (BestXSquare != x || BestYSquare != y) {

				ghost->PositionFinalXghost = ghost->xCube + (BestXSquare - x) * 0.25;
				ghost->PositionFinalYghost = ghost->yCube + (BestYSquare - y) * 0.25;

				ghost->xCubeSquare = BestXSquare;
				ghost->yCubeSquare = BestYSquare;

				ghost->PreviousXghost = ghost->xCube;
				ghost->PreviousYghost = ghost->yCube;

				ghost->isMoving = 1;
			}
		}
		else {

			if (ghost->PreviousXghost < ghost->PositionFinalXghost) {
				ghost->xCube += v0 * TIME / 1000.0;
				if (ghost->xCube >= ghost->PositionFinalXghost) {
					ghost->xCube = ghost->PositionFinalXghost;

				}
			}
			else if (ghost->PreviousXghost > ghost->PositionFinalXghost) {
				ghost->xCube -= v0 * TIME / 1000.0;
				if (ghost->xCube <= ghost->PositionFinalXghost) {
					ghost->xCube = ghost->PositionFinalXghost;
				}
			}

			if (ghost->PreviousYghost < ghost->PositionFinalYghost) {
				ghost->yCube += v0 * TIME / 1000.0;
				if (ghost->yCube >= ghost->PositionFinalYghost) {
					ghost->yCube = ghost->PositionFinalYghost;
				}
			}
			else if (ghost->PreviousYghost > ghost->PositionFinalYghost) {
				ghost->yCube -= v0 * TIME / 1000.0;
				if (ghost->yCube <= ghost->PositionFinalYghost) {
					ghost->yCube = ghost->PositionFinalYghost;
				}
			}


			if (ghost->xCube == ghost->PositionFinalXghost && ghost->yCube == ghost->PositionFinalYghost) {
				ghost->isMoving = 0;
			}
		}
	}
}

// Desenha uma string de caracteres na tela
void drawChars(char* s)
{
	for (int i = 0; i < strlen(s); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
}

// Renderiza a barra de status do jogo
void statusBarRender()
{
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos3f(status.x, status.y, 1.0);
	drawChars(status.buffer);
}

// Atualiza o texto exibido na barra de status
void updateStatusBarText(char* txt)
{

	if (strcmp(txt, status.buffer) != 0) {
		strcpy_s(status.buffer, strlen(txt) + 1, txt);
		glutPostRedisplay();
	}
}

// Verifica se há colisão entre o jogador e os fantasmas
void CheckCollision() {

	for (int i = 0; i < numGhosts(); i++) {
		if (player->xSquare == ghosts[i]->xCubeSquare &&
			player->ySquare == ghosts[i]->yCubeSquare) {
			gamePaused = 1;
			updateStatusBarText("YOU LOST!!!");
			printf("Collision detected\n");
			return;
		}
	}
	for (int i = 0; i < numSmartGhosts(); i++) {
		if (player->xSquare == smartGhosts[i]->xCubeSquare &&
			player->ySquare == smartGhosts[i]->yCubeSquare) {
			gamePaused = 1;
			updateStatusBarText("YOU LOST!!!");
			printf("Collision with smart Ghost detected\n");
			return;
		}
	}
}

// Gerencia a rotação do jogador para alinhar o ângulo corretamente ao se mover
void AnglePlayer() {

	if (player->theta[1] > 360.0) player->theta[1] -= 360.0;
	if (player->theta[1] < -360.0) player->theta[1] += 360.0;

	if (ActualAngle == LastAngle + 180.0) {

		player->theta[1] += ROTATION;

		if (player->theta[1] >= ActualAngle) {
			player->theta[1] = ActualAngle;
			LastAngle = ActualAngle;

		}
	}
	else if (ActualAngle == LastAngle - 180.0)
	{
		player->theta[1] -= ROTATION;

		if (player->theta[1] <= ActualAngle) {
			player->theta[1] = ActualAngle;
			LastAngle = ActualAngle;

		}

	}
	else if (ActualAngle == LastAngle + 90.0) {
		player->theta[1] += ROTATION;

		if (player->theta[1] >= ActualAngle) {
			player->theta[1] = ActualAngle;
			LastAngle = ActualAngle;
		}

	}
	else if (ActualAngle == LastAngle - 90.0) {
		player->theta[1] -= ROTATION;

		if (player->theta[1] <= ActualAngle) {
			player->theta[1] = ActualAngle;
			LastAngle = ActualAngle;
		}

	}
	else if (LastAngle == 0.0 && ActualAngle == LastAngle + 270.0) {
		player->theta[1] -= ROTATION;

		if (player->theta[1] <= ActualAngle - 360) {
			player->theta[1] = ActualAngle;
			LastAngle = ActualAngle;
		}

	}
	else if (LastAngle == 270.0 && ActualAngle == LastAngle - 270.0) {
		player->theta[1] += ROTATION;

		if (player->theta[1] >= ActualAngle && player->theta[1] < 270.0) {
			player->theta[1] = ActualAngle;
			LastAngle = ActualAngle;
		}

	}


}

// Define a posição da barra de status na tela
void statusBarPosition(int x, int y)
{
	status.x = x;
	status.y = y;
	strcpy_s(status.buffer, strlen("Status Bar") + 1, "Status Bar");
}

// Redefine a projeção do jogo ao redimensionar a janela
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
			2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
			2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();
	glScalef(zoom, zoom, zoom);
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(CameraX, 1.0, 0.0, 0.0);
	glRotatef(CameraZ, 0.0, 0.0, 1.0);


	drawRawMap(map);
	eixos();

	DrawPACMAN();
	statusBarRender();


	DrawAllGhost();

	glFlush();



	glutSwapBuffers();
}

// Inicializa variáveis globais e configurações do jogo
void init() {

	glClearColor(0.0, 0.0, 0.50196, 1.0);

	srand(time(NULL));
	zoom = 0.5;
	initVisitedMap();

	CameraX = 0.0;
	CameraZ = 0.0;

	player = CriatePlayer(90.0, 0.0, 0, 0.15, 0.0, 0.0);

	statusBarPosition(1, 0.5);

	int numberGhosts = numGhosts();


	ghosts = (Cube*)malloc(sizeof(cube) * numberGhosts);

	for (int i = 0; i < numberGhosts; i++) {
		ghosts[i] = CriateCube(0.0, 0.0, 0.0, 0.15, 0.0, 0.0, 0.1);
		ghosts[i]->isMoving = 0;
		ghosts[i]->PositionFinalXghost = 0.0;
		ghosts[i]->PositionFinalYghost = 0.0;
		printf("Ghost %d created!\n", i + 1);
	}

	int numberSmartGhosts = numSmartGhosts();


	smartGhosts = (Cube*)malloc(sizeof(cube) * numberSmartGhosts);

	for (int i = 0; i < numberSmartGhosts; i++) {
		smartGhosts[i] = CriateCube(0.0, 0.0, 0.0, 0.15, 0.0, 0.0, 0.1);
		smartGhosts[i]->isMoving = 0;
		smartGhosts[i]->PositionFinalXghost = 0.0;
		smartGhosts[i]->PositionFinalYghost = 0.0;
		printf("Ghost %d created!\n", i + 1);
	}

	// Posicionar todos os fantasmas burros
	for (int i = 0; i < numberGhosts; i++) {
		PlaceGhostRandomly(ghosts[i], map, player, ghosts, numberGhosts, smartGhosts, numberSmartGhosts);
	}

	// Posicionar todos os fantasmas inteligentes
	for (int i = 0; i < numberSmartGhosts; i++) {
		PlaceGhostRandomly(smartGhosts[i], map, player, ghosts, numberGhosts, smartGhosts, numberSmartGhosts);
	}
}

// Reseta o estado do jogo ao inicializar novamente os objetos e variáveis
void resetGame() {

	for (int i = 0; i < numGhosts(); i++) {
		if (ghosts[i] != NULL) {
			free(ghosts[i]);
			ghosts[i] = NULL;
		}
	}

	for (int i = 0; i < numSmartGhosts(); i++) {
		if (smartGhosts[i] != NULL) {
			free(smartGhosts[i]);
			smartGhosts[i] = NULL;
		}
	}

	zoom = 0.5;
	CameraX = 0.0;
	CameraZ = 0.0;
	gamePaused = 0;
	point = 0;
	xSquareInitial = -1;
	ySquareInitial = -1;
	PreviousY = 0;
	PreviousX = 0;
	PositionFinalX = 0;
	PositionFinalY = 0;
	ActualAngle = 180.0;
	LastAngle = 180.0;

	player->theta[0] = 90.0;
	player->theta[1] = 0.0;
	player->theta[2] = 0.0;
	player->scale = 0.15;
	player->xPacman = 0.0;
	player->yPacman = 0.0;
	isMoving = 0;

	init();
}

// Gerencia a lógica para teclas especiais (setas), ajustando o movimento do jogador com base no ângulo da câmera
void teclasEspeciais(int key, int x, int y) {

	if (isMoving == 1) {
		ActualKey = key;
		return;
	}

	int ValidXSquare, ValidYSquare;
	float CameraAngle = CameraZ;

	//[0, 360]
	while (CameraAngle < 0) CameraAngle += 360;
	while (CameraAngle >= 360) CameraAngle -= 360;

	// Determina a direção ajustada com base no ângulo
	int NewKey = key;
	if (CameraAngle >= 45 && CameraAngle < 135) { // Câmera virada para a direita
		if (key == GLUT_KEY_UP) NewKey = GLUT_KEY_RIGHT;
		else if (key == GLUT_KEY_DOWN) NewKey = GLUT_KEY_LEFT;
		else if (key == GLUT_KEY_LEFT) NewKey = GLUT_KEY_UP;
		else if (key == GLUT_KEY_RIGHT) NewKey = GLUT_KEY_DOWN;
	}
	else if (CameraAngle >= 135 && CameraAngle < 225) { // Câmera virada para trás
		if (key == GLUT_KEY_UP) NewKey = GLUT_KEY_DOWN;
		else if (key == GLUT_KEY_DOWN) NewKey = GLUT_KEY_UP;
		else if (key == GLUT_KEY_LEFT) NewKey = GLUT_KEY_RIGHT;
		else if (key == GLUT_KEY_RIGHT) NewKey = GLUT_KEY_LEFT;
	}
	else if (CameraAngle >= 225 && CameraAngle < 315) { // Câmera virada para a esquerda
		if (key == GLUT_KEY_UP) NewKey = GLUT_KEY_LEFT;
		else if (key == GLUT_KEY_DOWN) NewKey = GLUT_KEY_RIGHT;
		else if (key == GLUT_KEY_LEFT) NewKey = GLUT_KEY_DOWN;
		else if (key == GLUT_KEY_RIGHT) NewKey = GLUT_KEY_UP;
	}

	// Movimenta o jogador com base na tecla ajustada
	if (NewKey == GLUT_KEY_UP) {
		ValidXSquare = player->xSquare;
		ValidYSquare = player->ySquare + 1;

		if (player->ySquare < yTabSize() - 1 && map[ValidXSquare][ValidYSquare] == 1) {
			PreviousY = player->ySquare;
			player->ySquare++;
			PositionFinalY = player->yPacman + 0.25;
			ActualAngle = FRONT;
			isMoving = 1;
			glutPostRedisplay();
		}
	}
	else if (NewKey == GLUT_KEY_DOWN) {
		ValidXSquare = player->xSquare;
		ValidYSquare = player->ySquare - 1;

		if (player->ySquare > 0 && map[ValidXSquare][ValidYSquare] == 1) {
			PreviousY = player->ySquare;
			player->ySquare--;
			PositionFinalY = player->yPacman - 0.25;
			ActualAngle = BACK;
			isMoving = 1;
			glutPostRedisplay();
		}
	}
	else if (NewKey == GLUT_KEY_LEFT) {
		ValidXSquare = player->xSquare - 1;
		ValidYSquare = player->ySquare;

		if (player->xSquare > 0 && map[ValidXSquare][ValidYSquare] == 1) {
			PreviousX = player->xSquare;
			player->xSquare--;
			PositionFinalX = player->xPacman - 0.25;
			ActualAngle = LEFT;
			isMoving = 1;
			glutPostRedisplay();
		}
	}
	else if (NewKey == GLUT_KEY_RIGHT) {
		ValidXSquare = player->xSquare + 1;
		ValidYSquare = player->ySquare;

		if (player->xSquare < xTabSize() - 1 && map[ValidXSquare][ValidYSquare] == 1) {
			PreviousX = player->xSquare;
			player->xSquare++;
			PositionFinalX = player->xPacman + 0.25;
			ActualAngle = RIGHT;
			isMoving = 1;
			glutPostRedisplay();
		}
	}

	glutPostRedisplay();
}

// Gerencia a pontuação do jogador e o estado do jogo 
PointsPlayer(int xSquare, int ySquare, int numSquareInvalid) {

	int SizeMap = xTabSize() * yTabSize();

	if (map[xSquare][ySquare] == 1) {

		if (visitedMap[xSquare][ySquare] == 0) {
			visitedMap[xSquare][ySquare] = 1;
			point++;

			if (SizeMap - numSquareInvalid == point + 1) {

				updateStatusBarText("YOU WIN!!");

				printf("\nYOU WIN\n");

				gamePaused = 1;

				glutTimerFunc(RESET, resetGame, 0);

			}
		}

	}

}

// Movimenta o jogador de forma suave entre as casas no tabuleiro
void MovingPlayer() {

	if (isMoving == 0) {
		return;
	}


	if (PreviousY < player->ySquare) {
		if (player->yPacman < PositionFinalY) {
			player->yPacman += v0 * TIME / 1000.0;

			if (player->yPacman >= PositionFinalY) {
				player->yPacman = PositionFinalY;
				isMoving = 0;

				PointsPlayer(player->xSquare, player->ySquare, numSquareInvalid);
			}
		}
	}

	else if (PreviousY > player->ySquare) {
		if (player->yPacman > PositionFinalY) {
			player->yPacman -= v0 * TIME / 1000.0;

			if (player->yPacman <= PositionFinalY) {
				player->yPacman = PositionFinalY;
				isMoving = 0;

				PointsPlayer(player->xSquare, player->ySquare, numSquareInvalid);
			}
		}
	}


	if (PreviousX < player->xSquare) {
		if (player->xPacman < PositionFinalX) {
			player->xPacman += v0 * TIME / 1000.0;

			if (player->xPacman >= PositionFinalX) {
				player->xPacman = PositionFinalX;
				isMoving = 0;

				PointsPlayer(player->xSquare, player->ySquare, numSquareInvalid);
			}
		}
	}

	else if (PreviousX > player->xSquare) {
		if (player->xPacman > PositionFinalX) {
			player->xPacman -= v0 * TIME / 1000.0;

			if (player->xPacman <= PositionFinalX) {
				player->xPacman = PositionFinalX;
				isMoving = 0;

				PointsPlayer(player->xSquare, player->ySquare, numSquareInvalid);
			}
		}
	}




	if (isMoving == 0 && ActualKey != -1) {
		int key = ActualKey;
		ActualKey = -1;
		teclasEspeciais(key, 0, 0);
	}
}


void teclado(char key, int x, int y) {
	switch (key) {
	case '+':
		if (zoom + 0.2 <= ZOOM_MAX) {
			zoom += 0.2;
			glutPostRedisplay();
		}
		break;
	case '-':
		if (zoom / 1.2 >= ZOOM_MIN) {
			zoom /= 1.2;
			glutPostRedisplay();
		}
		break;
	case 'q': exit(0); break;
	case 'Q': exit(0);; break;
	case 'p': gamePaused = !gamePaused; printf("Game %s\n", gamePaused ? "Paused" : "Resumed"); break;	// Alternar pausa/despausa
	case 'r':resetGame(); glutPostRedisplay(); break;
	case 'R':resetGame(); glutPostRedisplay(); break;

	}

	glutPostRedisplay();

}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		lastX = x;
		lastY = y;
	}
}

// Atualiza a câmera com base no movimento do mouse
void motion(int x, int y) {

	CameraZ += (x - lastX) * 0.2;
	CameraX -= (y - lastY) * 0.2;
	lastX = x;
	lastY = y;

	if (CameraX < -50)
		CameraX = -50;
	else if (CameraX > 20)
		CameraX = 20;

	glutPostRedisplay();
}

// Atualiza o estado do jogo periodicamente, incluindo animações e verificações
void update(int value) {

	AnglePlayer();

	// Continuar com a lógica normal do jogo se não estiver pausado
	if (gamePaused == 0) {
		if (isMoving == 1) {
			internalPos += internalVel * (TIME / 1000.0);
			if (internalPos >= 1.0) {
				internalVel *= -1;
				internalPos = 1.0;
			}
			if (internalPos <= -1.0) {
				internalVel *= -1;
				internalPos = -1.0;
			}
		}

		sprintf(status.buffer, "POINTS = %i", point);
		updateStatusBarText(status.buffer);

		MovingPlayer();
		MovingDumGhost();
		moveTowardPlayer();

		CheckCollision();
	}

	glutTimerFunc(TIME, update, TIME);
	glutPostRedisplay();
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutCreateWindow("PAC-MAN");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);

	glutMotionFunc(motion);
	glutSpecialFunc(teclasEspeciais);
	glutKeyboardFunc(teclado);

	glutMouseFunc(mouse);

	glEnable(GL_DEPTH_TEST);

	printf("WELCOME PAC-MAN!\n");
	printf("Autores: Santiago Molina (24151)\n");
	printf("Autores: Daniel Palma (24664)\n");
	printf("\nComputacao Grafica\n");
	printf("\nQ -> Quit Game\n");
	printf("r -> Restart Game\n");
	printf("+/- -> Zoom Game\n");
	printf("p -> Pause Game\n");

	if (argc <= 1) {
		printf("Please run as:\n");
		printf("%s <map file>", argv[0]);
		exit(0);
	}
	printf("\nCreating map...\n");
	openMapsFile(argv[1]);
	map = nextMap();
	printMap();

	init();



	glutTimerFunc(TIME, update, TIME);

	glutMainLoop();
}