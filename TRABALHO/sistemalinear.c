#include "sistemalinear.h"

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

SistLinear_t *alocaSistLinear(unsigned int n, unsigned int m){
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

SistLinear_t * lerSistemaMinQuad(TabelasPontos *TP){
/*Esta função cria o sistema linear referente ao método Mínimos Quadrados.
Como o primeiro elemento da matriz é um somatório de 1's com limite n (qtdade de
valores x), o primeiro elemento é igual a n. Observe que em um sist. nxn, é necessario
calcular somente os valores da primeira linha e da última coluna. Por exemplo, onde Si é um somatorio
S1a  + S2b  +  S3c + S4d
S5e  + S6f  +  S7g + S8h
S9i  + S10j +  S11k + S12l
S13m + S14n +  S15o + S16p
É necessario calcular somente S1a S2b S3c S4d S8h S12l S16p, e copiar os valores para as outras posições
onde S5e = S2b, S6f = S9i = S3c ...
O primeiro for interno: 
-calcula os valores da primeira linha do sistema (aproveitando as multiplicações)
-soma xi^i nos respectivos somatórios
O segundo for interno:
-calcula os valores da ultima coluna do sistema
-aproveita o aux calculado anteriormente pois A[1][n-1] = A[0][n-1]*x (onde x é o valor tabelado)
*/
    int n = TP->n;
    int m = TP->m;
    SistLinear_t *SL = alocaSistLinear(n, m);

    for(int i=0; i<m; i++){//para cada função
        for(int j=0; j<n; j++){//para cada elemento da função
             SL->b[i*n] += TP->tabelaFuncoes[i*n + j];//calcula a soma de todos os valores da funcao
        }
    }


    for(int i=0; i<m; i++){//para cada função
        for(int j=0; j<n; j++){//para cada elemento da função
            real_t aux2=TP->tabelaFuncoes[i*n + j];//pega o elemento
            for(int k=1; k<n; k++){//para cada somatorio
                aux2=aux2*TP->tabelaValores[j];
                SL->b[i*n + k] += aux2;
            }
        }
    }


    SL->A[0] = n;

    real_t aux=1;
    for(int i=0; i<n; i++){
        for(int j=1; j<n; j++){
            aux = aux*TP->tabelaValores[i];
            SL->A[j] += aux; 
        }

        for(int j=1; j<n; j++){
            aux = aux*TP->tabelaValores[i];
            SL->A[j*n + n-1] += aux;
        }
        aux=1;//reseta aux pra calcular Xi+1 X²i+1..
    }

    //copiando valores já calculados
    for(int i=1; i<n; i++){
    /*copia os somatorios da primeira linha
    Observe que o elemento 0 faz 0 cópias. O elemento 1 faz 1 cópia
    j é o numero de cópias.
    copia o elemento i para a diagonal do mesmo*/
        for(int j=0; j<i; j++){
            SL->A[(1+j)*n + i-(j+1)] = SL->A[i];
        }
    }

    for(int i=n-2; i>-1; i--){
    /*Observe que o elemento n-1 faz 0 cópias. O elemento n-2 faz 1 cópia
    j é o numero de cópias
    copia o elemento i para a diagonal do mesmo*/
        for(int j=0; j<n-1-i; j++){
            SL->A[(i+j+1)*n + n-1-(j+1)] = SL->A[i*n + n-1];
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

    for(int k=0; k<SL->m; k++){//trocar linha de todas as funções
        aux = SL->b[k*n + iPivo];
        SL->b[k*n + iPivo] = SL->b[k*n + i];
        SL->b[k*n + i] = aux;
    }

}

void escalona (SistLinear_t *SL){
    int n = SL->n;
    for (int i=0; i < n; ++i) {
	    // int iPivo = encontraMax(SL, i);
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

void multiplyLU(SistLinear_t *SL){
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
        x[funcLine*n + i] = b[i];
        for(int j = i+1; j<n; ++j)
            x[funcLine*n + i] -= A[i*n + j]*x[funcLine*n + j];
        x[funcLine*n + i] /= A[i*n + i];
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

void fatoracaoLU(SistLinear_t *SL, real_t *x){
    int n=SL->n;

    for(int k=0; k<SL->m; k++){
        real_t y[n];
        retrossubsL(SL->L, SL->b, y, n, k);
        retrossubsU(SL->A, y, x, n, k);

    }

}
