#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int aux=0;
pthread_mutex_t lock;
pthread_cond_t cond01;
pthread_cond_t cond02;

void *thread1(void *args){
    pthread_mutex_lock(&lock);
    if(aux==0){
        pthread_cond_wait(&cond01, &lock);
    }
    printf("Fique a vontade.\n");
    aux++;
    if(aux==3){
	    pthread_cond_signal(&cond02);
    }
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void *thread2(void *args){
    printf("Seja bem-vindo.\n");
    pthread_mutex_lock(&lock);
    aux++;
    pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&cond01);
    pthread_exit(NULL);
}

void *thread3(void *args){
    pthread_mutex_lock(&lock);
    if(aux<3){
        pthread_cond_wait(&cond02,&lock);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void *thread4(void *args){
    pthread_mutex_lock(&lock);
    if(aux==0){
        pthread_cond_wait(&cond01,&lock);
    }
    printf("Sente-se por favor.\n");
    aux++;
    if(aux==3){
	    pthread_cond_signal(&cond02);
    }
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main(){
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond01,NULL);
    pthread_cond_init(&cond02,NULL);
    pthread_t tid[4];
    if(pthread_create(&tid[0],NULL,thread1,NULL)){
        fprintf(stderr,"ERRO pthread_create\n");
        return 1;
    }
    if(pthread_create(&tid[1],NULL,thread2,NULL)){
        fprintf(stderr,"ERRO pthread_create\n");
        return 1;
    }
    if(pthread_create(&tid[2],NULL,thread3,NULL)){
        fprintf(stderr,"ERRO pthread_create\n");
        return 1;
    }
    if(pthread_create(&tid[3], NULL, thread4, NULL)){
        fprintf(stderr,"ERRO pthread_create\n");
        return 1;
    }
    for(int thread=0;thread<4;thread++){
        if(pthread_join(tid[thread],NULL)){
            fprintf(stderr,"Erro ao executar pthread_join()\n");
            return 2;
        }
    }
    return 0;
}