#pragma once
#include <vector>
#include <functional>
#define GetKey(x) GetKeyState(x) & 0x80
typedef std::vector<int> VectorInt;
typedef std::vector<char> VectorChar;
typedef std::vector<std::string> VectorString;
class PrivateFuncs
{
public:
	static void ExecuteAfterWhenKey(int secs, std::function<void()> func, char c);
	static void ExecuteAfter(int ms, std::function<void()> func);
	int DistanceFrom(int x, int y);
};

