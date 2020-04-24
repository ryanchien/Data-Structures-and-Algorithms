/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    if (t == NULL || t->right == NULL)
			return;
    Node * temp = t;
    Node * tRight = t->right;
		t = tRight;
		temp->right = tRight->left;
		tRight->left = temp;
		temp->height = 1 + max(heightOrNeg1(temp->left), heightOrNeg1(temp->right));
		tRight->height = 1 + max(heightOrNeg1(tRight->left), heightOrNeg1(tRight->right));
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    if (t == NULL || t->left == NULL)
			return;
	  Node * temp = t;
    Node * tLeft = t->left;
		t = tLeft;
		temp->left = tLeft->right;
		tLeft->right = temp;
		temp->height = 1 + max(heightOrNeg1(temp->left), heightOrNeg1(temp->right));
		tLeft->height = 1 + max(heightOrNeg1(tLeft->left), heightOrNeg1(tLeft->right));
	
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
		rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    if (subtree == NULL)
			return;
		subtree->height = 1 + max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right));
		int bal = heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left);
		if (bal == 1 || bal == 0 || bal == -1) //already balanced
			return;
		if (bal < 0) {
			int balLeft = heightOrNeg1((subtree->left)->right) - heightOrNeg1((subtree->left)->left);
			if (balLeft > 0)
				rotateLeftRight(subtree);
			else
				rotateRight(subtree);
		}
		else if (bal > 0) {
			int balRight = heightOrNeg1((subtree->right)->right) - heightOrNeg1((subtree->right)->left);
			if (balRight < 0)
				rotateRightLeft(subtree);
			else
				rotateLeft(subtree);
		}
		//subtree->height = 1 + max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right));
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    //base case: leaf node
    if (subtree == NULL) 
			subtree = new Node(key, value);
		//left subtree
		else if (key < subtree->key)
			insert(subtree->left, key, value);
		//right subtree
		else if (key > subtree->key)
			insert(subtree->right, key, value);
		else if (key == subtree->key)
			subtree->value = value;

		rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
				rebalance(subtree);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);
				rebalance(subtree);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
						subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
						Node *& iop_ = iop(subtree->left);
            swap(subtree, iop_);
						delete iop_;
						iop_ = NULL;
        } else {
            /* one-child remove */
            // your code here
            Node * child = subtree->right;
            if (subtree->left != NULL)
							child = subtree->left;
						delete subtree;
						subtree = child;
        }
        // your code here
        rebalance(subtree);
    }
}

template <class K, class V>
typename AVLTree<K,V>::Node*& AVLTree<K,V>::iop(Node*& subtree) {
	if(subtree->right == NULL)
		return subtree;
	else
		return iop(subtree->right);
}
