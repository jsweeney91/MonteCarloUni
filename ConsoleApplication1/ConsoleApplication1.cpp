// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <functional>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono;


using namespace std::this_thread;

void monte_carlo_pi(unsigned int iterations) {
	auto millis = duration_cast <milliseconds >(system_clock::now().time_since_epoch());
	default_random_engine e(millis.count());
	
	uniform_real_distribution<double> distribution(0.0, 1.0);
	unsigned int in_circle = 0;
	for(unsigned int i = 0;	i < iterations;++i) {
		auto x = distribution(e);
		auto y = distribution(e);
		
		auto length = sqrt((x * x) + (y * y));

		if(length <= 1.0)
			++in_circle;
	}
	auto pi = (4.0 * in_circle) /static_cast<double>(iterations);
}

int main()
{
	ofstream data("montecarlo.csv", ofstream::out);

	for (unsigned int num_threads = 0; num_threads <= 6; ++num_threads) {
		auto total_threads = static_cast<unsigned int>(pow(2.0, num_threads));
		cout << "Number of threads " << total_threads << endl;
		data << "num_threads_" << total_threads;
		for (unsigned int iters = 0; iters < 100; ++iters) {
			auto start = system_clock::now();
			vector<thread> threads;
			for (unsigned int n = 0; n < total_threads; ++n) {
				threads.push_back(thread(monte_carlo_pi, static_cast<unsigned int>(pow(2.0, 24.0 - num_threads))));
			}
			for (auto &t : threads)
				t.join();
			auto end = system_clock::now();
			auto total = end - start;
			data << ", " << duration_cast<milliseconds>(total).count();
		}
		data << endl;
	}
	data.close();
    return 0;
}

