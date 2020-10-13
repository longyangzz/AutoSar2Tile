#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
//∂®“ÂCSV∂¡¿‡
class CSVreader
{
public:
	CSVreader(const char *);
	~CSVreader();
	int readline();
	double data[10];
private:
	ifstream _csvInput;
};
