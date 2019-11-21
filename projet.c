#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
//direction
#define N 78
#define E 69
#define S 83
#define W 87
//color
#define GRN   "\x1B[32m"
#define MAG   "\x1B[35m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

typedef struct token {
	int aX;
	int aY;
	char aPlayer;
}Tokens;

Tokens generer_token(int pX, int pY){
	Tokens elem;
	elem.aX=pX;
	elem.aY=pY;
	elem.aPlayer='_';
	return elem;
}

Tokens token_Hasard(int n, char plateau[n][n]){
	Tokens t=generer_token(rand()%n,rand()%n);
	if(plateau[t.aX][t.aY]!='_')
		return token_Hasard(n,plateau);
	return t;
}

void affichePlateau(int n, char plateau[n][n]){
	
	for(int i=0; i < n; ++i){
		if(i==0)
			printf(BLU "  |"RESET);
		if(i<10)
			printf(BLU " %d|"RESET,i);
		if(i>=10)
			printf(BLU "%d|"RESET,i);
	}
		
	printf("\n");
	for(int i=0;i<n;++i){
		if(i<10)
			printf(BLU " %d|"RESET,i);
		if(i>=10)
			printf(BLU "%d|"RESET,i);
		//debut de ligne
		for(int j=0;j<n;++j){

			printf(" %c|",plateau[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void remplitAleatoire(int n, char plateau[n][n]){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			plateau[i][j]='_';
		}
	}
}

void convertitDirection(char direction[3], int* x, int* y){
	*x=0;
	*y=0;
	for(int i=0;direction[i]!='\0';++i){
		if(direction[i]==N)
			*y=-1;
		else if(direction[i]==S)
			*y=1;
		else if(direction[i]==E)
			*x=1;
		else if(direction[i]==W)
			*x=-1;
	}
}		

int valideCoord(int x, int y, int n){
	if(x<0 || x>=n || y<0 || y>=n)
		return 0;
	return 1;
}

int emplacementVide(int n, char plateau[n][n], int x, int y){
	if(plateau[x][y]=='_')
		return 1;
	return 0;
}

int alignement(int x, int y, char direction[3], int n, char plateau[n][n], int nbAlignement){
	int dX,dY;
	convertitDirection(direction,&dX,&dY);
	char motif=plateau[x][y];
	for(int i=1;i<nbAlignement;i++){
		if(motif!=plateau[x+dX*i][y+dY*i] || !valideCoord(x+dX*i,y+dY*i,n))
			return 0;
	}
	return 1;
}

int testAlignement(int x, int y, int n, char plateau[n][n], int nbAlignement){
	return alignement(x,y,"N",n,plateau,nbAlignement) || alignement(x,y,"E",n,plateau,nbAlignement) || alignement(x,y,"S",n,plateau,nbAlignement) || 
		   alignement(x,y,"W",n,plateau,nbAlignement) || alignement(x,y,"NE",n,plateau,nbAlignement) || alignement(x,y,"SE",n,plateau,nbAlignement) || 
		   alignement(x,y,"SW",n,plateau,nbAlignement) || alignement(x,y,"NW",n,plateau,nbAlignement);
}
	
void placeJoueur(int n, char plateau[n][n]){
	int x,y;
	printf("Saisir les coordonnees de l'endroit ou vous voulez jouer :\nx = ");
	scanf("%d",&x);
	printf("y = ");
	scanf("%d",&y);
	while(!valideCoord(x,y,n) || !emplacementVide(n,plateau,x,y)){
		printf("Coordonnees impossibles ! Veuillez saisir de nouvelles coordonnees 0<=(x,y)<=%d :\nx = ",n-1);
		scanf("%d",&x);
		printf("y = ");
		scanf("%d",&y);
	}
	plateau[x][y]='O';
}

int gagnerAux(int n, char plateau[n][n], int nbAlignement, char joueur){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			if(plateau[i][j]==joueur){
				if(testAlignement(i,j,n,plateau,nbAlignement)){
					return 1;
				}
			}
		}
	}
	return 0;
}

int gagner(int n, char plateau[n][n], int nbAlignement){
	if(gagnerAux(n,plateau,nbAlignement,'X')==1){
		printf("\nVous avez perdu !");
		return -1000;
	}
	if(gagnerAux(n,plateau,nbAlignement,'O')==1){
		printf("\nVous avez gagné !");
		return 1000;
	}
	return 0;
}

void placeHasardAux(int n, char plateau[n][n], Tokens *t){
	int x=rand()%n;
	int y=rand()%n;
	while(!emplacementVide(n,plateau,x,y)){
		x=rand()%n;
		y=rand()%n;
	}
	plateau[x][y]='X';
	(*t).aX=x;
	(*t).aY=y;
}


int possible(int n, char plateau[n][n], int nbAlignement, Tokens *t){
	for(int i=(*t).aX-nbAlignement+1;i<(*t).aX+nbAlignement;i++){
		for(int j=(*t).aX-nbAlignement+1;j<(*t).aX+nbAlignement;j++){
			if(plateau[i][j]=='_')
				return 1;
		}
	}
	return 0;
}

void placeHasard(int n, char plateau[n][n], Tokens *t, int nbAlignement){
	if((*t).aX==-1){
		placeHasardAux(n,plateau,&(*t));
		return;
	}
	if(possible(n,plateau,nbAlignement,&(*t))){
		int x=(*t).aX-nbAlignement+1+rand()%(2*nbAlignement-1);
		int y=(*t).aX-nbAlignement+1+rand()%(2*nbAlignement-1);
		while(!emplacementVide(n,plateau,x,y)){
			x=(*t).aX-nbAlignement+1+rand()%(2*nbAlignement-1);
			y=(*t).aX-nbAlignement+1+rand()%(2*nbAlignement-1);
		}
		plateau[x][y]='X';
		printf("%d %d\n", x,y);
		(*t).aX=x;
		(*t).aY=y;
		return;

	}
	placeHasardAux(n,plateau,&(*t));
}



int placeIAAux(int n, char plateau[n][n], int nbAlignement, char joueur1, char joueur2, Tokens *t){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			if(plateau[i][j]=='_'){
				plateau[i][j]=joueur1;
				if(gagnerAux(n,plateau,nbAlignement,joueur1)==1){
					*t=generer_token(i,j);
					return 1;
				}
				plateau[i][j]=joueur2;
				if(gagnerAux(n,plateau,nbAlignement,joueur2)==1){
					plateau[i][j]=joueur1;
					return 2;
				}
				plateau[i][j]='_';
			}
		}
	}
	return 3;
}

void placeIA(int n, char plateau[n][n], int nbAlignement, Tokens *tok){
	Tokens t=generer_token(-1,-1);
	if(placeIAAux(n,plateau,nbAlignement,'X','O',&t)==3){ //coup de l'ia n1
		for (int i = 0; i < n; ++i){
			for (int j = 0; j < n; ++j){
				if(plateau[i][j]=='_'){
					plateau[i][j]='X';
					if(placeIAAux(n,plateau,nbAlignement,'X','O',&t)==1){ //pion placé on arrete
						plateau[t.aX][t.aY]='_';
						(*tok).aX=i;
						(*tok).aY=j;
						return;
					}
					if(placeIAAux(n,plateau,nbAlignement,'O','X',&t)==3){//on regarde si le joueur quand est ce que le joueur peut gagner au tour d'apres qu'il joue
						plateau[i][j]='O';
						if(placeIAAux(n,plateau,nbAlignement,'O','X',&t)==1){
							plateau[i][j]='_';
							plateau[t.aX][t.aY]='X';
							(*tok).aX=t.aX;
							(*tok).aY=t.aY;							
							return;
						}
					}
					plateau[i][j]='_';
				}
			}
		}
		placeHasard(n,plateau,&(*tok),nbAlignement);
	}
}

int main(){
	srand(time(NULL));
	Tokens tok=generer_token(-1,-1);
	int n=0;
	int alignement=0;
	while(n<3 || n>15){
		printf("choisir la taille du plateau:");
		scanf("%d",&n);
	}
	while(alignement<2 || alignement>n){
		printf("choisir un nombre de pion a alignement:");
		scanf("%d",&alignement);
	}
	char plateau[n][n];
	remplitAleatoire(n,plateau);
	char direction[3]="NW";
	int x,y;
	convertitDirection(direction,&x,&y);
	affichePlateau(n,plateau);
	while(gagner(n,plateau,alignement)==0){	
		placeJoueur(n,plateau);
		affichePlateau(n,plateau);
		if(gagner(n,plateau,alignement)!=0)
			break;
		sleep(1);
		placeIA(n,plateau,alignement,&tok);
		affichePlateau(n,plateau);
	}
	
	
	return 0;
}

