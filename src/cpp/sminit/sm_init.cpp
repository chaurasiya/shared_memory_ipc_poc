#include <functional>
#include <cstdio>
#include <utility>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
int main() {
	using namespace boost::interprocess;
	try {
		shared_memory_object::remove("MySharedMemory");

		printf("Creating a memory segment for shared memory...\n");
		//Shared memory front-end that is able to construct objects
		//associated with a c-string.Erase previous shared memory with the name
		//to be used and create the memory segment at the specified address and intialize resources
		managed_shared_memory segment(create_only,
						"MySharedMemory",
						65536);
		typedef int KeyType;
		typedef float MappedType;
		typedef std::pair<const int, float> ValueType;
		printf("Creating an allocator which will allow to place containers in managed shared memory.\n");
		typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;
		printf("Creating an alias of map containe which will hold the data\n");
		typedef map<KeyType,MappedType,std::less<KeyType>, ShmemAllocator> ShmMap;
		printf("Initialize the shared memory STL compatible allocator\n");
		ShmemAllocator alloc_inst(segment.get_segment_manager());
		printf("constructing a shared memory map\n");
		ShmMap *myshmMap=segment.construct<ShmMap>("SLT_MAP")(std::less<int>(),alloc_inst);
		printf("Filling the map container with numbers...\n");
		for(int i=0;i<100;i++){
			myshmMap->insert(std::pair<const int, float>(i,(float)i));
		}

	} catch(std::exception &e){
		printf(" error %s\n", e.what());
	}
	return 0;
}

