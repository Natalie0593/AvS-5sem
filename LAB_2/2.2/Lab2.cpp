#include "task2.h"

using namespace std;


int main()
{
	thread consumers[ConsumerNum];
	thread producers[ProducerNum];

	for (int i = 0; i < ConsumerNum; i++)
	{
		consumers[i] = thread(consumer<DynamicQueue>, std::ref(dynamicQueue));
		producers[i] = thread(producer<DynamicQueue>, std::ref(dynamicQueue));
	}

	for (int i = 0; i < ConsumerNum; i++)
	{
		consumers[i].join();
		producers[i].join();
	}

	cout << "Sum: " << globalSum << endl;
	

	return 0;
}