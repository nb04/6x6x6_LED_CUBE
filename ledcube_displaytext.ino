int dataPin = 4;
int latchPin = 5;
int clockPin = 6;
byte SR[7];
uint8_t level[6] = {B10111111, B11011111, B11101111,
                    B11110111, B11111011, B11111101
                   };
char input;
int message[100] = {};
int len = 0;
int letts[] = {33, 23, 33, 23, 33, 26, 11, 4, 3, 26, 2, 20, 1, 4, 26}; //6x6x6 led cube
void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  for(int i=0; i<100; i++){
    message[i]=0;
  }
  Serial.begin(9600);
  Serial.println("Type something to be displayed on the cube");
}

void loop() {
  if (Serial.available()) {
    input = Serial.read();
    Serial.print("You typed: " );
    message[len] = input;
    Serial.println(message[len]);
    Serial.print("len: ");
    Serial.println(len);
    len += 1;
    if (int(input) == 10) {
      Serial.println("Here");
      int output[len-1] = {};
      for (int i = 0; i < 100; i++) {
        if (message[i] >96 and message[i] < 123) {
          message[i] -= 97;
        }
        else if(message[i] == 32){
          message[i] = 26;
        }
        else if(message[i] == 10){
          for(int j=0; j<len-1; j++){
            output[j] = message[j];
          }
          break;
        }
        Serial.println(message[i]);
      }
      displayChars(output, sizeof output / sizeof output[0]);
      len = 0;
      for(int i=0; i<100; i++){
       message[i]=0;
      }
    }
  }
}
void displayChars(int letters[], int n) { //give the number values of the letters you want to display.
  //note a = 0 and z = 25, space = 26, numbers are 27-36
  uint8_t alphabet[][6] = {{B00100100, B00100100, B00111100, B00100100, B00100100, B00011000},//a
    {B00111000, B00100100, B00100100, B00111000, B00100100, B00111000},//b
    {B00011000, B00100100, B00100000, B00100000, B00100100, B00011000},//c
    {B00111000, B00100100, B00100100, B00100100, B00100100, B00111000},//d
    {B00111100, B00100000, B00100000, B00111000, B00100000, B00111100},//e
    {B00100000, B00100000, B00100000, B00111000, B00100000, B00111100},//f
    {B00011000, B00100100, B00101100, B00100000, B00100100, B00011000},//g
    {B00100100, B00100100, B00100100, B00111100, B00100100, B00100100},//h
    {B00111000, B00010000, B00010000, B00010000, B00010000, B00111000},//i
    {B00110000, B01001000, B01001000, B00001000, B00001000, B00111110},//j
    {B00100100, B00100100, B00101000, B00110000, B00101000, B00100100},//k
    {B00111100, B00100000, B00100000, B00100000, B00100000, B00100000},//l
    {B01000100, B01000100, B01000100, B01010100, B01101100, B01000100},//m
    {B01000010, B01000110, B01001010, B01010010, B01100010, B01000010},//n
    {B00011000, B00100100, B00100100, B00100100, B00100100, B00011000},//o
    {B00100000, B00100000, B00111000, B00100100, B00100100, B00111000},//p
    {B00110100, B01001000, B01010100, B01000100, B01000100, B00111000},//q
    {B00100100, B00101000, B00111000, B00100100, B00100100, B00111000},//r
    {B00011000, B00100100, B00001000, B00010000, B00100000, B00011100},//s
    {B00010000, B00010000, B00010000, B00010000, B00010000, B01111100},//t
    {B00011000, B00100100, B00100100, B00100100, B00100100, B00100100},//u
    {B00010000, B00101000, B01000100, B01000100, B01000100, B01000100},//v
    {B01000100, B01101100, B01010100, B01000100, B01000100, B01000100},//w
    {B01000100, B00101000, B00010000, B00101000, B01000100, B00000000},//x
    {B00010000, B00010000, B00010000, B00010000, B00101000, B01000100},//y
    {B01111100, B01000000, B00100000, B00010000, B00001000, B01111100},//z
    {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},//space
    {B00011000, B00100100, B00100100, B00100100, B00100100, B00011000},//0
    {B00011100, B00001000, B00001000, B00001000, B00011000, B00001000},//1
    {B00111100, B00100000, B00010000, B00001000, B00100100, B00011000},//2
    {B00011000, B00100100, B00000100, B00001000, B00100100, B00011000},//3
    {B00000100, B00000100, B00000100, B00111100, B00100100, B00100100},//4
    {B00011000, B00100100, B00000100, B00111000, B00100000, B00111100},//5
    {B00011000, B00100100, B00100100, B00111000, B00100000, B00011100},//6
    {B00000100, B00000100, B00000100, B00000100, B00000100, B00111100},//7
    {B00011000, B00100100, B00100100, B00011000, B00100100, B00011000},//8
    {B00011000, B00000100, B00011100, B00100100, B00100100, B00011000}
  };//9
  for (int letter = 0; letter < n; letter++) {
    for (int row = 0; row < 6; row++) {
      for (int reps = 0; reps < 8; reps++) {
        for (int lvl = 0; lvl < 6; lvl++) {
          SR[6] = level[lvl];
          SR[5 - row] = alphabet[letters[letter]][5 - lvl];
          shift_out();
          delayMicroseconds(1);
          clear_out();
        }
      }
    }
  }
}
void clear_out() { //turns the whole cube off
  for (int i = 0; i < 6; i++) {
    SR[i] = B00000000;
  }
  SR[6] = B11111111;
  shift_out();
}
void shift_out() { // this will update the binary code to the shift registers
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 7; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, SR[i]);
  }
  digitalWrite(latchPin, 1);
}
