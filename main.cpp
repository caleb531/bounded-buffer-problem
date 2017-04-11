/**
 * CS 433 Programming Assignment 4: Producer/Consumer Problem
 * Caleb Evans, Jonathan Tapia
 * April 19, 2017
 * @description
 * This program is a simulation of the producer/consumer (or bounded buffer)
 * problem using semaphores to ensure synchronization
 * @howToRun
 * Run `make` to compile the program, then execute the compiled program by
 * running `./bpp.out` with three arguments. The first argument is the number of
 * seconds the program will sleep before timing out and terminating. The second
 * argument is the number of producer threads to create. The third and final
 * argument is the number of consumer threads to create.
 */
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

// bufNotInUse ensures that the buffer is never being used by two or more
// threads at the same time (this is equivalent to the "mutex" lock)
pthread_mutex_t bufNotInUse;
// bufNotEmpty prevents the consumer from consuming if the buffer is empty (this
// is equivalent to the "full" semaphore)
sem_t bufNotEmpty;
// bufNotFull prevents the producer from producing if the buffer is full (this
// is equivalent to the "empty" semaphore)
sem_t bufNotFull;

// Sleep for a random number of milliseconds
void sleepRandomTime() {
	usleep((rand() % (THREAD_MAX_SLEEP_TIME * S_TO_MS)) * MS_TO_US);
}

// Insert an item into the buffer, returning 0 on success and -1 on failure
void* produce(void *ptr) {
	while (true) {
		sleepRandomTime();

		// Wait until the buffer is no longer full
		sem_wait(&bufNotFull);
		// Wait until the buffer is no longer in use
		pthread_mutex_lock(&bufNotInUse);

		bufferItem item = rand() % 100;
		int insertStatus = buffer.insertItem(item);
		// If the buffer is not full
		if (insertStatus == 0) {
			// Even with the semaphores in place, cout will stil experience
			// synchronization issues if multiple items are cout'ed in sequence;
			// to solve this, write the output to a separate string, then cout
			// the stream as one large string; see
			// <http://stackoverflow.com/a/26909227/560642>
			ostringstream ss;
			ss << "produce " << item << " => " << buffer.str() << endl;
			cout << ss.str();
		}

		// Signify that the buffer is no longer in use
		pthread_mutex_unlock(&bufNotInUse);
		// Signify that the buffer is definitely not empty at this point
		sem_post(&bufNotEmpty);

	}
	pthread_exit(0);
}

// Remove an item from the buffer and place it into the given item pointer,
// returning 0 on success and -1 on failure
void* consume(void *ptr) {
	while (true) {
		sleepRandomTime();

		// Wait until there is an item to consume
		sem_wait(&bufNotEmpty);
		// Wait until the buffer is no longer in use
		pthread_mutex_lock(&bufNotInUse);

		bufferItem item;
		int removeStatus = buffer.removeItem(item);
		// If the buffer is not empty
		if (removeStatus == 0) {
			ostringstream ss;
			ss << "consume " << item << " => " << buffer.str() << endl;
			cout << ss.str();
		}

		// Signify that the buffer is no longer in use
		pthread_mutex_unlock(&bufNotInUse);
		// Signify that the buffer is definitely not full at this point
		sem_post(&bufNotFull);

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

// Initialize the program's mutex locks and semaphores (semaphores count as
// locks, don't they?)
void initializeLocks() {
	int pshared = 0;
	pthread_mutexattr_t attr;
	pthread_mutex_init(&bufNotInUse, &attr);
	sem_init(&bufNotEmpty, pshared, 0);
	sem_init(&bufNotFull, pshared, BUFFER_MAX_SIZE);
}

// Destroy the program's mutex locks and semaphores
void destroyLocks() {
	pthread_mutex_destroy(&bufNotInUse);
	sem_destroy(&bufNotEmpty);
	sem_destroy(&bufNotFull);
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

	initializeLocks();
	createProducers(numProducers);
	createConsumers(numConsumers);

	// Sleep for the given amount of time before terminating the program
	sleep(sleepTime);

	destroyLocks();
	cout << "terminate" << endl;
	// When main() terminates, any created child threads automatically terminate
	return 0;

}
#pragma GCC diagnostic pop
