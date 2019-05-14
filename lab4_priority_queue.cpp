#include "lab4_priority_queue.hpp"
#include <iostream>

using namespace std;

// PURPOSE: Parametric constructor
// initializes heap to an array of (n_capacity + 1) elements
PriorityQueue::PriorityQueue(unsigned int n_capacity) {
	heap = new TaskItem*[n_capacity + 1];
	heap[0] = NULL;
	size = 0;
	capacity = n_capacity;
}

// PURPOSE: Explicit destructor of the class PriorityQueue
PriorityQueue::~PriorityQueue() {
	for (unsigned int i = 0; i < size; i++) {
		delete heap[i];
	}
	delete [] heap;
}

// PURPOSE: Returns the number of elements in the priority queue
unsigned int PriorityQueue::get_size() const {
	return size;
}

// PURPOSE: Returns true if the priority queue is empty; false, otherwise
bool PriorityQueue::empty() const {
	if (size == 0 || capacity == 0) return true;
	return false;
}

// PURPOSE: Returns true if the priority queue is full; false, otherwise
bool PriorityQueue::full() const {
	if (size == capacity) return true;
	return false;
}

// PURPOSE: Prints the contents of the priority queue; format not specified
void PriorityQueue::print() const {
	for (unsigned int i = 1; i < size; i++) {
		cout << heap[i]->priority;
	}
}

// PURPOSE: Returns the max element of the priority queue without removing it
// if the priority queue is empty, it returns (-1, "N/A")
PriorityQueue::TaskItem PriorityQueue::max() const {
	if (empty()) return TaskItem(-1, "NULL");
	return *(heap[1]);
}

// PURPOSE: Inserts the given value into the priority queue
// re-arranges the elements back into a heap
// returns true if successful and false otherwise
// priority queue does not change in capacity
bool PriorityQueue::enqueue( TaskItem val ) {
	if (size == capacity) return false;
	if (size == 0) heap[1] = new TaskItem(val);
	else {
		int i = size + 1;
		heap[i] = new TaskItem(val);
		while (i > 1 && heap[i/2]->priority < heap[i]->priority) {
			TaskItem* temp = heap[i];
			heap[i] = heap[i/2];
			heap[i/2] = temp;
			i /= 2;
		}
	}
	++size;
	return true;
}

// PURPOSE: Removes the top element with the maximum priority
// re-arranges the remaining elements back into a heap
// returns true if successful and false otherwise
// priority queue does not change in capacity
bool PriorityQueue::dequeue() {
	if (size <= 0 || capacity == 0) return false;
	if (size == 1) {
		delete heap[1];
		heap[1] = NULL;
		--size;
	}
	else {
		// [0][5][3][4]
		// [0][4][3][4]
		// [0][4][3]
		// heap[1] < heap[2]
		heap[1]->priority = heap[size]->priority;
		delete heap[size];
		heap[size] = NULL;
		--size;
		int i = size;
		while (i > 1 && heap[i/2]->priority < heap[i]->priority) {
			TaskItem* temp = heap[i];
			heap[i] = heap[i/2];
			heap[i/2] = temp;
			i /= 2;
		}
	}
	return true;
}
