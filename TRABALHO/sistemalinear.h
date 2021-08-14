#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef float real_t;

typedef struct {
  int n, m;
  real_t *tabelaValores;
  real_t *tabelaFuncoes;
} TabelasPontos;


typedef struct
{
    unsigned int n, m; // tamanho do SL
    real_t *A;     // matriz dos coeficientes
    real_t *b; // termos independentes
    real_t *L;
} SistLinear_t;


TabelasPontos *alocaTabelaPontos(unsigned int n, unsigned int m);
void lerTabelaPontos(TabelasPontos *TP);
void imprimeTabela(TabelasPontos *TP);
SistLinear_t *alocaSistLinear(unsigned int n, unsigned int m);
SistLinear_t * lerSistemaInter(TabelasPontos *TP);
void imprimeSistema(SistLinear_t *SL, int funcLine);
int encontraMax(SistLinear_t *SL, int i);
void trocaLinha(SistLinear_t *SL, int i, int iPivo);
void escalona (SistLinear_t *SL);
void imprimeLU(SistLinear_t *SL);
void multiplyLU(SistLinear_t *SL);
void retrossubsU(real_t *A, real_t *b, real_t *x, int n, int funcLine);
void retrossubsL(real_t *A, real_t *b, real_t *x, int n, int funcLine);
void fatoracaoLU(SistLinear_t *SL);