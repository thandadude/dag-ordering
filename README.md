DAG Ordering problem.
================

Source code requires GMock >= 1.1.0 & CMake >= 2.6.  Use following commands to configure &
build example:

    cd build
    cmake ..
    make
	
	
Problem: Find out ordering of Query pairs in DAG.
print output 1 if a < b, -1 if a > b, and a zero if the ordering between a and b is
undefined.


Solution: 'findQueryOrdering' API uses BFS with queue based mechanism which gives time complexity of O(v + e) where v = vertices and e = edges when
using additional visited array of size v. Additional space complexity of O(v) due to tracking if vertices are visited 
or not.

Additionally tried using topographical sort to squeeze out the additional time complexity by using divide and conquer but bfs seems to be the clear cut case as we use list to store vertices and access them like array.

The speed of the algorithm is always O(v + e) in terms of time complexity and additional space of O(v) for tracking the destination vertices to be searched.

The function 'findQueryOrdering' is already optimized to use caching or visited list vertices so it shouldn't be scanned again. Addtionally, Dijkstra’s algorithm can be used if each vertices has additional data structure consists of weights that can keep track of its underlying DAG. Then we can solve in O(E + VLogV) time using Dijkstra’s algorithm.
Since this is an un-weighted DAG, Dijkstra’s algorithm is un-optimized.

With unlimited memory accessible, I'd use hashtable with linear probing. That means each vertices has singular entry in the hashtable with separate chaining. This linked list can contain chains that are stored on heap that points to any of the hashtables slots. This will be treated as edges and serve low memory as linked lists can be simply pointers to the hash slots. Cache of already visited hashslot via traversing chains need to be added for performance.
Additionally, a locked hashtable will suit the purpose of traversing a single hashslot treated as 'findQueryOrdering' to find the ordering.
h[source] = --> --> --> destination.
Utilizing CPU core programming to provide exclusive access to each hashslot can dramatically increase the processing time for each query search. 
This can be also done on Nvidia GPU's using CUDA programming.


