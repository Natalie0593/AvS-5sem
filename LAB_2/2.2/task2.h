#include "DynamicQueue.h"
#include "FixedMutexQueue.h"

using namespace std;
using namespace std::chrono;

static const int TaskNum = 4 * 1024 * 1024;
static const int ProducerNum = 4;
static const int ConsumerNum = 4;

int globalSum = 0;
atomic<int> counter{ 0 };


static FixedSizeQueue staticQueue;

static DynamicQueue dynamicQueue;

template <class T>
void producer(T& queue) {
	for (int i = 0; i < TaskNum / ConsumerNum; i++) {
		queue.push(1);
	}
}

template <class T>
void consumer(T& queue) {
	int localSum = 0;
	uint8_t elem;
	int local_counter = 0;
	while (true) {
		local_counter = counter;
		if (local_counter >= TaskNum)
			break;
		if (queue.pop(elem)) {
			localSum += elem;
			local_counter = counter++;
		}
	}
	globalSum += localSum;
}

