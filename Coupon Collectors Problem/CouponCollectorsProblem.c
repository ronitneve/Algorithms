//Ronit Neve
//1001856292
//Assignment 1

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long double fact(int n);
void CalDraws(int seed, int NofIterations, int NofFirstType, int NofSecondType, float PofFirstType, float PofSecondType);
long double nCr(int n, int r);

// Function to implement the nCr Formula.
long double nCr(int n, int r) {
  return fact(n) / (fact(r) * fact(n - r));
}

long double fact(int n) {
  long double res = 1;
  for (int i = 2; i <= n; i++)
    res = res * i;
  return res;
}
//CalDraws function is used for random simulation.
void CalDraws(int seed, int NofIterations, int NofFirstType, int NofSecondType, float PofFirstType, float PofSecondType) {
  srand(seed);
  int FlagFirstType = 0;
  int FlagSecondType = 0;
  long double sum = 0;
  int temp;
  float p = ((PofFirstType) * NofFirstType);
  for (int i = 0; i < NofIterations; i++) {

    int draws = 0;
    int f1 = 0, Flag1;
    int f2 = 0, Flag2;
    float pick;
    int array1[51] = {
      0
    };
    while (f1 < NofFirstType || f2 < NofSecondType) {
      // Randomly probablity is generated to get the coupon type
      pick = (rand() % 100000);
      pick = pick / 100000;
      if (pick <= p) {
        Flag1 = (rand() % NofFirstType);
        // Random First type Coupon is generated, an array is maintained to keep track of which coupon is found.
        if (array1[Flag1] == 0) {
          array1[Flag1] = 1;
          f1++;

        }

      } else {
        // Random Second type Coupon is generated, an array is maintained to keep track of which coupon is found.
        Flag2 = (rand() % NofSecondType) + NofFirstType;
        if (array1[Flag2] == 0) {
          array1[Flag2] = 1;
          f2++;

        }
      }
      draws++;
    }
    sum = sum + draws;
    //Total Draws is calculated
  }
  printf("Empirical boxes: %Lf", (sum / NofIterations));
}

// Driver code 
int main() {
  int NofCoupons;
  int NofFirstType;
  int NofSecondType;
  int seed;

  int NofIterations;
  long double PofFirstType;
  long double PofSecondType;
  printf("Enter total number of coupons and number of coupons of first type: \n");
  scanf("%d", & NofCoupons);
  scanf("%d", & NofFirstType);

  printf("Enter probability for each coupon of first type: \n");
  scanf("%Lf", & PofFirstType);

  NofSecondType = NofCoupons - NofFirstType;
  PofSecondType = (1 - (PofFirstType * NofFirstType)) / NofSecondType;
  printf("There are %d coupons of second type with probability %Lf \n", NofSecondType, PofSecondType);

  printf("Enter number of iterations for empirical: \n");
  scanf("%d", & NofIterations);

  printf("Enter seed: \n");
  scanf("%d", & seed);

  long double megaSum = 0.0;
  int countIterations = 0;
  for (int i = 0; i < NofCoupons; i++) {

    long double SUM = 0.0;
    for (int j = 0; j <= i; j++) {
      if ((NofFirstType < j) || (NofSecondType < (i - j))) {
        continue;
      }
      //This Eq implements the formula (14b)
      SUM = SUM + (long double) ((nCr(NofFirstType, j)) * (nCr(NofSecondType, (i - j)))) / (1 - (PofFirstType * j) - (PofSecondType * (i - j)));
      countIterations++;
    }
    megaSum = megaSum + (pow(-1, (NofCoupons - 1 - i)) * SUM);
  }

  printf("Theoretical: %Lf  Count: %d \n", megaSum, countIterations);
  CalDraws(seed, NofIterations, NofFirstType, NofSecondType, PofFirstType, PofSecondType);
}