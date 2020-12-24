// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================

#include "sumFactors.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

//bool threadsCreated = false;
volatile int cancel_flag=-1;
int numOfThreads=-1;
pthread_t threads[256];//max limit
pthread_barrier_t barrier;
pthread_mutex_t mutex;
int64_t num=-1;
struct Task {
  int64_t id=-1;
  int64_t start_num=-1;
  int64_t end_num=-1;
  int64_t sDiv=-1;
  
} tasks[256];

// calculates smallest divisor of n that is less than n
//
// returns 0 if n is prime or n < 2

void * doTask(void * obj){
  struct Task * t=(struct Task *) obj;
  int64_t i = t->start_num;
  int64_t max = t->end_num;
  //use id to cancel threads
  while( i <= max) {
    if (i>=num) break;
    if (num % i == 0) {
      t->sDiv=i;
      pthread_mutex_lock(&mutex);
      cancel_flag=t->id;
      pthread_mutex_unlock(&mutex);
      break;
    }
    if (i+2>=num) break;
    if (num % (i+2) == 0){
      t->sDiv= i + 2;
      pthread_mutex_lock(&mutex);
      cancel_flag=t->id;
      pthread_mutex_unlock(&mutex);
      break;
    }
    if (cancel_flag<=t->id) {
      pthread_cancel(pthread_self());
    }
    //i += 6; for this max-i % 6 must be 0
    i+=6  ;
  }
  //printf("--last i-value: %d--\n",(int)i);
  pthread_exit(NULL);
}

int64_t get_smallest_divisor(int64_t n)
{
  cancel_flag=-1;
  if( n <= 3) return 0; // 2 and 3 are primes
  if( n % 2 == 0) return 2; // handle multiples of 2
  if( n % 3 == 0) return 3; // handle multiples of 3
  int64_t i = 5;
  int64_t max = sqrt(n);
  int64_t po = ceil(ceil((max-i)/6)/numOfThreads);
  int64_t val=0;
  //if (!threadsCreated){
    //threadsCreated=true;
    if ((max-i)<0) return val;
    for (int j=0; j<numOfThreads; j++){
      tasks[j].id=j;
      num=n;
      tasks[j].sDiv=-1;
     
      int64_t begin =( i + (6*po)* (j));
      int64_t end = (( i + (6*po) * (j+1)));
            
      tasks[j].start_num=begin;
      tasks[j].end_num=end;
     
      //printf("task %d (max-%d): (%d,%d)\n",j,(int)max,(int)tasks[j].start_num,(int)tasks[j].end_num);
      pthread_create(&threads[j],NULL, doTask, (void *) &tasks[j]);
    }
    
    for (int j=0; j<numOfThreads;j++){
      pthread_join(threads[j],NULL);
      //printf("task %d sdiv= %d\n",(int)j, (int)tasks[j].sDiv);
    }
    for (int j=0;j<numOfThreads;j++){
      if (tasks[j].sDiv > 0) {
        val=tasks[j].sDiv;
        break;
      }
    }
    //}
  //printf("sDiv of %d = %d\n---------------\n",(int)n,(int)val);
  return val;
}

// reads in a positive integers in range (2 .. 2^63-1) from standard input
// for each number on input:
//   if the number is prime, it is ignored
//   determines the smallest factor of the composite number
// prints out the sum of all smallest factors
//
// if no composite numbers are found, it returns 0
//
// please note: the current implementation is single-threades, so
// n_threads argument is ignored
//
// you may modify this function
//
int64_t sum_factors(int n_threads)
{
  numOfThreads=n_threads;
  pthread_barrier_init(&barrier,NULL,n_threads);
  int64_t sum = 0;

  while( 1) {
    int64_t num;
    if( ! (std::cin >> num) ) break;

    int64_t div = get_smallest_divisor(num);
    //printf("sdiv of %d = %d\n",(int)num,(int)div);
    sum += div;
  }
  return sum;
}
