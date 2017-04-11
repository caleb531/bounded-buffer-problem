/**
 * Shared buffer implementation
 */

#include <iostream>
#include <sstream>
#include <string>
#include "buffer.h"
using namespace std;

Buffer::Buffer() {
	front = 0;
	rear = -1;
	count = 0;
}

int Buffer::insertItem(bufferItem item) {
	if (count < BUFFER_MAX_SIZE) {
		rear = (rear + 1) % BUFFER_MAX_SIZE;
		buffer[rear] = item;
		count += 1;
		return 0;
	} else {
		return -1;
	}
}

int Buffer::removeItem(bufferItem& item) {
	if (count > 0) {
		item = buffer[front];
		front = (front + 1) % BUFFER_MAX_SIZE;
		count -= 1;
		return 0;
	} else {
		return -1;
	}
}

string Buffer::str() {
	ostringstream ss;
	ss << "[";
	// Every item in the buffer except the last should be followed by a comma
	for (int i = front; i != rear; i = (i + 1) % BUFFER_MAX_SIZE) {
		ss << buffer[i] << ", ";
	}
	if (count > 0) {
		ss << buffer[rear];
	}
	ss << "]";
	return ss.str();
}
