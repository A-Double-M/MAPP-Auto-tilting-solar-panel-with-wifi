#include <cstdio>
#include <iterator>
#undef __ARM_FP

#include "mbed.h"
#include <stdio.h>
#include <string.h>

#define MAXIMUM_BUFFER_SIZE  256
#define UART1_TX    PB_6
#define UART1_RX    PB_7
#define UART2_TX    PA_2
#define UART2_RX    PA_3
#define UART3_TX    PC_10
#define UART3_RX    PC_11

static DigitalOut led_PB14(PB_14);
static DigitalOut led_PB15(PB_15);

static BufferedSerial serial_port(UART3_TX, UART3_RX);

char bufRx[MAXIMUM_BUFFER_SIZE] = {0};
char bufTx[MAXIMUM_BUFFER_SIZE] = {0};
char bufCommand[MAXIMUM_BUFFER_SIZE] = {0};

uint32_t num1 = 0, num2 = 0, num3 = 0, num4 = 0;

char commandReceived = 0;
bool commandFound = false;
bool connectionClosed = false;

void extract_command(char *input, char *output) {
    char *cmd_start = strstr(input, "command");
    if (cmd_start) {
        cmd_start = strchr(cmd_start, '=');
        if (cmd_start && *(cmd_start + 1) != '\0') {
            cmd_start += 2;
            printf("The command is : %c\n", *cmd_start);
            commandFound = true;
            strcpy(output, cmd_start);
            char *end = strpbrk(output, " \r\n");
            if (end) {
                *end = '\0';
            }
            printf("The extracted command is : %s\n", output);
        } else {
            commandFound = false;
        }
    }
}

void UpdateClient(char chCommand){
    sprintf(bufTx, "AT+CIPSEND=0,21\r\n");
    num1 = serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(500);

    num2 = serial_port.read(bufRx, sizeof(bufRx) - 1);
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);        
    }

    snprintf(bufTx, sizeof(bufTx), "Command: %c is done!\r\n", chCommand);
    num1 = serial_port.write(bufTx, strlen(bufTx));
    printf("Send %d characters to client.\n", num1);
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx) - 1);
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("Reply from ESP01: %s\n", bufRx);        
    }
    thread_sleep_for(1000);
}

void CloseConnection(void){
    sprintf(bufTx, "AT+CIPCLOSE=0\r\n");
    num1 = serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx) - 1);
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);        
    }
    thread_sleep_for(1000);
}

int main(void){
    printf("\n========== MAIN START ==========\n");

//    mbed::Watchdog::get_instance().stop();

    serial_port.set_baud(115200);
    serial_port.set_format(8, BufferedSerial::None, 1);
    serial_port.set_flow_control(mbed::BufferedSerial::Disabled);

    sprintf(bufTx, "AT\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(500);

    sprintf(bufTx, "AT+CWMODE=1\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(500);

    sprintf(bufTx, "AT+CIFSR\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(500);

    sprintf(bufTx, "AT+CWJAP=\"OPPO Reno8 T\",\"b8mukjmp\"\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(500);

    sprintf(bufTx, "AT+CIFSR\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(500);

    sprintf(bufTx, "AT+CIPMUX=1\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(1000);

    sprintf(bufTx, "AT+CIPSERVER=1,80\r\n");
    thread_sleep_for(1000);
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);
    }
    thread_sleep_for(500);

    sprintf(bufTx, "AT+CIPSEND=0,71\r\n");
    serial_port.write(bufTx, strlen(bufTx));
    thread_sleep_for(50);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("%s\n", bufRx);        
    }
    thread_sleep_for(200);

    sprintf(bufTx, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nHello\r\n");
    num1 = serial_port.write(bufTx, strlen(bufTx));
    printf("Send %d characters to client.\n", num1);
    thread_sleep_for(1000);

    num2 = serial_port.read(bufRx, sizeof(bufRx));
    if (num2 > 0 && num2 < MAXIMUM_BUFFER_SIZE) {
        bufRx[num2] = '\0';
        printf("Reply from ESP01: %s\n", bufRx);    
    }
    thread_sleep_for(1000);

    while(1) {
        commandFound = false;

        num2 = serial_port.read(bufRx, sizeof(bufRx));
        thread_sleep_for(1000);
        num3 = serial_port.read(bufRx + num2, sizeof(bufRx)-num2);
        if (num2 + num3 >= MAXIMUM_BUFFER_SIZE) {
            printf("Combined read too large.\n");
            return 1;
        }
        
        bufRx[num2+num3] = '\0';        
        printf("Command: %s\n", bufRx);  

        extract_command(bufRx, bufCommand);

        commandReceived = commandFound ? *bufCommand : 0;
        printf("Command: %c\n", commandReceived); 

        switch (commandReceived){
            case '0': break;
            case '1': UpdateClient(commandReceived); break;
            case '2': break;
            case '3': UpdateClient(commandReceived); break;
            default:
                CloseConnection();
                connectionClosed = true;
                break;
        }

        if (connectionClosed) {
            printf("CC:true\n");
            break;
        } else {
            printf("CC:Should loop the while.\n");
        }

        thread_sleep_for(1000);  
    }
    printf("==== Suppose to end here ====\n");
}