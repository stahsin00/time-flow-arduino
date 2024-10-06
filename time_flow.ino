/*
 * SimpleReceiver.cpp
 *
 * Demonstrates receiving ONLY NEC protocol IR codes with IRremote
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 ************************************************************************************
 * MIT License
 *
 * Copyright (c) 2020-2023 Armin Joachimsmeyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ************************************************************************************
 */

#include <Arduino.h>

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
#define DECODE_NEC  // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_FAST

//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

//#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

//#define RAW_BUFFER_LENGTH  750 // For air condition remotes it requires 750. Default is 200.

/*
 * This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
 */
#define IR_RECEIVE_PIN_ONE 2
#define IR_RECEIVE_PIN_TWO 3
#define LED_PIN 9
#define NUMPIXELS 30
#include <IRremote.hpp>  // include the library
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;  // Wait for Serial to become available. Is optimized away for some cores.


  // Just to know which program is running on my Arduino
  // Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);
  IrReceiver.begin(3, ENABLE_LED_FEEDBACK);

  // Serial.print(F("Ready to receive IR signals of protocols: "));
  // printActiveIRProtocols(&Serial);
  strip.begin();
  strip.show();
}

void loop() {
  /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
  if (IrReceiver.decode()) {

    /*
         * Print a summary of received data
         */
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print extended info
      // IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume();  // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    } else {
      // Serial.println();
      Serial.println(IrReceiver.decodedIRData.command);
      switch (IrReceiver.decodedIRData.command) {
        case 22:
          setColor(255, 255, 0);
          break;
        case 12:
          setColor(255, 0, 255);
          break;
        case 8:
          setColor(255, 255, 255);
          break;
        case 66:
          setColor(0, 0, 255);
          break;
        case 69:
          setColor(0, 0, 0);
          break;
      }
      IrReceiver.resume();  // Early enable receiving of the next IR frame
                            // IrReceiver.printIRResultShort(&Serial);
                            // IrReceiver.printIRSendUsage(&Serial);
    }
  }

}

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}
