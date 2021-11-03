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

struct edge{
    int v1;
    int v2;
    int weight;
};

struct CompareCost{
    bool operator()(edge const& e1, edge const& e2)
    {
        return e1.weight > e2.weight;
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

void printEdges(vector<edge> graph){
    for(std::vector<edge>::size_type it = 0; it != graph.size(); ++it){
        edge temp = graph[it];
        cout<<temp.v1<<"->"<<temp.v2<<" ";
    }
    cout<<endl;
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
    priority_queue<edge, vector<edge>, CompareCost> q;
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

// need to make k resctricted 
vector<vector<edge> > fullComponent(vector<edge>  graph, int v, int k){
    vector<vector<edge> > fc;

    vector<edge> t;
    bool isLeaf[v+1];
    for(int i=0;i<=v;i++){
        isLeaf[i] = false;
    }
    for(std::vector<edge>::size_type it = 0; it != graph.size(); ++it){
        edge temp = graph[it];
        if(! isLeaf[temp.v1] && ! isLeaf[temp.v2]){
            isLeaf[temp.v1] = true;
            isLeaf[temp.v2] = true;
            t.push_back(temp);
        }
        else{
            fc.push_back(t);
            t.clear();
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

int computeGain(vector<edge> T, vector<edge> K){
    vector<edge> TK;
    for(std::vector<edge>::size_type it = 0; it != T.size(); ++it){
        TK.push_back(T[it]);
    }
    for(std::vector<edge>::size_type it = 0; it != K.size(); ++it){
        TK.push_back(K[it]);
    }

    return findCost(T)-findCost(TK);

}

int computeLoss(vector<edge> K, int v, int *R, vector<edge> &CK){
    int collapsed[v+1];
    int loss = 0;
    for(int i= 0; i<=v; i++)
        collapsed[i] = 0;
    for(std::vector<edge>::size_type it = 0; it != K.size(); ++it){
        edge e = K[it];
        if(e.v1 != 0 && e.v2 != 0){
            if(R[e.v1 -1] == 0){
                if(collapsed[e.v1] == 0){
                    loss += e.weight;
                    collapsed[e.v1] = e.v2;
                    cout<<"loss v1 "<<e.v1<<" collasped. connected to "<<e.v2<<endl;
                }
                else{
                    edge temp;
                    temp.v1 = collapsed[e.v1];
                    temp.v2 = e.v2;
                    temp.weight = e.weight;
                    CK.push_back(temp);
                }
            }
            else if (R[e.v2-1] == 0){
                if(collapsed[e.v2] == 0){
                    loss += e.weight;
                    collapsed[e.v2] = e.v2;
                    cout<<"loss v2 "<<e.v2<<" collasped. connected to "<<e.v1<<endl;
                }
                else{
                    edge temp;
                    temp.v1 = e.v1;
                    temp.v2 = collapsed[e.v2];
                    temp.weight = e.weight;
                    CK.push_back(temp);
                }
            }
            else{
                CK.push_back(e);
            }
        }
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

vector<edge> kLCA(int** graph, int v, int* R, int k){
    int **H = new int*[v+1];
    for(int i=0; i<=v; i++){
        H[i] = new int[v+1];
        for(int j=0; j<=v; j++){
            H[i][j] = 0;
            if(graph[i][j] != 0 && R[j-1] != 0){
                H[i][j] = graph[i][j];
            }
        }
    }
    //printGraph(H, v);

    vector<edge> T = prim(H, v, R);
    //printEdges(T);
    //return H;
    while(true){
        vector<vector<edge> > fc = fullComponent(T, v, k);
        float r;
        float max = INT_MIN;
        vector<edge> maxK;
        vector<edge> keepCK;
        for(std::vector<edge>::size_type it = 0; it != fc.size(); ++it){
            float t;
            vector<edge> CK;
            CK.clear();
            int loss = computeLoss(fc[it], v, R, CK);
            if(loss != 0)
                t = computeGain(T, fc[it]) / loss;
            else
                t = 0;
            if(t > max){
                max = t;
                maxK = fc[it];
                keepCK = CK;
            }
        }
        r = max;
        
        if(r <= 0)
            return prim(H, v, R);

        H = findUnion(H, maxK, v);
        T = prim(findUnion(T, keepCK, v), v, R);
        
    }

    
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
    printGraph(graph, v);
    int R[v] = {1,1,1,1,0,1,1,0,1,0};

    vector<edge> MST = prim(graph, v, R);
    /*
    printEdges(MST);
    vector<vector<edge> > t = fullComponent(MST, v, 1);
    for(std::vector<edge>::size_type it = 0; it != t.size(); ++it)
        printEdges(t[it]);
    */
    
    vector<edge> H = kLCA(graph, v, R, 1);
    printEdges(H);

    return 0;
}
