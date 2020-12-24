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
    int64_t pointerToList = 0;
    
    unsigned long int size = processes.size();
    unsigned long int jobs_remaining = size;
    unsigned long int count = size;
    std::queue<int64_t> waiting;
    seq.clear();
    int64_t seq_len=0;
    std::vector<Process> temp_processes = processes;
    int64_t back=-1;
    //int64_t temp_quantum = quantum;
    //int QWEi=0;
    bool notAdded =false;
    printf("total jobs= %d\n", (int)size);
    while(1) {
        //printf("outer while\n");
        //break;
        if (jobs_remaining == 0) break;
        //if (current>35) break;
        ///do whatever should happen at time curr_time
        //if (QWEi==25) break;
        //QWEi++;
        /////////////////////////////////////////
        //if (temp_processes[pointerToList].arrival_time <= current){
            //arr_curr++;
        
        while (count>0 && (processes[pointerToList].arrival_time <= current)){
            printf("%d :: pushing process %d with %d arrival time\n",(int)current, processes[pointerToList].id,(int)processes[pointerToList].arrival_time);
            if (processes[pointerToList].arrival_time==current && notAdded){
                waiting.push(back);
                notAdded=false;
            }
            processes[pointerToList].start_time=-1;
            processes[pointerToList].finish_time=-1;
            waiting.push(pointerToList);
            pointerToList++;
            count--; //keeps track of number of processes
        }    
        while (processes[pointerToList].arrival_time<=current){
            if (processes[pointerToList].arrival_time==current && !isPushed){
                waiting.push(waiting.front());
                waiting.pop();
                isPushed=true;
            }
            waiting.push(pointerToList);
            bursts.push_back(processes[pointerToList].burst);
            pointerToList++;
        }
        if (!waiting.empty()){
            if (processes[waiting.front()].start_time<0){
                processes[waiting.front()].start_time=current;
            }
            if (temp_processes[waiting.front()].burst>quantum){
                //seq.push_back(processes[waiting.front()].id);
                current+=quantum;
                temp_processes[waiting.front()].burst-=quantum;
                back=waiting.front();
                count++;
                notAdded=true;
                //waiting.push(waiting.front());
                waiting.pop();
            }else if (temp_processes[waiting.front()].burst==quantum){
                //seq.push_back(processes[waiting.front()].id);
                current+=quantum;
                temp_processes[waiting.front()].burst=0;
                waiting.pop();
                processes[waiting.front()].finish_time=current;
                jobs_remaining--;
            }else{
                //seq.push_back(processes[waiting.front()].id);
                current+=temp_processes[waiting.front()].burst;
                temp_processes[waiting.front()].burst=0;
                waiting.pop();
                processes[waiting.front()].finish_time=current;
                jobs_remaining--;
            }
            continue;
        }
        // else{
        //     current=processes[pointerToList+1].arrival_time
        // }
        current++;
        //if (count==0){jobs_remaining--;}
        // if (!waiting.empty()){
        //     //Process pro=waiting.front();
        //     if (processes[waiting.front()].start_time==-1){
        //         printf("changing\n");
        //         processes[waiting.front()].start_time=current;
        //     }
        //     printf("%d\n",(int)processes[waiting.front()].start_time);
        //     if (temp_processes[waiting.front()].burst>quantum){
        //         if (max_seq_len>seq_len){
        //             seq.push_back(temp_processes[waiting.front()].id);   
        //             seq_len++;
        //         }
        //         current+=quantum;
        //         temp_processes[waiting.front()].burst-=quantum;
        //         back=waiting.front();
        //         notAdded=true;
        //         waiting.pop();
        //     }else if (temp_processes[waiting.front()].burst==quantum){
        //         if (max_seq_len>seq_len){
        //             seq.push_back(temp_processes[waiting.front()].id);   
        //             seq_len++;
        //         }
        //         current+=quantum;
        //         temp_processes[waiting.front()].burst=0;
        //         waiting.pop();
        //         jobs_remaining--;
        //     }else{
        //         //process done
        //         if (max_seq_len>seq_len){
        //             seq.push_back(temp_processes[waiting.front()].id);   
        //             seq_len++;
        //         }
        //         current+=temp_processes[waiting.front()].burst;
        //         temp_processes[waiting.front()].burst=0;
        //         processes[waiting.front()].finish_time=current;
        //         jobs_remaining--;
        //         //waiting.front().burst=0;
        //         waiting.pop();
        //     }
        // }else{
        //     //if ((pointerToList+1)<(int64_t)size) current=processes[pointerToList+1].arrival_time;
        //     current++;
        // }
        
    }
    
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