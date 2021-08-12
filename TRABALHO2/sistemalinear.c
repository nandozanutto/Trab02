#include "sistemalinear.h"

SistemaLinear *alocaSistLinear(int n, int m) {

  SistemaLinear *SL = (SistemaLinear*)malloc(sizeof(SistemaLinear));
  SL-> n = n;
  SL-> m = m;
  SL->L = (real_t*)malloc(m * n * sizeof(real_t));
  SL->U = (real_t*)malloc(m * n * sizeof(real_t));
  SL->b = (real_t*)malloc(m * sizeof(real_t));

  return SL;
}

TabelasPontos *alocaTabelaPontos(int n, int m) {
  TabelasPontos *TP = (TabelasPontos*)malloc(sizeof(TabelasPontos));
  TP->tabelaValores = (real_t*)malloc(n * sizeof(real_t));
  TP->tabelaFuncoes = (real_t*)malloc(m * n * sizeof(real_t));
}

void lerTabelaPontos(TabelasPontos *TP,int n, int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    scanf("%f", &TP->tabelaValores[i]);
  }

  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      scanf("%f", &TP->tabelaFuncoes[i*m] + j);
    }
  }
}

void matrizInterpolacao(SistemaLinear *SL, TabelasPontos *TP) {
  int i, j, n, m;
  int multX;
  m = SL-> m;
  n = SL-> n;



  for (i = 0; i < m; i++) {
    multX = pow(TP->tabelaValores[i], )
    for (j = 0; j < n; j++) {
      printf("%f", TP->tabelaFuncoes[i*m] + j );
    }
    printf("\n");
  }
}

void imprimeResultado(SistemaLinear *SL, TabelasPontos *TP) {
  int i, j, n, m;
  m = SL-> m;
  n = SL-> n;

  for (i = 0; i < n; i++) {
    printf("%f ", TP->tabelaValores[i]);
  }
  printf("\n");

  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("%f", TP->tabelaFuncoes[i*m] + j );
    }
    printf("\n");
  }
}
