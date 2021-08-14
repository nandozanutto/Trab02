#include "sistemalinear.h"



int main(){
    unsigned int n, m;
    scanf("%d %d", &n, &m);
    real_t * resultInter = (real_t*)calloc(n*m, sizeof(real_t));
    real_t * resultQuad = (real_t*)calloc(n*m, sizeof(real_t));

    TabelasPontos *TP = alocaTabelaPontos(n, m);
    lerTabelaPontos(TP);
    // imprimeTabela(TP);


    SistLinear_t *SLInter = lerSistemaInter(TP);
    escalona(SLInter);
    fatoracaoLU(SLInter, resultInter);

    //********************************************

    SistLinear_t *SLQuad = lerSistemaMinQuad(TP);
    escalona(SLQuad);
    fatoracaoLU(SLQuad, resultQuad);
    
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++)
            printf("%.19f ", resultInter[i*n + j]);
        printf("\n");

        for(int j=0; j<n; j++)
            printf("%.19f ", resultQuad[i*n + j]);
        printf("\n");
    }


}
