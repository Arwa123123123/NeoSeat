# NeoSeat

**AI-Powered Adaptive Wheelchair Seat Prototype**

NeoSeat is an adaptive wheelchair-seat concept designed to improve seating stability and comfort during motion. The system senses movement, classifies its intensity, and adjusts a bench-scale seat platform in response.

## How It Works

1. **Motion Sensing**  
   An MPU6050 accelerometer and gyroscope detect motion intensity and tilt direction.

2. **ML Classification**  
   A lightweight on-device classifier categorizes motion into:
   - Stable
   - Mild
   - Strong

3. **Adaptive Response**  
   An ESP32 processes the sensor data and determines the response intensity and direction.

4. **Seat Adjustment**  
   An MG996R servo moves the one-axis platform in the opposite direction of the detected tilt.

## Hardware

- ESP32
- MPU6050
- MG996R servo
- One-axis hinged platform
- External 5V servo power supply
- Memory foam and honeycomb gel layer in the proposed seat design

## Current Prototype

The current bench-scale prototype demonstrates:

- Real-time motion sensing
- Stable / Mild / Strong motion classification
- Direction-aware servo response
- Integrated sensor-to-actuator operation

## Current Development Stage

**Working bench-scale prototype**

The prototype is intended to demonstrate the sensing, classification, and actuation concept. It is not currently designed or validated for human load, clinical use, or road use.

## Project

**Project:** NeoSeat  
**Team:** EquiMotion  
**Project Number:** 4  
**Track:** Everyday Life  
**Category:** Individuals  

AI Hackathon for People with Disabilities 2026 — KSCDR
