#include "sistemalinear.h"

SistemaLinear *alocaSistLinear(unsigned int n, unsigned int m) {

  SistemaLinear *SL = (SistemaLinear*)malloc(sizeof(SistemaLinear));
  SL-> n = n;
  SL-> m = m;
  SL->L = (real_t*)malloc(m * n * sizeof(real_t));
  SL->U = (real_t*)malloc(m * n * sizeof(real_t));
  SL->y = (real_t*)malloc(m * sizeof(real_t));
  SL->valores = (real_t*)malloc(m * sizeof(real_t));


  return SL;
}

TabelasPontos *alocaTabelaPontos(unsigned int n, unsigned int m) {
  TabelasPontos *TP = (TabelasPontos*)malloc(sizeof(TabelasPontos));
  TP->tabelaValores = (real_t*)malloc(n * sizeof(real_t));
  TP->tabelaFuncoes = (real_t*)malloc(n * n * sizeof(real_t));
}

void lerTabelaPontos(TabelasPontos *TP, unsigned int n, unsigned int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    scanf("%f", &TP->tabelaValores[i]);
  }

  for (i = 0; i < m; i++) {
    for (j = 0; j < m; j++) {
      scanf("%f", &TP->tabelaFuncoes[i*m] + j);
    }
  }
}


// void matrizAjuste(SistemaLinear *SL, TabelasPontos *TP) {
//   SL->A[0][0] = n;
//
//   real_t aux=1;
//   for(int i=0; i<n; i++){
//       for(int j=1; j<n; j++){
//           aux = aux*matriz[0][i];
//           SL->A[0][j] += aux;
//       }
//
//       for(int j=1; j<n; j++){
//           aux = aux*matriz[0][i];
//           SL->A[j][n-1] += aux;
//       }
//       aux=1;//reseta aux pra calcular Xi+1 X²i+1..
//   }
//
//   //copiando valores já calculados
//   for(int i=1; i<n; i++){
//   /*copia os somatorios da primeira linha
//   Observe que o elemento 0 faz 0 cópias. O elemento 1 faz 1 cópia
//   j é o numero de cópias.
//   copia o elemento i para a diagonal do mesmo*/
//       for(int j=0; j<i; j++){
//           SL->A[1+j][i-(j+1)] = SL->A[0][i];
//       }
//   }
//
//   for(int i=n-2; i>-1; i--){
//   /*Observe que o elemento n-1 faz 0 cópias. O elemento n-2 faz 1 cópia
//   j é o numero de cópias
//   copia o elemento i para a diagonal do mesmo*/
//       for(int j=0; j<n-1-i; j++){
//           SL->A[i+j+1][n-1-(j+1)] = SL->A[i][n-1];
//       }
//   }
// }
//


void matrizInterpolacao(SistemaLinear *SL, TabelasPontos *TP) {
  int i, j, n, m;
  int multX;
  m = SL-> m;
  n = SL-> n;

  // SL->b = &TP->tabelaFuncoes;
  for(i = 0; i < n; i++){
      SL->U[i * n] = 1;
      SL->U[i * n + 1] = TP->tabelaValores[i];
      printf("%d\n",(i*n)+1 );
  }
  for(i = 0; i < n; i++){


      for(j = 2; j < n; j++){
          SL->U[i * n + j] = SL->U[i * n + j - 1] * SL->U[i * n + j - 1];
          printf(".%d %d %d\n",(i*n)+j, (i*n)+j-1, (i*n)+m  );
      }
  }
}


void matrizL(SistemaLinear *SL, TabelasPontos *TP) {
  int i, j;
  int n = SL->n;
  SL->y[0] = TP->tabelaValores[0] ;
  // printf("===========================\n");
  // imprimeMatriz(SL->L, n, n);
  // printf("===========================\n");

  for (i = 1; i < n; i++){
    real_t soma = 0;
    for (j = 0; j < n ; j++) {
      if (i != j) {
        soma += SL->y[j] * SL->L[(i * n) + j];
        // printf("%f %f %d \n", SL->y[j] , SL->L[i * n + j], i * n + j);

      }
    }
    // printf("%f\n", soma);
    SL->y[i] = (TP->tabelaValores[i] - soma) /SL->L[i * n + i];
   printf(" ==> %f\n",SL->y[i] );
  }
}

void matrizU(SistemaLinear *SL, TabelasPontos *TP) {
     int i, j;
     int n = SL->n;
     SL->valores[n-1] = SL->y[n-1]/SL->U[(n * n) -1];
     printf("===========================\n");
     imprimeMatriz(SL->U, n, n);
     printf("===========================\n");
     for (i = n - 2; i >= 0; i--) {
       real_t soma = 0;
       for (j = n - 1; j >= 0; j--){
         if ((i!=j) && (SL->U[(i * n)+j] != 0.0)) {
           soma += SL->U[(i * n) + j +1] * SL->y[j];
           printf("----%f %f %d \n", SL->U[(i * n)+j] , SL->y[j], i * n +j);
         }


       }
       SL->valores[i] = (SL->y[i] - soma)/ SL->U[i * n + i];
        printf(" ==> %f\n",SL->valores[i] );
     }


}

void fatoracaoLU(SistemaLinear *SL, TabelasPontos *TP) {
  int i;
  int n = SL->n;

    matrizL(SL, TP);
    matrizU(SL, TP);
    for (i = 0; i < n; i++) {
      printf(".%f\n", SL->valores[i]);
    }
}


int encontraMax(SistemaLinear *SL, int i){
    int copiaI = i;
    int iPivo = i;
    int n = SL->n;
    while (copiaI < n - 1)
    {
        if (fabs(SL->U[(copiaI + 1)*n +i]) > fabs(SL->U[iPivo*n +i]))
        {
            iPivo = copiaI + 1;
        }
        copiaI++;
    }

    return iPivo;
}

void trocaLinha(SistemaLinear *SL, TabelasPontos *TP, int i, int iPivo){

    int j;
    int n = SL->n;
    real_t aux;
    for (int j = i; j < n; j++) {
        aux = SL->U[i * n + j];
        SL->U[i*n +j] = SL->U[iPivo* n +j];
        SL->U[iPivo* n +j] = aux;
    }
    aux = TP->tabelaValores[i];
    TP->tabelaValores[i] = TP->tabelaValores[iPivo];
    TP->tabelaValores[iPivo] = aux;
}

void triangulacao(SistemaLinear *SL, TabelasPontos *TP) {
  int i, j, k, iPivo;
  int n = SL->n;
  real_t m;
  for (i = 0; i < n; i++) {
    iPivo = encontraMax(SL, i);
    if (i != iPivo){
        trocaLinha(SL,TP, i, iPivo);
    }
    SL->L[i * n + i] = 1;
    for (j = i + 1; j < n; j++) {

      m = SL->U[j * n + i]/SL->U[i * n + i];
      SL->U[j * n + i] = 0;
      for (k = i + 1; k < n; k++) {
        SL->U[n*k+j] -= SL->U[i*n +j]* m;
      }
      SL->L[j*n +i] = m;
    }
  }

  printf("\n");
  imprimeMatriz(SL->U, n, n);
  printf("\n\n" );
  imprimeMatriz(SL->L, n, n);
  printf("\n\n" );
}

void imprimeMatriz(real_t *matriz, unsigned int n, unsigned int m){
  int i,j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("%f ", matriz[(i * n)+ j] );
    }
    printf("\n");
  }
}

void imprimeResultado(SistemaLinear *SL, TabelasPontos *TP) {
  int i, j, n, m;

  m = SL-> m;
  n = SL-> n;


  // for (i = 0; i < n; i++) {
  //   printf("%f ", TP->tabelaValores[i]);
  // }
  // printf("\n");
  //
  // for (i = 0; i < m; i++) {
  //   for (j = 0; j < n; j++) {
  //     printf("%f", TP->tabelaFuncoes[(i * n)+ j] );
  //   }
  //   printf("\n");
  // }


  for (i = 0; i < m; i++) {
     printf("%f", SL->valores[i]);
    for (j = 1; j < n; j++) {
      printf("%f",  SL->valores[j]*pow(TP->tabelaValores[j], j) );
    }
    printf("\n");
  }
}
