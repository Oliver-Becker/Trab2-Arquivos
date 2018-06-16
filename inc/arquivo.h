/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Rafael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#ifndef _ARQUIVO_H_
#define _ARQUIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AlteraTopoDaPilha(char* nomeArquivo, int topoPilha);
int TopoDaPilha(char* nomeArquivo);
void AlteraStatusDoArquivo(char* nomeArquivo, char status);
int ConfereConsistenciaDoArquivo(char* nomeArquivo);
void CriaArquivoDeSaida(char* nomeArquivo);

#endif
