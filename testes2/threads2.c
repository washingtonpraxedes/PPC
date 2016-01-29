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
pthread_mutex mutex_controle_cheio[4];
pthread_mutex mutex_controle_vazio[4];

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
	pthread_mutex_lock(&mutex_acesso[jog_id-1],NULL);
	if(decks[jog_id-1].n>0){
		jog[jog_id-1].baralho[index_carta] = d[jog_id-1].fila[topo];
		d[jog_id-1] = reorganizar_deck_apos_saque(d[jog_id-1]);
		pthread_mutex_unlock(&mutex_controle_cheio[jog_id-1],NULL);
	}
	else pthread_mutex_lock(&mutex_controle_vazio[jog_id-1],NULL);
	pthread_mutex_unlock(&mutex_acesso[jog_id-1],NULL);
}

void descartar(int jog_id, int index_carta){
	pthread_mutex_lock(&mutex_acesso[jog_id%4],NULL);
		if(decks[jog_id%4].n!=N){
		d[jog_id%4].fila[base] = jog[jog_id%4].baralho[index_carta];
		d[jog_id%4] = reorganizar_deck_apos_descarte(d[jog_id%4]);
		pthread_mutex_unlock(&mutex_controle_vazio[jog_id%4],NULL);
	}
	else pthread_mutex_lock(&mutex_controle_vazio[jog_id-1],NULL);
	pthread_mutex_unlock(&mutex_acesso[jog_id-1],NULL);

}

void verificar_vitoria(){

}

int escolhar_descarte(){

}

int main(){
	embaralhar();
	sleep(2);
	embaralhar();
	sleep(2);
	embaralhar();

return 0;
}

