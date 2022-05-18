volatile boolean l;
volatile int k = 1;
volatile uint8_t frame[320][240];

//TC1 ch 0
void TC0_Handler()
{
        //Serial.print(k);
        //Serial.print("aaa\n");
        TC_GetStatus(TC0, 0);
        for(int i=0; i < 320; i++) {
          for(int j=0; j < 240; j++)
            frame[i][j] = k;
        }
        // send vsync
        // for line send signal and hsync
        Serial.print(k);
        Serial.print("a\n");
        k++;
}

void TC3_Handler()
{
        TC_GetStatus(TC1, 0);
        //digitalWrite(72, l = !l);
        Serial.print("====\n");
        for(int i=0; i < 320; i++) {
          for(int j=0; j < 240; j++)
            Serial.print(arr[i][j]);
        }
        Serial.print("====\n");
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
}

void setup(){
        Serial.begin(9600);
        pinMode(72,OUTPUT);
        startTimer(TC0, 0, TC0_IRQn, 30); //TC1 channel 0, the IRQ for that channel and the desired frequency
        //startTimer(TC1, 0, TC3_IRQn, 1);
}

void loop(){
}
