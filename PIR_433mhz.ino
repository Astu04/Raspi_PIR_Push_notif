#include <RCSwitch.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/power.h>
#include <RF12.h>
//#include <avr/wdtJWP.h>
#include <Ports.h>
#include <PinChangeInt.h>

#define PIR 8
#define tr433 7
#define transcode 12345 // Modify the value here! As in RFSniffer.cpp


RCSwitch mySwitch = RCSwitch();
ISR(WDT_vect) {
  Sleepy::watchdogEvent();
}

int previousADCSRA;


void setup() {

  pinMode(PIR, INPUT);
  // Transmitter is connected to Arduino Pin #10
  mySwitch.enableTransmit(tr433);

  PRR = bit(PRTIM1);                           // only keep timer 0 going
  ADCSRA &= ~ bit(ADEN); bitSet(PRR, PRADC);   // Disable the ADC to save power
  PCintPort::attachInterrupt(PIR, wakeup, CHANGE);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);

  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);


}


/*
ISR (WDT_vect) {
  wdt_disable();
} */

void startSleeping() { // Source: www.disk91.com/2014/technology/hardware/arduino-atmega328p-low-power-consumption/
  // clear various "reset" flags
  MCUSR = 0;
  // allow changes, disable reset, enable Watchdog interrupt
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interval (see datasheet p55)
  WDTCSR = bit (WDIE) | bit (WDP2) | bit (WDP1);    // 128K cycles = approximativly 1 second
  wdt_reset();  // start watchdog timer
  set_sleep_mode (SLEEP_MODE_PWR_DOWN); // prepare for powerdown
  sleep_enable();

  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);

  previousADCSRA = ADCSRA;
  ADCSRA &= ~(1 << ADEN); //Disable ADC
  ACSR = (1 << ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  DIDR1 = (1 << AIN1D) | (1 << AIN0D); //Disable digital input buffer on AIN1/0

  power_twi_disable();
  power_spi_disable();
  power_usart0_disable(); //Needed for serial.print
  power_timer0_disable(); //Needed for delay and millis()
  power_timer1_disable();
  power_timer2_disable(); //Needed for asynchronous 32kHz operation

  sleep_cpu ();   // power down !
}

void wakeup() { // We should see which of this things we need to remove them.
  power_twi_enable();
  power_spi_enable();
  power_usart0_enable();
  power_timer0_enable();
  power_timer1_enable();
  power_timer2_enable();
  power_adc_enable();
  ADCSRA = previousADCSRA;
  // BOD is automatically restarted at wakeup
}

void loop() {
  startSleeping();
  sleep_disable();
  wakeup(); 
  if ( digitalRead(PIR) ) mySwitch.send(transcode, 24); 
}
