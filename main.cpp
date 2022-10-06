#include <iostream>
#include <fstream>

#include "Graph.h"
#include "ArgumentManager.h"

using namespace std;

int main(int argc, char *argv[]) {
  ArgumentManager am(argc, argv);
  ifstream input(am.get("input"));
  ofstream output(am.get("output"));
  
  
  /*ifstream input("input33.txt");
  ofstream output("output31.txt");*/
  

  // Initialize variables
  int numVertices;
  input >> numVertices;
  Graph graph(numVertices);

  // Input the edges
  int vertex0, vertex1, weight;
  
  while (input >> vertex0 >> vertex1 >> weight) {
    graph.addEdge(vertex0, vertex1, weight);
  }

  // Criteria 1: Check if all locations are connected to all other locations
  for (int vertex = 0; vertex < graph.getNumVertices(); vertex++) { 
    if (!graph.isConnectedToAllVertices(vertex)) {
      output << "1. No" << endl << "Bad" << endl;
      return 0;
    }
  }
  output << "1. Yes" << endl;

  // Criteria 2: Check for locations with more than 2 adjacent locations and assign origin vertex for Criteria 3

  int originVertex = 0;
  vector<int> veryConnectedVertices; // holds vertices passing criteria 2

  for (int vertex = 0; vertex < graph.getNumVertices(); vertex++) {
    graph.getNumEdges(vertex);
    if (graph.getNumEdges(vertex) > 2) {
      veryConnectedVertices.push_back(vertex);
    }
  }

  if (veryConnectedVertices.size() > 0) {
    originVertex = veryConnectedVertices[0];
    output << "2. Yes (";
    
    for (int i = 0; i < veryConnectedVertices.size() - 1; i++) {
      int vertex = veryConnectedVertices[i];
      output << "Location " << vertex << ", ";
    }
    
    output << "Location " << veryConnectedVertices[veryConnectedVertices.size() - 1] << ")" << endl;
  }
  else {
    output << "2. No" << endl;
  }

  // Criteria 3: Compare the weight of MST and SPT. Check if difference <= 10
  int mstWeight = graph.getMSTWeight(originVertex);
  int sptWeight = graph.getSPTWeight(originVertex);

  output << ((abs(mstWeight - sptWeight) <= 10) ? "3. Yes " : "3. No ")
    << "(MST=" << mstWeight
    << ", SPT=" << sptWeight
    << ")" << endl;

  // Print final evaluation 
  if (veryConnectedVertices.size() > 0 && (abs(mstWeight - sptWeight) <= 10)) {
    output << "Good" << endl;
  } 
  else {
    output << "Fair" << endl;
  }
  
  return 0;
}