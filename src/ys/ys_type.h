#ifndef _YS_TYPE_H_
#define _YS_TYPE_H_

#define YS_SSL_ON  (1)
#define YS_SSL_OFF (0)

#define YS_RECV_BUFFER_SIZE 16 // (8192)
#define YS_SEND_BUFFER_SIZE 16 // (8192)

typedef std::array<unsigned char, YS_RECV_BUFFER_SIZE> recv_buffer;
typedef std::array<unsigned char, YS_SEND_BUFFER_SIZE> send_buffer;

#define YS_FIRSR_LINE_MAX_SIZE     (8  * 1024)
#define YS_HEAD_MAX_SIZE           (64 * 1024)
#define YS_BODY_MAX_SIZE           (64 * 1024 * 1024)

#endif