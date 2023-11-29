#include "safeVector.h"

void safeVector::clear()
{
	std::vector <std::string>().swap(mes);
}

void safeVector::push_back(std::string str)
{
	std::lock_guard<std::mutex> lock(mutexMes);
	mes.push_back(str);
}

std::string& safeVector::operator[](int i)
{
	return mes[i];
}

int safeVector::size()
{
	return mes.size();
}
