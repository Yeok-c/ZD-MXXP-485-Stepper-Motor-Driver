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

int main(int argc, char *argv[])
{
    int ret;
    float retf;
    uint16_t *table;
    uint16_t *table__;
    uint16_t *table_0;
    uint16_t *table_1;
    modbus_t *mb;
    
    char port[20];  
    printf("argc = %d, argv[1] = %s\n", argc, argv[1]);
    if(argc == 2)
        strcpy(port, argv[1]);
    else 
        strcpy(port, PORT_NAME);
    printf("libmodbus modbu-rtu master demo: %s, 9600, N, 8, 1\n", port);

    mb = modbus_new_rtu(port, 9600, 'N', 8, 1);

    modbus_set_debug(mb, TRUE);

    if (mb == NULL)
    {
        modbus_free(mb);
        printf("new rtu failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    modbus_set_slave(mb, 1);
    ret = modbus_connect(mb);

    if(ret == -1)
    {
        modbus_close(mb);
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        return 0;
    }
    while(1)
    {
        printf("\n\nREG_ADDRESS     : "); ret = read_value(mb, REG_ADDRESS      ); unparse_2_bytes(ret);
        printf("\n\nREG_MICROSTEP   : "); ret = read_value(mb, REG_MICROSTEP    ); unparse_2_bytes(ret);
        printf("\n\nREG_POS_MODE    : "); ret = read_value(mb, REG_POS_MODE     ); unparse_2_bytes(ret);
        printf("\n\nREG_ACC_STEP_H  : "); ret = read_value(mb, REG_ACC_STEP_H   ); unparse_4_bytes(ret);
        printf("\n\nREG_ACC_PARAM_H : "); retf = read_fvalue(mb, REG_ACC_PARAM_H); unparse_float_to_bytes(retf);       
        printf("\n\nREG_INIT_PERIOD : "); ret = read_value(mb, REG_INIT_PERIOD  ); unparse_2_bytes(ret);
        printf("\n\nREG_MAX_PERIOD  : "); ret = read_value(mb, REG_MAX_PERIOD   ); unparse_2_bytes(ret);
        printf("\n\nREG_MAX_DIST_H  : "); ret = read_value(mb, REG_MAX_DIST_H   ); unparse_4_bytes(ret);
        printf("\n\nREG_ZERO_POS_H  : "); ret = read_value(mb, REG_ZERO_POS_H   ); unparse_4_bytes(ret);
        printf("\n\nREG_LIM_SW_OFST : "); ret = read_value(mb, REG_LIM_SW_OFST  ); unparse_2_bytes(ret);
        sleep(1);
    }

    modbus_close(mb);
    modbus_free(mb);
    system("pause");
    return 0;
}

int num_bytes(int reg_addr){
    int nb;
    if ((reg_addr == REG_ADDRESS) || (reg_addr == REG_MICROSTEP) || (reg_addr == REG_POS_MODE) || (reg_addr == REG_INIT_PERIOD) || (reg_addr == REG_MAX_PERIOD)){
        nb=1;
    } else {
        nb=2;
    }
    return nb;
}

float read_fvalue(modbus_t *ctx, int reg_addr){
    uint16_t table[2] = {0,0};
    int ret, nb;
    float value;
    assert(reg_addr == REG_ACC_PARAM_H);
    nb = num_bytes(reg_addr);
    ret = modbus_read_registers(ctx, reg_addr, nb, table);
    value = parse_bytes_to_float(table);
    if(ret != -1)
        printf("read success : register addr: %d, size = %d byte, value = %f \n", reg_addr, nb, value);
    else
    {
        printf("read error: %s\n", modbus_strerror(errno));
        return ret;
    }
    return value;

}

int read_value(modbus_t *ctx, int reg_addr){
    uint16_t table[2] = {0,0};
    int ret, value, nb;
    assert(reg_addr != REG_ACC_PARAM_H);
    nb = num_bytes(reg_addr);
    ret = modbus_read_registers(ctx, reg_addr, nb, table);
    if (reg_addr != REG_ACC_PARAM_H){
        value = parse_bytes(table, nb);     
    }

    if(ret != -1)
        printf("read success : register addr: %d, size = %d byte, value = %d \n", reg_addr, nb, value);
    else
    {
        printf("read error: %s\n", modbus_strerror(errno));
        return ret;
    }
    return value;
}

/* 
    Parses register values into integer values
    Each register contains value 0-255 (2 hex numbers)
    Eg. 1 register input:
        Input: [0x03][0x20]
        Output: 800
    Eg. 2 register input:
        Input: [0x00][0x00] [0x07][0xD0]
        Output: 2000

    TODO: 
    Change to << 16 and << 8 later!
*/
long parse_bytes(uint16_t table[], int nb){
    long intNumber;
    if(nb==1){
        intNumber = table[1]<<8 + table[0];
    } 
    else if(nb==2){
        intNumber = table[0]*256*256 + table[1];
    }
    return intNumber;
}

/* 
    Parses bytes into floats, always 2 register input
    Eg. Input: [0x3C][0x23][0xD7][0x0A]
        Output: 0.010000 
*/
float parse_bytes_to_float(uint16_t table[]){
    float f;
    uint16_t table_[] = {table[1], table[0]};
    memcpy(&f, &table_, sizeof(f));
    return f;
}

// Inverse function parse_bytes
uint16_t * unparse_2_bytes(long intNumber){
    static uint16_t table[2] = {0,0};
    static uint16_t table_[2] = {0,0};
    memcpy(&table, &intNumber, sizeof(intNumber));
    
    table_[0] = table[0] >> 8;   // shift the higher 8 bits
    table_[1] = table[0] & 0xff; // mask the lower 8 bits;
    printf("Unparse results: [%02X][%02X]", table_[0], table_[1]); 
    return table_;
}


uint16_t * unparse_4_bytes(long intNumber){
    static uint16_t table[2] = {0,0};
    static uint16_t table_[2] = {0,0};
    memcpy(&table, &intNumber, sizeof(intNumber));

    table_[0] = table[1] >> 8;   // shift the higher 8 bits
    table_[1] = table[1] & 0xff; // mask the lower 8 bits;
    table_[2] = table[0] >> 8;   // shift the higher 8 bits
    table_[3] = table[0] & 0xff; // mask the lower 8 bits;
    
    printf("Unparse results: [%02X][%02X] [%02X][%02X]", table_[0], table_[1],  table_[2], table_[3]); 
    return table_;
}


// Inverse function parse_bytes_to_float
uint16_t * unparse_float_to_bytes(float f){
    static uint16_t table[2] = {0,0};
    static uint16_t table_[2] = {0,0};
    memcpy(&table, &f, sizeof(f));
    table_[0] = table[1] >> 8;   // shift the higher 8 bits
    table_[1] = table[1] & 0xff; // mask the lower 8 bits;
    table_[2] = table[0] >> 8;   // shift the higher 8 bits
    table_[3] = table[0] & 0xff; // mask the lower 8 bits;

    printf("Unparse results: [%02X][%02X] [%02X][%02X]", table_[0], table_[1],  table_[2], table_[3]); 
    return table_;
}

/*

        // for(int i = 0; i < 3; i++)
        //     table[i] += 1;

        // ret = modbus_write_registers(mb, 0x0F, 3, table);
        // if(ret == 3)
        //     printf("write success: 0x%02x 0x%02x 0x%02x \n", table[0], table[1], table[2]);
        // else
        // {
        //     printf("write error: %s\n", modbus_strerror(errno));
        //     break;
        // }
        // Sleep(1000);
*/