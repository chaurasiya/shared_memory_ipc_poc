#include <functional>
#include <cstdio>
#include <iostream>
#include <utility>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include "JNIProvider.h"
#include <atomic>
#include "src/cpp/shared/shm_generic_def.h"
using namespace boost::interprocess;
std::atomic<bool> shMemInitialized(false);

typedef int KeyType;
typedef float MappedType;
typedef std::pair<const int, float> ValueType;
typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;
typedef map<KeyType,MappedType,std::less<KeyType>, ShmemAllocator> ShmMap;

offset_ptr<ShmMap> sharedMemoryMap;
offset_ptr<Shared::Map>	SLT1_MAP;
offset_ptr<Shared::Map>	SLT2_MAP;

/*
 * Class:     JNIProvider
 * Method:    initConfig
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_JNIProvider_initConfig
  (JNIEnv *env, jobject thisObject) {
	if(!shMemInitialized.load()) {
	    try {
	    	printf("Opening a memory segment for shared memory...\n");
			//Shared memory front-end that is able to construct objects
			//associated with a c-string.Erase previous shared memory with the name
			//to be used and create the memory segment at the specified address and intialize resources
			Shared::Segment seg(boost::interprocess::open_only, "MySharedMemory");
			Shared::Allocator alloc(seg.get_segment_manager());

			printf("accessing a shared memory map\n");
			SLT1_MAP = seg.find<Shared::Map>("SLT1_MAP").first;
			SLT2_MAP = seg.find<Shared::Map>("SLT2_MAP").first;
			
			//access the map in SHM through the offset ptr                                                         
            Shared::Map::iterator iter;
			iter=SLT1_MAP->begin();
            for(; iter!=SLT1_MAP->end();iter++)
            {
                   std::cout<<"\n "<<iter->first<<" "<<iter->second;
            }
		} catch(std::exception &e){
			printf("error occcured while fetching the shared memory= %s", e.what());
			exit(-1);
		}
		//Now shared memory is intialized, we need to set the atomic variable to true
		shMemInitialized = true;
	}
	return;	
  }

void testMethod() {
	try {
	    	printf("Opening a memory segment for shared memory...\n");
			//Shared memory front-end that is able to construct objects
			//associated with a c-string.Erase previous shared memory with the name
			//to be used and create the memory segment at the specified address and intialize resources
			Shared::Segment seg(boost::interprocess::open_only, "MySharedMemory");
			Shared::Allocator alloc(seg.get_segment_manager());

			printf("accessing a shared memory map\n");
			offset_ptr<Shared::Map>	SLT1_MAP = seg.find<Shared::Map>("SLT1_MAP").first;
			offset_ptr<Shared::Map>	SLT2_MAP = seg.find<Shared::Map>("SLT2_MAP").first;
			
			//access the map in SHM through the offset ptr                                                         
            Shared::Map::iterator iter;
			iter=SLT1_MAP->begin();
            for(; iter!=SLT1_MAP->end();iter++)
            {
                   std::cout<<"\n "<<iter->first<<" "<<iter->second;
            }
		} catch(std::exception &e){
			printf("error occcured while fetching the shared memory= %s", e.what());
			exit(-1);
		}
		
}
  int main(int argc, char const *argv[])
{
	/* code */
	//testMethod();
	return 0;
}

/* Class:     JNIProvider
 * Method:    generateToken
 * Signature: ([Ljava/lang/String;[Ljava/lang/String;)I
 *   
 */
 JNIEXPORT jint JNICALL Java_JNIProvider_generateToken
   (JNIEnv *env, jobject thisObject, jobjectArray dataStringArray, jobjectArray tokenStringArray){
	if(sharedMemoryMap==NULL) {
		printf(" error: tokenizer has been called without initializing the configs...");
		return 0;
	}
	int datasize = env->GetArrayLength(dataStringArray);
	int tokenSize = env->GetArrayLength(tokenStringArray);
	if(datasize!=tokenSize){
	  	printf("Both the datastring array and token string array should be of same size\n");
		return 0;
	}
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

/*
 * Class:     JNIProvider
 * Method:    generateToken
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_JNIProvider_generateToken__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *env, jobject thisObject, jstring data, jstring sessionKey) {
	  if(SLT1_MAP==NULL || SLT2_MAP==NULL) {
		printf(" error: tokenizer has been called without initializing the configs...");
		return NULL;
	}
	if (!data) { 
        return env->NewStringUTF("");
	}

    const jclass stringClass = env->GetObjectClass(data);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(data, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string stringData = std::string((char *)pBytes, length);
	std::string token="";
	if(stringData.length()==1) {
		
		env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);
    	env->DeleteLocalRef(stringJbytes);
    	env->DeleteLocalRef(stringClass);
		auto it1 = SLT1_MAP->find(stringData.c_str());
		auto firstToken = it1->second;
		auto it2 = SLT2_MAP->find(firstToken);
		return env->NewStringUTF(it2->second.c_str());
	}
	for(int i=1;i<stringData.length();i++) {
		std::string tokenToSearch=std::string(1,stringData[i-1])+stringData[i];
		auto it1 = SLT1_MAP->find(tokenToSearch.c_str());
		auto firstToken = it1->second;
		auto it2 = SLT2_MAP->find(firstToken);
		token+=it2->second[0];
	}
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);

	return env->NewStringUTF(token.c_str());
	//return ret;
  }


