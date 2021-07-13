#include <functional>
#include <cstdio>
#include <iostream>
#include <utility>
#include "JNIProvider.h"
#include <atomic>
#include "src/cpp/shared/shm_generic_def.h"
using namespace boost::interprocess;


void testMethod() {
	try {
	    	Shared::Segment seg(boost::interprocess::open_only, "MySharedMemory");
			
			printf("accessing a shared memory map\n");
			offset_ptr<Shared::Map> SLT1_MAP = seg.find<Shared::Map>("SLT1_MAP").first;
			offset_ptr<Shared::Map> SLT2_MAP = seg.find<Shared::Map>("SLT2_MAP").first;
			//access the map in SHM through the offset ptr                                                         
            Shared::Map::iterator iter;
			iter=SLT1_MAP->begin();
            for(; iter!=SLT1_MAP->end();iter++)
            {
                   std::cout<<"\n "<<iter->first<<" "<<iter->second;
            }
			std::string stringData="ab";
			auto it1 = SLT1_MAP->find(stringData.c_str());
			auto firstToken = it1->second;
			auto it2 = SLT2_MAP->find(firstToken);
			std::cout << "Original=" << stringData << ", Token=" <<it2->second << std::endl;

		} catch(std::exception &e){
			printf("error occcured while fetching the shared memory= %s", e.what());
			exit(-1);
		}
		
}

  int main(int argc, char const *argv[])
{
	/* code */
	testMethod();
	//testToken();
	return 0;
}

/*
 * Class:     JNIProvider
 * Method:    generateToken
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_JNIProvider_generateToken__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *env, jobject thisObject, jstring data, jstring sessionKey) {
	Shared::Segment seg(boost::interprocess::open_only, "MySharedMemory");
			
	printf("accessing a shared memory map\n");
	offset_ptr<Shared::Map> SLT1_MAP = seg.find<Shared::Map>("SLT1_MAP").first;
	offset_ptr<Shared::Map> SLT2_MAP = seg.find<Shared::Map>("SLT2_MAP").first;
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
	char prev;
	for(int i=1;i<stringData.length();i++) {
		std::string tokenToSearch;
		if(i==1){
			tokenToSearch=std::string(1,stringData[i-1])+stringData[i];
		} else {
			tokenToSearch=std::string(1,prev)+stringData[i];
		}
		auto it1 = SLT1_MAP->find(tokenToSearch.c_str());
		//std::cout << "TokenToSearch=" << tokenToSearch << ", FirstToken=" << it1->second << std::endl;
		auto firstToken = it1->second;
		auto it2 = SLT2_MAP->find(firstToken);
		//std::cout << "TokenToSearch=" << it1->second << ", SecondToken=" <<  it2->second << std::endl;
		if(i<stringData.length()-1) {
			token+=it2->second[0];
			prev=it2->second[1];
		} else {
			token+=it2->second.c_str();
		}
	}
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
	//delete stringData;
	return env->NewStringUTF(token.c_str());
	//return ret;
  }


/*
 * Class:     JNIProvider
 * Method:    getOriginal
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_JNIProvider_getOriginal
  (JNIEnv *env, jobject thisObject, jstring token, jstring sessionKey) {
	  Shared::Segment seg(boost::interprocess::open_only, "MySharedMemory");
			
	printf("accessing a shared memory map\n");
	offset_ptr<Shared::Map> SLT1_MAP = seg.find<Shared::Map>("SLT1_MAP").first;
	offset_ptr<Shared::Map> SLT2_MAP = seg.find<Shared::Map>("SLT2_MAP").first;
	  if(SLT1_MAP==NULL || SLT2_MAP==NULL) {
		printf(" error: tokenizer has been called without initializing the configs...");
		return NULL;
	}
	if (!token) { 
        return env->NewStringUTF("");
	}		

    const jclass stringClass = env->GetObjectClass(token);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(token, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string stringData = std::string((char *)pBytes, length);
	std::string original="";
	if(stringData.length()==1) {
		
		env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);
    	env->DeleteLocalRef(stringJbytes);
    	env->DeleteLocalRef(stringClass);
		auto it2 = SLT2_MAP->find(stringData.c_str());
		auto firstToken = it2->second;
		auto it1 = SLT1_MAP->find(firstToken);
		return env->NewStringUTF(it1->second.c_str());
	}
	char prev;
	for(int i=stringData.length()-1;i>0;i--) {
		std::string tokenToSearch;
		if(i==stringData.length()-1){
			tokenToSearch=std::string(1,stringData[i-1])+stringData[i];
		} else {
			tokenToSearch=stringData[i-1]+std::string(1,prev);
		}
		auto it2 = SLT2_MAP->find(tokenToSearch.c_str());
		std::cout << "TokenToSearch=" << tokenToSearch << ", FirstToken=" << it2->second << std::endl;
		auto firstToken = it2->second;
		auto it1 = SLT1_MAP->find(firstToken);
		std::cout << "TokenToSearch=" << firstToken << ", SecondToken=" <<  it1->second << std::endl;
		if(i>0) {
			original+=it1->second[1];
			prev=it2->second[0];
		} else {
			original+=it2->second.c_str();
		}
	}
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
	//delete stringData;
	return env->NewStringUTF(original.c_str());
  }

