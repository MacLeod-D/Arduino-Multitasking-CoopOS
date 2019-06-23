// (C) 2019 Helmut Weber 


// Bit manipulation
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_TOGGLE(a,b) ((a) ^= (1ULL<<(b)))

#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1
#define BIT_CHECKH(a,b) ((a&(1<<b)) != 0)
#define BIT_CHECKL(a,b) ((a&(1<<b)) == 0)


/*
B0 = digital pin 8
B1 = digital pin 9
B2 = digital pin 10
B3 = digital pin 11
B4 = digital pin 12
B5 = digital pin 13
B6 is mapped to the crystal, do not use.
B7 is mapped to the crystal, do not use.
C0 = analog pin 0  // Could be used as digital pins
C1 = analog pin 1
C2 = analog pin 2
C3 = analog pin 3
C4 = analog pin 4
C5 = analog pin 5
C6 = analog pin 6, available only on Arduino mini.
C7 = analog pin 7, available only on Arduino mini.
D0 = digital pin 0, used for serial communication, save it's state.
D1 = digital pin 1, used for serial communication, save it's state.
D2 = digital pin 2
D3 = digital pin 3
D4 = digital pin 4
D5 = digital pin 5
D6 = digital pin 6
D7 = digital pin 7
*/

// DigitalWrite
#define BITSETD0      BIT_SET(PORTD,0)
#define BITSETD1      BIT_SET(PORTD,1)
#define BITSETD2      BIT_SET(PORTD,2)
#define BITSETD3      BIT_SET(PORTD,3)
#define BITSETD4      BIT_SET(PORTD,4)
#define BITSETD5      BIT_SET(PORTD,5)
#define BITSETD6      BIT_SET(PORTD,6)
#define BITSETD7      BIT_SET(PORTD,7)

#define BITSETD8      BIT_SET(PORTB,0)
#define BITSETD9      BIT_SET(PORTB,1)
#define BITSETD10     BIT_SET(PORTB,2)
#define BITSETD11     BIT_SET(PORTB,3)
#define BITSETD12     BIT_SET(PORTB,4)
#define BITSETD13     BIT_SET(PORTB,5)

#define BITCLEARD0      BIT_CLEAR(PORTD,0)
#define BITCLEARD1      BIT_CLEAR(PORTD,1)
#define BITCLEARD2      BIT_CLEAR(PORTD,2)
#define BITCLEARD3      BIT_CLEAR(PORTD,3)
#define BITCLEARD4      BIT_CLEAR(PORTD,4)
#define BITCLEARD5      BIT_CLEAR(PORTD,5)
#define BITCLEARD6      BIT_CLEAR(PORTD,6)
#define BITCLEARD7      BIT_CLEAR(PORTD,7)

#define BITCLEARD8      BIT_CLEAR(PORTB,0)
#define BITCLEARD9      BIT_CLEAR(PORTB,1)
#define BITCLEARD10     BIT_CLEAR(PORTB,2)
#define BITCLEARD11     BIT_CLEAR(PORTB,3)
#define BITCLEARD12     BIT_CLEAR(PORTB,4)
#define BITCLEARD13     BIT_CLEAR(PORTB,5)


#define BITTOGGLED0      BIT_TOGGLE(PORTD,0)
#define BITTOGGLED1      BIT_TOGGLE(PORTD,1)
#define BITTOGGLED2      BIT_TOGGLE(PORTD,2)
#define BITTOGGLED3      BIT_TOGGLE(PORTD,3)
#define BITTOGGLED4      BIT_TOGGLE(PORTD,4)
#define BITTOGGLED5      BIT_TOGGLE(PORTD,5)
#define BITTOGGLED6      BIT_TOGGLE(PORTD,6)
#define BITTOGGLED7      BIT_TOGGLE(PORTD,7)

#define BITTOGGLED8      BIT_TOGGLE(PORTB,0)
#define BITTOGGLED9      BIT_TOGGLE(PORTB,1)
#define BITTOGGLED10     BIT_TOGGLE(PORTB,2)
#define BITTOGGLED11     BIT_TOGGLE(PORTB,3)
#define BITTOGGLED12     BIT_TOGGLE(PORTB,4)
#define BITTOGGLED13     BIT_TOGGLE(PORTB,5)


// DigitalRead
#define BITCHECKD0      BIT_CHECK(PIND,0)
#define BITCHECKD1      BIT_CHECK(PIND,1)
#define BITCHECKD2      BIT_CHECK(PIND,2)
#define BITCHECKD3      BIT_CHECK(PIND,3)
#define BITCHECKD4      BIT_CHECK(PIND,4)
#define BITCHECKD5      BIT_CHECK(PIND,5)
#define BITCHECKD6      BIT_CHECK(PIND,6)
#define BITCHECKD7      BIT_CHECK(PIND,7)

#define BITCHECKD8      BIT_CHECK(PORTB,0)
#define BITCHECKD9      BIT_CHECK(PORTB,1)
#define BITCHECKD10     BIT_CHECK(PORTB,2)
#define BITCHECKD11     BIT_CHECK(PORTB,3)
#define BITCHECKD12     BIT_CHECK(PORTB,4)
#define BITCHECKD13     BIT_CHECK(PORTB,5)

uint8_t *MyPinToPort (int pin );

uint8_t MyPinToBitMask (int pin);

  
