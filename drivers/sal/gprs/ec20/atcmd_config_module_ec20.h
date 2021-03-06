/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _ATCMD_CONFIG_MODULE
#define _ATCMD_CONFIG_MODULE

#include "aos/hal/uart.h"

/**
 * AT related platform-dependent things are here, including:
 *   1. AT command;
 *   2. AT response code;
 *   3. AT delimiter;
 *   4. AT event;
 *   5. Uart port used by AT;
 *   6. ...
 */

// AT command
#define AT_CMD_ENET_SEND "AT+ENETRAWSEND"
#define AT_CMD_ENTER_ENET_MODE "AT+ENETRAWMODE=ON"
#define AT_CMD_EHCO_OFF "AT+UARTE=OFF"
#define AT_CMD_TEST "AT"

// Delimiter
#define AT_RECV_PREFIX "\r\n"
#define AT_RECV_SUCCESS_POSTFIX "OK"
#define AT_RECV_FAIL_POSTFIX "ERROR\r\n"
#define AT_SEND_DELIMITER "\r"

// send delay
#define AT_SEND_DATA_DELAY_MS 50

// AT event
#define AT_EVENT_ENET_DATA "+ENETEVENT:"

// uart config
#define AT_UART_BAUDRATE 115200
#define AT_UART_DATA_WIDTH DATA_WIDTH_8BIT
#define AT_UART_PARITY NO_PARITY
#define AT_UART_STOP_BITS STOP_BITS_1
#define AT_UART_FLOW_CONTROL FLOW_CONTROL_DISABLED
#define AT_UART_MODE MODE_TX_RX




void ec20_modemon(void);
void ec20_modemoff(void);
void ec20_forcesoff(void);

int ec20_modemcheck(void);
int ec20_uart_init(void);
int ec20_gprs_ip_init(void);
int ec20_gprs_got_ip(uint8_t * ip);
int ec20_gprs_status_check(uint8_t * sig, uint32_t bit);
int ec20_domaintoip(char *domain, char ip[16]);
int ec20_start(sal_conn_t *conn);
int ec20_close(int fd, int32_t remote_port);
int ec20_send(int fd,
                 uint8_t *data,
                 uint32_t len,
                 char remote_ip[16],
                 int32_t remote_port,
                 int32_t timeout);

int ec20_ftp_status_check(uint8_t * usr, uint8_t * pwd);
int ec20_ftp_start(uint8_t * domain, uint16_t port, uint8_t * fold);
int ec20_ftp_get(uint8_t * file, uint8_t * outbuf, uint32_t offset, uint16_t * len);
int ec20_ftp_close(void);


#endif
