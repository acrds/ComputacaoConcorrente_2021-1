/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementar uma solução sequencial e uma solução concorrente para o problema de encontrar o menor e o maior valor em um  vetor de números reais*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

//podia usar variavel global já que foi nos liberado, mas acho que seria mais prudente o struct
typedef struct{
   float larger;
   float smaller;
   int id;
}tArgs;

long int dim; //dimensao de vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim com alocacao dinamica
tArgs *retorno;
//tArgs *args;


//fluxo da thread
void *tarefa(void *arg){
    tArgs *args = (tArgs* ) arg;
    long int tamBloco = dim/nthreads; //tamanho do bloco de cada thread
    long int ini = args->id * tamBloco; //elemento inicial do bloco da thread
    long int fim = ini + tamBloco; //elemento final nao processado do bloco da thread
    
    if( args->id == nthreads - 1){ 
        fim = dim; 
    } else{
        fim = ini + tamBloco; //trata o resto se houver
    } 

    //achar menor e menor elemento
    float menorElemento = vetor[ini]; 
    float maiorElemento = vetor[ini]; 
    for(long int i=ini ; i<fim ; i++){
        if(vetor[i]>maiorElemento) {
            maiorElemento = vetor[i];
        }
        if(vetor[i]<menorElemento) {
            menorElemento = vetor[i]; 
        }
    }
    args -> larger = maiorElemento;
    args -> smaller = menorElemento;
    
//retorna o resultado 
pthread_exit((void*)args);
}

//fluxo principal
int main(int argc , char* argv[]){
    pthread_t *tid; //identificadores das threads no sistema
    double ini, fim; //tomada de tempo
    float bigger;
    float little;
    float listaMaiorElemento;
    float listaMenorElemento;
    float ma; 
    float me;
    tArgs *args;
    if(argc < 3){
        fprintf(stderr, "Digite o nome do programa: %s <dimensao do vetor> e <numero de threads>\n", argv[0]);
        return 1;
    }
     
    dim = atoll(argv[1]); //precisa ser atoll por ser um número grande
    nthreads = atoi(argv[2]); // não tem necessidade já que é recomendado usar ate 4 threads (numero proximo ao numero de cores)

    //aloca o vetor de entrada
    
    vetor=(float*) malloc(sizeof(float)*dim);
    if(vetor == NULL){
        fprintf(stderr, "Erro--malloc\n");
        return 2;
    } 
    
    args=malloc(sizeof(tArgs) * nthreads);
    retorno = malloc(sizeof(tArgs));

    for(long int i=0; i<dim; i++){
        vetor[i] = 1000.1/(i+1);
    }
    //computacao sequencial
    bigger = vetor[0];
    little = vetor[0];
    GET_TIME(ini);
    for(long int i=0; i<dim; i++){
        if(vetor[i] > bigger){
            bigger = vetor[i];
        } 
        if(vetor[i] < little){
            little = vetor[i]; 
        }   
    }
    printf("Maior Elemento na Computação Sequencial: %f\n", bigger);
    printf("Menor Elemento na Computação Sequencial: %f\n" , little);

    GET_TIME(fim);
    printf("Tempo total sequencial: %lf\n",fim - ini);

    
    //computacao concorrente   
    GET_TIME(ini);
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL) {
        fprintf(stderr, "Erro--malloc\n");
        return 2;
    }
    //criar as threads
    for(long int i=0; i<nthreads; i++){
        args[i].id = i;
        if(pthread_create(tid+i,NULL,tarefa,&args[i])){
            fprintf(stderr,"Erro--pthread_create\n");
            return 3;
        }
    }
    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++){
        if(pthread_join(*(tid+i),(void**)&retorno)){
            fprintf(stderr,"Erro--pthread_create\n");
            return 3;
        }
        listaMaiorElemento = retorno->larger;
        listaMenorElemento = retorno->smaller;
    }
    //inicializando o 'ma' e o 'me'
    ma = vetor[0];
    me = vetor[0];

    for(long int i=0 ; i<nthreads; i++){
        if(listaMaiorElemento>ma) {
            ma = listaMaiorElemento;
        }
        if(listaMenorElemento<me){
            me = listaMenorElemento;
        }     
    }

    GET_TIME(fim);
    printf("Tempo total concorrente: %lf\n",fim - ini);

    //exibir resultados
    printf("Maior Elemento na Computação Concorrente: %f\n", ma);
    printf("Menor Elemento na Computação Concorrente: %f\n" , me);
    
    //libera as areas de memoria alocadas
    free(vetor);
    free(tid);
    //free(retorno); //dava erro de segmental fault
    //free(args);
    printf("Tudo certo!\n");
    


    return 0;
}