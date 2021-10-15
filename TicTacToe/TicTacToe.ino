#include <Wire.h>
#include <string.h>
#include <stdint.h>
#include <Adafruit_SSD1306.h>

#define K0 9
#define K1 8
#define K2 7
#define K3 6
#define K4 5
#define K5 4
#define K6 3
#define K7 2
#define K8 10
#define sound 12

Adafruit_SSD1306 display(128, 64);

//GAME - 1, WON - 2, DRAW - 3
uint8_t state = 1; 

uint8_t fields[9]={};
uint8_t turn = 0;
uint32_t t = 0;

void changeState(uint8_t s){
    display.clearDisplay();
    display.display();
    t = millis();
    state = s;
}

void clickKey(uint8_t n){
    display.println(turn%2==0?'X':'O');
    display.display();
    fields[n] = turn%2==0?'X':'O';
    turn++;
}

void checkWin(){
    if(fields[1] == fields[0] && fields[1]==fields[2] && fields[0]!=0 && fields[1]!=0 && fields[2]!=0){
      changeState(2);
      return;
    }
    else if(fields[3] == fields[4] && fields[4]==fields[5] && fields[3]!=0 && fields[4]!=0 && fields[5]!=0){
      changeState(2);
      return;
    }
    else if(fields[6] == fields[7] && fields[7]==fields[8] && fields[6]!=0 && fields[7]!=0 && fields[8]!=0){
      changeState(2);
      return;
    }
    else if(fields[0] == fields[3] && fields[3]==fields[6] && fields[0]!=0 && fields[3]!=0 && fields[6]!=0){
      changeState(2);
      return;
    }
    else if(fields[1] == fields[4] && fields[4]==fields[7] && fields[4]!=0 && fields[1]!=0 && fields[7]!=0){
      changeState(2);
      return;
    }
    else if(fields[5] == fields[2] && fields[2]==fields[8] && fields[5]!=0 && fields[8]!=0 && fields[2]!=0){
      changeState(2);
      return;
    }
    else if(fields[4] == fields[0] && fields[4]==fields[8] && fields[0]!=0 && fields[4]!=0 && fields[8]!=0){
      changeState(2);
      return;
    }
    else if(fields[4] == fields[2] && fields[4]==fields[6] && fields[2]!=0 && fields[4]!=0 && fields[6]!=0){
      changeState(2);
      return;
    }

    //check if draw
    uint8_t notEmpty = 0;
    for(uint8_t i=0;i<9;i++){
      if(fields[i]!=0) notEmpty++;
    }

    if(notEmpty==9){
      changeState(3);
    }
}

void setup() {
  memset(fields, 0, 9);
  Wire.begin();
  Serial.begin(9600);

  //init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
      Serial.println("Failed to init OLED...\r\n");
      for(;;);
  }
  Serial.println("Screen init ok");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.clearDisplay();
  display.setCursor(30, 26);
  display.println("Tic tac toe");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(2);

  //init keys
  pinMode(K0, INPUT_PULLUP);
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  pinMode(K3, INPUT_PULLUP);
  pinMode(K4, INPUT_PULLUP);
  pinMode(K5, INPUT_PULLUP);
  pinMode(K6, INPUT_PULLUP);
  pinMode(K7, INPUT_PULLUP);
  pinMode(K8, INPUT_PULLUP);
  pinMode(sound, OUTPUT);
}

void loop() {
  
    if(state==1){
      display.drawLine(0, 22, 128, 22, WHITE);
      display.drawLine(0, 44, 128, 44, WHITE);
      display.drawLine(42, 0, 42, 64, WHITE);
      display.drawLine(86, 0, 86, 64, WHITE);
      display.display();
  
      checkWin();

      if(!digitalRead(K0)){
        if(fields[0]==0){
          display.setCursor(16, 4);
          clickKey(0);
        }
        else{
          tone(sound, 1000, 200);
        }
        
        while(!digitalRead(K0));
      }
      if(!digitalRead(K1)){
          if(fields[1]==0){
            display.setCursor(60, 4);
            clickKey(1);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K1));
      }
      if(!digitalRead(K2)){
          if(fields[2]==0){
            display.setCursor(104, 4);
            clickKey(2);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K2));
      }
      if(!digitalRead(K3)){
        if(fields[3]==0){
            display.setCursor(16,27);
            clickKey(3);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K3));
      }
      if(!digitalRead(K4)){
        if(fields[4]==0){
            display.setCursor(60,27);
            clickKey(4);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K4));
      }
      if(!digitalRead(K5)){
        if(fields[5]==0){
            display.setCursor(104, 27);
            clickKey(5);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K5));
      }
      if(!digitalRead(K6)){
        if(fields[6]==0){
            display.setCursor(16,50);
            clickKey(6);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K6));
      }
      if(!digitalRead(K7)){
        if(fields[7]==0){
            display.setCursor(60, 50);
            clickKey(7);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K7));
      }
      if(!digitalRead(K8)){
        if(fields[8]==0){
            display.setCursor(104, 50);
            clickKey(8);
          }
          else{
            tone(sound, 1000, 200);
          }
          
          while(!digitalRead(K8));
      }
    }
    else if(state==2){
        display.setCursor(35, 26);
        display.print(turn%2==0?'O':'X');
        display.print(" won");
        display.display();
      
        //play a winner song 
        tone(sound, 600, 2000);
      
      if(millis() - t> 5000){
        memset(fields, 0, 9);
        turn = 0;
        noTone(sound);

        changeState(1);
      }
    }
    else if(state==3){
        display.setCursor(40, 26);
        display.println("Draw");
        display.display();
      
      if(millis() - t> 5000){
        memset(fields, 0, 9);
        turn = 0;
        
        changeState(1);
      }
    }
}
