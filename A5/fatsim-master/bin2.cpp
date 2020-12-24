// this is the file you need to edit

#include "fatsim.h"
#include <cstdio>
#include <unordered_map>
#include <stack>
#include <algorithm>

// reimplement this function
void fat_sim(const std::vector<long> & fat, long & longest_file_blocks, long & unused_blocks)
{
  
  std::unordered_map<long, long> length;
  int64_t size = (int64_t)fat.size(); 
  bool* visited= new bool[size+1];
  std::vector<long> cycleNodes;//bool* cycleNode= new bool[size];
  std::stack<long> stack;
  //int64_t numOfVisited=0;
  //visited.push_back(0);
  long next;
  for (int i=0; (unsigned long int)i<fat.size(); i++){  
    next = fat[i];
    visited[i]=true;
    stack.push(i);
    if (!visited[next]){
      while(1){
        visited[next]=true;
        stack.push(next);
        next=fat[next];
        if (next==-1){
          stack.pop();
          long len = 1;
          while (!stack.empty()){
            next=stack.top();
            stack.pop();
            length[next]=len;
            len++;
          }
          break;
        }
        if (visited[next]){
          if (length[next]==-1){//cycle
            while (!stack.empty()){
              length[next]=-1;
              cycleNodes.push_back(next);
              stack.pop();
              next= stack.top();
            }
          }else if (length[next]>0){
            long len = length[next]+1;
            while (!stack.empty()){
              next=stack.top();
              stack.pop();
              length[next]=len;
              len++;
            }
          }
          break;
        }
      }
    }else{
      if (length[next]==-1){//cycle
        while (!stack.empty()){
          length[next]=-1;
          cycleNodes.push_back(next);
          stack.pop();
          next= stack.top();
        }
      }else if (length[next]>0){
        long len = length[next]+1;
        while (!stack.empty()){
          next=stack.top();
          stack.pop();
          length[next]=len;
          len++;
        }
      }else if (length[next]==0){
        printf("EROOOOOOOOOOOOOOOOOOOOOOOOOOOOORRRRRR\n");
      }
    }
  }

  //std::vector<long> lenVector;
  printf("Length <>= [");
  for (auto & x : length){
    printf("%ld == %ld, ",x.first,x.second);//.emplace_back(-x.second);
  }
  printf("]\n");
  printf("cycle = [");
  for (int i=0; (unsigned long int)i<cycleNodes.size();i++) printf("%ld, ",cycleNodes[i]);
    printf("]\n");

  //std::partial_sort(lenVector.begin(), lenVector.begin()+1,lenVector.end());//max_element(lenVector);
  
  longest_file_blocks = 10;//-lenVector[0];
  unused_blocks = cycleNodes.size();
  delete[] visited;
  //delete[] cycleNode
}

