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
#include "charlieplex.h"
#include <avr/io.h>

volatile uint8_t charlieArray[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile uint16_t frameCount = 0;

void writeCharlieplexLED(void) {
    static uint8_t currentLED = 0;

    PORTA.DIR = 0b00000000;
    PORTA.OUT = 0b00100000;
    switch(currentLED) {
        case 0:
        case 1:
        case 2:
        case 3:
            PORTA.DIR |= ((1 << OUTPUT_0));
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            PORTA.DIR |= ((1 << OUTPUT_1));
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            PORTA.DIR |= ((1 << OUTPUT_2));
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            PORTA.DIR |= ((1 << OUTPUT_3));
            break;
        case 16:
        case 17:
        case 18:
        case 19:
            PORTA.DIR |= ((1 << OUTPUT_4));
            break;
    }

    if(charlieArray[currentLED] == 1) {
        switch(currentLED) {
            case 4:
            case 8:
            case 12:
            case 16:
                PORTA.DIR |= ((1 << OUTPUT_0));
                PORTA.OUT = ((1 << 5) | (1 << OUTPUT_0));
                break;
            case 0:
            case 9:
            case 13:
            case 17:
                PORTA.DIR |= ((1 << OUTPUT_1));
                PORTA.OUT = ((1 << 5) | (1 << OUTPUT_1));
                break;
            case 1:
            case 5:
            case 14:
            case 18:
                PORTA.DIR |= ((1 << OUTPUT_2));
                PORTA.OUT = ((1 << 5) | (1 << OUTPUT_2));
                break;
            case 2:
            case 6:
            case 10:
            case 19:
                PORTA.DIR |= ((1 << OUTPUT_3));
                PORTA.OUT = ((1 << 5) | (1 << OUTPUT_3));
                break;
            case 3:
            case 7:
            case 11:
            case 15:
                PORTA.DIR |= ((1 << OUTPUT_4));
                PORTA.OUT = ((1 << 5) | (1 << OUTPUT_4));
                break;
        }
    }

    currentLED++;
    if(currentLED > 19) currentLED = 0;
}
