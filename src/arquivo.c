/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <arquivo.h>

void AlteraTopoDaPilha(char* nomeArquivo, int topoPilha) {
	if (nomeArquivo == NULL || topoPilha < -1)
		return;

	FILE* fp = fopen(nomeArquivo, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, 1, SEEK_SET);		// Pula o char do status para ler o topoPilha

	//faz a alteração do topo da pilha
	fwrite(&topoPilha, sizeof(topoPilha), 1, fp);
	fclose(fp);
}

//retorna o topo da pilha atual
int TopoDaPilha(char* nomeArquivo) {
	//verifica se o nome do arquivo é válidp
	if (nomeArquivo == NULL)
		return -2;

	//verifica se o arquivo foi aberto corretamente
	FILE* fp = fopen(nomeArquivo, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, 1, SEEK_SET);		// Pula o char do status para ler o topoPilha

	//faz a leitura do topo da pilha
	int topoPilha;
	fread(&topoPilha, sizeof(topoPilha), 1, fp);
	fclose(fp);

	return topoPilha;
}

void AlteraStatusDoArquivo(char* nomeArquivo, char status) {
	//confere se o nome do arquivo e se o valor do status são válido 
	if (nomeArquivo == NULL || (status != 0 && status != 1))
		return;

	FILE* fp = fopen(nomeArquivo, "rb+");
	if (fp == NULL)
		return;

	//escreve o novo valor do status do arquivo
	fwrite(&status, sizeof(status), 1, fp);
	fclose(fp);
}

//verifica qual é a consistência do arquivo e retorna tal valor
int ConfereConsistenciaDoArquivo(char* nomeArquivo) {
	
	if (nomeArquivo == NULL)
		return 0;

	FILE* fp = fopen(nomeArquivo, "rb");
	if (fp == NULL)
		return 0;

	char status;
	fread(&status, sizeof(status), 1, fp);
	fclose(fp);

	return status;
}

// Função para criar um novo arquivo de saída, já inserindo o cabeçalho.
void CriaArquivoDeSaida(char* nomeArquivo) {
	if (nomeArquivo == NULL)
		return;

	FILE* fp = fopen(nomeArquivo, "wb+");
	char status = 0;	// Status para indicar a consistência do arquivo de dados.
	int topoPilha = -1;	// Armazena o RRN do último registro logicamente removido.
	fwrite(&status, sizeof(status), 1, fp);
	fwrite(&topoPilha, sizeof(topoPilha), 1, fp);

	fclose(fp);
}

