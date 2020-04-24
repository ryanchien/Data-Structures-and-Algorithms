/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */
#include <iostream>
/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
	if (length_ != 0)
		return List<T>::ListIterator(head_);
	else 
		return List<T>::ListIterator(NULL);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
	if (length_ != 0)
		return List<T>::ListIterator(tail_->next);
	else
		return List<T>::ListIterator(NULL);
}

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <typename T>
List<T>::~List() {
  /// @todo Graded in MP3.1
  _destroy();
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  //cout << "dtor" << endl;
	if (head_ != NULL) {
		ListNode * prevnode = NULL;
		ListNode * currnode = head_;
		for (;length_ > 0; length_--) {
			prevnode = currnode;
			currnode = currnode->next;
			delete prevnode;
		}
	}
	head_ = NULL;
	tail_ = NULL;
	//cout << "length: " << length_ << endl;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
	ListNode * node = new ListNode(ndata);
	if (length_ != 0) {
		head_->prev = node;
		node->next = head_;
		node->prev = NULL;
		head_ = node;
	}
	else {
		node->next = NULL;
		node->prev = NULL;
		head_ = node;
		tail_ = node;
	}
	length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode * node = new ListNode(ndata);
	if (length_ != 0) {
		tail_->next = node;
		node->prev = tail_;
		node->next = NULL;
		tail_ = node;
	}
	else {
		node->next = NULL;
		node->prev = NULL;
		head_ = node;
		tail_ = node;
	}
	length_++;
}

/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.1
  if (startPoint != endPoint) {
		ListNode * start = startPoint;
		ListNode * end = endPoint;
		ListNode * startprev = startPoint->prev;
		ListNode * endnext = endPoint->next;
		ListNode * node = startPoint;
		ListNode * temp = NULL;
		while (node != endnext) {
			//swap next and prev for every node
			temp = node->next;
			node->next = node->prev;
			node->prev = temp;
			node = node->prev;
		}
		if (startprev != NULL) {
			//make node before startpoint point to new start
			startprev->next = end;
			end->prev = startprev;
		}
		else {
			//make new head
			end->prev = NULL;
			head_ = end;
		}
		if (endnext != NULL) {
			//make node after endpoint point to new end
			endnext->prev = start;
			start->next = endnext;
		}
		else {
			//make new tail
			start->next = NULL;
			tail_ = start;
		}
		endPoint = start;
		startPoint = end;
	}
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.1
  if (n > length_) {
		reverse(head_, tail_);
		return;
	}
  if (length_!=0) {
		ListNode * start = head_; //startpoint to be passed into reverse()
		ListNode * end = start; //endpoint to be passed into reverse()
		while (start != NULL) {
			for (int i = 0; i < n-1; i++) { //shift endpoint n-1 times
				if ((end->next) != NULL) //check for end of list
					end = end->next;
			}
			reverse(start, end);
			//change new startpoint and endpoint
			end = end->next;
			start = end;
		}
	}
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in MP3.1
  if (length_==0 || length_==1)
		return;
	else {
		ListNode * currnode = head_->next;
		ListNode * nextnode = (head_->next)->next;
		while (currnode != NULL) {
			nextnode = currnode->next;
			if (currnode != head_ && currnode != tail_) {
				//fix pointers of prev and next nodes
				(currnode->prev)->next = currnode->next;
				(currnode->next)->prev = currnode->prev;
				//move currnode to end of list
				tail_->next = currnode;
				currnode->prev = tail_;
				currnode->next = NULL;
				tail_ = currnode;
			}
			//skip next node
			if (nextnode != NULL)
				currnode = nextnode->next;
			//end of list
			else
				return;
		}
	}
}

/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <typename T>
List<T> List<T>::split(int splitPoint) {
    if (splitPoint > length_)
        return List<T>();

    if (splitPoint < 0)
        splitPoint = 0;

    ListNode * secondHead = split(head_, splitPoint);

    int oldLength = length_;
    if (secondHead == head_) {
        // current list is going to be empty
        head_ = NULL;
        tail_ = NULL;
        length_ = 0;
    } else {
        // set up current list
        tail_ = head_;
        while (tail_ -> next != NULL)
            tail_ = tail_->next;
        length_ = splitPoint;
    }

    // set up the returned list
    List<T> ret;
    ret.head_ = secondHead;
    ret.tail_ = secondHead;
    if (ret.tail_ != NULL) {
        while (ret.tail_->next != NULL)
            ret.tail_ = ret.tail_->next;
    }
    ret.length_ = oldLength - splitPoint;
    return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.2
	//undefined behavior?
  if (splitPoint <= 0 || splitPoint > length_)
		return start;
  ListNode * currnode = NULL;
	ListNode * newnode = NULL;
	//find node to split
	for (int i = 0; i < splitPoint; i++) {
		if (currnode == NULL)
			currnode = start;
		else
			currnode = currnode->next;
	}
	if (currnode != NULL) {
		newnode = currnode->next;
		//check if currnode is tail
		if (newnode != NULL) {
			currnode->next = NULL; //set currnode as tail
			newnode->prev = NULL; //set newnode as head
		}
		return newnode;
	}
	else
		return start;
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  ListNode * temp = NULL;
	ListNode * res = NULL;
	ListNode * firstNext = NULL;
	ListNode * secondNext = NULL;

	if (first != NULL)
		firstNext = first->next;
	if (second != NULL)
		secondNext = second->next;

	if (first->data < second->data) {
		res = first;
		res->next = NULL;
		res->prev = NULL;
		temp = res;
		first = firstNext;
	}
	else {
		res = second;
		res->next = NULL;
		res->prev = NULL;
		temp = res;
		second = secondNext;
	}

  while (first!=NULL || second!=NULL) {
		if (first!=NULL && second!=NULL) {
			if (first->data < second->data)
				mergeHelper(first, temp);
			else
				mergeHelper(second, temp);
		}
		else {
			if (first != NULL) {
				if (first == temp)
					first = first->next;
				mergeHelper(first, temp);
			}
			else {
				if (second == temp)
					second = second->next;
				mergeHelper(second, temp);
			}
		}
	}
  return res;
}

template <typename T>
void List<T>::mergeHelper(ListNode *& node, ListNode *& res) {
	if (node == NULL)
		return;
	node->prev = res;
	res->next = node;
	node = node->next;
	res = res->next;
}

/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <typename T>
void List<T>::sort() {
    if (empty())
        return;
    head_ = mergesort(head_, length_);
    tail_ = head_;
    while (tail_->next != NULL)
        tail_ = tail_->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  //base case
  if (chainLength <= 1)
		return start;
	else {
  	ListNode * right = split(start, chainLength/2);
		start = mergesort(start, chainLength/2);
		right = mergesort(right, chainLength - chainLength/2);
		return merge(start, right);
	}
}
