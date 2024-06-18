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
		novo->numChaves++;

	for (int j = 0; j < novo->numChaves; j++)
	{
		int q = j + t;
		if (novo->folha)
			q -= 1;
		novo->chave[j] = no->chave[q];
	}

	if (!no->folha)
		for (int j = 0; j < t; j++)
			novo->filhos[j] = no->filhos[j + t];
	else
	{
		novo->filhos[novo->numChaves] = no->filhos[no->numChaves];
		no->filhos[no->numChaves] = NULL;
		no->filhos[t - 1] = novo;
	}

	no->numChaves = t - 1;

	for (int j = pai->numChaves; j > i; j--)
		pai->filhos[j + 1] = pai->filhos[j];

	pai->filhos[i + 1] = novo;

	for (int j = pai->numChaves - 1; j >= i; j--)
		pai->chave[j + 1] = pai->chave[j];

	pai->chave[i] = no->chave[t - 1];
	pai->numChaves++;
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
		no->numChaves++;
		no->filhos[no->numChaves] = no->filhos[no->numChaves - 1];
		no->filhos[no->numChaves - 1] = NULL;
	}
	else
	{
		while (i >= 0 && chave < no->chave[i])
		{
			i -= 1;
		}

		i++;
		verificaNoCheio(no->filhos[i]);
		if (verificaNoCheio(no->filhos[i]))
		{
			divideNo(no, i, no->filhos[i]);
			if (chave > no->chave[i])
			{
				i++;
			}
		}

		insereNoNaoCheio(no->filhos[i], chave);
	}
}

void insere(ArvB *arvore, int chave)
{
	NO *raiz = arvore->raiz;
	if (verificaNoCheio(raiz))
	{
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
		insereNoNaoCheio(raiz, chave);
	}
}

int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

int maxSubarvore(NO *no, NO **x)
{
	NO *p = no;
	while (!p->folha)
	{
		p = p->filhos[p->numChaves];
	}

	*x = p;

	return p->chave[p->numChaves - 1];
}

int minSubarvore(NO *no)
{
	NO *p = no;

	while (!p->folha)
	{
		p = p->filhos[0];
	}

	return p->chave[1];
}

int buscaSeq(NO *no, int chave)
{
	int i = 0;

	while (no->chave[i] != chave)
	{
		i++;
		if (i == no->numChaves)
			return -1;
	}

	return i;
}

void mesclaNos(NO *y, NO *z, NO *pai, int i)
{
	if (!y->folha)
	{
		int k = pai->chave[i];
		y->chave[y->numChaves] = k;
		y->numChaves++;
	}

	int n = y->numChaves;

	for (int j = 0; j < z->numChaves; j++)
	{
		y->chave[n + j] = z->chave[j];
		y->numChaves++;
	}

	for (int j = 0; j <= z->numChaves; j++)
	{
		y->filhos[n + j] = z->filhos[j];
	}

	for (int j = i; j < pai->numChaves - 1; j++)
	{
		pai->chave[j] = pai->chave[j + 1];
	}

	for (int j = i + 1; j < pai->numChaves; j++)
	{
		pai->filhos[j] = pai->filhos[j + 1];
	}

	pai->numChaves--;

	free(z);
}

void removeDoNo(NO *no, int chave)
{
	// NO FOLHA
	int i = buscaSeq(no, chave);
	for (int j = i; j < no->numChaves - 1; j++)
		no->chave[j] = no->chave[j + 1];

	no->filhos[no->numChaves - 1] = no->filhos[no->numChaves];
	no->filhos[no->numChaves] = NULL;

	no->numChaves--;
}

void removeDaSubarvore(NO *no, int chave)
{
	if (no->folha)
	{
		removeDoNo(no, chave);
		return;
	}

	int i = buscaSeq(no, chave);
	if (i != -1)
	{
		// CASO 2
		
		NO *y = no->filhos[i];
		NO *z = no->filhos[i + 1];
		if (y->numChaves >= t)
		{
			// 2a
			NO *x;
			int pred = maxSubarvore(y, &x);

			NO *prox = x->filhos[x->numChaves];

			for (int j = prox->numChaves; j > 0; j--)
				prox->chave[j] = prox->chave[j - 1];

			// prox eh folha
			prox->filhos[prox->numChaves + 1] = prox->filhos[prox->numChaves];
			prox->filhos[prox->numChaves] = NULL;

			prox->chave[0] = pred;
			prox->numChaves++;

			removeDaSubarvore(y, pred);

			no->chave[i] = pred;

			removeDaSubarvore(z, chave);
		}
		else if (z->numChaves >= t)
		{
			// 2b
			int suc = minSubarvore(z);
			no->chave[i] = suc;
			removeDaSubarvore(z, chave);
		}
		else
		{
			// 2c
			mesclaNos(y, z, no, i);
			removeDaSubarvore(y, chave);
		}
	}
	else
	{
		// CASO 3
		int j = no->numChaves - 1;
		while (j >= 0 && chave < no->chave[j])
			j -= 1;
		j++;

		NO *filho = no->filhos[j];

		if (filho->numChaves == t - 1)
		{
			NO *irmaoEsq = j > 0 ? no->filhos[j - 1] : NULL;
			NO *irmaoDir = j < no->numChaves ? no->filhos[j + 1] : NULL;

			// 3a
			if (irmaoEsq && irmaoEsq->numChaves >= t)
			{
				int x = irmaoEsq->chave[irmaoEsq->numChaves - 1];
				int y = no->chave[j - 1];
				no->chave[j - 1] = x;

				for (int q = filho->numChaves; q > 0; q--)
				{
					filho->chave[q] = filho->chave[q - 1];
				}

				for (int q = filho->numChaves + 1; q > 0; q--)
				{
					filho->filhos[q] = filho->filhos[q - 1];
				}

				if (!filho->folha)
				{
					filho->chave[0] = y;
					filho->filhos[0] = irmaoEsq->filhos[irmaoEsq->numChaves];
				}
				else {
					filho->chave[0] = x;
					irmaoEsq->filhos[irmaoEsq->numChaves - 1] = irmaoEsq->filhos[irmaoEsq->numChaves];
				}

				irmaoEsq->numChaves--;
				filho->numChaves++;
			}
			else if (irmaoDir && irmaoDir->numChaves >= t)
			{
				int x = irmaoDir->chave[0];
				int y = no->chave[j];
				no->chave[j] = x;

				if (!filho->folha)
				{
					filho->chave[filho->numChaves] = y;
					filho->filhos[filho->numChaves + 1] = irmaoDir->filhos[0];
				}
				else
				{
					filho->chave[filho->numChaves] = x;
					filho->filhos[filho->numChaves + 1] = filho->filhos[filho->numChaves];
					filho->filhos[filho->numChaves] = NULL;
				}

				for (int q = 0; q < irmaoDir->numChaves - 1; q++)
					irmaoDir->chave[q] = irmaoDir->chave[q + 1];

				for (int q = 0; q < irmaoDir->numChaves; q++)
					irmaoDir->filhos[q] = irmaoDir->filhos[q + 1];

				irmaoDir->numChaves--;
				filho->numChaves++;
			}

			// 3b
			else if (irmaoEsq)
			{
				mesclaNos(irmaoEsq, filho, no, j - 1);
				filho = irmaoEsq;
			}
			else if (irmaoDir)
			{
				mesclaNos(filho, irmaoDir, no, j);
			}
		}

		removeDaSubarvore(filho, chave);
	}
}

void removeDaArvore(ArvB *arvore, int chave)
{
	NO *raiz = arvore->raiz;

	if (raiz->numChaves == 0)
		return;

	removeDaSubarvore(raiz, chave);

	if (raiz->numChaves == 0 && !raiz->folha)
	{
		arvore->raiz = raiz->filhos[0];
		free(raiz);
	}
}

bool criaArvoreB(ArvB *arvore)
{
	NO *x;

	if (!(x = (NO *)malloc(sizeof(NO))))
		return false;

	x->folha = true;
	x->numChaves = 0;
	for (int i = 0; i < 2 * t; i++)
	{
		x->filhos[i] = NULL;
	}
	arvore->raiz = x;
	return true;
}

void imprimeEmOrdem(NO *no, FILE *arq_saida)
{
	if (!no)
		return;
	fprintf(arq_saida, "(");

	for (int i = 0; i <= no->numChaves; i++)
	{
		if (!no->folha)
		{
			imprimeEmOrdem(no->filhos[i], arq_saida);
			if (i < no->numChaves)
			{
				fprintf(arq_saida, " ");
			}
		}
		if (i < no->numChaves)
		{
			fprintf(arq_saida, "%d", no->chave[i]);
			if (i < no->numChaves - 1 || !no->folha)
			{
				fprintf(arq_saida, " ");
			}
		}
	}

	fprintf(arq_saida, ")");
}

void imprimeDados(NO *no)
{
	if (!no || no->numChaves == 0)
		return;

	NO *p = no;
	while (!p->folha)
		p = p->filhos[0];
	
	while (p)
	{
		for (int i = 0; i < p->numChaves; i++)
			printf("%d ", p->chave[i]);
		p = p->filhos[p->numChaves];
	}
}

void executaComando(char comando, ArvB *arvore, FILE *arq_entrada, FILE *arq_saida)
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
		removeDaArvore(arvore, chave);
		break;
	case 'p':
		if (arvore->raiz->numChaves > 0)
		{
			imprimeEmOrdem(arvore->raiz, arq_saida);
			fprintf(arq_saida, "\n");
		}
		else
		{
			fprintf(arq_saida, "Vazia\n");
		}
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
	FILE *arq_saida = fopen(argv[2], "w");
	char comando = fgetc(arq_entrada);

	ArvB *arvore = (ArvB *)malloc(sizeof(ArvB));
	if (!criaArvoreB(arvore))
		return -1;

	while (comando != 'f')
	{
		if (comando != '\n')
			executaComando(comando, arvore, arq_entrada, arq_saida);

		comando = fgetc(arq_entrada);
	}

	fclose(arq_entrada);
	fclose(arq_saida);

	return 1;
}