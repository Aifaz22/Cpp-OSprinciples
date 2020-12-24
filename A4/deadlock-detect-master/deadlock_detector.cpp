/*
* Author: Aifaz Dhuka
* CPSC457 Assignment 4
* TA: Michelle Nguyen
* Looks for deadlocks based on input graph edges
* Returns the edge index that resulted in the deadlock
* and all the processes involved in the deadlock
*/

#include "common.h"

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
    std::vector<std::vector<int>> f_adj_list;
    std::vector<int> f_out_counts;
} ;

void detect_deadlock(
    const std::vector<std::string> & edges, int & edge_index, std::vector<std::string> & cycle)
{
    Graph graph;
    Word2Int w2i;
    // let's try to guess the results :)
    std::vector<std::string> zeros;
    std::vector<int> f_zeros;
    edge_index=-1;
    std::vector<int> v;
    cycle.clear();
    int nodes=0;
    unsigned long int numOfEdges = edges.size();
    std::unordered_map<int,std::string> names;
    for (int index=0; (unsigned long int)index<numOfEdges;index++){
        std::string e =edges[index];
       
        std::vector<std::string> edge =split(e);
        edge[0]="Pro:"+edge[0];
        edge[2]="Res:"+edge[2];
        
        int pro = w2i.get(edge[0]);
        int res = w2i.get(edge[2]);
        names[pro]=edge[0];
        names[res]=edge[2];
       //initializing
        if (edge[1]=="->"){
            //process requesting resource
            if (nodes==pro){
                v.clear();
                graph.f_adj_list.push_back(v);
                graph.f_out_counts.push_back(1);
                nodes++;
            }else{
                graph.f_out_counts[pro]++;
            }
            if (nodes==res){
                v.clear();
                v.push_back(pro);
                graph.f_adj_list.push_back(v);
                graph.f_out_counts.push_back(0);
                nodes++;
            }else{
                graph.f_adj_list[res].push_back(pro);
            }
        }
        else{
            //process holding resource
            if (nodes==pro){
                v.clear();
                v.push_back(res);
                graph.f_adj_list.push_back(v);
                graph.f_out_counts.push_back(0);
                nodes++;
            }else{
                graph.f_adj_list[pro].push_back(res);
            }
            if (nodes==res){
                v.clear();
                graph.f_adj_list.push_back(v);
                graph.f_out_counts.push_back(1);
                nodes++;
            }else{
                graph.f_out_counts[res]++;
            }
        }
        // INITIALIZATION DONE 
        
        //topological sort
        
        std::vector<int> f_temp_out_counts;
        f_temp_out_counts=graph.f_out_counts;
        //add elements with outgoing=0 to zero vector
        int numberOfZeros=0;
        for (int m = 0; m <nodes; m++){
            if (f_temp_out_counts[m]==0){
                f_zeros.push_back(m);
                numberOfZeros++;
            }
        }
        while (!f_zeros.empty()){
            //get an element form zero
            int nameID = f_zeros.back();
            f_zeros.pop_back();
            //check if removeing the node selected from 0 resulted in 
            //another node's outgoing to 0. If so add it to zero vector
            for (int n : graph.f_adj_list[nameID]){
                f_temp_out_counts[n]--;
                if (f_temp_out_counts[n]==0){
                    f_zeros.push_back(n);
                    numberOfZeros++;
                }
            }
        }
        //get the processes from the adj list
        if (numberOfZeros==nodes) continue;

        //run only if necessary
        for (int m=0; m <nodes; m++){
            if (f_temp_out_counts[m]!=0){
                if (names[m][0]=='P'){
                    cycle.push_back(names[m].substr(4));
                }
            }
        }
        if (!cycle.empty()){
            edge_index = index;
            return;
        }       
    }
}
