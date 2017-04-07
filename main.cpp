#include <pthread.h>
#include <iostream>
using namespace std;

// Define a shared buffer accessible by all functions and threads
typedef int bufferItem;
static const int BUFFER_SIZE = 5;
bufferItem buffer[BUFFER_SIZE];

// Insert an item into the buffer, returning 0 on success and -1 on failure
int insertItem(bufferItem item) {
	// TODO: write insertion logic
	return 0;
}

// Remove an item from the buffer and place it into the given item pointer,
// returning 0 on success and -1 on failure
int removeItem(bufferItem *item) {
	// TODO: write removal logic
	return 0;
}

int main() {

	return 0;

}
