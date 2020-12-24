// you need to modify this file

#include "scheduler.h"
#include "common.h"
#include <vector>
#include <queue>

// this is the function you should edit
//
// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrival_times, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//

void simulate_rr(
    int64_t quantum, 
    int64_t max_seq_len,
    std::vector<Process> & processes,
    std::vector<int> & seq) 
{
    // replace the wrong implementation below with your own!!!!
    int64_t current = 0;
    int64_t pointerToList=0;
    unsigned long int jobs_remaining = processes.size();
    unsigned long int size=jobs_remaining;
    seq.clear();
    std::queue<int64_t> waiting;
    std::vector<int64_t> bursts;
    //int64_t temp_quantum=quantum;
    int64_t toBePushed;
    bool isPushed=true;
    
    while(1) {
        if (jobs_remaining == 0) break;
        while (processes[pointerToList].arrival_time<current && pointerToList<size){
            // if (processes[pointerToList].arrival_time==current && !isPushed ){
            //     waiting.push(toBePushed);
            //     isPushed=true;
            // }
            waiting.push(pointerToList);
            bursts.push_back(processes[pointerToList].burst);
            pointerToList++;
        }
        if(!isPushed){
            waiting.push(toBePushed);
            isPushed=true;
        }
        while (processes[pointerToList].arrival_time==current && pointerToList<size){
            waiting.push(pointerToList);
            bursts.push_back(processes[pointerToList].burst);
            pointerToList++;
        }
        // std::queue<int64_t> waiting2=waiting;
        // printf("Queue: [");
        // while (!waiting2.empty()){
        //     printf("%ld, ",waiting2.front());
        //     waiting2.pop();
        // }
        // printf("]\n");
        
        if (waiting.empty()){
            if (seq.empty() || seq.back()!=(-1)) seq.push_back(-1);
            current=processes[pointerToList].arrival_time;
            //printf("#################################empty##################################\n");
        }else{
            
            int64_t index =waiting.front();
            if (processes[index].start_time==-1) processes[index].start_time=current;
            if (seq.empty() || seq.back()!=(processes[index].id)) seq.push_back(processes[index].id);
            // if (bursts[index]<=0){
            //     waiting.pop();
            //     continue;
            // }
            if (bursts[index]>quantum){
                bursts[index]-=quantum;
                current+=quantum;
                toBePushed=index;
                waiting.pop();
                isPushed=false;
                //waiting not updated
            }else if (bursts[index]==quantum){
                current+=quantum;
                bursts[index]=0;
                jobs_remaining--;
                waiting.pop();
                processes[index].finish_time=current;
            }else if (bursts[index]<quantum){
                current+=bursts[index];
                bursts[index]=0;
                jobs_remaining--;
                waiting.pop();
                processes[index].finish_time=current;
            }
            // bursts[index]-=quantum;
            // if (bursts[index]==0){
            //     current++;
            //     processes[index].finish_time=current;
            //     jobs_remaining--;
            //     waiting.pop();
            //     temp_quantum=quantum;
            //     continue;
            // }
            // if (temp_quantum==0){
            //     waiting.push(index);
            //     waiting.pop();
            //     temp_quantum=quantum;
            // }
            
            // current++;
        }
    }
    printf("*******************current: %ld**************\n",current);
    
}
/*
seq.clear();
    for (auto & p : processes) {
        p.finish_time = p.arrival_time + p.burst;
        p.start_time = p.arrival_time;
        seq.push_back(p.id);
    }
********************************************************************
curr_time = 0
while(1) {
    ... do whatever should happen
        at time curr_time
    if simulation done break
    curr_time ++
}
***************FCFS*************
curr_time = 0
jobs_remaining = size of job queue
while(1) {
    if jobs_remaining == 0 break
    if process in cpu is done
        mark process done
        set CPU idle
        jobs_remaining --
        continue
    if a new process arriving
        add new process to RQ
        continue
    if cpu is idle and RQ not empty
        move process from RQ to CPU
        continue
    execute one burst of job on CPU
    curr_time ++
}

*/