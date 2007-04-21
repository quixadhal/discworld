#ifndef __SMTP_H__
#define __SMTP_H__

#define SMTP_HOST "127.0.0.1"
#define SMTP_PORT 25
#define MUD_IP_NAME "discworld.imaginary.com"

#undef DEBUG

#ifdef DEBUG
#define TP(s) tell_object(find_player("turrican"), s)
#else
#define TP(s)
#endif

#endif /* __SMTP_H__ */
