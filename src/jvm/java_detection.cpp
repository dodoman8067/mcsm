#include "java_detection.h"

bool getJava(){
    int code = std::system("java -version");
    return code == 0;
}