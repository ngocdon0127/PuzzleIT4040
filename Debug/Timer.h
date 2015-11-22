/*
* Main author(s) : Quang Dung PHAM
*
* email: quang.pham@uclouvain.be
*
* Copyright 2011  Quang Dung PHAM
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/

#ifndef TIMER_H
#define TIMER_H

using namespace std;
#include <sys/time.h>

class Timer{
public:
	timeval	_start_t;
	
	Timer();
	~Timer();
	
	double getElapsedTime();	
};

#endif