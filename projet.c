#include<stdio.h>
#include<stdlib.h>
#define N 78
#define E 69
#define S 83
#define W 87


void affichePlateau(int n, char plateau[n][n]){
	for(int i=0;i<n;++i){
	printf("%d  ",i);
		for(int j=0;j<n;++j){
			printf("%c ",plateau[i][j]);
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
	
void placeJoueur(int n, char plateau[n][n]){
	int x,y;
	printf("Saisir les coordonnees de l'endroit ou vous voulez jouer :\nx = ");
	scanf("%d",&x);
	printf("y = ");
	scanf("%d",&y);
	while(!valideCoord(x,y,n)){
		printf("Coordonnees impossibles ! Veuillez saisir de nouvelles coordonnees 0<x,y<%d :\nx = ",n-1);
		scanf("%d",&x);
		printf("y = ");
		scanf("%d",&y);
	}
	plateau[x][y]='O';
}

int testAlignement(int x, int y, int n, char plateau[n][n], int nbAlignement){
	return alignement(x,y,"N",n,plateau,nbAlignement) || alignement(x,y,"E",n,plateau,nbAlignement) || alignement(x,y,"S",n,plateau,nbAlignement) || 
		   alignement(x,y,"W",n,plateau,nbAlignement) || alignement(x,y,"NE",n,plateau,nbAlignement) || alignement(x,y,"SE",n,plateau,nbAlignement) || 
		   alignement(x,y,"SW",n,plateau,nbAlignement) || alignement(x,y,"NW",n,plateau,nbAlignement);
}

int gagner(int n, char plateau[n][n], int nbAlignement){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			if(plateau[i][j]!='_'){
				if(testAlignement(i,j,n,plateau,nbAlignement)){
					if(plateau[i][j]=='X'){
						printf("\nVous avez perdu !");
						return -1000;
					}
					printf("\nVous avez gagné !");
					return 1000;
				}
			}
		}
	}
	return 0;
}

int main(){
	char plateau[10][10];
	remplitAleatoire(10,plateau);
	char direction[3]="NW";
	int x,y;
	convertitDirection(direction,&x,&y);
	affichePlateau(10,plateau);
	while(gagner(10,plateau,5)==0){	
		placeJoueur(10,plateau);
		affichePlateau(10,plateau);
	}
	
	
	return 0;
}
