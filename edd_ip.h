#ifndef EDD_IP
#define EDD_IP

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <vector>
#include <limits>

#include "graph.h"

using namespace std;

int minCost = std::numeric_limits<int>::max();
int **solution;

bool checkDone(Graph* g, int* T){
    for(int i=0; i<g->numDestServers; i++){
        if(T[g->R[i]-1] != 1)
            return false; 
    }
    return true;
}

int ** fillOut(Graph* g, int **path, int *T, int jump, int cost){
    if(jump > g->dLimit){
        return NULL;
    }
    int copy[g->v] ={0};
    for(int i=0;i<g->v;i++)
        copy[i] = T[i];
    for(int i=1; i<=g->v; i++){
        if(T[i-1] == 1){
            for(int j=1;j<=g->v;j++){
                if(g->graph[i][j] != 0 && T[j-1] != 1){
                    if(g->S[j-1] == 1){
                        copy[j-1] = 1;
                        path[i][j] = 1;
                        cost++;
                    }
                }
            }
        }
    }
    if(checkDone(g, copy)){
        if(cost < minCost){
            minCost = cost;
            solution = path;
            return path;
        }
        return NULL;
    }
    fillOut(g, path, copy, jump+1, cost);
    
}

int ** findPath(Graph* g, int* comb, int len){
    int T[g->v] = {0};
    int **path = new int*[g->v+1];
    for(int i=0;i<=g->v;i++){
        path[i] = new int[g->v+1];    
    }
    int cost = 0;
    for(int i=0; i<len;i++){
        T[comb[i]-1] = 1;
        path[0][comb[i]] = 1;
        cost += g->delta;
    }
    if(checkDone(g, T)){
        if(cost < minCost){
            minCost = cost;
            solution = path;
            return path;
        }
        return NULL;
    }
    path = fillOut(g, path, T, 1, cost);
    return path;
}

int* combinations(Graph *g, int* comb, int last, int len){
    int *ret = new int[len+1];
    findPath(g, comb, len);
    for(int i = 0; i < len; i++){
        ret[i] = comb[i];
    }
    for(int i = last+1; i <= g->v; i++){
        ret[len] = g->V[i];
        combinations(g,ret, i, len+1);
    }
    return comb;
}

int** full(Graph *g){
    int *comb = new int[1];
    for(int i=1; i<=g->v; i++){
        comb[0] = g->V[i];
        combinations(g,comb, i, 1);
    }
    printPath(solution, g->v);
    return solution;
}

#endif
