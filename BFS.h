#include <iostream>
#include <list>
#include "node.h"
#include "raylib.h"
#include <vector>
#pragma once
using namespace std;

class Graph
{
    int numVertices;
    bool *visited;
    int *predecessor;
    int *distance;

public:
    Graph(int vertices);
    void addEdge(vector<int> adj[], int src, int dest);
    void BFS(vector<int> adj[], int startVertex, int endVertex, Node nodes[]);
};

Graph::Graph(int vertices)
{
    numVertices = vertices;
}

void Graph::addEdge(vector<int> adj[], int src, int dest)
{
    adj[src].push_back(dest);
    adj[dest].push_back(src);
}

void Graph::BFS(vector<int> adj[], int startVertex, int endVertex, Node nodes[])
{
    visited = new bool[numVertices];    // holds whether or not a node has been visited
    predecessor = new int[numVertices]; // holds the parent node to a node on the shortest path from the start
    distance = new int[numVertices];    // holds the distance from start to any given node

    list<int> queue;

    for (int i = 0; i < numVertices; i++)
    {
        visited[i] = false;    // we start having not visited any nodes
        distance[i] = INT_MAX; // set all our distances to be inf
        predecessor[i] = -1;   // -1 means there is no parent node
    }

    visited[startVertex] = true;
    distance[startVertex] = 0;
    queue.push_back(startVertex);

    while (!queue.empty())
    {
        int currVertex = queue.front();
        queue.pop_front();
        if (currVertex != startVertex && currVertex != endVertex)
        {
            nodes[currVertex].setState(Node::Processed); // colour a node as processed
        }
        for (int i = 0; i < adj[currVertex].size(); i++)
        {
            if (visited[adj[currVertex][i]] == false)
            {
                visited[adj[currVertex][i]] = true;                      // set our node as visited
                distance[adj[currVertex][i]] = distance[currVertex] + 1; // add 1 to the distance since its an unweighted graph
                predecessor[adj[currVertex][i]] = currVertex;            // mark its parent
                queue.push_back(adj[currVertex][i]);                     // add it to the queue
                nodes[adj[currVertex][i]].setState(Node::Queued);        // colour it as queued

                if (adj[currVertex][i] == endVertex)
                {
                    nodes[adj[currVertex][i]].setState(Node::End);
                    // now we have found the end, so we want to backtrack along
                    // the shortest path and re-colour those nodes before returning
                    vector<int> path;
                    int backtracker = endVertex;
                    path.push_back(backtracker);
                    while (predecessor[backtracker] != -1)
                    {
                        path.push_back(predecessor[backtracker]);
                        if (predecessor[backtracker] != startVertex)
                        {
                            nodes[predecessor[backtracker]].setState(Node::ShortestPath);
                        }
                        backtracker = predecessor[backtracker];
                    }
                    return; // break out of the algo
                }
            }
        }
    }
}
