/**********************************************************************************
 * Quick sort serial by 
 * Alessandro Piccolo and Anton Sjöberg
 *
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quick_sort(double *vector, int a, int b);
void swap(double *vector, double temp, int i, int j);
int dbgSorted(double* data, int len);
void print_vector(double *vector, int len);

int main(int argc, char *argv[]) {
  clock_t t_init, t_end;
  int i;
  double t_tot;
  double *data;
  int len = atoi(argv[1]);

  data = (double *)malloc(len*sizeof(double));

  /* Generate random numbers */
  srand(time(NULL));
  for (i = 0; i < len; i++) {
    data[i] = drand48();
  }

  /* Quick sort */
  t_init = clock();
  quick_sort(data, 0, len-1);
  t_end = clock();
  t_tot = (double)(t_end - t_init) / CLOCKS_PER_SEC;
  printf("Time elapsed: %f \n", t_tot);
  
  if (!dbgSorted(data, len)) {
    printf("Error not sorted \n");
  }
  
  return 0;
}

/*
 * Sorts a vector with recursive quick sort algorithm,
 * a and b defines the interval of what to sort.
 * b is also the index of our pivot element
 */
void quick_sort(double *vector, int a, int b) { 
  double pivot;
  int i, j;

  if(a < b) {
    pivot = vector[b];
    i = a;
    j = b;

    while(i < j) {
      while(vector[i] < pivot) {
        i++;
      }
      while(vector[j] >= pivot) {
        j--;
      }
      if(i < j) {
        swap(vector, vector[i], i, j);
      }
    }
    swap(vector, pivot, b, i);
    
    quick_sort(vector, a, i-1);
    quick_sort(vector, i+1, b);
  }
}

/*
 * Swaps elem in place i with j in vector
 */
void swap(double *vector, double temp, int i, int j) {
  vector[i] = vector[j];
  vector[j] = temp;
}

/*
 * Checks if the data is sorted
 */
int dbgSorted(double* data, int len) {
  int i;
  for (i = 0; i < len-1; ++i) {
    if (data[i] > data[i+1]) {
      return 0;
    }
  }
  return 1;
}

/*
 * Prints vector
 */
void print_vector(double* vector, int len) {
  int i;
  for (i = 0; i < len; i++) {
    printf("%f ", vector[i]);
  }
  printf("\n");
}
