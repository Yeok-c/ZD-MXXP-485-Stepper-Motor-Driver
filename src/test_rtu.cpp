#include "test_rtu_master.c"
#include <iostream>

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
    modbus_set_response_timeout(mb, 5, 0);
    

    if (mb == NULL)
    {
        modbus_free(mb);
        printf("new rtu failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    // Set address of 'mb' here
    modbus_set_slave(mb, 1);
    ret_status = modbus_connect(mb);

    if(ret_status == -1)
    {
        modbus_close(mb);
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        printf("You may need to start terminal as root or use sudo");
        return 0;
    }

    
    printf("\n Homing... waiting for HOME limit switch to be pressed \n"); 
    homing(mb);

    int homing_flag = 1;
    while(homing_flag == 1){
        // printf("\nREAD REG_CURRENT_POS_H : "); 
        ret_uint32 = read_value(mb, REG_CURRENT_POS_H  ); 
        if(ret_uint32<=1){
            homing_flag = 0;
        }
        sleep(1);
    }
    
    int program_flag = 1;
    while(program_flag == 1){

        std::cout << "\nEnter a command: ";
        unsigned char x{ }; // define variable x to hold user input (and zero-initialize it)
        std::cin >> x;

        switch(x){
            case '8':
                goto_position(mb, 10000);
            break;

            case '2':
                goto_position(mb, 0);
            break;

            case '6':
                move_backwards(mb,2000);
            break;

            case '4':
                move_forwards(mb,2000);
            break;

            case '5':
                stop(mb);
            break;

            case 'l':
                lock_when_stopped(mb);
            break;

            case 'u':
                unlock_when_stopped(mb);
            break;


            case 'h':
                homing(mb);
            break;

            case 'b':
                // Read button states
                button_buffer = read_button_states(mb); 
                printf("Button results: [%02X][%02X][%02X] \n", 
                button_buffer[0], button_buffer[1], button_buffer[2]); 
            break;
            case 'e':
                // Read motion-related states
                printf("\nREAD REG_CURRENT_POS_H : "); ret_uint32 = read_value(mb, REG_CURRENT_POS_H  ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
                printf("\nREAD REG_TARGET_POS_H : "); ret_uint32 = read_value(mb, REG_TARGET_POS_H  ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
                printf("\nREAD REG_CURRENT_STATE : "); ret_uint16 = read_value(mb, REG_CURRENT_STATE  ); writeUInt16ToBufferBigEndian(ret_uint16, buffer); print_state_message(ret_uint16);
            break;
            case 'r':
                // Read register states
                printf("\nREAD REG_ADDRESS     : "); ret_uint16 = read_value(mb, REG_ADDRESS      ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
                printf("\nREAD REG_MICROSTEP   : "); ret_uint16 = read_value(mb, REG_MICROSTEP    ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
                printf("\nREAD REG_LOCK_MODE    : "); ret_uint16 = read_value(mb, REG_LOCK_MODE     ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
                printf("\nREAD REG_ACC_STEP_H  : "); ret_uint32 = read_value(mb, REG_ACC_STEP_H   ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
                printf("\nREAD REG_ACC_PARAM_H : "); ret_float  = read_fvalue(mb, REG_ACC_PARAM_H ); writeFloatToBufferBigEndian( ret_float, buffer_2);   
                printf("\nREAD REG_INIT_PERIOD : "); ret_uint16 = read_value(mb, REG_INIT_PERIOD  ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
                printf("\nREAD REG_MAX_PERIOD  : "); ret_uint16 = read_value(mb, REG_MAX_PERIOD   ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
                printf("\nREAD REG_MAX_DIST_H  : "); ret_uint32 = read_value(mb, REG_MAX_DIST_H   ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
                printf("\nREAD REG_ZERO_POS_H  : "); ret_uint32 = read_value(mb, REG_ZERO_POS_H   ); writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
                printf("\nREAD REG_LIM_SW_OFST : "); ret_uint16 = read_value(mb, REG_LIM_SW_OFST  ); writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            break;
            case 'w': // Write to registers
                // printf("\nWRITE REG_ADDRESS     : "); ret_status = read_value(mb, REG_ADDRESS, 1);
                printf("\nWRITE REG_MICROSTEP   : "); ret_status = write_value(mb, REG_MICROSTEP,32);  
                printf("\nWRITE REG_LOCK_MODE   : "); ret_status = write_value(mb, REG_LOCK_MODE, 0x40); // 0x40 for lock, 0x50 for unlock
                printf("\nWRITE REG_ACC_STEP_H  : "); ret_status = write_value(mb, REG_ACC_STEP_H,   1000);
                printf("\nWRITE REG_ACC_PARAM_H : "); ret_status = write_fvalue(mb, REG_ACC_PARAM_H,  0.01);       
                printf("\nWRITE REG_INIT_PERIOD : "); ret_status = write_value(mb, REG_INIT_PERIOD,  800);
                printf("\nWRITE REG_MAX_PERIOD  : "); ret_status = write_value(mb, REG_MAX_PERIOD,   250);
                printf("\nWRITE REG_MAX_DIST_H  : "); ret_status = write_value(mb, REG_MAX_DIST_H,   10000);
                printf("\nWRITE REG_ZERO_POS_H  : "); ret_status = write_value(mb, REG_ZERO_POS_H,   0);
                printf("\nWRITE REG_LIM_SW_OFST : "); ret_status = write_value(mb, REG_LIM_SW_OFST,  0);                
                printf("\nFLASHING PARAMETERS : "); ret_status = flash_parameters(mb);                
            break;
            case 'q':
                printf("Quitting and releasing serial");
                modbus_close(mb);
                modbus_free(mb);
                program_flag = 0;
            break;
            default:
                printf("Command not found");
            break;
        }




    }

    system("pause");
    return 0;
}