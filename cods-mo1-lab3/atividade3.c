//Soma todos os elementos de um vetor de inteiro
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<math.h>

int nthreads; //numero de threads 
long long int nelementos; //numero de elementos da serie para calcular pi

void * tarefa(void * arg) {
    double * pi = 0;
    pi = (double*) malloc(sizeof(double));
    if(pi == NULL) { fprintf(stderr, "Erro-malloc\n"); exit(1); }

    long int id = (long int) arg; //id thread
    long long int tamBloco = nelementos/nthreads;
    long long int ini = id*tamBloco;
    long long int fim;
    if(id == nthreads-1) fim = nelementos;
    else fim = ini+tamBloco;
    double sinal;

    if (ini%2 == 0) sinal = -1;
    else sinal = 1;

    for(long long int i=ini; i<fim; i++){
        sinal *= -1;
        *pi += sinal/(1 + 2*i);
    } 

   pthread_exit((void *) pi); 
}

//fluxo principal
int main(int argc, char *argv[]) {
    double *retorno;
    double somaSeq= 0; //soma sequencial
    double somaConc= 0; //soma concorrente
    double ini, fim; //tomada de tempo
    pthread_t *tid; //identificadores das threads no sistema

    //recebe e valida os parametros de entrada (nelementos, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <numero de elementos> <numero threads>\n", argv[0]);
        return 1; 
    }
    nelementos = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    
    //soma sequencial dos elementos
    GET_TIME(ini);
    double * pi = 0;
    pi = malloc(sizeof(double));
    if(pi == NULL) { fprintf(stderr, "Erro-malloc\n"); exit(1); }

    double sinal = -1;

    for(long long int i=0; i<nelementos; i++){
        sinal *= -1;
        *pi += sinal/(1 + 2*i);
    } 
    somaSeq = 4* *pi;
    GET_TIME(fim);
    printf("Tempo sequencial:  %lf\n", fim-ini);

    //soma concorrente dos elementos
    GET_TIME(ini);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //criar as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    
    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        somaConc += *retorno;
    }
    somaConc *= 4;
    GET_TIME(fim);
    printf("Tempo concorrente:  %lf\n", fim-ini);

    //exibir os resultados
    printf("Soma seq:  %.12lf\n", somaSeq);
    printf("Soma conc: %.12lf\n", somaConc);

    //libera as areas de memoria alocadas
    free(tid);

    return 0;
}