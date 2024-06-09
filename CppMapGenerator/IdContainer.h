#pragma once
#include <iostream>
#include <map>

template <typename T>
class IdContainer
{
private:
	std::map<int, T> map;

public:
	//IdContainer()
	//{
	//	addEntry(0, T());
	//}

	void addEntry(int ID, const T& type) { map.emplace(ID, type); } // should start at 1 - 0 is reserved
	T& getEntry(int ID) { return map.at(ID); }
	int getEntryCount() { return map.size(); }
	void removeEntry(int ID) { map.erase(ID); }
	auto getIterator() { return map.begin(); }
	auto getEnd() { return map.end(); } // i have no idea what the type is
	bool containsID(int ID) { return map.count(ID); }
};

