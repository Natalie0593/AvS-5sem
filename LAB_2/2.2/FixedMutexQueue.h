#include <iostream>
#include <vector>
#include <array>
#include <mutex>
#include <condition_variable>

class LimitMutexQueue {
private:
	std::vector<uint8_t> array;
	std::mutex mutex;
	std::condition_variable condition_variable_push, condition_variable_pop;
	size_t head, tail, size;

public:
	LimitMutexQueue(size_t size) : head(0), tail(0), size(0) {
		array.resize(size);
	}
	void push(uint8_t value);
	bool pop(uint8_t& value);
};

void LimitMutexQueue::push(uint8_t value) {
	std::unique_lock<std::mutex> lock(mutex);
	condition_variable_push.wait(lock, [&] { return size < array.size(); });
	array[tail % array.size()] = value;
	tail++;
	size++;
	lock.unlock();
	condition_variable_pop.notify_one();
}

bool LimitMutexQueue::pop(uint8_t& value) {
	std::unique_lock<std::mutex> lock(mutex);
	if (condition_variable_pop.wait_for(lock, std::chrono::milliseconds(1), [&] {return size > 0; })) {
		value = array[head % array.size()];
		head++;
		size--;
		lock.unlock();
		condition_variable_push.notify_one();
		return true;
	}
	else {
		return false;
	}
}
