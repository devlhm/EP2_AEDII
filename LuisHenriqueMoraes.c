#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3 /* grau min da árvore*/

typedef int TipoChave;

typedef struct str_no
{
	TipoChave chave[2 * t - 1];
	struct str_no *filhos[2 * t];
	int numChaves;
	bool folha;
} NO;

typedef struct
{
	NO *raiz;
} ArvB;

bool verificaNoCheio(NO* no) {
	return no->numChaves == (2*t)-1;
}

void divideNo(NO* pai, int i, NO* no) {
	NO* novo = (NO*) malloc(sizeof(NO));
	novo->folha = no->folha;
	novo->numChaves = t-1;

	for(int j = 0; j < t - 1; j++) {
		novo->chave[j] = no->chave[j + t];
	}

	if(!no->folha) {
		for(int j = 0; j < t; j++) {
			novo->filhos[j] = no->filhos[j + t];
		}
	}

	no->numChaves = t-1;

	for(int j = pai->numChaves; j > i; j--) {
		pai->filhos[j + 1] = pai->filhos[j];
	}

	pai->filhos[i + 1] = novo;

	for(int j = pai->numChaves - 1; j >= i; j--) {
		pai->chave[j+1] = pai->chave[j];
	}

	pai->chave[i] = no->chave[t - 1];
	pai->numChaves += 1;
}

void insereNoNaoCheio(NO* no, int chave)
{
	int i = no->numChaves - 1;
	if(no->folha) {
		while(i >= 0 && chave < no->chave[i]) {
			no->chave[i+1] = no->chave[i];
			i -= 1;
		}

		no->chave[i+1] = chave;
		no->numChaves += 1;
	} else {
		while(i >= 0 && chave < no->chave[i]) {
			i -= 1;
		}

		i += 1;

		if(verificaNoCheio(no->filhos[i])) {
			divideNo(no, i, no->filhos[i]);
			if(chave > no->chave[i]) {
				i += 1;
			}
		}

		insereNoNaoCheio(no->filhos[i], chave);
	}
}

void insere(ArvB* arvore, int chave) {
	NO* raiz = arvore->raiz;
	if(verificaNoCheio(raiz)) {
		NO* s = (NO*) malloc(sizeof(NO));
		arvore->raiz = s;
		s->folha = false;
		s->numChaves = 0;
		s->filhos[0] = raiz;
		divideNo(s, 0, raiz);
		insereNoNaoCheio(s, chave);
	} else {
		insereNoNaoCheio(raiz, chave);
	}
}

bool criaArvoreB(ArvB *arvore)
{
	NO *x;

	if (!(x = (NO *)malloc(sizeof(NO))))
		return false;

	x->folha = true;
	x->numChaves = 0;
	arvore->raiz = x;
	return true;
}

void executaComando(char comando, ArvB *arvore, FILE *arq_entrada)
{
	switch (comando)
	{
	case 'i':
		int chave;
		fscanf(arq_entrada, "%d", &chave);
		insere(arvore, chave);
		break;
	case 'r':
		int chave;
		fscanf(arq_entrada, "%d", &chave);

		// remove key
	case 'p':

		// print tree, se vazia print "Vazia"

		break;
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		return -1;
	}

	FILE *arq_entrada = fopen(argv[1], "r");
	char comando = fgetc(arq_entrada);

	ArvB *arvore = (ArvB *)malloc(sizeof(ArvB));
	if(!criaArvoreB(arvore)) return -1;

	while (comando != 'f')
	{
		printf("%c", comando);
		if (comando != '\n')
			executaComando(comando, arvore, arq_entrada);

		comando = fgetc(arq_entrada);
	}

	return 1;
}