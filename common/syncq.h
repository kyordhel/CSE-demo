#pragma once
#ifndef __SYNCQ_H__
#define __SYNCQ_H__

#include <mutex>
#include <queue>
#include <condition_variable>


template<typename T>
class SyncQ{
public:
	SyncQ<T>(const int& capacity=-1): mutex(), capacity(capacity){}

	SyncQ<T>(const SyncQ<T>& other) = delete;
	SyncQ<T>(SyncQ<T>&& other): mutex(){
		std::swap(*this, other);
	};

public:

	void enqueue(const T& value){
		std::unique_lock<std::mutex> lock(mutex);

		while( (capacity > 0) && (q.size() >= capacity) )
			cvHasSpace.wait(lock, [this]{ return this->q.size() >= this->capacity; });
		q.push(value);
		lock.unlock();
		cvHasItems.notify_one();
	}

	T dequeue(){
		std::unique_lock<std::mutex> lock(mutex);

		while( q.empty() )
			cvHasItems.wait(lock, [this]{ return !this->q.empty(); });
		T value = q.front();
		q.pop();
		lock.unlock();
		cvHasSpace.notify_one();
		return value;
	}


public:
	SyncQ<T>& operator= (SyncQ&& other){
		SyncQ<T> tmp; // Forces destruction of other at the end of method
		std::swap(tmp, other); // it is more memory efficient.
		std::swap(*this, tmp);
		return *this;
	}

private:
	const int capacity;
	std::queue<T> q;
	std::mutex mutex;
	std::condition_variable cvHasSpace;
	std::condition_variable cvHasItems;

};


#endif // __SYNCQ_H__


