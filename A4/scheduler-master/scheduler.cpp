/*
* Author: Aifaz Dhuka
* CPSC457 Assignment 4
* TA: Michelle Nguyen
* Takes in arrival and bursts and simulates
* round robin scheduling based on the input quantum.
* Returns the start  time and finish time of each process
* and the sequence in which they run
*/

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
    //current time
    int64_t current = 0;
    //tracker for process list
    int64_t pointerToList=0;

    unsigned long int jobs_remaining = processes.size();
    unsigned long int size=jobs_remaining;
    seq.clear();
    int64_t seq_len=0;
    //ready queue
    std::queue<int64_t> waiting;
    //bursts copy
    std::vector<int64_t> bursts;
    //int64_t temp_quantum=quantum;
    std::vector<int> ids;
    //track what and when to push back the element in ready queue
    int64_t toBePushed;
    bool isPushed=true;
    
    int64_t nextArrival=-1;
    while(1) {
        if (jobs_remaining == 0) break;

        //adds processes to ready queue if they have previously arrived
        while (processes[pointerToList].arrival_time<current && (unsigned long int) pointerToList<size){
            waiting.push(pointerToList);
            bursts.push_back(processes[pointerToList].burst);
            pointerToList++;
        }
        //adds process back to the ready queue and avoid doing that if it is the only process
        if(!isPushed){
            waiting.pop();
            waiting.push(toBePushed);
            isPushed=true;
        }
        //adds processes to ready queue if they have arrived at this instance
        while (processes[pointerToList].arrival_time==current && (unsigned long int) pointerToList<size){
            waiting.push(pointerToList);
            bursts.push_back(processes[pointerToList].burst);
            pointerToList++;
        }
        if ((unsigned long int) pointerToList<size){
            nextArrival=processes[pointerToList].arrival_time;
        }
        
        if (waiting.empty()){
            //empty queue
            if((seq_len <= max_seq_len)){
                if ((seq.empty() || seq.back()!=(-1))) {
                    seq.push_back(-1);
                    seq_len++;
                }
            }
            //fast forward to next process arrival
            current=processes[pointerToList].arrival_time;
        }else{
            //not empty queue
            //take first process
            int64_t index =waiting.front();
            int64_t numOfTimesForArrival=0;
            int64_t numOfTimesForFinish=0;
            ids.clear();
            std::vector<int64_t> indexes;
            int64_t small_burst_wait=-1;
            //id_bursts.clear();
            //set the start time if appeared for the first time
            if (processes[index].start_time==-1) processes[index].start_time=current;
            //ids.push_back(processes[index].id);
            indexes.push_back(index);
            //id_bursts.push_back(processes[index].burst);
            small_burst_wait=processes[index].burst;
            waiting.pop();
            waiting.push(index);
            while(waiting.front()!=index){
                //ids.push_back(processes[waiting.front()].id);
                indexes.push_back(waiting.front());
                //id_bursts.push_back(processes[waiting.front()].burst);
                if (small_burst_wait>processes[waiting.front()].burst){
                    small_burst_wait=processes[waiting.front()].burst;
                }
                waiting.push(waiting.front());
                waiting.pop();
            }
            numOfTimesForArrival=(nextArrival-current)/(waiting.size()*quantum);
            numOfTimesForFinish=small_burst_wait/quantum;
            int64_t numOfTimes=std::min(numOfTimesForArrival,numOfTimesForFinish);
            
            if (numOfTimes>=1){
                current+= numOfTimes*waiting.size()*quantum;
                for (int64_t ind: indexes){
                    bursts[ind]-=numOfTimes*quantum;
                    if (bursts[ind]==0){
                        if (ind==index){
                            waiting.pop();
                        }else{
                            waiting.push(index);
                            waiting.pop();
                            while(waiting.front()!=index){
                                if (ind!=waiting.front()){
                                    waiting.push(waiting.front());
                                }
                                waiting.pop();
                            }
                        }
                        jobs_remaining--;
                    }
                }
                
                ///////////////////////////////////////////////////////////////////////////////
                //if((seq_len < max_seq_len)){
                //if ((seq.empty() || seq.back()!=(processes[index].id))) {
                    for (int n=0; n<numOfTimes;n++){
                        if (seq_len<max_seq_len){
                            for (int m=0; (unsigned long int)m<indexes.size();m++) {
                                seq.push_back(processes[indexes[m]].id);    
                                //seq.push_back(processes[index].id);
                                seq_len++;
                            }
                        }
                    }
                continue;        
                //}
                //}
                ///////////////////////////////////////////////
            }
            /*
            bst = 6,8,3
            num =      size = 3        q=3
            seq= 1,2,1,2
            
            */
            if (bursts[index]>quantum){
                //if burst more than quantum, we have to add it back to queue
                // for (int i; (unsigned long int)i <waiting.size();i++){
                //     totalTime+=processes[waiting[i]].burst;
                // }
                if((seq_len < max_seq_len)){
                    if ((seq.empty() || seq.back()!=(processes[index].id))) {
                        seq.push_back(processes[index].id);
                        seq_len++;
                    }
                }
                bursts[index]-=quantum;
                current+=quantum;
                toBePushed=index;
                //waiting.pop();
                isPushed=false;
            }else if (bursts[index]==quantum){
                if((seq_len < max_seq_len)){
                    if ((seq.empty() || seq.back()!=(processes[index].id))) {
                        seq.push_back(processes[index].id);
                        seq_len++;
                    }
                }
                //if burst=quantum, we just add quantum to array and decrease from jobs remaining and pop the process from the queue
                current+=quantum;
                bursts[index]=0;
                jobs_remaining--;
                waiting.pop();
                processes[index].finish_time=current;
            }else if (bursts[index]<quantum){
                //if burst less than quantum, we do the same as above except add the burst value to the quantum
                current+=bursts[index];
                bursts[index]=0;
                jobs_remaining--;
                waiting.pop();
                processes[index].finish_time=current;
            }
        }
    }   
}