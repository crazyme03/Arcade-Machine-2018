#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

uint16_t colors[] = {
  matrix.Color(255, 255, 255), matrix.Color(0, 255, 0)};
double framerate = 300l;
double delaytime = 1000000/framerate;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix.setTextColor(colors[0]);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}
int w = matrix.width();
double cursory = matrix.width();
double p1 = 0;
double p2 = 0;
unsigned long loopstart = 0;
double ballxmovement = .2;
double ballymovement = 0;
double ballx = 7;
double bally = 7;
double ballmaxychange = .1;
bool gameover = false;
double AA = 1;
//uint16_t color[] = {
//  matrix.Color(255,255,255),};
void loop() {
  loopstart = micros();
  matrix.fillScreen(0);
  // put your main code here, to run repeatedly:
  if(gameover == false){
    for(int i = 0; i < 1 ; i++){
      ballx = ballx + ballxmovement;
      bally = bally + ballymovement;
      matrix.drawPixel(ballx,bally,matrix.Color(0,255,0));
      if(ballx < 0 && (abs((p1+2) - bally)) <= 2){
        ballxmovement = -ballxmovement;
        if(ballymovement < 0){
          ballymovement = ballmaxychange*((-(p1+2) + bally)/2.0);
        }else{
          ballymovement = ballmaxychange*((+(p1+2) - bally)/2.0);
        }
      } else if(ballx < 0){
        gameover = true;
        
      } else if(ballx > 15 && (abs((p2+2) - bally)) <= 2){
        ballxmovement = -ballxmovement;
        if(ballymovement < 0){
          ballymovement = ballmaxychange*(((p1+2) - bally)/2.0);
        }else{
          ballymovement = ballmaxychange*((-(p1+2) + bally)/2.0);
        }
      }else if(ballx > 15){
        gameover = true;
      }
      if(bally < 0 || bally > 15){
        ballymovement = -ballymovement;
      }
    }
    for(int x = 0; x < 16; x++){
      for(int y = 0; y < 16; y++){
        if(x <= (ballx+1) && x >= (ballx-1) && y <= (bally+1) && y >= (bally-1)){
          AA = sqrt(sq(ballx-x) + sq(bally-y));
          AA = (sqrt(2)-AA)/sqrt(2);
        }else{
          AA = 0;
        }
        matrix.drawPixel(x,y,matrix.Color(255*AA,255*AA,255*AA));
      }
    }
    if( digitalRead(2)== false && p1 < 12){
      p1 = p1+.4;
    }else if(digitalRead(3) == false && p1 > 0){
      p1 = p1-.4;
    }
    //matrix.drawFastVLine(0,p1,4,colors[0]);
    if( digitalRead(4)== false && p2 < 12){
      p2 = p2+.4;
    }else if(digitalRead(5) == false && p2 > 0){
      p2 = p2-.4;
    }
    //matrix.drawFastVLine(15,p2,4,colors[0]);
    for(int i = 0; i < 8; i++){
      if(i <= 3){
        matrix.drawPixel(0,p1+i,matrix.Color(255,255,255));
      }else{
        matrix.drawPixel(15,p2+(i-4),matrix.Color(255,255,255));
      }
    }
    Serial.println((delaytime - (micros() - loopstart)));
    Serial.println(delaytime);
    if(delaytime > micros() - loopstart){
      delayMicroseconds(delaytime - (micros() - loopstart));
    }
    matrix.show();
  }else{
    if(digitalRead(7) == false){
      gameover = false;
      ballx = 7;
      bally = 7;
      ballymovement = .1;
      ballxmovement = .1;
      cursory = w;
    }
    matrix.setCursor(cursory,0);
    matrix.print(F("Game Over"));
    matrix.show();
    matrix.setTextColor(matrix.Color(255,0,0));
    cursory = cursory - .1;
    if(cursory < -53){
      cursory = w;
//      color[0] = matrix.color(random(255),random(255),random(255));
    }
    
  }
}
