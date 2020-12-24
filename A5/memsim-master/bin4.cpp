// this is the file you need to edit

#include "memsim.h"
#include <cstdio>
#include <list>
#include <set>
#include <unordered_map>
#include <math.h>
#include <iostream>

struct Partition {
  long tag ;
  long size ;
  int64_t addr ;
};
typedef std::list<Partition>::iterator PartitionRef;
struct scmp {
  bool operator()( const PartitionRef & c1, const PartitionRef & c2) const {
    if( c1-> size == c2-> size)
      return c1-> addr < c2-> addr;
    else
      return c1-> size < c2-> size;
  }
};

// feel free to use this class as a starting point

struct Simulator {
  Simulator(int64_t page_size) { page_size_ = page_size; }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  void allocate(int tag, int size) { 
    //std::cout << "--------------------------------------------"<< std::endl;
    //printf("started\n");
    //bool sizeSufficient =false;
    bool mergeToEnd = false;
    PartitionRef fit_node_i;
    Partition dummy_part;
    dummy_part.size=size;
    std::list<Partition> dummy { dummy_part };
    // Use std::set lower bound member function
    auto fit_i = free_blocks.lower_bound( dummy.begin() );
    PartitionRef toSplit=*fit_i;
    
    // for (PartitionRef it = partitions.begin(); it != partitions.end(); ++it){
    //   if (it->tag==0 && it->size>=size){
    //     sizeSufficient =true;
    //     if (toSplit->size>it->size){
    //       toSplit=it;//use this partition if it is a good fit
    //     }
    //   }
    // }
    if (fit_i==free_blocks.end()){ //need pages
      //printf("Need More Memory!!!!!\n");
      int64_t freeEndMemory=0;
      if (partitions.back().tag==0){
        freeEndMemory+=partitions.back().size;
        mergeToEnd=true;
      }
      while (freeEndMemory<size){ //keep adding pages
        freeEndMemory+=page_size_;
        numOfPages+=1;
      }
      //merge to prev empty
      if (mergeToEnd){
        partitions.back().size=freeEndMemory;
        free_blocks.erase(std::prev(partitions.end()));
      }else{ //new partition required
        Partition prevPartition= partitions.back();
        Partition newPartition;
        newPartition.size=freeEndMemory;
        newPartition.addr=prevPartition.addr+prevPartition.size;
        newPartition.tag=0;
        partitions.push_back(newPartition);
      }
      //page creation done///////////////////////////////////////////////

      toSplit=std::prev(partitions.end());//newly added
      free_blocks.insert(toSplit);
      //printf("Memory Added!\n");
      //printList();

    }
    /*
              ****************************::::::::::::::ISSUE::::::::::::*******************************
              The adress is always 0, it does not update
              *******************************************************************************************
    */
    //std::cout << "Address to store: " << toSplit->addr << std::endl;
    if (toSplit->size==size){
      //no split required
      toSplit->tag=tag;
      free_blocks.erase(toSplit);
    }else if (toSplit->size>size){
      //split required
      //create a Partitionto fill the memory
      Partition newFullPartition;
      newFullPartition.size=size;
      newFullPartition.addr=toSplit->addr;
      newFullPartition.tag=tag;
      //update address and size of the previously empty partition
      toSplit->addr=newFullPartition.addr+newFullPartition.size;
      toSplit->size-=size;
      PartitionRef pos = toSplit;
      //insert new partition in front of toSplit
      partitions.insert(pos,newFullPartition);
      free_blocks.erase(toSplit);
      free_blocks.insert(toSplit);
    }
    //printList();
    //std::cout << "--------------------------------------------"<< std::endl;
  }
  
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void deallocate(int tag) { 
    //std::cout << "--------------------------------------------"<< std::endl;
    for (std::list<Partition>::iterator it = partitions.begin(); it != partitions.end(); ++it){
       if (it->tag==tag){//look for the same tags
         it->tag=0; //give it free tag
         if (it==partitions.begin()){ //if we are at the first partition, look if next is free. If So merge
           //printf("3");
           auto next = std::next(it);
           if (next->tag==0){
             it->size+=next->size;
             free_blocks.erase(next);
             partitions.erase(next);
             
           }
           //printf("3\n");
         } else if (it==std::prev(partitions.end())){//if we are at the last partition, look if prev is free. If So merge
           //printf("4");
           auto prev = std::prev(it);
           if (prev->tag==0){
             it->size+=prev->size;
             it->addr=prev->addr;
             free_blocks.erase(prev);
             partitions.erase(prev);
           }
           //printf("4\n");
         }else{ //look if next or/and prev is free. If So merge
           //printf("5");
           auto prev = std::prev(it);
           auto next = std::next(it);
           if (prev->tag==0 && next->tag==0){
             it->size+=prev->size;
             it->addr=prev->addr;
             it->size+=next->size;
             free_blocks.erase(next);
             free_blocks.erase(prev);
             partitions.erase(next);
             partitions.erase(prev);
           }else if (next->tag==0){
             it->size+=next->size;
             free_blocks.erase(next);
             partitions.erase(next);
           }else if (prev->tag==0){
             it->size+=prev->size;
             it->addr=prev->addr;
             free_blocks.erase(prev);
             partitions.erase(prev);
           } 
         }
         free_blocks.erase(it);
         free_blocks.insert(it);
       }
     }
     //printList();
     //std::cout << "--------------------------------------------"<< std::endl;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void printList(){
    printf("Partitions=[");
    for (auto p: partitions){
      std::cout <<"[" << p.tag<<","<< p.size<<","<< p.addr<<"], ";
    }
    printf("]\n");
  }
  void getStats(MemSimResult & result)
  {
    // return the size of the maximum free partition (set to 0 if no free partitions exist)
    
    int64_t maxFree=0;
    for (Partition p: partitions){
      if (p.tag==0){
        if (p.size>maxFree){
          maxFree=p.size;
        }
      }
    }
    result.max_free_partition_size = maxFree;
    
    // return the total number of pages requested
    result.n_pages_requested = numOfPages;
  }
  int64_t numOfPages=0;
  int64_t page_size_;
  std::list<Partition> partitions;
  std::set<PartitionRef,scmp> free_blocks;
  std::unordered_map<long, std::vector<PartitionRef>> tagged_blocks;
};




void mem_sim(int64_t page_size, const std::vector<Request> & requests, MemSimResult & result)
{
  Simulator sim(page_size);
  sim.printList();
  Partition p0;
  p0.tag=0;
  p0.size=0;
  p0.addr=0;
  sim.partitions.push_back(p0);
  sim.printList();
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
  }
  sim.getStats(result);
}