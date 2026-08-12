# 03 - LED Blink

## Overview

A basic ESP32-S3 project controlling an external LED using GPIO 2.

## Circuit

![LED blink circuit](blink-circuit.jpg)

## Hardware

- ESP32-S3-WROOM-1 development board
- LED
- 220Ω resistor
- Breadboard
- Jumper wires

## How it works

GPIO 2 is configured as an output. The program sets GPIO 2 HIGH
for one second and LOW for one second, causing the LED to blink.

## Concepts

- GPIO
- Digital output
- ESP-IDF
- FreeRTOS task delays
- Basic LED circuit
