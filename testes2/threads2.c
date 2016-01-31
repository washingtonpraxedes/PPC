#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>


typedef struct jogador{
	int baralho[4];
}jogador;

typedef struct deck{
	int fila[3];
	int base,topo,N,n; //base = ultima carta / topo = primeira carta / N = número máximo de elementos / n = número de cartas presentes
}deck;

jogador jog[4];
deck decks[4];
sem_t mutex_acesso[4];
sem_t sem_controle_cheio[4];
sem_t sem_controle_vazio[4];

int* embaralhar(){
	srand((unsigned)time(NULL));
	int i,aux, j, t;
	int *vtr1 = malloc(24*sizeof(int));
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
	return vtr1;
}

deck reorganizar_deck_apos_saque(deck d){
	d.fila[d.topo] = -1;
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
	sem_wait(&mutex_acesso[jog_id-1]);
	jog[jog_id-1].baralho[index_carta] = decks[jog_id-1].fila[decks[jog_id-1].topo];
	decks[jog_id-1] = reorganizar_deck_apos_saque(decks[jog_id-1]);
	sem_post(&mutex_acesso[jog_id-1]);
	sem_post(&sem_controle_vazio[jog_id-1]);
}

void descartar(int jog_id, int index_carta){
	sem_wait(&sem_controle_vazio[jog_id%4]);
	sem_wait(&mutex_acesso[jog_id%4]);
	decks[jog_id%4].fila[decks[jog_id%4].base] = jog[jog_id%4].baralho[index_carta];
	decks[jog_id%4] = reorganizar_deck_apos_descarte(decks[jog_id%4]);		
	sem_post(&mutex_acesso[jog_id%4]);
	sem_post(&sem_controle_cheio[jog_id%4]);
}

void init_all_sems(){
	int i;
	for(i=0;i<4;i++){
		sem_init(&sem_controle_cheio[i],0,0);
		sem_init(&sem_controle_vazio[i],0,2);
		sem_init(&mutex_acesso[i],0,1);
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

int escolher_descarte(){
	srand((unsigned)time(NULL));
	return rand()%4;
}

void* thread_func(void *args){

}

int main(){
	int i;
	int *bar =  embaralhar();
	for(i=0;i<4;i++) {jog[0].baralho[i] = *(bar+i); decks[i].base = 0; decks[i].topo = 2;}
	for(i=4;i<8;i++) jog[1].baralho[i%4] = *(bar+i);
	for(i=8;i<12;i++) jog[2].baralho[i%4] = *(bar+i);
	for(i=12;i<16;i++) jog[3].baralho[i%4] = *(bar+i);
	for(i=16;i<18;i++) decks[0].fila[i%2] = *(bar+i);
	for(i=18;i<20;i++) decks[1].fila[i%2] = *(bar+i);
	for(i=20;i<22;i++) decks[2].fila[i%2] = *(bar+i);
	for(i=22;i<24;i++) decks[3].fila[i%2] = *(bar+i);

	
return 0;
}

