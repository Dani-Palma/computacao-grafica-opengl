
/*

O trabalho foi feito pelo Daniel Palma          nº24664


---------------------------------------- FUNCIONAMENTO GERAL DO PAINT -----------------------------------------------------

No estado inserir para mudar a cor de fundo clico com o click esquerdo e para mudar a cor de contorno clico com o click direito sobre a cor que quero

Apenas uso o click direito para mudar a cor de contorno, o resto é tudo com o esquerdo

Para apagar a forma clico sobre a figura e apaga

Para mover forma clico a primeira vez sobre a forma e deixo de clicar, o segundo clique serve para definir onde ela fica

Para alterar a cor de fundo da figura clico cobre essa figura com o clique esquerdo e clico com o botao esquerdo do rato sobre a cor quero, para mudar a cor de contorno é a mesma coisa mas uso o botao direito para clicar na cor que quero

Para alterar a forma clico sobre ela a primeira vez e o segundo clique serve para a definir

Por ultimo tambem se pode mudar de estado e de figura pelo teclado é apenas ver as teclas que fazem essas funcionalidades no código abaixo

*/
#include "Formas.h"
#include "BDFormas.h"
#include "Ferramentas.h"
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <stdio.h>

int contador = 0;

int janelaX = 700;
int janelaY = 500;


void desenharCabecalho() { // Pintar o fundo do cabeçalho e desenhar

    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_QUADS);
    glVertex2f(0.0, janelaY - 60);  
    glVertex2f(janelaX, janelaY - 60); 
    glVertex2f(janelaX, janelaY);  
    glVertex2f(0.0, janelaY); 
    glEnd();
}

void tela() {
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    glVertex2f(-4.0, 0.0);
    glVertex2f(4.0, 0.0);
    glVertex2f(0.0, -4.0);
    glVertex2f(0.0, 4.0);
    glEnd();

    desenharBDFormas(); 

    desenharCabecalho();

    desenharForma(corAtiva);  //desenha o quadrado com a cor ativa e contorno

    for (i = 0; i < nPaletaCores; i++) {
        desenharForma(paletaCor[i]);
    }

    atualizarPaleta(paletaForma, nPaletaFormas, formaAtual);  //atualizar a cor das formas do cabecalho para saber qual é a atual

    atualizarPaleta(paletaEstado, nPaletaEstados, estadoAtual);
    
    for (i = 0; i < nPaletaEstadosLinhas; i++) {
        desenharForma(paletaEstadoLinha[i]);
    }

    glFlush();

    printf("clique: %i\n", ++contador); //Conta o número de cliques
}

void rato(GLint botao, GLint estado, GLint x, GLint y) {
    y = janelaY - y;  


    if (y > janelaY - 60) {

        if(!criarForma && !alterando){  //Nao deixa clicar nas paletas qaundo esta a criar se uma forma ou a alterar-se

            cabecalho(botao, estado, x, y); 

        }
    }
    else {
        ratoTela(botao, estado, x, y);  
    }
}

void init() {
    srand(time(NULL));

    iniciarBDFormas(50);

    printBDFormas();

    initFerramentas();

    initPaleta();

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glColor3f(1.0, 1.0, 1.0);

    glViewport(0, 0, janelaX, janelaY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, janelaX, 0.0, janelaY);
}

void remodelarTela(int w, int h) {

    float deltaY = h - janelaY;  //calcula a diferenca da altura da janela quando é rederecionada para o cabecalho atualizar ao mesmo tempo
    janelaX = w;  
    janelaY = h;  

    glViewport(0, 0, janelaX, janelaY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, janelaX, 0.0, janelaY);

    atualizarForma(corAtiva, 0, deltaY);

    for (int i = 0; i < nPaletaCores; i++) {
        atualizarForma(paletaCor[i], 0, deltaY); 
    }

    for (int i = 0; i < nPaletaFormas; i++) {
        atualizarForma(paletaForma[i], 0, deltaY); 
    }

    for (int i = 0; i < nPaletaEstados; i++) {
        atualizarForma(paletaEstado[i], 0, deltaY);
    }

    for (int i = 0; i < nPaletaEstadosLinhas; i++) {
        atualizarForma(paletaEstadoLinha[i], 0, deltaY); 
    }

    glutPostRedisplay();  
}

void teclado(unsigned char tecla, int x, int y)  //atalhos pelo teclado para as funcionalidades
{
    Forma f;

    y = janelaY - y;

    if (tecla == 'S' | tecla == 's')
        exit(0);

    

    switch (tecla) {

        if (estadoAtual == INSERIR) {
            case 'r':
            case 'R':
                formaAtual = RECTANGULO; break;

            case 'q':
            case 'Q':
                formaAtual = QUADRADO; break; 

            case 'i':
            case 'I':
                formaAtual = TRIANGULO_ISO; break;

            case 'l':
            case 'L':
                formaAtual = LINHA; break;

            case 't':
            case 'T':
                formaAtual = TRIANGULO_EQUI; break;

            case 'h':
            case 'H':
                formaAtual = HEXA; break;

            case 'c':
            case 'C':
                formaAtual = CIRCU; break;

            case 'e':
            case 'E':
                formaAtual = ESTRELA; break;
               
        }
        case '1':
            estadoAtual = INSERIR; break; 
        case '2':
            estadoAtual = APAGAR; break; 
        case '3':
            estadoAtual = MOVER; break; 
        case '4':
            estadoAtual = MUDARCOR; break;
        case '5':
            estadoAtual = MUDARFORMA; break;

    }
}

void movimentoPassivoRato(int x, int y) {
    y = janelaY - y;

    if (criarForma || alterando) {  //deixa alterar e criar a forma entre o clique inicial e final sem estar a clicar no rato
        mudar2Pontos(formaSelecionada, x, y);   
        glutPostRedisplay();
    }

    if (emMovimento && formaSelecionada) { //desenha a forma temporiariamente quando está a ser movida
        atualizarForma(formaSelecionada, x - pedirX(formaSelecionada), y - pedirY(formaSelecionada));
        glutPostRedisplay();
    }

}

// Função principal
void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(janelaX, janelaY);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Paint");
    glutDisplayFunc(tela);
    glutMouseFunc(rato);
    glutReshapeFunc(remodelarTela);
    glutKeyboardFunc(teclado); 
    glutPassiveMotionFunc(movimentoPassivoRato);

    init();

    glutMainLoop();
}
