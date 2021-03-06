#ifndef _PROTO_VMESS_TCP_H_
#define _PROTO_VMESS_TCP_H_

#include "pub/fd.h"
#include "pub/thread.h"

#include "proto/tcp.h"
#include "proto/buf.h"

#include "vmess.h"

// TODO: not thread-safe

typedef struct {
    TCP_SOCKET_HEADER
    vbuffer_t *read_buf; // client read from here
    rbuffer_t *raw_buf;
    // vbuffer_t *write_buf; // client write to here

    vmess_auth_t auth;
    vmess_serial_t *vser;

    vmess_config_t *config;

    union {
        target_id_t *proxy; // used by client
        target_id_t *target; // used by server
    } addr;

    // thread_t reader; // , writer;
    // mutex_t *write_mut;

    fd_t sock;
    // bool started;
} vmess_tcp_socket_t;

vmess_tcp_socket_t *
vmess_tcp_socket_new(vmess_config_t *config);

INLINE void
vmess_tcp_socket_set_proxy(vmess_tcp_socket_t *sock,
                           target_id_t *proxy)
{
    target_id_free(sock->addr.proxy);
    sock->addr.proxy = target_id_copy(proxy);
}

INLINE void
vmess_tcp_socket_set_target(vmess_tcp_socket_t *sock,
                            target_id_t *target)
{
    target_id_free(sock->addr.target);
    sock->addr.target = target_id_copy(target);
}

INLINE const target_id_t *
vmess_tcp_socket_get_target(vmess_tcp_socket_t *sock)
{
    return sock->addr.target;
}

INLINE void
vmess_tcp_socket_auth(vmess_tcp_socket_t *sock, uint64_t gen_time)
{
    vmess_auth_init(&sock->auth, sock->config, gen_time);
}

#endif
