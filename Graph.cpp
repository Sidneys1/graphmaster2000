#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <algorithm>
#include "Graph.h"

using namespace std;

Graph::Graph() {
	vCount = 0;
	edgeCount = 0;
	head = nullptr;
}

Graph::~Graph() {
	for(int i = 0; i < vCount; i++)
		delete vertices[i];
	for(int j = 0; j < edgeCount; ++j)
		delete edges[j];
}

void Graph::readFromFile(string file) {
	//Setup
	ifstream inFile;
	string line;
	

	//Opening and checking input is good;
	inFile.open(file);
	if(inFile.good() && !inFile.bad()) {
		
		//reading the first line and placing it in 'vCount'
		getline(inFile, line);
		stringstream ( line ) >> vCount;
	
		vector<string> input;

		while(getline(inFile, line)) 
			input.push_back(line);		
		inFile.close();

		for(int i = 0; i < vCount; ++i) {
			string name, v1, v2;
			float value;
			int weight;
		
			stringstream s(input.at(i));
			if(i < vCount) {
				s >> name >> value;
				addVertex(name.substr(0, name.size() - 1), value);
			} else {					
				s >> v1 >> v2 >> weight;
				addEdge(v1.substr(0, v1.size() - 1), v2.substr(0, v2.size() - 1), weight);
			}
		}

	} else {
		//Error reading, return this statement.
		cout << "Invalid file: " << file << endl;
	}

}

void Graph::writeToFile(string file) {
	string house = file;
}

bool Graph::empty() {
	return vertices.empty();
}

void Graph::addEdge(string v1, string v2, int weight) {
	Edge* e = new Edge({v1, v2, weight});
	edges.push_back(e);
	eMap[v1.append(v2)] = edgeCount;
	eMap[v2.append(v1)] = edgeCount;

	matrix[vMap[v1]][vMap[v2]] = weight;
	matrix[vMap[v2]][vMap[v1]] = weight;
	edgeCount++;
}

//For testing purposes.
void Graph::printEdges() {
	for(int i = 0; i < vCount; ++i) {
		for(int j = 0; j < vCount; ++j)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}
}

//Pretty straight forward, just make the object and adjust the matrix accordingly.
void Graph::addVertex(string name, float val) {
	
	Vertex* v = new Vertex({val, name, false, '\0'});
	vertices.push_back(v);
	matrix.push_back(vector<int>(vCount, 0));
	for(int i = 0; i < (vCount + 1); ++i) {
		matrix[i].push_back(0);
	}
	vMap[name] = (vCount);
	vCount++;
}

void Graph::printVertices() {
	for(int i = 0; i < vCount; ++i) 
		cout << i << ": " << vertices[i]->name << ", " << vertices[i]->value << endl;
}


int Graph::numConnectedComponents() {
	//Setting the dummy boolean in the structs to false.
	for(int i = 0; i < vertices.size(); i++)
		vertices[i]->latch = false;

	vector<set<int>> sets;
	int k;

	//Looping through all the vertices.
	for(int i = 0; i < vCount; ++i) {

		//If the vertice hasn't been seen, will do a BFS from it and add all nodes to a set.
		//The code is very similar to BFS.
		if(!(vertices[i])->latch) {
			queue<int> q;
			sets.push_back(set<int>());
			sets[sets.size() - 1].insert(i);
			q.push(i);
			while(!q.empty()) {
				k = q.front();
				q.pop();
				vertices[k]->latch = true;
				for(int j = 0; j < vCount; ++j) {
					if(matrix[k][j] != 0 && !vertices[j]->latch)
						q.push(j);
				}
			}
		}
	}
	return sets.size();
}

//if graph is not disjoint and number of edges = #vertices - 1
bool Graph::tree() {
	if(numConnectedComponents() == 1 && edgeCount == (vCount - 1))
		return true;
	else
		return false;
}

void Graph::minWeightComponent(string src) {
	string bogus = src;
}

//Just uses recursion
bool Graph::DFS(string source, string val) {
	//Setting all latches to false
	for(int i = 0; i < vCount; i++)
		vertices[i]->latch = false;

	//resolving indice and setting up boolean
	int i = vMap[source], v = vMap[val];
	bool found = false;
	
	//Starting recursion.
	recurDFS(i, v, found);

	return found;
}

void Graph::recurDFS(int indice, int val, bool& found) {
	vertices[indice]->latch = true;
	if(indice == val)
		found = true;
	if(!found) {
		for(int i = 0; i < vCount; ++i) {
			if(matrix[indice][i] != 0 && !vertices[i]->latch)
				recurDFS(i, val, found);
		}
	}
}

bool Graph::BFS(string source, string val) {
	queue<int> q;
	for(int i = 0; i < vCount; i++)
		vertices[i]->latch = false;
	int v = vMap[source], f = vMap[val];
	q.push(v);
	while(!q.empty()) {
		v = q.front();
		q.pop();
		vertices[v]->latch = true;
		if(v == f)
			return true;
		for(int i = 0; i < vCount; ++i) {
			if(matrix[v][i] != 0 && !vertices[i]->latch)
				q.push(i);
		}
	}
	return false;
}

//Uses BFS
int Graph::closeness(string v1, string v2) {
	int one = vMap[v1], two = vMap[v2], count = 1;
	queue<int> q;
	bool found = false;
	q.push(one);
	while(!q.empty()) {
		one = q.front();
		q.pop();
		vertices[one]->latch = true;

		for(int i = 0; i < vCount; ++i) {
			if(matrix[one][i] != 0 && !vertices[i]->latch) {
				q.push(i);
				if(i == two)
					found = true;
			}
		}
		if(!found)
			count++;
		else
			break;
	}
	return count;
}

// Here be dragons
bool Graph::partitionable() {
    
    if (vertices.size() <= 2)
        return true;
    
    // Assign a blank color to all vertexes.
    for(int i = 0; i < vertices.size(); i++)
		vertices[i]->color = '\0';
	// Mark the current vertex. We don't need to worry about names, all we care about is adjacency.
	int currIndex;
	
	// Assign the first node a color..
    vertices[0]->color = 'b';
	bool zeros = false;
	
	// Starting from the first node and iterating the the last node in increments of 1...
	for (currIndex = 0; currIndex < vertices.size(); currIndex++)
	{
	    // If this node doesn't already have a color, skip it. We'll process it the second time around
	    if (vertices[currIndex]->color == '\0')
	        zeros = true;
	    else // if it does have one, we've already processed it's path
	        continue;
	        
        // If this node is not partitionable, neither is the graph.
	    if (!recurPartitionable(currIndex, 'b'))
	        return false;
	}

    // Until we have no uncolored nodes.
    while (zeros)
    {
        bool set = false;
    	// Nth iteration, we need to process all previously unprocessed nodes!
    	for (currIndex = 0; currIndex < vertices.size(); currIndex++)
    	{
    	    // If uncolored...
    	    if (vertices[currIndex]->color == '\0')
    	    {
    	        // ...and it's the first, set color and mark first
    	        if (zeros && !set)
    	        {
    	            vertices[currIndex]->color = 'b';
                    zeros = false;
                    set = true;
    	        }
    	        else // if it's not the first, let us know that there are more uncolored roots.
    	            zeros = true;
    	    }
    	    else // if it does have a color already, we've already processed its path
    	        continue;
    	        
            // If this node is not partitionable, neither is the graph.
    	    if (!recurPartitionable(currIndex, 'b'))
    	        return false;
    	}
    }
	
	// If we never return false, the answer by default is true.
	return true;
}

// And more dragons...
bool Graph::recurPartitionable(int currIndex, char currColor)
{
    char nextColor = (currColor == 'r') ? 'b' : 'r';
    
    // The interior index (to check path from node )
    int inIndex;
    for (inIndex = 0; inIndex <= vertices.size(); inIndex++)
    {
        // We don't want to check itself! We also only want to check for edges that exist!
        if (inIndex != currIndex && matrix[currIndex][inIndex] > 0)
        {
            if (vertices[inIndex]->color == '\0') // If we have not assigned a color, assign the next color.
                vertices[inIndex]->color = nextColor;
            else if (vertices[inIndex]->color == currColor) // If we have assigned a color, and it is the same as the current color, we cannot partition
                return false;
            
            // If this node is not partitionable, neither is the graph.
            if (!recurPartitionable(inIndex, nextColor))
                return false;
        }
    }
}

bool Graph::isSubGraph(const Graph& g) {
	bool sub = true;
	if(g.vCount > vCount || g.edgeCount > edgeCount)
		return false;	
	else {

		//Checking that all the same vertices exist.
		int i = 0, j = 0;
		while(sub && i < g.vCount) {
			if(vMap.find(g.vertices[i]->name) == vMap.end())	//Going to the map and seeing name resolves.
				sub = false;
			++i;
		}

		//Checking all edges if all vertices are in set.
		while(sub && j < edgeCount) {
				if(eMap.find(((g.edges[j])->v1).append(g.edges[j]->v2)) == eMap.end())
					sub = false;
				++j;
		}
	}
	return sub;
}


//So this is definitely an NP problem. Could cheat and do calculations else where I suppose; or complete it Heuristically.
void printPathCloseVal(float value) {
	/*
	So here's the problem basically. We have a set of points in which repetition may occur and other restrictions are applied.
	We have to find a subset(s) of this such that the sum of all values is closest to the provided value.

	This will typically be a problem in NP, but we may write a heuristic algorithm that will increase this but may be inaccurate.
	I have emailed her to see if we can even do this.

	And we could also cheat. My idea is to create a heap/tree or a RB tree. And each time an edge/vertex is added we basically recalculate this
	NP problem. For each path calculated we create a node that holds the path and then insert it into the tree. And then when we call this function we can just
	search the tree for closest value. Obviously will have to create this whole tree data structure, but honestly not too hard as long as not a RB tree.

	I have created a mock one of these nodes in the header.

	*/
}
