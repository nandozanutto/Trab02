#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef float real_t;


typedef struct
{
    unsigned int n; // tamanho do SL 
    real_t **A;     // matriz dos coeficientes
    real_t *b; // termos independentes
    float *norma;
} SistLinear_t;

SistLinear_t *alocaSistLinear(unsigned int n)
{
    SistLinear_t *SL = (SistLinear_t *)malloc(sizeof(SistLinear_t));
    SL->n = n;
    SL->A = (real_t **)calloc(SL->n, sizeof(real_t *));//matriz dos coeficientes
    SL->b = (real_t *)calloc(SL->n, sizeof(real_t));
    SL->norma = (float *)calloc(SL->n, sizeof(float));

    for (int i = 0; i < SL->n; i++)
        SL->A[i] = (real_t *)calloc(SL->n, sizeof(real_t));

    return SL;
}


SistLinear_t * lerSistemaInter(real_t **matriz, unsigned int n, unsigned int func){
/*Esta função recebe como entrada a tabela de valores (chamada de matriz) e o tamanho do sistema
Primeiro é colocado 1 na primeira coluna da matriz dos coeficientes. Na segunda coluna da matriz de 
coef. é colocado o valor x tabelado. Após isso para calcular os valores das próximas colunas é feito
colunaAnterior*valorTabelado (para aproveitar multiplicações feitas). 
Exemplo: 1a + 20b + 20²c + xd
-> x = 20²*20 (coluna anterior vezes valor tabelado*/

    SistLinear_t *SL = alocaSistLinear(n);
/*A linha abaixo faz o vetor de termos independentes apontar para a segunda linha da tabela
Observe que a primeira linha da tabela são os valores x e as próximas linhas são as funções
Primeira função f(x): matriz[1], segunda função: matriz[2]. É feito assim para evitar cópias 
desnecessárias*/
    SL->b = matriz[1];

    for(int i=0; i<n; i++){
        SL->A[i][0] = 1;
        SL->A[i][1] = matriz[0][i];
    }

    for(int i=0; i<n; i++){
        for(int j=2; j<n; j++){
            SL->A[i][j] = SL->A[i][j-1]*SL->A[i][1];
        }
    }

    return SL;
}

void mudarFuncs(SistLinear_t * SL, int funcLine, real_t **matriz){
//esta função faz o ponteiro dos termos indep. apontar para a função da linha funcLine
    SL->b = matriz[funcLine+1];
}

real_t ** lerMatriz(unsigned int n, unsigned int m){
/*Esta função lê a tabela de entrada. Primeira linha são os valores de x. E as próximas linhas 
são os valores das funções f(x), g(x)....*/
    real_t **matriz = (real_t **)malloc((m+1) * sizeof(real_t *));
    for (int i = 0; i < m+1; i++)
        matriz[i] = (real_t *)malloc(n * sizeof(real_t));
    
    for(int i=0; i<m+1; i++){//m+1 pois a primeira linha não é contada por m
        for(int j=0; j<n; j++)
            scanf("%f", &matriz[i][j]);
    }

    return matriz;
}

void imprimeMatriz(real_t **matriz, unsigned int n, unsigned int m){
    for(int i=0; i<m+1; i++){
        for(int j=0; j<n; j++)
            printf("%f ", matriz[i][j]);
        printf("\n");
    }
}

void imprimeSistema(SistLinear_t *SL){
    for(int i=0; i<SL->n; i++){
        for(int j=0; j<SL->n; j++)
            printf("%f ", SL->A[i][j]);
        printf("%f\n", SL->b[i]);
    }
}

SistLinear_t * lerSistemaMinQuad(real_t **matriz, unsigned int n){
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
 
    SistLinear_t *SL = alocaSistLinear(n);
    SL->b = matriz[1];

    SL->A[0][0] = n;

    real_t aux=1;
    for(int i=0; i<n; i++){
        for(int j=1; j<n; j++){
            aux = aux*matriz[0][i];
            SL->A[0][j] += aux; 
        }

        for(int j=1; j<n; j++){
            aux = aux*matriz[0][i];
            SL->A[j][n-1] += aux;
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
            SL->A[1+j][i-(j+1)] = SL->A[0][i];
        }
    }

    for(int i=n-2; i>-1; i--){
    /*Observe que o elemento n-1 faz 0 cópias. O elemento n-2 faz 1 cópia
    j é o numero de cópias
    copia o elemento i para a diagonal do mesmo*/
        for(int j=0; j<n-1-i; j++){
            SL->A[i+j+1][n-1-(j+1)] = SL->A[i][n-1];
        }
    }

    return SL;

}


int main(){

    SistLinear_t * sistema;
    unsigned int n, m;
    if(0 > scanf("%d %d", &n, &m))
        exit(0);

    real_t ** matriz = lerMatriz(n, m);
    imprimeMatriz(matriz, n, m);
    printf("****************\n");
    sistema = lerSistemaMinQuad(matriz, n);
    imprimeSistema(sistema);



}