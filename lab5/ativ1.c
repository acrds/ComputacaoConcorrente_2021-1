#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


   /*Teremos 2 passos basicamente:
    Quando terminar o somatório local, terá que colocar no vetor global um número aleatório gerado 
    por essa thread na posição correspondente no vetor*/

#define NTHREADS 7
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int *v;

//funcao barreira
void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads-1)) { 
      //ultima thread a chegar na barreira
      pthread_cond_broadcast(&x_cond);
      bloqueadas=0;
    } else {
      bloqueadas++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa (void *arg){
    int id = *(int*) arg;
    int *acumulador;
    acumulador = (int*) malloc(sizeof(int*));
    for(int j=0;j<2; j++){
        if(1){} 
        if(j==0){
            for(int i=0; i<NTHREADS;i++){
                *acumulador += v[i];
            }
        }else{
            v[id] = rand() % 10;
        }
        barreira(NTHREADS);
    }
    printf("Thread %d acumulador=%d v=%d\n", id,*acumulador, v[id]);
    pthread_exit(acumulador);
}

//funcao principal
int main(int argc, char *argv[]){
    pthread_t threads[NTHREADS];
    int id[NTHREADS];
    int *threadAcumulada;
    int vthreadAcumuladas[NTHREADS];
    threadAcumulada = (int*) malloc(sizeof(int));
    //criacao de vetor com len = nthreads
    v=(int*) malloc(sizeof(int)*NTHREADS); 
    if(v==NULL){
      fprintf(stderr, "Erro--malloc\n"); 
      return 2;
    }
    srand(time(NULL)); //valores do v precisam ser diferentes a cada execução
    for(int i=0; i<NTHREADS; i++){
        v[i]=rand() % 10; //gera valores aleatorios
    }
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    //criacao de threads
    for(int i=0; i<NTHREADS; i++){
        id[i]=i;
        pthread_create(&threads[i],NULL,tarefa,(void*)&id[i]);
    }
    
    //aguardar retorno de threads
    for(int i=0; i<NTHREADS; i++){
        pthread_join(threads[i],(void*)&threadAcumulada);
        vthreadAcumuladas[i] = *threadAcumulada;
    }

    for(int i=0; i<NTHREADS; i++){
        printf(" %d ",vthreadAcumuladas[i] );
    }

    //verificar se são diferentes
    for(int i=1;i<NTHREADS;i++){
        if(vthreadAcumuladas[i-1] !=vthreadAcumuladas[i]){
            printf("Erro\n");
            return 3;
        }
    }
    printf("Todos são iguais\n");
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}