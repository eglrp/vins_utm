#include "HttpReq.h"
#include <iostream>
//#include <string.h>
#include <sstream>

#define MAX_BUFF_LENGTH 10

using namespace std;

int main(int argc, char** argv){

    HttpRequest* Http;
//    char http_msg[4096] = {0};
    char http_msg[] = "http://ptsv2.com/t/yutoutaro/post";

    char data[4906] = {0};
    stringstream msgBuff;
    int lastPostStamp = -100;
    for (int i=0; i<=100; i++){
        msgBuff << "(" <<  i  << ", " << i/10.0 << ")\n";
        if ((i-lastPostStamp) >= MAX_BUFF_LENGTH){
            // TODO post
            strcpy(data, msgBuff.str().c_str());
            char* response = (char*) malloc(BUFSIZE);
            if (Http->HttpPost(http_msg, data, response)) {
                cout << ">>>>>>>>>>>>>>>response>>>>>>>>>>>>>>>\n" << response
                     << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
            }else {
                cout << "No response.\r\n";
            }
            msgBuff.str(""); // clear buffer
            lastPostStamp = i; // reset counter
        }

    }









    return 0;
}