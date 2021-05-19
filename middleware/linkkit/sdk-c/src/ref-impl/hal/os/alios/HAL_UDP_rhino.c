/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aos/errno.h>
#include "ulog/ulog.h"
#include "aos/kernel.h"
#include <network/network.h>
//#include "iot_import.h"
#define NETWORK_ADDR_LEN        (16)        /* IP网络地址的长度 */
#define HAL_MAC_LEN             (17 + 1)    /* MAC地址的长度 */

typedef struct _network_addr_t {
    unsigned char
    addr[NETWORK_ADDR_LEN];         /* 目标UDP主机地址, 点分十进制IP地址 */
    unsigned short  port;           /* 目标UDP端口, 范围是0-65535 */
} NetworkAddr;

#define TRANSPORT_ADDR_LEN 16

#ifndef IP_PKTINFO
#define IP_PKTINFO IP_MULTICAST_IF
#endif

#ifndef IPV6_PKTINFO
#define IPV6_PKTINFO IPV6_V6ONL
#endif
extern int errno;
#define NETWORK_ADDR_LEN (16)

#define LOG_TAG "HAL_TL"

#define platform_info(format, ...) LOGI(LOG_TAG, format, ##__VA_ARGS__)
#define platform_err(format, ...) LOGE(LOG_TAG, format, ##__VA_ARGS__)
/**
 * @brief Create a UDP socket.
 *
 * @param [in] port: @n Specify the UDP port of UDP socket
 *
 * @retval  < 0 : Fail.
 * @retval >= 0 : Success, the value is handle of this UDP socket.
 * @see None.
 */
intptr_t HAL_UDP_create(char *host, unsigned short port)
{
    int                flag      = 1;
    int                ret       = -1;
    int                socket_id = -1;
    struct sockaddr_in local_addr; /*local addr*/

    if ((socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "socket create failed\r\n");
        return -1;
    }

    ret = setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if (ret < 0) {
        close(socket_id);
        fprintf(stderr, "setsockopt SO_REUSEADDR failed");
        return -1;
    }

    flag = 1;
#ifdef IP_RECVPKTINFO
    if ((ret = setsockopt(socket_id, IPPROTO_IP, IP_RECVPKTINFO, &flag,
                          sizeof(flag))) < 0)
#else  /* IP_RECVPKTINFO */
    if ((ret = setsockopt(socket_id, IPPROTO_IP, IP_PKTINFO, &flag,
                          sizeof(flag))) < 0)
#endif /* IP_RECVPKTINFO */
        if (ret < 0) {
            close(socket_id);
            fprintf(stderr, "setsockopt IP_PKTINFO failed\r\n");
            return -1;
        }


    memset(&local_addr, 0x00, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    if (NULL != host) {
        inet_aton(host, &local_addr.sin_addr);
    } else {
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    local_addr.sin_port = htons(port);
    ret = bind(socket_id, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // fprintf(stderr,"\r\n[%s LINE #%d]  Create socket port %d fd %d ret
    // %d\r\n",
    //                    __FILE__, __LINE__, port, socket_id, ret);
    return socket_id;
}

intptr_t HAL_UDP_create_without_connect(const char *host, unsigned short port)
{
    int                flag      = 1;
    int                ret       = -1;
    int                socket_id = -1;
    struct sockaddr_in local_addr; /*local addr*/

    if ((socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        platform_err("socket create failed\r\n");
        return (intptr_t)-1;
    }

    ret = setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if (ret < 0) {
        close(socket_id);
        platform_err("setsockopt SO_REUSEADDR failed");
        return (intptr_t)-1;
    }

    flag = 1;
#ifdef IP_RECVPKTINFO
    if ((ret = setsockopt(socket_id, IPPROTO_IP, IP_RECVPKTINFO, &flag,
                          sizeof(flag))) < 0)
#else  /* IP_RECVPKTINFO */
    if ((ret = setsockopt(socket_id, IPPROTO_IP, IP_PKTINFO, &flag,
                          sizeof(flag))) < 0)
#endif /* IP_RECVPKTINFO */
        if (ret < 0) {
            close(socket_id);
            platform_err("setsockopt IP_PKTINFO failed\r\n");
            return (intptr_t)-1;
        }


    memset(&local_addr, 0x00, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    if (NULL != host) {
        inet_aton(host, &local_addr.sin_addr);
    } else {
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    local_addr.sin_port = htons(port);
    ret = bind(socket_id, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // fprintf(stderr,"\r\n[%s LINE #%d]  Create socket port %d fd %d ret
    // %d\r\n",
    //                    __FILE__, __LINE__, port, socket_id, ret);
    return (intptr_t)socket_id;
}

int HAL_UDP_close_without_connect( intptr_t sockfd)
{
    return close((int)sockfd);
}

int HAL_UDP_connect( intptr_t sockfd,  const char *host,
                     unsigned short port)
{
    return 0;
}


int HAL_UDP_recv(intptr_t sockfd, unsigned char *p_data, unsigned int datalen,
                 unsigned int timeout_ms)
{
    return 0;
}

int HAL_UDP_sendto(intptr_t sockfd, const NetworkAddr *p_remote,
                   const unsigned char *p_data, unsigned int datalen,
                   unsigned int timeout_ms)
{
    int                rc        = -1;
    int                socket_id = -1;
    struct sockaddr_in remote_addr;

    if (NULL == p_remote || NULL == p_data) {
        return -1;
    }

    socket_id              = (int)sockfd;
    remote_addr.sin_family = AF_INET;
    if (1 !=
        (rc = inet_pton(remote_addr.sin_family, (const char *)p_remote->addr,
                        &remote_addr.sin_addr.s_addr))) {
        return -1;
    }
    remote_addr.sin_port = htons(p_remote->port);
    rc                   = sendto(socket_id, p_data, (size_t)datalen, 0,
                (const struct sockaddr *)&remote_addr, sizeof(remote_addr));
    if (-1 == rc) {
        return -1;
    }
    return rc;
}

int HAL_UDP_send( intptr_t sockfd,  const unsigned char *p_data,
                  unsigned int datalen,  unsigned int timeout_ms)
{
    int            ret;
    fd_set         write_fds;
    struct timeval timeout = { timeout_ms / 1000, (timeout_ms % 1000) * 1000 };

    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);

    ret = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
    if (ret == 0) {
        return 0; /* write timeout */
    }

    if (ret < 0) {
        if (errno == EINTR) {
            return -3; /* want write */
        }
        return -4; /* write failed */
    }

    ret = send(sockfd, (char *)p_data, (int)datalen, 0);

    if (ret < 0) {
        platform_err("send");
    }

    return ret;
}

int HAL_UDP_read(intptr_t p_socket, unsigned char *p_data, unsigned int datalen)
{
    long socket_id = -1;
    int  count     = -1;

    if (NULL == p_data || 0 == p_socket) {
        return -1;
    }

    socket_id = (long)p_socket;
    count     = (int)read(socket_id, p_data, datalen);

    return count;
}

int HAL_UDP_joinmulticast(intptr_t sockfd, char *p_group)
{
    int err       = -1;
    int socket_id = -1;

    if (NULL == p_group) {
        return -1;
    }

    /*set loopback*/
    int loop  = 1;
    socket_id = (int)sockfd;
    err =
      setsockopt(socket_id, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if (err < 0) {
        fprintf(stderr, "setsockopt():IP_MULTICAST_LOOP failed\r\n");
        return err;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(p_group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*default networt interface*/

    /*join to the mutilcast group*/
    err =
      setsockopt(socket_id, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (err < 0) {
        fprintf(stderr, "setsockopt():IP_ADD_MEMBERSHIP failed\r\n");
        return err;
    }

    return 0;
}

void HAL_UDP_close( intptr_t sockfd)
{
    long socket_id = -1;

    socket_id = (int)sockfd;
    close(socket_id);
}

int HAL_UDP_write(intptr_t p_socket, const unsigned char *p_data,
                  unsigned int datalen)
{
    int  rc        = -1;
    long socket_id = -1;

    socket_id = (long)p_socket;
    rc        = send(socket_id, (char *)p_data, (int)datalen, 0);
    if (-1 == rc) {
        return -1;
    }

    return rc;
}

int HAL_UDP_readTimeout(intptr_t p_socket, unsigned char *p_data,
                        unsigned int datalen, unsigned int timeout)
{
    int            ret;
    struct timeval tv;
    fd_set         read_fds;
    long           socket_id = -1;

    if (0 == p_socket || NULL == p_data) {
        return -1;
    }
    socket_id = (long)p_socket;

    if (socket_id < 0) {
        return -1;
    }

    FD_ZERO(&read_fds);
    FD_SET(socket_id, &read_fds);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ret =
      select(socket_id + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv);

    /* Zero fds ready means we timed out */
    if (ret == 0) {
        return -2; /* receive timeout */
    }

    if (ret < 0) {
        if (errno == EINTR) {
            return -3; /* want read */
        }

        return -4; /* receive failed */
    }

    /* This call will not block */
    return HAL_UDP_read(p_socket, p_data, datalen);
}