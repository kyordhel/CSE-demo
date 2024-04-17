#pragma once
#ifndef __HISTODATA_H__
#define __HISTODATA_H__

class HistoData{
public:
	HistoData();

public:
	int getMinB();
	int getMaxB();
	int getMinG();
	int getMaxG();
	int getMinR();
	int getMaxR();
	int getMax(const int& channel);
	int getMin(const int& channel);

	void setMinB(int value);
	void setMaxB(int value);
	void setMinG(int value);
	void setMaxG(int value);
	void setMinR(int value);
	void setMaxR(int value);
	void setMax(const int& channel, int value);
	void setMin(const int& channel, int value);

	void mergeWith(const HistoData& other);

private:
	int data[6];

};

#endif // __HISTODATA_H__
