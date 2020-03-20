
#define OLED                   // OLED display attached. If not, comment out

#ifdef OLED
/* ===============================OLED========================================== */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>       // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>   // https://github.com/adafruit/Adafruit_SSD1306
#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 32        // OLED display height, in pixels

/* ==== The 64 logo ==== */
#define LOGO_HEIGHT   30
#define LOGO_WIDTH    34
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B00000000, B11111001, B11100000, B00000000,
B00000000, B00000001, B11110011, B11001000, B00000000,
B00000000, B00000011, B11100111, B10011000, B00000000,
B00000000, B00000111, B11001111, B00111000, B00000000,
B00000000, B00001111, B10011110, B01111000, B00000000,
B00000000, B00011111, B00111100, B01111000, B00000000,
B00000000, B00111110, B01111000, B01111000, B00000000,
B00000000, B01111100, B11110000, B01111000, B00000000,
B00000000, B11111001, B11100000, B01111000, B00000000,
B00000001, B11110011, B11000000, B01111000, B00000000,
B00000011, B11100111, B11000000, B01111000, B00000000,
B00000111, B11001111, B11000000, B01111111, B11000000,
B00001111, B10011111, B11111111, B11111111, B10000000,
B00011111, B00111111, B11111111, B11111111, B00000000,
B00111110, B01111111, B11111111, B11111110, B00000000,
B00111100, B10000111, B11111111, B11111100, B00000000,
B01111000, B00000001, B11110000, B01111000, B00000000,
B01111000, B00000000, B11110000, B01111000, B00000000,
B01110000, B00000000, B01111000, B01111000, B00000000,
B11110000, B00000000, B01111000, B01111000, B00000000,
B11110000, B00000000, B01111000, B01111000, B00000000,
B11110000, B00000000, B01111000, B01111000, B00000000,
B01111000, B00000000, B01111000, B01111000, B00000000,
B01111000, B00000000, B11111000, B01111000, B00000000,
B01111100, B00000001, B11110000, B01111000, B00000000,
B00111111, B00000111, B11110000, B01111000, B00000000,
B00011111, B11111111, B11100000, B01111000, B00000000,
B00001111, B11111111, B11000000, B01111000, B00000000,
B00000111, B11111111, B00000000, B01111000, B00000000,
B00000000, B11111000, B00000000, B01111000, B00000000 };

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
 * show the title screen with lofo on OLED
 */
void showLogo(void) {
  display.clearDisplay();                                 // clear OLED
  display.drawBitmap(                                     // place the 64 logo in the middle of the OLED
    (display.width()  - LOGO_WIDTH ) /2,
    (display.height() - LOGO_HEIGHT) /2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();                                      // display it!
  delay(2000);                                            // wait 2 secs 
  display.clearDisplay();                                 // clear OLED
  display.setCursor(10,6);                                // position the cursor
  display.setTextSize(1);                                 // smalest text
  display.setTextColor(SSD1306_WHITE);                    // text color is white
  display.println(F("http://"));                          // put http://tech.guitarsite.de on oled
  display.setCursor(10,16);
  display.println(F("tech.guitarsite.de")); 
  display.display();                                      // display it
}

/*
 * write thet "Fire Rate" etc. on oled. 
 */
void writeText(void) {
  display.clearDisplay();                 //clear OLED
  display.setTextSize(2);                 // middle text size
  display.setTextColor(SSD1306_WHITE);    // Draw white text
  display.setCursor(7,0);                 // Start at top-left corner
  display.cp437(true);                    // Use full 256 char 'Code Page 437' font
  display.println(F("Fire rate:"));       // the text
  display.setCursor(66,18);               // 2nd line
  display.println(F("r/min"));            // the unit (rounds per minute)
  display.display();                      // display it
 }

/*
 *  write the fire rate to the OLED
 */
void writeFireRate(int rf_interval) {
  int cad;
  cad = 30000/(rf_interval);                              // calculate the fire rate in rounds per minute from the half period time
  display.fillRect( 3, 18, 60, 16, SSD1306_BLACK );       // clear the space for the fire rate: draw a black rectangle
  if (cad == 0) {
    display.setCursor(15,18);
    display.println(F("OFF"));                            // show OFF
    display.display(); 
  }
  else {
    if (cad < 10) { display.setCursor(3+3*12,18); }       // position the cursor (the fire rate is aligned right)
    else if (cad < 100) { display.setCursor(3+2*12,18); }
    else if (cad < 1000) { display.setCursor(3+12,18); }
    else display.setCursor(3,18); 
    display.println( cad );//                             // write fire rate to the display
  }
  display.display();                                      // display it
}

void writeStatusOff( void ){
  display.clearDisplay();
  display.clearDisplay();                 //clear OLED
  display.setTextSize(4);                 //large text size
  display.setCursor(28,2);                // position sursor
  display.println(F("OFF"));              // write OFF to OLED
  display.display(); 
}
/* ===============================OLED========================================== */
#endif  // ifdef OLED


#define rfspeed  A0                       // potentiometer input
#define blink_time 200                    // blink time (msecs)
#define snd_time 250                      // sound time (msecs)
#define debounce 30
#define rate_cycle 250                    // display the fire rate every <rate_cycle> msecs


int fire1 = 1;   // fire port #1 (active high)
int fire2 = 0;   // fire port #2 (active high)
int port2 = 4;   // port switch #1 = low, #2 = high
int nled1 = 5;   // led port #1 (active low)
int nled2 = 6;   // led port #2 (active low)
int nswitch = 7; // channel select switch (#1 = low, #2 = high)  
int nrf_on = 8;  // rapid fire toggle switch (active low)
int snd_out = 9; // output for piezo speaker
int nfire = 14;  // fire input from joystick

// int wait_time = 500;
int channel = 2;                  // default channel is port #2
int rapid_fire = 0;               // rapid fire flag
int loop_counter = blink_time;    // some loop counter for blinking the LEDs
int rapid_fire_interval = 200;    // rapid fire interval in mili seconds
int rf_loop_counter = 200;        // loop counter for rapid fire
int previous_fire_state = 1;      // previous state of the fire button
int debounce_counter = debounce;  // counter for debouncing rf switch
int rf_toggle = 0;                // flag to let the rapid fire only toggle once before the switch is released
int blink_state = 0;              // state of the led blink (while rapid fire)
int rf_state = 1;                 // state of rapid fire, 1 = fire, 0 = not fire
int pot_value;                    // value of the potentiometer
int fire_output = HIGH;           // fire output level
int snd_counter = 0;              // sound count down
int display_counter = 0;          // display cycle coundown
int last_rate = 0;                // last displayed fire rate

void setup() {
  digitalWrite(fire1, LOW);       // initialize with low (= inactive)
  pinMode(fire1,  OUTPUT);        // fire port #1 is an output
  
  digitalWrite(fire2, LOW);       // initialize with low (= inactive)
  pinMode(fire2, OUTPUT);         // fire port #2 is an output
  
  digitalWrite(port2, HIGH);      // initialize with high (= Port #2 is selected by default)
  pinMode(port2, OUTPUT);         // this signal determins which port is used. It is an output

  digitalWrite(nled1, HIGH);      // initialize with high (= led1 is off)
  pinMode(nled1, OUTPUT);         // led port #1 is an output

  digitalWrite(nled2, LOW );      // initialize with low (= led2 is on)
  pinMode(nled2, OUTPUT);         // led port #2 is an output
  
  pinMode(nswitch, INPUT);        // channel select switch is an input
  pinMode(nrf_on,  INPUT);        // rapid fire toggle switch is an input
  pinMode(nfire,   INPUT);        // nfire is an input
  
  tone( snd_out, 4000 );          // 1kz sound on snd_out
  delay( 250 );
  noTone( snd_out );

#ifdef OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    while(1) {                      // Don't proceed, loop forever
       tone( snd_out, 4000 );       // 1kz sound on snd_out
       delay( 250 );
       tone( snd_out, 2000 );       // 1kz sound on snd_out
       delay( 250 );
    }
  }
  showLogo();
#endif
}

void loop() {
   // test the channel switch and change ports if required
   if  (digitalRead( nswitch ) == LOW) {  // port #1 selected
      if (channel == 2) {                 // port has changed
         digitalWrite( fire2, LOW );      // no more firing at port #2
         digitalWrite( nled2, HIGH );     // switch off led2
         digitalWrite( nled1, LOW );      // switch on LED of Port#1
         digitalWrite( port2, LOW );      // switch joystick signals to port #1
         channel = 1;                     // store channel number
      }
   }
   else {                                 // port #2 selected
      if (channel == 1) {                 // port has changed
         digitalWrite( fire1, LOW );      // no more firing at port #1
         digitalWrite( nled1, HIGH );     // switch off led1
         digitalWrite( nled2, LOW );      // switch on LED of Port#2
         digitalWrite( port2, HIGH );     // switch joystick signals to port #2
         channel = 2;                     // store channel number
      }
   }

   pot_value = analogRead(rfspeed);                             // read the value of the potentiometer
   rapid_fire_interval = map( pot_value, 0, 1023, 200, 20 );    // calculate the rapiod fire (half) period in millisecs
   if (rapid_fire==1) {                                         // if rapid fire is switched on
     if (display_counter == 0) {                                // the rate is only displayed every <rate_cycle> msecs
#ifdef OLED
        writeFireRate(rapid_fire_interval);                     // display
#endif
        display_counter = rate_cycle;                           // reset counter
     }
     else {
       display_counter--;                                       // decrement counter
     }
   }

   // test the fire button and fire on the selected channel, if required
   if (digitalRead( nfire ) == LOW ) {                     // fire button pressed?
      if (rapid_fire == 1) {
         if (previous_fire_state == 1) {                   // did the button status change?
            rf_loop_counter = rapid_fire_interval;         // yes: reset the rapid fire loop counter
            rf_state = 1;                                  // and start firing immideately
         }
         else if (rf_loop_counter > 0) {                   // no:
            rf_loop_counter--;                             // decrement counter
         }
         else {                                            // if counter is zero
            rf_loop_counter = rapid_fire_interval;         // reset the rapid fire loop counter
            if (rf_state == 1) {                           // and toggle the rf_state
              rf_state = 0;
            }
            else {
              rf_state = 1;
            }
         }
      }
      fire_output = (rapid_fire == 0) || (rf_state == 1);   // press fire if rapid fire is off or rapid fire state is on
      if (channel == 1) {                                   // is channel 1 selected
        digitalWrite( fire1, fire_output );                 // fire on port #1 
      }
      else {                                                // channel 2 is selected
        digitalWrite( fire2, fire_output );                 // fire on port #2 
      }
      previous_fire_state = 0;                               
   }
   else {                                                   // no fire button pressed
     digitalWrite( fire1, LOW );                            // switch off both fire outputs
     digitalWrite( fire2, LOW );
     previous_fire_state = 1;
   }

   // test the rapid fire button and toggle rapid fire mode, if required
   if (digitalRead( nrf_on ) == LOW) {                      // rapid fire toggle switch pressed? 
      if (rf_toggle == 0) {
        if (debounce_counter > 0) {                         // debounce time not elapsed
          debounce_counter--;                               // decrement debounce counter
        }
        else {                                              // debounce time elapsed
          if (rapid_fire == 0) {                            // toggle rapid fire mode
            rapid_fire = 1;                                 // on
            blink_state = 0;                                // set blink state to off 
            rf_state = 1;                                   // set rf state: fire immideately
            loop_counter = rapid_fire_interval;             // initialize loop_counter
            snd_counter = snd_time;                         // start delay for sound
            tone( snd_out, 4000 );                          // Tone on
            display_counter = 0;           
#ifdef OLED            
            writeText();                                    // write "Fire rate: ..... r/min" on OLED dispklay
#endif              
          }
          else {
            rapid_fire = 0;
#ifdef OLED            
            writeStatusOff();                              // write "OFF" on OLED display 
#endif             
            if (channel == 1) {
              digitalWrite( nled1, LOW );                  // switch on LED of Port#1
            }
            else {
              digitalWrite( nled2, LOW );                  // switch on LED of Port#2
            }
          }
          rf_toggle = 1;                                   // inhibit further toggling until switch was released
        }
      }
   }
   else {
     debounce_counter = debounce;                           // reset debounce counter
     rf_toggle = 0;                                         // reset rf_toggle (allow next toggle of rapid fire state)
   }

   // blink the channel LED if rapid fire is on
   if  (rapid_fire == 1) {                                  // if rapid fire, then blinking
      if (loop_counter > 0) {                               // loop_counter elapsed?
        loop_counter--;                                     // no: decrement
      }
      else {                                                // yes:
        loop_counter = rapid_fire_interval;                 // initialize loop_counter
        if (blink_state == 0) {                             // toggle blink state
          blink_state = 1;                                  // led off
        }
        else {
          blink_state = 0;                                  // led on
        }
      }
      if (channel == 1)                                     // if channel 1 select, then blibk LED1
        if (blink_state == 0) {                             // switch on and off depending on the blink state
          digitalWrite( nled1, HIGH );     
        }
        else {
          digitalWrite( nled1, LOW );  
        }
      else {                               
        if (blink_state == 0) {
          digitalWrite( nled2, HIGH );
        }
        else {
          digitalWrite( nled2, LOW );  
        }  
      }
   }
   if (snd_counter > 0) {                                    // count down for the beeper
      if (--snd_counter == 0) {
        noTone( snd_out );                                   // if the count down is elaped, sound off 
      }
   }
   // this is the 1 mili second loop delay
   delay( 1 ); // delay 1 msec                               // the 1msec loop delay
}
