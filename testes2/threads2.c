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

/*deck reorganizar_deck_apos_saque(deck d){
	d.fila[d.topo] = -1;
	d.topo--;
	d.n--;
}

deck reorganizar_deck_apos_descarte(deck d){
	d.topo++;
	d.n++;
}*/

void matar_todos(){

}

void sacar_carta(int jog_id, int index_carta){
	int i;
	sem_wait(&sem_controle_vazio[jog_id]);
	sem_wait(&mutex_acesso[jog_id]);
	printf("deck %d antes de saque e baralho do jogador %d...\n",jog_id,jog_id);
	for(i=0;i<3;i++) printf("%d ",decks[jog_id].fila[i]);
	printf("\n");
	for(i=0;i<4;i++) printf("%d ",jog[jog_id].baralho[i]);
	printf("\n");
	if(decks[jog_id].fila[2] != -1){
		printf("jogador %d vai sacar carta %d do deck %d...\n",jog_id,decks[jog_id].fila[2],jog_id);
		jog[jog_id].baralho[index_carta] = decks[jog_id].fila[2];
		decks[jog_id].fila[2] = -1;	
	}else if(decks[jog_id].fila[1] != -1){
		printf("jogador %d vai sacar carta %d do deck %d...\n",jog_id,decks[jog_id].fila[1],jog_id);
		jog[jog_id].baralho[index_carta] = decks[jog_id].fila[1];
		decks[jog_id].fila[1] = -1;
	}else{
		printf("jogador %d vai sacar carta %d do deck %d...\n",jog_id,decks[jog_id].fila[0],jog_id);
		jog[jog_id].baralho[index_carta] = decks[jog_id].fila[0];
		decks[jog_id].fila[0] = -1;
	}
	decks[jog_id].n--;
	decks[jog_id].topo--;
	printf("deck %d e baralho do jogador %d depois de saque...\n",jog_id,jog_id);
	for(i=0;i<3;i++) printf("%d ",decks[jog_id].fila[i]);
	printf("\n");
	for(i=0;i<4;i++) printf("%d ",jog[jog_id].baralho[i]);	
	printf("\n");
	sem_post(&mutex_acesso[jog_id]);
	sem_post(&sem_controle_cheio[jog_id]);
}

void descartar(int jog_id, int index_carta){
	int i;
	sem_wait(&sem_controle_cheio[(jog_id+1)%4]);
	sem_wait(&mutex_acesso[(jog_id+1)%4]);
	printf("deck %d antes de descarte e baralho do jogador %d...\n",(jog_id+1)%4,jog_id);
	for(i=0;i<3;i++) printf("%d ",decks[(jog_id+1)%4].fila[i]);
	printf("\n");
	for(i=0;i<4;i++) printf("%d ",jog[jog_id].baralho[i]);
	printf("\n");
	printf("jogador %d vai descartar carta %d para o deck %d...\n",jog_id,jog[jog_id].baralho[index_carta],(jog_id+1)%4);
	decks[(jog_id+1)%4].fila[2] = decks[(jog_id+1)%4].fila[1];
	decks[(jog_id+1)%4].fila[1] = decks[(jog_id+1)%4].fila[0];
	decks[(jog_id+1)%4].fila[0] = jog[jog_id].baralho[index_carta];
	jog[jog_id].baralho[index_carta] = -1;
	decks[(jog_id+1)%4].n++;
	decks[(jog_id+1)%4].topo++;
	printf("deck %d e baralho do jogador %d depois de descarte...\n",(jog_id+1)%4,jog_id);
	for(i=0;i<3;i++) printf("%d ",decks[(jog_id+1)%4].fila[i]);
	printf("\n");
	for(i=0;i<4;i++) printf("%d ",jog[jog_id].baralho[i]);	
	printf("\n");	
	sem_post(&mutex_acesso[(jog_id+1)%4]);
	sem_post(&sem_controle_vazio[(jog_id+1)%4]);
}

void init_all_sems(){
	int i;
	for(i=0;i<4;i++){
		sem_init(&sem_controle_cheio[i],0,1);
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
	int i,j;
	for (i=0;i<4;i++){
		decks[i].n = 0;
		decks[i].N = 3;
		decks[i].topo = 1;
		decks[i].base = 0;
		for(j=0;j<3;j++) decks[i].fila[j] = -1;
	}

}

void verificar_vitoria(){

}

int escolher_descarte(){
	srand((unsigned)time(NULL));
	return rand()%4;
}

void* thread_func(void *args){
	int  id = *(int*)args;
	int i;
	while(1){
		int carta = escolher_descarte();
		printf("Jogador %d vai jogar agora...Seu deck agora:\n",id);
		for(i=0;i<4;i++) printf("%d ",jog[id].baralho[i]);
		printf("\n");
		descartar(id,carta);
		sacar_carta(id,carta);	
		printf("Deck do jogador %d depois da jogada...\n",id);
		for(i=0;i<4;i++) printf("%d ",jog[id].baralho[i]);
		printf("\n");
		//mostrar deck agora
		
	}

}

int main(){
	pthread_t threads[4];
	int i;
	int *bar =  embaralhar();
	init_all_sems();
	init_all_decks();
	for(i=0;i<4;i++) {jog[0].baralho[i] = *(bar+i); decks[i].base = 0; decks[i].topo = 2;}
	for(i=4;i<8;i++) jog[1].baralho[i%4] = *(bar+i);
	for(i=8;i<12;i++) jog[2].baralho[i%4] = *(bar+i);
	for(i=12;i<16;i++) jog[3].baralho[i%4] = *(bar+i);
	for(i=16;i<18;i++) decks[0].fila[i%2] = *(bar+i);
	for(i=18;i<20;i++) decks[1].fila[i%2] = *(bar+i);
	for(i=20;i<22;i++) decks[2].fila[i%2] = *(bar+i);
	for(i=22;i<24;i++) decks[3].fila[i%2] = *(bar+i);

	for(i=0;i<4;i++){
		pthread_create(&threads[i],NULL,thread_func,(void *)&i);
	}
	for(i=0;i<4;i++){
		pthread_join(threads[i],NULL);
	}

	
return 0;
}

