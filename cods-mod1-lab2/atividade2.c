#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *mat;
float *mat2;
float *saida;
int nthreads;

typedef struct{
    int id;
    int dim;
} tArgs;

void* tarefa(void *arg){
    tArgs *args = (tArgs*) arg;

    for( int i=args->id; i<args->dim; i+=nthreads ){
        for( int j=0; j<args->dim; j++){
            for( int k=0; k<args->dim; k++){
                saida[i*args->dim + j] += mat[i*args->dim + k] * mat2[k*args->dim + j];
            }
        }
    }
    // for( int j=0; j<args->dim; j++ ){
    //     saida[args->id] += mat[ (args->id) * (args->dim) + j ] * mat2[j];
    // }
    pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char* argv[]) {
    int dim; //dimensao da matriz de entrada
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao
    double inicio, fim, delta; //identificadores da macro timer

    GET_TIME(inicio);
    //leitura e avaliacao dos parametros de entrada
    if(argc<3) {
        printf("Digite: %s <dimensao da matriz> <quantidade de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if(nthreads > dim) nthreads = dim;

    //alocacao de memoria para as estruturas de dados
    mat = (float *) malloc(sizeof(float) * dim * dim);
    if( mat == NULL ) { printf("ERRO--malloc\n"); return 2;}
    mat2 = (float *) malloc(sizeof(float) * dim * dim);
    if( mat2 == NULL ) { printf("ERRO--malloc\n"); return 2;}
    saida = (float *) malloc(sizeof(float) * dim * dim);
    if( saida == NULL ) { printf("ERRO--malloc\n"); return 2;}

    //inicializacao das estruturas de dados de entrada e saida
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            mat[i*dim + j] = 1; //equivalente mat[i][j]
            mat2[i*dim + j] = 1;
            saida[i*dim+j] = 0;
        }
        // mat2[i] = 1;
        // saida[i] = 0;
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Inicialização: %lf\n", delta);

    GET_TIME(inicio);
    //multiplicacao da matriz1 pela matriz2
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}
    
    //criacao das threads
    for(int i=0; i<dim; i++){
        (args+i)->id = i;
        (args+i)->dim = dim;
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3; //break permite q a aplicacao continue mesmo com erros
        }
    }

    //espera pelo termino das threads
    for(int i=0; i<nthreads; i++){
        pthread_join(*(tid+i), NULL);
    }
    // for(i=0; i<dim; i++){
    //     for(j=0; j<dim; j++){
    //         saida[i] += mat[i*dim+j]*mat2[j];
    //     }
    // }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Criação das Threads, Execução da Multiplicação e Término: %lf\n", delta);

    GET_TIME(inicio);
    //exibicao dos resultados
    // puts("Matriz de entrada:");
    // for(int i=0; i<dim; i++){
    //     for(int j=0; j<dim; j++){
    //         printf("%.1f ", mat[i*dim+j]);
    //     }
    //     puts("");
    // }
    // puts("Matriz 2 de entrada:");
    // for(int j=0; j<dim; j++){
    //     printf("%.1f ", mat2[j]);
    // }
    // puts("");
    puts("Matriz de saida:");
    for(int j=0; j<dim; j++){
        printf("%.1f ", saida[j]);
    }
    puts("");

    //liberacao da memoria
    free(mat);
    free(mat2);
    free(saida);
    free(args);
    free(tid);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Finalização do Programa: %lf\n", delta);

    return 0;
}
