#ifndef __t2u_session_h__
#define __t2u_session_h__

enum t2u_mess_oper
{
    connect_request,
    connect_response,
    close_request,
    close_response,
    data_request,
    data_response,
    retrans_request,
    retrans_response,
};

typedef struct t2u_message_
{
    uint32_t magic_;            /* magic number, see T2U_MESS_MAGIC */
    uint16_t version_;          /* version, current 0x0001 */
    uint16_t oper_;             /* operation code, see t2u_mess_oper */
    uint32_t handle_;           /* handle for mapping session */
    uint32_t seq_;              /* handle based seq */
    char payload[0];            /* payload */
} t2u_message;

typedef struct session_message_
{
    t2u_message *data_;
    unsigned long len_;
} session_message;

typedef struct t2u_session_
{
    void *rule_;                    /* parent rule */
    sock_t sock_;                   /* with the socket */
    uint32_t handle_;               /* handle */
    uint32_t pair_handle_;          /* remote handle */
    unsigned long send_retries_;    /* retry count */
    int status_;                    /* 0 for non, 1 for connecting, 2 for establish */
    uint32_t send_seq_;             /* send seq */
    /* uint32_t recv_seq_; */       /* recv seq */
    session_message mess_;          /* send message list */
    t2u_event_data timeout_ev;      /* the timeout event */
    struct event* disable_event_;   /* disabled event */
} t2u_session;

#define T2U_PAYLOAD_MAX (1024)
#define T2U_MESS_BUFFER_MAX  (T2U_PAYLOAD_MAX + sizeof(t2u_message))
#define T2U_MESS_MAGIC (0x5432552E) /* "T2U." */

t2u_session *t2u_session_new(void *rule, sock_t sock);

void t2u_session_delete(t2u_session *session);

void t2u_session_assign_remote_handle(t2u_session *session, uint32_t remote_handle);
t2u_session *t2u_session_by_pair_handle(uint32_t remote_handle);
t2u_session *t2u_session_by_handle(uint32_t handle);


/* operations for sending udp */
void t2u_session_send_u(t2u_session *session);
void t2u_session_send_u_connect(t2u_session *session);
void t2u_session_send_u_connect_response(t2u_session *session, char *connect_message);
void t2u_session_send_u_data(t2u_session *session, char *data, size_t length);
void t2u_session_send_u_data_response(t2u_session *session, char *data_message, uint32_t error);


#endif /* __t2u_session_h__ */