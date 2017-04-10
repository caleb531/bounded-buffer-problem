/**
 * Shared buffer definition
 */

// Define a shared buffer accessible by all functions and threads
typedef int bufferItem;

// The maximum number of items allowed in the buffer
static const int BUFFER_MAX_SIZE = 5;

class Buffer {

	protected:

		bufferItem buffer[BUFFER_MAX_SIZE];
		int front;
		int rear;
		int count;

	public:

		// Initialize shared buffer
		Buffer();
		// Insert the given item into the buffer
		void insert(bufferItem item);
		// Remove an item from the buffer and return it
		bufferItem* remove();
		// Print the entire buffer to stdout
		void print();



};
