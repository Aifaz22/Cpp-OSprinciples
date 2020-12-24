// this is the file you need to edit

#include "memsim.h"
#include <cstdio>
#include <list>
#include <set>
#include <unordered_map>


// feel free to use this class as a starting point
struct Simulator {
  Simulator(int64_t page_size) { page_size_ = page_size; }
  void allocate(int tag, int size) { printf("allocating tag=%d size=%d\n", tag, size); }
  void deallocate(int tag) { printf("deallocating tag=%d\n", tag); }
  void getStats(MemSimResult & result)
  {
    // return the size of the maximum free partition (set to 0 if no free partitions exist)
    result.max_free_partition_size = 123;
    // return the total number of pages requested
    result.n_pages_requested = 9;
  }

  int64_t page_size_;
};

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
struct Simulator {
  // all partitions (so that I can get std::prev() and std::next() nodes
  std::list<Partition> partitions;
  // sorted partitions by size/address
  std::set<PartitionRef,scmp> free_blocks;
  // quick access to all tagged partitions
  std::unordered_map<long, std::vector<PartitionRef>> tagged_blocks;
};
// re-implement the following function
// ===================================
// input parameters:
//    page_size: integer in range [1..1,000,000]
//    requests: array of requests, each with tag and size
// output parameters:
//    result: populate with correct values before returning
void mem_sim(int64_t page_size, const std::vector<Request> & requests, MemSimResult & result)
{
  Simulator sim(page_size);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
  }
  sim.getStats(result);
}