#include "ZDSM.h"
int main(int argc, char *argv[])
{
    int ret;
    float retf;
    ZDSM zdsm;

    while(1)
    {
        printf("\n\nREG_ADDRESS     : "); ret = read_value(zdsm.mb, zdsm.REG_ADDRESS      ); unparse_2_bytes(ret);
        printf("\n\nREG_MICROSTEP   : "); ret = read_value(zdsm.mb, zdsm.REG_MICROSTEP    ); unparse_2_bytes(ret);
        printf("\n\nREG_POS_MODE    : "); ret = read_value(zdsm.mb, zdsm.REG_POS_MODE     ); unparse_2_bytes(ret);
        printf("\n\nREG_ACC_STEP_H  : "); ret = read_value(zdsm.mb, zdsm.REG_ACC_STEP_H   ); unparse_4_bytes(ret);
        printf("\n\nREG_ACC_PARAM_H : "); retf = read_fvalue(zdsm.mb, zdsm.REG_ACC_PARAM_H); unparse_float_to_bytes(retf);       
        printf("\n\nREG_INIT_PERIOD : "); ret = read_value(zdsm.mb, zdsm.REG_INIT_PERIOD  ); unparse_2_bytes(ret);
        printf("\n\nREG_MAX_PERIOD  : "); ret = read_value(zdsm.mb, zdsm.REG_MAX_PERIOD   ); unparse_2_bytes(ret);
        printf("\n\nREG_MAX_DIST_H  : "); ret = read_value(zdsm.mb, zdsm.REG_MAX_DIST_H   ); unparse_4_bytes(ret);
        printf("\n\nREG_ZERO_POS_H  : "); ret = read_value(zdsm.mb, zdsm.REG_ZERO_POS_H   ); unparse_4_bytes(ret);
        printf("\n\nREG_LIM_SW_OFST : "); ret = read_value(zdsm.mb, zdsm.REG_LIM_SW_OFST  ); unparse_2_bytes(ret);
        sleep(1);
    }
    system("pause");
    return 0;
}
