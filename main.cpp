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
int THREAD_MAX_SLEEP_TIME = 5;

// Initialize semaphores shared between producer and consumer
sem_t *semMutex = sem_open("mutex", O_CREAT, O_RDWR, 1);
sem_t *semFull = sem_open("full", O_CREAT, O_RDWR, 0);
sem_t *semEmpty = sem_open("empty", O_CREAT, O_RDWR, BUFFER_MAX_SIZE);

// Insert an item into the buffer, returning 0 on success and -1 on failure
void* produce(void *ptr) {
	srand(time(NULL));
	while (true) {
		sleep(rand() % THREAD_MAX_SLEEP_TIME + 1);

		sem_wait(semEmpty);
		sem_wait(semMutex);

		bufferItem item = rand() % 100;
		int insertStatus = buffer.insertItem(item);
		// If the buffer is not full
		if (insertStatus == 0) {
			cout << "produce " << item << endl;
		}

		sem_post(semMutex);
		sem_post(semFull);

	}
	pthread_exit(0);
}

// Remove an item from the buffer and place it into the given item pointer,
// returning 0 on success and -1 on failure
void* consume(void *ptr) {
	srand(time(NULL));
	while (true) {
		sleep(rand() % THREAD_MAX_SLEEP_TIME + 1);

		sem_wait(semFull);
		sem_wait(semMutex);

		bufferItem *item;
		int removeStatus = buffer.removeItem(item);
		// If the buffer is not empty
		if (removeStatus == 0) {
			cout << "consume " << (*item) << endl;
			delete item;
		}

		sem_post(semMutex);
		sem_post(semEmpty);

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

int main(int argc, char *argv[]) {

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

	createProducers(numProducers);
	createConsumers(numConsumers);

	// Sleep for the given amount of time before terminating the program
	sleep(sleepTime);
	cout << "terminate" << endl;
	// When main() terminates, any created child threads automatically terminate
	return 0;

}
