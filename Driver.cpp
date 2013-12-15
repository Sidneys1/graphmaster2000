#include <iostream>
#include "Graph.h"

using namespace std;

int main() {
    // Note:
	// Compiles and runs. Will test functions and leave results below.
	// Sorry about indentation, working with various editors will do that.
	// 12/12/2013 22:00
    
	/*
	* All successful results are based upon limited testing.
	*
	* Empty():
	*	Result: Success!
	*
	* addEdge():
	*	Result: Success!
	*
	* addVertx():
	*	Result: Success!
	*
	* numConnectedComponents();
	*	Result: Success!
	*
	* tree():
	*	Result: Success!
	*
	* minWeightComponents(): Incomplete.
	*
	* DFS():
	*	Result: Success!
	*
	* BFS():
	*	Result: Success!
	*
	* closeness():
	*	Result: Success!
	*
	* Partitionable():
	*	Result:
	* 		Failed, both returns were 'Not Partionable!'
	*	Cause:
	*
	* isSubGraph():
	*	Result:
	*		
	*/


	Graph g;

	cout << (g.empty() ? "\nEmpty" : "\nNot Empty") << endl;
	//Should print empty.

    g.addVertex("New York", 1);
    g.addVertex("Paris", 1);
    g.addVertex("London", 1);
    g.addVertex("Geneva", 18);
    
    g.addEdge("New York", "Paris", 1);
    g.addEdge("Paris", "London", 1);
    g.addEdge("London", "Geneva", 1);

   	cout << (g.empty() ? "Empty" : "Not Empty") << endl;
	//Expected: Not Empty

	cout << "**********End of initial empty tests.****************\n" << endl;

	/******Printing Vertics then Edges*************/
	cout << "Printing vertices: " << endl;
	g.printVertices();

	cout << "\nPrinting edges: " << endl;
	g.printEdges();

    cout << ((g.partitionable()) ? "Partitionable!" : "Not Partitionable!") << endl;
    
    g.addEdge("New York", "London", 1);
    
    cout << ((g.partitionable()) ? "Partitionable!" : "Not Partitionable!") << endl;
    
    /* Expected output: 
     *
     * Partitionable!
     * Not Partitionable!
     *
     */

	/******Printing The Matrix********************/
	cout << "\nPrinting edges: " << endl;
	g.printEdges();

	/******Testing Connected Components and Tree************/
	cout << "\nConnected Components: " << g.numConnectedComponents() << endl;
	//Should return 1;
	
	cout << (g.tree() ? "Is a tree." : "Is not a tree.") << endl;
	//Should not be a tree.

	g.addVertex("Berlin", 20);
	g.addVertex("Boston", 500);

	cout << "Connected Components: " << g.numConnectedComponents() << "\n" <<  endl;
	//Should return 3
	
	//Not testing for tree again since connected works.


	/*******Printing V and E************/
	cout << "\nVertices: " << endl;
	g.printVertices();

	cout << "\nEdges: " << endl;
	g.printEdges();

	/********Testing DFS***********/

	cout << (g.DFS("New York", "Geneva") ? "Found." : "Not Found.") << endl;
	//Should be found.

	cout << (g.DFS("London", "Berlin") ? "Found." : "Not Found.") << endl;
	//Should not be found.

	/*******Testing BFS*********/
	cout << endl;

	cout << (g.BFS("Paris", "Geneva") ? "Found." : "Not Found.") << endl;
	//Should be found

	cout << (g.BFS("Geneva", "Boston") ? "Found." : "Not Found.") << endl;
	//Should not be found.

	/********Testing Closeness***************/
	
	cout << "New York - Geneva: " << g.closeness("New York", "Geneva") << endl;
	//should return 2, possible error of 3.


	/*****************************************************************************************************/
	//New Round of testing.
	cout << "/******************************************/" << endl;
	cout << "Starting testing on graph loaded from file." << endl;
	Graph h;
	h.readFromFile("g.txt");

	cout << "\nPrinting vertices:" << endl;
	h.printVertices();

	cout << "\nPrinting edges:" << endl;
	h.printEdges();

	cout << "\nNumber of connected components: " << h.numConnectedComponents() << endl;
	//should be 2

	cout << (h.tree() ? "\nIs a tree." : "\nIsn't a tree") << endl;
	//Should not be a tree.

	cout << endl;
	h.minWeightComponent("Theta");

	cout << endl;
	h.minWeightComponent("Epsilon");

	cout << "\nDFS" << endl;
	cout << (h.DFS("Alpha", "Epsilon") ? "Found." : "Not Found.") << endl;
	//hould be found.

	cout << (h.DFS("Alpha", "Theta") ? "Found." : "Not Found.") << endl;
	//Should not be found.

	cout << "\nBFS" << endl;
	cout << (h.BFS("Epsilon", "Beta") ? "Found." : "Not Found.") << endl;
	//Should be found.

	cout << (h.BFS("Gamma", "Eta") ? "Found." : "Not Found.") << endl;
	//Should not be found.

	Graph sub;
	sub.addVertex("Alpha", 1);
	sub.addVertex("Beta", 1);
	sub.addVertex("Gamma", 1);
	sub.addVertex("Zeta", 1);
	sub.addVertex("Theta", 1);
	sub.addEdge("Alpha", "Beta", 1);
	sub.addEdge("Beta", "Delta", 1);
	sub.addEdge("Zeta", "Theta", 1);

	cout << (h.isSubGraph(sub) ? "\nIs a sub graph." : "\nIs not a sub graph.") << endl;
	//Should be a sub graph.

	sub.addEdge("Zeta", "Alpha", 1);
	cout << (h.isSubGraph(sub) ? "\nIs a sub graph." : "\nIs not a sub graph.") << endl;
	//should not be.
	
	//Witing graph to file same.txt
	//Writing subgraph to file sub.txt
	h.writeToFile("same.txt");
	sub.writeToFile("sub.txt");
}
