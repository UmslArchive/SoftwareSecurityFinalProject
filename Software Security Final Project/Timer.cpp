#include "Timer.h"

void Timer::start()
{
	startTime = high_resolution_clock::now();
}

void Timer::stop()
{
	endTime = high_resolution_clock::now();
}

void Timer::getElapsed(std::fstream &out)
{
	std::string temp;
	time_span = duration_cast<duration<double>>(endTime - startTime);
	temp =  "\nElapsed Time: " + std::to_string(time_span.count()) + " seconds\n";
	out << temp;
}
