#ifndef APPROXIMATE
#define APPROXIMATE

#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <bits/stdc++.h>
#include <set>
#include <limits.h>
#include <queue>

#include "graph.h"

using namespace std;

int delta = 9;

void addEdge(int** graph, int v1, int v2, int weight){
    graph[v1][v2] = weight;
    graph[v2][v1] = weight;
    return;
}




int minEdge(int *key, int *visited, int v){
    int min = INT_MAX;
    int ret;
    for(int i=0;i<=v; i++){
        if(visited[i] == 0 && key[i] < min){
            min = key[i];
            ret = i;
        }
    }
    return ret;
}

vector<edge> prim(int **graph, int v, int *R){
    bool visited[v+1];
    vector<edge>MST;
    priority_queue<edge, vector<edge>, CompareEdgeCost> q;
    for(int i=0;i<=v;i++){
        visited[i] = false;
    }
    int con[v];
    int max = INT_MIN;
    int highestCon;
    for(int i=1; i<=v; i++){
        con[i-1] = 0;
        for(int j=1; j<=v; j++){
            if(graph[i][j] != 0 && R[j-1] != 0){
                con[i-1]++;
            }
        }
        if(con[i-1] > max){
            max = con[i-1];
            highestCon = i;
        }
    }

    edge e;
    e.v1 = 0;
    e.v2 = highestCon;
    e.weight = delta;
    q.push(e);
    visited[0] = true;
    
    while(!q.empty()){
        e = q.top();
        q.pop();
        if( visited[e.v2])
            continue;
        MST.push_back(e);
        visited[e.v2] = true;
        for(int i=0; i<=v; i++){
            if(graph[e.v2][i] != 0 && !visited[i]){
                edge temp;
                temp.v1 = e.v2;
                temp.v2 = i;
                temp.weight = graph[e.v2][i];
                q.push(temp);
            }
        }
    }

    
    return MST;
}


stack<vector<edge> > nameFunc(Graph *g, int k, vector<edge> t, edge a){
    int count = 0;
    stack<vector<edge> > ret;
    stack<vector<edge>  > st;
    bool *track = new bool[g->v+1];
    for(int i=0;i<=g->v;i++){
        track[i] = false;
    }
    t.push_back(a);
    for(std::vector<edge>::size_type it = 0; it != t.size(); ++it){
        edge temp = t[it];
        track[temp.v1] = true;
        track[temp.v2] = true;
        if(temp.v1 == 0){
            count++;
        }
        else if(g->S[temp.v1-1] != 0)
            count++;
        if(temp.v2 == 0)
            count++;
        else if(g->S[temp.v2-1] != 0)
            count++;
    }
    edge tmp = t.back();
    if(count < k && g->S[tmp.v2-1] == 0){
        for(int i=1; i<=g->v;i++){
            if(g->graph[tmp.v2][i] != 0 && !track[i]){
                edge add;
                add.weight = g->graph[tmp.v2][i];
                add.v1 = tmp.v2;
                add.v2 = i;
                st = nameFunc(g, k, t, add);
                while(!st.empty()){
                    ret.push(st.top());
                    st.pop();
                }
            }
        }
    }
    else{
        ret.push(t);
    }

    return ret;
}



vector<vector<edge> > fullComponent(Graph* g, int k){
    vector<vector<edge> > fc;    
    vector<edge> t;
    stack<vector <edge> > hold;
    for(int i=0;i<=g->v;i++){
        for(int j=i+1;j<=g->v;j++){
            if(g->graph[i][j] != 0){
                edge add;
                add.v1 = i;
                add.v2 = j;
                add.weight = g->graph[i][j];
                t.clear();
                hold = nameFunc(g, k, t, add);
                while(!hold.empty()){
                    fc.push_back(hold.top());
                    hold.pop();
                }
            }
        }
    }
    return fc;
}

int findCost(vector<edge> g){
    int cost = 0;
    for(std::vector<edge>::size_type it = 0; it != g.size(); ++it){
        cost += g[it].weight;
    }
    return cost;
}

void findin(vector<edge> T, vector<edge> K){

}

int computeGain(vector<edge> T, vector<edge> K, int v){
    vector<edge> TK;

    int *reached = new int[v+1];
    for(int i=0;i<=v;i++){
        reached[i] = 0;
    }

    queue<int> *q = new queue<int>[v+1];
    for(std::vector<edge>::size_type it = 0; it != K.size(); ++it){
        q[K[it].v1].push(K[it].v2);
        reached[K[it].v1] = 1;
        reached[K[it].v2] = 1;
        TK.push_back(K[it]);
    }
    for(std::vector<edge>::size_type it = 0; it != T.size(); ++it){
        //q[T[it].v1].push(T[it].v2);
        reached[T[it].v1] = 1;
        reached[T[it].v2] = 1;
    }

    reached[0] = 0;
    while(!q[0].empty()){
        int tmp = q[0].front();
        q[0].pop();
        reached[tmp] = 0;
        while(!q[tmp].empty()){
            q[0].push(q[tmp].front());
            q[tmp].pop();
        }
    }
    

    for(std::vector<edge>::size_type it = 0; it != T.size(); ++it){
        bool check = true;
        if(reached[T[it].v1] != 0){
            TK.push_back(T[it]);
            reached[T[it].v1] = 0;
            while(!q[T[it].v1].empty()){
                int tmp = q[T[it].v1].front();
                q[T[it].v1].pop();
                reached[tmp] = 0;
                while(!q[tmp].empty()){
                    q[T[it].v1].push(q[tmp].front());
                    q[tmp].pop();
                }
            }
        }
        else if(reached[T[it].v2] != 0){
            TK.push_back(T[it]);
            reached[T[it].v2] = 0;
            while(!q[T[it].v2].empty()){
                int tmp = q[T[it].v2].front();
                q[T[it].v2].pop();
                reached[tmp] = 0;
                while(!q[tmp].empty()){
                    q[T[it].v2].push(q[tmp].front());
                    q[tmp].pop();
                }
            }
        }
    }
    /*
    cout<<"K"<<endl;
    printEdges(K);
    cout<<"T"<<endl;
    printEdges(T);
    cout<<"TK"<<endl;
    printEdges(TK);
    */
    return findCost(T)-findCost(TK);

}

int computeLoss(vector<edge> K, int v, int *R, vector<edge> &CK){
    int collapsed[v+1];
    int loss = 0;
    for(int i= 0; i<=v; i++)
        collapsed[i] = -1;
    for(std::vector<edge>::size_type it = 0; it != K.size(); ++it){
        edge e = K[it];
        edge temp = K[it];

        bool col = false;

        if(e.v1 != 0 ){
            if(R[e.v1 -1] == 0){
                if(collapsed[e.v1] == -1){
                    loss += e.weight;
                    if(collapsed[e.v2] != -1)
                        collapsed[e.v1] = collapsed[e.v2];
                    else
                        collapsed[e.v1] = e.v2;
                    col = true;
                }
                else{
                    temp.v1 = collapsed[e.v1];
                    //temp.v2 = e.v2;
                    //temp.weight = e.weight;
                    //CK.push_back(temp);
                    //col = true;
                }
            }
        }
        
        if(e.v2 != 0){
            if (R[e.v2 - 1] == 0){
                if(collapsed[e.v2] == -1){
                    loss += e.weight;
                    if(collapsed[e.v1] != -1)
                        collapsed[e.v2] = collapsed[e.v1];
                    else
                        collapsed[e.v2] = e.v1;
                    
                    col = true;
                }
                else{
                    //edge temp;
                    //temp.v1 = e.v1;
                    temp.v2 = collapsed[e.v2];
                    
                    //temp.weight = e.weight;
                    //CK.push_back(temp);
                    //col = true;
                }
                
            }
        }
        if(!col)
            CK.push_back(temp);
        
    }
    return loss;
}

int ** findUnion(int **graph, vector<edge> K, int v){
    for(std::vector<edge>::size_type it = 0; it != K.size(); ++it){
        edge e = K[it];
        if(graph[e.v1][e.v2] != 0){
            if(graph[e.v1][e.v2] > e.weight){
                graph[e.v1][e.v2] = e.weight;
            }
        }
        else{
            graph[e.v1][e.v2] = e.weight;
        }
    }
    return graph;

}

int ** findUnion(vector<edge> graph, vector<edge> K, int v){
    int **ret = new int*[v+1];
    for(int i=0;i<=v;i++){
        ret[i] = new int[v+1];
        for(int j=0;j<=v;j++){
            ret[i][j] = 0;
        }
    }
    for(std::vector<edge>::size_type it = 0; it != graph.size(); it++){
        edge e = graph[it];
        if(ret[e.v1][e.v2] == 0)
            ret[e.v1][e.v2] = e.weight;
        else{
            if(ret[e.v1][e.v2] > e.weight)
                ret[e.v1][e.v2] = e.weight;
        }
    }
    for(std::vector<edge>::size_type it = 0; it != K.size(); it++){
        edge e = K[it];
        if(ret[e.v1][e.v2] == 0)
            ret[e.v1][e.v2] = e.weight;
        else{
            if(ret[e.v1][e.v2] > e.weight)
                ret[e.v1][e.v2] = e.weight;
        }
    }
    return ret;
}

vector<edge> kLCA(Graph* g, int k){
    
    int **H = new int*[g->v+1];
    for(int i=0; i<=g->v; i++){
        H[i] = new int[g->v+1];
        for(int j=0; j<=g->v; j++){
            H[i][j] = 0;
            if(g->graph[i][j] != 0 && g->R[j-1] != 0){
                H[i][j] = g->graph[i][j];
            }
        }
    }
    

    /*
    int **H = new int*[g->v+1];
    for(int i=0;i<=g->v;i++){
        H[i] = new int[g->v+1];
        for(int j=0;j<=g->v; j++){
            H[i][j] = 0;
        }
    }

    for(int i=0;i<g->numDestServers;i++){
        H[0][g->R[i]] = g->delta;
        H[g->R[i]][0] = g->delta;
        for(int j=i+1; j<g->numDestServers;j++){
            H[g->R[i]][g->R[j]] = 1;
            H[g->R[j]][g->R[i]] = 1;
        }
    }
    */

    vector<edge> T = prim(H, g->v, g->S);
    while(true){
        vector<vector<edge> > fc = fullComponent(g, k);
        float r;
        float max = INT_MIN;
        vector<edge> maxK;
        vector<edge> keepCK;
        for(std::vector<edge>::size_type it = 0; it != fc.size(); ++it){
            float t;
            cout<<"full component:"<<endl;
            printEdges(fc[it]);
            vector<edge> CK;
            CK.clear();
            int loss = computeLoss(fc[it], g->v, g->S, CK);
            if(loss != 0)
                t = computeGain(T, fc[it], g->v) / loss;
            else
                t = computeGain(T, fc[it], g->v);
            if(t > max){
                max = t;
                maxK = fc[it];
                keepCK = CK;
            }
        }
        printEdges(T);
        cout<<"mak "<<endl;
        printEdges(maxK);
        cout<<"end maxl"<<endl;
        cout<<"ck"<<endl;
        printEdges(keepCK);
        cout<<"end ck"<<endl;
        cout<<"max"<<max<<endl;
        r = max;


        if(r <= 0){

            return prim(H, g->v, g->S);
        }

        
        H = findUnion(H, maxK, g->v);
        T = prim(findUnion(T, keepCK, g->v), g->v, g->S);
        
    }

    
}

#endif
