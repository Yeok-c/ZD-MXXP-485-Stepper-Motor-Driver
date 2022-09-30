# include "test_rtu_master.c"

int main(int argc, char *argv[])
{
    int ret_status;
    uint16_t ret_uint16;
    uint32_t ret_uint32;
    float ret_float;
    modbus_t *mb;

        
    uint8_t ButtonBuffer;
    uint8_t *button_buffer = &ButtonBuffer;
    defAllocator_uint8(button_buffer, 2);

    uint16_t myBuffer_2;
    uint16_t *buffer_2 = &myBuffer_2;
    defAllocator_uint16(buffer_2, 2);

    uint16_t myBuffer;
    uint16_t *buffer = &myBuffer;
    defAllocator_uint16(buffer, 1);

    uint32_t StateBuffer;
    uint32_t *state_buffer = &StateBuffer;
    defAllocator_uint32(state_buffer, 2);


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
    ret_status = modbus_connect(mb);

    if(ret_status == -1)
    {
        modbus_close(mb);
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    goto_home(mb);
    sleep(5);
    
    while(1)
    {


        // Read button states
        // retb = read_button_states(mb); printf("Button results: [%02X][%02X][%02X] \n", retb[0], retb[1], retb[2]); 

        // Read motion-related states
        // printf("\n\nREAD REG_CURRENT_POS_H : "); ret_uint32 = read_value(mb, REG_CURRENT_POS_H  ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
        // printf("\n\nREAD REG_TARGET_POS_H : "); ret_uint32 = read_value(mb, REG_TARGET_POS_H  ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
        // printf("\n\nREAD REG_CURRENT_STATE : "); ret_uint16 = read_value(mb, REG_CURRENT_STATE  ); writeUInt16ToBufferBigEndian(ret_uint16, buffer); print_state_message(ret_uint16);

        // Read register states
        // printf("\n\nREAD REG_ADDRESS     : "); ret_uint16 = read_value(mb, REG_ADDRESS      ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
        // printf("\n\nREAD REG_MICROSTEP   : "); ret_uint16 = read_value(mb, REG_MICROSTEP    ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
        // printf("\n\nREAD REG_POS_MODE    : "); ret_uint16 = read_value(mb, REG_POS_MODE     ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
        // printf("\n\nREAD REG_ACC_STEP_H  : "); ret_uint32 = read_value(mb, REG_ACC_STEP_H   ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
        // printf("\n\nREAD REG_ACC_PARAM_H : "); ret_float  = read_fvalue(mb, REG_ACC_PARAM_H ); writeFloatToBufferBigEndian( ret_float, buffer_2);   
        // printf("\n\nREAD REG_INIT_PERIOD : "); ret_uint16 = read_value(mb, REG_INIT_PERIOD  ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
        // printf("\n\nREAD REG_MAX_PERIOD  : "); ret_uint16 = read_value(mb, REG_MAX_PERIOD   ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
        // printf("\n\nREAD REG_MAX_DIST_H  : "); ret_uint32 = read_value(mb, REG_MAX_DIST_H   ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
        // printf("\n\nREAD REG_ZERO_POS_H  : "); ret_uint32 = read_value(mb, REG_ZERO_POS_H   ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
        // printf("\n\nREAD REG_LIM_SW_OFST : "); ret_uint16 = read_value(mb, REG_LIM_SW_OFST  ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
    
        // // Write to registers
        // printf("\n\nWRITE REG_MICROSTEP   : "); ret_status = write_value(mb, REG_MICROSTEP, 16); 
        // // printf("\n\nWRITE REG_ADDRESS     : "); ret_status = read_value(mb, REG_ADDRESS, 1);
        // printf("\n\nWRITE REG_MICROSTEP   : "); ret_status = write_value(mb, REG_MICROSTEP,32);  
        // // printf("\n\nWRITE REG_POS_MODE    : "); ret_status = write_value(mb, REG_POS_MODE, 1); // Not understand how this works
        // printf("\n\nWRITE REG_ACC_STEP_H  : "); ret_status = write_value(mb, REG_ACC_STEP_H,   1000);
        // printf("\n\nWRITE REG_ACC_PARAM_H : "); ret_status = write_fvalue(mb, REG_ACC_PARAM_H,  0.03);       
        // printf("\n\nWRITE REG_INIT_PERIOD : "); ret_status = write_value(mb, REG_INIT_PERIOD,  800);
        // printf("\n\nWRITE REG_MAX_PERIOD  : "); ret_status = write_value(mb, REG_MAX_PERIOD,   250);
        // printf("\n\nWRITE REG_MAX_DIST_H  : "); ret_status = write_value(mb, REG_MAX_DIST_H,   100000);
        // printf("\n\nWRITE REG_ZERO_POS_H  : "); ret_status = write_value(mb, REG_ZERO_POS_H,   0);
        // printf("\n\nWRITE REG_LIM_SW_OFST : "); ret_status = write_value(mb, REG_LIM_SW_OFST,  0);

        

    }

    modbus_close(mb);
    modbus_free(mb);
    system("pause");
    return 0;
}