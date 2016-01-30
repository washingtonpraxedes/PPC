#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>


typedef struct jogador{
	int[4] baralho;
}jogador;

typedef struct deck{
	int[3] fila;
	int base,topo,N,n; //base = ultima carta / topo = primeira carta / N = número máximo de elementos / n = número de cartas presentes
}deck;

jogador jog[4];
deck decks[4];
pthread_mutex mutex_acesso[4];
sem_t sem_controle_cheio[4];
sem_t sem_controle_vazio[4];

/*int*/void embaralhar(){
	srand((unsigned)time(NULL));
	int i, vtr1[24],aux, j, t;
	for (i=0;i<24;i++){
		vtr1[i] = 25;
	}
	for(i=0;i<24;i++){
		t=1;
		do{
			aux = rand()%24;
			for(j=0;j<24;j++){
				if(!(aux == vtr1[j])) t=1; 
				else {t = 0;break;}
			}
			if(t) vtr1[i] = aux;
		}while(!t);		
	}
	printf("deck ");
	for(i=0;i<24;i++) printf("%d ",vtr1[i]);
	printf("\n");
}

deck reorganizar_deck_apos_saque(deck d){
	d.fila[topo] = -1;
	d.topo--;
	d.n--;
}

deck reorganizar_deck_apos_descarte(deck d){
	d.base++;
	d.n++;
}

void matar_todos(){

}

void sacar_carta(int jog_id, int index_carta){
	sem_wait(&sem_controle_cheio[jog_id-1]);
	pthread_mutex_lock(&mutex_acesso[jog_id-1],NULL);
	jog[jog_id-1].baralho[index_carta] = decks[jog_id-1].fila[topo];
	decks[jog_id-1] = reorganizar_deck_apos_saque(decks[jog_id-1]);
	pthread_mutex_unlock(&mutex_acesso[jog_id-1],NULL);
	sem_post(&sem_controle_vazio[jog_id-1]);
}

void descartar(int jog_id, int index_carta){
	sem_wait(&sem_controle_vazio[jog_id%4]);
	pthread_mutex_lock(&mutex_acesso[jog_id%4],NULL);
	decks[jog_id%4].fila[base] = jog[jog_id%4].baralho[index_carta];
	decks[jog_id%4] = reorganizar_deck_apos_descarte(decks[jog_id%4]);		
	pthread_mutex_unlock(&mutex_acesso[jog_id%4],NULL);
	sem_post(&sem_controle_cheio[jog_id%4]);
}

void init_all_sems(){
	int i;
	for(i=0;i<4;i++){
		sem_init(&mutex_controle_cheio[i],0,0);
		sem_init(&mutex_controle_vazio[i],0,2);
		pthread_mutex_init(&mutex_acesso[i],NULL);
	}
}

void init_all_jog(){
	int i;
	for(i=0;i<4;i++){
		
	}
	
}

void init_all_decks(){
	int i;
	for (i=0;i<4;i++){
		
	}

}

void verificar_vitoria(){

}

int escolhar_descarte(){
	srand((unsigned)time(NULL));
	return rand()%4;
}

int main(){
	embaralhar();
	sleep(2);
	embaralhar();
	sleep(2);
	embaralhar();

return 0;
}

