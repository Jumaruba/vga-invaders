/**
 * @file toggle_timer.ino
 * @brief Turns on and off the light in arduino due. Source: https://forum.arduino.cc/t/timer-interrupts-on-due/127643/92
 * @version 0.1
 * @date 2022-05-14
*
 * @copyright Copyright (c) 2022
 * 
 */

volatile boolean l;

void TC0_Handler()
{
    long dummy=REG_TC0_SR0; // vital - reading this clears some flag
                            // otherwise you get infinite interrupts
    l= !l;
    digitalWrite(72,l);
}

void setup(){
  pinMode(72,OUTPUT);
  analogWrite(2,255);   // sets up some other registers I haven't worked out yet
  REG_PIOB_PDR = 1<<25; // disable PIO, enable peripheral
  // datasheet: page 908;  enable to write registers. 
  REG_TC0_WPMR=0x54494D00; 
 
  REG_TC0_CMR0=0b00000000000010011100010000000000; // set channel mode register (see datasheet)

  REG_TC0_RC0=100000000;    // counter period (10 seconds)
  REG_TC0_RA0=30000000;     // PWM value

  // START THE COUNTER 
  // Data sheet page 880. 
  // bit 1(CLKEN): enables the clock if CLKDIS not enabled.
  // bit 2(CLKDIS): disable the clock.
  // bit 3(SWTRG): the clock is reseted and the clock is restarted.  
  /**
   * @brief START THE COUNTER 
   * Data sheet page 880. 
   * bit 1(CLKEN): enables the clock if CLKDIS not enabled.
   *  bit 2(CLKDIS): disable the clock.
   * 
   */
  REG_TC0_CCR0=0b101;       // start counter


  REG_TC0_IER0=0b00010000;  // enable interrupt on counter=rc
  REG_TC0_IDR0=0b11101111;  // disable other interrupts

  NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interrupts

}

void loop(){

}