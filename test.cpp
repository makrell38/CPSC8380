#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "graph.h"
#include "testAlgorithms.h"
#include "edd_ip.h"
#include "edd_app.h"
#include "approximate.h"
#include <chrono>

using namespace std;

void printData(vector<vector<string> > data){
    vector<string> row;
    for(std::vector<vector<string> >::size_type it = 0; it != data.size(); ++it){
        row.clear();
        row = data[it];
        for(std::vector<string>::size_type it = 0; it != row.size(); ++it)
            cout<<row[it]<<" ";
        cout<<endl;
    }
}

int main(){

    /*
    // read in data
    vector<vector<string> > data;
    data = readInData("aws_edge_locations_na.csv");
    
    vector<vector<string> > cacheLocations;
    cacheLocations = readInData("aws_cache_locations_na.csv");
    
    // create graph
    // cloud node = 0
    // cacheLocations start at 1
    // data locations are after cacheLocations to end of graph

    int size = data.size() + cacheLocations.size();
    Graph *g = new Graph(size);
    g->dLimit = 2;
    g->delta = 9;
    cout<<g->v<<endl;

    // add edges from cachelocations to edge servers in the same region
    for(int i=0; i < cacheLocations.size(); i++){
        for(int j=0; j < data.size(); j++){
            if(cacheLocations[i][8] == data[j][8]){
                g->addEdge(i+1, cacheLocations.size()+j+1, 1);
            }
        }
    }
    
    // add cloud node edge to each edge server with weight delta
    for(int i=1; i<=size; i++){
        g->addEdge(0, i, g->delta);
    }
    // initialize all S to be 0
    g->S = new int[g->v];
    for(int i=0; i<g->v; i++){
        g->S[i] = 0;
    }
    
    // set target nodes
    // done randomly here
    int num_dest_servers = 10;
    int count = 0;
    while(count < num_dest_servers){
        int loc = rand() % size +1;
        if(g->S[loc] == 0)
            g->S[loc] = 1;
            count++;
    }

    g->setR(num_dest_servers);

    g->printGraph();
    */
    
    Graph *g = new Graph(10);
    g->delta = 9;
    g->dLimit = 2;
    g->addEdge(1,2,1);
    g->addEdge(1,5,1);
    g->addEdge(3,5,1);
    g->addEdge(2,3,1);
    g->addEdge(3,4,1);
    g->addEdge(3,6,1);
    g->addEdge(8,5,1);
    g->addEdge(9,5,1);
    g->addEdge(4,6,1);
    g->addEdge(6,7,1);
    g->addEdge(9,10,1);
    g->addEdge(7,10,1);

    for(int i=0;i<=g->v; i++){
        g->addEdge(0,i,g->delta);
    }
    
    g->S[0] = 1;
    g->S[3] = 1;
    g->S[4] = 1;
    g->S[5] = 1;
    g->S[8] = 1;
    g->S[9] = 1;
    g->setR(6);
    

    /*
    ofstream myfile;
    myfile.open("randomOutput.txt", fstream::app);



    auto start = std::chrono::high_resolution_clock::now();
    //random(g);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    myfile<<"1000 "<<duration.count()*100<<"\n";
    cout<<duration.count()<<endl;

    myfile.close();
    
    myfile.open("greedyOutput.txt", fstream::app);
    auto startg = std::chrono::high_resolution_clock::now();
    greedy(g);
    auto stopg = std::chrono::high_resolution_clock::now();
    auto durationg = std::chrono::duration_cast<std::chrono::microseconds>(stopg - startg);
    myfile<<"1000 "<<durationg.count()*100<<"\n";
    cout<<durationg.count()<<endl;
    */

    //full(g);

    /*
    Graph *Tms = new Graph(10);
    Tms->graph = CMST(g);
    //Tms->printGraph();
    Tms->printPath();
    Graph *Tedda = new Graph(10);
    Tedda->graph = EDDA(g, Tms->graph);
    //Tedda->printGraph();
    Tedda->printPath();
    */

    
    vector<edge> klca = kLCA(g, 6);
    printEdges(klca);
    Graph *g2 = new Graph(10);
    for(std::vector<edge>::size_type it = 0; it != klca.size(); ++it){
        g2->graph[klca[it].v1][klca[it].v2] = klca[it].weight;
    }
    Graph *something = new Graph(10);
    something->graph = EDDA(g, g2->graph);
    something->printPath();
    

    return 0;
}
