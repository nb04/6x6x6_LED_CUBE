int dataPin = 4;
int latchPin = 5;
int clockPin = 6;
byte SR[7];
int mode = 0;

uint8_t level[6] = {B10111111, B11011111, B11101111,
                    B11110111, B11111011, B11111101};
uint8_t idk[7] = {B00000000, B01000000, B01100000, B01110000, 
                  B01111000, B01111100, B01111110};
uint8_t single_on[] = {B01000000, B00100000, B00010000,
                       B00001000, B00000100, B00000010}; 
int LEVEL2[5] = {B10011111, B11001111, B11100111, B11110011, B11111001}; // this is an array for the neighbour levels
int LEVEL3[4] = {B10001111, B11000111, B11100011, B11110001}; 
int ROW2[5] =   {B01100000, B00110000, B00011000, B00001100, B00000110}; // for every neighbour rows
int ROW3[4] =   {B01110000, B00111000, B00011100, B00001110}; // for every neighbour rows                       

int button1 = 7;
int button2 = 9;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  Serial.begin(9600);
}

void loop(){
  //use buttons so switch between patterns
  if(mode == 0){
    mode = spiral();
  }
  else if(mode == 1){
    mode = single_random();
  }
  else if(mode == 2){
    mode = random2x2();
  }
  else if(mode == 3){
    mode = random3x3();
  }
}

int spiral(){
  for(int i=0; i<6; i++){
    for(int j=0; j<20; j++){//j = 0-19
      SR[6] = level[i];
      if(j < 6){//0-5
        SR[0] = single_on[j];
      }
      else if(9 < j and j < 16){//j = 10-15
        SR[5] = single_on[6-(j-9)];
      }
      else{
        if(j<10){//6-9
          SR[j-5]= single_on[5];
        }
        else{//16-19
          SR[6-(j-14)] = single_on[0];
        }
      }
    shift_out();
    if(digitalRead(button1)==HIGH){
      delay(300);
      return(3);
    }
    if(digitalRead(button2)==HIGH){
      delay(300);
      return(1);
    }
    delay(map(analogRead(A6), 0, 720, 8, 128));
    clear_out();
    }
  }
  return 0;
}

int single_random(){
  SR[6] = level[random(0,6)];
  SR[random(0,6)] = single_on[random(0,6)];
  shift_out();
  if(digitalRead(button1)==HIGH){
    delay(300);
    return(0);
  }
  if(digitalRead(button2)==HIGH){
    delay(300);
    return(2);
  }
  delay(map(analogRead(A6), 0, 720, 8, 56));
  clear_out();
  return(1);
}

int random2x2() {         //2x2 cube
  for (int k = 0; k < 20; k ++) {
    int SRrandom = random(0, 5);

    SR[6] = LEVEL2[random(0, 5)];
    SR[SRrandom] = SR[SRrandom + 1] = ROW2[random(0, 5)];
    shift_out();
    if(digitalRead(button1)==HIGH){
      delay(300);
      return(1);
    }
    if(digitalRead(button2)==HIGH){
      delay(300);
      return(3);
    }
    delay(map(analogRead(A6), 0, 720, 40, 320));
    clear_out();
  }
  return(2);
}

int random3x3() {         //3x3 cube
  for (int k = 0; k < 20; k ++) {
    int SRrandom = random(0, 4);

    SR[6] = LEVEL3[random(0, 4)];
    SR[SRrandom] = SR[SRrandom + 1] = SR[SRrandom + 2] = ROW3[random(0, 5)];
    shift_out();
    if(digitalRead(button1)==HIGH){
      delay(300);
      return(2);
    }
    if(digitalRead(button2)==HIGH){
      delay(300);
      return(0);
    }
    delay(map(analogRead(A6), 0, 720, 40, 320));
    clear_out();
  }
  return(3);
}

//////////////////////////////////////////////////

void clear_out() {              //this will clear_out all the leds
  for (int i = 0; i < 6; i++) {
    SR[i] = B00000000; 

  }
  SR[6] = B11111111;
  shift_out();                  //this will shift_out the 'clear_out' command.

}

void shift_out() {              // this will update the hexadecimal code to the shift registers
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 7; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, SR[i]);
  }
  digitalWrite(latchPin, 1);
}




void adjustableCube(){
  int stage = map(analogRead(A6), 0, 717, 11, 0);
  int index = 0;
  uint8_t lvls = B11111111;
  uint8_t rows[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
  for(int h=0; h<100; h++){
    for(int i=0; i<6; i++){
      for(int j=0; j<stage or j<6; j++){
        SR[6] = level[j];
        if(stage - j < 0){
          index = 0;
        }
        else if(stage - j > 6){
          index = 6;
        }
        else{
          index = stage - j;
        }
        SR[j] = idk[index];
        
        shift_out();
        delayMicroseconds(4);
        clear_out();
      }
    }
  }
}
