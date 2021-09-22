#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#include <semaphore.h>

sem_t condt1;
sem_t condt2;
sem_t condt3;

//funcoes das threads
void *thread1(void *args){
    sem_wait(&condt1);
    printf("Fique a vontade.\n");
    sem_post(&condt2);
    pthread_exit(NULL);
}

void *thread2(void *args){
    printf("Seja bem-vindo.\n");
    sem_post(&condt1);
    sem_post(&condt1);
    pthread_exit(NULL);
}

void *thread3(void *args){
    sem_wait(&condt2);
    sem_wait(&condt3);
    printf("Volte sempre!\n");
}

void *thread4(void *args){
    sem_wait(&condt1);
    printf("Sente-se por favor.\n");
    sem_post(&condt3);
    pthread_exit(NULL);
}

//funcao principal
int main(){
    sem_init(&condt1,0,0);
    sem_init(&condt2,0,0);
    sem_init(&condt3,0,0);
    pthread_t tid[4];
    if(pthread_create(&tid[0],NULL,thread1,NULL)){
        fprintf(stderr,"ERRO--pthread_create\n");
        return 1;
    }
    if(pthread_create(&tid[1],NULL,thread2,NULL)){
        fprintf(stderr,"ERRO--pthread_create\n");
        return 1;
    }
    if(pthread_create(&tid[2],NULL,thread3,NULL)){
        fprintf(stderr,"ERRO--pthread_create\n");
        return 1;
    }
    if(pthread_create(&tid[3],NULL,thread4,NULL)){
        fprintf(stderr,"ERRO--pthread_create\n");
        return 1;
    }
    for(int thread=0;thread<4;thread++){
        if(pthread_join(tid[thread],NULL)){
            fprintf(stderr,"EEEO--pthread_join()\n");
            return 2;
        }
    }
    return 0;
}