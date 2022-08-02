#include <Wire.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h> 
#include <time.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

/////////////////////////
//  MENU               //
/////////////////////////

Adafruit_SSD1306 display(128, 64);

//buzzer
#define sound 13

//keys
#define KNAV 12
#define KOK 11
#define K1 10
#define K2 9
#define K3 8
#define K4 7
#define K5 6
#define K6 5
#define K7 4
#define K8 3
#define K9 2

uint64_t kTime = 0;
uint8_t kImage_temp = 0;
uint8_t kImage = 0; //last pressed key

//states
#define MENU 0
#define TTT 1
#define SNAKE 2

uint8_t state = MENU;
uint8_t menu_position = TTT;

void buttonsRoutine(){
  if(kImage != 0){
    kTime = millis();
    return;
  }
  if(millis() - kTime > 100){

    if(!digitalRead(K1)){
      if(kImage_temp == K1){
        kImage_temp = 0;
        kImage = K1;
      }
      else{
        kImage_temp = K1;
      }
      return;
    }
    if(!digitalRead(K2)){
      if(kImage_temp == K2){
        kImage_temp = 0;
        kImage = K2;
      }
      else{
        kImage_temp = K2;
      }
      return;
    }
    if(!digitalRead(K3)){
      if(kImage_temp == K3){
        kImage_temp = 0;
        kImage = K3;
      }
      else{
        kImage_temp = K3;
      }
      return;
    }
    if(!digitalRead(K4)){
      if(kImage_temp == K4){
        kImage_temp = 0;
        kImage = K4;
      }
      else{
        kImage_temp = K4;
      }
      return;
    }
    if(!digitalRead(K5)){
      if(kImage_temp == K5){
        kImage_temp = 0;
        kImage = K5;
      }
      else{
        kImage_temp = K5;
      }
      return;
    }
    if(!digitalRead(K6)){
      if(kImage_temp == K6){
        kImage_temp = 0;
        kImage = K6;
      }
      else{
        kImage_temp = K6;
      }
      return;
    }
    if(!digitalRead(K7)){
      if(kImage_temp == K7){
        kImage_temp = 0;
        kImage = K7;
      }
      else{
        kImage_temp = K7;
      }
      return;
    }
    if(!digitalRead(K8)){
      if(kImage_temp == K8){
        kImage_temp = 0;
        kImage = K8;
      }
      else{
        kImage_temp = K8;
      }
      return;
    }
    if(!digitalRead(K9)){
      if(kImage_temp == K9){
        kImage_temp = 0;
        kImage = K9;
      }
      else{
        kImage_temp = K9;
      }
      return;
    }
    if(!digitalRead(KOK)){
      if(kImage_temp == KOK){
        kImage_temp = 0;
        kImage = KOK;
        delay(100);
      }
      else{
        kImage_temp = KOK;
      }
      return;
    }
    if(!digitalRead(KNAV)){
      if(kImage_temp == KNAV){
        kImage_temp = 0;
        kImage = KNAV;
        delay(100);
      }
      else{
        kImage_temp = KNAV;
      }
      return;
    }

    kTime = millis();
  }
}

void buttonsAction(){
  if (state == MENU){
    //nav hit -> switch menu position
    if (kImage == KNAV){
      tone(sound,8000, 20);
      if (menu_position == TTT){
        menu_position = SNAKE;
      }
      else{
        menu_position = TTT;
      }

      //clear key
      kImage = 0;
    }
    //ok hit -> switch to selected game
    if(kImage == KOK){
      tone(sound,8000, 30);
      if (menu_position == TTT){
        state = TTT;
        display.clearDisplay();
        ttt_setup();
      }
      else{
        state = SNAKE;
        display.clearDisplay();
        snake_setup();
      }
      
      //clear key
      kImage = 0;
    }
  }
  else if(state == TTT){
    //ok hit -> switch to menu
    if(kImage == KOK){
      tone(sound,8000, 30);
      state = MENU;
      display.setTextSize(1);
      display.clearDisplay();
    
      //clear key
      kImage = 0;
    }
  }
  else if(state == SNAKE){
    //ok hit -> switch to menu
    if(kImage == KOK){
      tone(sound,8000, 30);
      state = MENU;
      display.clearDisplay();
      
      //clear key
      kImage = 0;
    }
  }
}

/////////////////////////
//  TTT                //
/////////////////////////

//TTT - 0, GAME - 1, WON - 2, DRAW - 3
uint8_t ttt_state = 0;

uint8_t ttt_fields[9]={};
uint8_t ttt_turn = 0;
uint32_t ttt_t = 0;
uint32_t ttt_t2 = 0;

void ttt_changeState(uint8_t s){
    display.clearDisplay();
    display.display();
    ttt_t = millis();
    ttt_state = s;
}

void ttt_clickKey(uint8_t n){
    display.println(ttt_turn%2==0?'X':'O');
    display.display();
    ttt_fields[n] = ttt_turn%2==0?'X':'O';
    ttt_turn++;
}

void ttt_checkWin(){
    if(ttt_fields[1] == ttt_fields[0] && ttt_fields[1]==ttt_fields[2] && ttt_fields[0]!=0 && ttt_fields[1]!=0 && ttt_fields[2]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[3] == ttt_fields[4] && ttt_fields[4]==ttt_fields[5] && ttt_fields[3]!=0 && ttt_fields[4]!=0 && ttt_fields[5]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[6] == ttt_fields[7] && ttt_fields[7]==ttt_fields[8] && ttt_fields[6]!=0 && ttt_fields[7]!=0 && ttt_fields[8]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[0] == ttt_fields[3] && ttt_fields[3]==ttt_fields[6] && ttt_fields[0]!=0 && ttt_fields[3]!=0 && ttt_fields[6]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[1] == ttt_fields[4] && ttt_fields[4]==ttt_fields[7] && ttt_fields[4]!=0 && ttt_fields[1]!=0 && ttt_fields[7]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[5] == ttt_fields[2] && ttt_fields[2]==ttt_fields[8] && ttt_fields[5]!=0 && ttt_fields[8]!=0 && ttt_fields[2]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[4] == ttt_fields[0] && ttt_fields[4]==ttt_fields[8] && ttt_fields[0]!=0 && ttt_fields[4]!=0 && ttt_fields[8]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }
    else if(ttt_fields[4] == ttt_fields[2] && ttt_fields[4]==ttt_fields[6] && ttt_fields[2]!=0 && ttt_fields[4]!=0 && ttt_fields[6]!=0){
      ttt_changeState(2);
      //play a winner song 
      tone(sound, 1600, 600);
      return;
    }

    //check if draw
    uint8_t notEmpty = 0;
    for(uint8_t i=0;i<9;i++){
      if(ttt_fields[i]!=0) notEmpty++;
    }

    if(notEmpty==9){
      ttt_changeState(3);
      //play a draw song 
      tone(sound, 2200, 400);
    }
}

void ttt_setup(){
  memset(ttt_fields, 0,9);
  ttt_turn = 0;
  ttt_state = 0;
  ttt_t = 0;
  ttt_t2 = millis();
}

void ttt_loop(){
  if(ttt_state == 0){
    display.setCursor(25, 26);
    display.println("Tic Tac Toe");
    display.display(); 

    if(millis() - ttt_t2> 2000){
      ttt_state = 1;
      display.setTextSize(2);
      display.clearDisplay();
    }
  }
  else if(ttt_state==1){
    display.drawLine(0, 22, 128, 22, WHITE);
    display.drawLine(0, 44, 128, 44, WHITE);
    display.drawLine(42, 0, 42, 64, WHITE);
    display.drawLine(86, 0, 86, 64, WHITE);
    display.display();

    ttt_checkWin();

    if(kImage == K1){
      if(ttt_fields[0]==0){
        display.setCursor(16, 4);
        ttt_clickKey(0);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K2){
      if(ttt_fields[1]==0){
        display.setCursor(60, 4);
        ttt_clickKey(1);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K3){
      if(ttt_fields[2]==0){
        display.setCursor(104, 4);
        ttt_clickKey(2);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K4){
      if(ttt_fields[3]==0){
        display.setCursor(16,27);
        ttt_clickKey(3);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K5){
      if(ttt_fields[4]==0){
        display.setCursor(60,27);
        ttt_clickKey(4);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K6){
      if(ttt_fields[5]==0){
        display.setCursor(104, 27);
        ttt_clickKey(5);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K7){
      if(ttt_fields[6]==0){
        display.setCursor(16,50);
        ttt_clickKey(6);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K8){
      if(ttt_fields[7]==0){
        display.setCursor(60, 50);
        ttt_clickKey(7);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
    if(kImage == K9){
      if(ttt_fields[8]==0){
        display.setCursor(104, 50);
        ttt_clickKey(8);
        delay(100);
      }
      else{
        tone(sound, 2000, 100);
      }

      //clear key
      kImage = 0;
    }
  }
  else if(ttt_state==2){
      display.setCursor(40, 26);
      display.print(ttt_turn%2==0?'O':'X');
      display.print(" won");
      display.display();
    
    if(millis() - ttt_t> 3000){
      memset(ttt_fields, 0, 9);
      ttt_turn = 0;
      noTone(sound);

      ttt_changeState(1);
    }
  }
  else if(ttt_state==3){
      display.setCursor(40, 26);
      display.println("Draw");
      display.display();
    
    if(millis() - ttt_t> 3000){
      memset(ttt_fields, 0, 9);
      ttt_turn = 0;
      
      ttt_changeState(1);
    }
  }
}

/////////////////////////
//  SNAKE              //
/////////////////////////

#define UP K2
#define RIGHT K6
#define LEFT K4
#define DOWN K8

#define X 0
#define Y 1

uint8_t snake_blockCount = 0;
uint32_t snake_points = 0;
uint32_t snake_t = 0;
uint32_t snake_t2 = 0;

//1 -> GAME, 2-> GAME OVER
uint8_t snake_state = 0;

uint8_t snake_rectSide = 7;
uint8_t snake_circleRadius = 2;

int8_t snake_coordinateX[17] = {8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120};
int8_t snake_coordinateY[8] = {7,14,21,28,35,42,49,56};

int8_t snake_circleCoordinates[2] = {};

uint8_t snake_direction = UP;
uint32_t snake_move = millis();
uint32_t draw_field = millis();

struct snake_Block{
  int8_t x;
  int8_t y;

  snake_Block(){}

  snake_Block(int8_t xt,int8_t yt){
    x=xt;
    y=yt;
  }

  void setBlock(const snake_Block* b){
    x = b->x;
    y = b->y;
  }
};

snake_Block snake_SNAKE[30];

bool snake_checkIfInsideSnake(uint8_t d, uint8_t value){
  for(uint8_t i = 0; i<snake_blockCount; i++){
    if(d == X){
      if(snake_SNAKE[i].x == value){
        return true;
      }
    }
    else{
      if(snake_SNAKE[i].y == value){
        return true;
      }
    }
  }
  return false;
}

//make tail longer
void snake_addBlock(){
  if(snake_direction == UP){
    snake_SNAKE[snake_blockCount] = snake_Block(snake_SNAKE[snake_blockCount-1].x, snake_SNAKE[snake_blockCount-1].y+snake_rectSide);
  }
  else if(snake_direction == RIGHT){
    snake_SNAKE[snake_blockCount] = snake_Block(snake_SNAKE[snake_blockCount-1].x+snake_rectSide, snake_SNAKE[snake_blockCount-1].y);
  }
  else if(snake_direction == LEFT){
    snake_SNAKE[snake_blockCount] = snake_Block(snake_SNAKE[snake_blockCount-1].x-snake_rectSide, snake_SNAKE[snake_blockCount-1].y);
  }
  else if(snake_direction == DOWN){
    snake_SNAKE[snake_blockCount] = snake_Block(snake_SNAKE[snake_blockCount-1].x, snake_SNAKE[snake_blockCount-1].y-snake_rectSide);
  }
  snake_blockCount++;
}

void snake_drawRect(uint16_t x, uint16_t y, uint8_t color){
  uint16_t leftCorner = x-((snake_rectSide-1)/2);
  uint16_t upperCorner = y-((snake_rectSide-1)/2);

  display.drawRect(leftCorner,upperCorner,snake_rectSide,snake_rectSide,color);
  //display.display();
}

void snake_drawFilledCircle(){
  display.drawCircle(snake_circleCoordinates[0], snake_circleCoordinates[1], snake_circleRadius, BLACK);
  display.fillCircle(snake_circleCoordinates[0], snake_circleCoordinates[1], snake_circleRadius, BLACK);
  
  uint8_t randomX;
  uint8_t randomY;

  do{
    randomX = snake_coordinateX[random(17)];
    randomY = snake_coordinateY[random(8)];
  }while(snake_checkIfInsideSnake(X,randomX) && snake_checkIfInsideSnake(Y, randomY));

  snake_circleCoordinates[0] = randomX;
  snake_circleCoordinates[1] = randomY;

  display.drawCircle(snake_circleCoordinates[0], snake_circleCoordinates[1], snake_circleRadius, WHITE);
  display.fillCircle(snake_circleCoordinates[0], snake_circleCoordinates[1], snake_circleRadius, WHITE);
}

//draw the whole frame
void snake_drawField(){
  //draw snake
  for(uint8_t i = 0; i<snake_blockCount; i++){
    snake_drawRect(snake_SNAKE[i].x, snake_SNAKE[i].y,WHITE);
  }
}

void snake_checkCollision(){
  display.drawCircle(snake_circleCoordinates[0], snake_circleCoordinates[1], snake_circleRadius, WHITE);
  display.fillCircle(snake_circleCoordinates[0], snake_circleCoordinates[1], snake_circleRadius, WHITE);
  display.display();

  if(snake_SNAKE[0].x == snake_circleCoordinates[0] && snake_SNAKE[0].y == snake_circleCoordinates[1]){
    snake_addBlock();
    snake_drawFilledCircle();
    snake_points++;
    tone(sound, 1800, 80);
  }
}

void snake_checkGameOver(){
  for(uint8_t i = 1; i < snake_blockCount; i++){
    if(snake_SNAKE[0].x == snake_SNAKE[i].x && snake_SNAKE[0].y == snake_SNAKE[i].y){
      snake_t = millis();
      snake_state = 2;
      tone(sound, 8000, 60);
      break;
    }
  }
}

void snake_moveSnake(){
  snake_drawRect(snake_SNAKE[snake_blockCount-1].x, snake_SNAKE[snake_blockCount-1].y, BLACK);

  //move segments right
  for(int8_t i = snake_blockCount-2; i>=0; i--){
    snake_SNAKE[i+1].setBlock(&snake_SNAKE[i]);
  }

  //adding segment at the beginning
  if(snake_direction == UP){
    snake_SNAKE[0].setBlock(&snake_SNAKE[1]);
    snake_SNAKE[0].y-=snake_rectSide;

    //check the boundaries
    if(snake_SNAKE[0].y <= 0){
      snake_SNAKE[0].y = 63;
    }
  }
  else if(snake_direction == RIGHT){
    snake_SNAKE[0].setBlock(&snake_SNAKE[1]);
    snake_SNAKE[0].x+=snake_rectSide;

    if(snake_SNAKE[0].x >= 127){
      snake_SNAKE[0].x = 1;
    }
  }
  else if(snake_direction == LEFT){
    snake_SNAKE[0].setBlock(&snake_SNAKE[1]);
    snake_SNAKE[0].x-=snake_rectSide;

    if(snake_SNAKE[0].x <= 1){
      snake_SNAKE[0].x = 127;
    }
  }
  else if(snake_direction == DOWN){
    snake_SNAKE[0].setBlock(&snake_SNAKE[1]);
    snake_SNAKE[0].y+=snake_rectSide;

    if(snake_SNAKE[0].y >= 63){
      snake_SNAKE[0].y = 0;
    }
  }
}

void snake_reset(){
  snake_direction = UP;
  memset(snake_SNAKE, 0, 30*sizeof(snake_Block));
  snake_blockCount = 0;
  snake_SNAKE[snake_blockCount] =  snake_Block(22,21);
  snake_blockCount++;
  snake_addBlock();
  snake_addBlock();
  snake_points = 0;
  
  snake_t2 = millis();
  snake_move = millis();
  draw_field = millis();
}

void snake_setup(){
  //generate random circle coordinates
  randomSeed(analogRead(A0));
  snake_circleCoordinates[0] = snake_coordinateX[random(17)];
  snake_circleCoordinates[1] = snake_coordinateY[random(8)];

  snake_reset();
}

void snake_loop(){
  if(snake_state ==0){
    display.setCursor(45, 26);
    display.println("Snake");
    display.display(); 

    if(millis() - snake_t2> 2000){
      snake_state = 1;
      display.clearDisplay();
    }
  }
  else if(snake_state == 1){
    snake_checkCollision();
    snake_checkGameOver();

    //move snake
    if(millis() - snake_move >170){
      snake_move = millis();
      snake_moveSnake();
    }
    
    //refresh screen
    if(millis() - draw_field > 60){
      draw_field = millis();
      snake_drawField();
    }

    if(kImage == UP){
      if(snake_direction == DOWN) snake_direction = DOWN;
      else snake_direction = UP;

      //clear key
      kImage = 0;
    }
    if(kImage == RIGHT){
      if(snake_direction == LEFT) snake_direction = LEFT;
      else snake_direction = RIGHT;

      //clear key
      kImage = 0;
    }
    if(kImage == LEFT){
      if(snake_direction == RIGHT) snake_direction = RIGHT;
      else snake_direction = LEFT;
      
      //clear key
      kImage = 0;
    }
    if(kImage == DOWN){
      if(snake_direction == UP) snake_direction = UP;
      else snake_direction = DOWN;
      
      //clear key
      kImage = 0;
    }
  }
  else if(snake_state == 2){
    display.clearDisplay();
    display.setCursor(30, 26);
    display.print("Pkt: ");
    display.print(snake_points);
    display.display(); 

    if(millis() - snake_t> 4000){
        snake_state = 1;
        display.clearDisplay();
        snake_reset();
        snake_t = millis();
    }
  }
}

/////////////////////////
//  MAIN               //
/////////////////////////

const uint8_t logo[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x30, 0xe0, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x78, 0xe0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x78, 0xe0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x30, 0xe0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x06, 0x02, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x40, 0x00, 0x00, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x20, 0x00, 0x00, 0x30, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x18, 0x00, 0x00, 0xc0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x07, 0x80, 0x0e, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x7f, 0xf0, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
  Wire.begin();
  Serial.begin(9600);

  //init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
      Serial.println("Failed to init OLED...\r\n");
      for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //logo
  display.drawBitmap(0, 0, logo, 128, 64, WHITE);
  display.display();
  delay(2000);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //init keys
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  pinMode(K3, INPUT_PULLUP);
  pinMode(K4, INPUT_PULLUP);
  pinMode(K5, INPUT_PULLUP);
  pinMode(K6, INPUT_PULLUP);
  pinMode(K7, INPUT_PULLUP);
  pinMode(K8, INPUT_PULLUP);
  pinMode(K9, INPUT_PULLUP);
  pinMode(KOK, INPUT_PULLUP);
  pinMode(KNAV, INPUT_PULLUP);

  kTime = millis();
}

void loop() {
  //keys
  buttonsRoutine();
  buttonsAction();
  
  //menu
  if (state == MENU){
    
    display.clearDisplay();
    display.setCursor(50, 8);
    display.println("MENU");
    display.setCursor(30, 26);
    display.println("TIC TAC TOE");
    display.setCursor(45, 44);
    display.println("SNAKE");

    //draw underline
    if (menu_position == TTT){
      display.drawFastHLine(30, 36, 68, WHITE);
    }
    else{
      display.drawFastHLine(45, 54, 30, WHITE);
    }
    display.display();
  }
  else if(state == TTT){
    ttt_loop();
  }
  else if(state == SNAKE){
    snake_loop();
  }

}
