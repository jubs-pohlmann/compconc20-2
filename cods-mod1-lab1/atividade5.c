/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C e a funcao que espera as threads terminarem */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
int sizeArray = 1000;
int array[1000]; //inicializa um array de 0s

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads, startIndex, endIndex;
} t_Args;

//funcao executada pelas threads
void *Increment (void *arg) {
  t_Args *args = (t_Args *) arg;

  for(int i=args->startIndex; i<=args->endIndex; i++){
      array[i] ++;
      printf("%d -- Valor: %d, Thread: %d \n", i, array[i], args->idThread);
  }
  
  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  t_Args *arg; //receberá os argumentos para a thread

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Aloca e preenche argumentos para thread %d\n", thread);
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread; 
    arg->nThreads = NTHREADS; 
    arg->startIndex = sizeArray*thread/NTHREADS; 
    arg->endIndex = sizeArray*(thread+1)/NTHREADS - 1;
    
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, Increment, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  printf("--Thread principal terminou\n");
  pthread_exit(NULL);
}
