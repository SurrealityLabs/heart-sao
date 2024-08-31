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
#include "boardspecific.h"
#include "common.h"
#include "charlieplex.h"
#include "debounce.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void heartAlternate(uint16_t alternateInterval);
void heartTopFill(uint16_t fillInterval);
void heartMarquee(uint16_t marqueeInterval);
void heartChaser(uint16_t chaseInterval);
void heartChaserInverse(uint16_t chaseInterval);

void heartAlternate(uint16_t fillInterval) {
    uint8_t i = 0;

    cli();
    setAll(0);
    if(frameCount >= ( 2 * fillInterval)) {
        frameCount = 0;
    } else if(frameCount >= fillInterval) {
        for(i = 0; i < 20; i+=2) {
            charlieArray[i] = 1;
        }
    } else {
        for(i = 1; i < 20; i+=2) {
            charlieArray[i] = 1;
        }
    }
    sei();
}

void heartTopFill(uint16_t fillInterval) {
    uint8_t currentStage;
    uint8_t i;

    cli();

    if(frameCount >= (32 * fillInterval)) {
        frameCount = 0;
    } else if(frameCount >= (11 * fillInterval)) {
        currentStage = (frameCount / fillInterval) - 11;
        setAll(1);
        for(i = 0; i <= currentStage; i++) {
            charlieArray[i] = 0;
            if(i) {
                charlieArray[20-i] = 0;
            }
        }
    } else {
        currentStage = frameCount / fillInterval;
        setAll(0);
        for(i = 0; i <= currentStage; i++) {
            charlieArray[i] = 1;
            if(i) {
                charlieArray[20-i] = 1;
            }
        }
    }

    sei();
}

void heartMarquee(uint16_t marqueeInterval) {
    cli();
    setAll(0);

    if(frameCount >= (4 * marqueeInterval)) {
        frameCount = 0;
    } else {
        uint8_t currentStage = frameCount / marqueeInterval;
        charlieArray[currentStage] = 1;
        charlieArray[currentStage + 4] = 1;
        charlieArray[currentStage + 8] = 1;
        charlieArray[currentStage + 12] = 1;
        charlieArray[currentStage + 16] = 1;
    }
    sei();
}

void heartChaser(uint16_t chaseInterval) {
    static uint8_t chaseOffset = 0;

    cli();
    setAll(0);

    if(frameCount >= chaseInterval) {
        frameCount = 0;
        chaseOffset++;
    }
    
    charlieArray[(chaseOffset) % 20] = 1;
    charlieArray[(chaseOffset + 1) % 20] = 1;
    charlieArray[(chaseOffset + 2) % 20] = 1;

    sei();
}

void heartChaserInverse(uint16_t chaseInterval) {
    static uint8_t chaseOffset = 0;

    cli();
    setAll(1);

    if(frameCount >= chaseInterval) {
        frameCount = 0;
        chaseOffset++;
    }
    
    charlieArray[(chaseOffset) % 20] = 0;
    charlieArray[(chaseOffset + 1) % 20] = 0;
    charlieArray[(chaseOffset + 2) % 20] = 0;

    sei();
}

void modeSwitcher(uint8_t mode) {
    switch(mode) {
        case 0:
            allOff();
            PORTA.OUT &= 0b11100000;
            if(getDebounceState() == DEBOUNCE_IDLE) doSleep();
            break;
        case 1:
            allOn();
            break;
        case 2:
            allBlink(750);
            break;
        case 3:
            heartAlternate(300);
            break;
        case 4:
            heartTopFill(150);
            break;
        case 5:
            heartMarquee(300);
            break;
        case 6:
            heartChaser(150);
            break;
        case 7:
            heartChaserInverse(150);
            break;
        default:
            mode = 0;
            break;
    }
    return;
}
