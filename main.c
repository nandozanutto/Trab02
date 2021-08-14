#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef float real_t;

typedef struct {
  int n, m;
  real_t *tabelaValores;
  real_t *tabelaFuncoes;
} TabelasPontos;


typedef struct
{
    unsigned int n, m; // tamanho do SL 
    real_t *A;     // matriz dos coeficientes
    real_t *b; // termos independentes
    real_t *L;
} SistLinear_t;


TabelasPontos *alocaTabelaPontos(unsigned int n, unsigned int m) {
  TabelasPontos *TP = (TabelasPontos*)malloc(sizeof(TabelasPontos));
  TP->n = n;
  TP->m = m;
  TP->tabelaValores = (real_t*)malloc(n * sizeof(real_t));
  TP->tabelaFuncoes = (real_t*)malloc(n * m * sizeof(real_t));
}

void lerTabelaPontos(TabelasPontos *TP) {
  int i, j;
  int n = TP->n;
  int m = TP->m;
  for (i = 0; i < n; i++) {
    scanf("%f", &TP->tabelaValores[i]);
  }

  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      scanf("%f", &TP->tabelaFuncoes[i*n] + j);
    }
  }
}

void imprimeTabela(TabelasPontos *TP){

    for(int i=0; i<TP->n; i++)
        printf("%f ", TP->tabelaValores[i]);
    printf("\n");

    for(int i=0; i<TP->m; i++){
        for(int j=0; j<TP->n; j++)
            printf("%f ", TP->tabelaFuncoes[i*TP->n + j]);
        printf("\n");
    }


}

SistLinear_t *alocaSistLinear(unsigned int n, unsigned int m)
{
    SistLinear_t *SL = (SistLinear_t *)malloc(sizeof(SistLinear_t));
    SL->n = n;
    SL->m = m;
    
    SL->A = (real_t *)calloc(n*n, sizeof(real_t));//matriz dos coeficientes
    SL->b = (real_t *)calloc(n*m,  sizeof(real_t));//matriz das funções
    SL->L = (real_t *)calloc(n*n,  sizeof(real_t));


    for(int i=0; i<n; i++)
        SL->L[i*n + i] = 1; //colocando 1 na diagonal principal

    return SL;
}

SistLinear_t * lerSistemaInter(TabelasPontos *TP){
    /*Esta função recebe como entrada a tabela de valores (chamada de matriz) e o tamanho do sistema
    Primeiro é colocado 1 na primeira coluna da matriz dos coeficientes. Na segunda coluna da matriz de 
    coef. é colocado o valor x tabelado. Após isso para calcular os valores das próximas colunas é feito
    colunaAnterior*valorTabelado (para aproveitar multiplicações feitas). 
    Exemplo: 1a + 20b + 20²c + xd
    -> x = 20²*20 (coluna anterior vezes valor tabelado*/
    int n = TP->n;
    int m = TP->m;
    SistLinear_t *SL = alocaSistLinear(n, m);
    /*A linha abaixo faz o vetor de termos independentes apontar para a segunda linha da tabela
    Observe que a primeira linha da tabela são os valores x e as próximas linhas são as funções
    Primeira função f(x): matriz[1], segunda função: matriz[2]. É feito assim para evitar cópias 
    desnecessárias*/
    SL->b = TP->tabelaFuncoes;


    for(int i=0; i<n; i++){
        SL->A[i*n] = 1;
        SL->A[i*n + 1] = TP->tabelaValores[i];
    }

    for(int i=0; i<n; i++){
        for(int j=2; j<n; j++){
            SL->A[i*n + j] = SL->A[i*n+j-1]*SL->A[i*n + 1];
        }
    }

    return SL;
}

void imprimeSistema(SistLinear_t *SL, int funcLine){
    int n = SL->n;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%f ", SL->A[i*n+j]);
        printf("%f\n", SL->b[funcLine*n + i]);
    }
}

int encontraMax(SistLinear_t *SL, int i){
    int n=SL->n;
    double maior = SL->A[i*n + i];
    int maiorLinha = i;
    for(int linha=i+1; linha<n; linha++)
        if(fabs(SL->A[linha*n + i]) > fabs(maior)){
            maior = SL->A[linha*n + i];
            maiorLinha = linha;
        }
    return maiorLinha;
}

void trocaLinha(SistLinear_t *SL, int i, int iPivo){
    double aux;
    int n=SL->n;
    for(int coluna=0; coluna<SL->n; coluna++){
        aux = SL->A[iPivo*n + coluna];
        SL->A[iPivo*n + coluna] = SL->A[i*n + coluna];
        SL->A[i*n + coluna] = aux;
    }

    aux = SL->b[iPivo];
    SL->b[iPivo] = SL->b[i];
    SL->b[i] = aux;

}

void escalona (SistLinear_t *SL){
    int n = SL->n;
    for (int i=0; i < n; ++i) {      
	    int iPivo = encontraMax(SL, i);      
        // if ( i != iPivo )         
        //     trocaLinha(SL, i, iPivo);
        for(int k=i+1; k < n; ++k) {         
            double m = SL->A[k*n + i]/SL->A[i*n + i]; 
            // printf("m: %f\n", m);        
            SL->A[k*n + i] = 0;
            SL->L[k*n + i] = m;
            for(int j=i+1; j < n; ++j)            
                SL->A[k*n + j] -= SL->A[i*n + j] * m;         
            // SL->b[k] -= SL->b[i] * m;      
        }   
	}
}

void imprimeLU(SistLinear_t *SL){
    int n=SL->n;
    printf("Matriz L\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%f ", SL->L[i*n + j]);
        printf("\n");
    }
    printf("\nMatriz U\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%f ", SL->A[i*n + j]);
        printf("\n");
    }
}

void multiplyLU(SistLinear_t *SL)
{
    int i, j, k;
    int n = SL->n;
    real_t res[n][n];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            res[i][j] = 0;
            for (k = 0; k < n; k++)
                res[i][j] += SL->L[i*n + k] * SL->A[k*n + j];

        }
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%f ", res[i][j]);
        printf("\n");
    }
    
}


void retrossubsU(real_t *A, real_t *b, real_t *x, int n, int funcLine){
    for(int i=n-1; i>=0; --i){
        x[i] = b[funcLine*n + i];
        for(int j = i+1; j<n; ++j)
            x[i] -= A[i*n + j]*x[j];
        x[i] /= A[i*n + i];
    }
}

void retrossubsL(real_t *A, real_t *b, real_t *x, int n, int funcLine){
    for(int i=0; i<n; ++i){
        x[i] = b[funcLine*n + i];
        for(int j = i-1; j>=0; --j)
            x[i] -= A[i*n + j]*x[j];
        x[i] /= A[i*n + i];
    }
}

void fatoracaoLU(SistLinear_t *SL){
    int n=SL->n;

    for(int k=0; k<SL->m; k++){
        real_t y[n];
        real_t x[n];
        retrossubsL(SL->L, SL->b, y, n, k);
        retrossubsU(SL->A, y, x, n, 0);
        for(int i=0; i<n; i++)
            printf("%f ", x[i]);
        printf("\n");
    }

}

int main(){
    unsigned int n, m;
    scanf("%d %d", &n, &m);
    real_t * aux = (real_t*)malloc(n*sizeof(real_t));
    // printf("%d %d\n", n,m );
    TabelasPontos *TP = alocaTabelaPontos(n, m);
    lerTabelaPontos(TP);
    // imprimeTabela(TP);
    SistLinear_t *SL = lerSistemaInter(TP);
    // imprimeSistema(SL, 1);
    escalona(SL);
    // printf("\n\n");
    // imprimeSistema(SL, 0);
    fatoracaoLU(SL);

}
