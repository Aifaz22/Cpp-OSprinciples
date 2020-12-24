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
    PartitionRef fit_node_i;
    Partition dummy_part;
    dummy_part.size=size;
    std::list<Partition> dummy { dummy_part };
    // Use std::set lower bound member function
    auto fit_i = free_blocks.lower_bound( dummy.begin() );
    if (fit_i==free_blocks.end()){
      int64_t pagesNeeded = ceil(size/page_size_);
      if (partitions.back().tag==0){
        partitions.back().size+=pagesNeeded*page_size_;
        free_blocks.erase(partitions.end());
        free_blocks.insert(partitions.end());
      }else{
        int64_t nextAddr = partitions.back().addr + partitions.back().size;
        Partition p;
        p.addr=nextAddr;
        p.size=pagesNeeded*page_size_;
        p.tag=0;
        partitions.push_back(p);
        free_blocks.insert(partitions.end());
      }
      numOfPages+=pagesNeeded;
      fit_node_i = partitions.end();
    }else{
      fit_node_i = *fit_i;
    }
    if (fit_node_i->size==size){//no spliting required.
      fit_node_i->tag=tag;
      free_blocks.erase(fit_node_i);
    }else{
      long nextSize=fit_node_i->size-size;
      long nextAddr=fit_node_i->addr+size;
      Partition emptyPart;
      emptyPart.tag=0;
      emptyPart.size=nextSize;
      emptyPart.addr=nextAddr;

      fit_node_i->tag=tag;
      fit_node_i->size=size;
      partitions.insert(fit_node_i, emptyPart);
      free_blocks.erase(fit_node_i);
      free_blocks.insert((fit_node_i));
    }
    tagged_blocks[tag].push_back(fit_node_i);
    //}
    
    //printf("allocating tag=%d size=%d\n", tag, size); 
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void deallocate(int tag) { 
     for (PartitionRef it : tagged_blocks[tag]){
       if (it->tag==tag){
         it->tag=0;
         if (it==partitions.begin()){
           auto next = std::next(it,1);
           if (next->tag==0){
             it->size+=next->size;
             partitions.erase(next);
             free_blocks.erase(next);
             free_blocks.erase(it);
           }
         } else if (it==partitions.end()){
           auto prev = std::prev(it,1);
           if (prev->tag==0){
             it->size+=prev->size;
             it->addr=prev->addr;
             partitions.erase(prev);
             free_blocks.erase(prev);
             free_blocks.erase(it);
           }
         }else{
           auto prev = std::prev(it,1);
           auto next = std::next(it,1);
           if (prev->tag==0){
             it->size+=prev->size;
             it->addr=prev->addr;
             partitions.erase(prev);
             free_blocks.erase(prev);
             free_blocks.erase(it);
             free_blocks.insert(it);
           }
           if (next->tag==0){
             it->size+=next->size;
             partitions.erase(next);
             free_blocks.erase(next);
             free_blocks.erase(it);
             free_blocks.insert(it);
           }
         }
         
       }
     }
    //printf("deallocating tag=%d\n", tag); 
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void getStats(MemSimResult & result)
  {
    // return the size of the maximum free partition (set to 0 if no free partitions exist)
    
    auto p = free_blocks.rbegin();
    if (p==free_blocks.rend()) result.max_free_partition_size = 0;
    else{
      PartitionRef x = *p;
      result.max_free_partition_size = x->size;
    }
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
  // Partition p0;
  // p0.tag=0;
  // p0.size=0;
  // p0.addr=0;
  // sim.partitions.push_back(p0);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
  }
  sim.getStats(result);
}