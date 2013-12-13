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

	cout << (g.BFS("Paris", 18) ? "Found." : "Not Found.") << endl;
	//Should be found

	cout << (g.BFS("Geneva", 500) ? "Found." : "Not Found.") << endl;
	//Should not be found.

	/********Testing Closeness***************/
	
	cout << "New York - Geneva: " << g.closeness("New York", "Geneva") << endl;
	//should return 2, possible error of 3.
}
