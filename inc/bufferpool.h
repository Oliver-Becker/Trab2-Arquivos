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
	int RRN;
	int flag;
	REGISTRO_ARVORE *pagina;
}PAGINA;

typedef struct{
	int pageFault;
	int pageHit;
	PAGINA indice[5];
}BUFFER_POOL;

BUFFER_POOL* CriaBufferPool();
int ScaBufferPool(BUFFER_POOL*);
void InsercaoBufferPool(BUFFER_POOL*, int);
void RemocaoBufferPool(BUFFER_POOL*, int);
REGISTRO_ARVORE* GetBufferPool(BUFFER_POOL*, int);
void InsereArquivoBuffer(BUFFER_POOL*);


#endif
