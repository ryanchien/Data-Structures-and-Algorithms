/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
		if (first[curDim] < second[curDim])
			return true;
		else if (first[curDim]==second[curDim] && first<second)
			return true;
		else
			return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
		double distCurr = 0.0;
		double distPot = 0.0;
		for (int i=0; i<Dim; i++) {
			distCurr += pow(target[i] - currentBest[i], 2);
			distPot += pow(target[i] - potential[i], 2);
		}
		if (distPot < distCurr)
			return true;
		else if (distPot==distCurr && potential<currentBest)
			return true;
		else
			return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
		vector<Point<Dim>> temp = newPoints;
		root = KDTreeHelper(temp, 0, temp.size()-1, 0);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::KDTreeHelper(vector<Point<Dim>>& vec, int left, int right, int dim) {
	if (left > right)
		return NULL;
	else {
		int medIdx = 0;
		int n = vec.size();
		quickSort(vec, left, right, dim);
		//find median index in sorted vector, check if size of vector is odd/even
		if (n%2 == 1)
			medIdx = (n-1)/2;
		else
			medIdx = (n/2)-1;
		vector<Point<Dim>> leftVec;
		vector<Point<Dim>> rightVec;
		//populate vector for left/right subtrees to recurse
		for (int i=0; i<medIdx; i++) {
			leftVec.push_back(vec[i]);
		}
		for (int i=medIdx+1; i<n; i++) {
			rightVec.push_back(vec[i]);
		}
		typename KDTree<Dim>::KDTreeNode * node = new KDTreeNode(vec[medIdx]);
		node->left = KDTreeHelper(leftVec,  0, leftVec.size()-1, (dim+1)%Dim);
		node->right = KDTreeHelper(rightVec, 0, rightVec.size()-1, (dim+1)%Dim);	
		return node;
	}
}

/*
 * quicksorts vector by passed in dimension.
 */
template <int Dim>
void KDTree<Dim>::quickSort(vector<Point<Dim>>& vec, int left, int right, int dim) {
	if (left < right) {
		int index = quickSortHelper(vec, left, right, dim);
		quickSort(vec, left, index-1, dim);
		quickSort(vec, index+1, right,dim);
	}
}

template <int Dim>
int KDTree<Dim>::quickSortHelper(vector<Point<Dim>>& vec, int left, int right, int dim) {
	Point<Dim> pivot = vec[right];
	int index = left;
	for (int i = left; i < right; i++) {
		if (smallerDimVal(vec[i], pivot, dim)) {
			swap(vec[index], vec[i]);
			index++;
		}
	}
	swap(vec[index], vec[right]);
	return index;
}


template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
	_copy(other);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
	if (&rhs != this) {
		_destroy(root);
		_copy(rhs);
	}
 	return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
	_destroy(root);
}

template <int Dim>
void KDTree<Dim>::_destroy(typename KDTree<Dim>::KDTreeNode *& node) {
	if (node==NULL)
		return;
	else {
		_destroy(node->left);
		_destroy(node->right);
		delete node;
		node = NULL;
	}
}

template <int Dim>
void KDTree<Dim>::_copy(const KDTree<Dim>& other) {
	root = other.root;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

	Point<Dim> best = findBest(query, root, 0);
	return best;
}

// traverse down kdtree to find leaf within bounds
template <int Dim>
Point<Dim> KDTree<Dim>::findBest(const Point<Dim>& query, typename KDTree<Dim>::KDTreeNode* const & node, int dim) const {
	bool exactMatch = true;
	for (int i=0; i<Dim; i++) {
		if (query[i] != node->point[i]) {
			exactMatch = false;
			break;
		}
	}
	//base case: can no longer traverse down tree or exact match
	if (exactMatch || (node->left==NULL && node->right==NULL)) {
		return node->point;
	}
	Point<Dim> currBest;
	bool traversedLeft = true;
	if (smallerDimVal(query, node->point, dim)) {
		if (node->left==NULL)
			currBest = node->point;
		else
			currBest = findBest(query, node->left, (dim+1)%Dim);
	}
    else if (!smallerDimVal(query, node->point, dim)) {
		traversedLeft = false;
		if(node->right==NULL)
			currBest = node->point;
		else
			currBest = findBest(query, node->right, (dim+1)%Dim);
	}
	//recurse left/right subtree depending on smallerDimVal
		//back traversal
		//check if curr node is closer than currBest
	if (shouldReplace(query, currBest, node->point))
		currBest = node->point;
	//check if splitting plane dist is in search radius to recurse into other subtree
	double bestRad = 0.0;
	for (int i=0; i<Dim; i++) {
		bestRad += pow(query[i] - currBest[i], 2);
	}
	double distSplitPlane = pow(query[dim] - node->point[dim], 2);
	if (distSplitPlane <= bestRad) {
		Point<Dim> newBest;
		if (traversedLeft && node->right!=NULL) {
			newBest = findBest(query, node->right, (dim+1)%Dim);
			if (shouldReplace(query, currBest, newBest))
				return newBest;
		}
		else if (!traversedLeft && node->left!=NULL) {
			newBest = findBest(query, node->left, (dim+1)%Dim);
			if (shouldReplace(query, currBest, newBest))
				return newBest;
		}
	}
	return currBest;
}
