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

using namespace std;

#include "Timer.h"
#include <cstddef>

Timer::Timer(){
	gettimeofday(&_start_t,NULL);	
}

Timer::~Timer(){
}

double Timer::getElapsedTime(){
	timeval t;
	gettimeofday(&t,NULL);
	
	double	elapsedtime = (t.tv_sec - _start_t.tv_sec) * 1000.0;
	elapsedtime += (t.tv_usec - _start_t.tv_usec) / 1000.0;
	
	elapsedtime = elapsedtime*0.001;
	
	/*double	elapsedtime = (t.tv_sec - _start_t.tv_sec);
	elapsedtime += (t.tv_usec - _start_t.tv_usec) / 1000000.0;*/
	
	return elapsedtime;
}
