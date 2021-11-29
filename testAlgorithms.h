#ifndef TEST_ALGORITHMS
#define TEST_ALGORITHMS

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include "graph.h"

using namespace std;


int sendToNeighbors(Graph* g, int* T, int node, int run){
    int cost = 0;
    queue<int> reached;
    for(int i=1;i<=g->v;i++){
        if(i != node){
            if(g->graph[node][i] != 0){
                if(g->S[i-1] == 1 && T[i-1] != 1){
                    T[i-1] = 1;
                    cout<<node<<" -> "<<i<<endl;
                    cost++;
                    reached.push(i);
                }
            }
        }
    }
    if(run < g->dLimit){
        int next;
        while(!reached.empty()){
            next = reached.front();
            reached.pop();
            cost += sendToNeighbors(g,T,next,run+1);
        }
    }
    return cost;
}

int greedy(Graph* g){
    int cost=0;
    int T[g->v];
    int con[g->v];
    for(int i=0;i<g->v;i++){
        T[i] = 0;
        con[i] = 0;
    }
    bool check = true;
    for(int i=1;i<=g->v;i++){
        for(int k=i;k<=g->v;k++){
            if(g->graph[i][k] != 0){
                if(g->S[k-1] != 0)
                    con[i-1]++;
                if(g->S[i-1] !=0)
                    con[k-1]++;
            }
        }
    }

    while(check){
        int max = 0;
        int node;
        for(int i=0;i<g->v;i++){
            if(con[i] > max){
                if(T[i] != 1){
                    max = con[i];
                    node = i+1;
                }
            }
        }
        T[node-1] = 1;
        cost += g->delta;
        cost += sendToNeighbors(g,T,node,1);
        check = false;
        for(int i=0;i<g->v;i++){
            if(g->S[i] == 1 && T[i] != 1){
                check=true;
            }
        }
    }
    return cost;
}

int random(Graph *g){
    int cost = 0;
    int T[g->v];
    for(int i=0;i<g->v;i++){
        T[i] = 0;
    }

    bool check = true;
    while(check){
        int node = rand()%g->v +1;
        while (g->S[node-1] != 1 || T[node-1] != 0){
            node = rand()%g->v +1;
        }
        cout<<"C2E node: " << node <<endl;
        cost += g->delta;
        T[node-1] = 1;
        cost += sendToNeighbors(g, T, node, 1);
        check = false;
        for(int i=0;i<g->v;i++){
            if(g->S[i] == 1 && T[i] != 1){
                check=true;
            }
        }
    }
    return cost;
}

#endif
