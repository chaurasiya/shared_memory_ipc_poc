#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <cstdlib> // for exit function
// This program reads values from the file 'example.dat'
// and echoes them to the display until a negative value
// is read.
#include <vector>
#include <map>


typedef std::string SLTKeyType;
typedef std::string SLTValueType;
void readAndPrepareMap(std::map<SLTKeyType,SLTValueType> SLT_MAP, std::string file_name) {
	ifstream indata; // indata is like cin
   	std::string line; // variable for input value
    indata.open(file_name); // opens the file
   	if(!indata) { // file couldn't be opened
    	cerr << "Error: file could not be opened" << endl;
    	exit(1);
   	}
	indata >> line;
   	while ( !indata.eof() ) { // keep reading until end-of-file	
		std::string character = line.substr(0,line.find(","));
		std::string token = line.substr(line.find(",")+1);
		SLT_MAP.insert(std::make_pair(character,token));
    	indata >> line; // sets EOF flag if no value found
   	}
   	indata.close();
   	cout << "End-of-file reached.." << endl;
}

int main()
{
   	std::map<SLTKeyType,SLTValueType> SLT1_MAP;
	std::map<SLTKeyType,SLTValueType> SLT2_MAP;
	readAndPrepareMap(SLT1_MAP,"SLT_1.csv");
	readAndPrepareMap(SLT2_MAP,"SLT_2.csv");
   return 0;
}
