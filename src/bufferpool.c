/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.

	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <bufferpool.h>


BUFFER_POOL* CriaBufferPool() {
	BUFFER_POOL *buffer = (BUFFER_POOL*) malloc(sizeof(BUFFER_POOL)); 

	buffer->pageHit = 0;
	buffer->pageFault = 0;

	for(int i = 0; i < TAMANHO_BUFFER; i++) {
		buffer->pagina[i].RRN = -1;
		buffer->pagina[i].flag = 0;
		buffer->pagina[i].consistencia = 1;
	}

	return buffer;
}

void DestroiBufferPool(BUFFER_POOL* buffer) {
	if (buffer == NULL)
		return;

	AtualizaRegistrosInconsistentes(buffer);

	free(buffer);
}

// Copia o conteúdo de um registro para outro.
void CopiaRegistro(REGISTRO_ARVORE* origem, REGISTRO_ARVORE* destino) {
	destino->quantidadeChaves = origem->quantidadeChaves;

	int i;
	for(i = 0; i < ORDEM_DA_ARVORE-1; i++){
		destino->ponteiroSubarvore[i] = origem->ponteiroSubarvore[i];
		destino->chaveBusca[i] = origem->chaveBusca[i];
		destino->ponteiroDados[i] = origem->ponteiroDados[i];
	}
	destino->ponteiroDados[i] = origem->ponteiroDados[i];
}

// Recupera o registro especificado pelo RRN do buffer pool. Caso não esteja no buffer, busca o re-
// gistro no arquivo, insere no buffer e o retorna.
REGISTRO_ARVORE* LeRegistroBufferPool(BUFFER_POOL *buffer, int RRN) {
	REGISTRO_ARVORE* registro;

	int i;
	for(i = 1; i < TAMANHO_BUFFER; i++) {	// Procura o registro nas posições do buffer.
		if(buffer->pagina[i].RRN == RRN) { // Se encontrado, retorna o registro do buffer.
			buffer->pageHit++;	// Encontrou o registro, aumenta o page hit.
			registro = (REGISTRO_ARVORE*) malloc(sizeof(REGISTRO_ARVORE));
			CopiaRegistro(&(buffer->pagina[i].registro), registro);
			return registro;
		}
	}

	buffer->pageFault++;	// Não encontrou o registro, aumenta o page fault.

	// Caso o registro não esteja no buffer, lê do arquivo.
	registro = LeRegistroArvore(RRN);
	InsereRegistroBufferPool(buffer, RRN, registro); // Insere o novo registro no buffer.

	return registro;
}

// Escreve o registro da posição 'numeroRegistro' no arquivo de índices para que este fique consis-
// tente com o buffer. Deve ser conferida a consistencia do registro antes de chamar a função.
void AtualizaRegistroInconsistente(BUFFER_POOL* buffer, int numeroRegistro) {
	if (buffer == NULL || numeroRegistro < 0 || numeroRegistro >= TAMANHO_BUFFER)
		return;

	// Escreve o registro no arquivo de índices.
	InsereRegistroArvore( &(buffer->pagina[numeroRegistro].registro),
				 buffer->pagina[numeroRegistro].RRN);

	buffer->pagina[numeroRegistro].consistencia = 1; // Atualiza a consistência do registro.
}

void CopiaPagina(PAGINA* origem, PAGINA* destino) {
	destino->RRN = origem->RRN;
	destino->flag = origem->flag;
	destino->consistencia = origem->consistencia;
	CopiaRegistro(&(origem->registro), &(destino->registro));
}
// Função para rodar a política de substituição do buffer (SCA) e retorna o número da página a ser
// substituida.
void SecondChanceAlgorithm(BUFFER_POOL *buffer) {
	int i = 0;
	while(buffer->pagina[i].flag == 1 || buffer->localRaiz == i) { // Se for a raiz, continua
		buffer->pagina[i].flag = 0;	// para o proximo registro, pois ela deve sempre
		i = (i + 1) % TAMANHO_BUFFER;	// permanecer no buffer pool.
	}

	if (buffer->pagina[i].consistencia == 0)	// Se o registro estiver inconsistente
		AtualizaRegistroInconsistente(buffer, i);	// atualiza o conteudo no arquivo.

	// Realiza um shift de página para a esquerda, de modo a deixar a última página livre para
	// inserir um novo elemento.
	for (; i < TAMANHO_BUFFER-1; ++i) {
		CopiaPagina(&(buffer->pagina[i+1]), &(buffer->pagina[i]));
		if (buffer->localRaiz == (i+1))	// Caso a raiz tenha sofrido shift, muda a variável
			buffer->localRaiz = i;	// de referencia.
	}
}

// Insere o registro no buffer pool. Caso já esteja no buffer, atualiza seu conteúdo e marca-o como
// inconsistente. Se não estiver, insere no local
void InsereRegistroBufferPool(BUFFER_POOL *buffer, int RRN, REGISTRO_ARVORE* registro) {
	int i;
	for(i = 0; i < TAMANHO_BUFFER; i++) {	// Percorre os registros do buffer;
		if(buffer->pagina[i].RRN == RRN) {	// Caso o registro já esteja no buffer.
			// Copia o registro atualizado para o buffer.
			CopiaRegistro(registro, &(buffer->pagina[i].registro));
			buffer->pagina[i].consistencia = 0;	// Marca-o como inconsistente.
			buffer->pagina[i].flag = 1;	// Marca que foi visitado novamente.
			return;
		}
		if(buffer->pagina[i].RRN == -1) // Confere se tem espaço livre no buffer.
			break;
	}

	int RRNraiz = RRNdaRaiz();

	// Caso não haja espaço livre, roda o algorítmo SCA para selecionar qual registro irá sair.
	if(i == TAMANHO_BUFFER) {
		if(RRNraiz == RRN) {	// Caso o registro a ser inserido seja a raíz, altera a re-
			buffer->localRaiz = -1;	// ferencia da raiz do buffer para um valor invalido,
		}				// pois o buffer já não contém mais a raíz da árvore.

		SecondChanceAlgorithm(buffer);	// Chama o algorítimo da política de substituição.
		i--;
	}

	// Insere o registro no buffer.
	buffer->pagina[i].RRN = RRN;
	buffer->pagina[i].flag = 0;
	buffer->pagina[i].consistencia = 1;
	CopiaRegistro(registro, &(buffer->pagina[i].registro));

	if(RRNraiz == RRN) // Atualiza a referência da raíz, caso preciso.
		buffer->localRaiz = RRN;
}

void AtualizaRegistrosInconsistentes(BUFFER_POOL *buffer) {
	for(int i = 0; i < TAMANHO_BUFFER; i++) {
		if (buffer->pagina[i].consistencia == 0)
			AtualizaRegistroInconsistente(buffer, i);
	}
}

void InsereArquivoBuffer(BUFFER_POOL *buffer) {

	FILE *fp = fopen(ARQUIVO_BUFFER, "a");

	fprintf(fp, "Page fault: %d; Page hit: %d.\n", buffer->pageFault, buffer->pageHit);

	fclose(fp);
}
