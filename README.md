# ESP32 Flappy Bird

## Overview
This project is an implementation of Flappy Bird on the **ESP32** using the **Arduino framework**. It is designed to run on an **SSD1306 OLED display**.

## Video Demo
<div align="center">
  <video src="https://github.com/user-attachments/assets/f719393d-b9db-4083-ade1-38d7253682c6" width="300" controls>
  </video>
</div>

## Features
- **Controls:** flight mechanics using a physical push button (GPIO 23).
- **Graphics:** visuals optimized for a 128x64 SSD1306 display.
- **High Score:** Your best score is saved to the ESP32's flash memory.
- **Score Reset:** Press the **BOOT** button (GPIO 0) to reset the high score.
- **Audio:** Integrated buzzer (GPIO 18) for jump sounds and game-over sound effects.

## Hardware Requirements
- **ESP32** Development Board
- **SSD1306 OLED Display** (I2C: SCL=22, SDA=21)
- **Push Button** (Connected to GPIO 23)
- **Buzzer** (Connected to GPIO 18)

## Installation
1. Clone this repository.
2. Open `Flappy_bird.ino` in **Arduino IDE**.
3. Select board **ESP32 Dev Module**.
4. Click **Upload**.

## Wiring Reference
| Component | ESP32 Pin |
| :--- | :--- |
| OLED SCL | GPIO 22 |
| OLED SDA | GPIO 21 |
| Button | GPIO 23 |
| Buzzer | GPIO 18 |

