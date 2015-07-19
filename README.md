# Raspi_PIR_Push_notification
A PIR with Push Notifications using Raspberry Pi & full log via web

Steps: 
1- Install wiringPi https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/
2- Get 433utils:  https://github.com/ninjablocks/433Utils
3- sudo apt-get update && sudo apt-get install urlwatch lighttpd
4- nano ~/.urlwatch/urls.txt -> http://localhost/log.txt
3- Wire everything up (See tutorials in 433 utils/wiringPi)
4- Sign in http://pushetta.com/ , create a channel, go to dashboard and get the API key.  Install the app on your compatible device ( See list in http://pushetta.com/pushetta-downloads/ ). Subscribe to the channel you just created. 
5- Download this git, move RFSniffer.cpp to ???/433Utils/RPi_utils/. Modify as needed and compile it. 
6- Modify and upload the .ino to the ATMEGA328 following this tutorial: https://www.arduino.cc/en/Tutorial/ArduinoToBreadboard . 
7. Modify the .py file with the API KEY and the channel name. My channel is "Stairs PIR". 
8. Move all the files to /var/www/ and write in the console ./RFSniffer

If I haven't writed something wrong and you have done everything correctly, as you wave the sensor, you should hear a bell on your device! Let me know if I failed some where. 


As homework, move all but log.txt out of /var/www/, so it isn't visible to everyone. You could also password protect log.txt

My ATMEGA328P-PU consumption keeps at 90uA most of the time, so a battery should last a long time. The PIR sensor I picked eats by himself 50uA. 
