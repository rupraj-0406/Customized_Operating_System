#include "stdint.h"
#include "stdarg.h"

extern int writeu(char *buffer, int buffer_size);
extern unsigned char keyboard_readu(void);

static int udecimal_to_string(char *buffer, int position, uint64_t digits)
{
    char digits_map[10] = "0123456789";
    char digits_buffer[25];
    int size = 0;

    do {
        digits_buffer[size++] = digits_map[digits % 10];
        digits /= 10;
    } while (digits != 0);

    for (int i = size-1; i >= 0; i--) {
        buffer[position++] = digits_buffer[i];
    }

    return size;
}

static int decimal_to_string(char *buffer, int position, int64_t digits)
{
    int size = 0;

    if (digits < 0) {
        digits = -digits;
        buffer[position++] = '-';
        size = 1;
    }

    size += udecimal_to_string(buffer, position, (uint64_t)digits);
    return size;
}

static int hex_to_string(char *buffer, int position, uint64_t digits)
{
    char digits_buffer[25];
    char digits_map[16] = "0123456789ABCDEF";
    int size = 0;

    do {
        digits_buffer[size++] = digits_map[digits % 16];
        digits /= 16;
    } while (digits != 0);

    for (int i = size-1; i >= 0; i--) {
        buffer[position++] = digits_buffer[i];
    }

    buffer[position++] = 'H';

    return size+1;
}

static int read_string(char *buffer, int position, const char *string)
{
    int index = 0;

    for (index = 0; string[index] != '\0'; index++) {
        buffer[position++] = string[index];
    }

    return index;
}

int prints(const char *format, ...)
{
    char buffer[1024];
    int buffer_size = 0;
    int64_t integer = 0;
    char *string = 0;
    va_list args;

    va_start(args,format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            buffer[buffer_size++] = format[i];
        }
        else {
            switch (format[++i]) {
                case 'x':
                    integer = va_arg(args, int64_t);
                    buffer_size += hex_to_string(buffer, buffer_size, (uint64_t)integer);
                    break;

                case 'u':
                    integer = va_arg(args, int64_t);
                    buffer_size += udecimal_to_string(buffer, buffer_size, (uint64_t)integer);
                    break;

                case 'd':
                    integer = va_arg(args, int64_t);
                    buffer_size += decimal_to_string(buffer, buffer_size, integer);
                    break;

                case 's':
                    string = va_arg(args, char*);
                    buffer_size += read_string(buffer, buffer_size, string);
                    break;

                default:
                    buffer[buffer_size++] = '%';
                    i--;
            }
        }     
    }

    buffer_size = writeu(buffer, buffer_size);
    va_end(args);

    return buffer_size;
}
///////////////
// static int read_cmd(char *buffer)
// {
//     char ch[2] = { 0 };
//     int buffer_size = 0;

    
//     while (1) {
//         ch[0] = keyboard_readu();
        
//         if (ch[0] == '\n' || buffer_size >= 1024) {
//             prints("%s", ch);
//             break;
//         }
//         else if (ch[0] == '\b') {    
//             if (buffer_size > 0) {
//                 buffer_size--;
//                 prints("%s", ch);    
//             }           
//         }          
//         else {     
//             buffer[buffer_size++] = ch[0]; 
//             prints("%s", ch);        
//         }
//     }

//     return buffer_size;
// }
////////////////////// 

//int scans(const char *format, char *name)
void strcpy(char *name,char *buffer){
    int i=0;
    for(i=0;buffer[i]!='\0';i++){
        name[i]=buffer[i];
    }
}
int scans(char name[])
{
    char buffer[1024]={0};
    int buffer_size = 0;//read_cmd(buffer);
    //int64_t integer = 0;
    //char *string = 0;
    char ch[2] = { 0 };
    while (1) {
        ch[0] = keyboard_readu();
        
        if (ch[0] == '\n' || buffer_size >= 1024) {
            prints("%s", ch);
            break;
        }
        else if (ch[0] == '\b') {    
            if (buffer_size > 0) {
                buffer_size--;
                prints("%s", ch);    
            }           
        }          
        else {     
            //buffer[buffer_size++] = ch[0]; 
            name[buffer_size++]=ch[0];
            prints("%s", ch);        
        }
    }
    //buffer[buffer_size+1] = '\0';
    //strcpy(name,buffer);
    return buffer_size;
}

/*
#define MAXLN	1024
#define SPACE	"\t\n\f\r " 

char * check(char *bptr)
{
	while (*bptr != '\n') {
		if (!isspace(*bptr))
			return bptr;
		++bptr;	
	}
	return bptr;
}
char *fillbuff(char *buff)
{
	fgets(buff, MAXLN, stdin);
	return buff;
}

int simplescanf(const char *fmt, ...)
{
	char buff[MAXLN], *str, *bptr, *ch;		
	int len = 0, count = 0, space = 0,*var;
	char tmp[MAXLN];
	va_list ap;
	va_start(ap, fmt);
	bptr = fillbuff(buff);
	while (*fmt) {
		len = 0;
		while (*fmt && isspace(*fmt)) {
			++fmt;
			++space;
		}
		if (!*fmt)
			break;
		if (*fmt == 's') {  ///////////////////////////////////////////////////////////////////////////////////////////////////
			bptr = check(bptr);
			if (*bptr == '\n') {
				bptr = fillbuff(buff);
				continue;
			}
			str = va_arg(ap, char *);
			len = strcspn(bptr, SPACE);
			strncpy(str, bptr, len);
			bptr += len;
			++count;
			space = 0;
		} else if (*fmt == 'd') {           
			bptr = check(bptr);
			if (*bptr == '\n') {
				bptr = fillbuff(buff);
				continue;
			}
			var = va_arg(ap, int *);
			len = strcspn(bptr, SPACE);
			strncpy(tmp, bptr, len);
			*var = atoi(tmp);
			bptr += len;
			++count;
			space = 0;
		} else if (*fmt == 'c') {
			len = 1;
			if (space > 0) {
				bptr = check(bptr);
				if (*bptr == '\n') {
					bptr = fillbuff(buff);
					continue;
				}
			}
			str = va_arg(ap, char *);
			strncpy(str, bptr, len);
			if (*bptr == '\n') {
				bptr = fillbuff(buff);
				len = 0;
			}		
			bptr += len;
			++count;
			space = 0;
		} else
			;
		++fmt;
	}
	va_end(ap);
	return count;
}
*/






//va_list args;

    // va_start(args,format);

    // for (int i = 0; format[i] != '\0'; i++) {
    //     if (format[i] != '%') {
    //         buffer[buffer_size++] = format[i];
    //     }
    //     else {
    //         switch (format[++i]) {
    //             case 'x':
    //                 integer = va_arg(args, int64_t);
    //                 buffer_size += hex_to_string(buffer, buffer_size, (uint64_t)integer);
    //                 break;

    //             case 'u':
    //                 integer = va_arg(args, int64_t);
    //                 buffer_size += udecimal_to_string(buffer, buffer_size, (uint64_t)integer);
    //                 break;

    //             case 'd':
    //                 integer = va_arg(args, int64_t);
    //                 buffer_size += decimal_to_string(buffer, buffer_size, integer);
    //                 break;

    //             case 's':
    //                 string = va_arg(args, char*);
    //                 buffer_size += read_string(buffer, buffer_size, string);
    //                 break;

    //             default:
    //                 buffer[buffer_size++] = '%';
    //                 i--;
    //         }
    //     }     
    // }

    // buffer_size = writeu(buffer, buffer_size);
    // va_end(args);  