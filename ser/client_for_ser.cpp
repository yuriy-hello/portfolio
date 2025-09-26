#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "ser_func.h"

int main()
{
    // Create a socket
    int s0 = socket(AF_INET, SOCK_STREAM, 0);

    if (s0 < 0) {
        printf("Can't create a socket\n");
        return 1;
    }

    // Fill in the address of server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    const char* server_host = "localhost";

    #define DEFINE_STRUCT_HOSTENT
    #ifndef DEFINE_STRUCT_HOSTENT

        /* Структура hostent определена в <netdb.h> следующим образом: */
            struct hostent {
                    char    *h_name;        /* официальное имя машины */
                    char    **h_aliases;    /* список псевдонимов */
                    int     h_addrtype;     /* тип адреса машины */
                    int     h_length;       /* длина адреса */
                    char    **h_addr_list;  /* список адресов */
            }
            #define h_addr  h_addr_list[0]  /* для совместимости с предыдущими версиями */

        /* Полями структуры hostent являются: */

            h_name      // (официальное имя машины); 
            h_aliases   // (оканчивающийся нулем массив альтернативных имен машины); 
            h_addrtype  // (тип адреса; в настоящее время всегда AF_INET); 
            h_length    // (длина адреса в байтах); 
            h_addr_list // (оканчивающийся нулем массив сетевых адресов машины в сетевом порядке байтов); 
            h_addr      // (первый адрес в h_addr_list определен для совместимости с более ранними версиями).

    #endif // DEFINE_STRUCT_HOSTENT

    // Resolve the server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(server_host);
    if (host == NULL) {
        printf("Cannot define host address\n");
        close(s0);
        return 2;
    }
    server_addr.sin_family = AF_INET;
    
    short server_port = 1234;
    
    server_addr.sin_port = htons(server_port);

    // Print a resolved address of server (the first IP of the host)
    printf("server addr = %d.%d.%d.%d, port %d\n",
            host -> h_addr_list[0][0] & 0xff,
            host -> h_addr_list[0][1] & 0xff,
            host -> h_addr_list[0][2] & 0xff,
            host -> h_addr_list[0][3] & 0xff,
            (int) server_port);
    
    // Write resolved IP address of a server to the address structure
    memmove(&(server_addr.sin_addr.s_addr), host -> h_addr_list[0], 4);

    print_str_sockaddr_in(&server_addr);

    // Connect to the localhost server
    int res = connect(s0, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (res < 0)
    {
        printf("Can't connect\n");
        close(s0);
        return 3;
    }

    printf("Connected. Reading a server message.\n");

    char buffer[1024];
    char answer[] = "!STOP\r\n";
    
    write(s0, answer, strlen(answer));
    
    res = read(s0, buffer, 1024);
    if (res < 0) {
        printf("Read error\n");
        // free(buffer);
        close(s0);
        return 3;
    }
    buffer[res] = '\0'; // may buffer[res] = 0 such as 0 == int('\0')
    printf("Received %d byte:\n%s\n", res, buffer);

    // free(buffer);
    close(s0);

    /*
    s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        printf("Can't create a socket\n");
        return 1;
    }
    res = connect(s0, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (res < 0)
    {
        printf("Can't connect\n");
        close(s0);
        return 3;
    }
    write(s0, answer, strlen(answer));
    
    res = read(s0, buffer, 1024);
    if (res < 0) {
        printf("Read error\n");
        // free(buffer);
        close(s0);
        return 3;
    }
    buffer[res] = '\0'; // may buffer[res] = 0 such as 0 == int('\0')
    printf("Received %d byte:\n%s\n", res, buffer);
    close(s0);
    */

    return 0;
}