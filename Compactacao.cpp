#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>

struct tree {
    struct tree *esq, *dir;
    int freq;
    int symbol;
};
typedef struct tree Tree;

struct florest {
    Tree *tree;
    struct florest *prox;
};
typedef struct florest Florest;

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


void addTable(Table **table, char palavra[100]) { //adicionar/aumentar a frequencia de uma palavra na tabela
    int i = 1;
    Table *aux = *table; 
    if (*table == NULL) { //se nao tiver nenhuma palavra ainda
        *table = (Table *)malloc(sizeof(Table)); //reservo na memoria um endereço do tamanho de Table
        (*table)->prox = NULL; //como é a primeira ent o prox é null
        (*table)->info.simbolo = 0; //vai ser o simbolo 0 pq é a primeira palavra da tabela
        (*table)->info.freq = 1; //comeca a frequencia como 1 pq ja apareceu uma vez
        strcpy((*table)->info.palavra, palavra); //copio o palavra pra tabela
    } else { //se nao for a primeira
        while (aux->prox != NULL && strcmpi(aux->info.palavra, palavra) != 0) { //enquanto nao chegar no fim ou
            aux = aux->prox;//ando pro prox o ponteiro atual da tabela			//eu nao achar uma palavra igual
            i++;//aumento o valor do codigo de acordo com o indice na tabela
        }

        if (strcmpi(aux->info.palavra, palavra) == 0) { // se eu achei uma palavra igual
            aux->info.freq++; // adiciono um na frequencia
        } else { //se nao achei uma palavra igual
            aux->prox = (Table *)malloc(sizeof(Table));// salvo na memoria um espaço pra novo indice de table
            aux->prox->prox = NULL; //o prox do novo endereco recebe null
            aux->prox->info.simbolo = i; //coloco o simbolo pelo indice que calculei no while
            aux->prox->info.freq = 1; //coloco a freq como 1 pq é nova palavra
            strcpy(aux->prox->info.palavra, palavra); //copio a palavra
        }
    }
}

void showTable(Table *table) { //exibo a tabela
    while (table != NULL) {
        printf("%s FREQ=%i | ", table->info.palavra, table->info.freq);
        table = table->prox;
    }
}

void fillTableWithFrequency(Table **table, char frase[]) {
    int i, j, tam = strlen(frase);
    char palavra[100];
    for (i = 0; i < tam; i++) //ando na frase até acabar 
	{
        if (i > 0 && frase[i - 1] == ' ') //caso nao for o primeiro elemento e se for um espaço o ultimo elemento lido
            addTable(table, " "); //adiciono na tabela mais um na frquencia do espaço

        for (j = 0; i < tam && frase[i] != ' '; i++) {
            palavra[j++] = frase[i]; //separo palavra por palavra
        }
        palavra[j] = '\0';
        
        addTable(table, palavra); //adiciono/ aumento a frequencia da palavra escrita
    }
}

Tree *createTree(Tree *e, int s, int freq, Tree *d) {
    Tree *aux = (Tree *)malloc(sizeof(Tree));
    aux->esq = e;
    aux->dir = d;
    aux->freq = freq;
    aux->symbol = s;
    return aux;
}

void createFlorest(Florest **flo, Table *table) { //vou criar a floresta apenas com as folhas com o simbolo das palavras e a frequencia das mesmas
    Florest *aux, *ant, *nc;
    while (table != NULL) { //enquanto tiver palavras 
        if (*flo == NULL) { //verifico se é o primeiro
            *flo = (Florest *)malloc(sizeof(Florest)); //aloco na memoria espaço pra Florest
            (*flo)->prox = NULL; //falo que o prox é null, afinal é o primeiro
            (*flo)->tree = createTree(NULL, table->info.simbolo, table->info.freq, NULL); //adiciono a nova arvore
        } else {
            nc = (Florest *)malloc(sizeof(Florest)); //aloco na memoria espaço pra Florest
            nc->prox = NULL; //prox é null
            nc->tree = createTree(NULL, table->info.simbolo, table->info.freq, NULL);//coloco a arvore no novo nó da floresta

			//INSERIR ORDENADO

            if ((*flo)->tree->freq < nc->tree->freq) {//verifico é no primeiro que vou iserir
                nc->prox = *flo; //prox recebe o primeiro
                *flo = nc; //o primeiro recebe o novo
            } else {
                aux = (*flo)->prox; //inicio para busca
                ant = *flo; //inicio para busca

                while (aux != NULL && aux->tree->freq > nc->tree->freq) { //enquanto nao acabar ou achar uma frequencia menor
                    ant = aux; //ant vai pro atual do aux
                    aux = aux->prox; //aux anda 1
                }

                if (aux == NULL) { //verifico se é no final que vou iserir
                    ant->prox = nc; //coloco no ant pq aux é null, ou seja inseri na ultima posicao
                } else { //vou inserir no meio
                    nc->prox = aux;  //prox do novo é o aux que tem frequencia menor ou igual
                    ant->prox = nc;  //ant que antes apontava para aux agr aponta pro novo
                }
            }
        }
        table = table->prox; //ando um na tabela de palavras/simbolos/frequencia
    }
}

void exibeFloresta(Florest *f) { //exibo a floresta
    while (f != NULL) {
        printf("%d %i\n", f->tree->symbol, f->tree->freq);
        f = f->prox;
    }
}

void insertTree(Florest **flo, Florest *newf) {
    Florest *aux, *ant;
    if (*flo == NULL) {//veririco se o unico "nunca vai ser pq quando é o ultimo eu coloco direto no *florest hehe"
        *flo = newf;//coloco em primeiro da floresta
    } else {
        aux = (*flo)->prox; //comeco do segundo
        ant = *flo; //começo do primeiro
        while (aux != NULL && aux->tree->freq > newf->tree->freq) { //enquanto nao chegar no fim ou uma frequencia menor ou igual
            ant = aux; //ant pro aux
            aux = aux->prox; //aux pro prox
        }

        if (aux == NULL) { //verifico se é no final que vou inserir
            ant->prox = newf; //o ant é o ultimo ent o prox dele recebe o novo
        } else { //se for no meio
            newf->prox = aux; //novo prox recebe aux que possui frequencia menor ou igual
            ant->prox = newf; //ant que apontava pro aux agora aponta pro novo que por si aponta pro aux
        }
    }
}

void createHuffmanTree(Florest **florest) { //vou reduzir a floresta a apenas um nó que contem a estrutura de huffman
    Florest *aux, *ant, *novo, *ant2;
    Tree *td, *te;
    
    while ((*florest)->prox != NULL) { //enquanto tiver mais que um
        if ((*florest)->prox->prox != NULL) { //se existir mais que 2
        
            aux = (*florest)->prox; //começo do prox ja pq sei que existe mais que um pelo while
            ant = ant2 = *florest; //começo o anterior, e o ant do ant que vai receber a reducao de dois nos para um

            while (aux->prox != NULL) { //até chegar nos dois ultimos nós
                if (ant2 != ant) //se for a primeira repetição nao ando o ant2
                    ant2 = ant2->prox;
                ant = aux; //ando ant
                aux = aux->prox; //ando aux
            }

            td = ant->tree; //pego a arvore da direita que esta em ant
            te = aux->tree; //pego a arvore da esquerda que esta em aux

            novo = (Florest *)malloc(sizeof(Florest)); //crio nova floresta
            novo->prox = NULL; //proximo recebe null
            novo->tree = createTree(td, -1, td->freq + te->freq, te); //coloco uma nova arvore com simbolo -1 pq nao é folha, e o dir e esq recebe as arvores dos dois ultimos nós da floresta

            free(aux); //libero o ultimo no
            free(ant); //libero o penultimo no
            ant2->prox = NULL; //faço o anterior do ant receber null para nao dar erro na repeticao

            insertTree(&(*florest), novo); // insiro o novo elemento de forma ORDENADA na floresta
            
        } else { //caso nao tenha mais que dois
            ant = *florest; //recebo o primeiro
            aux = (*florest)->prox; //recebo o segundo

            td = ant->tree; //recebo a arvore do primeiro
            te = aux->tree; //recebo a arvore do segundo

            novo = (Florest *)malloc(sizeof(Florest)); //crio novo nó
            novo->prox = NULL; //prox é null pq ele é a arvore de huffman completa
            novo->tree = createTree(td, -1, td->freq + te->freq, te);//crio o ultimo nó com dir e esq tendo as duas ultimas arvores da floresta

            free(ant);//libero o ultimo
            free(aux);//libero o penultimo
            *florest = novo;//coloco no começo da floresta o ultimo nó
        }
    }
}

void exibeh(Tree *A) { //exibir arvore de huffman
    static int n = -1;
    if (A == NULL)
        return;
    n++;
    exibeh(A->dir);
    for (int i = 0; i < 5 * n; i++)
        printf(" ");
    printf("(%d, %d)\n", A->symbol, A->freq);
    exibeh(A->esq);
    n--;
}

void puthuff(Tree * tree, Table ** tab, int cod[],int i){
	int j=0;
	Table * aux;
	if(tree!=NULL){
		if(tree->esq==NULL && tree->dir == NULL){
			//buscar na tabela e colocar o indice
			
			aux=*tab;
			while(aux!=NULL && aux->info.simbolo != tree->symbol)
				aux = aux->prox;
			
			if(aux!=NULL)
			{
					for(j=0;j<i;j++){
						aux->info.hcode[j] = cod[j];
					}
					aux->info.TL = i;
			}
			else
			{
				printf("ERRO, SIMBOLO NAO ENCONTRADO NA TABELA\n");
			}
			
		}
		else
		{
			cod[i]=0;
			puthuff(tree->esq,&(*tab),cod,i+1);
			cod[i]=1;
			puthuff(tree->dir,&(*tab),cod,i+1);
		}
	}
}

void putHuffCodeInTable(Tree * tree,Table ** tab){
	int cod[256];
	puthuff(tree,&(*tab),cod,0);
}

void showFullTable(Table *tab) {
    system("cls");

    printf("\n%-10s | %-10s | %-20s | %-15s\n", "Simbolo", "Frequencia", "Huffman Code", "Palavra");
    printf("---------------------------------------------------------------\n");

    while (tab != NULL) {
        printf("%-10d | %-10d | ", tab->info.simbolo, tab->info.freq);
        for (int j = 0; j < tab->info.TL; j++) {
            printf("%d", tab->info.hcode[j]);
        }
        printf("%-*s | %-15s\n", 20 - tab->info.TL, "", tab->info.palavra);

        tab = tab->prox;
    }

    printf("---------------------------------------------------------------\n");
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
	printf("Algoritmo para compactacao de textos");
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

char menu(char frase[10000],int codify[10000],int TL){
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
	gotoxy(30,1);
	printf("Frase a ser Codificada:");
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
	gotoxy(10,10+i);
	printf("B - Exibir Tabela");
	gotoxy(10,14+i);
	printf("ESC - Sair do programa");
	
	j=TL/89;
	quadrado(9,22,100,24+j,14);
	gotoxy(10,23);
	for(k=0;k<TL;k++){
		printf("%d",codify[k]);
		if(k>0 && k%89==0)
			gotoxy(10,23+j++);
	}

	
	textbackground(0);
	textcolor(14);
	return getch();
}

void saveInDiscTable(Table *table){
	FILE * ptr = fopen("tableRegister.dat","wb");
	while(table!=NULL){
		fwrite(&table->info,sizeof(Info),1,ptr);
		table=table->prox;
	}
	
	fclose(ptr);
}

void foundHuffCode (Table *t,char p[100], int h[256],int * TL){
	int i,j;
	while(t!=NULL && strcmpi(p,t->info.palavra)!=0){
		t=t->prox;
	}
	
	if(t!=NULL){
		for(i=0;i<t->info.TL;i++){
			h[i]=t->info.hcode[i];
		}
		
		*TL=t->info.TL;
	}
}

int codifyAndSaveInDisc(Table *table, Tree * tree, int s[10000],char f[10000]){
	char byte[8],palavra[100];
	int huffcode[256] , TL=0,tam=strlen(f),cTL=0;
	union buty b;
	int i,j=0,k=0,l;
	FILE * ptr = fopen("codifyWords.dat","wb");
	
    for (i = 0; i < tam; i++) //ando na frase até acabar 
	{
       
    	for (j = 0; i < tam && f[i] != ' '; i++) {
            palavra[j++] = f[i]; //separo palavra por palavra
        }
        palavra[j] = '\0';
        
        //adicionar palavra
        foundHuffCode(table, palavra, huffcode,&TL);
        
        for(l=0;l<TL;l++){
        	byte[k++]=huffcode[l];
        	s[cTL++]=huffcode[l];
        	if(k==8){
        		//descarregar buffer
        		b.bi.b0=byte[0];
        		b.bi.b1=byte[1];
        		b.bi.b2=byte[2];
        		b.bi.b3=byte[3];
        		b.bi.b4=byte[4];
        		b.bi.b5=byte[5];
        		b.bi.b6=byte[6];
        		b.bi.b7=byte[7];
        		fwrite(&b.num,sizeof(char),1,ptr);
        		k=0;
        	}
        }
        
        if (f[i] == ' ') //caso nao for o primeiro elemento e se for um espaço o ultimo elemento lido
        {
        	//adicionar espaço
            foundHuffCode(table, " ", huffcode,&TL);

	        for(l=0;l<TL;l++){
	        	byte[k++]=huffcode[l];
	        	s[cTL++]=huffcode[l];
	        	if(k==8){
	        		//descarregar buffer
	        		b.bi.b0=byte[0];
	        		b.bi.b1=byte[1];
	        		b.bi.b2=byte[2];
	        		b.bi.b3=byte[3];
	        		b.bi.b4=byte[4];
	        		b.bi.b5=byte[5];
	        		b.bi.b6=byte[6];
	        		b.bi.b7=byte[7];
	        		fwrite(&b.num,sizeof(char),1,ptr);
	        		k=0;
	        	}
	        }
             
        }
        
             
    }
    
    FILE *ptrA = fopen("bitesPlus.dat","wb");
    
    if(k>0 && k<8){
    	l=8-k;
    	for(;k<8;k++){
    		byte[k]=0;
    		s[cTL++]=0 ;
    	}
    	fwrite(&l,sizeof(int),1,ptrA);
   		b.bi.b0=byte[0];
	    b.bi.b1=byte[1];
	    b.bi.b2=byte[2];
		b.bi.b3=byte[3];
	   	b.bi.b4=byte[4];
		b.bi.b5=byte[5];
		b.bi.b6=byte[6];
	   	b.bi.b7=byte[7];
    	fwrite(&b.num,sizeof(char),1,ptr);
    }
    
	fclose(ptrA);
	fclose(ptr);
	return cTL;
}


int main() {
    char frase[10000] = "ola ludmila eu te amo muito garota voce e muito incrivel e eu te amo muito muito mesmo\0"; // Frase qualquer para gerar a estrutura
	int codifyS[10000];
	SetConsoleTitle("Codificacao de frase atraves de algoritmos de arvore de Huffman"); 
	char op;
	inicioTOP();
	scanf("%c",&op);
	int flag=0,flag2=0,flag3=0,TL;
	
    Table *tab = NULL; // tabelas das palavras com a frequencia e o simbolo das mesmas, até agora sem o huffcode
    Tree *huffmanTree; // arvore final
    Florest *flo = NULL; // floresta para montar as arvores
    fillTableWithFrequency(&tab, frase); // coloco as palavras o simbolo e a frequencia na tabela, ainda sem huffcode
    createFlorest(&flo, tab); //crio a floresta apenas com as folhas com o simbolo e frequencia presentes na tabela
    
    createHuffmanTree(&flo); //deixo a floresta apenas com uma arvore que possui toda a estrutura de huffman
    
    huffmanTree = flo->tree; //salvo a ultima arvore da floresta
    
    free(flo); //excluo a floresta, todos os outros nós da floresta ja foram excluidos na funcão createHuffmanTree
    				
    			
    putHuffCodeInTable(huffmanTree,&tab);
    
    saveInDiscTable(tab);
    
    TL = codifyAndSaveInDisc(tab,huffmanTree,codifyS,frase);
    
    do{
    	op= toupper(menu(frase,codifyS,TL));
    	switch(op){
    		case 'A':
    			    system("cls");
    				showTree(huffmanTree,65,8,45);
    				getch();
				break;
    		case 'B':
					showFullTable(tab);
					getch();
    			break;		
    			
    		case 27:
    			break;
    		default:
    			gotoxy(10,20);
    			printf("Comando nao identificado");
    			getch();
    			break;
    		
    	}
    	
    }while(op!=27);
}

