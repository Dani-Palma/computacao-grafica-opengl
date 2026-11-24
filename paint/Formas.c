#include "Formas.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

struct forma {
	int tipo;                // Tipo da forma
	float x, y;              // Pontos iniciais das formas
	float xSize, ySize;      // Lados ou dimensões
	float r, g, b;           // Cores das formas
	float rContorno, gContorno, bContorno; // Cores dos contornos
};

Forma novaForma(int tipo, float x, float y, float xSize, float ySize) {
	Forma f = (Forma)malloc(sizeof(struct forma));

	f->x = x;
	f->y = y;
	f->xSize = xSize;
	f->ySize = ySize;

	f->r = 1.0;
	f->g = 1.0;
	f->b = 1.0;

	f->rContorno = 0.0;
	f->gContorno = 0.0;
	f->bContorno = 0.0;

	f->tipo = tipo;

	return f;
}

Forma novoRectangulo(float x, float y, float xSize, float ySize) {
	return novaForma(RECTANGULO, x, y, xSize, ySize);
}

Forma novoQuadrado(float x, float y, float lado) {
	return novaForma(QUADRADO, x, y, lado, lado);
}

Forma novoTrianguloIso(float x, float y, float xSize, float ySize) {
	return novaForma(TRIANGULO_ISO, x, y, xSize, ySize);
}

Forma novaLinha(float x, float y, float xSize, float ySize) {
	return novaForma(LINHA, x, y, xSize, ySize);
}

Forma novoTrianguloEqui(float x, float y, float xSize, float ySize) {
	return novaForma(TRIANGULO_EQUI, x, y, xSize, ySize);
}

Forma novoHexa(float x, float y, float raio) {
	return novaForma(HEXA, x, y, raio, raio); 
}

Forma novaCircu(float x, float y, float raio) {
	return novaForma(CIRCU, x, y, raio, raio);
}

Forma novaEstrela(float x, float y, float raio) {
	return novaForma(ESTRELA, x, y, raio, raio);
}




Forma novoRectangulo2Pontos(float xi, float yi, float xf, float yf) { 
	return novaForma(RECTANGULO, xi, yi, xf - xi, yf - yi);
}

Forma novoQuadrado2Pontos(float xi, float yi, float xf, float yf) {
	return novaForma(QUADRADO, xi, yi, xf - xi, xf - xi);
}

Forma novoTrianguloIso2Pontos(float xi, float yi, float xf, float yf) {
	return novaForma(TRIANGULO_ISO, xi, yi, xf - xi, yf - yi);
}

Forma novaLinha2Pontos(float xi, float yi, float xf, float yf) {
	return novaForma(LINHA, xi, yi, xf - xi, yf - yi);
}

Forma novoTrianguloEqui2Pontos(float xi, float yi, float xf, float yf) {
	return novaForma(TRIANGULO_EQUI, xi, yi, xf - xi, yf - yi);
}

Forma novoHexa2Pontos(float xi, float yi, float xf, float yf) {
	float raio = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) / 2.0; 
	return novaForma(HEXA, xi, yi, raio, raio); 
}

Forma novaCircu2Pontos(float xi, float yi, float xf, float yf) {
	float raio = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) / 2.0; 
	return novaForma(CIRCU, (xi + xf) / 2.0, (yi + yf) / 2.0, raio, raio);
}

Forma novaEstrela2Pontos(float xi, float yi, float xf, float yf) {
	float centroX = (xi + xf) / 2.0;
	float centroY = (yi + yf) / 2.0;

	float raio = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) / 2.0;

	// Cria a forma estrela com base no centro e no raio calculados
	return novaForma(ESTRELA, centroX, centroY, raio, raio); 
}


int pedirX(Forma f) { return f->x;}
int pedirY(Forma f) { return f->y;}


int escolher(Forma f, float x, float y) {

	if(f->tipo == HEXA || f->tipo == CIRCU || f->tipo == ESTRELA)
		return ((x - f->x) * (x - f->x) + (y - f->y) * (y - f->y)) <= (f->xSize * f->xSize);
	else {
		float xMin = fmin(f->x, f->x + f->xSize); // Calcula o menor valor de X
		float xMax = fmax(f->x, f->x + f->xSize); // Calcula o maior valor de X
		float yMin = fmin(f->y, f->y + f->ySize); // Calcula o menor valor de Y
		float yMax = fmax(f->y, f->y + f->ySize); // Calcula o maior valor de Y

		// Verifica se o ponto está dentro do retângulo delimitador
		return (x >= xMin && x <= xMax && y >= yMin && y <= yMax);
	}
}

void apagarForma(Forma f) {
	free(f);
}

void atualizarForma(Forma f, float dx, float dy) {
	f->x += dx;
	f->y += dy;
}

void mudar2Pontos(Forma f, float x, float y) {
	
	f->xSize = x - f->x;
	f->ySize = y - f->y;

	if (f->tipo == QUADRADO || f->tipo == TRIANGULO_EQUI ) {

		if (f->xSize >= 0) {
			if (f->ySize >= 0) {
				if (f->ySize >= f->xSize) {
					f->xSize = f->ySize;

				}
				else
				{
					f->ySize = f->xSize;
				}
			}
			else
			{
				if (-(f->ySize) >= f->xSize) {
					f->xSize = -(f->ySize);
				}
				else
				{
					f->ySize = -(f->xSize);
				}
			}
		}
		else
		{
			if (f->ySize >= 0) {
				if (f->ySize >= -(f->xSize))
					f->xSize = -(f->ySize);
				else
					f->ySize = -(f->xSize);
			}
			else {
				if (-(f->ySize) >= -(f->xSize))
					f->xSize = f->ySize;
				else
					f->ySize = f->xSize;
			}
		}
	}
}


void corFundo(Forma f, float r, float g, float b){
	f->r = r;
	f->g = g;
	f->b = b;
}

void corContorno(Forma f, float rContorno, float gContorno, float bContorno) {
	f->rContorno = rContorno;
	f->gContorno = gContorno;
	f->bContorno = bContorno;
	
}

void corAtualFundo(Forma f, float* c) {
	c[0] = f->r;
	c[1] = f->g;
	c[2] = f->b;
}

void corAtualContorno(Forma f, float* ct) {
	ct[0] = f->rContorno;
	ct[1] = f->gContorno;
	ct[2] = f->bContorno;
}


void printfForma(Forma f) {
	printf("Forma: (%.2f,%.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\t",
		f->x, f->y,
		f->x, f->y + f->ySize,
		f->x + f->xSize, f->y + f->ySize,
		f->x + f->xSize, f->y);


	printf("Centro(%.2f,%.2f) Tamanho{%.2f,%.2f}\n",
		f->x + (f->xSize / 2), f->y + (f->ySize / 2),
		f->xSize, f->ySize);

}

void desenharTriangulo(Forma f) {
	glColor3f(f->r, f->g, f->b);
	glBegin(GL_POLYGON);
	glVertex2f(f->x, f->y);
	glVertex2f(f->x + (f->xSize / 2.0), f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y);
	glEnd();

	glColor3f(f->rContorno, f->gContorno, f->bContorno);
	glBegin(GL_LINE_LOOP);
	glVertex2f(f->x, f->y);
	glVertex2f(f->x + (f->xSize / 2.0), f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y);
	glEnd();
}

void desenharQuadrilatero(Forma f) {
	glColor3f(f->r, f->g, f->b);
	glBegin(GL_POLYGON);
	glVertex2f(f->x, f->y);
	glVertex2f(f->x, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y);
	glEnd();

	glColor3f(f->rContorno, f->gContorno, f->bContorno);
	glLineWidth(2.0); //Grossura da linha de contorno
	glBegin(GL_LINE_LOOP);
	glVertex2f(f->x, f->y);
	glVertex2f(f->x, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y + f->ySize);
	glVertex2f(f->x + f->xSize, f->y);
	glEnd();
}

void desenharLinha(Forma f) {
	glColor3f(f->r, f->g, f->b);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex2f(f->x, f->y);             
	glVertex2f(f->x + f->xSize, f->y + f->ySize); 
	glEnd();
}

void desenharHexa(Forma f ) {
	glColor3f(f->r, f->g, f->b);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 6; i++) {
		float angulo = 2.0 * PI * i / 6.0;
		glVertex2f(f->x + f->xSize * cos(angulo), f->y + f->ySize * sin(angulo));
	}
	glEnd();

	glColor3f(f->rContorno, f->gContorno, f->bContorno);
	glLineWidth(2.0); 
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 6; i++) {
		float angulo = 2.0 * PI * i / 6.0;
		glVertex2f(f->x + f->xSize * cos(angulo), f->y + f->ySize * sin(angulo));
	}
	glEnd();
}

void desenharCircu(Forma f) {
	glColor3f(f->r, f->g, f->b);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float angulo = i * PI / 180.0;
		glVertex2f(f->x + cos(angulo) * f->xSize, f->y + sin(angulo) * f->ySize);
	}
	glEnd();

	glColor3f(f->rContorno, f->gContorno, f->bContorno);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float angulo = i * PI / 180.0;
		glVertex2f(f->x + cos(angulo) * f->xSize, f->y + sin(angulo) * f->ySize);
	}
	glEnd();
}

void desenharEstrela(Forma f) {
	int pontos = 10;  
	float raioMaior = f->xSize;
	float raioMenor = f->xSize / 2.5; 
	float anguloInicial = -PI / 2; 

	glColor3f(f->r, f->g, f->b);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < pontos; i++) {
		float angulo1 = anguloInicial + (2.0 * PI * i / pontos);
		float angulo2 = anguloInicial + (2.0 * PI * (i + 1) / pontos);

		float raio1;
		if (i % 2 == 0) {
			raio1 = raioMaior;
		}
		else {
			raio1 = raioMenor;
		}

		float raio2;
		if ((i + 1) % 2 == 0) {
			raio2 = raioMaior;
		}
		else {
			raio2 = raioMenor;
		}

		float x1 = f->x + cos(angulo1) * raio1;
		float y1 = f->y + sin(angulo1) * raio1;

		float x2 = f->x + cos(angulo2) * raio2;
		float y2 = f->y + sin(angulo2) * raio2;

		glVertex2f(f->x, f->y); 
		glVertex2f(x1, y1);      
		glVertex2f(x2, y2);     
	}
	glEnd();

	
	glColor3f(f->rContorno, f->gContorno, f->bContorno);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < pontos; i++) {
		float angulo = anguloInicial + (2.0 * PI * i / pontos);

		float raio;
		if (i % 2 == 0) {
			raio = raioMaior;
		}
		else {
			raio = raioMenor;
		}

		float x = f->x + cos(angulo) * raio;
		float y = f->y + sin(angulo) * raio;
		glVertex2f(x, y);
	}
	glEnd();
}


void desenharForma(Forma f) {
	switch (f->tipo) {
	case RECTANGULO: desenharQuadrilatero(f); break;
	case QUADRADO: desenharQuadrilatero(f); break;
	case TRIANGULO_ISO: desenharTriangulo(f); break;
	case LINHA: desenharLinha(f); break;
	case TRIANGULO_EQUI: desenharTriangulo(f); break;
	case HEXA: desenharHexa(f); break;
	case CIRCU: desenharCircu(f); break;
	case ESTRELA: desenharEstrela(f); break; 
	}
}