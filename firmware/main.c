/*
This file is part of Valentines Heart 1.0.

Valentines Heart 1.0 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Valentines Heart 1.0 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Valentines Heart 1.0.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "hardware_setup.h"
#include "debounce.h"
#include "charlieplex.h"
#include "common.h"
#include "boardspecific.h"

int main(void) {
	// configure oscillator for 10mhz operation
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = 0;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = 1;

	// configure PORTA for output on all 5 charlieplex pins, pullup on PA6
    PORTA.DIR = 0b00111110;
    PORTA.OUT = 0b00000000;
	PORTA.PIN6CTRL = 0b00001000;

	// disable analog comparator and ADC
    AC0.CTRLA = 0;
	ADC0.CTRLA = 0;
	ADC0.MUXPOS = 0;
	ADC0.MUXNEG = 0;

    //TimerA set for 60hz frame update and debouncing
    TCA0.SINGLE.CTRLA = 5; // enable, f_clk_per/4
	TCA0.SINGLE.CTRLB = 0; // normal mode
	TCA0.SINGLE.CTRLD = 0;
	TCA0.SINGLE.INTCTRL = 1; // OVF interrupt
	TCA0.SINGLE.PER = 41667;
	TCA0.SINGLE.CNT = 0;

	//TimerB0 set for 60x20 = 1200 Hz for charlieplex refresh
    TCB0.CTRLA = 1; // enable, f_clk_per/2
	TCB0.CTRLB = 0; // Periodic interrupt mode
	TCB0.INTCTRL = 1; // CAPT interrupt
	TCB0.CCMP = 8333;
	TCB0.CNT = 0;

    sei();

    uint8_t mode = 1;
    uint16_t lastFrameCount = 0;

    setAll(0);
    
    while(1) {
        if(lastFrameCount != frameCount) {
            modeSwitcher(mode);
            lastFrameCount = frameCount;
            if(debounceButton()) {
                mode++;
                mode %= 8;
                frameCount = 0;
            }
        }
    }

    return 0;
}

ISR(TCB0_INT_vect) {
    writeCharlieplexLED();
}

ISR(TCA0_OVF_vect) {
    frameCount++;
    registerDebounceCount();
}

/*ISR(PCINT0_vect) {

}*/
