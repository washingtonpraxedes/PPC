#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct jogador{
	
}jogador;

typedef struct deck{

}deck;

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

void matar_todos(){

}

void sacar_carta(){

}

void descartar(){

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

