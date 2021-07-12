#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <cstdlib> 
#include "shm_generic_def.h"


void fillShareMemoryMapWithSLTs(Shared::Map *myshmMap, std::string file_name,const Shared::Allocator& alloc) {
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
		//SLT_MAP.insert(std::make_pair(character,token));
		myshmMap->emplace(Shared::String(character.c_str(), alloc),Shared::String(token.c_str(), alloc));
    	indata >> line; // sets EOF flag if no value found
   	}
   	indata.close();
   	cout << "End-of-file reached.." << endl;
}
int main() {
	
	try {
		boost::interprocess::shared_memory_object::remove("MySharedMemory");
		cout << "Creating a memory segment for shared memory..." << endl;
		// Create shared memory with 2MB initial memory
    	Shared::Segment seg(boost::interprocess::create_only, "MySharedMemory", 2097152);
    	Shared::Allocator alloc(seg.get_segment_manager());
		
		// An instance of the string comparator, to construct the map
    	Shared::StringComparator cmp;

    	// Construct the shared memory map
    	Shared::Map *SLT1_MAP = seg.construct<Shared::Map>("SLT1_MAP")(cmp, alloc);
        Shared::Map *SLT2_MAP = seg.construct<Shared::Map>("SLT2_MAP")(cmp, alloc);
		cout << "Filling the map container with numbers..." << endl;
		fillShareMemoryMapWithSLTs(SLT1_MAP,"/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/config/SLT_1.csv",alloc);
		fillShareMemoryMapWithSLTs(SLT2_MAP,"/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/config/SLT_2.csv",alloc);
        //std::string character="ab";
        //std::string token="ef";
        //SLT1_MAP->emplace(Shared::String(character.c_str(), alloc),Shared::String(token.c_str(), alloc));
        // this is now okay too
        std::string skey("gf");
        auto it = SLT1_MAP->find(skey.c_str());
        if (it!=SLT1_MAP->end())
            cout << "Found: '" << it->first << "' -> '" << it->second << "'\n";
	} catch(std::exception &e){
		cerr << "Error: " << e.what() << endl;
	}
	return 0;
}

