// Suppress the pesky "sem_init() is deprecated" warnings on macOS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <sstream>
#include <unistd.h>
#include "buffer.h"
using namespace std;

// Initialize a buffer shared between all functions and threads
Buffer buffer;
// The maximum number a seconds a producer/consumer thread can sleep before it
// must execute
int THREAD_MAX_SLEEP_TIME = 3;
// Seconds-to-milliseconds conversion factor
const int S_TO_MS = 1000;
// Milliseconds-to-microseconds conversion factor
const int MS_TO_US = 1000;

// Declare semaphores shared between producer and consumer
sem_t semMutex;
sem_t semFull;
sem_t semEmpty;

// Sleep for a random number of milliseconds
void sleepRandomTime() {
	usleep((rand() % (THREAD_MAX_SLEEP_TIME * S_TO_MS)) * MS_TO_US);
}

// Insert an item into the buffer, returning 0 on success and -1 on failure
void* produce(void *ptr) {
	while (true) {
		sleepRandomTime();

		sem_wait(&semEmpty);
		sem_wait(&semMutex);

		bufferItem item = rand() % 100;
		int insertStatus = buffer.insertItem(item);
		// If the buffer is not full
		if (insertStatus == 0) {
			// Write the output to a separate stream and then cout the stream as
			// one large string so as to avoid any synchronization issues with
			// cout; even with the mutex lock/semaphore in place, the data may
			// still be written to stdout after the lock is released, rendering
			// the semaphore insufficient for making cout completely thread-safe
			// (this has been confirmed by our own testing); see
			// <http://stackoverflow.com/a/26909227/560642>
			ostringstream ss;
			ss << "produce " << item << endl;
			cout << ss.str();
		}

		sem_post(&semMutex);
		sem_post(&semFull);

	}
	pthread_exit(0);
}

// Remove an item from the buffer and place it into the given item pointer,
// returning 0 on success and -1 on failure
void* consume(void *ptr) {
	while (true) {
		sleepRandomTime();

		sem_wait(&semFull);
		sem_wait(&semMutex);

		bufferItem item;
		int removeStatus = buffer.removeItem(item);
		// If the buffer is not empty
		if (removeStatus == 0) {
			ostringstream ss;
			ss << "consume " << item << endl;
			cout << ss.str();
		}

		sem_post(&semMutex);
		sem_post(&semEmpty);

	}
	pthread_exit(0);
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

void createProducers(int numProducers) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (int i = 0; i < numProducers; i++) {
		pthread_t tid;
		pthread_create(&tid, &attr, produce, NULL);
	}
}

void createConsumers(int numConsumers) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (int i = 0; i < numConsumers; i++) {
		pthread_t tid;
		pthread_create(&tid, &attr, consume, NULL);
	}
}

void initializeSemaphores() {
	int pshared = 0;
	sem_init(&semMutex, pshared, 1);
	sem_init(&semFull, pshared, 0);
	sem_init(&semEmpty, pshared, BUFFER_MAX_SIZE);
}

void destroySemaphores() {
	sem_destroy(&semMutex);
	sem_destroy(&semFull);
	sem_destroy(&semEmpty);
}

int main(int argc, char *argv[]) {

	// Seed random number generator
	srand(time(NULL));

	// How long to sleep before terminating
	int sleepTime = getIntArg(argv[1], "sleep time");
	// The number of producer threads
	int numProducers = getIntArg(argv[2], "# of producers");
	// The number of consumer threads
	int numConsumers = getIntArg(argv[3], "# of consumers");

	// Display entered program input for convenience
	cout << "sleep time: " << sleepTime << "s" << endl;
	cout << "# producers: " << numProducers << endl;
	cout << "# consumers: " << numConsumers << endl;

	initializeSemaphores();
	createProducers(numProducers);
	createConsumers(numConsumers);

	// Sleep for the given amount of time before terminating the program
	sleep(sleepTime);

	destroySemaphores();
	cout << "terminate" << endl;
	// When main() terminates, any created child threads automatically terminate
	return 0;

}
#pragma GCC diagnostic pop
