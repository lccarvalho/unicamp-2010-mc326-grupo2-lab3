/******************************************************************************/
/* Grupo 2:                                                                   */
/*          Felipe Augusto da Silva    RA 096993                              */
/*          Lucas Barbi Rezende        RA 097042                              */
/*          Luiz Claudio Carvalho      RA 800578                              */
/*                                                                            */
/* MC236EF  1o semestre 2010                                           UNICAMP*/
/* Laboratório    03 
      - Processamento Co-Sequencial de arquivos
      - Implementação de Sort-Merge
   
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envelope.h"

#define tamPrimCampoHd 20
#define tamUltCampoHd 40


/* Definição de tipo booleano */
typedef enum {false, true} Boolean;


/* Definição de um vetor de ponteiros para caractere. Uma vez inicializados,
   cada um dos ponteiros do vetor irá apontar para a posição inicial do
   conteúdo do campo[i] do registro correspondente. Caso seja necessário
   carregar mais de um registro do banco de dados na memória, cada um de seus 
   campos poderá ser acessado por: registro[m].campo[n]   */
typedef char** Record;


/* Definição de estrutura que armazena as características de um campo de um
   arquivo de dados. Estas características devem ser extraídas de um arquivo de
   configuração ou de uma seção específica do arquivo de dados */
typedef struct {
        char nome[tamPrimCampoHd+1];
        char tipo;
        int inicio;
        int tamanho;
        Boolean obrig;
        char msg[tamUltCampoHd+1];
        } Header;



typedef struct Ordena {
  Record  reg;
  int  index;
  struct Ordena *prox;
} Ordena, *RecSM;




void AbreArquivoFixo(char* nome, FILE** arqIn, FILE** arqCfg);
/* Abre o arquivo de entrada e seu respectivo arquivo de configuração (.cfg) */ 
  
void CarregaHeader(Header** h, int* numcampos, FILE* arqCfg);
/* Carrega o vetor head com os campos do banco de dados definido por arqCfg */
  
int LeChaveOrdena(char* nomecampo, Header* head, int numcampos);  
/* Retorna o índice do campo chave de ordenação para uso em Record.
   Retorna -1 se não houver tal campo nas configurações do arquivo de dados */

int ValidaTamMem(char* arg, int tamreg);
/* Verifica se arg representa um inteiro != 0 e retorna seu valor dividido
   por tam. Retorna -1 em caso de erro. */

int NumRegs(FILE* arq, int tamreg);
/*  Retorna o numero total de registros de tamanho 'tamreg' em 'arq' */

FILE** CriaCorrida(FILE* arq, int maxreg, int tamreg, int key, Header* h, int numcampos, int* n, int totalregs, int* nread, int* nwrite);
/* Retorna um vetor de arquivos (corrida) cada um deles com max registros de arq,
   de tamanho tam, ordenados por key. Coloca em n o número de arquivos da 
   corrida */

void ImprimeRegFixo(Record rec, FILE* arq, int numcampos);
/* Grava, na posição corrente em arq, os dados de rec.                        */

void OrdenaRegistros(Record** rec, int i, int key);
/* Ordena um vetor de registros com i elementos, usando o campo indicado por
   key como chave de ordenação                                                */

FILE* SortMerge(FILE** ppFile, int inf, int sup, int max, Header h, int key);
/* Invoca CriaOrdNulo() para inicializar um vetor de estruturas de ordenação 
      (RecSM). Utiliza para isso os dados de h para alocar a memória necessária.
   Carrega este vetor com os registros de topo dos 'max' 1o.s arquivos
      a partir de ppFile[inf], inserindo os elementos na ordem determinada
      pela chave de ordenação
   Cria um novo arquivo temporário e invoca CospePuxa() para inserir neste o 'menor'
      dos registros do vetor de ordenação neste arquivo, puxar o próximo registro
      do arquivo de onde ele veio, inseri-lo na estrutura de ordenação na posição
      correta e cuspir o próximo para o arquivo temporário, até que os 'max' arquivos
      tenham sido esgotados. Utiliza uma variável local (acabou), inicialmente 
      igual a max, decrementando esta variável cada vez que encontrar um eof e parando
      quando chegar a zero.
   Acabando de tratar estes 'max' arquivos, a função apaga os 'max' arquivos 
      tratados. Altera os índices de ppFile para que os arquivos ainda não tratados
      ocupem as posições 'inf+1', 'inf+2', etc até o fim. Os índices que não 
      estão mais em uso devem passar a apontar para NULL.
   Se o novo 'sup' (índice do último arquivo válido de ppFile) for menor que o 
      antigo 'inf' é porque esta corrida acabou. Se for este o caso, chama a si 
      mesma com inf = 0. Senão chama com inf = 1 para acabar a corrida.
*/














