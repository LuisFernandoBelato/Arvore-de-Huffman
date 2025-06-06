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



// ARVORE DE HUFFMAN E SUAS FUNÇÕES



struct TreeHuffman
{
	int codigo;
	int freq;
	struct TreeHuffman *esq;
	struct TreeHuffman *dir;
};
typedef struct TreeHuffman TreeHuffman;


TreeHuffman* CriaNoHuff (int codigo, int freq, TreeHuffman *NoEsq, TreeHuffman *NoDir)
{
	TreeHuffman *Nova = (TreeHuffman*) malloc(sizeof(TreeHuffman));
	Nova->codigo = codigo;
	Nova->freq = freq;
	Nova->esq = NoEsq;
	Nova->dir = NoDir;
	return Nova;
}




// LISTA HUFFMAN E SUAS FUNÇÕES



struct ListaHuffman
{
	struct TreeHuffman *No;
	struct ListaHuffman *prox;
};
typedef struct ListaHuffman ListaHuffman;

char IsEmpty (ListaHuffman *LH)
{
	return LH == NULL;
}

void InsereOrdenado (ListaHuffman **LH, TreeHuffman *TH)
{
	
	ListaHuffman *Aux, *Nova = (ListaHuffman*) malloc(sizeof(ListaHuffman));
	Nova->No = TH;
	Nova->prox = NULL;
	
	
	if (*LH == NULL) // LISTA VAZIA
		*LH = Nova;
	else // LISTA TEM CAIXAS
	{
		if ((*LH)->No->freq > Nova->No->freq) // VAI SER A PRIMEIRA CAIXA
		{
			Nova->prox = *LH;
			(*LH) = Nova;
		}
		else
		{
			Aux = *LH;
			while (Aux->prox != NULL && Aux->prox->No->freq < Nova->No->freq) // ACHAR A POSIÇÃO IDEAL PARA INSERIR A "NOVA" CAIXA
				Aux = Aux->prox;
			Nova->prox = Aux->prox;
			Aux->prox = Nova;
		}
	}
}

void Retirar (ListaHuffman **LH, ListaHuffman **Retorno)
{
	*Retorno = *LH;
	(*LH) = (*LH)->prox;
}

void ExibeLHR (ListaHuffman *LH)
{
	if (LH != NULL)
	{
		printf("C: %d ------ F: %d\n",LH->No->codigo,LH->No->freq);
		ExibeLHR(LH->prox);
	}
}




// LISTA AUXILIAR E SUAS FUNÇÕES - PARA GUARDAR AS PALAVRAS




struct Lista 
{
	int codigo;
	char Palavra[500];
	char Codigo_De_Huffman[500];
	int freq;
	struct Lista *prox;
};
typedef struct Lista Lista;

char IsEmpty (Lista *L)
{
	return L == NULL;
}

Lista* CriaCaixa (char Palavra[])
{
	Lista *Nova = (Lista*) malloc (sizeof(Lista));
	Nova->prox = NULL;
	Nova->freq = 0;
	Nova->codigo = 1;
	strcpy(Nova->Palavra,Palavra);
	return Nova;
}

void Insere (Lista **L, char Palavra[])
{
	Lista *Aux;
	if (*L == NULL)
		*L = CriaCaixa(Palavra);
	else
	{
		Aux = *L;
		while (Aux->prox != NULL)
			Aux = Aux->prox;
		Aux->prox = CriaCaixa(Palavra);
		Aux->prox->codigo = Aux->codigo + 1;
	}
}

Lista* BuscaPalavra (Lista *L, char Palavra[])
{
	while (L != NULL && strcmp(L->Palavra,Palavra) != 0)
		L = L->prox;
	return L;
}

Lista* BuscaCodigo (Lista *L, int Codigo)
{
	while (L != NULL && L->codigo != Codigo)
		L = L->prox;
	return L;
}

void ExibeR (Lista *L)
{
	if (L != NULL)
	{
		printf("P: %s ------ F: %d ------ C: %d\n",L->Palavra,L->freq,L->codigo);
		ExibeR(L->prox);
	}
}


// ESTRUTURA PARA REGISTRO EM ARQUIVO BINÁRIO


struct Registro
{
	int codigo;
	char Palavra[1000];
	char Huffman[1000];
};
typedef struct Registro Registro;




//
// ### FUNÇÕES GERAIS ###
//



void ExibeArvoreDeHuffman (TreeHuffman *TH, int *n)
{
	if (TH != NULL)
	{
		(*n)++;
		ExibeArvoreDeHuffman(TH->dir,&*n);
		for (int i = 0; i < 5*(*n);i++) printf(" ");
		printf("(%d,%d)\n",TH->codigo,TH->freq);
		ExibeArvoreDeHuffman(TH->esq,&*n);
		(*n)--;
	}
}

void In_Ordem (TreeHuffman *TH)
{
	if (TH != NULL)
	{
		In_Ordem(TH->esq);
		printf("Freq: %d ------ Cod: %d\n",TH->freq,TH->codigo);
		In_Ordem(TH->dir);
	}
}

void Pre_Ordem (TreeHuffman *TH)
{
	if (TH != NULL)
	{
		printf("Freq: %d ------ Cod: %d\n",TH->freq,TH->codigo);
		Pre_Ordem(TH->esq);
		Pre_Ordem(TH->dir);
	}
}

void Codigos_De_Huffman_Pre_Ordem (TreeHuffman *TH, char Codigo_De_Huffman[], int tl, char lado, Lista *L)
{
	Lista *Aux;
	if (TH != NULL)
	{
		if (lado == 'E')
			Codigo_De_Huffman[tl] = '0';
		else
			Codigo_De_Huffman[tl] = '1';
			
			
		if (TH->esq == NULL && TH->dir == NULL) // CHEGOU NA FOLHA
		{
			Codigo_De_Huffman[tl+1] = '\0';
			
			// PROCURA PELO CÓDIGO DO NÓ NA LISTA E INSERE O CÓDIGO DE HUFFMAN
			
			Aux = BuscaCodigo(L,TH->codigo);
			if (Aux != NULL)
				strcpy(Aux->Codigo_De_Huffman,Codigo_De_Huffman);
		}
		
		Codigos_De_Huffman_Pre_Ordem (TH->esq,Codigo_De_Huffman,tl+1,'E',L);
		Codigos_De_Huffman_Pre_Ordem (TH->dir,Codigo_De_Huffman,tl+1,'D',L);
	}
}

void LeBin ()
{
	FILE *PtrBin = fopen("Dados_Para_Descompactação.dat","rb+");
	Registro R;
	fread(&R,sizeof(Registro),1,PtrBin);
	while (!feof(PtrBin))
	{
		printf("C . %d - ",R.codigo);
		printf("P . %s - ",R.Palavra);
		printf("H . %s\n",R.Huffman);
		fread(&R,sizeof(Registro),1,PtrBin);
	}
	fclose(PtrBin);
}

void Remaneja (char String[], int i)
{
	for (; String[i] != '\0';i++)
		String[i] = String[i+1];
}

void LimpaString (char String[])
{
	for (int i=0; i < strlen(String); i++)
	{
		if (String[i] >= 65 && String[i] <= 90)
			String[i] = String[i] + 32;
		else
		if (String[i] == '.' || String[i] == ',' || String[i] == '!' || String[i] == '?' || String[i] == ';' || String[i] == ':')
			Remaneja(String,i);
	}
}


//
// ### FUNÇÕES PRINCIPAIS ###
//



void Monta_Palavras_e_Frequencias (char Texto[], Lista **L)
{
	char Palavra[1000];
	Palavra[0] = '\0';
	int i,j,z;
	
	for (i=0; i < strlen(Texto);i++)
	{
		if (Texto[i] == 32 || i == 0)
		{
			if (i == 0)
				j = i;
			else
				j = i+1;
			z = 0;
			while (Texto[j] != 32 && Texto[j] != '\0' && Texto[j] != '.' && Texto[j] != ',' && Texto[j] != '!' && Texto[j] != '?')
				Palavra[z++] = Texto[j++];
			Palavra[z] = '\0';
			
			// SÓ INSERE AS PALAVRAS SE NÃO ACHAR NA LISTA
			if (!BuscaPalavra(*L,Palavra))
				Insere(&*L,Palavra);
			Palavra[0] = '\0';
		}
	}
	int Espacos = 0;
	for (i=0; i < strlen(Texto);i++)
	{
		Palavra[0] = '\0';
		if (Texto[i] == 32)
		{
			Palavra[0] = ' ';
			Palavra[1] = '\0';
			Espacos++;
			if (!BuscaPalavra(*L,Palavra))
				Insere(&*L,Palavra);
		}
	}
	//ExibeR(*L);
	Lista *Aux = *L;
	char PalvAux[500];
	while (Aux != NULL)
	{
		for (i=0; i < strlen(Texto);i++)
		{
			if (Texto[i] == 32 || i == 0)
			{
				if (i == 0)
					j = i;
				else
					j = i+1;
				z = 0;
				while (Texto[j] != 32 && Texto[j] != '\0' && Texto[j] != '.' && Texto[j] != ',' && Texto[j] != '!' && Texto[j] != '?')
					Palavra[z++] = Texto[j++];
				Palavra[z] = '\0';
				
				// SÓ INSERE AS PALAVRAS SE NÃO ACHAR NA LISTA
				if (strcmp(Palavra,Aux->Palavra) == 0)
					Aux->freq++;
				Palavra[0] = '\0';
			}
		}
		Aux = Aux->prox;
	}
	
	
	Palavra[0] = ' ';
	Palavra[1] = '\0';
	Aux = BuscaPalavra(*L,Palavra);
	if (Aux != NULL)
		Aux->freq = Espacos;
		
	Aux = *L;
	int QtdePalavras = 0;
	while (Aux != NULL)
		{
			QtdePalavras += Aux->freq;
			Aux = Aux->prox;
		}	
		
	printf("\n    ### SEPARACAO DAS PALAVRAS ###\n\n");
	ExibeR(*L);
	printf("\n\nQuantidade de Palavras: %d\n",QtdePalavras);
	
}

void Monta_Arvore_Huffman(ListaHuffman **LH, TreeHuffman **TH, Lista **L)
{
	// MONTAR A LISTA DE HUFFMAN
	
	Lista *AuxL = *L;
	while (AuxL != NULL)
	{
		InsereOrdenado(&*LH,CriaNoHuff(AuxL->codigo,AuxL->freq,NULL,NULL));
		AuxL = AuxL->prox;
	}
	
	printf("\n\n ### MONTAGEM DA LISTA ###\n\n");
	ExibeLHR(*LH);
	printf("\n\n\n\n");
	
	// MONTAR A ARVORE DE HUFFMAN
	
	ListaHuffman *Aux1, *Aux2;
	
	while ((*LH)->prox != NULL)
	{
		Retirar(&*LH, &Aux1);
		Retirar(&*LH, &Aux2);
		InsereOrdenado(&*LH,CriaNoHuff(-1,Aux1->No->freq + Aux2->No->freq,Aux1->No,Aux2->No));
	}
	printf("   ### ARVORE DE HUFFMAN ###\n\n");
	int n = -1;
	ExibeArvoreDeHuffman((*LH)->No,&n);
	//Pre_Ordem((*LH)->No);
	*TH = (*LH)->No;
}

void Gera_Codigos_de_Huffman(TreeHuffman **TH, Lista **L)
{
	char Codigo_De_Huffman[500];
	int tl = 0;
	
	Codigos_De_Huffman_Pre_Ordem((*TH)->esq,Codigo_De_Huffman,tl,'E',*L);
	Codigos_De_Huffman_Pre_Ordem((*TH)->dir,Codigo_De_Huffman,tl,'D',*L);
	
	Lista *Aux = *L;
	printf("\n\n       ### CODIGOS DE HUFFMAN ###\n\n");
	while (Aux != NULL)
	{
		printf("C = %d /// F = %d /// P = %s /// H = %s\n",Aux->codigo,Aux->freq,Aux->Palavra,Aux->Codigo_De_Huffman);
		Aux = Aux->prox;
	}
}

void Cria_Arquivos_Binario_Texto (TreeHuffman **TH, Lista **L, char Frase[])
{
	// CRIA ARQUIVO BINÁRIO
	
	FILE *PtrBin = fopen("Dados_Para_Descompactação.dat","wb+");
	Registro R;
	Lista *Aux = *L;
	while (Aux != NULL)
	{
		R.codigo = Aux->codigo;
		strcpy(R.Palavra,Aux->Palavra);
		strcpy(R.Huffman,Aux->Codigo_De_Huffman);
		fwrite(&R,sizeof(Registro),1,PtrBin);
		Aux = Aux->prox;
	}
	fclose(PtrBin);
	
	// CRIA ARQUIVO TEXTO
	
	Aux = *L;
	char SequenciaBinaria[1000], Palavra[1000], Espaco[3];
	SequenciaBinaria[0] = '\0'; Espaco[0] = 32; Espaco[1] = '\0';
	int i = 0,j = 0,z = 0,tl = 0;
	for (i=0; i < strlen(Frase);i++)
	{
		if (Frase[i] == 32 || i == 0)
		{
			if (i == 0)
				j = i;
			else
				j = i+1;
			z = 0;
			while (Frase[j] != 32 && Frase[j] != '\0')
				Palavra[z++] = Frase[j++];
			Palavra[z] = '\0';
			
			Aux = BuscaPalavra(*L,Palavra);
			if (Aux != NULL)
			{
				strcat(SequenciaBinaria,Aux->Codigo_De_Huffman);
				tl += strlen(Aux->Codigo_De_Huffman);
			}
			if (Frase[j] == 32)
			{
				Aux = BuscaPalavra(*L,Espaco);
				if (Aux != NULL)
				{
					strcat(SequenciaBinaria,Aux->Codigo_De_Huffman);
					tl += strlen(Aux->Codigo_De_Huffman);
				}	
			}
			Palavra[0] = '\0';
		}
	}
	SequenciaBinaria[tl] = '\0';
	
	FILE * PtrTxt = fopen("SequenciaBinaria.txt","w+");
	fprintf(PtrTxt,"%s\n",&SequenciaBinaria);
	fclose(PtrTxt);
	
	printf("\n\nFraseOrig : %s\n",Frase);
	printf("\n\nSequenBin : %s\n",SequenciaBinaria);
}





//
// ### MAIN ###
//




int main ()
{
	TreeHuffman *TH = NULL;
	ListaHuffman *LH = NULL;
	Lista *L = NULL;
	char Texto[5000], Frase[2000];
	
	
	// 'TEXTO' PARA A ARVORE DE HUFFMAN
	// 'FRASE' PARA CODIFICACÃO
	
	
	
	// EXEMPLOS PARA TESTE
	
	
	// EXEMPLO 1 'MAIOR'
	
	//strcpy(Texto,"A pneumoultramicroscopicossilicovulcanoconiose é uma doença rara, causada pela inalação de partículas finas provenientes da atividade vulcânica. A complexidade do termo pneumoultramicroscopicossilicovulcanoconiótico requer atenção até mesmo os profissionais de saúde. É essencial a conscientização sobre os riscos e cuidados necessários para evitar exposição a substâncias como o silício vulcânico, que pode desencadear essa condição peculiar. Preocupações com o paraclorobenzilpirrolidinonetilbenzimidazol e outros compostos são crescentes devido aos potenciais impactos na saúde. No entanto, é crucial manter um equilíbrio na avaliação desses riscos para não alimentar desnecessariamente a hipopotomonstrosesquipedaliofobia, o medo irracional de palavras longas enfatiza a necessidade de clareza na comunicação sobre temas científicos, incluindo o monosialotetraesosilgangliosideo. Em um mundo onde até mesmo o monosialotetraesosilgangliosideo é discutido em termos científicos, a informação clara e acessível sobre essas substâncias é fundamental para promover uma compreensão mais ampla e consciente da saúde e do meio ambiente.");
	//strcpy(Frase,"A pneumoultramicroscopicossilicovulcanoconiose requer atenção aos riscos de exposição a substâncias, como o silício vulcânico. A hipopotomonstrosesquipedaliofobia, medo de palavras longas, enfatiza a clareza na comunicação, incluindo o monosialotetraesosilgangliosideo.");
	
	// EXEMPLO 2 'MENOR'
	
	strcpy(Texto,"O cachorro correu, correu, correu no parque enquanto brincava, brincava, brincava com a bola. O cachorro pulava, pulava, pulava, o cachorro latia, latia, latia, o cachorro corria, corria, corria atrás da bola, da bola, da bola, o cachorro estava realmente, realmente, realmente muito animado. O cachorro é um animal muito, muito, muito leal, o cachorro é um amigo fiel, fiel, fiel, o cachorro é o melhor amigo do homem, do homem, do homem.");
	strcpy(Frase,"cachorro brincava, pulava, corria, latia.");
	
	
	//////////////////////////////////////

	LimpaString(Texto); LimpaString(Frase);
	 
	FILE * Ptr = fopen("Frase.txt","w");
	fprintf(Ptr,"Texto : %s\n",Texto);
	fprintf(Ptr,"\n\nCODIFI : %s\n",Frase);
	fclose(Ptr);
	
	Monta_Palavras_e_Frequencias(Texto,&L);
	Monta_Arvore_Huffman(&LH,&TH,&L);
	Gera_Codigos_de_Huffman(&TH,&L);
	Cria_Arquivos_Binario_Texto(&TH,&L,Frase);
	
	return 0;
}
