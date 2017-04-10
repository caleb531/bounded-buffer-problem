#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include "buffer.h"
using namespace std;

// Initialize a buffer shared between all functions and threads
Buffer buffer;

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

// Return the given string argument as an integer
int getIntArg(char* strArg, const char* argLabel) {
	if (!strArg) {
		cerr << "Missing argument for " << argLabel << endl;
		exit(1);
	}
	// Read string argument into stream and then read stream into integer to
	// perform conversion
	istringstream stream(strArg);
	int intArg;
	if (stream >> intArg) {
		return intArg;
	} else {
		cerr << "Invalid " << argLabel << ": " << strArg << endl;
		exit(1);
	}
}

int main(int argc, char *argv[]) {

	// How long to sleep before terminating
	int sleepTime = getIntArg(argv[1], "sleep time");
	// The number of producer threads
	int numProducers = getIntArg(argv[2], "# of producers");
	// The number of consumer threads
	int numConsumers = getIntArg(argv[3], "# of consumers");

	return 0;

}
