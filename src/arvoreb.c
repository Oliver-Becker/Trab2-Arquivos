/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <arvoreb.h>

#define POSICAO_NO_RAIZ 1
#define POSICAO_ALTURA POSICAO_NO_RAIZ + 4
#define POSICAO_ULTIMO_RRN POSICAO_ALTURA + 4

void CriaArvoreB() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "wb+");
	char status = 0;	// Status para indicar a consistência do arquivo.
	int noRaiz = -1;	// RRN do nó raíz.
	int altura = -1;	// Altura máxima da árvore.
	int ultimoRRN = -1;	// Último RRN usado por um nó da árvore.

	fwrite(&status, sizeof(status), 1, fp);
	fwrite(&noRaiz, sizeof(noRaiz), 1, fp);
	fwrite(&altura, sizeof(altura), 1, fp);
	fwrite(&ultimoRRN, sizeof(ultimoRRN), 1, fp);

	fclose(fp);
}

void AlteraRRNdaRaiz(int RRN) {
	if (RRN < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, POSICAO_NO_RAIZ, SEEK_SET);	// Vai para a posição do noRaiz no cabeçalho.

	fwrite(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);
}

int RRNdaRaiz() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, POSICAO_NO_RAIZ, SEEK_SET);	// Vai para a posição do noRaiz no cabeçalho.

	int RRN;
	fread(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);

	return RRN;
}

void AlteraAlturaDaArvore(int altura) {
	if (altura < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, POSICAO_ALTURA, SEEK_SET);	// Vai para a posição da altura no cabeçalho.

	fwrite(&altura, sizeof(altura), 1, fp);
	fclose(fp);
}

int AlturaDaArvore() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, POSICAO_ALTURA, SEEK_SET);	// Vai para a posição da altura no cabeçalho.

	int altura;
	fread(&altura, sizeof(altura), 1, fp);
	fclose(fp);

	return altura;
}

void AlteraUltimoRRN(int RRN) {
	if (RRN < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, POSICAO_ULTIMO_RRN, SEEK_SET);// Vai para a posição do ultimoRRN no cabeçalho.

	fwrite(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);
}

int UltimoRRN() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, POSICAO_ULTIMO_RRN, SEEK_SET);// Vai para a posição do ultimoRRN no cabeçalho.

	int RRN;
	fread(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);

	return RRN;
}

REGISTRO_ARVORE* CriaRegistroArvore() {
	REGISTRO_ARVORE* registro = (REGISTRO_ARVORE*)malloc(sizeof(REGISTRO_ARVORE));
	registro->quantidadeChaves = 0;
	memset(registro->ponteiroSubarvore, -1, sizeof(int) * ORDEM_DA_ARVORE);
	memset(registro->chaveBusca, -1, sizeof(int) * (ORDEM_DA_ARVORE - 1));
	memset(registro->ponteiroDados, -1, sizeof(int) * (ORDEM_DA_ARVORE - 1));

	return registro;
}

void InsereRegistroArvore(REGISTRO_ARVORE* registro, int RRN) {
	if (registro == NULL || RRN < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, BYTE_OFFSET_ARVORE(RRN), SEEK_SET);

	fwrite(&registro->quantidadeChaves, sizeof(int), 1, fp);
	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){
		fwrite(&registro->ponteiroSubarvore[i], sizeof(int), 1, fp);
		fwrite(&registro->chaveBusca[i], sizeof(int), 1, fp);
		fwrite(&registro->ponteiroDados[i], sizeof(int), 1, fp);
	}

	fwrite(&registro->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, fp);
	
	fclose(fp);
}

REGISTRO_ARVORE* LeRegistroArvore(int RRN) {
	FILE *fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return NULL;

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

int InsereIndice(int chaveBusca, int RRNRegistroDeDados) {

/*	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -1;
*/
	REGISTRO_ARVORE* registro = NULL;
	int raiz = RRNdaRaiz();

	if (raiz == -2) {	// Caso a função retorne erro.
//		fclose(fp);
		return -1;
	}
	if (raiz < 0) {		// Caso ainda não haja nenhum registro de índice no arquivo.
		registro = CriaRegistroArvore();

		// Muda o cabeçalho, já que será inserido o primeiro registro de índice.
		AlteraRRNdaRaiz(0);
		AlteraUltimoRRN(0);
		AlteraAlturaDaArvore(0);

		// Insere o índice no registro (como é um nó folha, não terá ponteiros de filho).
		registro->quantidadeChaves = 1;
		registro->chaveBusca[0] = chaveBusca;
		registro->ponteiroDados[0] = RRNRegistroDeDados;

		// Por fim, escreve o novo registro no arquivo de índice.
		InsereRegistroArvore(registro, 0);
		
		free(registro);
		return 0;	// Finaliza a inserção, sem erros.
	}

	//registro = LeRegistroArvore(raiz);




	return 0;
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
	return BuscaArvoreB(LeRegistroArvore(filho), chave);
}
