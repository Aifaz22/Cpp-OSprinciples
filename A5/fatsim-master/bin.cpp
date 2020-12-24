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
  std::vector<long> cycleNodes;//bool* cycleNode= new bool[size];
  std::stack<long> stack;
  int64_t numOfVisited=0;
  //visited.push_back(0);
  long next = fat[0];
  visited[0]=true;
  stack.push(0);
  for (int i=0; (unsigned long int)i<fat.size(); i++){  
    if (numOfVisited>=fat.size()) break; //checks if all nodes are already visited or not
    if(!visited[i]){ //checks if a particular start point is visited before
      next = fat[i];
      visited[i]=true;
      numOfVisited++;
      stack.push(i);
    }else{
      continue;
    }
    while (next!=-1){ //loop till reached end
      if (visited[next] && (length[next]==0 || length[next]==-1)){
        //cycle condition-----> (is visited and either for fist time or have been recognized as a cycle)
        while (!stack.empty()){
          long node = next;
          length[node]=-1;
          cycleNodes.push_back(node);
          node= stack.top();
          stack.pop();
        }
        break;
      }
      if (visited[next] && length[next]>0){ //a memoized noncycle chain reached
        long len=length[next];
        while (!stack.empty()){
          len++;
          long node =stack.top();
          stack.pop();
          length[node]=len;
        }
        break;
      }
        visited[next]=true;
        numOfVisited++;
        stack.push(next);
        next = fat[next];

    }

    long len=0;
    while (!stack.empty()){ //runs only when no memoized point or cycle reached
      len++;
      long node =stack.top();
      stack.pop();
      length[node]=len;
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

