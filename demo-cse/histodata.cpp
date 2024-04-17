#include <algorithm>
#include "histodata.h"

static inline
int clamp(const int& value, int min=0, int max=0xff){
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

static inline
void clamp(int& value, int min=0, int max=0xff){
	if (value < min) value = min;
	if (value > max) value = max;
}

HistoData::HistoData():
	data{255,0,255,0,255,0}
	{}

int HistoData::getMinB(){ return data[0]; }
int HistoData::getMaxB(){ return data[1]; }
int HistoData::getMinG(){ return data[2]; }
int HistoData::getMaxG(){ return data[3]; }
int HistoData::getMinR(){ return data[4]; }
int HistoData::getMaxR(){ return data[5]; }
int HistoData::getMax(const int& channel){
	switch(channel){
		case 0: return data[1];
		case 1: return data[3];
		case 2: return data[5];
		default: return -1;
	}
}
int HistoData::getMin(const int& channel){
	switch(channel){
		case 0: return data[0];
		case 1: return data[2];
		case 2: return data[4];
		default: return -1;
	}
}

void HistoData::setMinB(int value){
	clamp(value);
	data[0] = std::min(value, data[0]);
}

void HistoData::setMaxB(int value){
	clamp(value);
	data[1] = std::max(value, data[1]);
}

void HistoData::setMinG(int value){
	clamp(value);
	data[2] = std::min(value, data[2]);
}

void HistoData::setMaxG(int value){
	clamp(value);
	data[3] = std::max(value, data[3]);
}

void HistoData::setMinR(int value){
	clamp(value);
	data[4] = std::min(value, data[4]);
}

void HistoData::setMaxR(int value){
	clamp(value);
	data[5] = std::max(value, data[5]);
}

void HistoData::setMax(const int& channel, int value){
	clamp(value);
	switch(channel){
		case 0: data[1] = value; break;
		case 1: data[3] = value; break;
		case 2: data[5] = value; break;
	}
}

void HistoData::setMin(const int& channel, int value){
	clamp(value);
	switch(channel){
		case 0: data[0] = value; break;
		case 1: data[2] = value; break;
		case 2: data[4] = value; break;
	}
}

void HistoData::mergeWith(const HistoData& other){
	data[0] = std::min(data[0], other.data[0]);
	data[2] = std::min(data[2], other.data[2]);
	data[4] = std::min(data[4], other.data[4]);

	data[1] = std::max(data[1], other.data[1]);
	data[3] = std::max(data[3], other.data[3]);
	data[5] = std::max(data[5], other.data[5]);
}
