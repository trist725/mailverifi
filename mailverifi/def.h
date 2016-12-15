
#pragma once
#ifndef __DEF_H__
#define __DEF_H__

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <openssl/ssl.h>

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#ifndef HAVE__STRNICMP
#define HAVE__STRNICMP
#define _strnicmp strncasecmp
#endif

#define OutputDebugStringA(buf)

typedef unsigned short WORD;
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct hostent* LPHOSTENT;
typedef struct servent* LPSERVENT;
typedef struct in_addr* LPIN_ADDR;
typedef struct sockaddr* LPSOCKADDR;

#define LINUX
#else
#include <winsock2.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")

//Add "openssl-0.9.8l\inc32" to Additional Include Directories
#include "openssl\ssl.h"

#if _MSC_VER < 1400
#define snprintf _snprintf
#else
#define snprintf sprintf_s
#endif
#endif

#define TIME_IN_SEC		3*60		// how long client will wait for server response in non-blocking mode

// TLS/SSL extension
enum SMTP_SECURITY_TYPE
{
	NO_SECURITY,
	USE_TLS,
	USE_SSL,
};

enum ESmtpError
{
	CSMTP_NO_ERROR = 0,
	WSA_STARTUP = 100, // WSAGetLastError()
	WSA_VER,
	WSA_SEND,
	WSA_RECV,
	WSA_CONNECT,
	WSA_GETHOSTBY_NAME_ADDR,
	WSA_INVALID_SOCKET,
	WSA_INVALID_PORT,
	WSA_HOSTNAME,
	WSA_IOCTLSOCKET,
	WSA_SELECT,
	BAD_IPV4_ADDR,
	UNDEF_MSG_HEADER = 200,
	UNDEF_MAIL_FROM,
	UNDEF_SUBJECT,
	UNDEF_RECIPIENTS,
	UNDEF_LOGIN,
	UNDEF_PASSWORD,
	BAD_LOGIN_PASSWORD,
	BAD_DIGEST_RESPONSE,
	BAD_SERVER_NAME,
	UNDEF_RECIPIENT_MAIL,
	COMMAND_MAIL_FROM = 300,
	COMMAND_EHLO,
	COMMAND_AUTH_PLAIN,
	COMMAND_AUTH_LOGIN,
	COMMAND_AUTH_CRAMMD5,
	COMMAND_AUTH_DIGESTMD5,
	COMMAND_DIGESTMD5,
	COMMAND_DATA,
	COMMAND_QUIT,
	COMMAND_RCPT_TO,
	MSG_BODY_ERROR,
	CONNECTION_CLOSED = 400, // by server
	SERVER_NOT_READY, // remote server
	SERVER_NOT_RESPONDING,
	SELECT_TIMEOUT,
	FILE_NOT_EXIST,
	MSG_TOO_BIG,
	BAD_LOGIN_PASS,
	UNDEF_XYZ_RESPONSE,
	LACK_OF_MEMORY,
	TIME_ERROR,
	RECVBUF_IS_EMPTY,
	SENDBUF_IS_EMPTY,
	OUT_OF_MSG_RANGE,
	COMMAND_EHLO_STARTTLS,
	SSL_PROBLEM,
	COMMAND_DATABLOCK,
	STARTTLS_NOT_SUPPORTED,
	LOGIN_NOT_SUPPORTED
};

enum SMTP_COMMAND
{
	command_INIT,
	command_EHLO,
	command_AUTHPLAIN,
	command_AUTHLOGIN,
	command_AUTHCRAMMD5,
	command_AUTHDIGESTMD5,
	command_DIGESTMD5,
	command_USER,
	command_PASSWORD,
	command_MAILFROM,
	command_RCPTTO,
	command_DATA,
	command_DATABLOCK,
	command_DATAEND,
	command_QUIT,
	command_STARTTLS
};

Command_Entry command_list[] =
{
	{ command_INIT, 0, 5 * 60, 220, SERVER_NOT_RESPONDING },
	{ command_EHLO, 5 * 60, 5 * 60, 250, COMMAND_EHLO },
	{ command_AUTHPLAIN, 5 * 60, 5 * 60, 235, COMMAND_AUTH_PLAIN },
	{ command_AUTHLOGIN, 5 * 60, 5 * 60, 334, COMMAND_AUTH_LOGIN },
	{ command_AUTHCRAMMD5, 5 * 60, 5 * 60, 334, COMMAND_AUTH_CRAMMD5 },
	{ command_AUTHDIGESTMD5, 5 * 60, 5 * 60, 334, COMMAND_AUTH_DIGESTMD5 },
	{ command_DIGESTMD5, 5 * 60, 5 * 60, 335, COMMAND_DIGESTMD5 },
	{ command_USER, 5 * 60, 5 * 60, 334, UNDEF_XYZ_RESPONSE },
	{ command_PASSWORD, 5 * 60, 5 * 60, 235, BAD_LOGIN_PASS },
	{ command_MAILFROM, 5 * 60, 5 * 60, 250, COMMAND_MAIL_FROM },
	{ command_RCPTTO, 5 * 60, 5 * 60, 250, COMMAND_RCPT_TO },
	{ command_DATA, 5 * 60, 2 * 60, 354, COMMAND_DATA },
	{ command_DATABLOCK, 3 * 60, 0, 0, COMMAND_DATABLOCK },	// Here the valid_reply_code is set to zero because there are no replies when sending data blocks
	{ command_DATAEND, 3 * 60, 10 * 60, 250, MSG_BODY_ERROR },
	{ command_QUIT, 5 * 60, 5 * 60, 221, COMMAND_QUIT },
	{ command_STARTTLS, 5 * 60, 5 * 60, 220, COMMAND_EHLO_STARTTLS }
};

typedef struct tagCommand_Entry
{
	SMTP_COMMAND       command;
	int                send_timeout;	 // 0 means no send is required
	int                recv_timeout;	 // 0 means no recv is required
	int                valid_reply_code; // 0 means no recv is required, so no reply code
	ESmtpError error;
}Command_Entry;

#endif // __DEF_H__
