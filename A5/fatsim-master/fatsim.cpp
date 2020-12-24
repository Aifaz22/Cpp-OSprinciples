/*
* Author: Aifaz Dhuka
* CPSC457 Assignment 5
* TA: Michelle Nguyen
* Simulates fat simulator that basically 
* finds longest file blocks and number of blocks
* involved directly or indirectly in a cycle by 
* using DFS.
*/



#include "fatsim.h"
#include <cstdio>
#include <unordered_map>
#include <stack>
#include <algorithm>


void fat_sim(const std::vector<long> & fat, long & longest_file_blocks, long & unused_blocks)
{
  
  std::unordered_map<long, long> length;
  int64_t size = (int64_t)fat.size(); 
  bool* visited= new bool[size];
  
  //initialise all elements of array as false
  for (int i=0;(unsigned long int)i<fat.size();i++) visited[i]=false;
  
  std::vector<long> cycleNodes;
  std::stack<long> stack;
  bool cycleFound = false;
  
  long next=0;
  
  for (int i=0; (unsigned long int) i<fat.size(); i++){
    cycleFound=false;
    next=i;
    long len=1;
    //DFS*****************
    while (next!=-1){
      if (visited[next]){
        if (length[next]<=0){//break if part of cycle and set cycleFound as true
          cycleFound = true;
          break;
        }else{//break and set len as the length of the memoised node+1
          len=length[next]+1;
          break;
        }
      }
      visited[next]=true;
      stack.push(next);
      next= fat[next];
    }
    if (cycleFound){//add all elements from stack to umap with length as -1
      while (!stack.empty()){
        long node =stack.top();
        length[node]=-1;
        cycleNodes.push_back(node);
        stack.pop();
      }
      
    }else{//no cycle then add elements from stack to umap with their lengths
      while (!stack.empty()){
        length[stack.top()]=len;
        stack.pop();
        len++;
      }
    }   
  }

  std::vector<long> lenVector;
  //get all lengths to vectors
  for (auto & x : length){
    lenVector.emplace_back(-x.second);
  }
  
  std::partial_sort(lenVector.begin(), lenVector.begin()+1,lenVector.end());//get the largest value at the first index
  
  if (-lenVector[0]<0){
    longest_file_blocks=0;
  }else{
    longest_file_blocks = -lenVector[0];
  }
  unused_blocks = cycleNodes.size();
  delete[] visited; //free array 
}

