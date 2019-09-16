#include "HttpReq.h"
#include <iostream>
#include <string.h>

void printUsage(){
    cout << "Usage:\r\n\ttestHttp GET [url]\ntestHttp POST [url] [message]\n";
    cout << "Example:\n./testHTTP POST http://54.169.63.192/utmapi/api/pilotui/GetPilotFlights [\\\"thura1\\\"]\n";
}

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "No mode is found from the imput.\n";
        printUsage();
        return 0;
    }
    bool isGet;
    if (strstr(argv[1],"GET")){
        cout << "GET mode enabled.\n";
        isGet = true;
    }else if (strstr(argv[1],"POST")){
        cout << "POST mode enabled.\n";
        isGet = false;
    }else{
        cerr << "No GET or POST is detected. Please check the usage.\n";
        printUsage();
        return 0;
    }

    if (argc < 3){
        cerr << "No URL is found.\n";
        printUsage();
        return 0;
    }



    HttpRequest* Http;
    char http_return[4096] = {0};
    char http_msg[4096] = {0};
//    strcpy(http_msg, "http://www.analysis-api.tm-uas.com/UtmApi/api/flights/telemetry/append/");
//    strcpy(http_msg, "http://www.wikipedia.org");
    strcpy(http_msg, argv[2]);

//    http://www.analysis-api.tm-uas.com/utmapi/api/pilotui/GetPilotFlights
    if (isGet) {
        if(Http->HttpGet(http_msg, http_return)){
            cout << ">>>>>>>>>>>>>>>HTTP_return>>>>>>>>>>>>>>>\n" << http_return << endl;
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n\r\n";
        } else {
            cout << "No return received.\r\n";
        }
    }

//    double x = 1.0, y = 2.0, z = 0.0;

//    cout << "http header template:\n++++++++++\n" << "POST /api/flights HTTP/1.1\r\nHost:flightrack-env.ap-southeast-1.elasticbeanstalk.com\r\nContent-Type: application/json\r\nCache-Control:no-cache\r\nContent-Length:79\r\n\r\n" << "++++++++++\n";
//    strcpy(data, "{\"longitude\":");
//    char tempD2c[3] = {0};
//    sprintf(tempD2c, "%1.1f", x); strcpy(data, tempD2c);
//    strcpy(data, ",\"latitude\":");
//    sprintf(tempD2c, "%1.1f", y); strcpy(data, tempD2c);
//    strcpy(data, ", \"altitude\":");
//    sprintf(tempD2c, "%1.1f", z); strcpy(data, tempD2c);
//    strcpy(data, ", \"flightmode\": ");
//    strcpy(data, "NIL");
//    strcpy(data, "\"}");
    if (!isGet){
        char data[4096] ={0};
        if (argc < 4){
            cout << "No message to found, default message is used.\r\n";
            strcpy(data, "[\"yutoutaro\"]");
        } else{
//            strcpy(data, "[\"thura1\"]"); //
            strcpy(data, argv[3]);
        }
        cout << "<<<<<<<<<<Message<<<<<<<<<<\n" << data << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

        char* response = (char*) malloc(BUFSIZE);
        if (Http->HttpPost(http_msg, data, response)) {
            cout << ">>>>>>>>>>>>>>>response>>>>>>>>>>>>>>>\n" << response
                 << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        }else {
            cout << "No response.\r\n";
        }
    }
    return 0;
}