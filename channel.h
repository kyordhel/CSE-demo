#pragma once
#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <mutex>
#include <utility>
#include <iostream>
#include <condition_variable>

#include <opencv2/core.hpp>

#include "syncq.h"

template<typename T>
class Channel{
public:
	Channel<T>(){
		ix = ++Channel::ccount;
	}

	Channel<T>(const Channel<T>& other) = delete;
	Channel<T>(Channel<T>&& other){
		std::swap(*this, other);
		std::cout << "Moved channel " << ix << " (ctor)" << std::endl;
	};

public:
	void fetchOne(T& value){
		value = q1.dequeue();
	}

	void sendOne(const T& value){
		q1.enqueue(value);
	}

	void fetchTwo(T& value){
		value = q2.dequeue();
	}

	void sendTwo(const T& value){
		q2.enqueue(value);
	}



public:
	Channel<T>& operator= (Channel&& other){
		Channel<T> tmp; // Forces destruction of other at the end of method
		std::swap(tmp, other); // it is more memory efficient.
		std::swap(*this, tmp);
		std::cout << "Moved channel " << ix << " (op=)" << std::endl;
		return *this;
	}

/*
	Channel<T>& operator= (const Channel&& other){
		std::swap(*this, other);
	}
*/

private:
	SyncQ<T> q1, q2;

	int ix = 0;
	static int ccount;
};

template<typename T>
int Channel<T>::ccount = 0;

#endif // __CHANNEL_H__


