## Build & Run
```
export BASE_PROJECT_LOCATION=${BASE_PROJECT_LOCATION:-"/home/abhi/Desktop/shared_memory_ipc_poc"}
cd ${BASE_PROJECT_LOCATION}/src/cpp/


####NEW COMMAND START####
g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sm_access.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -o sm_access.o -lpthread -lrt -lstdc++

g++ -shared -fPIC -o libsm_access.so sm_access.o -lc -lrt -lstdc++

java -cp . -Djava.library.path=/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/jni JNIProvider 

####END####


g++ -I ../../../plugins/boost_1_75_0/ ./sm_init_enhanced.cpp -lpthread -lrt

g++ -I ../../../plugins/boost_1_75_0/ -I /home/abhi/Desktop/shared_memory_ipc_poc/ ./sm_init_enhanced.cpp -lpthread -lrt

g++ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -I ../../../../plugins/boost_1_75_0/ -I /home/abhi/Desktop/shared_memory_ipc_poc/ ./sm_access_enhanced.cpp -lpthread -lrt

g++ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sm_access.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -lpthread -lrt 




#######Enhanced Library Code###########

g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sm_access_enhanced.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -I /home/abhi/Desktop/shared_memory_ipc_poc/ -o sm_access.o -lpthread -lrt -lstdc++

g++ -shared -fPIC -o libsm_access.so sm_access.o -lc -lrt -lstdc++

java -cp . -Djava.library.path=/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/jni JNIProvider 



```