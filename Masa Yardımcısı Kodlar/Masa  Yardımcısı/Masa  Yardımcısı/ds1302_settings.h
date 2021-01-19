#ifndef DS1302_SETTINGS_H
#define DS1302_SETTINGS_H

#define USE_ADELAY_LIBRARY    0           // Set to 1 to use my ADELAY library, 0 to use internal delay functions

#define USE_TIMING            5           // Can be 2 (2V slower) or 5 (5V faster) - see DS1302 datasheet

#define SCLK_PORT             PORTB       // Specify the port and pin for each of these three signals
#define SCLK_PIN              0          
#define SIO_PORT              PORTB
#define SIO_PIN               1
#define CE_PORT               PORTB
#define CE_PIN                2

#define TCS_REGISTER_DEFAULT  0x00        // Trickle charge register default value - see DS1302 datasheet for values

#endif

