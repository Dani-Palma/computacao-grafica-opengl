#ifndef __FORMAS_H__
#define __FORMAS_H__


#define RECTANGULO 0
#define QUADRADO 1
#define TRIANGULO_ISO 2
#define LINHA 3
#define TRIANGULO_EQUI 4
#define HEXA 5
#define CIRCU 6
#define ESTRELA 7
#define PI 3.14159265358979323846



typedef struct forma* Forma;


Forma novoRectangulo(float, float, float, float);
Forma novoQuadrado(float, float, float);
Forma novoTrianguloIso(float, float, float, float);
Forma novaLinha(float, float, float, float);
Forma novoTrianguloEqui(float, float, float, float);
Forma novoHexa(float, float, float);                                                              
Forma novaCircu(float, float, float);
Forma novaEstrela(float, float, float);

Forma novoRectangulo2Pontos(float, float, float, float);
Forma novoQuadrado2Pontos(float, float, float, float);
Forma novoTrianguloIso2Pontos(float, float, float, float);
Forma novaLinha2Pontos(float, float, float, float);
Forma novoTrianguloEqui2Pontos(float, float, float, float);
Forma novoHexa2Pontos(float, float, float, float);
Forma novaCircu2Pontos(float, float, float, float);
Forma novaEstrela2Pontos(float, float, float, float);


int pedirX(Forma);
int pedirY(Forma);


int escolher(Forma, float, float);

void apagarForma(Forma );

void atualizarForma(Forma, float, float);

void mudar2Pontos(Forma, float, float);

void corFundo(Forma, float, float, float);

void corContorno(Forma, float, float, float);

void corAtualContorno(Forma, float*);

void corAtualFundo(Forma, float*);

void printfForma(Forma);

void desenharForma(Forma);


#endif