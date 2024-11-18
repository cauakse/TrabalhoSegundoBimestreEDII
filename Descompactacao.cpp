#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include <malloc.h>


struct tree {
    struct tree *esq, *dir;
    int freq;
    int symbol;
};
typedef struct tree Tree;

struct info {
    int simbolo, freq;
    int hcode[256],TL;
    char palavra[100];
};
typedef struct info Info;

struct tabela {
    struct tabela *prox;
    Info info;
};
typedef struct tabela Table;

struct bits {
	unsigned char b7:1;
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
	unsigned char b0:1;
};

union buty{
	struct bits bi;
	unsigned char num;
};



void quadrado(int ci,int li,int cf,int lf,int cor)
{
	textcolor(cor);
	gotoxy(ci,li);
        printf("%c",201);
    gotoxy(ci,lf);
        printf("%c",200);
    gotoxy(cf,li);
        printf("%c",187);
    gotoxy(cf,lf);
        printf("%c",188);
        
    for(int i=ci+1;i<cf;i++)
    {
    	gotoxy(i,li);
    	printf("%c",205);
    	gotoxy(i,lf);
    	printf("%c",205);
    }
    for(int i=li+1;i<lf;i++)
    {
    	gotoxy(ci,i);
    	printf("%c",186);
    	gotoxy(cf,i);
    	printf("%c",186);
    }
    textcolor(15);
}


void inicioTOP()
{
	gotoxy(2,2);
	printf("Algoritmo para descompactacao de textos");
	textcolor(0);
	textbackground(15);
	quadrado(1,1,120,40,4);
	textcolor(15);
	textbackground(0);
	int i,j,k;
	char frase [23];
	strcpy(frase,"HUFFMAN'S TREE\0");
	for(i=0;i<strlen(frase);i++)
	{
		for(j=strlen(frase);j>i;j--)
		{
			gotoxy(j+51+1,12);printf(" ");
			gotoxy(j+51,12);
			for(k=0;k<8;k++)
			{
				printf("%c",rand()%26+64);
				gotoxy(j+51,12);
			}
			gotoxy(j+51+1,12);printf(" ");
			Sleep(5);
		}
		gotoxy(i+51+1,12);printf(" ");
		gotoxy(i+51,12);
		printf("%c",frase[i]);
	}
	while(!kbhit())
	{
		Sleep(500);
		gotoxy(45,20);
		printf("PRESS ANY KEY TO CONTINUE");
		Sleep(500);
		gotoxy(45,20);
		printf("                          ");
	}

	system("cls");
}


void insertInTable(Table ** tab, Info info){
	Table *aux;
	if(*tab==NULL){
		*tab = (Table *) malloc(sizeof(Table));
		(*tab)->prox=NULL;
		(*tab)->info=info;
	}
	else
	{
		aux = *tab;
		while(aux->prox!=NULL){
			aux=aux->prox;
		}
		
		aux->prox = (Table *) malloc(sizeof(Table));
		aux->prox->prox=NULL;
		aux->prox->info=info;
	}
	
}

void showTree(Tree *tree, int x, int y, int espacoHorizontal) {
    if (tree != NULL) {
        gotoxy(x,y);
        printf("(%d)", tree->symbol);

        if (tree->esq != NULL) {
            int novoXEsq = x - espacoHorizontal; // Nova coordenada x para o galho esquerdo
            int novoYEsq = y + 1; // Nova coordenada y para o galho esquerdo
            for (int i = x; i > novoXEsq; i--) {
                gotoxy(i, novoYEsq);
                printf("-");
            }

            int novoYFilhoEsq = novoYEsq + 2; // Nova coordenada y para o filho esquerdo
            gotoxy(novoXEsq, novoYFilhoEsq);
            printf("/");

            int novoEspacoHorizontalEsq = espacoHorizontal / 2; // Novo espaçamento horizontal para o próximo nó
            showTree(tree->esq, novoXEsq, novoYFilhoEsq + 1, novoEspacoHorizontalEsq);
        }

        if (tree->dir != NULL) {
            int novoXDir = x + espacoHorizontal; // Nova coordenada x para o galho direito
            int novoYDir = y + 1; // Nova coordenada y para o galho direito
            for (int i = x; i < novoXDir; i++) {
                gotoxy(i, novoYDir);
                printf("-");
            }

            int novoYFilhoDir = novoYDir + 2; // Nova coordenada y para o filho direito
            gotoxy(novoXDir, novoYFilhoDir);
            printf("\\");

            int novoEspacoHorizontalDir = espacoHorizontal / 2; // Novo espaçamento horizontal para o próximo nó
            showTree(tree->dir, novoXDir, novoYFilhoDir + 1, novoEspacoHorizontalDir);
        }
    }
}


void createTreeByTable(Table ** table, Tree ** tree){
	FILE * ptr = fopen("tableRegister.dat","rb");
	Table * tab=NULL,*aux;
	Tree * tre,*taux;
	Info info;
	int i;
	if(ptr!=NULL){
		fread(&info,sizeof(Info),1,ptr);
		while(!feof(ptr)){
			insertInTable(&tab,info);
			fread(&info,sizeof(Info),1,ptr);
		}
		fclose(ptr);
		
		tre = (Tree*) malloc(sizeof(Tree));
		tre->dir=NULL;
		tre->esq=NULL;
		taux=tre;
		aux=tab;
			
		while(aux!=NULL){
			tre=taux;
			
			for(i=0;i<aux->info.TL;i++){
				if(aux->info.hcode[i]==0)
				{
					if(tre->esq==NULL){
						tre->symbol=-1;
						tre->esq=(Tree *) malloc(sizeof(Tree));
						tre->esq->esq=NULL;
						tre->esq->dir=NULL;
					}
					
					tre=tre->esq;
				}
				else
				{
					if(tre->dir==NULL){
						tre->symbol=-1;
						tre->dir=(Tree *) malloc(sizeof(Tree));
						tre->dir->esq=NULL;
						tre->dir->dir=NULL;
					}
					tre=tre->dir;
				}
			}
			
			tre->symbol=aux->info.simbolo;
			
			aux=aux->prox;	
		}
		
		*tree=taux;
		*table=tab;
	}
}

void concatene(char frase[10000],int s,Table *t){
	int i;
	while(t!=NULL && t->info.simbolo != s){
		t=t->prox;
	}
	
	if(t!=NULL){
		if(strcmp(t->info.palavra," ") == 0)
		{
			i=strlen(frase);
			frase[i]=' ';
			frase[i+1]='\0';
		}
		else
		strcat(frase,t->info.palavra);
	}
}

void decodeSentence(Table * table, Tree * tree, char f [10000],int c[10000],int *te){
	
	int TL=0,code[10000],i,minus;
	union buty b;
	Tree *aux;
	
	FILE * ptr = fopen("codifyWords.dat","rb");
	FILE *ptrA = fopen("bitesPlus.dat","rb");
	
	if(ptrA!=NULL){
		fread(&minus,sizeof(int),1,ptrA);
		fclose(ptrA);
	}

	f[0]='\0';
	
	if(ptr!=NULL){
		fread(&b.num,sizeof(char),1,ptr);
		while(!feof(ptr)){
			//retirar do byte os bits e transformar pra inteiro
			code[TL++] = (int)b.bi.b0;
			code[TL++] = (int)b.bi.b1;
			code[TL++] = (int)b.bi.b2;
			code[TL++] = (int)b.bi.b3;
			code[TL++] = (int)b.bi.b4;
			code[TL++] = (int)b.bi.b5;
			code[TL++] = (int)b.bi.b6;
			code[TL++] = (int)b.bi.b7;
			fread(&b.num,sizeof(char),1,ptr);
		}

		for(i=0;i<TL-minus+1;i++)
			c[i]=code[i];
			
		*te=TL-minus+1;
		
		aux=tree;
		for(i=0;i<TL-minus+1;i++){
			if(aux->dir==NULL && aux->esq==NULL){
				concatene(f,aux->symbol,table);
				aux=tree;
			}
			
			if(code[i]==0){
				aux=aux->esq;
			}
			else{
				if(code[i]==1)
					aux=aux->dir;
			}
		}
		fclose(ptr);
	}
	
}

char menu(char frase[10000],int deco[10000],int TL){
	system("cls");
	fflush(stdin);
	int i, j,k,l=0;
	textbackground(4);
	textcolor(11);
	for(i=0;i<120;i++){
		for(j=0;j<40;j++){
			gotoxy(i,j);
			printf(" ");
		}
	}
	
	gotoxy(35,1);
	printf("Frase ja Decodificada:");
	i=strlen(frase)/100;
	quadrado(10,2,100,6+i,11);
	for(k=3;k<5+i && l<strlen(frase);k++){
		for(j=11;j<100 && l<strlen(frase);j++){
			gotoxy(j,k);
			printf("%c", frase[l]);
			l++;
		}
	}
	
	quadrado(9,7+i,100,21,14);
	gotoxy(10,8+i);
	printf("Digite a opcao que deseja");
	gotoxy(10,9+i);
	printf("A - Exibir Arvore de Huffman");
	gotoxy(10,14+i);
	printf("ESC - Sair do programa");
	
	j=TL/89;
	quadrado(9,22,100,24+j,14);
	gotoxy(10,23);
	for(k=0;k<TL;k++){
		printf("%d",deco[k]);
		if(k>0 && k%89==0)
			gotoxy(10,23+j++);
	}

	
	textbackground(0);
	textcolor(14);
	return getch();
}


int main (){
	Tree * huffmanTree = NULL;
	Table * table;
	char sentence[10000],op;
	int deco[10000], TL;	
	createTreeByTable(&table,&huffmanTree);
	decodeSentence(table,huffmanTree,sentence,deco,&TL);
	//printf("%s",sentence);
	inicioTOP();
	scanf("%c",&op);
	
	do
	{
		op=toupper(menu(sentence,deco,TL));
		
		switch(op){
			case 'A':
				system("cls");
				showTree(huffmanTree,65,8,45);
				getch();
				break;
			case 27:
				break;
			default:
				break;
		}
	}while(op!=27);

}
