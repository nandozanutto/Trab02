#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef float real_t;


typedef struct
{
    unsigned int n; // tamanho do SL
    real_t **A;     // coeficientes
    real_t *b; // termos independentes
    float *norma;
} SistLinear_t;

SistLinear_t *alocaSistLinear(unsigned int n)
{
    SistLinear_t *SL = (SistLinear_t *)malloc(sizeof(SistLinear_t));
    SL->n = n;
    SL->A = (real_t **)calloc(SL->n, sizeof(real_t *));//valores tabelados
    SL->b = (real_t *)calloc(SL->n, sizeof(real_t));
    SL->norma = (float *)calloc(SL->n, sizeof(float));

    for (int i = 0; i < SL->n; i++)
        SL->A[i] = (real_t *)calloc(SL->n, sizeof(real_t));

    return SL;
}


SistLinear_t * lerSistemaInter(real_t **matriz, unsigned int n, unsigned int func){
    SistLinear_t *SL = alocaSistLinear(n);
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
    SL->b = matriz[funcLine+1];
}

real_t ** lerMatriz(unsigned int n, unsigned int m){

    real_t **matriz = (real_t **)malloc((m+1) * sizeof(real_t *));
    for (int i = 0; i < m+1; i++)
        matriz[i] = (real_t *)malloc(n * sizeof(real_t));
    
    for(int i=0; i<m+1; i++){
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
        aux=1;
    }

    //copiando valores já calculados
    for(int i=1; i<n; i++){
        for(int j=0; j<i; j++){
            SL->A[1+j][i-(j+1)] = SL->A[0][i];
        }
    }

    for(int i=n-2; i>-1; i--){
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