#pragma once
#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <mutex>
#include <utility>
#include <iostream>
#include <condition_variable>

#include <opencv2/core.hpp>


template<typename T>
class Channel{
public:
	Channel<T>(): mutex(), hasItem(false){
		ix = ++Channel::ccount;
		cvWrite.notify_one();
	}

	Channel<T>(const Channel<T>& other) = delete;
	Channel<T>(Channel<T>&& other): mutex(){
		std::swap(*this, other);
		std::cout << "Moved channel " << ix << " (ctor)" << std::endl;
	};

public:
	void fetch(T& value){
		std::unique_lock<std::mutex> lock(mutex);
		cvRead.wait(lock, [this]{ return this->hasItem; });
		value = this->value;
		hasItem = false;
		lock.unlock();
		cvWrite.notify_one();
		// printf("Channel %d is empty\n", ix);
	}

	void send(const T& value){
		std::unique_lock<std::mutex> lock(mutex);
		cvWrite.wait(lock, [this]{ return !this->hasItem; });
		this->value = value;
		hasItem = true;
		lock.unlock();
		cvRead.notify_one();
		// printf("Channel %d has 1 element\n", ix);
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
	T value;
	std::mutex mutex;
	std::condition_variable cvRead;
	std::condition_variable cvWrite;
	int ix = 0;
	bool hasItem;
	static int ccount;
};

template<typename T>
int Channel<T>::ccount = 0;

#endif // __CHANNEL_H__


