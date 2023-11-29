#pragma once
#include <vector>
#include <string>
#include <mutex>
class safeVector
{
public:
	safeVector() {};
	void clear();
	void push_back(std::string str);
	std::string& operator[] (int i);
	int size();
private:
	std::mutex mutexMes;
	std::vector<std::string> mes;
};

