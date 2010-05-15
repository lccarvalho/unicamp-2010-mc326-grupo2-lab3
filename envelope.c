/******************************************************************************/
/* Grupo 2:                                                                   */
/*          Felipe Augusto da Silva    RA 096993                              */
/*          Lucas Barbi Rezende        RA 097042                              */
/*          Luiz Claudio Carvalho      RA 800578                              */
/*                                                                            */
/* MC236EF  1o semestre 2010                                           UNICAMP*/

/* envelope.c - conjunto de funções envelopadoras */

#include "envelope.h"



FILE * Fopen(char *arg1, char *arg2){
/* Abre um arquivo. Em caso de erro, imprime mensagem de erro e finaliza
   a execução do programa */
     
     FILE *f;
     f = fopen(arg1, arg2);
     
     if(f == NULL) {
          fprintf(stderr, "%s\n", "Erro arquivo\n");
          system("pause");
          exit (0);
     }
     
     return f;
     
} /* Fopen */


void * Malloc(int tamanho){
/* Aloca e retorna um espaço de memória de 'tamanho' bytes. Caso não haja
   memória suficiente, imprime mensagem de erro e finaliza a execução do
   programa */
     
     void * ptr;
     ptr = malloc(tamanho);
     
     if(ptr == NULL) {
            //fprintf(stderr, "%s\n", MSG_ERRO_MEMORIA);
            system("pause");
            exit (0);
     }
     
     return ptr;
     
} /* Malloc */
