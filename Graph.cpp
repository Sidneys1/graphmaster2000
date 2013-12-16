#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "Graph.h"

using namespace std;

Graph::Graph() {
	vCount = 0;
	edgeCount = 0;
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
	int count;

	//Opening and checking input is good;
	inFile.open(file);
	if(inFile.good() && !inFile.bad()) {
		
		//reading the first line and placing it in 'vCount'
		getline(inFile, line);
		stringstream ( line ) >> count;
	
		vector<string> input;

		while(getline(inFile, line)) 
			input.push_back(line);		
		inFile.close();

		for(int i = 0; i < (int)input.size(); ++i) {
			string name, v1, v2;
			float value;
			int weight;
		
			stringstream s(input.at(i));
			if(i < count) {
				s >> name >> value;
				name = name.substr(0, name.size()-1);
				addVertex(name, value);
			} else {					
				s >> v1 >> v2 >> weight;
				v1 = v1.substr(0, v1.size()-1);
				v2 = v2.substr(0, v2.size()-1);
				addEdge(v1, v2, weight);
			}
		}
	} else {
		//Error reading, return this statement.
		cout << "Invalid file: " << file << endl;
	}
}

void Graph::writeToFile(string file) {
	ofstream outFile;
	outFile.open(file, ofstream::trunc);
	if(outFile.good()) {
		outFile << vCount << "\n";
		for(int i = 0; i < vCount; ++i) 
			outFile << vertices[i]->name << ", " << vertices[i]->value << "\n";
		
		for(int j = 0; j < edgeCount; ++j) {
			outFile << edges[j]->v1 << ", " << edges[j]->v2 
				<< ", " << edges[j]->weight << "\n";
		}
	} else {
		cout << "Failed to write to file." << endl;
	}
	outFile.close();
}

bool Graph::empty() {
	return vertices.empty();
}

void Graph::addEdge(string v1, string v2, int weight) {
	if(weight > 0 && vMap.find(v1) != vMap.end() //Checkign to make sure it is a valid weight and that both of the vertices exist
		&& vMap.find(v2) != vMap.end()) {
		
		if(eMap.find(v1 + v2) == eMap.end()) { //If the edge already exists, no need for a new one, will jsut update the weight
			Edge* e = new Edge({v1, v2, weight});
			edges.push_back(e);
			eMap[v1 + v2] = edgeCount;
			eMap[v2 + v1] = edgeCount;

			edgeCount++;
		} else {
			int i = eMap[v1 + v2];
			edges[i]->weight = weight;
		}
		matrix[vMap[v1]][vMap[v2]] = weight;
		matrix[vMap[v2]][vMap[v1]] = weight;
	}
}

//For testing purposes.`
void Graph::printEdges() {
	for(int i = 0; i < vCount; ++i) {
		for(int j = 0; j < vCount; ++j)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}
}

//Pretty straight forward, just make the object and adjust the matrix accordingly.
void Graph::addVertex(string name, float val) {
	if(val > 0) {
		if(vMap.find(name) == vMap.end()) {
			Vertex* v = new Vertex({val, name, false, '\0'});
			vertices.push_back(v);
			matrix.push_back(vector<int>(vCount, 0));
			for(int i = 0; i < (vCount + 1); ++i) 
				matrix[i].push_back(0);
			values.push_back(val);
			vMap[name] = vCount;
			vCount++;
		} else {
			int v = vMap[name];
			vertices[v]->value = val;
			values[v] = val;
		}
	}
}

void Graph::printVertices() {
	for(int i = 0; i < vCount; ++i)
		cout << i << ": " << vertices[i]->name << ", " << vertices[i]->value << endl;
	
//	for(auto it = vMap.begin(); it != vMap.end(); ++it) 
//		cout << " " << it->first << " : " << it->second << endl;
}


int Graph::numConnectedComponents() {
	//Setting the dummy boolean in the structs to false.
	for(int i = 0; i < vCount; i++)
		vertices[i]->latch = false;

	int k, sSize = 0;
	sets.clear();
	//Looping through all the vertices.
	for(int i = 0; i < vCount; ++i) {

		//If the vertice hasn't been seen, will do a BFS from it and add all nodes to a set.
		//The code is very similar to BFS.
		if(!(vertices[i]->latch)) {
			queue<int> q;
			sets.push_back(set<int>());
			++sSize;
			sets[sSize-1].insert(i);
			q.push(i);
			while(!q.empty()) {
				k = q.front();
				q.pop();
				vertices[k]->latch = true;
				for(int j = 0; j < vCount; ++j) {
					if(matrix[k][j] > 0 && (vertices[j]->latch == false)) {
						q.push(j);
						sets[sSize-1].insert(j);
					}
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
	//Need to keep track of the used vertices and edges in the graph.
	vector<int> verts;
	vector<int> edge;
	int base = vMap[src];
	int size = 0;

	//Have to fill the components.
	int num = numConnectedComponents();

	for(int z = 0; z < vCount; ++z)
		(vertices[z])->latch = false;

	//Getting count of vertices in the component.
	int y = 0, f = 0;
	while(f == 0 && y < num) {
		if(sets[y].find(base) != sets[y].end()) {
			size = sets[y].size();
			f = 1;
		}
		y++;
	}

	int count = 1;
	vertices[base]->latch = true;
	verts.push_back(base);
	while(count < size) {

		string e;
		int min = 100000;
		for(int i = 0; i < count; ++i) {
			for(int j = 0; j < vCount; ++j) {
				int value = matrix[verts[i]][j];
				if(value > 0 && !vertices[j]->latch) {
					if(value < min) {
						base = j;
						min = value;
						e = (vertices[verts[i]]->name) + (vertices[j]->name);
					}
				}
			}
		}
		edge.push_back(eMap[e]);
		verts.push_back(base);
		vertices[base]->latch = true;
		count++;
	}

	//printing out the graph in format 
	cout << "{{";
	for(int t = 0; t < count; ++t) {
		cout << vertices[verts[t]]->name << (t == (count - 1) ? ")" : ", ");
	}
	cout << "}, {";
	for(int s = 0; s < (count - 1); ++s) {	
		cout << "(" << edges[edge[s]]->v1 << ", " << edges[edge[s]]->v2 
			<< (s == (count - 2) ? ")" : "), ");
	}
	cout << "}}" << endl;
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
	int v = vMap[source];
	int f = vMap[val];
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
		while(sub && j < g.edgeCount) {
				if(eMap.find(((g.edges[j])->v1) + (g.edges[j]->v2)) == eMap.end())
					sub = false;
				++j;
		}
	}
	return sub;
}

void Graph::printPathCloseVal(float value) {
	paths.clear();
	tolerance = value;
	//Collecting subsets whose sums are closest to value.
	for(int i = 0; i < vCount; ++i) {
		vector<float> a = values;
		float partial = a[i];
		a[i] = -1;
		vector<int> v(1, i);
		if(partial == value) {
			paths.push_back(v);
			tolerance = 0;
		} else if(partial < value) {
			if((value - partial) <= tolerance) { 
				if((value - partial) < tolerance) {
					paths.clear();
					tolerance = (value - partial);
				}
				paths.push_back(v);
			}	
			setSum(a, v, partial, value, i);
		} else {
			if((partial - value) <= tolerance) {
				if((partial - value) < tolerance) {
					paths.clear();
					tolerance = (partial - value);
				}
				paths.push_back(v);
			}	
		}
	}

	//Checking for repeats or permutations
	for(int i = 0; i < paths.size(); ++i) {
		for(int j = 0; j < paths.size(); ++j) {
			if(j != i) {
				if(is_permutation(paths[i].begin(), paths[i].end(), paths[j].begin())) {
					paths[i].clear();
					paths[i].push_back(-1);
				}
			}
		}		
	}

	//removing repeats and permutations.
	vector<vector<int>> print;
	for(int z = 0; z < paths.size(); ++z) {
		if(paths[z][0] != -1)
			print.push_back(paths[z]);
	}

	for(int j = 0; j < print.size(); ++j) {
		int size = print[j].size();
		for(int k = 0; k < size; ++k)
			cout << vertices[print[j][k]]->name << ((k == size - 1) ? "" : " -> ");
		cout << "\n---" << endl;
	} 
}

void Graph::setSum(vector<float> subset, vector<int> path, float psum, float value, int prev) {
	for(int i = 0; i < vCount; ++i) {
		vector<float> a = subset;
		vector<int> p = path;
		if(a[i] != -1 && matrix[prev][i] != 0) {
			float partial = psum + a[i];
			a[i] = -1;
			if(partial == value) {
				p.push_back(i);
				if(tolerance != 0)
					paths.clear();
				tolerance = 0;
				paths.push_back(p);
			} else if(partial > value) {
				if((partial - value) <= tolerance) {
					p.push_back(i);
					if((partial - value) != tolerance) {
						paths.clear();
						tolerance = (partial - value);
					}
					paths.push_back(p);
				}
			} else {
				if((value - partial) <= tolerance) {
					p.push_back(i);
					if((value - partial) != tolerance) {
						paths.clear();
						tolerance = (value - partial);
					}
					paths.push_back(p);
				} else {
					p.push_back(i);
				}
				setSum(a, p, partial, value, i);
			} 
		}
	}
}
