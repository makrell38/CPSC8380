#ifndef GRAPH
#define GRAPH

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

class Graph{
    public:
        int **graph;
        int v;
        int* V;
        int *S;
        int *R;
        int delta;
        int dLimit;
        int numDestServers;
        int c = 0;
        Graph(int);
        void printGraph(void);
        void addEdge(int v1, int v2, int weight);
        void setR(int num);
        void printPath(void);
        int computeCost(void);
};

Graph::Graph(int len){
    this->v = len;
    this->graph = new int*[this->v+1];
    this->V = new int[this->v+1];
    this->S = new int[this->v];
    for(int i=0; i<=this->v; i++){
        this->V[i] = i;
        this->graph[i] = new int[this->v+1];
        for(int j=0; j<=this->v; j++){
            this->graph[i][j] = 0;
        }
    }
    for(int i=0; i<v; i++){
        this->S[i] = 0;
    }
}

void Graph::addEdge(int v1, int v2, int weight){
    this->graph[v1][v2] = weight;
    this->graph[v2][v1] = weight;
    return;
}

void Graph::printGraph(void){
    for(int i =0; i <= this->v; i++){
        cout << "[";
        for(int j=0; j<=this->v; j++){
            cout<< this->graph[i][j] << " ";
        }
        cout<<"]"<<endl;
    }
}

void Graph::setR(int num){
    this->numDestServers = num;
    this->R = new int[num];
    int t = 0;
    for(int i=0; i<this->v; i++){
        if(this->S[i] != 0){
            this->R[t] = i+1;
            t++;
        }
    }
}

void Graph::printPath(void){
    for(int i=0; i<=this->v; i++){
        for(int j=0; j<=this->v; j++){
            if(this->graph[i][j] != 0){
                cout<<i<<"->"<<j<<" ";
            }
        }
    }
    cout<<endl;
}

int Graph::computeCost(void){
    int cost = 0;
    for(int i=0; i<=this->v; i++){
        for(int j=0; j<=this->v; j++){
            if(this->graph[i][j] != 0){
                if(i == 0 || j ==0)
                    cost+=this->delta;
                else
                    cost+=1;
            }
        }
    }
    return cost;
}

vector<vector<string> > readInData(char* fileName){
    fstream f;
    f.open(fileName, ios::in);

    vector<vector<string> > ret;
    vector<string> row;
    string line, word, temp;

     while( getline(f, line) ){
        row.clear();
        stringstream s(line);
        while( getline(s, word, ',') ){
            row.push_back(word);
        }
        int num;
        temp = row[5];
        sscanf(temp.c_str(), "%d", &num);
        for(int i=0; i < num; i++){
            ret.push_back(row);
        }
    }

    f.close();

    return ret;
}

void printPath(int **graph, int v){
    for(int i=0; i<=v; i++){
        for(int j=0; j<=v; j++){
            if(graph[i][j] != 0){
                cout<<i<<"->"<<j<<" ";
            }
        }
    }
    cout<<endl;
}


struct edge{
    int v1;
    int v2;
    int weight;
};

struct CompareEdgeCost{
    bool operator()(edge const& e1, edge const& e2)
    {
        return e1.weight > e2.weight;
    }
};

void printEdges(vector<edge> graph){
    for(std::vector<edge>::size_type it = 0; it != graph.size(); ++it){
        edge temp = graph[it];
        cout<<temp.v1<<"->"<<temp.v2<<" ";
    }
    cout<<endl;
}

#endif
