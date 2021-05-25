#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


#define NTHREADS  4

/* Variaveis globais */
sem_t cond_intermed, cond1_final, cond2_final; // semaforo para exclusao mutua entre as threads

/* Thread A */
void *frase_inicial (void *threadid) {
    printf("Seja bem vindo!\n");
    sem_post(&cond_intermed);
    pthread_exit(NULL);
}

/* Thread B */ 
void *frase_intermediaria1 (void *threadid) {
    sem_wait(&cond_intermed);
    printf("Fique a vontade.\n");
    sem_post(&cond1_final);
    pthread_exit(NULL);
}

/* Thread C */ 
void *frase_intermediaria2 (void *threadid) {
    sem_wait(&cond_intermed);
    printf("Sente-se por favor.\n");
    sem_post(&cond2_final);
    pthread_exit(NULL);
}

/* Thread D */ 
void *frase_final (void *threadid) {
    sem_wait(&cond1_final);
    sem_wait(&cond2_final);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t tid[NTHREADS];
  int *id[3], t;

  for (t=0; t<NTHREADS; t++) {
    if ((id[t] = malloc(sizeof(int))) == NULL) {
       pthread_exit(NULL); return 1;
    }
    *id[t] = t+1;
  }

  /* Inicilaiza os semaforos */
  sem_init(&cond_intermed, 0, 1);
  sem_init(&cond1_final, 0, 1);
  sem_init(&cond2_final, 0, 1);

  /* Cria as threads */
  if (pthread_create(&tid[3], NULL, frase_inicial, (void *)id[3])) { printf("--ERRO: pthread_create()\n"); exit(-1); };
  if (pthread_create(&tid[2], NULL, frase_intermediaria1, (void *)id[2])) { printf("--ERRO: pthread_create()\n"); exit(-1); };
  if (pthread_create(&tid[1], NULL, frase_intermediaria2, (void *)id[1])) { printf("--ERRO: pthread_create()\n"); exit(-1); };
  if (pthread_create(&tid[0], NULL, frase_final, (void *)id[0])) { printf("--ERRO: pthread_create()\n"); exit(-1); };

  /* Espera todas as threads completarem */
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
    free(id[t]);
  } 

  pthread_exit(NULL);
}
