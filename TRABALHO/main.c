#include "sistemalinear.h"



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
