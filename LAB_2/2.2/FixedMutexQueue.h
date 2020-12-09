#include <iostream>
#include <vector>
#include <array>
#include <mutex>
#include <condition_variable>
static const int QueueSize = 16;

class FixedSizeQueue {
private:
	std::queue<uint8_t> data;
	std::mutex qmutex;
	condition_variable condition_variable_push, condition_variable_pop;

public:

	void push(uint8_t val) {

		unique_lock<std::mutex> lock(qmutex);

		condition_variable_push.wait(lock, [this] {return !isFull(); });
		data.push(val);
		lock.unlock();
		condition_variable_pop.notify_all();
	}

	bool pop(uint8_t& val) {
		unique_lock<mutex> lock(qmutex);
		while (data.empty()) {
			condition_variable_pop.wait_for(lock, chrono::milliseconds(1));
			if (data.empty()) {
				lock.unlock();
				return false;
			}
		}
		val = data.front();
		data.pop();
		lock.unlock();
		condition_variable_push.notify_all();
		return true;
	}
	bool isFull() {
		return data.size() >= QueueSize;
	}

};
