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

class ZDSM{
    public: 
        explicit ZDSM();
        ~ZDSM();

        int ret;
        uint16_t *table;
        modbus_t *mb;
        char port[20];  
        
        // VALUES THAT CAN BE READ
        int REG_ADDRESS   = 3000;
        int REG_MICROSTEP = 3001;
        int REG_POS_MODE  = 3002;
        int REG_ACC_STEP_H  = 3004; // and 3005 TWO BYTES
        int REG_ACC_PARAM_H = 3006; // and 3007 TWO BYTES
        int REG_INIT_PERIOD = 3008;
        int REG_MAX_PERIOD  = 3009;
        int REG_MAX_DIST_H  = 3010; // and 3011 TWO BYTES
        int REG_ZERO_POS_H  = 3012; // and 3013 TWO BYTES
        int REG_LIM_SW_OFST = 3014; // and 3015 TWO BYTES

        int read_value(modbus_t *ctx, int reg_addr);
        float read_fvalue(modbus_t *ctx, int reg_addr);

    private:
        int num_bytes(int reg_addr);
        long parse_bytes(uint16_t size_four_array[], int nb);
        float parse_bytes_to_float(uint16_t size_four_array[]);
        uint16_t * unparse_2_bytes(long intNumber);
        uint16_t * unparse_float_to_bytes(float f);
        uint16_t * unparse_4_bytes(long intNumber);

};