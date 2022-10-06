#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <limits.h>

#include "Edge.h"

using namespace std;

/*
adj list of 3 vertices:
vector<Edge>* adjList = { new vector<Edge> };
[0] -> []
[1] -> [Edge(1, 2, 5), Edge(1, 0, 3)]
[2] -> []
*/

class Graph {
private:
  int numVertices;
  vector<Edge>* adjList;
public:
  Graph(int _numVertices) :
    numVertices(_numVertices),
    adjList(new vector<Edge>[_numVertices]()) {}
  ~Graph() { delete[] adjList; }
  
  int const getNumVertices() { return numVertices; }
  int const getNumEdges(int vertex) { return adjList[vertex].size(); }
  bool const isConnectedToAllVertices(int originVertex);
  int const getMSTWeight(int originVertex);
  int const getSPTWeight(int originVertex);

  void addEdge(int vertex0, int vertex1, int weight);
};

bool const Graph::isConnectedToAllVertices(int originVertex) {
  /*
    1. Run a DFS. No need for output, only modify visited array
    2. If all vertices are visited, return true, else return false
  */

  // Step 1
  
  stack<int> s;
  bool* visited = new bool[numVertices]();

  s.push(originVertex);

  while (!s.empty()) {
    int vertex = s.top();
    s.pop();

    visited[vertex] = true;

    for (Edge edgeFromVertex : adjList[vertex]) {
      // push the adjacent vertices of all edges connected to vertex into the stack
      int adjacentVertex = edgeFromVertex.vertex1;
      
      if (!visited[adjacentVertex]) {
        s.push(adjacentVertex);
      }
    }
  }

  // Step 2

  for (int i = 0; i < numVertices; i++){
    if (!visited[i]) {
      delete[] visited;
      return false;
    }
  }

  delete[] visited;
  return true;
}

int const Graph::getMSTWeight(int originVertex) {
  /*
    Implement Prim's Algorithm
    source: https://www.geeksforgeeks.org/prims-algorithm-using-priority_queue-stl/
  */
  
  priority_queue<Edge, vector<Edge>, greater<Edge>> pq; // minheap
  bool* visited = new bool[numVertices]();
  int mstWeight = 0;

  // Push all edges of originVertex into pq
  for (Edge edge : adjList[originVertex]) {
    pq.push(edge);
  }

  while (!pq.empty()) {
    /*
      Look at the smallest edge, add it to MST if it does not result in a cycle
      1. Pop pq and store the Edge with the smallest weight
      2. Check if cycle:
        2.1 If at most 1 vertex of the Edge is visited, then it won't result in a cycle, so add its weight to the total MST weight
        2.2 Else, it will result in a cycle, do not add (continue)
      3. Push the edges of the Edge.vertex1 that do not originate from Edge.vertex0
    */

    // Step 1
    Edge edge = pq.top();
    pq.pop();

    visited[edge.vertex0] = true;

    // Step 2
    if (visited[edge.vertex0] && visited[edge.vertex1])
      continue;

    mstWeight += edge.weight;
    visited[edge.vertex1] = true;
    
    // Step 3
    for (Edge newEdge : adjList[edge.vertex1]) {
      pq.push(newEdge);
    }
  }

  delete[] visited;
  return mstWeight;
}

int const Graph::getSPTWeight(int originVertex) {
  /*
    Implement Djikstra's Algorithm
  */
  
  int* pathWeights = new int[numVertices];
  bool* visited = new bool[numVertices]();
  Graph SPT(numVertices);

  for (int i = 0; i < numVertices; i++) {
    pathWeights[i] = INT_MAX;
  }
  pathWeights[originVertex] = 0;
  
  for (int i = 0; i < numVertices; i++) {
    /*
      1. Find the unvisited index with the shortest path so far (v0) with weight (w0)
      2. Add the selected vertex v0's weight to sptWeight, set visited to true
      3. Loop through each Edge (edge):
        3.1. If edge.v1 is visited, continue
        3.2. If w0 + edge.weight < shortest path to edge.v1, update it to the newly found shortest path
    */

    // Step 1

    int vertex0 = 0;
    int weight0 = INT_MAX;

    for (int j = 0; j < numVertices; j++) {
      // cout << pathWeights[j] << endl;
      if (!visited[j] && pathWeights[j] < weight0) {
        vertex0 = j;
        weight0 = pathWeights[j];
      }
    }

    // Step 2
    // cout << "vertex0: " << vertex0 << endl << endl;
    visited[vertex0] = true;

    // Step 3
    for (Edge edge : adjList[vertex0]) {
      if (!visited[edge.vertex1]
      && pathWeights[vertex0] + edge.weight < pathWeights[edge.vertex1]) {
        pathWeights[edge.vertex1] = pathWeights[vertex0] + edge.weight;
        SPT.adjList[edge.vertex1] = SPT.adjList[vertex0];
        SPT.adjList[edge.vertex1].push_back(Edge(edge.vertex1, vertex0, edge.weight));
      }
    }
  }

  // Add the weights of all edges in the SPT, divide by 2
  int sptWeight = 0;

  visited = new bool[numVertices](); //() sets all to false
  visited[originVertex] = true;
  
  for (int vertex = 0; vertex < SPT.getNumVertices(); vertex++) {
    for (Edge edge : SPT.adjList[vertex]) {
      // cout << "(" << edge.vertex0 << ", " << edge.vertex1 << ", " << edge.weight << ")" << endl;
      if (!visited[edge.vertex0] || !visited[edge.vertex1]) {
        sptWeight += edge.weight;
        visited[edge.vertex0] = true;
        visited[edge.vertex1] = true;
      }
    }
  }
  
  delete[] pathWeights;
  delete[] visited;
  return sptWeight;
  
}

void Graph::addEdge(int vertex0, int vertex1, int weight) {
  adjList[vertex0].push_back(Edge(vertex0, vertex1, weight));
  adjList[vertex1].push_back(Edge(vertex1, vertex0, weight));
}

#endif