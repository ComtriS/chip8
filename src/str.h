#ifndef __STR_H__
#define __STR_H__

#define XSTR(x)  #x
#define STR(x)   XSTR(x)

int str_count(char* str, char c);

#endif /* __STR_H__ */
