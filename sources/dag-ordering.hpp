
#include <iostream> 
#include <list> 
#include <stack> 
#include <vector>
#include <memory>
#include <functional>
#include <exception>
#include <fstream>

class CDagOrdering
{
public:
	CDagOrdering() = default;
	CDagOrdering(unsigned int vertices);
	CDagOrdering(const std::string &path);
	~CDagOrdering() = default;


	//This api adds child & parent pair to the existing or new DAG.
	//param 1: child vertex.
	//param 2: parent vertex.
	//returns: void or throws runtime exception. 
	//exceptions can be: invalid dag structure (e.g: no of vertices = 4, vertices 6,8 are getting added)...etc
	void addEdgePair(unsigned int childVertex,unsigned int parentVertex);

	//This api loads DAG structure from file, parses it and creates DAG in memory data structure.
	//param: path to file.
	//returns: void or throws runtime exception or file not found error.
	void loadDAGFile();

	//Outputs ordering information from the mQueryList.
	//1 if a < b, -1 if a > b, and a zero if the ordering between a and b is undefined.
	//returns nothing and throws nothing.
	//O(v + e) where v = vertices and e = edges when using additional visited array of size v.
	//Additional space complexity of O(v) due to 
	void findQueryOrdering();

	//Topographical sort then run through bfs to identify ordering within source-destination.
	//found that topographical sort doesn't matter much as the worst case scenario is always 
	//O(v + e) where v = vertices and e = edges when using additional visited array of size v.
	void topologicalSort();

	//displays DFS sorted DAG.
	void display();

	//displays adjacency list of DAG.
	void displayAll();

private:

	unsigned int mVertices;						//maintains max vertices that can be added.

	std::string  mDagfilePath;					//loads dag structure from file.
	std::unique_ptr<std::list<int>[]> mDagList;	//maintains array of DAG edges.
	std::vector<std::pair<int, int>> mQueryList;//the query list.

};
