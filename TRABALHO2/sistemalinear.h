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
    real_t *y;
    real_t *valores;

} SistemaLinear;

typedef struct {
  real_t *tabelaValores;
  real_t *tabelaFuncoes;
} TabelasPontos;

SistemaLinear *alocaSistLinear(unsigned int n, unsigned int m);
TabelasPontos *alocaTabelaPontos(unsigned int n, unsigned int m);
void lerTabelaPontos(TabelasPontos *TP,unsigned int n, unsigned int m);
void matrizInterpolacao(SistemaLinear *SL, TabelasPontos *TP);
void trocaLinha(SistemaLinear *SL, TabelasPontos *TP, int i, int iPivo);
int encontraMax(SistemaLinear *SL, int i);
void triangulacao(SistemaLinear *SL, TabelasPontos *TP);
void matrizL(SistemaLinear *SL, TabelasPontos *TP);
void matrizU(SistemaLinear *SL, TabelasPontos *TP);
void fatoracaoLU(SistemaLinear *SL,TabelasPontos *TP);
void imprimeMatriz(real_t *matriz, unsigned int n, unsigned int m);
void imprimeResultado(SistemaLinear *SL, TabelasPontos *TP);
