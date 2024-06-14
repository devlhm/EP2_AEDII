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

bool verificaNoCheio(NO *no)
{
	return no->numChaves == (2 * t) - 1;
}

void divideNo(NO *pai, int i, NO *no)
{
	NO *novo = (NO *)malloc(sizeof(NO));
	novo->folha = no->folha;
	novo->numChaves = t - 1;
	if (novo->folha)
		novo->numChaves += 1;

	for (int j = 0; j < novo->numChaves; j++)
	{
		int q = j + t;
		if (novo->folha)
			q -= 1;
		novo->chave[j] = no->chave[q];
	}

	if (!no->folha)
	{
		for (int j = 0; j < t; j++)
		{
			novo->filhos[j] = no->filhos[j + t];
		}
	}
	else
	{
		novo->filhos[novo->numChaves] = no->filhos[no->numChaves];
		no->filhos[no->numChaves] = NULL;
		no->filhos[t - 1] = novo;
	}

	no->numChaves = t - 1;

	for (int j = pai->numChaves; j > i; j--)
	{
		pai->filhos[j + 1] = pai->filhos[j];
	}

	pai->filhos[i + 1] = novo;

	for (int j = pai->numChaves - 1; j >= i; j--)
	{
		pai->chave[j + 1] = pai->chave[j];
	}

	pai->chave[i] = no->chave[t - 1];
	pai->numChaves += 1;
}

void insereNoNaoCheio(NO *no, int chave)
{
	int i = no->numChaves - 1;
	if (no->folha)
	{
		while (i >= 0 && chave < no->chave[i])
		{
			no->chave[i + 1] = no->chave[i];
			i -= 1;
		}

		no->chave[i + 1] = chave;
		no->numChaves += 1;
		no->filhos[no->numChaves] = no->filhos[no->numChaves - 1];
		no->filhos[no->numChaves - 1] = NULL;
	}
	else
	{
		while (i >= 0 && chave < no->chave[i])
		{
			i -= 1;
		}

		i += 1;
		verificaNoCheio(no->filhos[i]);
		if (verificaNoCheio(no->filhos[i]))
		{
			divideNo(no, i, no->filhos[i]);
			if (chave > no->chave[i])
			{
				i += 1;
			}
		}

		insereNoNaoCheio(no->filhos[i], chave);
	}
}

void insere(ArvB *arvore, int chave)
{
	printf("Insere %d: ", chave);
	NO *raiz = arvore->raiz;
	if (verificaNoCheio(raiz))
	{
		printf(" raiz cheia");
		NO *s = (NO *)malloc(sizeof(NO));
		arvore->raiz = s;
		s->folha = false;
		s->numChaves = 0;
		s->filhos[0] = raiz;
		divideNo(s, 0, raiz);

		insereNoNaoCheio(s, chave);
	}
	else
	{
		printf(" raiz vazia");
		insereNoNaoCheio(raiz, chave);
	}
	puts("");
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

void imprimeEmOrdem(NO *no)
{
	if (!no)
		return;
	printf("(");

	for (int i = 0; i <= no->numChaves; i++)
	{
		if (!no->folha)
		{
			imprimeEmOrdem(no->filhos[i]);
			if (i < no->numChaves)
				printf(" ");
		}
		if (i < no->numChaves)
		{
			printf("%d", no->chave[i]);
			if (i < no->numChaves - 1 || !no->folha)
				printf(" ");
		}
	}

	printf(")");
}

void imprimeDados(NO *no) {
	NO* p = no;
	while(!p->folha) p = p->filhos[0];

	while(p) {
		for(int i = 0; i < p->numChaves; i++)
			printf("%d ", p->chave[i]);
		p = p->filhos[p->numChaves];
	}
}

void executaComando(char comando, ArvB *arvore, FILE *arq_entrada)
{
	int chave;
	switch (comando)
	{
	case 'i':
		fscanf(arq_entrada, "%d", &chave);
		insere(arvore, chave);
		break;
	case 'r':
		fscanf(arq_entrada, "%d", &chave);

		// remove key
	case 'p':
		if (arvore->raiz->numChaves > 0) {
			imprimeEmOrdem(arvore->raiz);
			puts("");
		} else
			puts("Vazia");
		puts("");
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
	if (!criaArvoreB(arvore))
		return -1;

	while (comando != 'f')
	{
		if (comando != '\n')
			executaComando(comando, arvore, arq_entrada);

		comando = fgetc(arq_entrada);
	}

	return 1;
}