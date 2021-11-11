#ifndef EDD_APP
#define EDD_APP

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


struct path{
    vector<int> p;
    int cost = INT_MIN;
};

struct CompareCost{
    bool operator()(path const& p1, path const& p2)
    {
        return p1.cost > p2.cost;
    }
};

void sendToNeighbors(Graph* g, int **Tms, int node, int* T){
    queue<int> reached;
    for(int i=1;i<=g->v;i++){
        if(i != node){
            if(g->graph[node][i] != 0 && g->R[i-1] != 0){
                if(T[i-1] == 0){
                    Tms[node][i] = 1;
                    T[i-1] = 1;
                    reached.push(i);
                }
            }
        }
    }
    int next;
    while(!reached.empty()){
        next = reached.front();
        reached.pop();
        sendToNeighbors(g, Tms, next, T);
    }
}

int ** CMST(Graph* g){
    int** Tms= new int*[g->v+1];
    for(int i=0; i<=g->v; i++){
        Tms[i] = new int[g->v+1];
    }
    int T[g->v] = {0};

    bool check = false;
    while(!check){
        int con[g->v] = {0};
        for(int i=1; i<=g->v; i++){
            for(int j=1; j<=g->v; j++){
                if(g->graph[i][j] != 0 && Tms[i][j] == 0 && g->R[j-1] != 0){
                    con[i-1]++;
                }
            }
        }
        int max = 0;
        int hold;
        for(int i=0; i<g->v; i++){
            if(con[i] > max){
                max = con[i];
                hold = i+1;
            }
        }
        Tms[0][hold] = 1;
        T[hold-1] = 1;
        sendToNeighbors(g, Tms, hold, T);

        check = true;
        for(int i=0;i<g->v;i++){
            if(g->R[i] != 0 && T[i] == 0)
                check = false;
        }
    }
    return Tms;
}

path dijkstraClone(Graph* g, int v1, int v2){
    int dist[g->v+1] = {INT_MAX};
    bool set[g->v+1] = {false};
    dist[v1] = 0;
    priority_queue<path, vector<path>, CompareCost> q;

    path temp;
    temp.cost = 0;
    temp.p.push_back(v1);

    q.push(temp);

    while(!q.empty()){
        path check = q.top();
        q.pop();
        if(check.p.back() == v2)
            return check;
        for(int i=1; i<=g->v; i++){
            if(g->graph[check.p.back()][i] != 0){
                path add;
                add.p = check.p;
                add.p.push_back(i);
                add.cost= check.cost+1;
                q.push(add);
            }
        }
    }
}

int ** EDDA(Graph* g, int **Tms){
    //initialize variables
    int **Tedda = new int*[g->v+1];
    int parents[g->v+1];
    int d[g->v+1];
    int cost[g->v+1];

    //set parents, cost, and d
    for(int i=0; i<=g->v; i++){
        parents[i] = -1;
        cost[i] = INT_MAX;
        d[i] = 0;
    }

    // set Tedda to Tms and set parents[v] if v path in Tedda
    for(int i=0;i<=g->v;i++){
        Tedda[i] = new int[g->v+1];
        for(int j=0;j<=g->v;j++){
            Tedda[i][j] = Tms[i][j];
            if(Tms[i][j] != 0){
                parents[j] = i;
            }
        }
    }

    // set cost[v] if v in Tedda
    for(int i=1;i<=g->v;i++){
        if(parents[i] != -1){
            if(parents[i] == 0){
                cost[i] = cost[0] + g->delta;
                d[i] = 0;

            }
            else{
                cost[i] = 1+cost[parents[i]];
                d[i] = 1+d[parents[i]];
            }
        }
    }

    // for each edge server in DFS order
    bool visited[g->v+1] = {false};
    stack<int> st;
    st.push(0);

    // loop through each server in Tedda in DFS order
    while(!st.empty()){
        int s = st.top();
        st.pop();
        if(!visited[s])
            visited[s] = true;

        // if cloud node, add edges reached by cloud and continue
        if(s == 0){
            for(int i =1; i<=g->v;i++){
                if(Tedda[s][i] != 0){
                    if(!visited[i])
                        st.push(i);
                    }
            }
            continue;
        }

        // if not a destination server or depth is already less than dLimit, conitnue
        if(g->R[s-1] == 0 || d[s] <= g->dLimit){
            for(int i =1; i<=g->v;i++){
                if(Tedda[s][i] != 0){
                    if(!visited[i])
                        st.push(i);
                }
            }
            continue;
        }

        //find min cost path [c-s-v]
        int min = std::numeric_limits<int>::max();
        path t;
        for(int i=1;i<=g->v;i++){
            if(Tedda[0][i] != 0){
                path temp = dijkstraClone(g, i, s);
                if(temp.p.size() != 0){
                    if(temp.p.size()-1 < min){
                        min = temp.p.size()-1;
                        t = temp;
                    }
                }
            }
        }

        // if hops is less than g->dLimit, update for all edges in the path
        if(min <= g->dLimit){
            for(std::vector<int>::size_type it = 0; it != t.p.size(); ++it){
                parents[t.p[it]] = t.p[it-1];
                if(t.p[it-1] != 0){
                    cost[t.p[it]] = cost[t.p[it-1]] +1;
                    d[t.p[it]] = 0;
                }
                else{
                    cost[t.p[it]] = cost[t.p[it-1]] + g->delta;
                    d[t.p[it]] = d[t.p[it-1]] + 1;
                }
                Tedda[t.p[it-1]][t.p[it]] = 1;
                
            }

            // update shortest cost path for each node in R
            for(int i=1; i<=g->v; i++){
                if(i != s){
                    if(g->R[i-1] != 0){
                        path t2 = dijkstraClone(g, s, i);
                        if(t2.cost != INT_MAX){
                            if(cost[i] > cost[s] + t2.cost){
                                cost[i] = cost[s] + t2.cost;
                                for(std::vector<int>::size_type it = 0; it != t2.p.size(); ++it){
                                    Tedda[t2.p[it-1]][t2.p[it]] = 1;
                                    d[t2.p[it]] = d[t2.p[it-1]] + 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        // If depth limit is still exceeded, add path from cloud to s
        else{
            parents[s] = 0;
            cost[s] = g->delta;
            d[s] = 0;
            Tedda[0][s] = 1;
        }

        // Updated shortest cost path and parents for each child of s
        for(int i=1; i<=g->v; i++){
            if(Tedda[s][i] != 0){
                path t3 = dijkstraClone(g, s, i);
                if(cost[i] > cost[s] + t3.cost){
                    parents[i] = s;
                    cost[i] = cost[s] + t3.cost;
                }
                if(cost[s] > cost[i] + t3.cost){
                    parents[s] = i;
                    cost[s] = cost[i] + t3.cost;
                }
                for(int j=1; j<=g->v; j++){
                    if(parents[j] == 0){
                        d[j] = 0;
                    }
                    else
                        d[j] = d[parents[j]] +1;
                }
            }
        }

        // for visiting in DFS order
        for(int i =1; i<=g->v;i++){
            if(Tedda[s][i] != 0){
                if(!visited[i]){
                    st.push(i);
                }
            }
        }

    }

    // Prune unused edges in Tedda by parents[]
    for(int i=0; i<=g->v; i++){
        for(int j=0; j<=g->v; j++){
            if(Tedda[i][j] != 0){
                if(parents[j] != i)
                    Tedda[i][j] = 0;
            }
        }
    }

    // return completed Tedda graph
    return Tedda;
}

#endif
