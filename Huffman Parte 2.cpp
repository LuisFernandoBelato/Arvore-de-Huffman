#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>


//
// ### ESTRUTURAS DEFINIDAS ###
//



// ESTRUTURA PARA REGISTRO EM ARQUIVO BINÁRIO

struct Registro
{
	int codigo;
	char Palavra[1000];
	char Huffman[1000];
};
typedef struct Registro Registro;


int ARQBuscaCodigo (FILE *PtrBin, char Codigo_De_Huffman[])
{
	Registro R;
	rewind(PtrBin);
	fread(&R,sizeof(Registro),1,PtrBin);
	while (!feof(PtrBin) && strcmp(R.Huffman,Codigo_De_Huffman) != 0)
		fread(&R,sizeof(Registro),1,PtrBin);
	if (strcmp(R.Huffman,Codigo_De_Huffman) == 0)
		return ftell(PtrBin) - sizeof(Registro);
	else
		return -1;
}



// ARVORE PARA DECODIFICAÇÃO



struct Tree
{
	struct Tree *esq;
	struct Tree *dir;
};
typedef struct Tree Tree;


Tree* CriaNo ()
{
	Tree *Nova = (Tree*) malloc(sizeof(Tree));
	Nova->esq = Nova->dir = NULL;
	return Nova;
}



//
// ### FUNÇÕES GERAIS ###
//



void Percorre_Criando (Tree *ABB, char Codigo_De_Huffman[], int tl)
{
	if (Codigo_De_Huffman[tl] != '\0') // PERGUNTAR SE É O FINAL DA STRING
	{
		if (Codigo_De_Huffman[tl] == '0')
		{
			if (ABB->esq == NULL)
				ABB->esq = CriaNo();
			ABB = ABB->esq;
			Percorre_Criando(ABB,Codigo_De_Huffman,tl+1);
		}
		else
		{
			if (ABB->dir == NULL)
				ABB->dir = CriaNo();
			ABB = ABB->dir;
			Percorre_Criando(ABB,Codigo_De_Huffman,tl+1);
		}
	}
}


void Percorre_Ate_Folha (FILE *PtrBin ,Tree *ABB, char SequenciaBinaria[], char Codigo_De_Huffman[], char Palavra[], int *tlSB, int tlC) // VOU ATUALIZAR O TL PARA QUANDO CHAMAR DE NOVO CONTINUAR DA ONDE PAROU
{
	int pos;
	Registro R;
	if (ABB->esq == NULL && ABB->dir == NULL)
	{
		Codigo_De_Huffman[tlC] = '\0';
		pos = ARQBuscaCodigo(PtrBin,Codigo_De_Huffman);
		if (pos != -1)
		{
			fseek(PtrBin,pos,0);
			fread(&R,sizeof(Registro),1,PtrBin);
			strcpy(Palavra,R.Palavra);
		}
	}
	else
	{
		if (SequenciaBinaria[(*tlSB)] == '0')
		{
			Codigo_De_Huffman[tlC] = '0';
			*tlSB = (*tlSB) + 1;
			Percorre_Ate_Folha(PtrBin,ABB->esq,SequenciaBinaria,Codigo_De_Huffman,Palavra,&*tlSB,tlC+1);
		}
		else
		{
			Codigo_De_Huffman[tlC] = '1';
			*tlSB = (*tlSB) + 1;
			Percorre_Ate_Folha(PtrBin,ABB->dir,SequenciaBinaria,Codigo_De_Huffman,Palavra,&*tlSB,tlC+1);
		}
	}
}


//
// ### FUNÇÕES PRINCIPAIS ###
//


void Monta_Arvore_Binaria (Tree **Raiz)
{
	int tl=0;
	Registro R;
	*Raiz = CriaNo();
	FILE *PtrBin = fopen("Dados_Para_Descompactação.dat","rb");
	if (PtrBin != NULL)
	{
		fread(&R,sizeof(Registro),1,PtrBin);
		while (!feof(PtrBin))
		{
			Percorre_Criando(*Raiz,R.Huffman,tl);
			fread(&R,sizeof(Registro),1,PtrBin);
		}
		fclose(PtrBin);
	}
}

void Decodifica_Frase(Tree *Raiz)
{
	Registro R;
	int tl=0,tlteste=0;
	char SequenciaBinaria[1000], Codigo_De_Huffman[1000], Frase[5000], Palavra[1000];
	Frase[0] = '\0'; Palavra[0] = '\0', Codigo_De_Huffman[0] = '\0';
	FILE * PtrTxt = fopen("SequenciaBinaria.txt","r");
	if (PtrTxt != NULL)
	{
		fgets(SequenciaBinaria,1000,PtrTxt);
		FILE *PtrBin = fopen("Dados_Para_Descompactação.dat","rb");
		if (PtrBin != NULL)
		{
			while (tl < strlen(SequenciaBinaria)-1) // ATÉ DECODIFICAR TODA A SEQUENCIA BINÁRIA
			{
				Codigo_De_Huffman[0] = '\0';
				Palavra[0] = '\0';
				Percorre_Ate_Folha(PtrBin,Raiz,SequenciaBinaria,Codigo_De_Huffman,Palavra,&tl,0);
				strcat(Frase,Palavra);
				tlteste += strlen(Palavra);
			}
			Frase[tlteste] = '\0';
			printf("\n ### FRASE DECODIFICADA ### \n\n%s\n",Frase);
		}
		FILE * Ptr = fopen("Frase.txt","a");
		fprintf(Ptr,"\n\nDECODI : %s",Frase);
		fclose(Ptr);
		fclose(PtrBin);
		fclose(PtrTxt);
	}
}



int main ()
{
	
	Tree *Raiz = NULL;
	Monta_Arvore_Binaria(&Raiz);
	Decodifica_Frase(Raiz);

	return 0;
}
