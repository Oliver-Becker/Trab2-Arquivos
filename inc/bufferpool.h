/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#ifndef _BUFFERPOOL_H_
#define _BUFFERPOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arvoreb.h>

#define TAMANHO_BUFFER 5
#define ARQUIVO_BUFFER "buffer-info.text"

typedef struct {
	int RRN;  //RRN do registro
	int flag; //flag 0 é usado apenas uma vez e flag 1 é usado duas vezes
	int consistencia; //0 indica que o registro foi modificado e deve ser escrito no arquivo
	REGISTRO_ARVORE registro;
}PAGINA;

typedef struct{
	int pageFault; //flahas do buffer pool
	int pageHit; //acertos do buffer pool
	int localRaiz;	//número da pagina em que está a raiz
	PAGINA pagina[5]; //buffer pool com espaço para 5 registros
}BUFFER_POOL;

BUFFER_POOL* CriaBufferPool();
void DestroiBufferPool(BUFFER_POOL*);

REGISTRO_ARVORE* LeRegistroBufferPool(BUFFER_POOL*, int RRN);	// Função Get(RRN)
void InsereRegistroBufferPool(BUFFER_POOL*, int RRN, REGISTRO_ARVORE*);	// Função Put(RRN, page)
void AtualizaRegistrosInconsistentes(BUFFER_POOL*);	// Função Flush()

void InsereArquivoBuffer(BUFFER_POOL*);	// Escreve o total de Page Hit e Page Fault no arquivo

#endif
