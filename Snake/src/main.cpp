#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>
#include <SoftwareSerial.h>
#include <stdlib.h> 
#include <time.h> 

#include "Adafruit_SSD1306/Adafruit_SSD1306.h"

#define UP 2
#define RIGHT 3
#define LEFT 4
#define DOWN 5

uint8_t blockCount = 0;
uint32_t points = 0;
uint32_t t = 0;
uint32_t t2 = 0;

//1 -> GAME, 2-> GAME OVER
uint8_t state = 0;

uint8_t rectSide = 7;
uint8_t circleRadius = 2;

int8_t coordinateX[17] = {8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120};
int8_t coordinateY[8] = {7,14,21,28,35,42,49,56};

int8_t circleCoordinates[2] = {};

uint8_t direction = UP;
uint32_t move = millis();

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

enum Dim{
  x = 0,
  y = 1
};

bool checkIfInsideSnake(Dim d, uint8_t value){
  for(uint8_t i = 0; i<blockCount; i++){
    if(d == x){
      if(SNAKE[i].x == value){
        return true;
      }
    }
    else{
      if(SNAKE[i].y == value){
        return true;
      }
    }
  }
  return false;
}

//make tail longer
void addBlock(){
  if(direction == UP){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x, SNAKE[blockCount-1].y+rectSide);
  }
  else if(direction == RIGHT){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x+rectSide, SNAKE[blockCount-1].y);
  }
  else if(direction == LEFT){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x-rectSide, SNAKE[blockCount-1].y);
  }
  else if(direction == DOWN){
    SNAKE[blockCount] = Block(SNAKE[blockCount-1].x, SNAKE[blockCount-1].y-rectSide);
  }
  blockCount++;
}

void drawRect(uint16_t x, uint16_t y, uint8_t color){
  uint16_t leftCorner = x-((rectSide-1)/2);
  uint16_t upperCorner = y-((rectSide-1)/2);

  display.drawRect(leftCorner,upperCorner,rectSide,rectSide,color);
  //display.display();
}

void drawFilledCircle(){
  display.drawCircle(circleCoordinates[0], circleCoordinates[1], circleRadius, BLACK);
  display.fillCircle(circleCoordinates[0], circleCoordinates[1], circleRadius, BLACK);
  //display.display();

  uint8_t randomX;
  uint8_t randomY;

  do{
    randomX = coordinateX[random(17)];
    randomY = coordinateY[random(8)];
  }while(checkIfInsideSnake(x,randomX) || checkIfInsideSnake(y, randomY));

  circleCoordinates[0] = randomX;
  circleCoordinates[1] = randomY;

  display.drawCircle(circleCoordinates[0], circleCoordinates[1], circleRadius, WHITE);
  display.fillCircle(circleCoordinates[0], circleCoordinates[1], circleRadius, WHITE);
  //display.display();
}

//draw the whole frame
void drawField(){
  //draw snake
  for(uint8_t i = 0; i<blockCount; i++){
    drawRect(SNAKE[i].x, SNAKE[i].y,WHITE);
  }
}

void checkCollision(){
  display.drawCircle(circleCoordinates[0], circleCoordinates[1], circleRadius, WHITE);
  display.fillCircle(circleCoordinates[0], circleCoordinates[1], circleRadius, WHITE);
  display.display();

  if(SNAKE[0].x == circleCoordinates[0] && SNAKE[0].y == circleCoordinates[1]){
    addBlock();
    drawFilledCircle();
    points++;
  }
}

void checkGameOver(){
  for(uint8_t i = 1; i < blockCount; i++){
    if(SNAKE[0].x == SNAKE[i].x && SNAKE[0].y == SNAKE[i].y){
      t = millis();
      state = 2;
      break;
    }
  }
}

void moveSnake(){
  drawRect(SNAKE[blockCount-1].x, SNAKE[blockCount-1].y, BLACK);

  //move segments right
  for(int8_t i = blockCount-2; i>=0; i--){
    SNAKE[i+1].setBlock(&SNAKE[i]);
  }

  //adding segment at the beginning
  if(direction == UP){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].y-=rectSide;

    //check the boundaries
    if(SNAKE[0].y <= 0){
      SNAKE[0].y = 63;
    }
  }
  else if(direction == RIGHT){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].x+=rectSide;

    if(SNAKE[0].x >= 127){
      SNAKE[0].x = 1;
    }
  }
  else if(direction == LEFT){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].x-=rectSide;

    if(SNAKE[0].x <= 1){
      SNAKE[0].x = 127;
    }
  }
  else if(direction == DOWN){
    SNAKE[0].setBlock(&SNAKE[1]);
    SNAKE[0].y+=rectSide;

    if(SNAKE[0].y >= 63){
      SNAKE[0].y = 0;
    }
  }
}

void reset(){
  direction = UP;
  memset(SNAKE, 0, 20*sizeof(Block));
  blockCount = 0;
  SNAKE[blockCount] =  Block(22,21);
  blockCount++;
  addBlock();
  addBlock();
  points = 0;
}

void setup() {
  Wire.begin();
  serial.begin(9600);

  //init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
      serial.println("Failed to init OLED...\r\n");
      for(;;);
  }

  memset(SNAKE, 0, 20*sizeof(Block));
  display.clearDisplay();

  display.setCursor(30, 26);
  display.setTextSize(2);
  display.setTextColor(WHITE);

  //init snake
  SNAKE[blockCount] =  Block(22,21); //important, don't change the coordinates
  blockCount++;
  addBlock();
  addBlock();
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  
  //generate random circle coordinates
  randomSeed(analogRead(A0));
  circleCoordinates[0] = coordinateX[random(17)];
  circleCoordinates[1] = coordinateY[random(8)];

  t2 = millis();
  move = millis();
}

void loop() {
  if(state ==0){
    display.setCursor(30, 26);
    display.println("Snake");
    display.display(); 

    if(millis() - t2> 3000){
      state = 1;
      display.clearDisplay();
    }
  }
  else if(state == 1){
    checkCollision();
    checkGameOver();

    //refresh screen every second
    if(millis() - move >150){
      move = millis();
      drawField();
      moveSnake();
    }

    if(!digitalRead(UP)){
      if(direction == DOWN) direction = DOWN;
      else direction = UP;

      while(!digitalRead(UP));
    }
    if(!digitalRead(RIGHT)){
      if(direction == LEFT) direction = LEFT;
      else direction = RIGHT;

      while(!digitalRead(RIGHT));
    }
    if(!digitalRead(LEFT)){
      if(direction == RIGHT) direction = RIGHT;
      else direction = LEFT;
      
      while(!digitalRead(LEFT));
    }
    if(!digitalRead(DOWN)){
      if(direction == UP) direction = UP;
      else direction = DOWN;
      
      while(!digitalRead(DOWN));
    }
  }
  else if(state == 2){
    display.clearDisplay();
    display.setCursor(30, 26);
    display.print("Pkt: ");
    display.print(points);
    display.display(); 

    if(millis() - t> 5000){
        state = 1;
        display.clearDisplay();
        reset();
        t = millis();
    }
  }
}