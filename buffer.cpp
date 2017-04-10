/**
 * Shared buffer implementation
 */

#include <iostream>
#include "buffer.h"
using namespace std;

Buffer::Buffer() {
	front = 0;
	rear = -1;
	count = 0;
}

void Buffer::insert(bufferItem item) {
	if (count < BUFFER_MAX_SIZE) {
		rear = (rear + 1) % BUFFER_MAX_SIZE;
		buffer[rear] = item;
		count += 1;
	}
}

bufferItem* Buffer::remove() {
	if (count > 0) {
		bufferItem* item;
		*item = buffer[front];
		front = (front + 1) % BUFFER_MAX_SIZE;
		count -= 1;
		return item;
	} else {
		return NULL;
	}
}

void Buffer::print() {
	cout << "[ ";
	for (int i = 0; i < count; i++) {
		cout << buffer[i] << ' ';
	}
	cout << "]";
}
