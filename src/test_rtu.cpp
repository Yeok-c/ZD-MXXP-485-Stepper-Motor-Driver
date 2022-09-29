# include "test_rtu_master.c"

int main(int argc, char *argv[])
{
    int ret;
    float retf;
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