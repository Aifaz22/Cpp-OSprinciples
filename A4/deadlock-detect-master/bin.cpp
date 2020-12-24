// this is the ONLY file you should edit and submit to D2L

#include "common.h"
#include <iostream>
#include <queue>

/// this is the function you need to implement
/// parameter edges[] contains a list of request and assignment edges
///   example of a request edge, process "p1" resource "r1"
///     "p1 -> r1"
///   example of an assignment edge, process "XYz" resource "XYz"
///     "XYz <- XYz"
/// Parameters edge_index and cycle[] are used to return
/// results back to the caller.
///
/// You need to process edges[] one edge at a time, and run a deadlock
/// detection after each edge.
///
/// As soon as you detecte a deadlock, you need to set edge_index to the index of the edge
/// in edges[] that caused the deadlock. For example, edges[7] caused the deadlock,
/// then set edge_index=7. You must also populete cycle[] with the names of
/// processes that are in a deadlock. You can then return from the function without
/// processing any other edges.
///
/// To indicate deadlock was detected after processing all edges, you must
/// set edge_index = -1 and clear cycle[] e.g. by calling cycle.clear()
///

class Graph {
    public:
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
    std::vector<int> out_counts;
    //std::unordered_map<std::string, int> in_counts;
} graph;

void detect_deadlock(
    const std::vector<std::string> & edges, int & edge_index, std::vector<std::string> & cycle)
{
    edge_index=0;
    Word2Int w2i;
    // let's try to guess the results :)
    std::queue<int> zeros;
    std::vector<int> out;
    std::vector<std::string> names;
    for (std::string e:edges){
        simplify(e);
        std::vector<std::string> edge =split(e);
        if (edge[1]=="->"){
            //process requesting
            graph.adj_list["Res:"+edge[2]].push_back("Pro:"+edge[0]);
            int i = w2i.get("Pro:"+edge[0]);
            if (graph.out_counts.size()<=(long unsigned int)i){
                names.push_back(edge[0]);
                //out.push_back(1);
                graph.out_counts.push_back(1);
            }else{
                //out[i]++;
                graph.out_counts[i]++;
            }
            int j = w2i.get("Res:"+edge[2]);
            if (graph.out_counts.size()<=(long unsigned int)j){
                names.push_back(edge[2]);
                //out.push_back(0);
                graph.out_counts.push_back(0);
            }
            //printf("%s=%d\t\t%s=%d\n",edge[0].c_str(),i,edge[2].c_str(),j);
            //[w2i.get("Pro:"+edge[0])]
            // printf("********************\n");
            // if (graph.adj_list.size()==(long unsigned int)j){
            //     std::vector<int> a;
            //     a.push_back(i);
            //     graph.adj_list.push_back(a);
            // }else{
            //     graph.adj_list[j].push_back(i);
            // }
            
            
            

        }
        else{
            //process holding
            graph.adj_list["Pro:"+edge[0]].push_back("Res:"+edge[2]);
            int i = w2i.get("Pro:"+edge[0]);
            if (graph.out_counts.size()<=(long unsigned int) i){
                names.push_back(edge[0]);
                //out.push_back(0);
                graph.out_counts.push_back(0);
            }
            int j = w2i.get("Res:"+edge[2]);
            if (graph.out_counts.size()<= (long unsigned int) j){
                names.push_back(edge[2]);
                //out.push_back(1);
                graph.out_counts.push_back(1);
            }else{
                //out[j]++; 
                graph.out_counts[j]++;
            }
            // printf("reached\n");
            // if (graph.adj_list.size()==(long unsigned int)i){
            //     std::vector<int> a;
            //     a.push_back(j);
            //     graph.adj_list.push_back(a);
            // }else{
            //     graph.adj_list[i].push_back(j);
            // }
           
            
            //printf("%s=%d\t\t%s=%d\n",edge[0].c_str(),i,edge[2].c_str(),j);
            //graph.out_counts["Res:"+edge[2]]++;
            //graph.out_counts["Pro:"+edge[0]]+=0;
        }
        // for (auto name : names){
        //     printf("%s\n",name.c_str());
        // }
        // printf("**********\n");
        edge_index+=1;
        out=graph.out_counts;
        // out = out_counts; // make a copy of outdegrees since you will be modifying it
        // zeros[] = find all nodes in graph with outdegree == 0
        // while zeros.size() > 0:
        //   n = pop one entry from zeros[]
        //   for every n2 of adj_list[n]:
        //      out[n2] --
        //      if out[n2] == 0: append n2 to zeros[]
        // cycle[] = find all nodes in graph with outdegree==0 and representing processes
        for (auto i : graph.out_counts){
            printf("%d\n",i);
        }
        for (int i=0; (long unsigned int)i< out.size();i++){
           if (out[i]==0){
               zeros.push(i);
           }
        }
        cycle.clear();
        std::unordered_map<std::string, std::vector<std::string>> temp_adj_list;
        temp_adj_list=graph.adj_list;
        printf("[");
        for (auto u: out){
            printf("%d,",u);
        }
        printf("]\n");
        while (!zeros.empty()){
            int n =zeros.front();
            zeros.pop();
            cycle.push_back(names[n]);
            std::vector<std::string> changes = temp_adj_list[names[n]];
            temp_adj_list.erase(names[n]);
            for (std::string c: changes){
                int e = w2i.get(c);
                out[e]--;
                if (out[e]==0) zeros.push(e);
            }
        }
        //printf()
        for (auto & it : temp_adj_list){
            std::cout<<"after"<<it.first<<"={";
            for (auto s : it.second){
                std::cout<<s<<",";
            }
            std::cout<<"}"<<std::endl;
        }
        if (temp_adj_list.empty()){
            //zeros.clear();
            cycle.clear();
            edge_index=-1;
        }else{
            std::cout<<"full"<<std::endl;
            return;
        }

        /*
        for (auto & it : graph.out_counts){

         if (it.second==0) zeros.push_back(it.first);
            //printf("pushed %s : %d\n",it.first.c_str(),it.second);
        }
        std::vector<std::string> changes;
        cycle.clear();
        int i=0;
        for (auto & it : graph.adj_list){
            i++;
            for (auto e : it.second){
                printf("%d::> %s\n",i,e.c_str());
                 printf("%d = %d\n",x,it.second);
            printf("==> %d = %d\n",x,out[x]);
            }
         //if (it.second==0) zeros.push_back(it.first);
            //printf("pushed %s : %d\n",it.first.c_str(),it.second);
        }
        std::unordered_map<std::string, std::vector<std::string>> temp_adj_list;
        while (!zeros.empty()){
            //printf("**********************************************\n");
            
            temp_adj_list=graph.adj_list;
            changes = temp_adj_list[zeros.back()];
            ///////////////////////////////////
            temp_adj_list.erase(zeros.back());
            ///////////////////////////////////
            cycle.push_back(zeros.back());
            zeros.pop_back();
            
            for (auto e: changes){
                ////////////////////////
                std::unordered_map<std::string, int> temp_out_counts;
                temp_out_counts=graph.out_counts;
                int count=temp_out_counts[e];
                count--;
                temp_out_counts[e]=count;
                ////////////////////////
                if (count==0){
                    zeros.push_back(e);
                }
                
            }
        }
        
        if (temp_adj_list.empty()){
            zeros.clear();
            cycle=zeros;
            edge_index=-1;
        }else{
            return;
        }*/
    }
    
    
    
/*
out = out_counts; // make a copy of outdegrees since you will be modifying it
zeros[] = find all nodes in graph with outdegree == 0
while zeros.size() > 0:
 n = pop one entry from zeros[]
 for every n2 of adj_list[n]:
   out[n2] --
   if out[n2] == 0: append n2 to zeros[]
cycle[] = find all nodes in graph with outdegree==0 and representing processes
*/
    
    //cycle.push_back("cyclefound");
    //edge_index = 117;
}





/*edge_index+=1;
        for (auto & it : graph.out_counts){

         if (it.second==0) zeros.push_back(it.first);
            //printf("pushed %s : %d\n",it.first.c_str(),it.second);
        }
        std::vector<std::string> changes;
        cycle.clear();
        int i=0;
        for (auto & it : graph.adj_list){
            i++;
            for (auto e : it.second){
                printf("%d::> %s\n",i,e.c_str());
            }
            printf("************\n");
         //if (it.second==0) zeros.push_back(it.first);
            //printf("pushed %s : %d\n",it.first.c_str(),it.second);
        }
    
        std::unordered_map<std::string, std::vector<std::string>> temp_adj_list;
        while (!zeros.empty()){
            //printf("**********************************************\n");
            
            temp_adj_list=graph.adj_list;
            changes = temp_adj_list[zeros.back()];
            ///////////////////////////////////
            temp_adj_list.erase(zeros.back());
            ///////////////////////////////////
            cycle.push_back(zeros.back());
            zeros.pop_back();
            
            for (auto e: changes){
                ////////////////////////
                std::unordered_map<std::string, int> temp_out_counts;
                temp_out_counts=graph.out_counts;
                int count=temp_out_counts[e];
                count--;
                temp_out_counts[e]=count;
                ////////////////////////
                if (count==0){
                    zeros.push_back(e);
                }
                
            }
        }
        
        if (temp_adj_list.empty()){
            zeros.clear();
            cycle=zeros;
            edge_index=-1;
        }else{
            return;
        }*/
 