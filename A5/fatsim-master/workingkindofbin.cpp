
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
  bool cycleFound = false;
  //int64_t numOfVisited=0;
  //visited.push_back(0);
  long next=0;
  // visited[0]=true;
  // stack.push(0);
  for (int i=0; (unsigned long int) i<fat.size(); i++){
    cycleFound=false;
    next=i;
    // visited[next]=true;
    // stack.push(next);
    // next=fat[i];
    
    long len=1;
    while (next!=-1){
      if (visited[next]){
        if (length[next]<=0){
          cycleFound = true;
          break;
        }else{
          len=length[next]+1;
          break;
        }
      }
      visited[next]=true;
      stack.push(next);
      next= fat[next];
    }
    if (cycleFound){
      while (!stack.empty()){
        long node =stack.top();
        length[node]=-1;
        cycleNodes.push_back(node);
        stack.pop();
      }
    }else{//reached -1 or a read node ie not part of a cycle
      while (!stack.empty()){
        length[stack.top()]=len;
        stack.pop();
        len++;
      }
    }
    
  }

  std::vector<long> lenVector;
  //printf("Length <>= [");
  for (auto & x : length){
    lenVector.emplace_back(-x.second);
  }
  //printf("]\n");
 /* printf("cycle = [");
  for (int i=0; (unsigned long int)i<cycleNodes.size();i++) printf("%ld, ",cycleNodes[i]);
    printf("]\n");
*/
  std::partial_sort(lenVector.begin(), lenVector.begin()+1,lenVector.end());//max_element(lenVector);
  printf("Fat size: %lu\n", fat.size());
  printf("Length size: %lu\n", length.size());
  longest_file_blocks = -lenVector[0];
  unused_blocks = cycleNodes.size();
  delete[] visited;
  //delete[] cycleNode
}

