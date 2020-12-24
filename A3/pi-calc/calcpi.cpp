/*
* Author: Aifaz Dhuka
* CPSC457 Assignment 3
* TA: Michelle Nguyen
* Modified version with thread implementation
* This program takes in radius and number of threads
* as an input and returns count and approximated pi value
*/

// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pi() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//

#include "calcpi.h"
#include <pthread.h>

//keeps a record of task the thread is supposed to do
struct Task {
  int start_row=-1;
  int end_row=-1;
  unsigned long int partial_count=0;
} tasks[256];


int radius = -1;

//called by the threads and returns the count by the thread to its task struct
void * doTask(void * i)
{
  double rsq = (double) radius * radius;
  int64_t id = (int64_t) i;
  int start = tasks[id].start_row ;
  int end = tasks[id].end_row;
  unsigned long int partial = 0;
  
  for( double x = start ; x < end ; x ++)
    for( double y = -radius ; y <= radius ; y ++)
      if( x*x + y*y <= rsq) partial ++;
  // once it's finished, we want the thread to exit
  tasks[id].partial_count=partial;
  pthread_exit(NULL);

}

uint64_t count_pi(int r, int n_threads)
{
  
  radius=r;
  pthread_t threads[n_threads];
  
  
    for( int64_t i = 0 ; i < n_threads ; i ++){
    //divides tasks equally for each thread
      tasks[i].start_row=(-r + (2*r+1) * i / n_threads);
      tasks[i].end_row=(-r + (2*r+1) * (i+1) / n_threads);
      //create thread with integer i as argument to the doTask funtion 
      //that would be used to get (or set) a particular task's values
      pthread_create(&threads[i], NULL, doTask, (void *) i);
    }
 
  //now it would join the threads and also add the values partial counts from the 
  //task struct to the count variable and return count
  uint64_t count = 0;
  for ( int i = 0 ; i < n_threads ; i ++){
    pthread_join( threads[i],NULL);
    count+=tasks[i].partial_count;
  }
  
  return count;
}
