

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <errno.h>
#include "modbus.h"
#include <assert.h>
#include <errno.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Default port name
// You can specify when launching program
// eg. ./test_rtu /dev/ttyUSB1
#define PORT_NAME "/dev/ttyUSB0"


// NEMA full revolution number of steps
#define FULL_REVOLUTION_STEPS 145728 
// 198*23*32 //360/~1.8 (angle/nema_step) * 23 gear reduction * 32 microsteps 

// BUTTON STATUS - COILS
// Function code - 01 - Read coils
// [HOME] [UP] [DOWN]
#define REG_BUTTON_STATES_H 5000

// READ ONLY REGISTERS - TABLE 1
// Function code 03 - Read
// Note: Can read all 5 register at once starting at 1000
#define REG_CURRENT_POS_H  1000
#define REG_TARGET_POS_H   1002
#define REG_CURRENT_STATE  1004

// WRITE ONLY REGISTERS - TABLE 2
// Function code 06 - Write
// Moving commands
#define REG_MOVE_TO_ZERO    2000 // value irrelevant
#define REG_STOP_ALL        2001 // value irrelevant
#define REG_MOVE_TO_POS_H   2002 
#define REG_MOVE_FORWARD_H  2004
#define REG_MOVE_BACKWARD_H 2006 

// READ WRITE REGISTERS - Table 3
// Function code 03 - Read; 06/16 - Write 
#define REG_ADDRESS     3000
#define REG_MICROSTEP   3001
#define REG_LOCK_MODE    3002
#define REG_ACC_STEP_H  3004 
#define REG_ACC_PARAM_H 3006 
#define REG_INIT_PERIOD 3008
#define REG_MAX_PERIOD  3009
#define REG_MAX_DIST_H  3010
#define REG_ZERO_POS_H  3012
#define REG_LIM_SW_OFST 3014

// Idk why this isn't provided in the datasheet
#define REG_FLASH_CONFIGURATION 4002 

// POSSIBLE STATES FOR CURRENT STATE REGISTER - 1004 
#define STATE_UNDEFINED             0xFF // 开机未定义状态。当不是 0xff 时，各 bit 位表示不同意思
#define STATE_IDLE                  0x00 // 空闲状态
#define STATE_ACCEL                 0x01 // 加速状态
#define STATE_CONST                 0x02 // 恒速状态
#define STATE_DECCEL                0x03 // 减速状态
#define STATE_HOMING                0x08 // 电机复位中
#define STATE_ERROR_RETURNING       0x10 // 复位状态出错 ,复位时零位开关未检测
#define STATE_UP_BUTTON_PRESSED     0x60 // 运行过程撞到 Up 开关
#define STATE_DOWN_BUTTON_PRESSED   0x70 // 运行过程撞到 down 开关


uint32_t read_value(modbus_t *ctx, int device_addr, int reg_addr);
float read_fvalue(modbus_t *ctx, int device_addr, int reg_addr);

int write_value(modbus_t *ctx, int device_addr, int reg_addr, uint32_t value);
int write_fvalue(modbus_t *ctx, int device_addr, int reg_addr, float fvalue);

void defAllocator_uint8(uint8_t *buffer, uint8_t count);
void defAllocator_uint16(uint16_t *buffer, uint16_t count);
void defAllocator_uint32(uint32_t *buffer, uint32_t count);

void writeUInt32ToBufferBigEndian(uint32_t number, uint16_t* buffer);
void writeUInt16ToBufferBigEndian(uint16_t number, uint16_t* buffer);
void writeFloatToBufferBigEndian(float number, uint16_t* buffer);

int num_bytes(int reg_addr);
uint32_t parse_bytes(uint16_t size_four_array[], int nb);
float parse_bytes_to_float(uint16_t size_four_array[]);
uint8_t * unparse_2_bytes(uint16_t intNumber);
uint16_t * unparse_float_to_bytes(float f);
uint16_t * unparse_4_bytes(uint32_t intNumber);

uint8_t * read_button_states(modbus_t *ctx, int device_addr);
uint32_t * read_motion_states(modbus_t *ctx, int device_addr);

int homing(modbus_t *ctx, int device_addr);
int goto_position(modbus_t *ctx, int device_addr, uint32_t position);
int move_forwards(modbus_t *ctx, int device_addr, uint32_t steps);
int move_backwards(modbus_t *ctx, int device_addr, uint32_t steps);
int stop(modbus_t *ctx, int device_addr);

int lock_when_stopped(modbus_t *ctx, int device_addr);
int unlock_when_stopped(modbus_t *ctx, int device_addr);
int flash_parameters(modbus_t *ctx, int device_addr);
int change_address(modbus_t *ctx, int old_device_addr, int new_device_addr);
