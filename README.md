#  Soil Moisture Based Automatic Plant Watering System with GSM Alert using ARM7 LPC2129

## Project Overview

This project is designed to automate plant watering using the ARM7 LPC2129 microcontroller. The system uses a soil moisture sensor to monitor soil conditions and automatically controls a water pump.

A GSM module sends SMS notifications to the owner whenever the soil condition changes. An LCD displays the current soil status and watering condition.

## Components Used

- ARM7 LPC2129 Microcontroller
- Soil Moisture Sensor
- GSM Module (SIMCom A7670C)
- 16×2 LCD Display
- DC Water Pump
- Motor Driver
- Red LED
- Green LED
- Regulated Power Supply
- Connecting Wires

## Technologies Used

- Embedded C
- Keil µVision IDE
- Flash Magic
- UART Communication
- GSM AT Commands
- ARM7 Architecture

## Working Principle

1. The soil moisture sensor continuously monitors the soil condition.
2. The LPC2129 reads the sensor output through a GPIO pin.
3. When the soil becomes dry, the controller switches ON the water pump.
4. The green LED indicates dry soil and watering started.
5. When the soil becomes wet, the controller switches OFF the pump.
6. The red LED indicates wet soil and watering completed.
7. The GSM module sends SMS notifications to the owner's mobile phone.
8. The owner can send `*STATUS#` through SMS to receive the current soil and pump status.

## GSM Communication

The GSM module communicates with the LPC2129 through UART0 using AT commands.

**SMS Notifications:**
- `SOIL-DRY, WATERING STARTED`
- `SOIL-WET, WATERING DONE`

**Status Request:**

The owner sends `*STATUS#` to receive the current system status.

## Applications

- Smart Agriculture
- Home Gardening
- Automatic Irrigation Systems
- Greenhouse Monitoring
- Agricultural Automation

## Advantages

- Automatic plant watering
- Reduces manual effort
- Saves water
- Remote monitoring through SMS
- Low-cost embedded solution

## Project Details

- **Name:** Varun S
- **Vector ID:** V26CE1V1
- **Institution:** Vector Institute
- **Course:** Embedded Systems
- **Microcontroller:** ARM7 LPC2129

## Author

Varun S

#EmbeddedSystems #ARM7 #LPC2129 #EmbeddedC #GSM #SmartAgriculture #IoT
