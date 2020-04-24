#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

#include <string>
#include <iostream>

using namespace std;

/**
* @return The number of vertices in the Graph
*/
template <class V, class E>
unsigned int Graph<V,E>::size() const {
  // TODO: Part 2
	return vertexMap.size();
}


/**
* @return Returns the degree of a given vertex.
* @param v Given vertex to return degree.
*/
template <class V, class E>
unsigned int Graph<V,E>::degree(const V & v) const {
  // TODO: Part 2
	auto adjedgelist = incidentEdges(v);
	return adjedgelist.size();
}


/**
* Inserts a Vertex into the Graph by adding it to the Vertex map and adjacency list
* @param key The key of the Vertex to insert
* @return The inserted Vertex
*/
template <class V, class E>
V & Graph<V,E>::insertVertex(std::string key) {
  // TODO: Part 2
	V & v = *(new V(key));
	vertexMap.insert({key, v});
	list<edgeListIter> adjEdges;
	adjList.insert({key, adjEdges});
	return v;
}


/**
* Removes a given Vertex
* @param v The Vertex to remove
*/
template <class V, class E>
void Graph<V,E>::removeVertex(const std::string & key) {
  // TODO: Part 2
	//remove all incident edges from edgelist and adjlist
	list<edgeListIter> adjEdges = adjList[key];
	for (auto it=adjEdges.begin(); it!=adjEdges.end(); it++) {
		auto eLI = *it;
		Edge & e = (*eLI).get();
		removeEdge(e.source(), e.dest());
	}
	//remove vertex from vertexlist and adjlist
	vertexMap.erase(key);
	adjList.erase(key);
}


/**
* Inserts an Edge into the adjacency list
* @param v1 The source Vertex
* @param v2 The destination Vertex
* @return The inserted Edge
*/
template <class V, class E>
E & Graph<V,E>::insertEdge(const V & v1, const V & v2) {
  // TODO: Part 2
	//insert into edge list
	E & e = *(new E(v1, v2));
	edgeList.push_front(e);
	//insert into adj list
	auto it = edgeList.begin();
	string key1 = v1.key();
	string key2 = v2.key();
	list<edgeListIter> & adj1 = adjList[key1];
	list<edgeListIter> & adj2 = adjList[key2];
	adj1.push_front(it);
	adj2.push_front(it);
	return e;
}


/**
* Removes an Edge from the Graph
* @param key1 The key of the ource Vertex
* @param key2 The key of the destination Vertex
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const std::string key1, const std::string key2) {  
  // TODO: Part 2
	list<edgeListIter> & adj1 = adjList[key1];
	for (auto it=adj1.begin(); it!=adj1.end(); it++) {
		auto eLI = *it;
		Edge & e = (*eLI).get();
		//undirected edge a->b == b->a
		if (!e.directed()) {
			if ((e.source()==key1 && e.dest()==key2) || (e.source()==key2 && e.dest()==key1)) {
				edgeList.erase(eLI);
				adj1.erase(it);
				break;
			}
		}
		else {
			if (e.source()==key1 && e.dest()==key2) {
				edgeList.erase(eLI);
				adj1.erase(it);
				break;
			}
		}	
	}
	//if edge is undirected, have to remove second instance of edge from adjlist
	list<edgeListIter> & adj2 = adjList[key2];
	for (auto it=adj2.begin(); it!=adj2.end(); it++) {
		auto eLI = *it;
		Edge & e = (*eLI).get();
		if (!e.directed()) {
			if ((e.source()==key2 && e.dest()==key1) || (e.source()==key1 && e.dest()==key2)) {
				adj2.erase(it);
				break;
			}
		}
		else {
			if (e.source()==key2 && e.dest()==key1) {
				adj2.erase(it);
				break;
			}
		}
	}
}


/**
* Removes an Edge from the adjacency list at the location of the given iterator
* @param it An iterator at the location of the Edge that
* you would like to remove
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const edgeListIter & it) {
  // TODO: Part 2
	Edge & e = (*it).get();
	removeEdge(e.source(), e.dest());
}


/**
* @param key The key of an arbitrary Vertex "v"
* @return The list edges (by reference) that are adjacent to "v"
*/
template <class V, class E>  
const std::list<std::reference_wrapper<E>> Graph<V,E>::incidentEdges(const std::string key) const {
  // TODO: Part 2
	std::list<std::reference_wrapper<E>> edges;
	const list<edgeListIter> & adj = adjList.at(key);
	for (auto it=adj.begin(); it!=adj.end(); it++) {
		auto eLI = *it;
		auto rw = *eLI;
		Edge & e = rw.get();
		if (e.directed()) {
			if (e.source() == key)
				edges.push_front(rw);
		}
		else
			edges.push_front(rw);
	}
	return edges;
}


/**
* Return whether the two vertices are adjacent to one another
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
* @return True if v1 is adjacent to v2, False otherwise
*/
template <class V, class E>
bool Graph<V,E>::isAdjacent(const std::string key1, const std::string key2) const {
  // TODO: Part 2
	const list<edgeListIter> & adj1 = adjList.at(key1);
	for (auto it=adj1.begin(); it!=adj1.end(); it++) {
		auto eLI = *it;
		Edge & e = (*eLI).get();
		if (e.source()==key1 && e.dest()==key2) {
			return true;
		}	
	}
	return false;
}
