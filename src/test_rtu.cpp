#include "zdxxmp.c"
#include <iostream>

int open_size = 200000; // steps, slightly less than 53838 which is (23 gear reduction)*(360/1.8 step per angle)*(133/360 degrees to turn)
int device_addr = 1;
int m_open(modbus_t *ctx, int device_addr){
    move_forwards(ctx, device_addr, open_size);
    return 1;
}

int m_close(modbus_t *ctx, int device_addr){
    move_backwards(ctx, device_addr, open_size);
    // sleep(8);
    // unlock_when_stopped(ctx, device_addr);
    // sleep(0.5);
    // lock_when_stopped(ctx, device_addr);
    return 1;
}

int m_home(modbus_t *ctx, int device_addr){
    move_backwards(ctx, device_addr, open_size);
    sleep(8); // or read when ok
    homing(ctx, device_addr);
    sleep(2); // or read when ok
    return 1;
}

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
    if (argc == 2)
        strcpy(port, argv[1]);
    else
        strcpy(port, PORT_NAME);
    printf("libmodbus modbu-rtu master demo: %s, 19200, N, 8, 1\n", port);


    // modbus_new_rtu(const char *device, int baud, char parity, int data_bit, int stop_bit);
    mb = modbus_new_rtu(port, 19200, 'N', 8, 1);

    modbus_set_debug(mb, TRUE);
    modbus_set_response_timeout(mb, 0.5, 0);

    if (mb == NULL)
    {
        modbus_free(mb);
        printf("new rtu failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    ret_status = modbus_connect(mb);

    if (ret_status == -1)
    {
        modbus_close(mb);
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        printf("You may need to start terminal as root or use sudo");
        return 0;
    }

    // printf("\n Homing...\n");
    // m_home(mb, 1);

    int program_flag = 1;
    while (program_flag == 1)
    {

        std::cout << "\nEnter a command: ";
        unsigned char x{}; // define variable x to hold user input (and zero-initialize it)
        std::cin >> x;

        switch (x)
        {
        case '/':
            printf("Read from pressure sensor");
            for(device_addr = 18; device_addr < 22; device_addr++){
                printf("\nREAD PRESSURE Sensor %d: ", device_addr);
                ret_uint32 = read_pressure(mb, device_addr);
                writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
            }
            break;

        case '1':
            printf("Changing destination of commands sent to 1");
            device_addr=1;
            break;

        case '2':
            printf("Changing destination of commands sent to 2");
            device_addr=2;
            break;

        case 'o':
            m_open(mb, device_addr);
            break;

        case 'c':
            m_close(mb, device_addr);
            break;

        case 'b':
            move_backwards(mb, device_addr, 10000);
            break;

        case 'f':
            move_forwards(mb, device_addr, 10000);
            break;

        case 's':
            stop(mb, device_addr);
            break;

        case 'l':
            lock_when_stopped(mb, device_addr);
            break;

        case 'u':
            unlock_when_stopped(mb, device_addr);
            break;

        case 'a':
            change_address(mb, 2, 1);
            break;

        case 'h':
            m_home(mb, device_addr);
            break;

        case 'n':
            // Read button states
            button_buffer = read_button_states(mb, device_addr);
            printf("Button results: [%02X][%02X][%02X] \n",
                   button_buffer[0], button_buffer[1], button_buffer[2]);
            break;
        case 'e':
            // Read motion-related states
            printf("\nREAD REG_CURRENT_POS_H : ");
            ret_uint32 = read_value(mb, device_addr, REG_CURRENT_POS_H);
            writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
            printf("\nREAD REG_TARGET_POS_H : ");
            ret_uint32 = read_value(mb, device_addr, REG_TARGET_POS_H);
            writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
            printf("\nREAD REG_CURRENT_STATE : ");
            ret_uint16 = read_value(mb, device_addr, REG_CURRENT_STATE);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            print_state_message(ret_uint16);
            break;
        case 'r':
            // Read register states
            printf("\nREAD REG_ADDRESS     : ");
            ret_uint16 = read_value(mb, device_addr, REG_ADDRESS);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            printf("\nREAD REG_MICROSTEP   : ");
            ret_uint16 = read_value(mb, device_addr, REG_MICROSTEP);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            printf("\nREAD REG_LOCK_MODE    : ");
            ret_uint16 = read_value(mb, device_addr, REG_LOCK_MODE);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            printf("\nREAD REG_ACC_STEP_H  : ");
            ret_uint32 = read_value(mb, device_addr, REG_ACC_STEP_H);
            writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
            printf("\nREAD REG_ACC_PARAM_H : ");
            ret_float = read_fvalue(mb, device_addr, REG_ACC_PARAM_H);
            writeFloatToBufferBigEndian(ret_float, buffer_2);
            printf("\nREAD REG_INIT_PERIOD : ");
            ret_uint16 = read_value(mb, device_addr, REG_INIT_PERIOD);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            printf("\nREAD REG_MAX_PERIOD  : ");
            ret_uint16 = read_value(mb, device_addr, REG_MAX_PERIOD);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            printf("\nREAD REG_MAX_DIST_H  : ");
            ret_uint32 = read_value(mb, device_addr, REG_MAX_DIST_H);
            writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
            printf("\nREAD REG_ZERO_POS_H  : ");
            ret_uint32 = read_value(mb, device_addr, REG_ZERO_POS_H);
            writeUInt32ToBufferBigEndian(ret_uint32, buffer_2);
            printf("\nREAD REG_LIM_SW_OFST : ");
            ret_uint16 = read_value(mb, device_addr, REG_LIM_SW_OFST);
            writeUInt16ToBufferBigEndian(ret_uint16, buffer);
            break;
        case 'w': // Write to registers
            printf("\nWRITE REG_MICROSTEP   : ");
            ret_status = write_value(mb, device_addr, REG_MICROSTEP, 16);
            printf("\nWRITE REG_LOCK_MODE   : ");
            ret_status = write_value(mb, device_addr, REG_LOCK_MODE, 0x40); // 0x40 for lock, 0x50 for unlock
            printf("\nWRITE REG_ACC_STEP_H  : ");
            ret_status = write_value(mb, device_addr, REG_ACC_STEP_H, 300);
            printf("\nWRITE REG_ACC_PARAM_H : ");
            ret_status = write_fvalue(mb, device_addr, REG_ACC_PARAM_H, 0.5);
            printf("\nWRITE REG_INIT_PERIOD : ");
            ret_status = write_value(mb, device_addr, REG_INIT_PERIOD, 50);
            printf("\nWRITE REG_MAX_PERIOD  : ");
            ret_status = write_value(mb, device_addr, REG_MAX_PERIOD, 20);
            printf("\nWRITE REG_MAX_DIST_H  : ");
            ret_status = write_value(mb, device_addr, REG_MAX_DIST_H, 0);
            printf("\nWRITE REG_ZERO_POS_H  : ");
            ret_status = write_value(mb, device_addr, REG_ZERO_POS_H, 0);
            printf("\nWRITE REG_LIM_SW_OFST : ");
            ret_status = write_value(mb, device_addr, REG_LIM_SW_OFST, 500);
            printf("\nFLASHING PARAMETERS : ");
            ret_status = flash_parameters(mb, device_addr);
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