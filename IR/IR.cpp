#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <string>
#include <cerrno>
#include <sstream>
#include <regex>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>

#include <phpcpp.h>

using namespace std;

class IR : public {
    private:
     static constexpr const char* dev = "/dev/ttyUSBir";

    public:
     static string send(string code, int times){

        if(times > 5){
            times -= times/3;
        }

        //open serial connection to IR-Device
        int fd = serialOpen(IR::dev, 9600);

        if(fd == -1){
            return "{'success':'false','err':'Can not open Serial Connection to IR-Device'}";
        }

        delay(200);

        //reset the IR-Device
        serialPrintf(fd,":~:");
        delay(200);

        //convert given code to string


        //send code to IR-Device
        serialPrintf(fd,("p"+code+"]:").c_str());
        delay(300);

        for( int i = 1; 99 * i <= times; i++){
            std::cout << "w99:" << endl;

            //send amount of repetitions of the code to the IR-Device
            serialPrintf(fd, "w99:");
            delay(1000+300*99);
        }

        times %= 99;

        string timesStr = to_string(times);

        if(timesStr.length()==1){
            timesStr = "0"+timesStr;
        }

        std::cout << ("w"+timesStr+":").c_str() << endl;

        //send amount of repetitions of the code to the IR-Device
        serialPrintf(fd, ("w"+timesStr+":").c_str());
        delay(1000+300*times);

        return "{'success':'true','err':''}";
     }

     static string read(string[] &params){

        string mode = params[0];

        if(mode.length()==1){
            mode = "0"+mode;
        }

        int fd = serialOpen(IR::dev, 9600);

        if(fd == -1){
            return "{'success':'false','err':'Can not open Serial Connection to IR-Device'}";
        }

        serialPrintf(fd,":~:");
        delay(20);

        cout << "Please hold the button.." << flush;

        serialPrintf(fd,("l"+mode+":").c_str());

        for(int i = 200; i < 1500; i += 200){
            delay(i);
            cout << "." << flush;

        }

        cout << endl << "Processing..." << endl;
        delay(200);

        cout << endl << flush;

        string read = "";

        while (serialDataAvail (fd))
        {
            read += serialGetchar (fd);
        }

        regex e ("[^A-Za-z0-9\\\\_]+");
        string erg;

        regex_replace (std::back_inserter(erg), read.begin(), read.end(), e, "");

        return erg;

     }

     static string getMode(){

        //open serial connection to IR-Device
        int fd = serialOpen(IR::dev, 9600);

        if(fd == -1){
            return "{'success':'false','err':'Can not open Serial Connection to IR-Device'}";
        }

        //reset the IR-Device
        serialPrintf(fd,":~:");
        delay(20);

        //send code to IR-Device
        serialPrintf(fd,"v:");
        delay(300);

        string read = "";

        while (serialDataAvail (fd))
        {
            read += serialGetchar (fd);
        }

        regex e ("[^A-Za-z0-9\\\\_]+");
        string erg;

        regex_replace (std::back_inserter(erg), read.begin(), read.end(), e, "");

        return erg;
     }

     static string check(){

        int fd = serialOpen(IR::dev, 9600);

        if(fd == -1){
            return "0";
        }

        //reset the IR-Device
        serialPrintf(fd,":~:");
        delay(20);

        //send code to IR-Device
        serialPrintf(fd,"k:");
        delay(300);

        string read = "";

        while (serialDataAvail (fd))
        {
            read += serialGetchar (fd);
        }

        regex e ("[^A-Za-z0-9\\\\_]+");
        string erg;

        regex_replace (std::back_inserter(erg), read.begin(), read.end(), e, "");

        return to_string(erg.length());

     }
};

int main(int, char *[]){

    IR::send("21fe42425758160a1620160a1620162016201620160a1620160a1620160a160a160a160a1620160a1620160a16201620160a160a160a1620160a1620160a160a16201620162016",5);
    IR::send("21fef3f55758160a1620160a1620162016201620160a1620160a1620160a160a160a160a1620160a1620160a16201620160a160a160a1620160a1620160a160a16201620162016",5);
}