//name pin
int TS = 12;  //pedestrian touch switch 
int BZ = 11;  //speaker
int TR = A0;  //Traffic light
int TY = A1;  
int TG = A2;  
int PW = A3;  //pedestrian PW light
int PR = A4; 
int PG = A5; 

//name parameter
int freq = 3700; //speaker tone, buzzer hy07 resonance freq at 2600/3700 Hz
int val; //switch status
int T0 = 17; //PW pedestrian button
int T1 = 3; //button delay
int T2 = 2; //yellow light
int T3 = 1; //pedestrian green guand time
int T4 = 8; //pedestrian green time
int count = 9; //blinking time, must between 1 to 28
int i=0;
int j=0;
const uint8_t seg7[29]={0x81,0xf3,0x49,0x61,0x33,0x25,0x05,0xf1,0x01,0x21}; //common anode 0-9
unsigned long PRbeep = 0; //Pedestrian red light beep interval
unsigned long PRbeep2 = 0; 
unsigned long delayStart = 0; //delay start time

void beep(void) { //one beep for 100ms
  tone(BZ,freq,10) ;
  delay(15);
  tone(BZ,freq,10) ;
  delay(15) ;   
  tone(BZ,freq,10) ;
  delay(15);
  tone(BZ,freq,10) ;
  delay(55);
}

void pBlink(int k) {
  digitalWrite(PG, HIGH);
  PORTD |= B11111110;
  PORTD &= seg7[k];
  delay(500); 
  digitalWrite(PG, LOW);
  for (i=0; i < 5; i++) { //dual tone 5 burst per second
    beep();
  }
}

void setup() { //define input output
  pinMode(TS, INPUT); //touch button input
  pinMode(BZ, OUTPUT); //Buzzer output
  DDRD = B11111110; //digit seg g,f,e,d,c,b,a
  pinMode(TR, OUTPUT); //traffic light output
  pinMode(TY, OUTPUT);
  pinMode(TG, OUTPUT);
  pinMode(PW, OUTPUT); //pedestrian light output
  pinMode(PR, OUTPUT);
  pinMode(PG, OUTPUT);

  //initialize
  digitalWrite(TR, HIGH); //turn off all except TG & PR
  digitalWrite(TY, HIGH);
  digitalWrite(TG, LOW);
  digitalWrite(PW, HIGH);
  digitalWrite(PR, LOW);
  digitalWrite(PG, HIGH);
  PORTD |= B11111110;
  delayStart = millis();  // start tick for T0
  PRbeep = millis(); //start tick for 1s
}

void loop() {
  if((millis() - PRbeep)>=1000) { //test one second tick
    i=4;
    PRbeep = millis(); //restart tick for 1s
  }
  if(i>0) { //4 beep
    tone(BZ,freq,10);
    delay(15);
    --i;
  }
  val=digitalRead(TS);  //read Touch Switch
  if(val==HIGH || (millis() - delayStart) >= T0*1000) { //press button or timeout
    if(val==HIGH) {
      beep();
      digitalWrite(PW, LOW);
    }
    for (i=0; i < T1; i++) {
      beep();
      delay(900);
    }
    digitalWrite(TG, HIGH);
    digitalWrite(TY, LOW);
    for (i=0; i < T2; i++) {
      beep();
      delay(900);
    }
    digitalWrite(TY, HIGH);
    digitalWrite(TR, LOW);
    for (i=0; i < T3; i++) {
      beep();
      delay(900);
    }
    digitalWrite(PW, HIGH);
    digitalWrite(PR, HIGH);
    digitalWrite(PG, LOW);
    for (i=0; i < T4*10; i++) {
      beep();
    }
    constrain(count, 1, 28);
    for (j=count; j>  0; j--) { //for count to 0 set j>=0
      pBlink(j);
    }
    digitalWrite(PG, HIGH);
    digitalWrite(PR, LOW);
    PORTD |= B11111110; // blank digit
    for (i=0; i < T3; i++) {
      beep();
      delay(900);
    }
    digitalWrite(TY, LOW);
    for (i=0; i < T2; i++) {
      beep();
    delay(900);
    }
    digitalWrite(TR, HIGH);
    digitalWrite(TY, HIGH);
    digitalWrite(TG, LOW);
    delayStart = millis();
  } 
}
