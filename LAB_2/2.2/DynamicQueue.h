#include <iostream>
#include <thread>
#include <queue>
using namespace std;


class DynamicQueue {
private:
	std::queue<uint8_t> data;
	std::mutex qmutex;

public:

	void push(uint8_t val) {
		std::lock_guard<std::mutex> lock(qmutex);
		data.push(val);
	}
	bool pop(uint8_t& val) {
		unique_lock<mutex> ulock(qmutex);
		if (data.empty()) {
			ulock.unlock();
			this_thread::sleep_for(milliseconds(1));
			ulock.lock();
			if (data.empty()) {
				return false;
			}
		}
		val = data.front();
		data.pop();
		return true;
	}

};
