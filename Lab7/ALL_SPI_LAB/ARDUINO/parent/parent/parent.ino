 #include <SPI.h>

  void setup (void)
 {
    Serial.begin (115200);
    Serial.println ();

    digitalWrite(SS, HIGH);  // ensure SS stays high for now
    SPI.begin ();

    // Slow down the master a bit
    SPI.setClockDivider(SPI_CLOCK_DIV128);

        delay (1000);  // 1 second delay
        Serial.println ("Master Starting:");
    }  // end of setup

  byte transferAndWait (const byte what)
  {
    byte a = SPI.transfer (what);
    delayMicroseconds (100);
    //delay(500);
    return a;
  } // end of transferAndWait

  void loop (void)
  {

    Serial.println ("Start send");

    // enable Slave Select
    digitalWrite(SS, LOW);

    transferAndWait (1);  // add command
    transferAndWait (2);
    transferAndWait (3);
    transferAndWait (4);
    
      // disable Slave Select
    digitalWrite(SS, HIGH);

    Serial.println ("First 4 sent");

    // enable Slave Select
    digitalWrite(SS, LOW);

    transferAndWait (11);  // add command
    transferAndWait (12);
    transferAndWait (13);
    transferAndWait (14);
        
    // disable Slave Select
    digitalWrite(SS, HIGH);

    Serial.println ("Second 4 sent");

    }  // end of loop

