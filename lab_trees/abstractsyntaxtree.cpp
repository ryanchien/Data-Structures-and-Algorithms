#include "abstractsyntaxtree.h"

/**
 * Calculates the value from an AST (Abstract Syntax Tree). To parse numbers from strings, please use std::stod
 * @return A double representing the calculated value from the expression transformed into an AST
 */
double AbstractSyntaxTree::eval() const {
    // @TODO Your code goes here...
    if (root != NULL)
				return evalHelper(root);
		else
				return 0.0;
}

double AbstractSyntaxTree::evalHelper(Node * node) const {
		//base case: null root
		if (node == NULL)
				return 0.0;
		else if (node->elem == "+")
				return evalHelper(node->left) + evalHelper(node->right);
		else if (node->elem == "-")
				return evalHelper(node->left) - evalHelper(node->right);
		else if (node->elem == "*")
				return evalHelper(node->left) * evalHelper(node->right);
		else if (node->elem == "/")
				return evalHelper(node->left) / evalHelper(node->right);
		else
				return std::stod(node->elem);
}
