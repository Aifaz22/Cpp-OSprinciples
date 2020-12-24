//}
        /////////////////////////////////////////
        if (!waiting.empty()){
            printf("process is waiting\n");
            if (waiting.front().start_time==-1) waiting.front().start_time=current;
            if(waiting.front().burst>=quantum){ 
                
                waiting.front().burst--;
                quantum--;
                if (waiting.front().burst==0){
                    //job done
                    jobs_remaining--;
                    waiting.front().finish_time=current; 
                    waiting.pop();
                    quantum=temp_quantum;
                    if(max_seq_len>seq_len){
                        seq.push_back(waiting.front().id);   
                        seq_len++;
                    }
                }else if (quantum==0){
                    //job not done
                    waiting.push(waiting.front());
                    waiting.pop();
                    quantum=temp_quantum;
                    if(max_seq_len>seq_len){
                        seq.push_back(waiting.front().id);   
                        seq_len++;
                    }
                }
            }else{
                printf("idle\n");
                //job done within quantum
                current += waiting.front().burst;
                //for (int i=waiting.front().burst;i>0;i--){
                    if(max_seq_len>seq_len){
                        seq.push_back(waiting.front().id);   
                        seq_len++;
                    }
                //}
                waiting.front().finish_time=current;
                jobs_remaining--;
                quantum=temp_quantum;
                continue;
            }
            //if simulation done break
            current++;
        }
        current++;
        // else{
        //     //////////////////////////////
        //     current=processes[pointerToList+1].arrival_time;
        //     //pointerToList=current-1;
        //     /////////////////////////////////
        // }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!waiting.empty()){
            //Process pro=waiting.front();
            if (processes[waiting.front()].start_time==-1){
                printf("changing\n");
                processes[waiting.front()].start_time=current;
            }
            printf("%d\n",(int)processes[waiting.front()].start_time);
            if (temp_processes[waiting.front()].burst>quantum){
                if (max_seq_len>seq_len){
                    seq.push_back(temp_processes[waiting.front()].id);   
                    seq_len++;
                }
                current+=quantum;
                temp_processes[waiting.front()].burst-=quantum;
                back=waiting.front();
                notAdded=true;
                waiting.pop();
            }else if (temp_processes[waiting.front()].burst==quantum){
                if (max_seq_len>seq_len){
                    seq.push_back(temp_processes[waiting.front()].id);   
                    seq_len++;
                }
                current+=quantum;
                temp_processes[waiting.front()].burst=0;
                waiting.pop();
                jobs_remaining--;
            }else{
                //process done
                if (max_seq_len>seq_len){
                    seq.push_back(temp_processes[waiting.front()].id);   
                    seq_len++;
                }
                current+=temp_processes[waiting.front()].burst;
                temp_processes[waiting.front()].burst=0;
                processes[waiting.front()].finish_time=current;
                jobs_remaining--;
                //waiting.front().burst=0;
                waiting.pop();
            }
        }else{
            //if ((pointerToList+1)<(int64_t)size) current=processes[pointerToList+1].arrival_time;
            //current++;
        }