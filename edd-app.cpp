#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <bits/stdc++.h>
#include <set>
#include <limits.h>
#include <queue>


using namespace std;

int delta = 9;

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
    cout<<endl;
}

void printPath(int **graph, int v){
    for(int i=0; i<=v; i++){
        for(int j=0; j<=v; j++){
            if(graph[i][j] != 0){
                cout<<i<<"->"<<j<<" ";
            }
        }
        //cout<<endl;
    }
    cout<<endl;
}

void sendToNeighbors(int** graph, int v, int **Tms, int* R, int node, int* T){
    queue<int> reached;
    for(int i=1;i<=v;i++){
        if(i != node){
            if(graph[node][i] != 0 && R[i-1] != 0){
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
        sendToNeighbors(graph,v,Tms,R,next, T);
    }
}

int ** CMST(int** graph, int *R, int v){
    int** Tms= new int*[v+1];
    for(int i=0; i<=v; i++){
        Tms[i] = new int[v+1];
    }
    int T[v] = {0};

    bool check = false;
    while(!check){
        int con[v] = {0};
        for(int i=1; i<=v; i++){
            for(int j=1; j<=v; j++){
                if(graph[i][j] != 0 && Tms[i][j] == 0 && R[j-1] != 0){
                    con[i-1]++;
                }
            }
        }
        int max = 0;
        int hold;
        for(int i=0; i<v; i++){
            if(con[i] > max){
                max = con[i];
                hold = i+1;
            }
        }
        Tms[0][hold] = 1;
        T[hold-1] = 1;
        sendToNeighbors(graph, v, Tms, R, hold, T);

        check = true;
        for(int i=0;i<v;i++){
            if(R[i] != 0 && T[i] == 0)
                check = false;
        }
    }
    return Tms;
}

path dijkstraClone(int **graph, int v, int v1, int v2){
    int dist[v+1] = {INT_MAX};
    bool set[v+1] = {false};
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
        for(int i=1; i<=v; i++){
            if(graph[check.p.back()][i] != 0){
                path add;
                add.p = check.p;
                add.p.push_back(i);
                add.cost= check.cost+1;
                q.push(add);
            }
        }
    }
}

int ** EDDA(int **graph, int* R, int c, int H, int **Tms, int v){
    //initialize variables
    int **Tedda = new int*[v+1];
    int parents[v+1];
    int d[v+1];
    int cost[v+1];

    //set parents, cost, and d
    for(int i=0; i<=v; i++){
        parents[i] = -1;
        cost[i] = INT_MAX;
        d[i] = 0;
    }

    // set Tedda to Tms and set parents[v] if v path in Tedda
    for(int i=0;i<=v;i++){
        Tedda[i] = new int[v+1];
        for(int j=0;j<=v;j++){
            Tedda[i][j] = Tms[i][j];
            if(Tms[i][j] != 0){
                parents[j] = i;
            }
        }
    }

    // set cost[v] if v in Tedda
    for(int i=1;i<=v;i++){
        if(parents[i] != -1){
            if(parents[i] == 0){
                cost[i] = cost[0] + delta;
                d[i] = 0;

            }
            else{
                cost[i] = 1+cost[parents[i]];
                d[i] = 1+d[parents[i]];
            }
        }
    }

    // for each edge server in DFS order
    bool visited[v+1] = {false};
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
            for(int i =1; i<=v;i++){
                if(Tedda[s][i] != 0){
                    if(!visited[i])
                        st.push(i);
                    }
            }
            continue;
        }

        // if not a destination server or depth is already less than H, conitnue
        if(R[s-1] == 0 || d[s] <= H){
            for(int i =1; i<=v;i++){
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
        for(int i=1;i<=v;i++){
            if(Tedda[0][i] != 0){
                path temp = dijkstraClone(graph, v, i, s);
                if(temp.p.size() != 0){
                    if(temp.p.size()-1 < min){
                        min = temp.p.size()-1;
                        t = temp;
                    }
                }
            }
        }

        // if hops is less than H, update for all edges in the path
        if(min <= H){
            for(std::vector<int>::size_type it = 0; it != t.p.size(); ++it){
                parents[t.p[it]] = t.p[it-1];
                if(t.p[it-1] != 0){
                    cost[t.p[it]] = cost[t.p[it-1]] +1;
                    d[t.p[it]] = 0;
                }
                else{
                    cost[t.p[it]] = cost[t.p[it-1]] +delta;
                    d[t.p[it]] = d[t.p[it-1]] + 1;
                }
                Tedda[t.p[it-1]][t.p[it]] = 1;
                
            }

            // update shortest cost path for each node in R
            for(int i=1; i<=v; i++){
                if(i != s){
                    if(R[i-1] != 0){
                        path t2 = dijkstraClone(graph, v, s, i);
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
            cost[s] = delta;
            d[s] = 0;
            Tedda[0][s] = 1;
        }

        // Updated shortest cost path and parents for each child of s
        for(int i=1; i<=v; i++){
            if(Tedda[s][i] != 0){
                path t3 = dijkstraClone(graph, v, s, i);
                if(cost[i] > cost[s] + t3.cost){
                    parents[i] = s;
                    cost[i] = cost[s] + t3.cost;
                }
                if(cost[s] > cost[i] + t3.cost){
                    parents[s] = i;
                    cost[s] = cost[i] + t3.cost;
                }
                for(int j=1; j<=v; j++){
                    if(parents[j] == 0){
                        d[j] = 0;
                    }
                    else
                        d[j] = d[parents[j]] +1;
                }
            }
        }

        // for visiting in DFS order
        for(int i =1; i<=v;i++){
            if(Tedda[s][i] != 0){
                if(!visited[i]){
                    st.push(i);
                }
            }
        }

    }

    // Prune unused edges in Tedda by parents[]
    for(int i=0; i<=v; i++){
        for(int j=0; j<=v; j++){
            if(Tedda[i][j] != 0){
                if(parents[j] != i)
                    Tedda[i][j] = 0;
            }
        }
    }

    // return completed Tedda graph
    return Tedda;
}

int main(){
    int v = 10;
    int **graph = new int*[v+1];
    for(int i=0; i<=v; i++)
        graph[i] = new int[v+1];

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
    //printGraph(graph, v);
    int R[v] = {1,1,1,1,0,1,1,0,1,0};
    int **Tms = CMST(graph, R, v);
    //printGraph(Tms, v);
    int **Tedda = EDDA(graph, R, 0, 1, Tms, v);
    //printGraph(Tedda, v);
    cout<<"Tms path: "<<endl;
    printPath(Tms, v);
    cout<<"Tedda path: "<<endl;
    printPath(Tedda, v);

    return 0;
}
