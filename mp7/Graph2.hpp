#include <queue>
#include <algorithm>
#include <string>
#include <list>
#include <functional>
#include <vector>
#include <cfloat>
#include <iostream>
#include <limits>
using namespace std;

template <class V_byRef, class V>
class Compare {
	public:
		bool operator()(V_byRef lhs, V_byRef rhs) {
			V & vleft = lhs.get();
			V & vright = rhs.get();
			return stoi(vleft[vleft.key()]) > stoi(vright[vright.key()]);
		}
};

/**
 * Returns an std::list of vertex keys that creates some shortest path between `start` and `end`.
 *
 * This list MUST include the key of the `start` vertex as the first vertex in the list, the key of
 * the `end` vertex as the last element in the list, and an ordered list of all vertices that must
 * be traveled along the shortest path.
 * 
 * For example, the path a -> c -> e returns a list with three elements: "a", "c", "e".
 * 
 * @param start The key for the starting vertex.
 * @param end   The key for the ending vertex.
 */
template <class V, class E>
std::list<std::string> Graph<V,E>::shortestPath(const std::string start, const std::string end) {
	std::list<std::string> path;
	std::priority_queue<V_byRef, vector<V_byRef>, Compare<V_byRef, V>> q;
	std::unordered_map<string, string> prev;

	//iterate over vertexmap, set start dist to 0 and others to infinity and store in q
	for (auto it : vertexMap) {
		string key = it.first;
		V & v = it.second;
		if (key==start) {
			v[key] = to_string(0.0);
			q.push(v);
		}	
		else
			v[key] = to_string(INT_MAX);
		prev.insert({key, "none"}); //update map of previous vertex (key=current vertex)
	}

	//loop
	//pop from queue: current vertex
	//check incidentEdges
	//for each incidentEdge
	//find destination vertex from incidentEdge
	//calculate distance from start and update vertex distance and best previous vertex if distance is less than previously stored distance
	//stop when end vertex is popped
	while (!q.empty()) {
		V & currv = q.top();
		q.pop();
		string currkey = currv.key();
		if (currkey==end) {
			break;
		}
		auto adjEdges = incidentEdges(currv);
		for (auto edgeWrapper : adjEdges) {
			Edge & e = edgeWrapper.get();
			string nextkey = "";
			if (!e.directed())
				nextkey = (currkey==(e.dest()).key()) ? (e.source()).key() : (e.dest()).key();
			else
				nextkey = (e.dest()).key();
			V & nextv = vertexMap.at(nextkey);
			int dist = stoi(currv[currkey]) + (int)e.weight();
			if (dist < stoi(nextv[nextkey])) {
				nextv[nextkey] = to_string(dist);
				prev[nextkey] = currkey;
				q.push(nextv); //only push vertex onto queue if calculated distance is less than stored distance
			}
		}
	}
	//push end vertex onto path
	//end is key, hash key into prev map and hashed value (prev vertex) = new key, push prev vertex onto path
	//loop until hashed value is start, push start onto path
	path.push_front(end);
	string prevV = prev[end];
	while (prevV != "none") {
		path.push_front(prevV);
		prevV = prev[prevV];
	}
	return path;
}
