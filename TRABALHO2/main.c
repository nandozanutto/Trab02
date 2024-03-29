#include "sistemalinear.h"

int main(int argc, char const *argv[]) {

  unsigned int n, m;

  scanf("%d %d", &n, &m);
    printf("%d %d\n", n,m );
    SistemaLinear *SL = alocaSistLinear(n, m);
    TabelasPontos *TP = alocaTabelaPontos(n, m);
    lerTabelaPontos(TP, n, m);

    imprimeMatriz(TP->tabelaFuncoes, n, m);
    printf("-------------------------------------------------\n");
    matrizInterpolacao(SL,TP);
    imprimeMatriz(SL->U, n, m);
    printf("-------------------------------------------------\n");
    triangulacao(SL, TP);
    imprimeMatriz(SL->U, n, m);

    fatoracaoLU(SL,TP);
    imprimeResultado(SL, TP);

  // }
  return 0;
}
