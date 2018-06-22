/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <registro.h>

// Função para inserir strings com o campo em um novo registro
REGISTRO* InsereCamposEmRegistro(char* campo[]) {
	REGISTRO* registro = (REGISTRO*)malloc(sizeof(REGISTRO));

	registro->codEscola = atoi(campo[0]);
	strcpy(registro->dataInicio, (strcmp(campo[1], "0") ? campo[1] : DATA_VAZIA));
	strcpy(registro->dataFinal, (strcmp(campo[2], "0") ? campo[2] : DATA_VAZIA));

	registro->tamNome = strlen(campo[3]);
	if (registro->tamNome > 0) {
		registro->nomeEscola = (char*) malloc(sizeof(char) * registro->tamNome);
		strcpy(registro->nomeEscola, campo[3]);
	} else {
		registro->nomeEscola = CAMPO_VAZIO;
	}

	registro->tamMunicipio = strlen(campo[4]);
	if (registro->tamMunicipio > 0) {
		registro->municipio = (char*) malloc(sizeof(char) * registro->tamMunicipio);
		strcpy(registro->municipio, campo[4]);
	} else {
		registro->municipio = CAMPO_VAZIO;
	}

	registro->tamEndereco = strlen(campo[5]);
	if (registro->tamEndereco > 0) {
		registro->endereco = (char*) malloc(sizeof(char) * registro->tamEndereco);
		strcpy(registro->endereco, campo[5]);
	} else {
		registro->endereco = CAMPO_VAZIO;
	}

	return registro;
}

int AcrescentaRegistroNoFinal(char* nomeArquivo, REGISTRO* registro, int indice) {
	if (nomeArquivo == NULL || registro == NULL)
		return 0;

	FILE* fp = fopen(nomeArquivo, "ab+");
	if (fp == NULL)
		return 0;

	fseek(fp, 0, SEEK_END);
	printf("ftell: %d, RRN_ATUAL: %d\n", ftell(fp), RRN_ATUAL(ftell(fp)));

	int RRN = RRN_ATUAL(ftell(fp));	// Descobre o RRN do registro com base no byte offset atual.

	int registroExiste = 0;

	fwrite(&registroExiste, sizeof(int), 1, fp);
	fwrite(&(registro->codEscola), sizeof(int), 1, fp);
	fwrite(registro->dataInicio, sizeof(char), strlen(registro->dataInicio), fp);
	fwrite(registro->dataFinal, sizeof(char), strlen(registro->dataFinal), fp);
	fwrite(&registro->tamNome, sizeof(int), 1, fp);
	fwrite(registro->nomeEscola, sizeof(char), registro->tamNome, fp);
	fwrite(&registro->tamMunicipio, sizeof(int), 1, fp);
	fwrite(registro->municipio, sizeof(char), registro->tamMunicipio, fp);
	fwrite(&registro->tamEndereco, sizeof(int), 1, fp);
	fwrite(registro->endereco, sizeof(char), registro->tamEndereco, fp);

	int bytesRestantes = TAMANHO_REGISTRO;
	bytesRestantes -= 36 + registro->tamNome + registro->tamEndereco + registro->tamMunicipio;

	char* finalDoRegistro = (char*)malloc(sizeof(char) * bytesRestantes);
	fwrite(finalDoRegistro, sizeof(char), bytesRestantes, fp);

	fclose(fp);

	int retorno = 1;

	if (indice) {
		AlteraStatusDoArquivo(ARQUIVO_ARVORE, 0);
		int retorno = InsereIndice(registro->codEscola, RRN);
		AlteraStatusDoArquivo(ARQUIVO_ARVORE, 1);
	}

	free(finalDoRegistro);

	return retorno;
}

int ComparaCampoDoRegistro(REGISTRO* registro, char* nomeDoCampo, char* valor) {
	if (registro == NULL || nomeDoCampo == NULL || valor == NULL) {
		return 0;
	}

	if (!strcmp(nomeDoCampo, "codEscola"))
		return registro->codEscola == atoi(valor);
	else if (!strcmp(nomeDoCampo, "dataInicio"))
		return !strcmp(registro->dataInicio, valor);
	else if (!strcmp(nomeDoCampo, "dataFinal"))
		return !strcmp(registro->dataFinal, valor);
	else if (!strcmp(nomeDoCampo, "nomeEscola"))
		return !strcmp(registro->nomeEscola, valor);
	else if (!strcmp(nomeDoCampo, "municipio"))
		return !strcmp(registro->municipio, valor);
	else if (!strcmp(nomeDoCampo, "endereco"))
		return !strcmp(registro->endereco, valor);
	
	return 0;
}

void LiberaRegistro(REGISTRO* registro) {
	if (registro == NULL)
		return;

	if (registro->nomeEscola != NULL && strcmp(registro->nomeEscola, CAMPO_VAZIO))
		free(registro->nomeEscola);
	if (registro->municipio != NULL && strcmp(registro->municipio, CAMPO_VAZIO))
		free(registro->municipio);
	if (registro->endereco != NULL && strcmp(registro->endereco, CAMPO_VAZIO))
		free(registro->endereco);

	free(registro);
	registro = NULL;
}

int ImprimeRegistro(REGISTRO* registro) {
	if (registro == NULL)
		return -1;

	REGISTRO* r = registro;

	printf("%1$d %2$s %3$s %4$d%10$.*4$s%5$s %6$d%10$.*6$s%7$s %8$d %9$s\n", r->codEscola,
		r->dataInicio, r->dataFinal, r->tamNome, r->nomeEscola, r->tamMunicipio,
		r->municipio, r->tamEndereco, r->endereco, " ");

	return 1;
}
