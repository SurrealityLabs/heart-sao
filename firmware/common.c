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
#include "common.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void doSleep(void) {
/*
    TIMSK = (0 << OCIE1A) | (0 << OCIE1B) | (0 << OCIE0A) | (0 << OCIE0B) | (0 << TOIE1) | (0 << TOIE0);
    PCMSK = (1 << BUTTON);
    GIMSK = (1 << PCIE);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    uint8_t mcucr1 = MCUCR | (1 << BODS) | (1 << BODSE);
    uint8_t mcucr2 = mcucr1 & ~(1 << BODSE);

    cli();
    MCUCR = mcucr1;
    MCUCR = mcucr2;

    sei();
    sleep_cpu();
    cli();

    sleep_disable();
    TIMSK = (0 << OCIE1A) | (0 << OCIE1B) | (1 << OCIE0A) | (0 << OCIE0B) | (1 << TOIE1) | (0 << TOIE0);
    GIMSK = (0 << PCIE);
    sei();*/
}

void setAll(uint8_t state) {
    uint8_t i = 0;
    for(i=0; i<20; i++) {
        charlieArray[i] = state;
    }
}

void allOff(void) {
    cli();
    setAll(0);
    frameCount = 0;
    sei();
}

void allOn(void) {
    cli();
    setAll(1);
    frameCount = 0;
    sei();
}

void allBlink(uint16_t blinkInterval) {
    cli();
    if(frameCount >= (2 * blinkInterval)) {
        frameCount = 0;
    } else if(frameCount >= blinkInterval) {
        setAll(1);
    } else {
        setAll(0);
    }
    sei();
}

