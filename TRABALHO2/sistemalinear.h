#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef float real_t;

typedef struct {
    unsigned int n;
    unsigned int m;
    real_t *L;
    real_t *U;
    real_t *b;

} SistemaLinear;

typedef struct {
  real_t *tabelaValores;
  real_t *tabelaFuncoes;
} TabelasPontos;

SistemaLinear *alocaSistLinear(int n, int m);
TabelasPontos *alocaTabelaPontos(int n, int m);
void lerTabelaPontos(TabelasPontos *TP,int n, int m);
void imprimeResultado(SistemaLinear *SL, TabelasPontos *TP);
