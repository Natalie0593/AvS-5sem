#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <mutex>
#include <vector>
#include <atomic>
#include <array>
#include "windows.h"

using namespace std;
using namespace std::chrono;

static const int NumThreads = 32;
static const int NumTasks = 4000 * 4000;

std::array<int, NumTasks> A;

int mutex_addr = 0;
atomic<int> atomic_addr{ 0 };
mutex _lock;

int funcMutex() {
	int index;
	lock_guard<mutex> lock(_lock);
	index = mutex_addr++;
	return index;
}

void OneAddr(int plan) {
	int local_ind = 0;
	while (true) {
		if (plan ==  1) {
			local_ind = atomic_addr++;
		}
		else if(plan == 2)
		{
			local_ind = funcMutex();
		}
		else if (plan == 3) {
			local_ind = atomic_addr++;
			this_thread::sleep_for(chrono::nanoseconds(10));
		}
		else {
			local_ind = funcMutex();
			this_thread::sleep_for(chrono::nanoseconds(10));
		}
		if (local_ind >= NumTasks) 
			break;
		A[local_ind]++;
	}
}


int main()
{
	int q;

		cout << "1 - amomic\n2 - mutex\n3 - atomic sleep\n4 - mutex sleep";
		cin >> q;
		for (int i = 0; i < NumTasks; i++)
			A[i] = 0;

		auto startTime = high_resolution_clock::now();
		thread mythread[NumThreads];

		for (int i = 0; i < NumThreads; i++)
		{
			mythread[i] = thread(OneAddr, q);
		}

		for (int i = 0; i < NumThreads; i++)
		{
			mythread[i].join();
		}

		auto stopTime = high_resolution_clock::now();
		auto elapsed = duration_cast<nanoseconds>(stopTime - startTime);


		cout << "Sec: " << elapsed.count() * 1e-9 << endl;
	

	return 0;
}