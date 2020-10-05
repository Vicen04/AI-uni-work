/*
 * Timer.cpp
 *
 *  Created on: Nov 8, 2018
 *      Author: Connor
 */
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <string>		// String object
#include <vector>		// Vector object
#include "Timer.h"

Timer::Timer() {
	m_StartTime= {};
	m_bRunning = false;
	m_EndTime = {};
	lastTick = -1;
}
void Timer::start() {
	this->m_StartTime = std::chrono::system_clock::now();
	this->m_bRunning = true;
	this->starts.push_back(m_StartTime);
}
void Timer::stop() {
	this->m_EndTime = std::chrono::system_clock::now();
	this->m_bRunning = false;
	this->ends.push_back(m_EndTime);
}
double Timer::elapsedMilliseconds() {
	double elapsed = 0;
	std::chrono::time_point<std::chrono::system_clock> endTime;
	if (this->m_bRunning) {
		endTime = std::chrono::system_clock::now();
		elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(
				endTime - this->m_StartTime).count();
	}
	for (int i = 0, end_size = ends.size(); i < end_size; i++) {
		elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(
				this->ends[i] - this->starts[i]).count();
	}
	return elapsed;
}
double Timer::elapsedSeconds() {
	return elapsedMilliseconds() / 1000.0;
}
double Timer::elapsedTicks() {
	return floor(elapsedMilliseconds() / 10);
}
bool Timer::isRunning() {
	return this->m_bRunning;
}
bool Timer::newTick(){
	if(elapsedTicks() > lastTick){
		this->lastTick = elapsedTicks();
		return true;
	}
	return false;
}

