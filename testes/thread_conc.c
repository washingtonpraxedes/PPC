#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

pthread_t threads[3];

int baralho[15];
pthread_mutex_t mutex[3];
monte montes[3];

typedef struct{
	int idx, deck[5];
	
}jogador;

typedef struct{
	int carta,cheio;
	pthread_mutex_t equip;
}monte;

void numerar_baralho(){
	int i;
	for(i=0;i<12;i++){
		baralho[i] = i;
	}

}

void init_all(){
	int i;
	for(i=0;i<3;i++){
		pthread_mutex_init(&montes[i].equip,NULL);
		pthread_mutex_init(&mutex[i],NULL);
	}
}

jogador init_jogador(int id){
	int i,j=id;
	jogador jogador;
	for(i=0;i<4;i++){
		jogador.deck[i] = j;
		j++; 
	}
	jogador.idx = id;
	return jogador;
}

int check_win(jogador jog){
	for(i=0;i<4;i++){
			soma += jogador.deck[i];
		}
		if(soma == 23){
			printf("Jogador %d ganhou!!\n\n",jogador.idx);
			return 1;
		}
		else return 0;
}

void *thread_func(void * args){
	int id = *(int *) args;
	jogador jog = init_jogador(id);
	int i,soma=0;
	while(1){
		if(check_win(jog)) break;
		else{
			pthread_mutex_lock(&mutex[id%3]);
			pthread_mutex_lock(//monte.equip);
			//saca carta
			pthread_mutex_unlock(&mutex[id%3]);
			pthread_mutex_unlock(//monte.equip);
			pthread_mutex_lock(&mutex[(id+1)%3]);
			pthread_mutex_lock(//monte.equip);
			//descarta_carta
			pthread_mutex_unlock(&mutex[(id+1)%3];
			pthread_mutex_unlock(//monte.equip);
			if(check_win(jogador)) break;
			
			
		}
	}
	
	
}

int main (){
	int i;
	numerar_baralho();
	pthread_mutex_init(&mutex,NULL);
	for(i=0;i<3;i++){
		pthread_create(&threads[i],NULL,thread_func,NULL);
	
	}
	for(i=0;i<3;i++){
		pthread_join(threads[i],NULL);
	}

	


}



