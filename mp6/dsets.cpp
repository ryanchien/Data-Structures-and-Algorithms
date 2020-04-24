#include "dsets.h"
#include <iostream>
using namespace std;
/*
 * Creates n unconnected root nodes at the end of the vector
 */
void DisjointSets::addelements(int num) {
	for (int i=0; i<num; i++) {
		vec_.push_back(-1);
	}
}

/*
 * This function should compress paths and works as described in lecture
 */
int DisjointSets::find(int elem) {
	if (vec_[elem] < 0)
		return elem;
	else
		return _find(elem);
}
/*
 * recursive helper function for find
 */
int DisjointSets::_find(int elem) {
	if (vec_[elem] < 0)
		return elem;
	else {
		int root = _find(vec_[elem]);
		vec_[elem] = root;
		return root;
	}
}

/*
 * This function should be implemented as union-by-size
 */
void DisjointSets::setunion(int a, int b) {
	int root1 = find(a);
	int root2 = find(b);
	int newSize = vec_[root1] + vec_[root2];
	if (vec_[root1] <= vec_[root2]) {
		vec_[root2] = root1;
		vec_[root1] = newSize;
	}
	else {
		vec_[root1] = root2;
		vec_[root2] = newSize;
	}
}

/*
 * This function should return the number of nodes in the up-tree containing the element
 */
int DisjointSets::size(int elem) {
	//if elem is root
	if (vec_[elem] < 0)
		return vec_[elem] * -1;
	else {
		int root = vec_[elem];
		return vec_[root] * -1;
	}	
}
