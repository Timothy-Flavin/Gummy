#include"Timer.h"
#include<iostream>
Timer::Timer() {
m_StartTimepoint = std::chrono::high_resolution_clock::now();
}
Timer::~Timer(){
	Stop();
}
void Timer::Stop(){
	auto endTimePoint = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	std::cout << duration << "microseconds, " << ms << "milliseconds" << std::endl;
}