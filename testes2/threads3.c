#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

pthread_t threads[4];

int* embaralhar(int tam){
	srand((unsigned)time(NULL));
	int i,aux, j, t;
	int *vtr1 = malloc(tam*sizeof(int));
	for (i=0;i<tam;i++){
		vtr1[i] = 25;
	}
	for(i=0;i<tam;i++){
		t=1;
		do{
			aux = rand()%tam;
			for(j=0;j<tam;j++){
				if(!(aux == vtr1[j])) t=1; 
				else {t = 0;break;}
			}
			if(t) vtr1[i] = aux;
		}while(!t);		
	}
	printf("deck ");
	for(i=0;i<tam;i++) printf("%d ",vtr1[i]);
	printf("\n");
	return vtr1;
}

void* thread_func(void *args){
	printf("Executando thread %d...\n",*(int *)args);
	pthread_yield();
}

int main(){
	srand((unsigned)time(NULL));
	int i,j,*aux;
	for(j=0;j<5;j++){
	srand((unsigned)time(NULL));
	aux=embaralhar(4);
	for(i=0;i<4;i++){
		
		pthread_create(&threads[i],NULL,thread_func,(void*)&i); 
		pthread_join(threads[*(aux+i)],NULL);
		usleep(200000);
	}
	}
	//pthread_yield();
	//for(i=0;i<4;i++) 
	//pthread_exit(NULL);



}
