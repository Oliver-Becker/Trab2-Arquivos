/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#ifndef _ARVORE_B_H_
#define _ARVORE_B_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CABECALHO_ARVORE 13
#define TAMANHO_PAGINA 116
#define BYTE_OFFSET_ARVORE(RRN) (TAMANHO_PAGINA * (RRN)) + CABECALHO_ARVORE
#define ORDEM_DA_ARVORE 10 				// Ordem 'm' = 10.
#define ARQUIVO_ARVORE "indice.bin"

typedef struct {
	int quantidadeChaves;				// 'n'
	int ponteiroSubarvore[ORDEM_DA_ARVORE+1];	// 'P'  Foi adicionada uma posição a mais em
	int chaveBusca[ORDEM_DA_ARVORE];		// 'C'  cada vetor apenas para a implementa-
	int ponteiroDados[ORDEM_DA_ARVORE];		// 'Pr' ção do split, mas estas nunca serão
} REGISTRO_ARVORE;					//	mostradas no arquivo de índices.

void CriaArvoreB();
void AlteraRRNdaRaiz(int RRN);
int RRNdaRaiz();
void AlteraAlturaDaArvore(int altura);
int AlturaDaArvore();
void AlteraUltimoRRN(int RRN);
int UltimoRRN();
int InsereIndice(int chave, int RRN);
REGISTRO_ARVORE* LeRegistroArvore(int);
int BuscaArvoreB(REGISTRO_ARVORE*, int);
void ImprimeRegistroArvore(REGISTRO_ARVORE*);
void ImprimeArquivoArvoreB();

#endif
