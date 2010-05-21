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


#include "SortMerge.h"

/**FUNÇÕES USADAS NA DEPURAÇÃO - REMOVER PARA ENTREGA**************************/
void ImprimeRegistro(Record registro, Header *head, int numcampos){
/* Imprime todos os campos de um registro */
   int i;
   
   for(i=0; i<numcampos-1; i++) {
       fprintf(stdout, "%s: ", head[i].nome);
       fprintf(stdout, "%s \n", registro[i]);
   }
   printf("\n");
   
} /* ImprimeRegistro */















/**Funções auxiliares**********************************************************/

void TiraBrancosDoFinal(char* s){
/* Elimina todos os brancos em excesso no final de uma string. */
    
    int i = strlen(s) - 1;     //Último elemento da string
    while(i >= 0 && s[i] == ' ')
       i--;
    s[i+1] = '\0';
    
} /* TiraBrancosDoFinal */




Boolean VerificaDigitos(char *string) {
/* Retorna "true" se a string só contém digitos de 0-9
   ou retorna "false" caso contrário */

    int i, tam = strlen(string);
    
    for(i = 0; i < tam; i++)
       if(!isdigit(string[i]))
          return false;
    
    return true;

} /* VerificaDigitos */




Record LeRegistroFixo(char* linha, int n, Header* h) {
/* Transforma 'linha' em um registro do tipo (Record) com n campos. */
   
   int i;
   Record registro;
   
   registro = Malloc(sizeof(char*)*n);
   
   for(i=0;i<n;i++){
                     
          registro[i] = (char*)Malloc(sizeof(char)*(h[i].tamanho+1));
          strncpy(registro[i], &linha[h[i].inicio-1], h[i].tamanho);
          registro[i][h[i].tamanho] = '\0';
   }
   
   return registro;
   
}/* LeRegistroFixo */




void LiberaRegistro(Record registro, int n){
/* Libera todas as strings apontadas por record e também os apontadores */
     
     int i;
     for(i = 0; i < n; i++)
        free(registro[i]);
     free(registro);
     
} /* LiberaRegistro */




void ImprimeRegFixo(Record rec, FILE* arq, int numcampos, int tamreg, Header* h){
/* Grava, na posição corrente em arq, os dados de rec.                        */

   int i;
   char *linha = Malloc(sizeof(char)*(tamreg+1));
   
   linha[0] = '\0';
   
   /* junta todos os campos do registros em um unico bloco de memoria
      para dar apenas um fwrite */
   
   for(i = 0; i < numcampos; i++) {
         
         strcat(linha, rec[i]);
         
         if((i < numcampos-1) && (h[i+1].inicio > h[i].inicio + h[i].tamanho)) //campos com um '' no final
             strcat(linha, " ");
         
   }
   
   strcat(linha, "\n");
   
   fwrite(linha, tamreg, 1, arq);
   
   free(linha);

} /* ImprimeRegFixo */




int particiona(int inicio, int fim, Record** rec, int key, Header* h, Record* registro) {
/* Função auxiliar do quicksort */

    int pivo,ultbaixo,temp,i;
    int j;
    int a,b;

    pivo = inicio;

    ultbaixo = inicio;
    

    for(i=inicio+1; i<=fim; i++) {
            

         j=0;
                             
         while((int)(toupper(rec[0][i][key][j]))==(int)(toupper(rec[0][pivo][key][j])))
            j++;     
            

         if ((int)(toupper(rec[0][i][key][j]))<=(int)(toupper(rec[0][pivo][key][j]))) {
            ultbaixo++;
            *registro = rec[0][i];
            rec[0][i] = rec[0][ultbaixo];
            rec[0][ultbaixo] = *registro;
         }
    }
    
    
    *registro = rec[0][inicio];
    
    rec[0][inicio] = rec[0][ultbaixo];
    
    rec[0][ultbaixo]= *registro;
 
    return(ultbaixo);
    
} /* particiona */




void quick(int inicio, int fim, Record** rec, int key, Header* h, Record* registro) {
/* Ordena os registros pelo metodo quicksort */

     int meio;
     
     if (inicio<fim) {
        meio = particiona(inicio, fim, rec, key, h, registro);
        quick(inicio, meio-1, rec, key, h, registro);
        quick(meio+1, fim, rec, key, h, registro);
     }
     
} /* quick */




void OrdenaRegistros(Record** rec, int i, int key, Header* h, int n, Record* registro ){
/* Ordena um vetor de registros com i elementos, usando o campo indicado por
   key como chave de ordenação */
   

        quick(0, i-1, rec, key, h, registro);  
        
} /* OrdenaRegistros */




RecSM CriaRecSMNulo(int n){
/* Cria uma cabeça para uma lista de estruturas RecSM          */

   int i;
   RecSM res;
   res = malloc(sizeof(Ordena));
   res->reg = malloc(sizeof(char*)*n);

   for(i=0; i<n; i++){
            (res->reg)[i] = malloc(sizeof(char)*2);
            strcpy((res->reg)[i], " ");
   }

   res->index = -1;
   res->prox = res;
   return res;

} /* CriaRecSMNulo */



void InsereSMAux(RecSM p, Record rec, int c) {
/* Insere o termo '(rec,c)' após o nó apontado por 'p'.                  */
   
   RecSM q = malloc(sizeof(Ordena));
   q->reg = rec;
   q->index = c;
   q->prox = p->prox;
   p->prox = q;
   
} /* InsereSMAux */




void InsereSM(RecSM lista, Record rec, int i, int key){
/* Recebe um registro e o índice do arquivo de onde ele provêm e o insere em 
   lista, em ordem crescente do campo de rec indicado por key  */

   RecSM q = lista, antes;

   do {
               antes = q;
               q = q->prox;
      } while(q != lista && strcmp((q->reg)[key], rec[key]) < 1);

   InsereSMAux(antes, rec, i);              //inserção na lista   

} /* InsereSM */




void RemoveSM(RecSM lista, RecSM noh, int n) {
/* Remove noh da posição posterior a lista em uma lista de RecSM, liberando
   a memória alocada para noh                                                 */     

   int i;
   lista->prox = noh->prox;
   for(i=0; i<n; i++) free((noh->reg)[i]);
   free(noh->reg);
   free(noh);
   
} /* RemoveSM */





void LiberaSM(RecSM noh, int n) {
/* Remove noh da posição posterior a lista em uma lista de RecSM, liberando
   a memória alocada para noh                                                 */     

   int i;
   for(i=0; i<n; i++) free((noh->reg)[i]);
   free(noh->reg);
   free(noh);
   
} /* RemoveSM */

/*******************************************************Funções auxiliares*****/



void AbreArquivoFixo(char* nome, FILE** arqIn, FILE** arqCfg){
/* Abre o arquivo de entrada e seu respectivo arquivo de configuração (.cfg) */   
   
   *arqIn = Fopen(nome, "r");    /* Abre arquivo de dados */
   
   /* abre arquivo de configuração (mesmo nome com extensão .cfg) */
   nome[strlen(nome)-3] = '\0';
   *arqCfg = Fopen(strcat(nome, "cfg"), "r");
   
} /* AbreArquivoFixo */
  
  
  
  
void CarregaHeader(Header** h, int* numcampos, FILE* arqCfg){
/* Carrega o vetor head com os campos do banco de dados definido por arqCfg */

    int i, final;
    char c;
    
    fscanf(arqCfg, "%d", numcampos);     /* Le número de campos */
    fseek(arqCfg, 2, SEEK_CUR);
    *h = Malloc(sizeof(Header)*(*numcampos));     /* Aloca o vetor head */
    
    for(i = 0; i < *numcampos; i++) {
        
        /* Le nome */
        fread((*h+i)->nome, tamPrimCampoHd, 1, arqCfg);
        (*h+i)->nome[tamPrimCampoHd] = '\0';
        TiraBrancosDoFinal((*h+i)->nome);
    
        /* Le tipo */
        (*h+i)->tipo = fgetc(arqCfg);
        
        /* Le endereço de inicio e final de um campo e calcula seu tamanho */
        fscanf(arqCfg, "%d %d", &((*h+i)->inicio), &final);
        (*h+i)->tamanho = final - (*h+i)->inicio + 1;
        
        /* Le caractere de obrigatoriedade do campo */
        fseek(arqCfg, 1, SEEK_CUR);
        c = fgetc(arqCfg);
        if(c == 'S')
            (*h+i)->obrig = true;
        else
            (*h+i)->obrig = false;
            
        /* Le mensagem */    
        fseek(arqCfg, 1, SEEK_CUR);
        fread((*h+i)->msg, tamUltCampoHd, 1, arqCfg);
        (*h+i)->msg[tamUltCampoHd] = '\0';
        TiraBrancosDoFinal((*h+i)->msg);
    
        fseek(arqCfg, 2, SEEK_CUR);
    }
    
} /* CarregaHeader */



  
int LeChaveOrdena(char* nomecampo, Header* head, int numcampos){
/* Retorna o índice do campo chave de ordenação para uso em Record.
   Retorna -1 se não houver tal campo nas configurações do arquivo de dados */

   int i;
   
   if(strcmp(nomecampo, "RA") && strcmp(nomecampo, "NOME"))   //a chave deve ser RA ou NOME
       return -1;
   
   for(i = 0; i < numcampos; i++)
       if(!strcmp(nomecampo, head[i].nome))  //chave encontrada
           return i;
           
   return -1;      //não achou a chave
   
} /* LeChaveOrdena */




int ValidaTamMem(char* arg, int tamreg){
/* Verifica se arg representa um inteiro != 0 e retorna seu valor dividido
   por tam. Retorna -1 em caso de erro. */
   
   int mem;
   
   if(!VerificaDigitos(arg))    //verifica se é uma string numerica
       return -1;
   
   mem = atoi(arg)/tamreg;
   
   if(mem == 0)     //memoria menor que o tamanho de cada registro
       return -1;
    
   return mem;

} /* ValidaTamMem */



    

FILE** CriaCorrida(FILE* arq, int maxreg, int tamreg, int key, Header* h, int numcampos, int* n, int* totalregs, int* nread, int* nwrite){
/* Retorna um vetor de arquivos (corrida) cada um deles com max registros de arq,
   de tamanho tam, ordenados por key. Coloca em n o número de arquivos da 
   corrida */
   
   char *linha;
   char nome[10];
   int i, regsArq;
   Record *reg;
   FILE** ppFile;
   Record registro;
   int k;
   
   linha = Malloc(sizeof(char)*tamreg);
   reg = Malloc(sizeof(Record*)*maxreg);
   *n = 0;
   *totalregs = 0;
      
   ppFile = Malloc(sizeof(FILE*));
   
   while(!feof(arq)) {                                  //loop para a criação das corridas
   
        regsArq = 0;
        for(i = 0; i < maxreg && !feof(arq); i++){      //for para criar o vetor de registros de cada corrida
            
            fread(linha, tamreg, 1, arq);
            *nread += 1;
            (*totalregs)++;
            linha[tamreg-1] = '\0';
            reg[i] = LeRegistroFixo(linha, numcampos, h);
            regsArq++;
        }
        
        //criando um registro temporario para a ordenação
        registro = Malloc(sizeof(char*)*numcampos);
  
        for(k=0;k<numcampos;k++)
            registro[k] = (char*)Malloc(sizeof(char)*(h[k].tamanho+1));

   
        if(regsArq>1)           //se tiver mais de um registro no arquivo, chama a funcao de ordenacao
                OrdenaRegistros(&reg, regsArq, key, h, numcampos, &registro);
     
        ppFile = (FILE**) realloc (ppFile, ((*n)+1) * sizeof(FILE*));

        ppFile[*n] = tmpfile();      //cria arquivo temporário
        
        for(i = 0; i < regsArq; i++) {      //imprime no arquivo e desaloca o registro
            
            ImprimeRegFixo(reg[i], ppFile[*n], numcampos, tamreg, h);
            *nwrite += 1;
            LiberaRegistro(reg[i], numcampos);
        }

        rewind(ppFile[*n]);

        (*n)++;
      
   }

   LiberaRegistro(registro, numcampos);
   free(linha);
   free(reg);

   return ppFile;
   
} /* CriaCorrida */




FILE* SortMgAux(FILE** ppFile, int lote, Header* h, int key, int ncampos, 
                  int tamreg, int* read, int* write, Boolean fim, char* saida) {
/* Função auxiliar de SortMerge. Efetua uma fase de Merge                     */

   if(lote == 1) return ppFile[0];

   //variáveis da função   
   int i;
   char* linha = malloc(sizeof(char)*tamreg);
   Record rec;
   RecSM lista = CriaRecSMNulo(ncampos);                          //criação da estr. de ordenação
   RecSM q;
   int naoacabou = lote;
   FILE* arqOut;
   char nome[10];

   //criação de uma lista ligada de RecSM, ordenada, com 'max' elementos
   for(i=0; i<lote; i++) {
            fread(linha, tamreg, 1, ppFile[i]);                   //leitura e 
            (*read)++;
            rec = LeRegistroFixo(linha, ncampos, h);              //criação de registro
            InsereSM(lista, rec, i, key);                         //inserção na estr. de ordenação

   }

   //arquivo que receberá o resultado do merge
   if(fim) arqOut = Fopen(saida, "wt");
   else    arqOut = tmpfile();                                    

   while(naoacabou) {
            q = lista->prox;                                      //1o registro da lista
            i = q->index;

            ImprimeRegFixo(q->reg, arqOut, ncampos, tamreg, h);   //descarrega no temp
            (*write)++;

            RemoveSM(lista, q, ncampos);                          //remove q da lista
            fread(linha, tamreg, 1, ppFile[i]);                   //lê o proximo de onde veio q->reg
            (*read)++;
            
            if(!feof(ppFile[i])) {
                     rec = LeRegistroFixo(linha, ncampos, h);      
                     InsereSM(lista, rec, i, key);                //insere o novo reg na lista
            }                                                          

            else     naoacabou--;                                 //se acabou o arquivo
   }
   
   free(linha);
   LiberaSM(lista, ncampos);                        
   rewind(arqOut);   
   return arqOut;
      
} /* SortMgAux */




FILE* SortMerge(FILE** ppFile, int* corridas, int max, Header* h, int key, 
   int ncampos, int tamreg, int* nread, int* nwrite, int* nfases, char* saida) {
/* Efetua Merge dos '*corridas' arquivos de 'ppFile', todos com 'ncampos' campos, 
   registros de tamanho total tamreg e leiaute indicado em 'h'. Os registros são 
   ordenados pela chave 'key'. O número máximo de registros simultaneamente em 
   memória é indicado por 'max'. Atualiza em 'corridas', 'nwrite', 'nread' e
   'nfases' o número de arquivos temporários criados, escritas e leituras de
   registros e fases de merge realizadas. Grava o resultado com nome 'saida'.
*/

   int i, resta = *corridas;
   int batch;
   FILE* arqOut;
   (*nfases) = 0;
   Boolean fim = false;

   while(resta > 1) {
               batch = (max < resta)? max : resta;             //qte de arquivos que será 
                                                               //enviada para sort
               if(batch == resta) fim = true;                                                               
                                                               
               arqOut = SortMgAux(ppFile, batch, h, key, ncampos, tamreg, 
                                                   nread, nwrite, fim, saida);
               (*corridas)++;
               (*nfases)++;
               resta -= batch;               

               for(i=0; i<batch; i++)                          //fecha arquivos já
                        fclose(ppFile[i]);                     //processados


               for(i=0; i<resta; i++)                          //reindexa vetor
                        ppFile[i] = ppFile[i+batch];

               ppFile[resta] = arqOut;                         //insere resultado
                                                               //no vetor
               resta++;

   }
   (*corridas)--;                            //um dos arquivos não é temporário
   return arqOut;
   
} /* SortMerge */
