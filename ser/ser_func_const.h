#ifndef SER_FUNC_CONST_H
#define SER_FUNC_CONST_H
/*
    "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\n";
*/

/* HTTP/1.1 200 OK */
const char* response_to_cl_01 = "HTTP/1.1 200 OK\r\n";
/* HTTP/1.1 404 Not Found */
const char* response_to_cl_02 = "HTTP/1.1 404 Not Found\r\n\r\n";

/* Connection */
const char* header_Connection = "Connection: ";
/* Connection: close */
const char* Connection_value_01 = "close\r\n";
/* Connection: keep-alive */
const char* Connection_value_02 = "keep-alive\r\n";

/* Content-Type:  */
const char* header_Content_Type = "Content-Type: ";
/* Content-Type text/html */
const char* Content_Type_value_01 = "text/html\r\n";
/* Content-Type image/jpeg */
const char* Content_Type_value_02 = "image/jpeg\r\n";
/* Content-Type video/mp4 */
const char* Content_Type_value_03 = "video/mp4\r\n";
/* Content-Type audio/mpeg */
const char* Content_Type_value_04 = "audio/mpeg\r\n";

/* Content-Length */
const char* header_Content_Length = "Content-Length: ";

/* --------------------------------------------------- */

/* GET */
const char* req_method_GET = "GET";
/* POST  */
const char* req_method_POST = "POST";
/* PUT */
const char* req_method_PUT = "PUT";
/* DELETE */
const char* req_method_DELETE = "DELETE";

#endif // SER_FUNC_CONST_H
