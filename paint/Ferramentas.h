#ifndef FERRAMENTAS_H
#define FERRAMENTAS_H

#include "Formas.h"
#include <GL/glut.h>

//Estados
#define INSERIR 0
#define APAGAR 1
#define MOVER 2
#define MUDARCOR 3
#define MUDARFORMA 4


// Declaração de variáveis globais
int criarForma;
Forma formaSelecionada;
Forma* paletaCor;
Forma* paletaForma;
Forma* paletaEstado;
Forma* paletaEstadoLinha;
int nPaletaCores;
int nPaletaFormas;
int nPaletaEstados;
int nPaletaEstadosLinhas;
int formaAtual;
int estadoAtual;
int corAtiva;
int corAtivaC;
int emMovimento; 
int xUltimoClique, yUltimoClique; 
int alterando;

void initFerramentas(); 
void cabecalho(GLint, GLint, GLint, GLint);
void ratoTela(GLint, GLint, GLint, GLint);
void initPaleta();
void atualizarPaleta(Forma*, int, int);

#endif

