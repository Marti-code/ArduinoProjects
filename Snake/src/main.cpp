#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>
#include <SoftwareSerial.h>
#include "Adafruit_SSD1306/Adafruit_SSD1306.h"

#define UP 2
#define RIGHT 3
#define LEFT 4
#define DOWN 5

uint8_t blockCount = 0;
uint8_t direction = UP;
uint8_t rectSide = 7;
uint8_t move = millis();

SoftwareSerial serial(10,11);
Adafruit_SSD1306 display(128, 64);

struct Block{
  int8_t x;
  int8_t y;

  Block(){}

  Block(int8_t xt,int8_t yt){
    x=xt;
    y=yt;
  }

  void setBlock(const Block* b){
    x = b->x;
    y = b->y;
  }
};

Block SNAKE[20];

//make tail longer
void addBlock(){
  if(direction == UP){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x, SNAKE[blockCount-1].y+7);
  }
  else if(direction == RIGHT){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x+7, SNAKE[blockCount-1].y);

  }
  else if(direction == LEFT){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x-7, SNAKE[blockCount-1].y);

  }
  else if(direction == DOWN){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x, SNAKE[blockCount-1].y-7);

  }
  blockCount++;
}

void drawRect(uint16_t x, uint16_t y){
  uint16_t leftCorner = x-((rectSide-1)/2);
  uint16_t upperCorner = y-((rectSide-1)/2);

  display.drawRect(leftCorner,upperCorner,rectSide,rectSide,WHITE);
  display.display();
}

//draw frame
void drawField(){
  display.clearDisplay();
  for(uint8_t i = 0; i<blockCount; i++){
    drawRect(SNAKE[i].x, SNAKE[i].y);
  }
}

void moveSnake(){
  //move segments right
  for(int8_t i = blockCount-2; i>=0; i--){
    SNAKE[i+1].setBlock(&SNAKE[i]);
  }

  //adding segment at the beginning
  if(direction == UP){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].y-=7;

    //check the boundaries
    if(SNAKE[0].y == 0){
      SNAKE[0].y = 63;
    }
  }
  else if(direction == RIGHT){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].x+=7;

    if(SNAKE[0].x == 127){
      SNAKE[0].x = 1;
    }
  }
  else if(direction == LEFT){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].x-=7;

    if(SNAKE[0].x == 1){
      SNAKE[0].x = 127;
    }
  }
  else if(direction == DOWN){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].y+=7;

    if(SNAKE[0].y == 63){
      SNAKE[0].y = 0;
    }
  }
}

void setup() {
  Wire.begin();
  serial.begin(9600);

  //init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
      Serial.println("Failed to init OLED...\r\n");
      for(;;);
  }

  memset(SNAKE, 0, 20*sizeof(Block));
  display.clearDisplay();

  //important, don't change
  SNAKE[blockCount] =  Block(22,21);
  blockCount++;
  addBlock();
  addBlock();
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  move = millis();
}

void loop() {
  drawField();

  //refresh screen every second
  if(millis() - move > 1000){
    moveSnake();
    move = millis();
  }

  if(!digitalRead(UP)){
    if(direction == DOWN){
      direction = DOWN;
    }
    else{
      direction = UP;
    }
    while(!digitalRead(UP));
  }
  if(!digitalRead(RIGHT)){
    if(direction == LEFT){
      direction = LEFT;
    }
    else{
      direction = RIGHT;
    }
    while(!digitalRead(RIGHT));
  }
  if(!digitalRead(LEFT)){
    if(direction == RIGHT){
      direction = RIGHT;
    }
    else{
      direction = LEFT;
    }
    while(!digitalRead(LEFT));
  }
  if(!digitalRead(DOWN)){
    if(direction == UP){
      direction = UP;
    }
    else{
      direction = DOWN;
    }
    while(!digitalRead(DOWN));
  }
}