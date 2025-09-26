#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include "ser_func.h"
#include "ser_func_const.h"

void print_str_sockaddr_in(const struct sockaddr_in* sockaddr)
{
    printf("sin_family = \t%d\nsin_port = \t%d\nsin_addr = \t%d.%d.%d.%d\n",
            sockaddr -> sin_family,
            ntohs(sockaddr -> sin_port),
            (ntohl(sockaddr -> sin_addr.s_addr)  >> 24) & 0xff, 
            (ntohl(sockaddr -> sin_addr.s_addr)  >> 16) & 0xff, 
            (ntohl(sockaddr -> sin_addr.s_addr)  >> 8) & 0xff, 
            ntohl(sockaddr -> sin_addr.s_addr) & 0xff);
}

int client_processing(int s)
{
    char* buf = (char*) malloc(BUF_LEN); // # allocate memory # -> buf
    char* name_of_file = NULL;

    int res = read(s, buf, BUF_LEN - 1);
    if(res < 0)
    {
        printf("Can't read socket, socket descriptor = %d\n", s);
        free(buf);
        close(s);
        return 1;
    }

    buf[res] = '\0';
    // printf("Soket descriptor = %d\nReceived %d bytes:\n%s\n", s, res, buf);

    res = request_type(buf);
    if((res == 0) || (res == 2))
    {
        printf("Received %d bytes:\n%s\n", res, buf);
        printf("Get incorrect request, socket descriptor = %d\n", s);
        free(buf);
        close(s);
        return 2;
    }
    if(res > 3)
    {
        printf("Received %d bytes:\n%s\n", res, buf);
        printf("Server doesn't process this method, socket descriptor = %d\n", s);
        free(buf);
        close(s);
        return 3;
    }
    if(res == 1)
    {
        write(s, "OK, completing the work\r\n", 25);
        printf("Bye bye ..., socket descriptor = %d\n", s);
        free(buf);
        sleep(1);
        close(s);
        return 5;
    }

    char* tmp_ptr1 = strchr(buf, '/');
    char* tmp_ptr2 = strchr(tmp_ptr1, ' ');
    *tmp_ptr2 = '\0';
    name_of_file = (char*) malloc(tmp_ptr2 - tmp_ptr1 + 2); // # allocate memory # -> name_of_file
    *name_of_file = '.';
    strcpy(name_of_file + 1, tmp_ptr1);
    /* sprintf add '\0' to the end */
    res = sprintf(buf, "%s%s%s", response_to_cl_01, header_Connection, Connection_value_01);
    res = set_Content_Type(buf, name_of_file);
    if(res == 1)
    {
        printf("Unknown Content-Type, name of file: %s\n", name_of_file);
    }

    FILE* requested_fptr = fopen(name_of_file, "rb");
    if(requested_fptr == NULL)
    {
        printf("Can't open %s file\n", name_of_file);
        printf("socket descriptor: %d\n", s);
        write(s, response_to_cl_02, strlen(response_to_cl_02));
        free(name_of_file); // # free memory # -> name_of_file
        free(buf); // # free memory # -> buf
        sleep(1);
        close(s);
        return 4;
    }
    if(fseek(requested_fptr, 0, SEEK_END) != 0)
    {
        printf("fseek error in %s file\n", name_of_file);
        printf("socket descriptor: %d", s);
        free(name_of_file); // # free memory # -> name_of_file
        free(buf); // # free memory # -> buf
        fclose(requested_fptr);
        close(s);
        return 4;
    }
    long f_size = ftell(requested_fptr);
    if(f_size == -1l)
    {
        printf("ftell error in %s file\n", name_of_file);
        printf("socket descriptor: %d", s);
        free(name_of_file); // # free memory # -> name_of_file
        free(buf); // # free memory # -> buf
        fclose(requested_fptr);
        close(s);
        return 4;
    }

    sprintf(buf + strlen(buf), "%s%ld\r\n\r\n", header_Content_Length, f_size);
    fseek(requested_fptr, 0, SEEK_SET);
    // printf("\nbuf:\n%s\n\n", buf);
    res = write(s, buf, strlen(buf));

    for(long i = 0; i < f_size; i += BUF_LEN)
    {
        int len_read = fread(buf, 1, BUF_LEN, requested_fptr);
        write(s, buf, len_read);
    }

    free(name_of_file); // # free memory # -> name_of_file
    free(buf); // # free memory # -> buf
    fclose(requested_fptr);
    sleep(1);
    close(s);
    return 0;
}

int set_Content_Type(char* buf, char* name_of_file)
{
    char* tmp_ptr = strchr(name_of_file + 1, '.');
    ++tmp_ptr;
    if(strcmp(tmp_ptr, "html") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_01); // html
        return 0;
    }
    if(strcmp(tmp_ptr, "css") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_01); // css
        return 0;
    }
    if(strcmp(tmp_ptr, "txt") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_01); // txt
        return 0;
    }
    if(strcmp(tmp_ptr, "jpg") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_02); // jpg
        return 0;
    }
    if(strcmp(tmp_ptr, "jpeg") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_02); // jpeg
        return 0;
    }
    if(strcmp(tmp_ptr, "ico") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_02); // ico
        return 0;
    }
    if(strcmp(tmp_ptr, "webp") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_02); // webp
        return 0;
    }
    if(strcmp(tmp_ptr, "png") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_02); // png
        return 0;
    }
    if(strcmp(tmp_ptr, "mp4") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_03); // mp4
        return 0;
    }
    if(strcmp(tmp_ptr, "mp3") == 0)
    {
        strcat(buf,header_Content_Type);
        strcat(buf, Content_Type_value_04); // mp3
        return 0;
    }
    return 1;
}

void* cl_thread(void* arg)
{
    // struct completion_ser_work* csw = (struct completion_ser_work*) ptr;
    int* s = ((cl_thread_arg*) arg) -> s;
    int res = client_processing(*s);
    free(s);

    if(res == 5)
    {
        *(((cl_thread_arg*) arg) -> stop_flag) = false;
    }

    free((cl_thread_arg*) arg);
    return NULL;
}

int request_type(char* buf)
{
    char* tmp_ptr = strchr(buf, ' ');
    if(tmp_ptr == NULL)
    {
        if(strcmp(buf, "!STOP\r\n") == 0)
        {
            return 1;
        }
        return 2;
    }

    int req_len = tmp_ptr - buf + 1;
    char* req = (char*) malloc(req_len);
    --req_len;
    for(int i = 0; i < req_len; ++i)
    {
        req[i] = buf[i];
    }
    req[req_len] = '\0';

    if(strcmp(req, req_method_GET) == 0)
    {
        return 3;
    }
    if(strcmp(req, req_method_POST) == 0)
    {
        return 4;
    }
    if(strcmp(req, req_method_PUT) == 0)
    {
        return 5;
    }
    if(strcmp(req, req_method_DELETE) == 0)
    {
        return 6;
    }

    return 0;
}