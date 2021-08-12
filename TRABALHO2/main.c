#include "sistemalinear.h"

int main(int argc, char const *argv[]) {

  unsigned int n, m;

  while ((0 < scanf("%d %d", &n, &m))) {
    printf("%d %d\n", n,m );
    SistemaLinear *SL = alocaSistLinear(n, m);
    TabelasPontos *TP = alocaTabelaPontos(n, m);
    lerTabelaPontos(TP, n, m);
    imprimeResultado(SL, TP);

  }
  return 0;
}
