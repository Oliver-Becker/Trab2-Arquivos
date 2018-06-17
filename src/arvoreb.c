/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <arvoreb.h>

int RRNdaRaiz(){

	FILE *fp = fopen(ARQUIVO_ARVORE, "rb");

	fseek(fp, 1, SEEK_SET);
	
	int raiz;
	fread(&raiz, sizeof(int), 1, fp);

	fclose(fp);

	return raiz;
}

REGISTRO_ARVORE* CriaStruct(int RRN){


	FILE *fp = fopen(ARQUIVO_ARVORE, "rb");

	fseek(fp, BYTE_OFFSET_ARVORE(RRN), SEEK_SET);

	REGISTRO_ARVORE *reg = (REGISTRO_ARVORE*) malloc(sizeof(REGISTRO_ARVORE));

	fread(&reg->quantidadeChaves, sizeof(int), 1, fp);

	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){

		fread(&reg->ponteiroSubarvore[i], sizeof(int), 1, fp);
		fread(&reg->chaveBusca[i], sizeof(int), 1, fp);
		fread(&reg->ponteiroDados[i], sizeof(int), 1, fp);
	}

	fread(&reg->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, fp);

	fclose(fp);
	
	return reg;
}

int BuscaArvoreB(REGISTRO_ARVORE *reg, int chave){

	int i = 0;
	
	while(i < reg->quantidadeChaves && chave > reg->chaveBusca[i])
		i++;

	if(chave == reg->chaveBusca[i]) {
		int RRN = reg->ponteiroDados[i];
		free(reg);
		return RRN;
	}

	if(reg->ponteiroSubarvore[0] == -1)
		return -1;

	int filho = reg->ponteiroSubarvore[i];
	free(reg);
	return BuscaArvoreB(CriaStruct(filho), chave);
}


