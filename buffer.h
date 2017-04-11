/**
 * Shared buffer definition
 */
#include <string>
using namespace std;

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
		int insertItem(bufferItem item);
		// Remove an item from the buffer and store it into the given variable
		int removeItem(bufferItem&);
		// Return a human-readable string representing the current state of the
		// buffer; this method is named str() for the sake of consistency with
		// the STL stringstream method of the same name
		string str();



};
