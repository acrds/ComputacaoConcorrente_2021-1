/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementar um programa concorrente, com duas threads (alem da thread principal), para elevar ao quadrado cada elemento de um vetor de 10000 elementos.(Para cada elemento ai do vetor, calcular (ai)**2 e escrever o resultado na mesma posicao do elemento.)*/

#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define NELEMENTOS 10000
int v[NELEMENTOS];

//funcao que a thread vai executar
void *tarefa(void *arg){
    int ident = *(int *)arg; 
    if (ident == 1){
        printf("Ola, sou a thread %d \n", ident);
        for (int i=0; i < (NELEMENTOS/2); i++){
            v[i] = i*i;
        }}
        else{printf("Ola, sou a thread %d \n", ident);
        for (int i = (NELEMENTOS/2); i < NELEMENTOS; i++){
            v[i] = i*i;
        }}
    pthread_exit(NULL);
}

//funcao principal
int main(void){
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS];     //identificador local da thread
    int qtdErros=0;
    for (int i=0; i < NTHREADS; i++){
        ident[i] = i;
        if (pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i]))
            printf("ERRO -- pthread_create\n");
    }

    for (int i=0; i < NTHREADS; i++){
        //espera as threads terminarem
        if (pthread_join(tid[i], NULL))
            printf("ERRO -- pthread_join\n");
    }
    
    for (int i=0; i < NELEMENTOS; i++){
        if(v[i]!=i*i){ //verificar se o programa tá fazendo o objetivo do lab1
            printf("Valores diferentes. Apareceu %d e era %d.",v[i],i*i);
            qtdErros++;
        }
    }
    if(qtdErros == 0){
        printf("Ok!\n");}
        else{ printf("Houve erro!");
    }
    printf("Ola, sou a thread principal\n");
    //pthread_exit(NULL);
    return 0;
}