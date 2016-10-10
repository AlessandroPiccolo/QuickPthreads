/**********************************************************************************
 * Divide and conquer quick sort
 * by Alessandro Piccolo, Anton Sjöberg and Jonatan Bergquivst
 *
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void *quick_sort_pthread(void *arg);
void quick_sort(double *vector, int a, int b);
void swap(double *vector, double temp, int i, int j);
int  dbgSorted(double* data, int len);
void print_vector(double *vector, int len);

typedef struct $
{
  int a;            /* Starting point of the vector  */
  int b;            /* End point of the vector       */
  double* vector;   /* The vector                    */
} input_args;

int main(int argc, char *argv[]) {
  clock_t t_init, t_end;
  
  int len = atoi(argv[1]);        /* Length of vector to sort              */
  int n_threads = atoi(argv[2]);  /* Number of threads                     */
  int i, j;                       
  int n_elem = 0;                 /* Number of elem in different buckets   */
  int n_elem_previous = 0;        /* Previous buckets nr of elements       */
  
  double t_tot;                   /* Time elapsed for qs-algorithm         */
  double *data;                   /* Data-vector to sort                   */
  double interval = 1.d/n_threads;  /* Defined homogenous interval           */
  input_args args[n_threads];     /* Arguments to pass to the threads      */
  pthread_t threads[n_threads];
  data = (double *)malloc(len*sizeof(double));

  /* Generate random numbers */
  srand(time(NULL));
  for (i = 0; i < len; i++) {
    data[i] = drand48();
  }
  
  /* Quick sort with multiple threads */
  t_init = clock();
  /* Sort elements to the buckets */ 
  for (i = 0; i < n_threads; i++) {
    if (i != n_threads-1) {
      for (j = n_elem; j < len; j++) {
        if (data[j] <= (i+1)*interval) {
          swap(data, data[j], j, n_elem);
          n_elem++;
        }
      }
      args[i].a = n_elem_previous;
      args[i].b = n_elem-1;
      args[i].vector = data;
      pthread_create(&threads[i], NULL, quick_sort_pthread, (void *) &args[i]);
      n_elem_previous = n_elem;
    }
    else { /* Do not have to swap elements for last bucket */
      args[i].a = n_elem;
      args[i].b = len-1;
      args[i].vector = data;
      pthread_create(&threads[i], NULL, quick_sort_pthread, (void *) &args[i]);
    } 
  }
  for(i=0; i < n_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  
  t_end = clock();
  t_tot = (double)(t_end - t_init) / CLOCKS_PER_SEC;
  printf("Time elapsed: %f \n", t_tot);

  if (!dbgSorted(data, len)) {
    printf("Error not sorted \n");
  }
  free(args);
  free(data);
  return 0;
}

/*
 * Prepare and starts quick sort algorithm  
 */ 
void *quick_sort_pthread(void *input_arg) { 
  input_args *arg = (input_args *) input_arg;
  quick_sort(arg->vector, arg->a, arg->b);
  pthread_exit(NULL);
}

/*
 * Sorts a vector with recursive quick sort algorithm,
 * a and b defines the interval of what to sort.
 * b is also the index of our pivot element
 */
void quick_sort(double *vector, int a, int b) { 
  if(a < b) {
    double pivot = vector[b];
    int i = a;
    int j = b;

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
