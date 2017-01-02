//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_MULTITHREADINGVECTOR_H
#define TINTORRENT_MULTITHREADINGVECTOR_H

#include <vector>
#include <mutex>

template<typename T>
class MultithreadingVector {
	std::vector<T> vec;
	std::mutex mutex;
public:
	void lock(){
		mutex.lock();
	}

	void unlock(){
		mutex.unlock();
	}

	void addElement( T& elem){
		mutex.lock();
		vec.push_back(elem);
		mutex.unlock();
	}

	std::vector<T> &getVec(){
		return vec;
	}
};


#endif //TINTORRENT_MULTITHREADINGVECTOR_H
