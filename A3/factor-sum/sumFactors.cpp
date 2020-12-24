/*
* Author: Aifaz Dhuka
* CPSC457 Assignment 1
* TA: Michelle Nguyen
* Modified version with thread implementation, thread 
* cancellation, mutexes and thread reuse (barriers)
* This program takes in numbers as an input and
* returns the sum of smallest divisors.
*/

#include "sumFactors.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

pthread_barrier_t myBarrier;
pthread_mutex_t myMutex;
int64_t num;
int numOfThreads=0;
volatile int cancel_flag=-1;
int64_t sum = 0;
bool read_suc=false;

struct Param{
    int threadID;
};
struct task{
  int64_t sDiv=-1;
}tasks[265];


// calculates smallest divisor of n that is less than n
//
// returns 0 if n is prime or n < 2
//
// examples:
//    n =  1         --> result = 0
//    n =  2         --> result = 0
//    n =  9         --> result = 3
//    n = 35         --> result = 5
//    n = 8589934591 --> result = 7
//
//not parallelized when not working with multiple threads
int64_t old_get_smallest_divisor(int64_t n)
{
  if( n <= 3) return 0; // 2 and 3 are primes
  if( n % 2 == 0) return 2; // handle multiples of 2
  if( n % 3 == 0) return 3; // handle multiples of 3
  int64_t i = 5;
  int64_t max = sqrt(n);
  while( i <= max) {
    if (n % i == 0) return i;
    if (n % (i+2) == 0) return i + 2;
    i += 6;
  }
  return 0;
}

// parallelized version old_get_smallest_divisor when working with threads

int64_t get_smallest_divisor(int64_t n, int64_t tid)
{
  if( n <= 3) return 0; // 2 and 3 are primes
  if( n % 2 == 0) return 2; // handle multiples of 2
  if( n % 3 == 0) return 3; // handle multiples of 3
  int64_t i = 5;
  int64_t max = sqrt(n);
  int64_t val =0;
  if ((max-i)<0) return val;
  //we calculate the task based on the threadid
  int64_t po = ceil(ceil((max-i)/6)/numOfThreads);
  int64_t begin =( i + (6*po)* (tid));
  int64_t end = (( i + (6*po) * (tid+1)));
  tasks[tid].sDiv=-1; //assigning smallest div to an invalid number
  
  //we only run while begin is less than equal to end
  while( begin <= end) {
    if (begin>=n) { //if we are going over the range required to calculate --> break
      break;
    }

    if (n % begin == 0){
      //if begin is small div, change task's sdiv
      //and set cancel flag to the tid
      //using mutex we avoid having race conditions while updating the shared global variable
     
      tasks[tid].sDiv=begin;
       pthread_mutex_lock(&myMutex);////////////////////////
      if (cancel_flag==-1 || cancel_flag>tid) cancel_flag=tid;
      pthread_mutex_unlock(&myMutex);
      break;
    }
    if (begin+2>=n) break;
    if (n % (begin+2) == 0) {
      //if begin+2 is small div, change task's sdiv
      //and set cancel flag to the tid
      //using mutex we avoid having race conditions while updating the shared global variable
     
      tasks[tid].sDiv=begin+2;
      pthread_mutex_lock(&myMutex);/////////////
      if (cancel_flag==-1 || cancel_flag>tid) cancel_flag=tid;
      pthread_mutex_unlock(&myMutex);
      break;
    }
    //thread checks, if it has to cancel itself
    if (cancel_flag<=tid && cancel_flag!=-1) {
      break;
    }
    begin += 6;
  }
  //getting the smallest div from all the tasks
  for (int j=0;j<numOfThreads;j++){
      if (tasks[j].sDiv > 0) {
        val=tasks[j].sDiv;
        break;
      }
    }
  return val;
}

//checks if we read successfully or not
bool read() {
    if( ! (std::cin >> num) ){
        return false;
    }
    else{
        return true; 
      }
}

//reads from input and then call get smallest divisor and add it to the sum
void * thread_start(void * param){
  struct Param * threadParam = ((struct Param *)param);
  int64_t tid = threadParam->threadID;
  int64_t div=0;

  
  
  while( 1) {
    int res= pthread_barrier_wait(&myBarrier);
    //do serial work
    if( res == PTHREAD_BARRIER_SERIAL_THREAD){
      cancel_flag=-1;
      read_suc=read();
      sum+=div;
    }
    pthread_barrier_wait(&myBarrier);
    //do parallel work
    if (read_suc==true){
      div = get_smallest_divisor(num, tid);
    }else{
      break;
    }
  }
  pthread_exit(NULL);
}

// reads in a positive integers in range (2 .. 2^63-1) from standard input
// for each number on input:
//   if the number is prime, it is ignored
//   determines the smallest factor of the composite number
// prints out the sum of all smallest factors
//
// if no composite numbers are found, it returns 0
//
int64_t sum_factors(int n_threads)
{
  if (n_threads!=1){ //threads required
    //initializes barriers and mutexes
    pthread_barrier_init(&myBarrier, NULL, n_threads);
    pthread_mutex_init(&myMutex,NULL);
    numOfThreads=n_threads;
    //create thread params
    struct Param threadParams[n_threads];
    // create threads
    pthread_t threadPool[n_threads];
    for (int i = 0; i < n_threads; i++)
    {
        threadParams[i].threadID = i;
        //send a thread to do thread_start with tID as argument to get the task value
        pthread_create(&threadPool[i], NULL, thread_start, &threadParams[i]);
    }
    for (int i = 0; i < n_threads; i++)
    {
      //join all threads
        pthread_join(threadPool[i], NULL);
    }
    //destroy mutex
    pthread_mutex_destroy(&myMutex);
    //retun the final value
    return sum;
  }else{//threads not required
    int64_t o_sum = 0;
    while( 1) {
      int64_t o_num;
      if( ! (std::cin >> o_num) ) break;
      int64_t o_div = old_get_smallest_divisor(o_num);
      //printf("sdiv of %d = %d\n",(int)num,(int)div);
      o_sum += o_div;
    }
    return o_sum;
  }
}
