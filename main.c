#include "ncursesUtils.h"
#include <ncurses.h>
#include <stdio.h>

int extendedEuclides(int, int, int *, int *);
  int alfa = 0, beta = 0, n = 0;
  int x, y, mcd = 0;

int main(int argc, char *argv[]) {
  startNcurses();
  char auxCharGeneral[256];
  do {
    int errCode = getNumber("Ingresa alfa:", &alfa);
    if (errCode != 0)
      handleError(errCode);
    errCode = getNumber("Ingresa beta:", &beta);
    if (errCode != 0)
      handleError(errCode);
    errCode = getNumber("Ingresa n:", &n);
    if (errCode != 0)
      handleError(errCode);

    mcd = extendedEuclides(alfa, n, &x, &y);
    if (mcd != 1) {
      sprintf(auxCharGeneral,"MCD(%d,%d) != 1, no son coprimos, ingresa otro numero!", alfa, n);
      showErrorMessage(auxCharGeneral);
    }

  } while (mcd != 1);
  
  if( x < 0 ) x+= n;
  if( y < 0 ) x+= n;
  int minusBeta = (n-beta)%n;
  if (minusBeta < 0) minusBeta += n;

  char results[9][256];
  sprintf(results[8], " ");
  if( alfa > n && beta < n )
    sprintf(results[8], "Alfa fue mayor a n, por lo que fue reducido");
  if (alfa < n && beta > n )
    sprintf(results[8], "Beta fue mayor a n, por lo que fue reducido");
  if(alfa > n && beta > n)
    sprintf(results[8], "Alfa y beta fueron mayores a n, por lo que fueron reducidos");
  sprintf(results[0],"EL ALGORITMO EXTENDIDO DE EUCLIDES OBTUVO:\n");
  sprintf(results[1],"(%d*%d) + (%d*%d) = %d", alfa, x, n, y, mcd);
  sprintf(results[2], "FORMULA DE CIFRADO:");
  beta = beta%n; alfa = alfa%n;
  sprintf(results[3], "C = (%dp + %d) mod %d",alfa,beta,n);
  sprintf(results[4],"FORMULA DE DESCIFRADO 1:");
  sprintf(results[5],"p = %d(C + %d) mod %d",x,minusBeta,n);
  sprintf(results[6],"FORMULA DE DESCIFRADO 2:");
  sprintf(results[7],"p = (%dC + %d) mod %d",x,(x*(minusBeta))%n,n);


  showResults(9,
              results[0],
              results[1],
              results[2],
              results[3],
              results[4],
              results[5],
              results[6],
              results[7],
              results[8]);

  endwin();

  return 0;
}

int extendedEuclides(int a, int b, int *x, int *y) {
  if (b == 0) {
    *x = 1;
    *y = 0;
    return a;
  }
  int x1, y1;
  int mcd = extendedEuclides(b, a % b, &x1, &y1);
  *x = y1;
  *y = x1 - (a / b) * y1;
  return mcd;
}
