#ifndef SER_FUNC_H
#define SER_FUNC_H

#define BUF_LEN int(1 << 12) // 4 Kb = 4096 b

/*  why when I use const char* compilator gives out error ? */
/*  const char* con_type_01 = "text/html\r\n"; */
/*
    g++ ser.cpp -c -o ser.o
    g++ ser_func.cpp -c -o ser_func.o
    g++ ser.o ser_func.o -o ser
    /usr/bin/ld: ser_func.o:(.data.rel.local+0x0): повторное определение «con_type_01»; 
    ser.o:(.data.rel.local+0x0): здесь первое определение
    collect2: error: ld returned 1 exit status
    make: *** [Makefile:2: ser] Ошибка 1
*/
/*  when I use
    const char con_type_01[] = "text/html\r\n";
    all work correct
*/

struct cl_thread_arg
{
    int* s;            // run socket descriptor
    bool* stop_flag;    // exit from the endless loop
};

void print_str_sockaddr_in(const struct sockaddr_in* sockaddr);
int client_processing(int s);
int set_Content_Type(char *buf, char* name_of_file);
void* cl_thread(void* arg);
int request_type(char* buf);

#endif // SER_FUNC_H