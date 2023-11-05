
# ZD-MXXP-485 Stepper Motor Driver

Control NEMA stepper motor drivers with RS485.

Register and functions designed for ZD-M42P-485 or ZD-M57P-485 NEMA stepper motor back-mounted drivers controller via RS485 such as [CAN總線步進馬達驅動器控制器RS485接口42/57型微小型驅動控制器
](https://world.taobao.com/item/634196223214.htm) or [57 integrated stepper motor driver module controller with cooling shell 485CAN pulse control mode
](https://www.aliexpress.com/item/1005004161246962.html).

Main contribution is c libraries ```zdmxxp.c``` and ```zdmxxp.h```. 

## Features
- Control stepper motor motion
  - homing
  - goto_position
  - move_forwards
  - move_backwards
  - stop

- Read limit switch states
  - read_button_states reads home, limit_down and limit_up switch states and returns their states (1 or 0) 

- Read motor state
  - REG_CURRENT_POS_H - Current position
  - REG_TARGET_POS_H - Target position currently moving towards
  - REG_CURRENT_STATE - Reads current state, which may be STATE_UNDEFINED, STATE_IDLE, STATE_ACCEL, STATE_CONST, STATE_DECCEL, STATE_HOMING, STATE_ERROR_RETURNING, STATE_UP_BUTTON_PRESSED, STATE_DOWN_BUTTON_PRESSED

- Read and write register values (the meanings of these registers are my own intepretation based on their Chinese terms)
  - REG_ADDRESS  - Device address  
  - REG_MICROSTEP - 1 / 2 / 4 / 8 / 16 / 32 (and more depending on driver)
  - REG_LOCK_MODE - Whether device locks itself from moving when not in motion
  - REG_ACC_STEP_H  - Stepper acceleration steps
  - REG_ACC_PARAM_H - Stepper acceleration parameter
  - REG_INIT_PERIOD - Period of stepper clocks when accelerating
  - REG_MAX_PERIOD  - Period of stepper clocks when at constant (aka max) speed
  - REG_MAX_DIST_H - Maximum steps that can be travelled by stepper
  - REG_ZERO_POS_H - Home position wrt to zero
  - REG_LIM_SW_OFST - Offset between home limit switch and designated home position

- Theoretically this implementation should be cross-platform between linux and windows. libmodbus is cross platform - the only linux-only dependency used by me is ```sleep``` - of which should be easily substituted for windows.

## Sources
The core modbus libraries are forked from [libmodbus](https://github.com/stephane/libmodbus). Register information is obtained and translated from [this product page](://world.taobao.com/item/634196223214.htm). I found that [this guide](https://aijishu.com/a/1060000000224798) was supremely helpful for me in using libmodbus. 

## Usage and demo
1. Clone repo
2. Run test_rtu with command ```./test_rtu```. This is a demo program. You can specify COM port with ```./test_rtu /dev/ttyUSB1```. You can change default value in ```zdxxmp.h```. 
3. To include this in your own projects, all dependencies are included in src folder. 

## Build
To build, G++ (and Gcc) is required. Makefile so just run terminal command  ```make``` in ```./ZD-MXXP-485-Stepper-Motor-Driver/src```
