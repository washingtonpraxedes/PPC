#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>


typedef struct jogador{
	int baralho[5];
}jogador;

typedef struct deck{
	int fila[2];
	int base,topo,N,n;
}deck;

jogador jog[4];
deck decks[4];
sem_t mutex_acesso[4];
sem_t sem_controle_cheio[4];
sem_t sem_controle_vazio[4];
sem_t mutex_acesso_jogador[4];
sem_t mutex_jogada;
int id_thread_winner=-1;

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
	return vtr1;
}

void sacar_carta(int jog_id){
	int i;
	sem_wait(&sem_controle_cheio[jog_id]);
	sem_wait(&mutex_acesso[jog_id]);
	printf("deck %d antes de saque e baralho do jogador %d...\n",jog_id,jog_id);
	for(i=0;i<2;i++) printf("%d ",decks[jog_id].fila[i]);
	printf("\n");
	for(i=0;i<5;i++) printf("%d ",jog[jog_id].baralho[i]);
	printf("\n");
	if(decks[jog_id].fila[1] != -1){
		printf("jogador %d vai sacar carta %d do deck %d...\n",jog_id,decks[jog_id].fila[1],jog_id);
		jog[jog_id].baralho[4] = decks[jog_id].fila[1];
		decks[jog_id].fila[1] = -1;
	}else{
		printf("jogador %d vai sacar carta %d do deck %d...\n",jog_id,decks[jog_id].fila[0],jog_id);
		jog[jog_id].baralho[4] = decks[jog_id].fila[0];
		decks[jog_id].fila[0] = -1;
	}
	decks[jog_id].n--;
	decks[jog_id].topo--;
	printf("deck %d e baralho do jogador %d depois de saque...\n",jog_id,jog_id);
	for(i=0;i<2;i++) printf("%d ",decks[jog_id].fila[i]);
	printf("\n");
	for(i=0;i<5;i++) printf("%d ",jog[jog_id].baralho[i]);	
	printf("\n");
	sem_post(&mutex_acesso[jog_id]);
	sem_post(&sem_controle_vazio[jog_id]);
}

void descartar(int jog_id, int index_carta){
	int i;
	sem_wait(&sem_controle_vazio[(jog_id+1)%4]);
	sem_wait(&mutex_acesso[(jog_id+1)%4]);
	printf("deck %d antes de descarte e baralho do jogador %d...\n",(jog_id+1)%4,jog_id);
	for(i=0;i<2;i++) printf("%d ",decks[(jog_id+1)%4].fila[i]);
	printf("\n");
	for(i=0;i<5;i++) printf("%d ",jog[jog_id].baralho[i]);
	printf("\n");
	printf("jogador %d vai descartar carta %d para o deck %d...\n",jog_id,jog[jog_id].baralho[index_carta],(jog_id+1)%4);
	decks[(jog_id+1)%4].fila[2] = decks[(jog_id+1)%4].fila[1];
	decks[(jog_id+1)%4].fila[1] = decks[(jog_id+1)%4].fila[0];
	decks[(jog_id+1)%4].fila[0] = jog[jog_id].baralho[index_carta];
	jog[jog_id].baralho[index_carta] = -1;
	decks[(jog_id+1)%4].n++;
	decks[(jog_id+1)%4].topo++;
	for(i=index_carta;i<4;i++) jog[jog_id].baralho[i] = jog[jog_id].baralho[i+1];
	printf("deck %d e baralho do jogador %d depois de descarte...\n",(jog_id+1)%4,jog_id);
	for(i=0;i<2;i++) printf("%d ",decks[(jog_id+1)%4].fila[i]);
	printf("\n");
	for(i=0;i<5;i++) printf("%d ",jog[jog_id].baralho[i]);	
	printf("\n");	
	sem_post(&mutex_acesso[(jog_id+1)%4]);
	sem_post(&sem_controle_cheio[(jog_id+1)%4]);
}

void init_all_sems(){
	int i;
	for(i=0;i<4;i++){
		sem_init(&sem_controle_cheio[i],0,2);
		sem_init(&sem_controle_vazio[i],0,0);
		sem_init(&mutex_acesso[i],0,1);
		sem_init(&mutex_acesso_jogador[i],0,1);
	}
	sem_init(&mutex_jogada,0,1);
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
		decks[i].N = 2;
		decks[i].topo = 1;
		decks[i].base = 0;
		for(j=0;j<2;j++) decks[i].fila[j] = -1;
	}

}

void init_all_jogs(){
	int i,j;
	for (i=0;i<4;i++){
		for(j=0;j<5;j++) jog[i].baralho[j] = -1;
	}

}

int escolher_descarte(){
	srand((unsigned)time(NULL));
	return rand()%4;
}

void reinvindicar_vitoria(int id_jog){
	sem_wait(&mutex_jogada);
		printf("\n\n\n\nCHEGOU AQUI!!!!!!!!!!!!!!!!!!!!!!\nTHREAD_WINNER %d\nTHREAD %d\n",id_thread_winner,id_jog);
		if(id_thread_winner == -1){ 
		id_thread_winner = id_jog;printf("JOGADOR %d VENCEU!!!!\n\n\n",id_thread_winner);
		exit(0);
		}
	sem_post(&mutex_jogada);
}

void* thread_func(void *args){
	int  id = *(int*)args;	
	int i=0;
	int win = 0;
		if(jog[id].baralho[0]%6 == jog[id].baralho[1]%6 && jog[id].baralho[0]%6 == jog[id].baralho[2]%6 && jog[id].baralho[0]%6 == jog[id].baralho[3]%6) {reinvindicar_vitoria(id); pthread_exit(NULL);}
		printf("\n\nThread %d executando...\n\n",id);
		int carta = escolher_descarte();
		printf("Jogador %d vai jogar agora...Seu deck agora:\n%d %d %d %d %d",id,jog[id].baralho[0],jog[id].baralho[1],jog[id].baralho[2],jog[id].baralho[3],jog[id].baralho[4]);
		printf("\n");
		sacar_carta(id);	
		descartar(id,carta);
		printf("Deck do jogador %d depois da jogada...\n%d %d %d %d %d",id,jog[id].baralho[0],jog[id].baralho[1],jog[id].baralho[2],jog[id].baralho[3],jog[id].baralho[4]);
		printf("\n");
		win = 0;
		if(jog[id].baralho[0]%6 == jog[id].baralho[1]%6 && jog[id].baralho[0]%6 == jog[id].baralho[2]%6 && jog[id].baralho[0]%6 == jog[id].baralho[3]%6) {reinvindicar_vitoria(id); pthread_exit(NULL);}
	i++;
}

int main(){
	pthread_t threads[4];
	int i,j=0;
	int *bar =  embaralhar(24);
	printf("deck ");
	for(i=0;i<24;i++) {if(!(i%4)) printf("|");printf("%d ",bar[i]);}
	printf("\n");
	init_all_sems();
	init_all_decks();
	init_all_jogs();
	for(i=0;i<4;i++) {jog[0].baralho[i] = *(bar+i); decks[i].base = 0; decks[i].topo = 2;}
	for(i=4;i<8;i++) jog[1].baralho[i%4] = *(bar+i);
	for(i=8;i<12;i++) jog[2].baralho[i%4] = *(bar+i);
	for(i=12;i<16;i++) jog[3].baralho[i%4] = *(bar+i);
	for(i=16;i<18;i++) decks[0].fila[i%2] = *(bar+i);
	for(i=18;i<20;i++) decks[1].fila[i%2] = *(bar+i);
	for(i=20;i<22;i++) decks[2].fila[i%2] = *(bar+i);
	for(i=22;i<24;i++) decks[3].fila[i%2] = *(bar+i);
	int *vtr;
	while(1){
		printf("\n\n\n");
		vtr = embaralhar(4);	
		printf("\n\n");
		for(i=0;i<4;i++){
			if(!pthread_create(&threads[*(vtr+i)],NULL,thread_func,(void *)(vtr+i))) printf("Iniciando thread %d\n",*(vtr+i));
			else printf("Falhou em criar a thread %d\n",i);
			//pthread_join(threads[*(vtr+i)],NULL);
		}
		usleep(100000);	
	}
return 0;
}
