#include <functional>
#include <cstdio>
#include <iostream>
#include <utility>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include "JNIProvider.h"
using namespace boost::interprocess;
typedef int KeyType;
typedef float MappedType;
typedef std::pair<const int, float> ValueType;
typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;
typedef map<KeyType,MappedType,std::less<KeyType>, ShmemAllocator> ShmMap;
int main() {
	using namespace boost::interprocess;
	try {
		printf("Opening a memory segment for shared memory...\n");
		//Shared memory front-end that is able to construct objects
		//associated with a c-string.Erase previous shared memory with the name
		//to be used and create the memory segment at the specified address and intialize resources
		managed_shared_memory segment(open_only,
						"MySharedMemory");
		typedef int KeyType;
		typedef float MappedType;
		typedef std::pair<const int, float> ValueType;
		printf("Creating an allocator which will allow to place containers in managed shared memory.\n");
		typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;
		printf("Creating an alias of map containe which will hold the data\n");
		typedef map<KeyType,MappedType,std::less<KeyType>, ShmemAllocator> ShmMap;
		printf("Initialize the shared memory STL compatible allocator\n");
		ShmemAllocator alloc_inst(segment.get_segment_manager());
		printf("accessing a shared memory map\n");
		offset_ptr<ShmMap> m_pmap=segment.find<ShmMap>("SLT_MAP").first;
		ShmMap::iterator itr = m_pmap->begin();
		for(;itr!=m_pmap->end();itr++){
			printf("Pair(%d, %f)\n",itr->first,itr->second);
		}

	} catch(std::exception &e){
		printf(" error %s\n", e.what());
		shared_memory_object::remove("MySharedMemory");
	}
	return 0;
}
bool accessedSHM=false;
offset_ptr<ShmMap> sharedMemoryMap;
void fetch_sharedmemory_map(){
	if(sharedMemoryMap!=NULL){
		return;
	}
	    try {
	    		printf("Opening a memory segment for shared memory...\n");
			//Shared memory front-end that is able to construct objects
			//associated with a c-string.Erase previous shared memory with the name
			//to be used and create the memory segment at the specified address and intialize resources
			managed_shared_memory segment(open_only,"MySharedMemory");
			ShmemAllocator alloc_inst(segment.get_segment_manager());
			printf("accessing a shared memory map\n");
			sharedMemoryMap=segment.find<ShmMap>("SLT_MAP").first;
		} catch(std::exception &e){
			printf(" error occcured while fetching the shared memory= %s", e.what());
		}
		return;
																							       
}

/* Class:     JNIProvider
 * Method:    generateToken
 * Signature: ([Ljava/lang/String;[Ljava/lang/String;)I
 *   
 */
 JNIEXPORT jint JNICALL Java_JNIProvider_generateToken
   (JNIEnv *env, jobject thisObject, jobjectArray dataStringArray, jobjectArray tokenStringArray){
	int datasize = env->GetArrayLength(dataStringArray);
	int tokenSize = env->GetArrayLength(tokenStringArray);
	if(datasize!=tokenSize){
	  	printf("Both the datastring array and token string array should be of same size\n");
		return 0;
	}
	fetch_sharedmemory_map();
	for (int i=0; i < datasize; ++i) 
	{
		    jstring stringJNI =(jstring)env->GetObjectArrayElement(dataStringArray, i);
		    const char *dataString = env->GetStringUTFChars(stringJNI, 0);
		    std::cout << "printing the input data=" << dataString << std::endl;
		    env->ReleaseStringUTFChars(stringJNI, dataString);
		    env->DeleteLocalRef(stringJNI);
	}
	return 1;
 }


