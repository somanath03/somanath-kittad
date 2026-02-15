# DoseGuardian-Intelligent-Pill-Taking-Assistant

# 🩺 DOSE GUARDIAN – Intelligent-Pill-Taking-Assistant

## 📌 Project Description
**DOSE GUARDIAN** is an embedded systems project developed using the **LPC21xx (ARM7TDMI-S)** microcontroller.  
The system acts as an intelligent **medicine reminder**, allowing the user to configure **time, date, and day** using a keypad and LCD interface.  
When the configured medicine time matches the real-time clock (RTC), the system alerts the user using a **buzzer and LED**.

This project follows **industry-style embedded firmware design**, using modular drivers and clear separation of hardware abstraction and application logic.

---

## ✨ Key Features
- Real-Time Clock (RTC) based timekeeping
- User configurable:
  - Time (Hour / Minute / Second)
  - Date (Date / Month / Year)
  - Day of week
- Arrow-based LCD user interface (UP / DOWN / LEFT / RIGHT)
- Custom LCD characters using CGRAM
- Medicine alert using:
  - Buzzer (audio)
  - LED (visual)
- Keypad-based navigation and confirmation
- Leap year aware date handling
- Input validation (no zero date/month bugs)
- Modular, reusable embedded C code
- Suitable for academic mini-projects and demos

---

## 🧰 Hardware Requirements
| Component | Description |
|---------|-------------|
| Microcontroller | LPC21xx (ARM7TDMI-S) |
| Display | 16×2 LCD (HD44780 compatible) |
| Input | 4×4 Matrix Keypad |
| RTC | Internal LPC21xx RTC |
| Alert Devices | Buzzer, LED |
| Switches | SW1, SW2 |
| Power Supply | 5V regulated |
| Programming | JTAG / Flash Magic |

---

## 💻 Software Requirements
- Keil µVision IDE
- Embedded C 
- Flash Magic (for flashing)
- Proteus (for simulation)

---

## 🔌 Pin Configuration

### 5.1 LCD Pin Configuration (8-bit Mode)
| LCD Pin | LPC21xx Pin | Description |
|------|------------|-------------|
| RS | P0.8 | Register Select |
| RW | P0.9 | Read / Write |
| EN | P0.10 | Enable |
| D0–D7 | P0.0 – P0.7 | 8-bit Data Bus |

---

### 5.2 Keypad Pin Configuration
| Keypad Line | LPC21xx Pin | Direction |
|-----------|-------------|-----------|
| Rows (R0–R3) | P1.16 | Output |
| Columns (C0–C3) | P1.23 | Input |

---

### 5.3 RTC Pin Configuration
| Signal | LPC21xx Pin | Description |
|------|-------------|-------------|
| RTC Clock | Internal | On-chip RTC module |
| RTC Battery | VBAT | Backup battery support |

---

### 5.4 Alert Devices
| Device | LPC21xx Pin | Description |
|------|-------------|-------------|
| Buzzer | P1.24 | Medicine alert sound |
| LED | P1.27 | Visual alert indication |

---

### 5.5 Switch Configuration
| Switch | LPC21xx Pin | Description |
|------|-------------|-------------|
| SW1 | P0.20 | User input switch |
| SW2 | P1.25 | User input switch |

---

## 📁 Project Folder Structure

FINAL_DOSE_GARDIAN  
│  
├── DOSE_GAURDIAN.c → Main application file  
├── lcd.c / lcd.h → LCD driver  
├── lcd_defines.h → LCD commands & macros  
├── keypad.c / keypad.h → Keypad driver  
├── keypad_defines.h → Key mappings  
├── rtc.c / rtc.h → RTC driver  
├── rtc_defines.h → RTC registers & macros  
├── MD_CLK_SET.c → Time/Date/Day arrow mode logic  
├── MD_CLK_SET_2.c → Medicine scheduling logic  
├── delay_header.h → Delay prototypes  
├── def_delay.c → Delay implementation  
├── defines.h → Global macros & pin definitions  
├── types.h → Custom data types  
└── README.md → Project documentation

---

## ⚙️ Functional Overview
1. System initializes LCD, RTC, keypad, buzzer, and LED
2. User sets:
   - Time
   - Date
   - Day
3. User configures medicine alert time
4. RTC continuously compares current time with medicine time
5. On match:
   - Buzzer turns ON
   - LED turns ON
   - LCD shows alert
6. User acknowledges alert using keypad

---

## 🧠 Key Files Explained

### `DOSE_GAURDIAN.c`
- Main entry point
- Initializes peripherals
- Calls configuration and alert functions

### `MD_CLK_SET.c`
- Arrow-based UI for:
  - Time
  - Date
  - Day
- Handles UP/DOWN/LEFT/RIGHT navigation

### `lcd.c / lcd.h`
- LCD initialization
- Command, character, and string display
- Custom CGRAM character support

### `keypad.c / keypad.h`
- Matrix keypad scanning
- Debounced key detection

### `rtc.c / rtc.h`
- RTC initialization
- Time/date read and write functions

---

## 🔄 Step-by-Step Code Implementation Flow
1. MCU reset and boot
2. GPIO direction configuration
3. LCD initialization
4. Custom LCD characters loaded (arrows)
5. RTC initialization
6. Keypad initialization
7. Startup message display
8. Enter main super-loop
9. User configuration mode via keypad
10. Store time/date/day values
11. Continuous RTC monitoring
12. Trigger alert on time match
13. User acknowledgment clears alert

---

### 📸 Hardware & Simulation Outputs
The following results were captured during testing:

## LCD Time & Date Configuration Screen
 <img width="1178" height="818" alt="image" src="https://github.com/user-attachments/assets/22328169-ccee-4dba-abeb-58d693637b4a" />
## Medicine Alert Trigger Screen
 <img width="1174" height="754" alt="Screenshot 2025-12-29 164007" src="https://github.com/user-attachments/assets/c836545a-8faa-4939-8cc3-75da38970268" />
## Buzzer and LED ON Indication
 <img width="1166" height="776" alt="image" src="https://github.com/user-attachments/assets/dac6cf40-4e66-4da1-b41a-81957de35e90" />

---

## ➕ Additional Notes (Best Practices)
- CGRAM characters are loaded **once** to avoid flicker
- Input values are range-checked to prevent invalid dates
- Code is modular and reusable
- Designed for easy expansion (EEPROM, GSM, IoT)

---

## 🚀 Future Enhancements
- EEPROM storage for power-loss recovery
- Multiple medicine schedules
- GSM/SMS or IoT notifications
- Mobile app integration
- Low-power sleep modes

---

## 🎓 Academic Context
This project is developed as an **Embedded Systems Mini Project** demonstrating:
- ARM7 microcontroller programming
- RTC, LCD, keypad interfacing
- Embedded C best practices
- Real-world application design

---

## 📜 License
This project is intended for **educational and academic use only**.

---
## 👨‍💻 Developed By

**Somanath Kittad**  
Embedded Systems Trainee | E&TC Graduate  

**Technical Skills:**  
Embedded C, ARM (LPC21xx), RTC, LCD, Keypad Interfacing, GPIO  

**Project Type:**  
Embedded Systems Mini Project (Academic)

*DOSE GUARDIAN – Reliable. Simple. Life-Saving.*
