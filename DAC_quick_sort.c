/**********************************************************************************
 * Divide and conquer quick sort
 * by Alessandro Piccolo and Anton Sjöberg
 *
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void quick_sort(double *vector, int a, int b);
void *quick_sort_pthread(void *arg);
void swap(double *vector, double temp, int i, int j);
void barrier();
int  dbgSorted(double* data, int len);
void print_vector(double *vector, int len);

int MAX_LEVEL;
volatile int number_threads = 0;
int number_threads_total = 0 ;
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mysignal;

typedef struct $
{
  int a;            /* Starting point of the vector  */
  int b;            /* End point of the vector       */
  double* vector;   /* The vector                    */
  int lvl;          /* The level                     */
} input_args;

int main(int argc, char *argv[]) {
  clock_t t_init, t_end;
  pthread_t first_thread;
  int i;
  double t_tot;                /* Time elapsed for qs-algorithm */
  double *data;                /* Data-vector to sort           */
  int len = atoi(argv[1]);     /* Length of vector to sort      */
  MAX_LEVEL = atoi(argv[2]);   /* Max depth lvl                 */
  
  /* Initialize and allocate */
  pthread_mutex_init(&mutex_lock,NULL);
  pthread_cond_init(&mysignal,NULL);
  data = (double *)malloc(len*sizeof(double));

  /* Generate random numbers */
  srand(time(NULL));
  for (i = 0; i < len; i++) {
    data[i] = drand48();
  }
  
  /* Quick sort with multiple threads */
  t_init = clock();
  input_args *arg = (input_args *)malloc(sizeof(input_args));
  arg->vector = data;
  arg->a = 0;
  arg->b = len-1;
  arg->lvl = 0;

  /* Start of quick sort algorithm om mult pthreads */ 
  number_threads++;
  number_threads_total++;
  pthread_create(&first_thread,NULL,quick_sort_pthread,(void *) arg);
  barrier();
  t_end = clock();
  t_tot = (double)(t_end - t_init) / CLOCKS_PER_SEC;
  printf("Time elapsed: %f \n", t_tot);
  printf("Total number of threads %d \n", number_threads_total);

  //print_vector(data, len);
  if (!dbgSorted(data, len)) {
    printf("Error not sorted \n");
  }
  free(arg);
  return 0;
}

/*
 * Sorts a vector with recursive quick sort algorithm,
 * a and b defines the interval of what to sort.
 * b is also the index of our pivot element
 */
void quick_sort(double *vector, int a, int b) { 
  int i, j;
  double pivot;

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
 * Sorts a vector with recursive quick sort algorithm,
 * a and b defines the interval of what to sort.
 * b is also the index of our pivot element, in a non-serial
 * manner.  
*/ 
void *quick_sort_pthread(void *input_arg) { 
  input_args *arg = (input_args *) input_arg;
  int i, j;
  double pivot;
  double* vector = arg->vector;
  pthread_t thread;
  
  /* Quick sort algorithm */
  if(arg->a < arg->b) {
    pivot = vector[arg->b];
    i = arg->a;
    j = arg->b;

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
    swap(vector, pivot, arg->b, i);
    
    if(arg->lvl < MAX_LEVEL){
      /* Dividing the vector into two new ones */
      input_args *a_arg = (input_args *)malloc(sizeof(input_args));
      a_arg->a = arg->a;
      a_arg->b = i-1;
      a_arg->vector = vector;
      a_arg->lvl = arg->lvl+1;

      input_args *b_arg = (input_args *)malloc(sizeof(input_args));
      b_arg->a = i+1;
      b_arg->b = arg->b;
      b_arg->vector = vector;
      b_arg->lvl = arg->lvl+1;

      /* Starts new thread */
      pthread_mutex_lock(&mutex_lock);
      number_threads++;
      number_threads_total++;
      pthread_mutex_unlock(&mutex_lock);
      pthread_create(&thread, NULL, quick_sort_pthread,(void *) a_arg);
      quick_sort_pthread((void *) b_arg);
    }
    else {
      quick_sort(vector, arg->a, i-1);
      quick_sort(vector, i+1, arg->b);
    }
  }

  pthread_mutex_lock(&mutex_lock);
  number_threads--;
  pthread_mutex_unlock(&mutex_lock);
  
  free(arg);
  pthread_exit(NULL);
}

/*
 * Swaps elem in place i with j in vector
 */
void swap(double *vector, double temp, int i, int j) {
  vector[i] = vector[j];
  vector[j] = temp;
}

/*
 * Barrier function. usleep might be 
 * commented away to get rid of the sleep.
 */
void barrier() {   
  while (number_threads > 0) {
    usleep(5);
  }  
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
