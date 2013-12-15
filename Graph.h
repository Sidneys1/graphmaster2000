#include <string>
#include <vector>
#include <set>
#include <unordered_map>

struct Vertex {
	float value;
	std::string name;
	bool latch;
	char color;
};

struct Edge {
	std::string v1;
	std::string v2;
	int weight;
};


//Mock node for if we cheat on the last fucntion
struct Node {
	//Vector of paths, basically each vector will hold the integer values of the vertices in an order. It
	//is two dimensional since multiple paths may have the same values. 
	std::vector<std::vector<int>> path;
	//Value of the path
	float value;
	//For use if it is placed in a tree.
	Node* right;
	Node* left;
};

class Graph {
	private:
		std::vector<std::vector<int>> matrix;
		std::vector<Edge*> edges;
		std::unordered_map<std::string, int> eMap;

		std::vector<Vertex*> vertices;
		std::unordered_map<std::string, int> vMap;
		Node* head;
		
		int vCount;
		int edgeCount;
		std::vector<std::vector<int>> sets;
		
	public:
		//Construct
		Graph();
		//Delete
		~Graph();
		//Read A Graph From A File
		void readFromFile(std::string file);
		//Write A Graph To A File
		void writeToFile(std::string file);
		//Empty
		bool empty();
		//Add Edge
		void addEdge(std::string v1, std::string v2, int weight);
		void printEdges();
		//Add Vertex
		void addVertex(std::string name, float val);
		void printVertices();
		//Count Connected Components
		int numConnectedComponents();
		//Tree Check
		bool tree();
		//Minimum Weight Components
		void minWeightComponent(std::string src);
		//Depth First Search
		bool DFS(std::string source, std::string val);
		void recurDFS(int indice, int val, bool& found);
		//Breadth First Search
		bool BFS(std::string source, std::string val);
		//Closeness
		int closeness(std::string v1, std::string v2);
		//Partition
		bool partitionable();
		bool recurPartitionable(int index, char currColor);
		//* Subgraph Test
		bool isSubGraph(const Graph& g);
		//Print Valued Path
		void printPathCloseVal(float value);
};
