int dataPin = 4;
int latchPin = 5;
int clockPin = 6;
byte SR[7]; // an array for the shift registers, SR[0-5] controls the rows and SR[6] controls the levels

uint8_t level[6] = {B10111111, B11011111, B11101111,
                    B11110111, B11111011, B11111101};
 
uint8_t single_on[] = {B01000000, B00100000, B00010000,
                       B00001000, B00000100, B00000010};    
                                     
uint8_t high = B11111111;
uint8_t low = B00000000;
int LEVEL2[5] = {B10011111, B11001111, B11100111, B11110011, B11111001}; 
int LEVEL3[4] = {B10001111, B11000111, B11100011, B11110001}; 
int ROW2[5] =   {B01100000, B00110000, B00011000, B00001100, B00000110};
int ROW3[4] =   {B01110000, B00111000, B00011100, B00001110};

int countdown[] = {30, 29, 28}; //3, 2, 1
int letts[] = {33, 23, 33, 23, 33, 26, 11, 4, 3, 26, 2, 20, 1, 4, 26}; //6x6x6 led cube

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  full(1000);
  delay(1000000);
}

void loop() {
  displayChars(letts, sizeof letts / sizeof letts[0]);
  full(1000);
  displayChars(countdown, sizeof countdown / sizeof countdown[0]);
  single_random(3000);
  GrowingCube(8);
  x_trail();
  x_snake();
  y_stretched();
  rain();
  spiral(1);
  for(int j=0; j<3; j++){
    random_on();
    random_off();
  }
  random2x2();
  for(int k=0; k<3; k++){
    x();
    y();
    z();
  }
  random3x3();
  centerCube();
  //frame4x4();
  frame6x6();
  full(500);
  random_off();
  delay(5000);
  //sun();
}

void random_on(){ // turns the cube on 1 random led at a time
  uint8_t cube[36] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, // variable for the states of each leds
                      B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                      B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                      B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                      B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                      B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
  int leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 
                37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 
                73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 
                108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 
                138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 
                168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 
                198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215}; // list of all the leds numbered that gets shuffled
                
  const int count = sizeof leds / sizeof leds[0]; // find size of list
  for (int i=0; i < count; i++){ // for shuffling the list leds[] 
     int n = random(0, count);  // Integer from 0 to questionCount-1
     int temp = leds[n];
     leds[n] =  leds[i];
     leds[i] = temp;
  } 
  
  for (int j=0; j < 216; j++){ // turns each led on in the random order of the list
    int b =  floor(leds[j]/6);
    bitWrite(cube[b], leds[j] - b*6 + 1, HIGH);
    for(int reps = 0; reps < 2; reps++){
      for(int k = 0; k < 6; k++){
        SR[6] = level[k];
        int base = k * 6;
        SR[0] = cube[base];
        SR[1] = cube[base + 1];
        SR[2] = cube[base + 2];
        SR[3] = cube[base + 3];
        SR[4] = cube[base + 4];
        SR[5] = cube[base + 5];
        shift_out();
        delayMicroseconds(1);
        clear_out();
      }
    }
  }
}
void random_off(){ // turns the cube off 1 random led at a time, opposite of random_on()
  uint8_t cube[36] = {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
                      B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
                      B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
                      B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
                      B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
                      B11111111, B11111111, B11111111, B11111111, B11111111, B11111111};
  int leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 
                37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 
                73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 
                108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 
                138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 
                168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 
                198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215};
  const int count = sizeof leds / sizeof leds[0];
  for (int i=0; i < count; i++){
     int n = random(0, count);  // Integer from 0 to questionCount-1
     int temp = leds[n];
     leds[n] =  leds[i];
     leds[i] = temp;
  }
  for (int j=0; j < 216; j++){
    int b =  floor(leds[j]/6);
    bitWrite(cube[b], leds[j] - b*6 + 1, LOW);
    for(int reps = 0; reps < 2; reps++){
      for(int k = 0; k < 6; k++){
        SR[6] = level[k];
        int base = k * 6;
        SR[0] = cube[base];
        SR[1] = cube[base + 1];
        SR[2] = cube[base + 2];
        SR[3] = cube[base + 3];
        SR[4] = cube[base + 4];
        SR[5] = cube[base + 5];
        shift_out();
        delayMicroseconds(1);
        clear_out();
      }
    }
  }
}
void single_random(int t){ // turns a single led on for a very short amount of time
  for(int i=0; i<(t * 0.59); i++){ 
    SR[6] = level[random(0,6)];
    SR[random(0,6)] = single_on[random(0,6)];
    shift_out();
    delayMicroseconds(1);
    clear_out();
  }
}

void x(){ // turns each layer of one face of the cube on at a time
  SR[6] = low;
  for(int i=0; i<5; i++){
    SR[0]=SR[1]=SR[2]=SR[3]=SR[4]=SR[5]=single_on[i];
    shift_out();
    delay(100);
  }
  for(int j=0; j<5; j++){
    SR[0]=SR[1]=SR[2]=SR[3]=SR[4]=SR[5]=single_on[5-j];
    shift_out();
    delay(100);
  }
}

void y(){ // turns each layer of one face of the cube on at a time
  for(int i=0; i<6; i++){
    SR[i] = high;
  }
  for(int j=0; j<5; j++){
    SR[6] = level[j];
    shift_out();
    delay(100);
  }
  for(int k=0; k<5; k++){
    SR[6] = level[5-k];
    shift_out();
    delay(100);
  }
}

void z(){ // turns each layer of one face of the cube on at a time 
  clear_out();
  for(int i=0; i<5; i++){
    SR[6] = low;
    SR[i] = high;
    shift_out();
    delay(100);
    clear_out();
  }
  for(int i=0; i<5; i++){
    SR[6] = low;
    SR[5-i]=high;
    shift_out();
    delay(100);
    clear_out();
  }
}

void x_snake(){ // turns all the layers of a face of the cube on in a zig-zag pattern I guess
  SR[6] = low;
  uint8_t one = B00000000;
  uint8_t two = B00000000;
  int row = 0;
  for(int b=0; b<42; b++){
    if(row < floor(b/6)){
       row = floor(b/6);
       two = one;
       one = B00000000;
    }
    if(row % 2 == 0){
      bitWrite(one, b - row * 6 + 1, 1);
      bitWrite(two, 6 - (b - row * 6), 0);
    }
    else{
      bitWrite(one, 6 - (b - row * 6), 1);
      bitWrite(two, b - row * 6 + 1, 0);
    }
    if(row < 6){
      SR[row]=one;
    }
    if(row > 0){
      SR[row-1]=two;
    }
    shift_out();
    delay(80);
  }
}

void y_stretched(){ // turns all the layers of a face of the cube on but doesn't turn layers off until the whole cube is on
  uint8_t hi = B11111111;
  for(int i=0; i<6; i++){
    SR[i] = high;
  }
  for(int j=1; j<7; j++){
    bitWrite(hi, j, 0);
    SR[6] = hi;
    shift_out();
    delay(100);
  }
  for(int j=1; j<7; j++){
    bitWrite(hi, j, 1);
    SR[6] = hi;
    shift_out();
    delay(100);
  }
  for(int k=1; k<7; k++){
    bitWrite(hi, 7-k, 0);
    SR[6] = hi;
    shift_out();
    delay(100);
  }
  for(int k=1; k<7; k++){
    bitWrite(hi, 7-k, 1);
    SR[6] = hi;
    shift_out();
    delay(100);
  }
  clear_out();
}

void x_trail(){ // turns all the layers of a face of the cube on but leaves some led behind I guess
  int trail[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
  const int count = sizeof trail / sizeof trail[0];
  for (int i=0; i < count; i++){ // randomizes the order of the values in the list trail[]
     int n = random(0, count);  // Integer from 0 to questionCount-1
     int temp = trail[n];
     trail[n] =  trail[i];
     trail[i] = temp;
  } 
  int n = 0;
  int m = 0;
  int show_end_longer = 10;
  for(int h=0; h<6; h++){
    n = h*3;
    m=h*36;   
    trail[n] += m;
    trail[n +1] += m;
    trail[n +2] += m;
    if(h == 5){
      show_end_longer = 120;
    }
    for(int reps=0; reps<show_end_longer; reps++){
      for(int j=0; j<6; j++){
        SR[6] = level[j];
        SR[0]=SR[1]=SR[2]=SR[3]=SR[4]=SR[5]=single_on[h];
        for(int k = 0; k < n; k++){
          int a = floor(trail[k]/36); //0-6 ex 4
          int b = trail[k] - a * 36; //very high #
          int c = floor(b/6);
          if((b - c*6)==j){
            SR[c] = B00000000;
            bitWrite(SR[c], 6-a, 1);
          }
        }
        shift_out();
        delayMicroseconds(1);
        clear_out();
      }
    }
  }
}

void sun(){ // this didn't really work, but its still here
  clear_out();
  for(int i = 0; i < 9999; i++){
    int a = floor(i/80);
    SR[6] = B11011011;
    SR[2] = SR[3] = B00011000;
    if(a%4 > 0){
      SR[1] = SR[4] = B00100100;
    }
    shift_out();
    delayMicroseconds(1);
    clear_out();
    if(a%4 == 2){
      SR[6] = B10111101;
      SR[0]=SR[5] = B01000010;
      shift_out();
      delayMicroseconds(1);
      clear_out();
    }
    SR[6] = B11100111;
    SR[2] = SR[3] = B00111100;
    SR[1] = SR[4] = B00011000;
    shift_out();
    delayMicroseconds(1);
    clear_out();
  }
}

void spiral(int reps){ // a single led lights up at a time, spiraling down the outside of the cube
  for(int n=0; n<reps; n++){ 
    for(int i=0; i<6; i++){
      for(int j=0; j<20; j++){//j = 0-19
        SR[6] = level[i];
        if(j < 6){
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
      delay(20);
      clear_out();
      }
    }
  }
}

void rain(){ // random leds lights up at the top layer and 'fall' to the bottom of the cube
  long int raindrops_x [] = {random(0, 6), random(0, 6), random(0, 6), random(0, 6), random(0, 6), random(0, 6)};
  long int raindrops_z [] = {random(0, 6), random(0, 6), random(0, 6), random(0, 6), random(0, 6), random(0, 6)};
  for(int i=0; i<300; i++){
    for(int j=0; j<36; j++){
      int k = j % 6;
      SR[6] = level[k];
      SR[raindrops_z[k]] = single_on[raindrops_x[k]];
      shift_out();
      delayMicroseconds(1);
      clear_out();
    }
    raindrops_x[5] = raindrops_x[4];
    raindrops_x[4] = raindrops_x[3];
    raindrops_x[3] = raindrops_x[2];
    raindrops_x[2] = raindrops_x[1];
    raindrops_x[1] = raindrops_x[0];
    raindrops_x[0] = random(0, 6);
    raindrops_z[5] = raindrops_z[4];
    raindrops_z[4] = raindrops_z[3];
    raindrops_z[3] = raindrops_z[2];
    raindrops_z[2] = raindrops_z[1];
    raindrops_z[1] = raindrops_z[0];
    raindrops_z[0] = random(0, 6);
  }
}

void displayChars(int letters[], int n){ //give the number values of the letters you want to display. 
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
                          {B00011000, B00000100, B00011100, B00100100, B00100100, B00011000}};//9
  for(int letter=0; letter<n; letter++){
    for(int row=0; row<6; row++){
      for(int reps=0; reps<8; reps++){
        for(int lvl=0; lvl<6; lvl++){
          SR[6] = level[lvl];
          SR[5-row] = alphabet[letters[letter]][5-lvl];
          shift_out();
          delayMicroseconds(1);
          clear_out();
        }
      }  
    }
  }
}

void GrowingCube(int n){ //turns cube on/off starting at a random corner and 'growing' until the cube is on/off
                         //n is the amount of times it loops
  bool previous[] = {};
  for(int a=0; a<n; a++){
    uint8_t lvl = B11111111;
    uint8_t row = B00000000;
    bool corner[] = {random(0,2),random(0,2),random(0,2)};
    for(int i=0; i<7; i++){
      int L = 0;
      if(corner[0] == 0){
        L = 7 - i;
      }
      else{
        L = i;
      }
      bitWrite(lvl, L, 0);
      int r = 0;
      if(corner[1] == 0){
        r = 7 - i;
      }
      else{
        r = i;
      }
      bitWrite(row, r, 1);
      int d = 0;
      if(corner[2] == 0){
        d = 5 - i;
        for(int v=5; v>d; v--){
          SR[v] = row;
        }
      }
      else{
        d = i;
        for(int w=0; w<d; w++){
          SR[w] = row;
        }
      }
      SR[6] = lvl;
      shift_out();
      delay(100);
      clear_out();
    }
    corner[0] = random(0,2);
    corner[1] = random(0,2);
    corner[2] = random(0,2);
    for(int i=6; i>=0; i--){
      int L = 0;
      if(corner[0] == 0){
        L = 7 - i;
      }
      else{
        L = i;
      }
      bitWrite(lvl, L, 1);
      int r = 0;
      if(corner[1] == 0){
        r = 7 - i;
      }
      else{
        r = i;
      }
      bitWrite(row, r, 0);
      int d = 0;
      if(corner[2] == 0){
        d = 5 - i;
        for(int v=5; v>d; v--){
          SR[v+1] = row;
        }
      }
      else{
        d = i;
        for(int w=0; w<d; w++){
          SR[w-1] = row;
        }
      }
      SR[6] = lvl;
      shift_out();
      delay(100);
      clear_out();
    }
  }
}

void frame6x6() { // lights up the frame of the cube
  for (int k = 0; k < 200; k ++) {
    SR[6] = B10111101;
    SR[0] = SR[5] = high;
    SR[1] = SR[2] = SR[3] = SR[4] = B01000010;
    shift_out();
    delayMicroseconds(5);
    clear_out();

    SR[6] = B11000011;
    SR[0] = SR[5] = B01000010;
    shift_out();
    delayMicroseconds(5);
    clear_out();
  }
}

void frame4x4() { //lights up the frame of the center 4x4x4 leds
  for (int k = 0; k < 200; k ++) {
    SR[6] = B11011011;
    SR[1] = SR[4] = B00111100;
    SR[2] = SR[3] = B00100100;
    shift_out();
    delayMicroseconds(5);
    clear_out();
    delayMicroseconds(5);

    SR[6] = B11100111;
    SR[1] = SR[4] = B00100100;
    shift_out();
    delayMicroseconds(5);
    clear_out();
    delayMicroseconds(5);
  }
}

void centerCube() { //lights up the four middle leds
  SR[6] = B11100111;
  SR[2] = SR[3] = B00011000;
  shift_out();
  delay(500);
}

void random2x2() { //lights up a 2x2 cube in a random position on the cube
  for (int k = 0; k < 20; k ++) {
    int SRrandom = random(0, 5);

    SR[6] = LEVEL2[random(0, 5)];
    SR[SRrandom] = SR[SRrandom + 1] = ROW2[random(0, 5)];
    shift_out();
    delay(100);
    clear_out();
  }
}

void random3x3() { //lights up a 3x3 cube in a random position on the cube
  for (int k = 0; k < 20; k ++) {
    int SRrandom = random(0, 4);

    SR[6] = LEVEL3[random(0, 4)];
    SR[SRrandom] = SR[SRrandom + 1] = SR[SRrandom + 2] = ROW3[random(0, 5)];
    shift_out();
    delay(100);
    clear_out();
  }
}

void full(int t) { // turns all the leds on, t is time in milliseconds
  for (int i = 0; i < 6; i ++) {  // this line turns all the rows high
    SR[i] = B11111111;
  }
  SR[6] = B00000000;  //0x00; // this line turns all the levels low
  shift_out();
  delay(t);
}

//////////////////////////////////////////////////////////////////////////////////////////

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
