#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <fstream>

using namespace std::chrono;

class Timer
{
private:
	high_resolution_clock::time_point startTime;
	high_resolution_clock::time_point endTime;
	duration<double> time_span;

public:
	void start();
	void stop();
	void getElapsed(std::fstream &out);

};

