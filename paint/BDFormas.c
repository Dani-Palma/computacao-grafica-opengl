#include "Formas.h"
#include "BDFormas.h"
#include <stdlib.h>
#include <stdio.h>

Forma* a;
int n;

void iniciarBDFormas(int num) {
	int i;

	n = num;

	a = malloc(sizeof(Forma) * n);

	for (i = 0; i < n; i++)
		a[i] = NULL;
}

int inserirBDFormas(Forma f) {
	int i;

	for (i = 0; i < n; i++)
		if (a[i] == NULL) {
			a[i] = f;
			return 1;
		}

	return 0;
}

void desenharBDFormas() {
	int i;
	for (i = 0; i < n; i++)
		if (a[i] != NULL)
			desenharForma(a[i]);
}



void printBDFormas() {
	int i;

	for (i = 0; i < n; i++)
		if (a[i] != NULL)
			printfForma(a[i]);
}


Forma escolherBDFormas(float x, float y) {
	int i;

	for (i = n - 1; i >= 0; i--)
		if (a[i] != NULL && escolher(a[i], x, y)) {
			return a[i];
		}

	return NULL;
}

int apagarBDFormas(Forma f) {
	int i;

	int formaApagada = 0;

	for (i = 0; i < n && a[i] != NULL; i++) {
		if (f == a[i]) {
			apagarForma(a[i]);
			formaApagada = 1;
			break;
		}
	}

	if (formaApagada) {
		for (i = i + 1; i < n && a[i] != NULL; i++) {
			a[i - 1] = a[i];
		}
		a[i - 1] = NULL;
	}

	return formaApagada;
}

