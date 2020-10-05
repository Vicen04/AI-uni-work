/*
 * Timer.h
 *
 *  Created on: Nov 20, 2018
 *      Author: Connor
 */
#include <chrono>
#include <ctime>
#include <cmath>
#include <string>		// String object
#include <vector>		// Vector object
#ifndef TIMER_H_
#define TIMER_H_

class Timer {
public:
	Timer();
	void start();
	void stop();
	double elapsedMilliseconds();
	double elapsedSeconds();
	double elapsedTicks();
	bool isRunning();
	bool newTick();

private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	bool m_bRunning;
	std::vector<std::chrono::time_point<std::chrono::system_clock>> starts;
	std::vector<std::chrono::time_point<std::chrono::system_clock>> ends;
	double lastTick;
};

#endif /* TIMER_H_ */
