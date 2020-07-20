
#include "dag-ordering.hpp"

int main(int argc, char** argv) {
	try {
		
		CDagOrdering cDag("c:\\TestDagFile.txt");

		//load DAG from file.
		cDag.loadDAGFile();
		//or
		/*cDag.addEdgePair(0, 1);
		cDag.addEdgePair(1, 2);
		cDag.addEdgePair(1, 3);*/


		cDag.display();
		cDag.displayAll();

		std::cout << std::endl << "===============================" << std::endl;
		cDag.findQueryOrdering();


		//try topographical sort then run through bfs to identify ordering within source-destination.
		//found that topographical sort doesn't matter much as the worst case scenario is always 
		//O(v + e) where v = vertices and e = edges when using additional visited array of size v.
		//cDag.topologicalSort();

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}


	return 0;
}

