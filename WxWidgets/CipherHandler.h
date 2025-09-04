#pragma once
#ifndef CipherHandler_h
#define CipherHandler_h
#include <stack>
#include <string>
#include <tchar.h>
#include <unordered_map>
#include <vector>

class CipherHandler {
public:
	std::string affineEncode(const std::string& str);
	std::string affineDecode(const std::string& encStr);
private:
	int modInverse(int a, int m);	

	// Private affine code values
	int a = 7;
	int b = 23;
	int m = 95;
};

#endif
