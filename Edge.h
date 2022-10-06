#ifndef EDGE_H
#define EDGE_H

//using namespace std;

/*
adj list of 3 vertices:
vector<Edge>* adjMatrix = { new vector<Edge> };
[0] -> []
[1] -> [Edge(1, 2, 5), Edge(1, 0, 3)]
[2] -> []

add edge from vertex 1 to vertex 2 with a weight of 5:
adjList[1] = new Edge(1, 2, 5, adjList[1]);  //first head is null
adjList[1] = new Edge(1, 0, 3, adjList[1]);  //resetting the second head to new node && set next to old head
*/

struct Edge {
  int vertex0, vertex1;
  int weight;

  Edge(int _vertex0, int _vertex1, int _weight) :
    vertex0(_vertex0),
    vertex1(_vertex1),
    weight(_weight) {}

  bool operator<(const Edge& other) const { return weight < other.weight; }
  bool operator>(const Edge& other) const { return weight > other.weight; }
};

#endif