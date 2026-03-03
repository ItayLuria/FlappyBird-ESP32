#include <SSD1306.h>
#include <Preferences.h>
#include "asset.h"

#define BUTTON_PIN      23
#define BOOT_BUTTON_PIN 0
#define BUZZER_PIN      18

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

#define TUBE_DISTANCE   32
#define TUBE_WIDTH      6
#define PATH_WIDTH      30

SSD1306  display(0x3c, 21, 22);
Preferences preferences;

float tubeX[4];
int bottomTubeHeight[4];

unsigned int score = 0;
unsigned int highScore;
unsigned int gameState = 0;

bool isFlyingUp = false;
bool isBuzzerOn = false;
bool increaseSpeed = false;
bool hasScored[4];

float birdX = 20.00;
float birdY = 28.00;
float speed = 0.01;

unsigned long keyPressTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT);

  preferences.begin("Flappy", false);
  highScore = preferences.getUInt("highScore", 0);
  preferences.end();

  display.init();

  for(int i = 0; i < 4; i++) {
    tubeX[i] = 128 + i * TUBE_DISTANCE;
    bottomTubeHeight[i] = random (8, 32);
  }

  display.flipScreenVertically();
}

void loop() {
  display.clear();

  if(gameState == 0) {  
    birdY = 28.00;
    score = 0;
    speed = 0.01;

    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 4, "Flappy ");

    display.drawXbm(64, 0, Building_width, Building_height, Building);
    display.drawXbm(birdX, birdY, Flappy_width, Flappy_height, Flappy);
    display.setColor(WHITE);
    display.fillRect(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 5);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Press to start");

    for(int i = 0; i < 4; i++) {
      tubeX[i] = 128 + ((i+1) * TUBE_DISTANCE);
      bottomTubeHeight[i] = random(8, 32);
      hasScored[i] = false;
    }

    if(digitalRead(BUTTON_PIN) == LOW) {
      gameState = 1;
      delay(50);
    }
  }

  else if (gameState == 1) {  
    display.setFont(ArialMT_Plain_10);
    display.drawString(3, 0, String(score));
    
    if(digitalRead(BUTTON_PIN) == LOW) {
      keyPressTime = millis();
      isFlyingUp = true;
      isBuzzerOn = true;
    } 

    display.setColor(WHITE);
    display.drawXbm(birdX, birdY, Flappy_width, Flappy_height, Flappy);

    for(int i = 0; i < 4; i++) {
      display.fillRect(tubeX[i], 0, TUBE_WIDTH, bottomTubeHeight[i]);
      display.fillRect(tubeX[i], bottomTubeHeight[i] + PATH_WIDTH, TUBE_WIDTH, SCREEN_HEIGHT - bottomTubeHeight[i] - PATH_WIDTH);
    }
  
    for(int i = 0; i < 4; i++) {
      tubeX[i] -= speed;

      if(tubeX[i] < birdX && !hasScored[i]){
        score++;
        hasScored[i] = true;

        if(score % 10 == 0){
          speed += 0.01;
        }
      }

      if(tubeX[i] + TUBE_WIDTH < 0){
        bottomTubeHeight[i] = random(8, 32);
        tubeX[i] = 128;
        hasScored[i] = false;
      }
    }

    if((keyPressTime + 80) < millis()) {
      isFlyingUp = false;
    }

    if((keyPressTime + 10) < millis()) {
      isBuzzerOn = false;
    }
  
    if(isFlyingUp) {
      birdY -= 0.025;
    } 
    else {
      birdY += 0.015;
    }

    if(isBuzzerOn){
      digitalWrite(BUZZER_PIN,1);
    }
    else{
      digitalWrite(BUZZER_PIN,0);
    }

    if(birdY > 63 || birdY < 0) { 
      digitalWrite(BUZZER_PIN,1);
      delay(200);
      digitalWrite(BUZZER_PIN,0); 
      delay(50);
      digitalWrite(BUZZER_PIN,1);
      delay(50);
      digitalWrite(BUZZER_PIN,0); 
      delay(50);
      digitalWrite(BUZZER_PIN,1);
      delay(50);
      digitalWrite(BUZZER_PIN,0); 

      if(score > highScore){
        highScore = score;
        preferences.begin("Flappy", false);
        preferences.putUInt("highScore", highScore);
        preferences.end();
      }
      gameState = 2;
    }

    for(int i = 0; i < 4; i++){
      if(tubeX[i] <= birdX + 7 && birdX + 7 <= tubeX[i] + 6) {
        if(birdY < bottomTubeHeight[i] || birdY + 8 > bottomTubeHeight[i] + PATH_WIDTH){
          digitalWrite(BUZZER_PIN,1);
          delay(200);
          digitalWrite(BUZZER_PIN,0); 
          delay(50);
          digitalWrite(BUZZER_PIN,1);
          delay(50);
          digitalWrite(BUZZER_PIN,0); 
          delay(50);
          digitalWrite(BUZZER_PIN,1);
          delay(50);
          digitalWrite(BUZZER_PIN,0); 

          if(score > highScore){
            highScore = score;
            preferences.begin("Flappy", false);
            preferences.putUInt("highScore", highScore);
            preferences.end();
          }
          gameState = 2;
          delay(50);
        }
      }
    }

    display.drawRect(0, 0, 128, 64);
  }

  else{ 
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Your score: " + String(score));
    display.drawString(0, 20, "High score: " + String(highScore));

    display.setFont(ArialMT_Plain_10);
    display.drawString(32, 44, "Click to restart");

    display.setFont(ArialMT_Plain_10);
    display.drawString(5, 54, "Click BOOT to reset score");

    if(digitalRead(BUTTON_PIN) == LOW){
      gameState = 0;
      delay(200);
    }

    if(digitalRead(BOOT_BUTTON_PIN) == LOW){
      score = 0;
      highScore = 0;
      preferences.begin("Flappy", false);
      preferences.putUInt("highScore", highScore);
      preferences.end();
    }
  }
  
  display.display();
}