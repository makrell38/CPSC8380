#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <vector>
#include <limits>

using namespace std;

int R[] = {1,2,3,4,6,7,9};
int S[10] = {1,1,1,1,0,1,1,0,1,0};
//int R[] = {1,2,3,4,6,7,9};
int numDestServers = 7;
int delta = 9;
int limit = 1;
int minCost = std::numeric_limits<int>::max();
int **solution;

void addEdge(int** graph, int v1, int v2, int weight){
    graph[v1][v2] = weight;
    graph[v2][v1] = weight;
    return;
}

void printGraph(int** graph, int v){
    for(int i =0; i <= v; i++){
        cout << "[";
        for(int j=0; j<=v; j++){
            cout<< graph[i][j] << " ";
        }
        cout<<"]"<<endl;
    }
}

/*
void printPath(int* ret, int len){
    for(int i = 0; i<len; i++){
        cout<<ret[i]<<" ";
    }
    cout<<endl;
}
*/

void printPath(int** graph, int v){
    cout<<"SOLUTION FOUND"<<endl;
    for(int i=0;i<=v;i++){
        for(int j=0; j<=v; j++){
            if(graph[i][j] != 0){
                cout<<i<<"->"<<j<<" cost "<<minCost<<endl;
            }
        }
    }
    cout<<"END OF SOLUTION"<<endl<<endl;
}

bool checkDone(int* T){
    for(int i=0; i<numDestServers; i++){
        if(T[R[i]-1] != 1)
            return false; 
    }
    //cout<<"DOPNE"<<endl;
    return true;
}

int ** fillOut(int **graph, int **path, int *T, int v, int jump, int cost){
    if(jump > limit){
        return NULL;
    }
    int copy[v] ={0};
    for(int i=0;i<v;i++)
        copy[i] = T[i];
    for(int i=1; i<=v; i++){
        if(T[i-1] == 1){
            for(int j=1;j<=v;j++){
                if(graph[i][j] != 0 && T[j-1] != 1){
                    if(S[j-1] == 1){
                        copy[j-1] = 1;
                        path[i][j] = 1;
                        cost++;
                    }
                }
            }
        }
    }
    if(checkDone(copy)){
        if(cost < minCost){
            minCost = cost;
            printPath(path, v);
            solution = path;
            return path;
        }
        return NULL;
    }
    fillOut(graph, path, copy, v, jump+1, cost);
    
}

int ** findPath(int **graph, int* V, int* comb, int len, int v){
    int T[v] = {0};
    //int S[v] = {0};
    int **path = new int*[v+1];
    for(int i=0;i<=v;i++){
        path[i] = new int[v+1];    
    }
    int cost = 0;
    for(int i=0; i<len;i++){
        T[comb[i]-1] = 1;
        //S[comb[i]] = 1;
        path[0][comb[i]] = 1;
        cost += delta;
    }
    //printPath(S, v);
    if(checkDone(T)){
        if(cost < minCost){
            minCost = cost;
            printPath(path,v);
            return path;
        }
        return NULL;
    }
    path = fillOut(graph, path, T, v, 1, cost);
    return path;
}

int* combinations(int** graph, int* V, int* comb, int last, int len, int v){
    int *ret = new int[len+1];
    findPath(graph, V, comb, len, v);
    for(int i = 0; i < len; i++){
        ret[i] = comb[i];
    }
    for(int i = last+1; i <= v; i++){
        ret[len] = V[i];
        combinations(graph, V, ret, i, len+1, v);
    }
    //printPath(comb, len);
    return comb;
}

void full(int** graph, int v, int* V){
    int *comb = new int[1];
    for(int i=1; i<=v; i++){
        comb[0] = V[i];
        combinations(graph, V, comb, i, 1, v);
    }
    return;
}

int main(){
    int v = 10;
    
    int *V = new int[v+1];
    for(int i=1;i<=v;i++)
        V[i] = i;
    // Add cloud to V
    V[0] = 0;

    int **graph;
    graph = new int*[v];
    for(int i = 0; i < v+1; i++)
        graph[i] = new int[v+1];

    
    int S[v] = {0};
    int I[v] = {0};


    addEdge(graph, 5,8,1);
    addEdge(graph, 5,2,1);
    addEdge(graph, 9,8,1);
    addEdge(graph, 7,8,1);
    addEdge(graph, 10,9,1);
    addEdge(graph, 6,9,1);
    addEdge(graph, 6,7,1);
    addEdge(graph, 3,7,1);
    addEdge(graph, 6,3,1);
    addEdge(graph, 1,3,1);
    addEdge(graph, 1,4,1);
    addEdge(graph, 4,10,1);
    addEdge(graph, 2,4,1);
    addEdge(graph, 1,2,1);

    
    for(int i = 1; i <= v; i++)
        addEdge(graph, 0, i, delta);
    printGraph(graph, v);
    
   full(graph, v, V);

    printPath(solution, v);

}
