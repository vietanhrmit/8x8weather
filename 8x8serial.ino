const int latchPin = 8;//Pin connected to ST_CP of 74HC595
const int clockPin = 12;//Pin connected to SH_CP of 74HC595
const int dataPin = 11; //Pin connected to DS of 74HC595
const int delay_ms = 200; //higher delay -> slower blink
String py_cmd = "";     //get input from python
int w_mode = 1; //display weather info
int t_mode = 0; //display temperature info
int p_mode = 0; //display air pollution info
int c_mode = 0; //show an empty screen since it's "off"
float timecount = 0;
//screens - numbers and weather icons
int nums[][8] = {
  {0xFF, 0xC1, 0xBE, 0xBE, 0xBE, 0xC1, 0xFF, 0xFF,}, //0
  {0xFF, 0xDF, 0xDF, 0x80, 0xFF, 0xFF, 0xFF, 0xFF,}, //1
  {0xFF, 0xDC, 0xBC, 0xBA, 0xB6, 0xCE, 0xFF, 0xFF,}, //2
  {0xFF, 0xDD, 0xBE, 0xB6, 0xB6, 0xC9, 0xFF, 0xFF,}, //3
  {0xFB, 0xF3, 0xEB, 0xDB, 0x80, 0xFB, 0xFF, 0xFF,}, //4
  {0xFF, 0x8D, 0xAE, 0xAE, 0xAE, 0xB1, 0xFF, 0xFF,}, //5
  {0xFF, 0xC1, 0x96, 0xB6, 0xB6, 0xD9, 0xFF, 0xFF,}, //6
  {0xFF, 0xBF, 0xBC, 0xB3, 0xAF, 0x9F, 0xFF, 0xFF,}, //7
  {0xFF, 0xC9, 0xB6, 0xB6, 0xB6, 0xC9, 0xFF, 0xFF,}, //8
  {0xFF, 0xCD, 0xB6, 0xB6, 0xB4, 0xC1, 0xFF, 0xFF,}, //9
};
String icons[]        = {"thunderstorm", "rain", "snow","clear_sky", "clouds", "mist",};
int thunderstorm[][8] = {
  {0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F,}, 
  {0x1F, 0x1F, 0x3A, 0x31, 0x2B, 0x3F, 0x1F, 0x1F,}, 
};
int rain[][8] = {
  {0x1F, 0x1F, 0x3F, 0x33, 0x3F, 0x3F, 0x10, 0x1F,},
  {0x1F, 0x10, 0x3F, 0x3F, 0x33, 0x3F, 0x1F, 0x1F,},
  };
int snow[][8] ={
  {0xE4, 0xFB, 0xFB, 0xDF, 0xDF, 0x27, 0xDF, 0xDF,},
  {0xDF, 0x27, 0xDF, 0xDF, 0xFF, 0xFB, 0xFB, 0xE4,},
};
int clear_sky[][8] ={
  {0xFF, 0xFF, 0xFF, 0xC7, 0xBB, 0xBB, 0xBB, 0xC7,},
  {0xFF, 0xC7, 0xBB, 0xBB, 0xBB, 0xC7, 0xFF, 0xFF,},
};
int clouds[][8] = {
  {0xAF, 0x6F, 0x6F, 0x6E, 0xAD, 0xCD, 0xFD, 0xFE,},
  {0xF6, 0xF6, 0xF6, 0xFA, 0xFC, 0xCF, 0xAF, 0x6F,},
};
int mist[][8] ={
  {0xFA, 0xDA, 0xDE, 0x5E, 0x57, 0x57, 0x57, 0x57,},
  {0xEA, 0xEA, 0xFA, 0xFA, 0xDE, 0xDE, 0x5F, 0x57,},
};
int screen_off[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
int temp_weather[][8] = {
  {0xFF, 0xFF, 0xFF, 0xC7, 0xBB, 0xBB, 0xBB, 0xC7,},
  {0xFF, 0xC7, 0xBB, 0xBB, 0xBB, 0xC7, 0xFF, 0xFF,},
};
int temp_temp[32] = {                             /*show the number 66*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*empty the screen to start a number*/
  0xFF, 0xC1, 0x96, 0xB6, 0xB6, 0xD9, 0xFF, 0xFF,
  0xFF, 0xC1, 0x96, 0xB6, 0xB6, 0xD9, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*reset the screen to end a number*/
};
int temp_pol[8] = {0xFF, 0xC1, 0xBE, 0xBE, 0xBE, 0xC1, 0xFF, 0xFF,}; //temporarily display 0

void setup ()
{
  //set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(1);
}
void copy(int dst[], int src[], int len){
  for (int i = 0; i<len; i++){
    dst[i] = src[i];
  }
}
void copy2d(int dst[][8], int src[][8]){ //only used to copy weather arrays
  for (int i = 0; i<2; i++){
    for (int j = 0; j<8; j++){
      dst[i][j] = src[i][j];
    }
  }
}
void loop()
{ 
  //w: Blink weather display. Example: w0 -> blink thunderstorm
  //t: Scroll temperature. Example: t23 -> scroll 2 and 3
  //p: Display air pollution level. Example: p5 -> display 5 (very bad)
  while (!Serial.available());
  py_cmd = Serial.readStringUntil('\n');
  if ((py_cmd[0]==(char('c'))) || (py_cmd[0]==(char('C')))){
    w_mode = 0;
    t_mode = 0;
    p_mode = 0;
    c_mode = 1;
  }
  else if ((py_cmd[0]==(char('w'))) || (py_cmd[0]==(char('W')))) {
    w_mode = 1;
    t_mode = 0;
    p_mode = 0;
    c_mode = 0;
    int wcode = int(py_cmd[1]) - 48; //convert char to its int equivalent
    switch(wcode){
      case 0:
        copy2d(temp_weather, thunderstorm);
        break;
      case 1:
        copy2d(temp_weather, rain);
        break;
      case 2:
        copy2d(temp_weather, snow);
        break;
      case 3:
        copy2d(temp_weather, clear_sky);
        break;
      case 4:
        copy2d(temp_weather, clouds);
        break;
      case 5:
        copy2d(temp_weather, mist);
        break;
    }
    //Serial.println("w = 1");
  }
  else if ((py_cmd[0]==(char('t'))) || (py_cmd[0]==(char('T')))) {
    w_mode = 0;
    t_mode = 1;
    p_mode = 0;
    c_mode = 0;
    int t_1 = int(py_cmd[1]) - 48;
    int t_2 = int(py_cmd[2]) - 48;
    int temp_container[4][8];
    copy(temp_container[0], screen_off, 8);
    copy(temp_container[1], nums[t_1],8);
    copy(temp_container[2], nums[t_2],8);
    copy(temp_container[3], screen_off,8);
    int new_temp[32];
    int temp_count = 0;
    for (int h = 0; h < 4; h++){
      for (int f = 0; f < 8; f++){
        new_temp[temp_count] = temp_container[h][f];
        temp_count ++;
      }
    }
    copy(temp_temp, new_temp, 32);
  }
  else if ((py_cmd[0]==(char('p'))) || (py_cmd[0]==(char('P')))) {
    w_mode = 0;
    t_mode = 0;
    p_mode = 1;
    c_mode = 0;
    int pol_code = int(py_cmd[1]) - 48;
    copy(temp_pol, nums[pol_code], 8);
    //Serial.println("p = 1");
  }
  while (c_mode == 1){
    still(screen_off);
  }
  while (w_mode == 1) {
    swap(temp_weather); //swap 6 and 9
  }
  while (t_mode == 1) {
    scroll_rl(temp_temp); //scroll 66
  }
  while (p_mode == 1) {
    still(temp_pol); //display number 4
  }
}
void still(int datalist[]) {
  int dat = 0x01;
  for (int n = 0; n < 8; n++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~datalist[n]); //flip the bits
    shiftOut(dataPin, clockPin, MSBFIRST, ~dat); //control COL of dot matrix
    digitalWrite(latchPin, HIGH);
    dat = dat << 1;
  }
}

void swap(int arr[][8]) {
  timecount += 1;
  if (timecount < delay_ms ) {
    still(arr[0]);
  }
  else if (timecount < 1) {

  }
  else if (timecount < delay_ms *2) {
    still(arr[1]);
  }
  else if (timecount < 1) {

  }
  else {
    timecount = 0;
  }
}
void scroll_rl(int arr[])
{
  int arr_len = sizeof(arr) / sizeof(arr[0]);
  for (int n = 0; n < 24; n++)
  {
    for (int t = 0; t < 10; t ++) //Show repeated 10 times
    {
      int dat = 0x01;
      for (int rows = n; rows < 8 + n; rows++)
      {
        shiftOut(dataPin, clockPin, MSBFIRST, ~arr[rows]); //control ROW of dot matrix
        shiftOut(dataPin, clockPin, MSBFIRST, ~dat); //control COL of dot matrix
        //return the latch pin high to signal chip that it
        //no longer needs to listen for information
        digitalWrite(latchPin, HIGH); //pull the latchPin to save the data
        //delay(2); //wait for a microsecond. Higher value -> slower scroll
        digitalWrite(latchPin, LOW); //ground latchPin and hold low for as long as you are transmitting
        dat = dat << 1;
        delay(2); //wait for a microsecond
      }
    }
  }
}
