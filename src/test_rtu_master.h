

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

#define PORT_NAME "/dev/ttyUSB1"


// BUTTON STATUS - COILS
// Function code - 01 - Read coils
#define REG_CURRENT_POS_H  5000

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
#define REG_POS_MODE    3002
#define REG_ACC_STEP_H  3004 
#define REG_ACC_PARAM_H 3006 
#define REG_INIT_PERIOD 3008
#define REG_MAX_PERIOD  3009
#define REG_MAX_DIST_H  3010
#define REG_ZERO_POS_H  3012
#define REG_LIM_SW_OFST 3014


int read_value(modbus_t *ctx, int reg_addr);
float read_fvalue(modbus_t *ctx, int reg_addr);
int num_bytes(int reg_addr);
long parse_bytes(uint16_t size_four_array[], int nb);
float parse_bytes_to_float(uint16_t size_four_array[]);
uint16_t * unparse_2_bytes(long intNumber);
uint16_t * unparse_float_to_bytes(float f);
uint16_t * unparse_4_bytes(long intNumber);

