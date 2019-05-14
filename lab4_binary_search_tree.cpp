#include <iostream>
#include "lab4_binary_search_tree.hpp"
#include <queue>
using namespace std;

// PURPOSE: Default/empty constructor
BinarySearchTree::BinarySearchTree() {
	(*this).root = NULL;
	(*this).size = 0;
}

// PURPOSE: Explicit destructor of the class BinarySearchTree
BinarySearchTree::~BinarySearchTree() {
	Delete(root);
}

void BinarySearchTree::Delete(BinarySearchTree::TaskItem* node) {
	if (node != NULL) {
		Delete(node->left);
		Delete(node->right);
		delete node;
		node = NULL;
	}
}

// PURPOSE: Returns the number of nodes in the tree
unsigned int BinarySearchTree::get_size() const {
	return size;
}

// PURPOSE: Returns the maximum value of a node in the tree
// if the tree is empty, it returns (-1, "N/A")
BinarySearchTree::TaskItem BinarySearchTree::max() const {
	if (root == NULL) return TaskItem(-1, "N/A");
	TaskItem* current = root;
	while (current->right != NULL) {
		current = current->right;
	}
	return *current;
}

// PURPOSE: Returns the minimum value of a node in the tree
// if the tree is empty, it returns (-1, "N/A")
BinarySearchTree::TaskItem BinarySearchTree::min() const {
	if (root == NULL) return TaskItem(-1, "N/A");
	TaskItem* current = root;
	while (current->left != NULL) {
		current = current->left;
	}
	return *current;
}

// PURPOSE: Returns the tree height
unsigned int BinarySearchTree::height() const {
	return Height(root) - 1;

}

unsigned int BinarySearchTree::Height(BinarySearchTree::TaskItem* x) const {
	if (x == NULL)
	        return 0;
	    else
	    {
	        /* compute the depth of each subtree */
	        int lDepth = Height(x->left);
	        int rDepth = Height(x->right);

	        /* use the larger one */
	        if (lDepth > rDepth)
	            return(lDepth + 1);
	        else return(rDepth + 1);
	    }
}


// PURPOSE: Prints the contents of the tree; format not specified
void BinarySearchTree::print() const {
}

// PURPOSE: Returns true if a node with the value val exists in the tree	
// otherwise, returns false
bool BinarySearchTree::exists( BinarySearchTree::TaskItem val ) const {
	if (root == NULL) return false;
	return Exists(root, val);
}

bool BinarySearchTree::Exists(BinarySearchTree::TaskItem* node, BinarySearchTree::TaskItem val) const {
	if (node->priority == val.priority) return true;
	else if (node->priority < val.priority) {
		if (node->right == NULL) return false;
		else return Exists(node->right, val);
	}
	else if (node->priority > val.priority) {
		if (node->left == NULL) return false;
		else return Exists(node->left, val);
	}
	return false;
}

// PURPOSE: Optional helper function that returns a pointer to the root node
BinarySearchTree::TaskItem* BinarySearchTree::get_root_node() {
    return root;
}

// PURPOSE: Optional helper function that returns the root node pointer address
BinarySearchTree::TaskItem** BinarySearchTree::get_root_node_address() {
	return &root;
}

// PURPOSE: Optional helper function that gets the maximum depth for a given node
int BinarySearchTree::get_node_depth( BinarySearchTree::TaskItem* n ) const {
	TaskItem* current = root;
	int depth = 0;

	if (current == NULL) return 0;
	while (current != n) {
		if (n->priority < current->priority) {
			current = current->left;
		}
		else if (n->priority > current->priority) {
			current = current->right;
		}
		++depth;
	}
	return depth;
}

// PURPOSE: Inserts the value val into the tree if it is unique
// returns true if successful; returns false if val already exists
bool BinarySearchTree::insert( BinarySearchTree::TaskItem val ) {
	if (exists(val)) return false;
	if (root == NULL) {
		root = new TaskItem(val);
		++size;
		return true;
	}
	else return Insert(root, val);
}

bool BinarySearchTree::Insert(BinarySearchTree::TaskItem* node, BinarySearchTree::TaskItem val) {
	if (val.priority < node->priority) {
		if (node->left == NULL) {
			node->left = new TaskItem(val);
			++size;
			return true;
		}
		else return Insert(node->left, val);
	}
	else if (val.priority > node->priority) {
		if (node->right == NULL) {
			node->right = new TaskItem(val);
			++size;
			return true;
		}
		else return Insert(node->right, val);
	}
	return false;
}

// PURPOSE: Removes the node with the value val from the tree
// returns true if successful; returns false otherwise
bool BinarySearchTree::remove( BinarySearchTree::TaskItem val ) {
	// Check for empty tree/invalid remove
	if (!exists(val) || size == 0) return false;

	// Base Case: Root with no children
	else if (size == 1 && root->priority == val.priority) {
		delete root;
		root = NULL;
		--size;
		return true;
	}
	// Go to recursive helper
	return RemoveLeaf(root, root, val);
}

bool BinarySearchTree::RemoveLeaf(BinarySearchTree::TaskItem* parent, BinarySearchTree::TaskItem* node, BinarySearchTree::TaskItem val) {
	// These steps cover traversal to the node to be removed
	// If current node is less than the node to be removed, we want to go to right subtree
	if (node->priority < val.priority) {
		if (node->right != NULL) return RemoveLeaf(node, node->right, val);
	}
	// Otherwise go to left subtree
	else if (node->priority > val.priority) {
		if (node->left != NULL) return RemoveLeaf(node, node->left, val);
	}
	// Say we reach the desired node
	else if (node->priority == val.priority) {
		// Case 1: No child Nodes
		if (node->right == NULL && node->left == NULL) {
			// compare node->priority with parent->priority
			if (node->priority > parent->priority) {
				delete node;
				node = NULL;
				parent->right = NULL;
				--size;
				return true;
			}
			else if (node->priority < parent->priority) {
				delete node;
				node = NULL;
				parent->left = NULL;
				--size;
				return true;
			}
			// Special case of root node removal
			else if (node->priority == parent->priority) {
				delete node;
				node = NULL;
				parent->right = NULL; // since we always look for min in right subtree
				--size;
				return true;
			}
		}
		// Case 2: One child node
		else if (node->right != NULL && node->left == NULL) {
			// set predecessor equal to node->right
			if (node == root) {
				root = root->right;
				delete root->right;
				root->right = NULL;
				--size;
			}
			else {
				parent->right = node->right;
				delete node;
				node = NULL;
				--size;
			}
		}
		else if (node->right == NULL && node->left != NULL) {
			if (node == root) {
				root = root->left;
				delete root->left;
				root->left = NULL;
				--size;
			}
			else {
				// set predecessor equal to node->right
				parent->right= node->left;
				delete node;
				node = NULL;
				--size;
			}
		}
		// Case 3: Two child nodes
		else if (node->left != NULL && node->right != NULL) {
			// replace node to be deleted with minimum node in right subtree
			TaskItem min = Min(node->right);
			node->priority = min.priority;
			// Remove minimum node
			return RemoveLeaf(node, node->right, min);
		}
		return true;
	}
	return false;
}

BinarySearchTree::TaskItem BinarySearchTree::Min(BinarySearchTree::TaskItem* node) {
	if (node->left == NULL) {
		return *node;
	}
	else return Min(node->left);
}


