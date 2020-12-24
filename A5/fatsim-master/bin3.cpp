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
  bool* visited= new bool[size];
  long* memory= new long[size];
  std::vector<long> cycleNodes;
  
  bool cycleFound = false;
  long next=0;
  
  for (int i=0; (unsigned long int)i<fat.size();i++){
  //DFS****************************************************************************
    std::stack<long> stack;
    next=i;
    while (next==-1){
      if (visited[next]) break;
      stack.push(next);
      next=fat[next];
    }
  ///******************************************************************************
    while (!stack.empty())
    {
      if (next==-1){
        memory[stack.top()]=0;
      }
    }
    
  }

  delete[] visited;
}

