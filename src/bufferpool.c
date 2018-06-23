/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <bufferpool.h>


BUFFER_POOL* CriaBufferPool(){

	BUFFER_POOL *buffer = (BUFFER_POOL*) malloc(sizeof(BUFFER_POOL)); 

	for(int i = 0; i < TAMANHO_BUFFER; i++){
		buffer->indice[i].RRN = -1;
		buffer->indice[i].flag = 0;
	}

	buffer->pageHit = 0;
	buffer->pageFault = 0;;

	return buffer;
}

int ScaBufferPool(BUFFER_POOL *buffer){

	int i = 1;
	while(buffer->indice[i].flag){

		if(buffer->indice[i].flag)
			buffer->indice[i].flag = 0;

		i++;

		if(i == TAMANHO_BUFFER-1)
			i = 1;
	}

	return i;
}

void InsercaoBufferPool(BUFFER_POOL *buffer, int RRN){

	if(buffer->indice[0].RRN == -1){

		buffer->indice[0].pagina = (REGISTRO_ARVORE*) malloc(sizeof(REGISTRO_ARVORE));
		buffer->indice[0].pagina = LeRegistroArvore(RRN);
		return;
	}
	else if(RRNdaRaiz() != buffer->indice[0].RRN){
		RemocaoBufferPool(buffer, buffer->indice[0].RRN);
		buffer->indice[0].pagina = LeRegistroArvore(RRNdaRaiz());
		return;
	}	

	int i;
	for(i = 1; i < TAMANHO_BUFFER; i++)
		if(buffer->indice[i].RRN == -1)
			break;

	if(i == TAMANHO_BUFFER)
		i = ScaBufferPool(buffer);


	buffer->indice[i].pagina = (REGISTRO_ARVORE*) malloc(sizeof(REGISTRO_ARVORE));
	buffer->indice[i].pagina = LeRegistroArvore(RRN);

	return;
}

void RemocaoBufferPool(BUFFER_POOL *buffer, int RRN){

	//funcionalidade14(RRN)

	int i;
	for(i = 1; i < TAMANHO_BUFFER; i++)
		if(buffer->indice[i].RRN == RRN)
			break;

	buffer->indice[i].RRN = -1;
	buffer->indice[i].flag = 0;
	free(buffer->indice[i].pagina);

	return;
}

REGISTRO_ARVORE* GetBufferPool(BUFFER_POOL *buffer, int RRN){

	int i;
	for(i = 1; i < TAMANHO_BUFFER; i++){
		if(buffer->indice[i].RRN == RRN){
			buffer->pageHit++;
			break;
		}
	}

	if(i == TAMANHO_BUFFER){
		buffer->pageFault++;
		return NULL;
	}

	return buffer->indice[i].pagina;

}

void InsereArquivoBuffer(BUFFER_POOL *buffer){

	FILE *fp = fopen(ARQUIVO_BUFFER, "a");

	char pageFault[20] = "Page fault = ";
	char pageHit[20] = " Page hit = ";
	char n[2] = "\n";

	fwrite(&pageFault, sizeof(pageFault), 1, fp);
	fwrite(&buffer->pageFault, sizeof(int), 1, fp);
	fwrite(&pageHit, sizeof(pageHit), 1, fp);
	fwrite(&buffer->pageHit, sizeof(int), 1, fp);
	fwrite(&n, sizeof(int), 1, fp);

	fclose(fp);

}
