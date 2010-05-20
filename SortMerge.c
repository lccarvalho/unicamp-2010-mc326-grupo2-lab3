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
   
   registro = malloc(sizeof(char*)*n);
   
   for(i=0;i<n;i++){
                    
      if(i==0 || i==3 || i==4 || i==5){    //campos que tem um ' ' no final
      
          registro[i] = (char*)malloc(sizeof(char)*(h[i].tamanho+2));
          strncpy(registro[i], &linha[h[i].inicio-1], h[i].tamanho+1);
          registro[i][h[i].tamanho + 1] = '\0';
      }
      else {
           
          registro[i] = (char*)malloc(sizeof(char)*(h[i].tamanho+1));
          strncpy(registro[i], &linha[h[i].inicio-1], h[i].tamanho);
          registro[i][h[i].tamanho] = '\0';
      }
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




void ImprimeRegFixo(Record rec, FILE* arq, int numcampos, int tamreg){
/* Grava, na posição corrente em arq, os dados de rec.                        */

   int i;
   char *linha = Malloc(sizeof(char)*(tamreg+1));
   
   linha[0] = '\0';
   
   /* junta todos os campos do registros em um unico bloco de memoria
      para dar apenas um fwrite */
   
   for(i = 0; i < numcampos; i++)
         strcat(linha, rec[i]);
   
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
    *h = malloc(sizeof(Header)*(*numcampos));     /* Aloca o vetor head */
    
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
   
   if(strcmp(nomecampo, "RA") && strcmp(nomecampo, "NOME"))
       return -1;
   
   for(i = 0; i < numcampos; i++)
       if(!strcmp(nomecampo, head[i].nome))
           return i;
           
   return -1;
   
} /* LeChaveOrdena */




int ValidaTamMem(char* arg, int tamreg){
/* Verifica se arg representa um inteiro != 0 e retorna seu valor dividido
   por tam. Retorna -1 em caso de erro. */
   
   int mem;
   
   if(!VerificaDigitos(arg))
       return -1;
   
   mem = atoi(arg)/tamreg;
   
   if(mem == 0)
       return -1;
       
   return mem;

} /* ValidaTamMem */




int NumRegs(FILE* arq, int tamreg){
/*  Retorna o numero total de registros de tamanho 'tamreg' em 'arq' */
    
    int n;
    
    fseek(arq, 0, SEEK_END);
    
    n = (ftell(arq)/tamreg);
    
    rewind(arq); 
    
    return n;
    
} /* NumRegs */


    

FILE** CriaCorrida(FILE* arq, int maxreg, int tamreg, int key, Header* h, int numcampos, int* n, int totalregs, int* nread, int* nwrite){
/* Retorna um vetor de arquivos (corrida) cada um deles com max registros de arq,
   de tamanho tam, ordenados por key. Coloca em n o número de arquivos da 
   corrida */
   
   char *linha;
   char nome[10];
   int i, j, regsArq = 0;
   Record *reg;
   FILE** ppFile;
   Record registro;
   int k;
   
   linha = malloc(sizeof(char)*tamreg);
   reg = malloc(sizeof(Record*)*maxreg);
   
   *n = totalregs / maxreg;             //numero de corridas
   if(totalregs % maxreg != 0)
       *n += 1;
       
   ppFile = malloc(sizeof(FILE*)*(*n));
   
   
   for(j = 0; j < *n && !feof(arq); j++) {      //for para a criação das corridas
   
        for(i = 0; i < maxreg && !feof(arq); i++){      //for para criar o vetor de registros de cada corrida
            fread(linha, tamreg, 1, arq);
            *nread += 1;
            linha[tamreg-1] = '\0';
            reg[i] = LeRegistroFixo(linha, numcampos, h);
            regsArq++;
        }
        
        //criando um registro temporario para a ordenação
        registro = malloc(sizeof(char*)*numcampos);
  
        for(k=0;k<numcampos;k++){
            if(k==0 || k==3 || k==4 || k==5)    //campos que tem um ' ' no final     
                    registro[k] = (char*)malloc(sizeof(char)*(h[k].tamanho+2));
            else            
                    registro[k] = (char*)malloc(sizeof(char)*(h[k].tamanho+1));
        }  
   
        if(regsArq>1)  //se tiver mais de um registro no arquivo, chama a funcao de ordenacao
                        OrdenaRegistros(&reg, regsArq, key, h, numcampos, &registro);
     
        itoa(j, nome, 10);
        strcat(nome, ".tmp");     //nome para o arquivo temporario
        ppFile[j] = Fopen(nome, "w+");       //cria arquivo temporario
        
        for(i = 0; i < regsArq; i++) {      //imprime no arquivo e desaloca o registro
            ImprimeRegFixo(reg[i], ppFile[j], numcampos, tamreg);
            *nwrite += 1;
            LiberaRegistro(reg[i], numcampos);
        }

        regsArq = 0;
   }

   LiberaRegistro(registro, numcampos);   
   free(linha);
   free(reg);
   
   for(j = 0; j < *n; j++)
       rewind(ppFile[j]);

   return ppFile;
   
} /* CriaCorrida */




FILE* SortMerge(FILE** ppFile, int inf, int sup, int max, Header* h, 
                                            int key, int ncampos, int tamreg) {

   //caso base da recursão   
   if(inf == sup) return ppFile[inf];
    
   //variáveis da função   
   int i;
   char* linha = malloc(sizeof(char)*tamreg);
   Record rec;
   RecSM lista = CriaRecSMNulo(ncampos);                       //criação da estr. de ordenação
   RecSM q;
   int naoacabou = max;
   FILE* arqOut;

   //criação de uma lista ligada de RecSM, ordenada, com 'max' elementos
   for(i=0; i<max; i++) {
            fread(linha, tamreg, 1, ppFile[inf+i]);            //leitura e criação de 
            rec = LeRegistroFixo(linha, ncampos, h);           //registro
            InsereSM(lista, rec, i, key);                      //inserção na estr. de ordenação
   }

   //arquivo que receberá os registros dos 'max' próximos arquivos da corrida
   arqOut = fopen("tmpSM2.tmp", "wt");

   while(naoacabou) {
            q = lista->prox;                                          //1o registro da lista
            i = q->index;

            ImprimeRegFixo(q->reg, arqOut, ncampos, tamreg);          //descarrega no temp
            RemoveSM(lista, q, ncampos);                              //remove q da lista
            fread(linha, tamreg, 1, ppFile[inf+i]);                   //lê o proximo de onde veio q->reg

            if(!feof(ppFile[inf + i])) {
                     rec = LeRegistroFixo(linha, ncampos, h);      
                     InsereSM(lista, rec, i, key);                    //insere o novo reg na lista
            }                                                          
            else     naoacabou--;                                     //se acabou o arquivo
   }
   free(linha);


//   LiberaSM(lista);                        
   return arqOut;
      
} /* SortMerge */
