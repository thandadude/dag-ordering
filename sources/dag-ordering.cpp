
#include "dag-ordering.hpp"
#include <iterator>
#include <string>
#include <regex>
#include <queue>


CDagOrdering::CDagOrdering(unsigned int vertices)
	: mVertices(vertices)
	, mDagfilePath()
	, mQueryList()
{
	mDagList.reset(new std::list<int>[vertices]);
}



CDagOrdering::CDagOrdering(const std::string& path)
	: mVertices()
	, mDagfilePath(path)
	, mDagList()
	, mQueryList()
{

}

void CDagOrdering::addEdgePair(unsigned int childVertex, unsigned int parentVertex)
{
	if (childVertex > (mVertices - 1) || (parentVertex) > (mVertices-1) ||
		childVertex < 0 || parentVertex < 0) 	{
		throw std::runtime_error("Invalid DAG pair...");
	}
	mDagList[parentVertex].push_back(childVertex);
}


void CDagOrdering::loadDAGFile()
{
	std::ifstream hDagfile;
	uint16_t tEdgePairs = 0, tQueryPairs = 0, edgePairs = 0, queryPairs = 0;
	try
	{
		hDagfile.open(mDagfilePath, std::ifstream::in);
		if (hDagfile.is_open()) {
			for (std::string line; std::getline(hDagfile, line); )
			{
				auto const re = std::regex{ R"(\s+)" };
				auto const vec = std::vector<std::string>(
					std::sregex_token_iterator{ line.begin(), line.end(), re, -1 },
					std::sregex_token_iterator{}
				);
				if (vec.size() == 3) {
					mVertices   = std::stoi(vec[0]);	//throws invalid_argument or out_of_range error.
					tEdgePairs  = std::stoi(vec[1]);	//throws invalid_argument or out_of_range error.
					tQueryPairs = std::stoi(vec[2]);	//throws invalid_argument or out_of_range error.
					mDagList.reset(new std::list<int>[mVertices]);
					mQueryList.clear();
				}
				else if (vec.size() == 2)
				{
					//parse edges
					if (edgePairs < tEdgePairs)		{
						addEdgePair(std::stoi(vec[0]), std::stoi(vec[1]));
						edgePairs++;
					}
					else if(queryPairs < tQueryPairs)
					{	//query pairs.
						mQueryList.push_back(std::make_pair(std::stoi(vec[0]), std::stoi(vec[1])));
						queryPairs++;
					}
				}
			}
			hDagfile.close();
		}
		else {
			std::cout << "Error opening File: '" << mDagfilePath << "'" << std::endl;
		}
	}
	catch (std::exception &e)	{
		if (hDagfile.is_open())		{
			hDagfile.close();
		}
		std::cout << "Exception Occurred: Cannot parse Dag file due to '" << e.what() << "'" << std::endl;
		return;
	}	
}


void CDagOrdering::display() {

	std::stack<int> Stack;
	std::vector<bool> visited(mVertices, false);

	std::function<void(int, std::vector<bool>&, std::stack<int>&)> f = [&](int v, std::vector<bool>& visited,
		std::stack<int>& Stack)
	{
		visited[v] = true;
		for (std::list<int>::iterator i = mDagList[v].begin(); i != mDagList[v].end(); ++i)
			if (!visited[*i])
				f(*i, visited, Stack);
		Stack.push(v);	// Push current vertex to stack which stores result 
	};

	// Sort starting from all vertices one by one 
	for (uint16_t i = 0; i < mVertices; i++) {
		if (visited[i] == false)
			f(i, visited, Stack);
	}

	while (Stack.empty() == false) {
		std::cout << Stack.top() << " ";
		Stack.pop();
	}
}




void CDagOrdering::displayAll()
{
	// Recur for all the vertices adjacent to this vertex 
	std::list<int>::iterator i;
	for (int k = 0; k < mVertices; k++) {
		std::cout << std::endl << "Adjacency List of " << k << ": ";
		for (i = mDagList[k].begin(); i != mDagList[k].end(); ++i) {
			std::cout << *i << " ";
		}
	}
}


//we use bfs for our un-weighted graphs as its the best use case for time complexity.
void CDagOrdering::findQueryOrdering()
{
	//create a local function called bfs.
	std::function<bool(int, int)> bfs = [&](int src, int dst)	{
		std::vector <bool> visited(mVertices, false);	//cache for visited vertices in our list.
		for (int i = 0; i < mVertices; i++)
			visited[i] = false;

		// Create a queue for BFS 
		std::list<int> queue;
		visited[src] = true;
		queue.push_back(src);

		while (!queue.empty()) {
			int s = queue.front(); queue.pop_front();

			//look in the adjacency list for our destination.
			for (std::list<int>::iterator i = mDagList[s].begin(); i != mDagList[s].end(); ++i) {
				if (*i == dst)
					return true;	//found.
				else if (!visited[*i]) {
					visited[*i] = true;
					queue.push_back(*i);
				}
			}
		}
		return false;	//not found.
	};

	//run through query list to find the ordering of child & parent vertices.
	for (auto&e: mQueryList) {

		//find a in b direction. if found, print -1, if not found then find b in a direction. if found, print 1, if not found then print 0. 
		int res = bfs(e.first, e.second) == true ? -1 : (bfs(e.second, e.first) == true ? 1 : 0);
		std::cout << std::endl << "Result: '"<< res << "'";
	}
}

//try topographical sort then run through bfs to identify ordering within source-destination.
void CDagOrdering::topologicalSort()
{
	std::vector<int> sorted;
	std::vector<bool> visited(mVertices, false);
	std::function<void(int, std::vector<bool>&, std::vector<int>&)> f = [&](int v, 
		std::vector<bool>& visited,
		std::vector<int>& sorted)
	{
		visited[v] = true;
		for (std::list<int>::iterator i = mDagList[v].begin(); i != mDagList[v].end(); ++i)
			if (!visited[*i])
				f(*i, visited, sorted);
		sorted.push_back(v);
	};

	//perform topographical sort to align the vertices with no edges on the top.
	for (uint16_t i = 0; i < mVertices; i++) {
		if (visited[i] == false)
			f(i, visited, sorted);
	}

	std::cout << std::endl;
	for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		std::cout << *it << ": ";
		for (std::list<int>::iterator i = mDagList[*it].begin(); i != mDagList[*it].end(); ++i) {
			std::cout << *i << " ";
		}
		std::cout << std::endl;
	}

	//now do find the ordering between source & destination vertices.
	//This is almost as similar as using bfs in our 'findQueryOrdering' api with O(v + e) time complexity.
	//Here, we reverse traverse the sorted DAG, to find the link that connects source->destination or vice-versa.
	//Since the time complexity is O(v + e) for sorting and O(v3).
#if 0
	for (auto& e : mQueryList) {
		std::cout << std::endl;
		for(auto &v: sorted) {
			//look in the adjacency list for our destination.
			for (std::list<int>::iterator i = mDagList[s].begin(); i != mDagList[s].end(); ++i) {
				if ()	{
				}
			}
			std::cout << std::endl;
		}
	}
#endif
}


