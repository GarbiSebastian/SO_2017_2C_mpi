
#include <string>

std::string timestamp(){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%F %X", tm);
    return timestamp;
}