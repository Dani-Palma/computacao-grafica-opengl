#include "Ferramentas.h"
#include "BDFormas.h"
#include "Formas.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

float rEstado, gEstado, bEstado; // Cores Fundo

float rCEstado, gCEstado, bCEstado; // Cores Contorno


void initFerramentas() {  //Inicializat variaveis
	rEstado = 1.0;
	gEstado = 1.0;
	bEstado = 1.0;
    criarForma = 0;
	corAtiva = 0;
	corAtivaC = 0;
	estadoAtual = INSERIR;
	emMovimento = 0;
	alterando = 0;
    formaSelecionada = NULL;
	formaAtual = RECTANGULO;
}

void atualizarEstadoAtivo(int novoEstado) { // atualiza o estado
	estadoAtual = novoEstado;
	glutPostRedisplay(); 
}

void atualizarPaleta(Forma* paleta, int nPaletas, int paletaAtual) {
	for (int i = 0; i < nPaletas; i++) {
		if (i == paletaAtual) {
			corFundo(paleta[i], 0.0, 0.0, 1.0); // Destaca a forma atual com azul
		}
		else {
			corFundo(paleta[i], 1.0, 1.0, 1.0); // Fundo branco para as demais
		}
		desenharForma(paleta[i]); // Desenha a forma
	}
}


void initPaleta() { //Define as paletas no cabecalho
	
	corAtiva = novoQuadrado(10, 475, 20);				//Quadrado com a cor de fundo e de contorno atuais
	corFundo(corAtiva, rEstado, gEstado, bEstado);

	nPaletaCores = 11;
	paletaCor = malloc(sizeof(Forma) * nPaletaCores);

	nPaletaFormas = 8;
	paletaForma = malloc(sizeof(Forma) * nPaletaFormas);

	nPaletaEstados = 5;
	paletaEstado = malloc(sizeof(Forma) * nPaletaEstados);

	nPaletaEstadosLinhas = 14;							//Estas linhas e formas servem para desenhar dentro das paletas dos estados para distinguir os mesmos
	paletaEstadoLinha = malloc(sizeof(Forma) * nPaletaEstadosLinhas);


	paletaCor[0] = novoQuadrado(40, 475, 20);
	corFundo(paletaCor[0], 1.0, 0.0, 0.0);

	paletaCor[1] = novoQuadrado(70, 475, 20);
	corFundo(paletaCor[1], 0.0, 1.0, 0.0);

	paletaCor[2] = novoQuadrado(100, 475, 20);
	corFundo(paletaCor[2], 0.0, 0.0, 1.0);

	paletaCor[3] = novoQuadrado(130, 475, 20);
	corFundo(paletaCor[3], 1.0, 0.0, 1.0);

	paletaCor[4] = novoQuadrado(160, 475, 20);
	corFundo(paletaCor[4], 1.0, 1.0, 0.0);

	paletaCor[5] = novoQuadrado(10, 445, 20);
	corFundo(paletaCor[5], 1.0, 0.55, 0.0);

	paletaCor[6] = novoQuadrado(40, 445, 20);
	corFundo(paletaCor[6], 0.75, 0.75, 0.75);

	paletaCor[7] = novoQuadrado(70, 445, 20);
	corFundo(paletaCor[7], 0.87, 0.72, 0.53);

	paletaCor[8] = novoQuadrado(100, 445, 20);
	corFundo(paletaCor[8], 0.0, 0.78, 1.0);

	paletaCor[9] = novoQuadrado(130, 445, 20);
	corFundo(paletaCor[9], 0.29, 0.0, 0.51);

	paletaCor[10] = novoQuadrado(160, 445, 20);
	corFundo(paletaCor[10], 1.0, 1.0, 1.0);

	
	paletaForma[0] = novoRectangulo(470, 470, 40, 20);

	paletaForma[1] = novoQuadrado(520, 470, 20);

	paletaForma[2] = novoTrianguloIso(550, 470, 20, 30);

	paletaForma[3] = novaLinha(580, 470, 10, 20);

	paletaForma[4] = novoTrianguloEqui(590, 470, 20, 20);

	paletaForma[5] = novoHexa(630, 480, 13);

	paletaForma[6] = novaCircu(665, 480, 13);

	paletaForma[7] = novaEstrela(445, 480, -15);

	
	paletaEstado[0] = novoQuadrado(250, 473, 25),
	paletaEstadoLinha[0] = novaLinha(262.5, 475, 0, 13);
	corFundo(paletaEstadoLinha[0], 0.0, 0.0, 0.0);
	paletaEstadoLinha[1] = novoTrianguloEqui(254.5, 488, 15, 6);
	corFundo(paletaEstadoLinha[1], 0.0, 0.0, 0.0);

	paletaEstado[1] = novoQuadrado(280, 473, 25);
	paletaEstadoLinha[2] = novaLinha(286.5, 479, 14, 14);
	corFundo(paletaEstadoLinha[2], 0.0, 0.0, 0.0);
	paletaEstadoLinha[3] = novaLinha(286.5, 492, 14, -14);
	corFundo(paletaEstadoLinha[3], 0.0, 0.0, 0.0);

	paletaEstado[2] = novoQuadrado(310, 473, 25);
	paletaEstadoLinha[4] = novaLinha(313, 485.5, 18, 0);
	corFundo(paletaEstadoLinha[4], 0.0, 0.0, 0.0);
	paletaEstadoLinha[5] = novaLinha(323, 494.5, 8, -8);
	corFundo(paletaEstadoLinha[5], 0.0, 0.0, 0.0);
	paletaEstadoLinha[6] = novaLinha(323, 477.5, 8, 8);
	corFundo(paletaEstadoLinha[6], 0.0, 0.0, 0.0);

	paletaEstado[3] = novoQuadrado(265, 443, 25);
	paletaEstadoLinha[7] = novoRectangulo(269, 455.5, 17, 10);
	corFundo(paletaEstadoLinha[7], 0.0, 1.0, 0.0);
	corContorno(paletaEstadoLinha[7], 1.0, 1.0, 1.0);
	paletaEstadoLinha[8] = novoRectangulo(269, 455.5, 17, -10);
	corFundo(paletaEstadoLinha[8], 1.0, 0.0, 0.0);
	corContorno(paletaEstadoLinha[8], 1.0, 1.0, 1.0);


	paletaEstado[4] = novoQuadrado(295, 443, 25);	
	paletaEstadoLinha[9] = novaLinha(298, 455.5, 18, 0);
	corFundo(paletaEstadoLinha[9], 0.0, 0.0, 0.0);
	paletaEstadoLinha[10] = novaLinha(316, 455.5, -7, -7);
	corFundo(paletaEstadoLinha[10], 0.0, 0.0, 0.0);
	paletaEstadoLinha[11] = novaLinha(316, 455.5, -7, 7);
	corFundo(paletaEstadoLinha[11], 0.0, 0.0, 0.0);
	paletaEstadoLinha[12] = novaLinha(298, 455.5, 7, 7);
	corFundo(paletaEstadoLinha[12], 0.0, 0.0, 0.0);
	paletaEstadoLinha[13] = novaLinha(298, 455.5, 7, -7);
	corFundo(paletaEstadoLinha[13], 0.0, 0.0, 0.0);
	
}


void cabecalho(GLint botao, GLint estado, GLint x, GLint y) {
	int i;
	float cor[3];

	if (estado == GLUT_DOWN) { // Processar apenas cliques (botão pressionado)
		if (botao == GLUT_LEFT_BUTTON) {
			printf("-> Botão esquerdo pressionado!\n");
			if (estadoAtual == INSERIR || estadoAtual == MUDARCOR) { // Apenas deixa mudar a cor de fundo quando estou no estado inserir e mudar a cor
				for (i = 0; i < nPaletaCores; i++) {
					if (escolher(paletaCor[i], x, y)) {
						corAtualFundo(paletaCor[i], cor); // Obtém a cor da paleta
						if (estadoAtual == MUDARCOR && formaSelecionada) {
							corFundo(formaSelecionada, cor[0], cor[1], cor[2]); // Atualiza o fundo da forma selecionada no estado mudar a cor
							formaSelecionada = NULL; // Redefine após atualizar
						}
						else {
							// Atualiza as cores padrão
							rEstado = cor[0];
							gEstado = cor[1];
							bEstado = cor[2];
							corFundo(corAtiva, rEstado, gEstado, bEstado);
						}
						return; 
					}
				}
			}
			if (estadoAtual == INSERIR) { //Apenas deixa mudar de forma quando estou no modo inserir
				for (i = 0; i < nPaletaFormas; i++) {
					if (escolher(paletaForma[i], x, y)) {
						formaAtual = i; // Atualiza a forma ativa
					}
				}
			}

			for (i = 0; i < nPaletaEstados; i++) {
				if (escolher(paletaEstado[i], x, y)) {
					atualizarEstadoAtivo(i); // Atualiza o estado ativo
					printf("Estado atualizado para: %c\n", estadoAtual);
					return;
				}
			}
		}
		if (estadoAtual == INSERIR || estadoAtual == MUDARCOR) { // Apenas deixa mudar a cor de contorno quando estou no estado inserir e mudar a cor
			// Clique Direito - Atualizar cor de contorno
			if (botao == GLUT_RIGHT_BUTTON) {
				printf("-> Botão direito pressionado!\n");
				for (i = 0; i < nPaletaCores; i++) {
					if (escolher(paletaCor[i], x, y)) {
						corAtualFundo(paletaCor[i], cor); // Obtém a cor da paleta
						if (estadoAtual == MUDARCOR && formaSelecionada) {
							// Atualiza o contorno da forma selecionada no estado mudar cor
							corContorno(formaSelecionada, cor[0], cor[1], cor[2]);
							formaSelecionada = NULL; 
						}
						else {
							rCEstado = cor[0];
							gCEstado = cor[1];
							bCEstado = cor[2];
							corContorno(corAtiva, rCEstado, gCEstado, bCEstado);
						}
						return; 
					}
				}
			}
		}
	}

	glutPostRedisplay(); 

}


void criarUmaForma(int tipo, float x, float y) {

	printf("Estado de inserir\n");
	if (criarForma) {
		mudar2Pontos(formaSelecionada, x, y);
		criarForma = 0;
		formaSelecionada = NULL;
	}
	else {
		
		switch (tipo) {
		case RECTANGULO:
			formaSelecionada = novoRectangulo2Pontos(x, y, x, y);
			break;
		case QUADRADO:
			formaSelecionada = novoQuadrado2Pontos(x, y, x, y);
			break;
		case TRIANGULO_ISO:
			formaSelecionada = novoTrianguloIso2Pontos(x, y, x, y);
			break;
		case LINHA:
			formaSelecionada = novaLinha2Pontos(x, y, x, y);
			break;
		case TRIANGULO_EQUI:
			formaSelecionada = novoTrianguloEqui2Pontos(x, y, x, y);
			break;
		case HEXA:
			formaSelecionada = novoHexa2Pontos(x, y, x, y);
			break;
		case CIRCU:
			formaSelecionada = novaCircu2Pontos(x, y, x, y);
			break;
		case ESTRELA:
			formaSelecionada = novaEstrela2Pontos(x, y, x, y); 
			break;
		}

		// Aplica a cor e insere na base de dados
		corFundo(formaSelecionada, rEstado, gEstado, bEstado);
		corContorno(formaSelecionada, rCEstado, gCEstado, bCEstado);
		if (!inserirBDFormas(formaSelecionada)) {
			printf("MEMORIA CHEIA!!!\n");
			apagarForma(formaSelecionada);
			formaSelecionada = NULL;
		}
		else {
			criarForma = 1;
		}
	}
}


void apagarFormaRato(int x, int y) {
	
	Forma f = escolherBDFormas(x, y);
	printf("Estado de apagar\n");
	if (f != NULL) {
		printfForma(f);
		apagarBDFormas(f);
		glutPostRedisplay();
	}
}

void gerirMovimentoForma(int x, int y) {

	printf("Estado de mover forma\n");
	if (!emMovimento) { // Primeiro clique
		formaSelecionada = escolherBDFormas(x, y); 
		if (formaSelecionada) {
			printf("Forma selecionada para ser movida.\n");
			emMovimento = 1;
			glutPostRedisplay(); 
		}
	}
	else {
		if (formaSelecionada) { // Segundo clique
			atualizarForma(formaSelecionada, x - pedirX(formaSelecionada), y - pedirY(formaSelecionada));
			printf("Forma posicionada na posição final.\n");
		}
		emMovimento = 0;
		formaSelecionada = NULL;
		glutPostRedisplay();
	}
}


void alterarCores(int x, int y) {

	printf("Estado de alterar cor de fundo de contorno\n");
	formaSelecionada = escolherBDFormas(x, y);
	if (formaSelecionada) {
		printf("Forma selecionada para alteração de cor.\n");
	}
	else {
		printf("Nenhuma forma encontrada para alterar cor.\n");
	}
	glutPostRedisplay();
}

void alterarForma(int x, int y) {

	printf("Estado de alterar forma\n");
	if (!alterando) {
		formaSelecionada = escolherBDFormas(x, y);
		if (formaSelecionada) {
			printf("Forma selecionada para alterar.\n");
			alterando = 1;
		}
	}
	else {
		alterando = 0;
		formaSelecionada = NULL;
	}
}

void ratoTela(GLint botao, GLint estado, GLint x, GLint y) {

	if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		printf("-> Botão esquerdo pressionado!\n");
		switch (estadoAtual) {
		case INSERIR:
			criarUmaForma(formaAtual, x, y); 
			break;
		case APAGAR:
			apagarFormaRato(x, y);
			break;
		case MOVER:
			gerirMovimentoForma(x, y);
			break;
		case MUDARCOR:
			alterarCores(x, y);
			break;
		case MUDARFORMA:
			alterarForma(x, y);
			break;
		}
	}
}






