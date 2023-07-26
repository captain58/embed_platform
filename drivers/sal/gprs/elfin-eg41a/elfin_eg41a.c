/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "k_api.h"
#include "aos/kernel.h"
#include "log.h"
#include "aos/yloop.h"

#include <sal_import.h>
#include <atparser.h>

#include "gpio.h"
#include "board.h"
#include "modemlib.h"
#include "msgid.h"
#include "iomap.h"
#define TAG "elfin_eg41a_gprs_module"

#define ELFIN_EG41A_AT_CMD_SUCCESS_RSP "OK"
#define ELFIN_EG41A_AT_CMD_FAIL_RSP "ERROR"

#define AT_CMD_TEST "AT"
#define AT_CMD_TEST_RESULT "OK"
#define AT_CMD_ELFIN_EG41A_TEST "AT+PID"
#define AT_CMD_ELFIN_EG41A_TEST_RESULT "+ok=EG41\r\n"

#define AT_CMD_ECHO_OFF   "ATE0\r\n"
#define AT_CMD_BAUDRATE_SET "AT+IPR\r\n"
#define AT_CMD_FLOW_CONTROL "AT+IFC\r\n"

#define AT_CMD_SAVE_CONFIG  "AT&W\r\n"

#define AT_CMD_SIM_PIN_CHECK  "AT+CPIN?"
#define AT_CMD_SIGNAL_QUALITY_CHECK "AT+CSQ"
#define AT_CMD_NETWORK_REG_CHECK "AT+CREG?"
#define AT_CMD_GPRS_ATTACH_CHECK "AT+CGATT?\r\n"

#define AT_CMD_GPRS_PDP_DEACTIVE    "AT+QIDEACT=1"
#define AT_CMD_GPRS_PDP_ACTIVE      "AT+QIACT=1"

#define AT_CMD_MULTI_IP_CONNECTION "AT+CIPMUX"
#define AT_CMD_SEND_DATA_PROMPT_SET "AT+CIPSPRT"
#define AT_CMD_RECV_DATA_FORMAT_SET "AT+CIPSRIP"



#define AT_CMD_DOMAIN_TO_IP "AT+QIDNSGIP="
#define AT_CMD_DOMAIN_RSP   "\r\n+CDNSGIP: "

#define AT_CMD_START_TASK   "AT+QIACT?"
#define AT_CMD_BRING_UP_GPRS_CONNECT "AT+CIICR"
#define AT_CMD_GOT_LOCAL_IP "AT+CIFSR"

#define AT_CMD_START_CLIENT_CONN "AT+QIOPEN"
#define AT_CMD_START_CLIENT_DEACT "AT+QIDEACT"
#define AT_CMD_START_CLIENT_ACT "AT+QIACT"

#define AT_CMD_START_TCP_SERVER  "AT+CIPSERVER"

#define AT_CMD_CLIENT_CONNECT_OK "OK"
#define AT_CMD_CLIENT_CONNECT_FAIL "ERROR\r\n"

#define AT_CMD_STOP_CONN "AT+QICLOSE"

#define AT_CMD_SEND_DATA "AT+QISEND"

#define AT_CMD_DATA_RECV "\r\n+QIURC: \"recv\","//r\n+RECEIVE,"

#define AT_CMD_ELFIN_EG41A_PWR_DOWN "AT+QPOWD"
#define AT_CMD_ELFIN_EG41A_CGATT_1 "1"
#define AT_CMD_CLIENT_CONNECT_RET "+QIOPEN:"
#define AT_CMD_SEND_OK "SEND OK"
#define AT_CMD_MISI "AT+CIMI"
#define AT_CMD_APN "AT+QICSGP"
#define AT_CMD_APN_CHINAMOBILE  "cmnet"
#define AT_CMD_APN_CHINAUNINET  "UNINET"
#define AT_CMD_APN_CHINATELECOM "TELECOM"
#define AT_CMD_STATUS_CONTROL_CHECK "AT+QCFG"
#define AT_CMD_FTP_CFG "AT+QFTPCFG"
#define AT_CMD_FTP_CFG_CNT "\"contextid\",1"
#define AT_CMD_FTP_CFG_ACOUNT "\"account\""
#define AT_CMD_FTP_CFG_FT "\"filetype\",1"
#define AT_CMD_FTP_CFG_MD "\"transmode\",1"
#define AT_CMD_FTP_CFG_TO "\"rsptimeout\",90"

#define AT_CMD_START_FTP "AT+QFTPOPEN"
#define AT_CMD_FTP_CONNECT_RET "+QFTPOPEN:"
#define AT_CMD_FTP_FOLD_CFG "AT+QFTPCWD"
#define AT_CMD_FTP_FOLD_RET "+QFTPCWD:"
#define AT_CMD_FTP_GET "AT+QFTPGET"
#define AT_CMD_FTP_GET_RET "+QFTPGET:"

#define ELFIN_EG41A_AT_CMD_FTP_SUCCESS_RSP "CONNECT"
#define ELFIN_EG41A_AT_CMD_CME_ERROR       "+CME ERROR"
#define AT_CMD_STOP_FTP             "AT+QFTPCLOSE"
#define AT_CMD_STOP_FTP_DEACT       "AT+QIDEACT=1"

const uint8_t *CREG_SUCC_STR[] =
{
    "0,1",
    "0,5",
};
#define ELFIN_EG41A_DEFAULT_CMD_LEN    64
#define ELFIN_EG41A_DEFAULT_RSP_LEN    64
//0~11
#define ELFIN_EG41A_MAX_LINK_NUM       12

#define ELFIN_EG41A_DOMAIN_MAX_LEN     256
#define ELFIN_EG41A_DOMAIN_RSP_MAX_LEN 512
#define ELFIN_EG41A_DOMAIN_CMD_LEN (sizeof(AT_CMD_DOMAIN_TO_IP) + ELFIN_EG41A_DOMAIN_MAX_LEN + 1)

#define ELFIN_EG41A_CONN_CMD_LEN   (ELFIN_EG41A_DOMAIN_MAX_LEN + ELFIN_EG41A_DEFAULT_CMD_LEN)

#define ELFIN_EG41A_RETRY_MAX          3

extern gpio_dev_t brd_gpio_table[];


/* Change to include data slink for each link id respectively. <TODO> */
typedef struct link_s {
    int fd;
    aos_sem_t sem_start;
    aos_sem_t sem_close;
} link_t;

extern uint8_t inited;
static link_t g_link[ELFIN_EG41A_MAX_LINK_NUM];
extern aos_mutex_t g_link_mutex;
extern aos_mutex_t g_domain_mutex;
extern aos_sem_t   g_domain_sem;
extern aos_sem_t   g_ftp_start_sem;

static char  g_pcdomain_rsp[ELFIN_EG41A_DOMAIN_RSP_MAX_LEN];
static char  g_pccmd[ELFIN_EG41A_CONN_CMD_LEN];
extern netconn_data_input_cb_t g_netconn_data_input_cb;

void HAL_SAL_Check(void);

static int getgpsinfo(const char * src, char * dest, int idx, int destbufsize);

static int fd_to_linkid(int fd)
{
    int link_id;

    if (aos_mutex_lock(&g_link_mutex, AOS_WAIT_FOREVER) != 0) {
        LOG_INFO( "Failed to lock mutex (%s).", __func__);
        return -1;
    }
    for (link_id = 0; link_id < ELFIN_EG41A_MAX_LINK_NUM; link_id++) {
        if (g_link[link_id].fd == fd) {
            break;
        }
    }

    aos_mutex_unlock(&g_link_mutex);

    return link_id;
}

static void elfin_eg41a_gprs_qiopen_rsp_callback(void *arg, char *rspinfo, int rsplen)
{
//	    if (NULL == rspinfo || rsplen == 0) {
//	        LOG_INFO( "invalid input at %s \r\n", __func__);
//	        return;
//	    }
    int i = 0;
    do {
        at_read(&g_pcdomain_rsp[i], 1);
        if (g_pcdomain_rsp[i] == '\r' || g_pcdomain_rsp[i] == '\n') {
            break;
        }
        i++;
    } while (i<ELFIN_EG41A_DEFAULT_RSP_LEN);

    LOG_INFO( "qiopen rsp %s \r\n", g_pcdomain_rsp);
//	    memcpy(g_pcdomain_rsp, rspinfo, rsplen);
    aos_sem_signal(&g_domain_sem);
    return;
}


static void elfin_eg41a_gprs_qftpopen_rsp_callback(void *arg, char *rspinfo, int rsplen)
{
    int i = 0;
    do {
        at_read(&g_pcdomain_rsp[i], 1);
        if (g_pcdomain_rsp[i] == '\r' || g_pcdomain_rsp[i] == '\n') {
            break;
        }
        i++;
    } while (i < ELFIN_EG41A_DEFAULT_RSP_LEN);

    LOG_INFO( "qftp rsp %s \r\n", g_pcdomain_rsp);
//	    memcpy(g_pcdomain_rsp, rspinfo, rsplen);
    aos_sem_signal(&g_ftp_start_sem);
    return;
}

extern kbuf_queue_t * gst_FarpQueue;

static void elfin_eg41a_gprs_module_socket_data_handle(void *arg, char *rspinfo, int rsplen)
{
    unsigned char uclinkid = 0;
    unsigned char unusesymbol = 0;
    unsigned char datalen[6] = {0};
    unsigned char ipaddr[16] = {0};
    unsigned char port[6] = {0};
    int           i = 0;
    int           j = 0;
    int           len = 0;
    int           remoteport = 0;
    int           linkid = 0;
    char          *recvdata = NULL;

    at_read(&uclinkid, 1);
    linkid = uclinkid - '0';
    if (linkid < 0 || linkid >=  ELFIN_EG41A_MAX_LINK_NUM) {
        LOG_INFO( "Invalid link id 0x%02x !!!\r\n", linkid);
        return;
    }

    /*eat , char*/
    at_read(&unusesymbol, 1);

    /* get data len */
    i = 0;
    do {
        at_read(&datalen[i], 1);
        if (datalen[i] == '\r') {
            break;
        }
        if (i >= sizeof(datalen)) {
            LOG_INFO( "Too long length of data.datalen is %s \r\n", datalen);
            return;
        }
        if (datalen[i] > '9' || datalen[i] < '0') {
            LOG_INFO( "Invalid len string!!!, datalen is %s \r\n", datalen);
            return;
        }
        i++;
    } while (i < ELFIN_EG41A_DEFAULT_RSP_LEN);

    /* len: string to number */
    for (j = 0; j < i; j++) {
        len = len * 10 + datalen[j] - '0';
    }

    /*get ip addr and port*/
//	    i = 0;
//	    do {
//	        at_read(&ipaddr[i], 1);
//	        if (ipaddr[i] == ':') {
//	            break;
//	        }
//	        if (i >= sizeof(ipaddr)) {
//	            LOG_INFO( "Too long length of ipaddr.ipaddr is %s \r\n", ipaddr);
//	            return;
//	        }
//	
//	        if (!((ipaddr[i] <= '9' && ipaddr[i] >= '0') || ipaddr[i] == '.')) {
//	            LOG_INFO( "Invalid ipaddr string!!!, ipaddr is %s \r\n", ipaddr);
//	            return;
//	        }
//	        i++;
//	    } while (1);
//	
//	    ipaddr[i] = 0;
//	
//	    i = 0;
//	    do {
//	        at_read(&port[i], 1);
//	        if (port[i] == '\r') {
//	            break;
//	        }
//	        if (i >= sizeof(port)) {
//	            LOG_INFO( "Too long length of remote port.port is %s \r\n", port);
//	            return;
//	        }
//	
//	        if (port[i] > '9' || port[i] < '0') {
//	            LOG_INFO( "Invalid ipaddr string!!!, port is %s \r\n", port);
//	            return;
//	        }
//	        i++;
//	    } while (1);
//	
//	    port[i] = 0;
//	
//	    /*eat \n char*/
//	    at_read(&unusesymbol, 1);
//	
//	    for (j = 0; j < i; j++) {
//	        remoteport = remoteport * 10 + port[j] - '0';
//	    }
    /*eat \n char*/
    at_read(&unusesymbol, 1);

    /* Prepare socket data */
    recvdata = (char *)aos_malloc(len + 1);
    if (!recvdata) {
        LOG_INFO( "Error: %s %d out of memory.", __func__, __LINE__);
        return;
    }

    memset(recvdata, 0, len + 1);

    at_read(recvdata, len);

    if (g_netconn_data_input_cb && (g_link[linkid].fd >= 0)) {
        if (g_netconn_data_input_cb(g_link[linkid].fd, recvdata, len, ipaddr, remoteport)) {
            LOG_INFO( " %s socket %d get data len %d fail to post to sal, drop it\n",
                 __func__, g_link[linkid].fd, len);
        }
        
        uint8_t msg = MSG_IP1DATA;
        krhino_buf_queue_send(gst_FarpQueue, &msg, 1);
    }
    LOG_INFO( "socket data on link %d with length %d posted to sal\n", linkid, len);
    aos_free(recvdata);
    return;
}
//	static void elfin_eg41a_gprs_module_ftp_data_handle(void *arg, char *rspinfo, int rsplen)
//	{
//	    unsigned char uclinkid = 0;
//	    unsigned char unusesymbol = 0;
//	    unsigned char datalen[6] = {0};
//	    unsigned char ipaddr[16] = {0};
//	    unsigned char port[6] = {0};
//	    int           i = 0;
//	    int           j = 0;
//	    int           len = 0;
//	    int           remoteport = 0;
//	    int           linkid = 0;
//	    char          *recvdata = ftp_buf;
//	
//	
//	
//	    memset(recvdata, 0, len + 1);
//	
//	    at_read(recvdata, len);
//	
//	    if (g_netconn_data_input_cb && (g_link[linkid].fd >= 0)) {
//	        if (g_netconn_data_input_cb(g_link[linkid].fd, recvdata, len, ipaddr, remoteport)) {
//	            LOG_INFO( " %s socket %d get data len %d fail to post to sal, drop it\n",
//	                 __func__, g_link[linkid].fd, len);
//	        }
//	        
//	        uint8_t msg = MSG_IP1DATA;
//	        krhino_buf_queue_send(gst_FarpQueue, &msg, 1);
//	    }
//	    LOG_INFO( "%s socket data on link %d with length %d posted to sal\n",
//	         __func__, linkid, len);
//	    aos_free(recvdata);
//	    return;
//	}

static int elfin_eg41a_send_with_retry(const char *cmd, int cmdlen, bool delimiter, const char *data,
                                  int datalen, char *rspbuf, int buflen, const char *expectrsp, uint8_t times)
{
    int retry = 0;

    if (NULL == cmd || 0 == cmdlen || NULL == rspbuf ||
        0 == buflen || NULL == expectrsp) {
        LOG_INFO( "Invalid input %s %d\r\n", __func__, __LINE__);
        return -1;
    }

    while(true) {
        retry++;
        memset(rspbuf, 0, buflen);
        at_send_wait_reply(cmd, cmdlen, delimiter, data, datalen, rspbuf, buflen, NULL);
        if (strstr(rspbuf, expectrsp) == NULL) {
            if (retry > times) {
                return -1;
            }

            LOG_INFO( "%s %d failed rsp %s retry count %d\r\n", __func__, __LINE__, rspbuf, retry);
            aos_msleep(1000);
        } else {
            break;
        }
    }

    return 0;
}
static int elfin_eg41a_send_with_retry2(const char *cmd, int cmdlen, bool delimiter, const char *data,
                                  int datalen, char *rspbuf, int buflen, 
                                  const char **expectrsp, uint8_t rsparr, uint8_t times)
{
    int retry = 0;

    if (NULL == cmd || 0 == cmdlen || NULL == rspbuf ||
        0 == buflen || NULL == expectrsp) {
        LOG_INFO( "Invalid input %s %d\r\n", __func__, __LINE__);
        return -1;
    }

    while(true) {
        retry++;
        memset(rspbuf, 0, buflen);
        at_send_wait_reply(cmd, cmdlen, delimiter, data, datalen, rspbuf, buflen, NULL);
        int i = 0;
        for(i = 0; i < rsparr; i++)
        {
            if(strstr(rspbuf, expectrsp[i]) != NULL)
            {
                break;
            }
        }
        if (i >= rsparr) {
            if (retry > times) {
                return -1;
            }

            LOG_INFO( "%s %d failed rsp %s retry count %d\r\n", __func__, __LINE__, rspbuf, retry);
            aos_msleep(500);
        } else {
            break;
        }
    }

    return 0;
}

int elfin_eg41a_uart_selfadaption(const char *command, const char *rsp, uint32_t rsplen)
{
    char buffer[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (NULL == command || NULL == rsp || 0 == rsplen) {
        LOG_INFO("invalid input %s %d\r\n", __FILE__, __LINE__);
        return -1;
    }

    if (elfin_eg41a_send_with_retry(command, strlen(command), true, NULL, 0,
        buffer, ELFIN_EG41A_DEFAULT_RSP_LEN, rsp, 1) < 0) {
        return -1;
    }

    return 0;
}

int elfin_eg41a_uart_init(void)
{
    int ret = 0;
    char cmd[ELFIN_EG41A_DEFAULT_CMD_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    
    memset(g_pcdomain_rsp , 0, ELFIN_EG41A_DOMAIN_RSP_MAX_LEN);
    memset(g_link, 0, sizeof(g_link));
    
    at_reset_uart();

    for (int linknum = 0; linknum < ELFIN_EG41A_MAX_LINK_NUM; linknum++) {
        g_link[linknum].fd = -1;
    }

    /* uart baudrate self adaption*/
//	    ret = elfin_eg41a_uart_selfadaption(AT_CMD_TEST, AT_CMD_TEST_RESULT, strlen(AT_CMD_TEST_RESULT));
//	    if (ret) {
//	        LOG_INFO( "elfin_eg41a_uart_selfadaption fail \r\n");
//	        return ret;
//	    }
    /*turn off echo*/
    at_send_wait_reply(AT_CMD_ECHO_OFF, strlen(AT_CMD_ECHO_OFF), true,
                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        
        return -1;
    }

    /*set baudrate 115200*/
    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d", AT_CMD_BAUDRATE_SET, AT_UART_BAUDRATE);
    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0,
                       rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        
        return -1;
    }


//	    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    /*turn off flow control*/
//	    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d,%d", AT_CMD_FLOW_CONTROL, 0, 0);
//	    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0,
//	                       rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }

//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    /*save configuration */
//	    at_send_wait_reply(AT_CMD_SAVE_CONFIG, strlen(AT_CMD_SAVE_CONFIG), true,
//	                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }
    return 0;
}
#define CON_CSQ_CREG_RETRY_TIMES 20
int elfin_eg41a_gprs_status_check(uint8_t * sig, uint32_t bit)
{
    char cmd[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    int i = 0;
    if(bit & 0x01)
    {
        /*sim card status check*/
        if (elfin_eg41a_send_with_retry(AT_CMD_SIM_PIN_CHECK, strlen(AT_CMD_SIM_PIN_CHECK), true,
            NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
            LOG_INFO( "sim card status check failed\n");
            return -1;
        }
    }
    
    if(bit & 0x02)
    {
        for(i = 0; i < CON_CSQ_CREG_RETRY_TIMES; i++)
        {
            memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
            /*Signal quaility check*/
            at_send_wait_reply(AT_CMD_SIGNAL_QUALITY_CHECK, strlen(AT_CMD_SIGNAL_QUALITY_CHECK), true,
                               NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
            if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
                LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
                return -1;
            }
            LOG_INFO( "signal quality is %s \r\n", rsp);
            uint8_t aret[2] = {0};
            if(DecodeStrTwoChar(rsp,aret)!=0)
            {
                LOG_INFO( "signal decompress faliled! \r\n");
                return -1;
            }
            if(aret[0] < 99)
            {
                *sig = aret[0];
                break;
            }
            aos_msleep(4000);
        }
        if(i >= CON_CSQ_CREG_RETRY_TIMES)
        {
            LOG_INFO( "signal get failed!\r\n", __func__, __LINE__);
            return -1;
        }
    }
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    /*network registration check*/
//	    at_send_wait_reply(AT_CMD_NETWORK_REG_CHECK, strlen(AT_CMD_NETWORK_REG_CHECK), true,
//	                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }
//	    
//	    LOG_INFO( "network registration is %s \r\n", rsp);
    if(bit & 0x04)
    {
        memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
        memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
        snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_STATUS_CONTROL_CHECK,"\"gprsattach\"");
        if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
            NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
            LOG_INFO( "%s failed\n", cmd);
            return -1;
        }
//	
//	        memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	        memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
//	        snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_STATUS_CONTROL_CHECK,"\"nwscanmode\"");
//	        if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
//	            NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
//	            LOG_INFO( "%s failed\n", cmd);
//	            return -1;
//	        }
//	        memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	        memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
//	        snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_STATUS_CONTROL_CHECK,"\"nwscanseq\"");
//	        if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
//	            NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
//	            LOG_INFO( "%s failed\n", cmd);
//	            return -1;
//	        }
    }
//	    aos_msleep(3000);
    
    if(bit & 0x08)
    {
        for(i = 0; i < CON_CSQ_CREG_RETRY_TIMES; i++)
        {
            
            aos_msleep(5000);
            if (elfin_eg41a_send_with_retry2(AT_CMD_NETWORK_REG_CHECK, strlen(AT_CMD_NETWORK_REG_CHECK), true,
                NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, CREG_SUCC_STR, 2, 0) < 0) {
                
                continue;
            }
            else
                {break;}
        }
        if(i >= CON_CSQ_CREG_RETRY_TIMES)
        {
            LOG_INFO( "network registration failed\n");
            return -1;
        }
        LOG_INFO( "network registration is %s \r\n", rsp);
        for(i = 0; i < CON_CSQ_CREG_RETRY_TIMES; i++)
        {
            
            aos_msleep(3000);

            memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
            /*GPRS attach check*/
            if (elfin_eg41a_send_with_retry(AT_CMD_GPRS_ATTACH_CHECK, strlen(AT_CMD_GPRS_ATTACH_CHECK), true,
                NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, AT_CMD_ELFIN_EG41A_CGATT_1, 0) < 0) {
                
                continue;
            }
            else
            {
                break;
            }
        }
        if(i >= CON_CSQ_CREG_RETRY_TIMES)
        {
            LOG_INFO( "gprs attach check failed\n");
            return -1;
        }
    }
//	    at_send_wait_reply(AT_CMD_GPRS_ATTACH_CHECK, strlen(AT_CMD_GPRS_ATTACH_CHECK),true,
//	                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }

    LOG_INFO( "gprs attach check %s \r\n", rsp);

    return 0;
}

int elfin_eg41a_gprs_ip_init(void)
{
    char cmd[ELFIN_EG41A_DEFAULT_CMD_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};



    /*Deactivate GPRS PDP Context*/
    if (elfin_eg41a_send_with_retry(AT_CMD_GPRS_PDP_DEACTIVE, strlen(AT_CMD_GPRS_PDP_DEACTIVE), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "Deactivate GPRS PDP Context failed\n");
        
        return -1;
    }
    if (elfin_eg41a_send_with_retry(AT_CMD_GPRS_PDP_ACTIVE, strlen(AT_CMD_GPRS_PDP_ACTIVE), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "Deactivate GPRS PDP Context failed\n");
        
        return -1;
    }

    /*get operator*/
    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    if (elfin_eg41a_send_with_retry(AT_CMD_MISI, strlen(AT_CMD_MISI), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "get operator failed\n");
        
        return -1;
    }
        
    memset(cmd,0,ELFIN_EG41A_DEFAULT_CMD_LEN);
    if(strstr(rsp,"46000") || strstr(rsp,"46002") || strstr(rsp,"46007") || strstr(rsp,"46004"))
    {
        
        snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=1,1,\"%s\",\"\",\"\"", AT_CMD_APN, AT_CMD_APN_CHINAMOBILE);
    }
    else if(strstr(rsp,"46001") || strstr(rsp,"46006"))
    {
        
        snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=1,1,\"%s\",\"\",\"\"", AT_CMD_APN, AT_CMD_APN_CHINAUNINET);
    }
    else
    {
    
        snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=1,1,\"%s\",\"\",\"\"", AT_CMD_APN, AT_CMD_APN_CHINATELECOM);
    
    }
    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "set apn failed\n");
        
        return -1;
    }
//	    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
//	    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=1", AT_CMD_START_CLIENT_ACT);
//	    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "pccmd %s fail, rsp %s \r\n", cmd, rsp);
//	        
//	        return -1;
//	    }

    /*set multi ip connection mode*/
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d", AT_CMD_MULTI_IP_CONNECTION, 1);
//	    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }

    /*not prompt echo > when sending data*/
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
//	    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d", AT_CMD_SEND_DATA_PROMPT_SET, 0);
//	    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }

    /*Show Remote ip and port when receive data*/
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
//	    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d", AT_CMD_RECV_DATA_FORMAT_SET, 1);
//	    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	        return -1;
//	    }

    /* reg oob for domain and packet input*/
    at_register_callback(AT_CMD_CLIENT_CONNECT_RET, NULL, ELFIN_EG41A_DEFAULT_RSP_LEN,
                         elfin_eg41a_gprs_qiopen_rsp_callback, NULL);
    at_register_callback(AT_CMD_DATA_RECV, NULL, 0, elfin_eg41a_gprs_module_socket_data_handle, NULL);


    return 0;
}

static void elfin_eg41a_get_ip_delayed_action(void *arg)
{
    LOG_INFO( "post got ip event \r\n");
    aos_post_event(EV_WIFI, CODE_WIFI_ON_GOT_IP, 0xdeaddead);
}


int elfin_eg41a_gprs_got_ip(uint8_t * ip)
{
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    atcmd_config_t atcmd_config = {NULL, AT_RECV_PREFIX, NULL};

    /*start gprs stask*/
    if (elfin_eg41a_send_with_retry(AT_CMD_START_TASK, strlen(AT_CMD_START_TASK), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        return -1;
    }
	uint8_t * p = strstr(rsp, "\"");

    if(0 == DecodeStrFourChar(p, ip))
    {
//	        memcpy(ip, wret.byte, 4);
        return 0;
    }
    else
    {
        LOG_INFO( "decode ip addr failed\r\n", __func__, __LINE__);
        return -1;
    }
//	    /*bring up wireless connectiong with gprs*/
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    at_send_wait_reply(AT_CMD_BRING_UP_GPRS_CONNECT, strlen(AT_CMD_BRING_UP_GPRS_CONNECT), true,
//	                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	    }
//		
//	    /*try to got ip*/
//	    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	    at_send_wait_reply(AT_CMD_GOT_LOCAL_IP, strlen(AT_CMD_GOT_LOCAL_IP), true,
//	                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, &atcmd_config);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_FAIL_RSP) != NULL) {
//	        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
//	    }
//		
//	    /*delay 5 seconds to post got ip event*/
//	    aos_post_delayed_action(5000, elfin_eg41a_get_ip_delayed_action, NULL);
    //return 0;
}

static int elfin_eg41a_gprs_get_ip_only()
{
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    atcmd_config_t atcmd_config = {NULL, AT_RECV_PREFIX, NULL};
    at_send_wait_reply(AT_CMD_GOT_LOCAL_IP, strlen(AT_CMD_GOT_LOCAL_IP), true,
                       NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, &atcmd_config);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_FAIL_RSP) != NULL) {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        return -1;
    }
    return 0;
}

#define AT_CMD_GETENGMODE   "AT+CENG?"
#define AT_CMD_SETENGMODE   "AT+CENG=3,1"
#define AT_CMD_CLOSEENGMODE "AT+CENG=0,0"

#define AT_CMD_GETENGMODE_LEN  512
int32_t elfin_eg41a_get_gprs_network_info(char * bts, int btslen, char * neighterbts, int nbtslen)
{
    char rsp[AT_CMD_GETENGMODE_LEN] = {0};

    memset(bts,         0, btslen);
    memset(neighterbts, 0, nbtslen);

    if (!inited)
    {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
    }

    /* set engineering mode to engineering mode with neighbering cell info */
    memset(rsp, 0, sizeof(rsp));
    at_send_wait_reply(AT_CMD_SETENGMODE, strlen(AT_CMD_SETENGMODE), true,
                       NULL, 0, rsp, AT_CMD_GETENGMODE_LEN, NULL);

    if (strstr(rsp, ELFIN_EG41A_AT_CMD_FAIL_RSP) != NULL)
    {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        return -1;
    }

    /* get cell info */

    memset(rsp, 0, sizeof(rsp));
    at_send_wait_reply(AT_CMD_GETENGMODE, strlen(AT_CMD_GETENGMODE), true,
                       NULL, 0, rsp, AT_CMD_GETENGMODE_LEN, NULL);

    if (strstr(rsp, ELFIN_EG41A_AT_CMD_FAIL_RSP) != NULL)
    {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        return -1;
    }

    LOG_INFO( "%s %d rsp %s\r\n", __func__, __LINE__, rsp);

    char cellinfo_prefix_buf[64] = {"+CENG: 0,"};

    char * p_start = strstr(rsp, cellinfo_prefix_buf);

    if (p_start != NULL)
    {
        int cell = -1;
        int mcc  = 0;
        int mnc;
        int lac;
        int cellid;
        int bsic;
        int rxl;

        /* <cell>,<mcc>,<mnc>,<lac>,<cellid>,<bsic>,<rxl> */
        sscanf(p_start,
               "+CENG: %d,\"%d,%d,%x,%x,%d,%d\"",
               &cell, &mcc, &mnc, &lac, &cellid, &bsic, &rxl);

        if (cell != 0)
        {
            LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
            return -1;
        }

        int signal;
        int first_neighber = 1;

        while (mcc != 0)
        {
            signal = rxl - 113; /* signal strength = 2 * rxl - 113 dbm */

            /* -113 < signal < 0 */
            signal = signal > 0 ? 0 : (signal < (-113) ? -113 : signal);

            if (cell == 0)
            {
                /* mcc,mnc,lac,cellid,signal */
                snprintf(bts, btslen, "%d,%d,%d,%d,%d", mcc, mnc, lac, cellid, signal);
            }
            else
            {
                if (first_neighber)
                {
                    /* mcc,mnc,lac,cellid,signal */
                    snprintf(neighterbts, nbtslen, "%d,%d,%d,%d,%d", mcc, mnc, lac, cellid, signal);
                    first_neighber = 0;
                }
                else
                {
                    /* mcc,mnc,lac,cellid,signal */
                    snprintf(neighterbts + strlen(neighterbts), nbtslen - strlen(neighterbts), "|%d,%d,%d,%d,%d", mcc, mnc, lac, cellid, signal);
                }
            }

            memset(cellinfo_prefix_buf, 0, sizeof(cellinfo_prefix_buf));
            sprintf(cellinfo_prefix_buf, "+CENG: %d,", cell + 1);

            p_start = strstr(p_start, cellinfo_prefix_buf);

            if (p_start == NULL)
            {
                break;
            }

            mcc = 0;
            sscanf(p_start,
                   "+CENG: %d,\"%d,%d,%x,%x,%d,%d\"",
                   &cell, &mcc, &mnc, &lac, &cellid, &bsic, &rxl);
        }

    }

    /* close engineering mode */
    memset(rsp, 0, sizeof(rsp));
    at_send_wait_reply(AT_CMD_CLOSEENGMODE, strlen(AT_CMD_CLOSEENGMODE), true,
                       NULL, 0, rsp, AT_CMD_GETENGMODE_LEN, NULL);

    if (strstr(rsp, ELFIN_EG41A_AT_CMD_FAIL_RSP) != NULL)
    {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        return -1;
    }
    return 0;
    /* restore original engineering mode */
}

#define AT_CMD_GPS_DEFAULT_RSP_LEN         (256)
#define AT_CMD_GPS_POWER_OFF         "AT+CGNSPWR=0"
#define AT_CMD_GPS_POWER_ON          "AT+CGNSPWR=1"
#define AT_CMD_GPS_POWER_CHECK       "AT+CGNSPWR?"

#define AT_CMD_GPS_INTERVAL_CLOSE    "AT+CGNSURC=0"

#define AT_CMD_GPS_LASTPARSE_SET     "AT+CGNSSEQ=\"RMC\""
#define AT_CMD_GPS_POSITION_GET      "AT+CGNSINF"
#define GPS_TYPE_NAME_LEN            (16)
#define GET_GPS_INFO_MIN_NUM         (6)
#define SATELLITESINVIEWIDX 14
#define SATELLITESUSEDIDX   15
void elfin_eg41a_get_gps(float * latitude, float * longitude, float * altitude)
{
    int ret = 0;
    char rsp[AT_CMD_GPS_DEFAULT_RSP_LEN] = {0};

    *latitude = 90; /* north pole */
    *altitude = 0;

    if (!inited) {
        return;
    }

    memset(rsp, 0, sizeof(rsp));
    ret = at_send_wait_reply(AT_CMD_GPS_POWER_ON, strlen(AT_CMD_GPS_POWER_ON),
                             true, NULL, 0, rsp, AT_CMD_GPS_DEFAULT_RSP_LEN, NULL);

    if ((0 != ret) || (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL))
    {
        LOG_INFO( "%s %d failed rsp %s errno %d\r\n", __func__, __LINE__, rsp,ret);
        return;
    }

    memset(rsp, 0, sizeof(rsp));
    ret = at_send_wait_reply(AT_CMD_GPS_LASTPARSE_SET, strlen(AT_CMD_GPS_LASTPARSE_SET),
                             true, NULL, 0, rsp, AT_CMD_GPS_DEFAULT_RSP_LEN, NULL);

    if ((0 != ret) || (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL))
    {
        LOG_INFO( "%s %d failed rsp %s errno %d\r\n", __func__, __LINE__, rsp,ret);
        return;
    }

    memset(rsp, 0, sizeof(rsp));
    ret = at_send_wait_reply(AT_CMD_GPS_INTERVAL_CLOSE, strlen(AT_CMD_GPS_INTERVAL_CLOSE),
                             true, NULL, 0, rsp, AT_CMD_GPS_DEFAULT_RSP_LEN, NULL);

    if ((0 != ret) || (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL))
    {
        LOG_INFO( "%s %d failed rsp %s errno %d\r\n", __func__, __LINE__, rsp,ret);
        return;
    }

    memset(rsp, 0, sizeof(rsp));
    ret = at_send_wait_reply(AT_CMD_GPS_POSITION_GET, strlen(AT_CMD_GPS_POSITION_GET),
                             true, NULL, 0, rsp, AT_CMD_GPS_DEFAULT_RSP_LEN, NULL);

    if ((0 != ret) || (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL))
    {
        LOG_INFO( "%s %d failed rsp %s errno %d\r\n", __func__, __LINE__, rsp,ret);
        return;
    }

    LOG_INFO( "%s %d rsp %s errno %d\r\n", __func__, __LINE__, rsp,ret);


    /* +CGNSINF: <GNSS run status>,<Fix status>,
                 <UTC date & Time>,<Latitude>,<Longitude>, <MSL Altitude>,
                 <Speed Over Ground>, <Course Over Ground>,
                 <Fix Mode>,<Reserved1>,<HDOP>,
                 <PDOP>, <VDOP>,<Reserved2>,
                 <GNSS Satellites in View>, <GNSS Satellites Used>,
                 <GLONASS Satellites Used>,<Reserved3>,<C/N0 max>,<HPA>,<VPA> */

    char tmp_buf[128];
    char tmp_lat[32];
    char tmp_log[32];
    char tmp_alt[32];
    char tmp_gnssrunstatus[32];
    char tmp_satellites_used[32];
    char tmp_satellites_inview[32];

    memset(tmp_lat, 0, sizeof(tmp_lat));
    memset(tmp_log, 0, sizeof(tmp_log));
    memset(tmp_alt, 0, sizeof(tmp_alt));
    memset(tmp_gnssrunstatus, 0, sizeof(tmp_gnssrunstatus));
    memset(tmp_satellites_used, 0, sizeof(tmp_satellites_used));
    memset(tmp_satellites_inview, 0, sizeof(tmp_satellites_inview));

    ret = sscanf(rsp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
                 tmp_gnssrunstatus, tmp_buf, tmp_buf, tmp_lat, tmp_log, tmp_alt, tmp_buf,  /* <Speed Over Ground>*/
                 tmp_buf, tmp_buf, tmp_buf, tmp_buf, tmp_buf, tmp_buf, tmp_buf,
                 tmp_satellites_inview, tmp_satellites_used, tmp_buf, tmp_buf, tmp_buf, tmp_buf, tmp_buf);

    getgpsinfo(rsp, tmp_satellites_inview, SATELLITESINVIEWIDX, sizeof(tmp_satellites_inview));
    getgpsinfo(rsp, tmp_satellites_used,   SATELLITESUSEDIDX,   sizeof(tmp_satellites_used));

    int satellitesinview = atoi(tmp_satellites_inview);
    int satellitesused   = atoi(tmp_satellites_used);

    if (ret >= GET_GPS_INFO_MIN_NUM && satellitesinview && satellitesused)
    {
        *latitude  = (float)atof(tmp_lat);
        *longitude = (float)atof(tmp_log);
        *altitude  = (float)atof(tmp_alt);
        *altitude = *altitude > 0 ? *altitude : 0;
    }
}

static int getgpsinfo(const char * src, char * dest, int idx, int destbufsize)
{
    int srclen = strlen(src);
    int roopidx = 0;
    int cnt = 0;
    int bufcnt = 0;
    char * start;

    while (roopidx < srclen) {
        if (src[roopidx] == ',') {
            cnt++;
            if (cnt == idx) {
                break;
            }
        }
        roopidx++;
    }

    roopidx++;

    while ((src[roopidx] != ',') && (bufcnt < destbufsize)) {
        *dest++ = src[roopidx];
        roopidx++;
        bufcnt++;
    }

    return 0;
}




/************************************************************************
 * @function: HAL_SAL_ModemOn
 * @描述: 开机开机(执行后将占用串口)
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void HAL_SAL_ModemOn(void)
void elfin_eg41a_modemon(void)

{    

//	    _ATMODEM_VIN();
  
  //初始值
    
    SYS_GPO_Out(GPIO_MODEM_ONOFF, false);//hal_gpio_output_low(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_0();
                                            //复位(或重新上电)
    SYS_GPO_Out(GPIO_MODEM_PWR, true);//hal_gpio_output_high(&brd_gpio_table[GPIO_MODEM_PWR]);//_ATMODEM_PWRON();
    //_ATMODEM_PORTOPEN();
                                            //开机时序
    aos_msleep(1000);
    SYS_GPO_Out(GPIO_MODEM_ONOFF, true);//hal_gpio_output_high(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_1();
    aos_msleep(200);
    SYS_GPO_Out(GPIO_MODEM_ONOFF, false);//hal_gpio_output_low(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_0();
    aos_msleep(12000);

}
/************************************************************************
 * @function: HAL_SAL_ModemOff
 * @描述: 关机操作
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void elfin_eg41a_modemoff(void)
{
//	    hal_gpio_output_high(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_1();
//	    aos_msleep(1000);
//	    hal_gpio_output_low(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_0();
//	    aos_msleep(30000);

//	    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
//	    
//	
//	    /*turn off */
//	    if (elfin_eg41a_send_with_retry(AT_CMD_ELFIN_EG41A_PWR_DOWN, strlen(AT_CMD_ELFIN_EG41A_PWR_DOWN), true,
//	        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, 0) < 0) {
//	        LOG_INFO( "elfin_eg41a Power off failed\n");
//	        
//	          hal_gpio_output_high(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_1();
//	          aos_msleep(1000);
//	          hal_gpio_output_low(&brd_gpio_table[GPIO_MODEM_ONOFF]);//_ATMODEM_ONOFF_0();
//	        
//	    }
//	
//	    aos_msleep(30000);

    
    SYS_GPO_Out(GPIO_MODEM_PWR, false);//hal_gpio_output_low(&brd_gpio_table[GPIO_MODEM_PWR]);

    aos_msleep(1000);
}

void elfin_eg41a_forcesoff(void)
{
    SYS_GPO_Out(GPIO_MODEM_PWR, false);//hal_gpio_output_low(&brd_gpio_table[GPIO_MODEM_PWR]);
//	    aos_msleep(1);
}
/************************************************************************
 * @function: SYS_M72X_Check
 * @描述: 查询模块标识是否匹配
 * @参数: 
 * 
 * @返回: 
 * @return: bool true成功,false失败(成功后不关闭串口,将以识别到的串口配置长期占用)
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int elfin_eg41a_modemcheck(void)
{
    int ret = 0;
    /* uart baudrate self adaption*/
    //at_reset_uart();
    ret = elfin_eg41a_uart_selfadaption(AT_CMD_ELFIN_EG41A_TEST, AT_CMD_ELFIN_EG41A_TEST_RESULT, strlen(AT_CMD_TEST_RESULT));
    if (ret) {
        LOG_ERROR("elfin_eg41a_uart_selfadaption fail \r\n");
        return ret;
    }
    
    return ret;                            //返回成功
}
int elfin_eg41a_domaintoip(char *domain, char ip[16])
{
    return -1;
}
#if 0
int elfin_eg41a_domaintoip(char *domain, char ip[16])
{
    char *pccmd = NULL;
    char *head = NULL;
    char *end = NULL;
    int count = 0;
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }

    if (NULL == domain || NULL == ip) {
        LOG_INFO( "invalid input at %s \r\n", __func__);
        return -1;
    }

    if (strlen(domain) > ELFIN_EG41A_DOMAIN_MAX_LEN) {
        LOG_INFO( "domain length oversize at %s \r\n", __func__);
        return -1;
    }

    pccmd = g_pccmd;
    if (NULL == pccmd) {
        LOG_INFO( "fail to malloc memory %d at %s \r\n", ELFIN_EG41A_DOMAIN_CMD_LEN, __func__);
        return -1;
    }

    memset(pccmd, 0, ELFIN_EG41A_DOMAIN_CMD_LEN);
    snprintf(pccmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_DOMAIN_TO_IP, domain);

    aos_mutex_lock(&g_domain_mutex, AOS_WAIT_FOREVER);
restart:
    count++;
    if (count > ELFIN_EG41A_RETRY_MAX) {
        LOG_INFO("domain to ip retry failed!\r\n");
        aos_mutex_unlock(&g_domain_mutex);//HAL_MutexUnlock(g_domain_mutex);
        return -1;
    }

    at_send_wait_reply(pccmd, strlen(pccmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        goto err;
    }

    /*TODO wait for reponse for ever for now*/
    //aos_sem_wait(&g_domain_sem, AOS_WAIT_FOREVER);
    /*
     * formate is :
       +CDNSGIP: 1,"www.baidu.com","183.232.231.173","183.232.231.172"
       or :
       +CDNSGIP: 0,8
    */
//	    if ((head = strstr(g_pcdomain_rsp, domain)) == NULL) 
    if ((head = strstr(rsp, domain)) == NULL){
        LOG_INFO( "invalid domain rsp %s at %d\r\n", g_pcdomain_rsp, __LINE__);
        goto err;
    }

    head += (strlen(domain) + 3);
    if ((end = strstr(head, "\"")) == NULL) {
        LOG_INFO( "invalid domain rsp head is %s at %d\r\n", head, __LINE__);
        goto err;
    }

    if ((end - head) > 15 || (end - head) < 7) {
        LOG_INFO( "invalid domain rsp head is %s at %d\r\n", head, __LINE__);
        goto err;
    }

    /* We find a good IP, save it. */
    memcpy(ip, head, end - head);
    ip[end - head] = '\0';
    memset(g_pcdomain_rsp, 0, ELFIN_EG41A_DOMAIN_RSP_MAX_LEN);
    aos_mutex_unlock(&g_domain_mutex);

    LOG_INFO( "domain %s get ip %s \r\n", domain , ip);
    return 0;
err:
    if (elfin_eg41a_gprs_get_ip_only() != 0) {
        elfin_eg41a_gprs_ip_init();
        elfin_eg41a_gprs_got_ip();
        goto restart;
    }

    memset(g_pcdomain_rsp, 0, ELFIN_EG41A_DOMAIN_RSP_MAX_LEN);
    aos_mutex_unlock(&g_domain_mutex);

    return -1;

}
#endif
/************************************************************************
 * @function: HAL_SAL_Check
 * @描述: 初始化Modem,自动判断模块类型
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void HAL_SAL_Check(void)
//	{
//		uint32_t i = 0;                           //循环变量
//		
//		while(true)                             //循环检测模块
//		{
//	        aos_msleep(1000);       //开机前延时
//	        
//			elfin_eg41a_modemon();
//	
//			aos_msleep(3000);   //开机后延时
//			
//			if(!elfin_eg41a_modemcheck())       //确认是否为当前选定的模块
//			{
//	
//	            
//	//				DevModem->drive = (TModemDrive*)&gs_MODMDrvIntf[i];
//	//				DevModem->stt.bit.typeChecked = 1;
//				break;
//			}
//	
//	//			i++;                                //循环检测下一个
//	//			if(i >= sizeof(gs_MODMDrvIntf)/sizeof(TModemDrive))
//	//			{
//	//				i = 0;
//	//			}
//			elfin_eg41a_modemoff();            //关闭模块
//	        
//		} 
//	
//	
//		LOG_INFO("Modem recognized! \nBegin to init AT channel!\n");    
//	    
//	}

int elfin_eg41a_start(sal_conn_t *conn)
{
    int  linkid = 0;
    char *pccmd = NULL;
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }

    if (!conn || !conn->addr) {
        LOG_INFO( "%s %d - invalid input \r\n", __func__, __LINE__);
        return -1;
    }

    aos_mutex_lock(&g_link_mutex, AOS_WAIT_FOREVER);
    for (linkid = 0; linkid < ELFIN_EG41A_MAX_LINK_NUM; linkid++) {
        if (g_link[linkid].fd >= 0) {
            continue;
        }
//	        g_link[linkid].fd = conn->fd;
        break;
    }
    aos_mutex_unlock(&g_link_mutex);

    if (linkid >= ELFIN_EG41A_MAX_LINK_NUM) {
        LOG_INFO( "No link available for now, %s failed. \r\n", __func__);
        return -1;
    }

    pccmd = g_pccmd;
    if (NULL == pccmd) {
        LOG_INFO( "fail to malloc %d at %s \r\n", ELFIN_EG41A_CONN_CMD_LEN, __func__);
        goto err;
    }
    memset(pccmd, 0, ELFIN_EG41A_CONN_CMD_LEN);

//	    snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=%d", AT_CMD_START_CLIENT_DEACT, linkid);
//	    at_send_wait_reply(pccmd, strlen(pccmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
//	        LOG_INFO( "pccmd %s fail, rsp %s \r\n", pccmd, rsp);
//	        goto err;
//	    }
    

    memset(pccmd, 0, ELFIN_EG41A_CONN_CMD_LEN);

    switch (conn->type) {
        case TCP_SERVER:
//	            snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=%d,%d", AT_CMD_START_TCP_SERVER, 1, conn->l_port);
            snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=1,%d,\"TCP LISTENER\",\"127.0.0.1\",0,%d,1", AT_CMD_START_CLIENT_CONN, linkid, 
                     conn->r_port);

            at_send_wait_reply(pccmd, strlen(pccmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
            if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
                LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
                goto err;
            }
            break;
        case TCP_CLIENT:
          {
            snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=1,%d,\"TCP\",\"%s\",%d,0,1", AT_CMD_START_CLIENT_CONN, linkid, conn->addr,
                     conn->r_port);
//	            uint8_t suc[ELFIN_EG41A_DEFAULT_RSP_LEN];
//	            
//	            memset(suc, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
//	            snprintf(suc, ELFIN_EG41A_CONN_CMD_LEN - 1, "%d,0",  linkid);
            //at_set_timeout(600000);
            atcmd_config_t atcmd_config_client = { NULL, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, NULL};
            //atcmd_config_client.reply_success_postfix = &suc[0];
            at_send_wait_reply(pccmd, strlen(pccmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, &atcmd_config_client);
//	            if (strstr(rsp, AT_CMD_CLIENT_CONNECT_RET) == NULL) {
//	                LOG_INFO( "pccmd %s fail, rsp %s \r\n", pccmd, rsp);
//	                goto err;
//	            }
            //aos_msleep(150000);
            /*TODO wait for reponse for ever for now*/
            memset(g_pcdomain_rsp,0,64);
            krhino_sem_count_set(g_domain_sem.hdl, 0);

            int ret = aos_sem_wait(&g_domain_sem, 60000);

            uint8_t retid = 0xFF;
            uint16_t retflag = 0xFF;
            if(ret != 0 ||
                DecodeStrOneChar(g_pcdomain_rsp,&retid)!=0 || retid >= ELFIN_EG41A_MAX_LINK_NUM ||
                DecodeStrOneShort(g_pcdomain_rsp+2,&retflag)!=0 || retflag != 0)
            {
                LOG_INFO( "pccmd %s fail, rsp %s %s %d,%d \r\n", 
                    pccmd, AT_CMD_CLIENT_CONNECT_RET, rsp, retid, retflag);

                if(retflag == 561)
                {
                    uint8_t msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(gst_FarpQueue, &msg, 1);
                }
                goto err;
            }
            else
            {
//	                retid != linkid || 
                g_link[retid].fd = conn->fd;

            }
//	            at_set_timeout(10000);
            break;
          }
        case UDP_UNICAST:
          {
//	            snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=%d,\"UDP\",\"%s\",%d", AT_CMD_START_CLIENT_CONN, linkid, conn->addr,
//	                     conn->r_port);
            snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=1,%d,\"UDP\",\"%s\",%d,0,1", AT_CMD_START_CLIENT_CONN, linkid, conn->addr,
                     conn->r_port);

            atcmd_config_t atcmd_config_unicast = { NULL, AT_CMD_CLIENT_CONNECT_OK, AT_CMD_CLIENT_CONNECT_FAIL};
            at_send_wait_reply(pccmd, strlen(pccmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
//	            if (strstr(rsp, AT_CMD_CLIENT_CONNECT_FAIL) != NULL) {
//	                LOG_INFO( "pccmd %s fail, rsp %s \r\n", pccmd, rsp);
//	                goto err;
//	            }
            if (strstr(rsp, AT_CMD_CLIENT_CONNECT_RET) == NULL) {
                LOG_INFO( "pccmd %s fail, rsp %s \r\n", pccmd, rsp);
                goto err;
            }
            uint8_t wret[4] = {0};
            if(DecodeStrTwoChar(rsp,wret)!=0 || wret[0] != linkid || wret[1] != 0)
            {
                LOG_INFO( "pccmd %s fail, rsp %s \r\n", pccmd, rsp);
                goto err;
            }

            break;
          }
        case SSL_CLIENT:
        case UDP_BROADCAST:
        default:
            LOG_INFO( "elfin_eg41a gprs module connect type %d not support \r\n", conn->type);
            goto err;
    }

    return 0;
err:
    aos_mutex_lock(&g_link_mutex, AOS_WAIT_FOREVER);
    g_link[linkid].fd = -1;
    aos_mutex_unlock(&g_link_mutex);
    return -1;
}

int elfin_eg41a_close(int fd, int32_t remote_port)
{
    int  linkid = 0;
    int  ret = 0;
    char cmd[ELFIN_EG41A_DEFAULT_CMD_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }

    linkid = fd_to_linkid(fd);
    if (linkid < 0 || linkid >= ELFIN_EG41A_MAX_LINK_NUM) {
        LOG_INFO( "No connection found for fd (%d) in %s \r\n", fd, __func__);
        return -1;
    }

    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d", AT_CMD_STOP_CONN, linkid);
    at_send_wait_reply(cmd, strlen(cmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
        LOG_INFO( "cmd %s rsp is %s \r\n", cmd, rsp);
        ret = -1;
    }

    aos_mutex_lock(&g_link_mutex, AOS_WAIT_FOREVER);
    g_link[linkid].fd = -1;
    aos_mutex_unlock(&g_link_mutex);

    return ret;
}

int elfin_eg41a_send(int fd,
                 uint8_t *data,
                 uint32_t len,
                 char remote_ip[16],
                 int32_t remote_port,
                 int32_t timeout)
{
    int  linkid;
    char cmd[ELFIN_EG41A_DEFAULT_CMD_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }

    linkid = fd_to_linkid(fd);
    if (linkid < 0 || linkid >= ELFIN_EG41A_MAX_LINK_NUM) {
        LOG_INFO( "No connection found for fd (%d) in %s \r\n", fd, __func__);
        return -1;
    }

    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%d,%d", AT_CMD_SEND_DATA, linkid, len);



    /*TODO data send fail rsp is SEND FAIL*/
//	    if (elfin_eg41a_send_with_retry((const char *)cmd, strlen(cmd), true, NULL, 0,
//	        rsp, sizeof(rsp), ">", 0) < 0) {
//	        LOG_INFO( "cmd %s rsp %s at %s %d failed \r\n", cmd, rsp, __func__, __LINE__);
//	        return -1;
//	    }
    atcmd_config_t atcmd_config_client = { NULL, ">", NULL};

    at_send_wait_reply((const char *)cmd, strlen(cmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, &atcmd_config_client);
    if (strstr(rsp, ">") == NULL) {
        LOG_INFO( "tcpip cmd %s fail, rsp %s \r\n", cmd, rsp);
        return -1;
    }


    at_send_wait_reply(data, len, true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, NULL);
    if (strstr(rsp, AT_CMD_SEND_OK) == NULL) {
        LOG_INFO( "tcpip send data %s fail, rsp %s!!! \r\n", data, rsp);
        return -1;
    }


    return 0;
}

int elfin_eg41a_ftp_status_check(uint8_t * usr, uint8_t * pwd)
{
    char cmd[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    int i = 0;

    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_FTP_CFG,AT_CMD_FTP_CFG_CNT);
    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "ftp context id attach failed![%s]\n", rsp);
        return -1;
    }


    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s,\"%s\",\"%s\"", AT_CMD_FTP_CFG,AT_CMD_FTP_CFG_ACOUNT,usr,pwd);
    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "ftp account set failed![%s]\n", rsp);
        return -1;
    }
    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_FTP_CFG,AT_CMD_FTP_CFG_FT);
    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "ftp file type set failed![%s]\n", rsp);
        return -1;
    }

    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_FTP_CFG,AT_CMD_FTP_CFG_MD);
    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "ftp trans mode set failed![%s]\n", rsp);
        return -1;
    }

    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=%s", AT_CMD_FTP_CFG,AT_CMD_FTP_CFG_TO);
    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "ftp rsp time out [%s]\n", rsp);
        return -1;
    }


        
    at_register_callback(AT_CMD_FTP_CONNECT_RET, NULL, ELFIN_EG41A_DEFAULT_RSP_LEN,
                         elfin_eg41a_gprs_qftpopen_rsp_callback, NULL);
    at_register_callback(AT_CMD_FTP_FOLD_RET, NULL, ELFIN_EG41A_DEFAULT_RSP_LEN,
                         elfin_eg41a_gprs_qftpopen_rsp_callback, NULL);

    at_register_callback(AT_CMD_FTP_GET_RET, NULL, ELFIN_EG41A_DEFAULT_RSP_LEN,
                         elfin_eg41a_gprs_qftpopen_rsp_callback, NULL);

    LOG_INFO( "ftp cfg set %s \r\n", rsp);

    return 0;
}

int elfin_eg41a_ftp_start(uint8_t * domain, uint16_t port, uint8_t * fold)
{
    char *pccmd = NULL;
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }

    if (!domain) {
        LOG_INFO( "%s %d - invalid input \r\n", __func__, __LINE__);
        return -1;
    }


    pccmd = g_pccmd;
    if (NULL == pccmd) {
        LOG_INFO( "fail to malloc %d at %s \r\n", ELFIN_EG41A_CONN_CMD_LEN, __func__);
        return -1;
    }
    memset(pccmd, 0, ELFIN_EG41A_CONN_CMD_LEN);

    memset(g_pcdomain_rsp,0,64);

    snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=\"%s\",%d", AT_CMD_START_FTP, domain, port);
    atcmd_config_t atcmd_config_client = { NULL, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_AT_CMD_CME_ERROR};
    at_send_wait_reply(pccmd, strlen(pccmd), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, &atcmd_config_client);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
        LOG_INFO( "cmd %s fail, rsp %s \r\n", pccmd, rsp);
        return -1;
    }

    krhino_sem_count_set(g_ftp_start_sem.hdl, 0);

    int ret = aos_sem_wait(&g_ftp_start_sem, 60000);

    uint32_t retid = 0;
    uint32_t retflag = 0;
//	    if(ret != 0 ||
//	        DecodeStrOneChar(g_pcdomain_rsp,&retid)!=0 || retid != 0 || 
//	        DecodeStrOneShort(g_pcdomain_rsp+2,&retflag)!=0 || retflag != 0)
//	    {
//	        LOG_INFO( "pccmd %s fail, rsp %s %s %d,%d \r\n", 
//	            pccmd, AT_CMD_FTP_CONNECT_RET, g_pcdomain_rsp, retid, retflag);
//	
//	//	                if(retflag == 561)
//	//	                {
//	//	                    uint8_t msg = MSG_FARP_DISGPRS;
//	//	                    krhino_buf_queue_send(gst_FarpQueue, &msg, 1);
//	//	                }
//	        goto err_ftp;
//	    }
    if(ret == 0)
    {
        uint8_t * p = g_pcdomain_rsp;
        if(DecodeStrOneLong(p, &retid)!=0 || retid != 0)
        {
            goto err_ftp;
        }
        p= strstr(g_pcdomain_rsp, ",");
        if(p!=NULL)
        {
            if(DecodeStrOneLong(p+1,&retflag)!=0 || retflag != 0)
            {
                goto err_ftp;
            }
        }
        else
        {
            goto err_ftp;
        }
    }
    else
    {
        goto err_ftp;
    }

//	    at_register_callback(AT_CMD_FTP_FILE_RECV, NULL, ftp_len, 
//	                    elfin_eg41a_gprs_module_ftp_data_handle, NULL);
    memset(pccmd, 0, ELFIN_EG41A_CONN_CMD_LEN);

    memset(g_pcdomain_rsp,0,64);
    //\"/\"

    snprintf(pccmd, ELFIN_EG41A_CONN_CMD_LEN - 1, "%s=\"/%s/\"", AT_CMD_FTP_FOLD_CFG, fold);

    if (elfin_eg41a_send_with_retry(pccmd, strlen(pccmd), true,
        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
        LOG_INFO( "ftp cfg failed\n");
        return -1;
    }
        
    krhino_sem_count_set(g_ftp_start_sem.hdl, 0);
    ret = aos_sem_wait(&g_ftp_start_sem, 60000);
    
    retid = 0;
    retflag = 0;
//	    if(ret != 0 ||
//	        DecodeStrOneChar(g_pcdomain_rsp,&retid)!=0 || retid != 0 || 
//	        DecodeStrOneShort(g_pcdomain_rsp+2,&retflag)!=0 || retflag != 0)
//	    {
//	        LOG_INFO( "pccmd %s fail, rsp %s %s %d,%d \r\n", 
//	            AT_CMD_FTP_FOLD_CFG, AT_CMD_FTP_FOLD_RET, g_pcdomain_rsp, retid, retflag);
//	
//	        goto err_ftp;
//	    }
    if(ret == 0)
    {
        uint8_t * p = g_pcdomain_rsp;
        if(DecodeStrOneLong(p, &retid)!=0 || retid != 0)
        {
            goto ERR_FOLD;
        }
        p= strstr(g_pcdomain_rsp, ",");
        if(p!=NULL)
        {
            if(DecodeStrOneLong(p+1,&retflag)!=0 || retflag != 0)
            {
                goto ERR_FOLD;
            }
        }
        else
        {
            goto ERR_FOLD;
        }
    }
    else
    {
        goto ERR_FOLD;
    }

    return 0;
err_ftp:
    LOG_INFO(DBGFMT_A"pccmd %s fail, rsp %s %s %d,%d \r\n",DBGARG_A,
        pccmd, AT_CMD_FTP_CONNECT_RET, g_pcdomain_rsp, retid, retflag);

    return -1;
ERR_FOLD:
    LOG_INFO(DBGFMT_A"pccmd %s fail, rsp %s %s %d,%d \r\n",DBGARG_A,
        AT_CMD_FTP_FOLD_CFG, AT_CMD_FTP_FOLD_RET, g_pcdomain_rsp, retid, retflag);    
    return -1;
}
#define ELFIN_EG41A_FTP_BUF_MAX_LEN 1100
uint8_t ftp_buf[ELFIN_EG41A_FTP_BUF_MAX_LEN];

int elfin_eg41a_ftp_get(uint8_t * file, uint8_t * outbuf, uint32_t offset, uint16_t * len)
{
    uint32_t atcode = 0;
    uint32_t retlen = 0;
    int ret = 0;

    char cmd[ELFIN_EG41A_DEFAULT_CMD_LEN] = {0};
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};
    uint32_t buf_len = * len;
    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }

    if (!file) {
        LOG_INFO( "%s %d - invalid input \r\n", __func__, __LINE__);
        return -1;
    }


    if(buf_len == 0)
    {
        return 0;
    }

    memset(rsp, 0, ELFIN_EG41A_DEFAULT_RSP_LEN);
    memset(cmd, 0, ELFIN_EG41A_DEFAULT_CMD_LEN);
    snprintf(cmd, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s=\"%s\",\"COM:\",%d,%d", AT_CMD_FTP_GET,file,offset,buf_len);
//	    if (elfin_eg41a_send_with_retry(cmd, strlen(cmd), true,
//	        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_FTP_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
//	        LOG_INFO( "ftp rsp time out [%s]\n", rsp);
//	        return -1;
//	    }
    //at._timeout = 20000;
    memset(g_pcdomain_rsp,0,64);
    memset(ftp_buf,0,ELFIN_EG41A_FTP_BUF_MAX_LEN);
    uint8_t str_ok[ELFIN_EG41A_DEFAULT_CMD_LEN];
    memset(str_ok,0,ELFIN_EG41A_DEFAULT_CMD_LEN);
    snprintf(str_ok, ELFIN_EG41A_DEFAULT_CMD_LEN - 1, "%s\r\n", "OK");
    atcmd_config_t atcmd_config_client = { NULL, str_ok, "+CME ERROR:"};
    at_reset_uart();
    at_send_wait_reply(cmd, strlen(cmd), true,
                       NULL, 0, ftp_buf, ELFIN_EG41A_FTP_BUF_MAX_LEN, &atcmd_config_client);
    if (strstr(ftp_buf, ELFIN_EG41A_AT_CMD_FTP_SUCCESS_RSP) == NULL) {
        LOG_INFO( "%s %d failed rsp %s\r\n", __func__, __LINE__, rsp);
        if(DecodeStrOneLong(ftp_buf,&atcode) != 0 || atcode == 625)
        {
            return 1;
        }
        else 
        {
            return -1;
        }
    }

    uint8_t * pbuf = strstr(ftp_buf,ELFIN_EG41A_AT_CMD_FTP_SUCCESS_RSP);
    if(NULL != pbuf)
    {
        pbuf+=9;
    }
    else
    {
        goto GET_ERR;
    }
    krhino_sem_count_set(g_ftp_start_sem.hdl, 0);
    ret = aos_sem_wait(&g_ftp_start_sem, 60000);


    if(ret == 0)
    {
        uint8_t * p = g_pcdomain_rsp;
        if(DecodeStrOneLong(p, &atcode)!=0 || atcode != 0)
        {
            goto GET_ERR;
        }
        p= strstr(g_pcdomain_rsp, ",");
        if(p!=NULL)
        {
            if(DecodeStrOneLong(p+1,&retlen)!=0 || retlen > buf_len)
            {
                goto GET_ERR;
            }
        }
        else
        {
            goto GET_ERR;
        }
    }
    else
    {
        goto GET_ERR;
    }
//	    if(ret != 0 ||
//	        DecodeStrOneLong(g_pcdomain_rsp,&retoffset)!=0 || retoffset != offset || 
//	        DecodeStrOneLong(g_pcdomain_rsp+2,&retlen)!=0 || retlen >= buf_len)
//	    {
//	        LOG_INFO( "pccmd %s fail, rsp %s %s %d,%d \r\n", 
//	            pccmd, AT_CMD_FTP_CONNECT_RET, g_pcdomain_rsp, retid, retflag);
//	
//	//                  if(retflag == 561)
//	//                  {
//	//                      uint8_t msg = MSG_FARP_DISGPRS;
//	//                      krhino_buf_queue_send(gst_FarpQueue, &msg, 1);
//	//                  }
//	        //goto err_ftp;
//	        return -1;
//	    }
    memcpy(outbuf, pbuf, retlen);

    *len = (uint16_t)retlen;
    return 0;
GET_ERR:

    *len = 0;

    LOG_INFO( "ftp get fail, rsp %s %s \r\n", 
        AT_CMD_FTP_CONNECT_RET, g_pcdomain_rsp);
    return -1;
}

int elfin_eg41a_ftp_close(void)
{
    char rsp[ELFIN_EG41A_DEFAULT_RSP_LEN] = {0};

    if (!inited) {
        LOG_INFO( "%s elfin_eg41a gprs module haven't init yet \r\n", __func__);
        return -1;
    }


//	    if (elfin_eg41a_send_with_retry(AT_CMD_STOP_FTP, strlen(AT_CMD_STOP_FTP), true,
//	        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
//	        LOG_INFO( "cmd %s rsp is %s \r\n", AT_CMD_STOP_FTP, rsp);
//	        return -1;
//	    }
    atcmd_config_t atcmd_config_client = { NULL, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_AT_CMD_CME_ERROR};
    at_send_wait_reply(AT_CMD_STOP_FTP, strlen(AT_CMD_STOP_FTP), true, NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, &atcmd_config_client);
    if (strstr(rsp, ELFIN_EG41A_AT_CMD_SUCCESS_RSP) == NULL) {
        LOG_INFO( "cmd %s fail, rsp %s \r\n", AT_CMD_STOP_FTP, rsp);
        return -1;
    }

//	    if (elfin_eg41a_send_with_retry(AT_CMD_STOP_FTP_DEACT, strlen(AT_CMD_STOP_FTP_DEACT), true,
//	        NULL, 0, rsp, ELFIN_EG41A_DEFAULT_RSP_LEN, ELFIN_EG41A_AT_CMD_SUCCESS_RSP, ELFIN_EG41A_RETRY_MAX) < 0) {
//	        LOG_INFO( "cmd %s rsp is %s \r\n", AT_CMD_STOP_FTP_DEACT, rsp);
//	        return -1;
//	    }
        
    return 0;
}




