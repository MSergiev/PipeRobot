// Enable pins
static const byte EN[] = { 10, 9, 6, 5 };
// Phase pins
static const byte PH[] = { 12, 11, 8, 7 };

// Motor indices
static const byte FL = 1;
static const byte FR = 0;
static const byte RL = 2;
static const byte RR = 3;

// Directions
static const bool FWD = 0;
static const bool RVS = 1;

// Turning modifier
static const short MOD = 32;

// Axle parameters
short FD = 127;
short RD = 127;

// Direction flags
bool DIR = 0;

// Running flag
bool RUN = 0;

// Turbo flag
bool TURBO = 0;





void modAxle( short& axle, short val) {
  axle += val;
  if( axle < 0 ) axle = 0;
  else if ( axle > 255 ) axle = 255;
}

void revDelay() {
  for( byte m = 0; m < 4; ++m ) {
    analogWrite( EN[m], 0 );
  }
  delay(50);
}



void go() {
  RUN = !RUN; 
  if( !RUN ) {
    analogWrite( EN[FL], 0 );
    analogWrite( EN[FR], 0 );
    analogWrite( EN[RL], 0 );
    analogWrite( EN[RR], 0 );    
  }
  Serial.println( "GO" );
}

void turbo() {
  TURBO = !TURBO; 
  Serial.println( "TURBO" );
}



void forward() {
  DIR = FWD;
  revDelay();
  Serial.println( "FWD" );
}

void back() {
  DIR = RVS;
  revDelay();
  Serial.println( "RVS" );
}

void left() {
  modAxle( FD, -MOD );
  modAxle( RD, MOD );
  Serial.println( "LFT" );
}

void right() {
  modAxle( FD, MOD );
  modAxle( RD, -MOD );
  Serial.println( "RGT" );
}



void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println( "READY" );
}

void loop() {
  
  while( Serial.available() ) {
    char c = Serial.read();
    switch(c) {
      case 'g': go(); break;
      case 't': turbo(); break;
      
      case 'w': forward(); break;
      case 's': back(); break;
      case 'a': left(); break;
      case 'd': right(); break;
    }
  }

  if( RUN ){
    for( byte m = 0; m < 4; ++m ) {
      digitalWrite( PH[m], DIR );
    }

    analogWrite( EN[FL], (TURBO ? 255 : (byte)FD) );
    analogWrite( EN[FR], (TURBO ? 255 : (byte)(255-FD)) );
    
    analogWrite( EN[RL], (TURBO ? 255 : (byte)RD) );
    analogWrite( EN[RR], (TURBO ? 255 : (byte)(255-RD)) );
  }
}
