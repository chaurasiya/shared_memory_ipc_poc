## Build & Run
```
export BASE_PROJECT_LOCATION=${BASE_PROJECT_LOCATION:-"/home/abhi/Desktop/shared_memory_ipc_poc"}
cd ${BASE_PROJECT_LOCATION}/src/cpp/
g++ ./sminit/sm_init.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -lpthread -lrt -o sm_init.plm

g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sm_access.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -lpthread -lrt -o sm_access.o

####NEW COMMAND START####
g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sm_access.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -lpthread -lrt -o sm_access.o -lstdc++

g++ -shared -fPIC -o libsm_access.so sm_access.o -lc -lstdc++

java -cp . -Djava.library.path=/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/jni JNIProvider 

####END####




g++ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sm_access.cpp -I ${BASE_PROJECT_LOCATION}/plugins/boost_1_75_0/ -lpthread -lrt 
```