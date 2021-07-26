/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Projetar e implementar um algoritmo concorrente para o problema de multiplicação de matrizes e coletar informações sobre o seu tempo de execucão*/

#include<stdio.h>
#include<stdlib.h>
#include"timer.h"
#include<pthread.h>

float *matrizPrincipal;   	//primeira matriz
float *matrizAuxiliar;   	//segunda matriz
float *saida;   			//matriz de saida
float *checar; 				//checar se a multiplicação ta certa
int nthreads; 				//numero de threads
typedef struct{				//definir struct
	int id; 				// identificador do elemento que a thread vai processar
	int dim; 				// dimensao das estruturas de entrada
}tArgs;

/*Funcao que as threads irao executar */
void* tarefa(void *arg){
	tArgs *args = (tArgs*) arg;
	for(int i=args->id;i<args->dim;i+=nthreads){
		for(int j=0; j<args->dim;j++){
			for(int k=0;k<args->dim;k++){
				saida[i*args->dim+j]+=matrizPrincipal[i*args->dim+k]*matrizAuxiliar[k*args->dim+j];
			}
		}
	}
	pthread_exit(NULL);
}

/*Fluxo principal*/
int main(int argc, char*argv[]){
	int dim; 			//dimensao da matriz de entrada
	pthread_t *tid; 	//identificadores das threads no sistema
	tArgs *args;		//identificadores locais das threads e dimensao
	int i;
	int j;
   	double begin;
	double end;
	double duration;
	GET_TIME(begin);

   	/*Etapa de leitura e avaliacao dos parametros de entrada*/
	if(argc<3){ //<3 pq um será a o arg passado na hora de pedir pra compilar no ./ outro da dim e outro do numero de thread.
		printf("Quantidade de parâmetros insuficiente. Digite: %s <dimensao da matriz> \n ", argv[0]);
		return 1;
	}

	dim=atoi(argv[1]); //funcao atoi transforma string pra inteiro
	nthreads = atoi(argv[2]);
	if(nthreads>dim) nthreads=dim;

	/*Etapa de alocacao de memoria para as estruturas de dados*/
	matrizPrincipal=(float*) malloc(sizeof(float)*dim*dim); /* - malloc armazena uma quantidade de bytes no primeiro espaço de memória que dê para esses bytes estarem alocados de forma contínua na memória
    - malloc retorna ponteiro pra void, por isso faz um typecast para float [(float*)]
    - sizeof é o tipo que será usado */
	if(matrizPrincipal==NULL){ //caso o malloc não ache o espaço ele retorna NULL
		printf("Erro --malloc\n");
		return 2;
	}

	matrizAuxiliar= (float*) malloc(sizeof(float)*dim*dim); 
	if(matrizAuxiliar==NULL){
		printf("Erro --malloc\n");
		return 2;
	}

	saida= (float*) malloc(sizeof(float)*dim*dim); 
	if(saida==NULL){
		printf("Erro --malloc\n");
		return 2;
	}

	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){ //na primeira vez que passar aqui vai estar atuando somente na primeira linha da matriz
			matrizPrincipal[i*dim+j]=i+j;
			matrizAuxiliar[i*dim+j]=j;
			saida[i*dim+j]=0;
		}
	}

	GET_TIME(end);
   	duration = end-begin;
  	printf("Duracao de inicializacao: %lf\n", duration);
	
	/*Etapa de multiplicacao da matriz pelo vetor */
   	GET_TIME(begin);
   	/*Etapa de alocacao das estruturas*/
	tid=(pthread_t*)malloc(sizeof(pthread_t)*nthreads);
	if(tid==NULL) {
		puts("Erro"); 
		return 2;
	}

	args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
	if(args==NULL) {
		puts("Erro"); 
		return 2;
	}

	/*Etapa de criacao das threads*/
	for(int i =0; i<nthreads;i++){
		(args+i)->id=i;
		(args+i)->dim=dim;
		if(pthread_create(tid+i,NULL,tarefa,(void*)(args+i))){
			puts("Erro--pthread_create");
			return 3;
		}
	}

	/*Etapa de espera pelo termino das threads*/
	for(int i=0;i<nthreads;i++){
		pthread_join(*(tid+i),NULL);
	}
	
	GET_TIME(end);   //finaliza a contagem do tempo
   	duration = end-begin; //calcula a duracao
   	printf("Duracao da multiplicacao: %lf\n", duration);
	
	
	checar= (float*) malloc(sizeof(float)*dim*dim); //alocando e inicializando a matriz de verificacao, para nao haver interferencia
	if(checar==NULL){
		puts("Erro");
		return 2;
	}

	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			checar[i*dim+j]=0;
		}
	}

	for( i=0;i<dim;i++){
		for( j=0;j<dim;j++){
			for(int k=0;k<dim;k++){
				checar[i*dim+j]+=matrizPrincipal[i*dim+k]*matrizAuxiliar[k*dim+j];
			}  
		}
	}
	
	
	/*Etapa de Verificacao de erros */
	int aux=0;
	for( i=0;i<dim;i++){
		for( j=0;j<dim;j++){
			if(checar[i*dim+j]!=saida[i*dim+j])
			aux+=1;
		}
	}
	
	if(aux==0){
		puts("Sem erros");
	}
	else{
		puts("Erro durante a execucao");
	}

	/*Etapa de liberacao de memoria */
	free(matrizPrincipal);
	free(matrizAuxiliar);
	free(saida);
	free(args);
	free(tid);
	free(checar); 
	GET_TIME(begin);
	GET_TIME(end);  
   	duration=end-begin;
   	printf("Duracao da finalizacao: %lf\n", duration);
	return 0;
}

/*ANEXO*/
	/*Etapa de exibicao dos resultados.*/
	/*ESTA ETAPA FICARÁ COMENTADO POIS TRABALHAREMOS COM MATRIZES GRANDES */
	/*for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			printf("%.1f",matrizPrincipal[i*dim+j]);
		}
		printf("\n");
	}
	printf("\n");

	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			printf("%.1f",matrizAuxiliar[i*dim+j]);
		}
		printf("\n");
	}
	printf("\n");

	for( i=0;i<dim;i++){
		for( j=0;j<dim;j++){
			printf("%.1f",saida[i*dim+j]);
		}
		printf("\n");
	}
	printf("\n");

	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			printf("%.1f",checar[i*dim+j]);
		}
		printf("\n");
	}
	printf("\n");
	*/