/*
  RF_Sniffer
  
  Hacked from http://code.google.com/p/rc-switch/
  
  by @justy to provide a handy RF code sniffer
*/

// Overwrite this file with the original one

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

#define DTTMFMT "%Y-%m-%d %H:%M:%S " // I got this function from http://stackoverflow.com/questions/2770555/how-do-i-save-time-to-a-file
#define DTTMSZ 21
static char *getDtTm (char *buff) {
    time_t t = time (0);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}

//void printftime();      
     
RCSwitch mySwitch;
 


int main(int argc, char *argv[]) {

     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;
     
     if(wiringPiSetup() == -1)
       return 0;

     mySwitch = RCSwitch();
     mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2
     
  
       while(1) {
  
      if (mySwitch.available()) {
    
        int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          //printf("Unknown encoding\n"); // Commented this
        } else {    

          if (value == 12345) { // Modify this value, as in the .ino
           char buff[DTTMSZ];
           fstream filestr;
           filestr.open ("log.txt", fstream::out|fstream::app);
           filestr << getDtTm (buff) << "Movement detected" << std::endl;
           filestr.close();

           system("./Raspi_PIR.sh"); 
           sleep(10); 
       } }

        fflush(stdout); // force immediate output. Got this from http://stackoverflow.com/questions/30831263/how-to-redirect-standard-output-to-a-file-whats-wrong-with-this-code
        mySwitch.resetAvailable();

      }
  }
  exit(0);
}
