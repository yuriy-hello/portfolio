#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include "ser_func.h"

int main()
{
    int listen_port = 1234; // listening port

    /*
        socket()  creates  an endpoint for communication and returns a file de‐
        scriptor that refers to that endpoint.  The file descriptor returned by
        a successful call will be the lowest-numbered file descriptor not  cur‐
        rently open for the process.
    */
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    /*  #include <sys/socket.h>
        int socket(int domain, int type, int protocol);
            domain: defined in <sys/socket.h> (see man socket)
                AF_INET         IPv4 Internet protocols
                                AF - address family
            type:
                SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based
                                byte streams.  An out-of-band data transmission  mecha‐
                                nism may be supported.
            protocol:
                0               The  protocol  specifies  a  particular  protocol  to  be used with the
                                socket.  Normally only a single protocol exists to support a particular
                                socket type within a given protocol family, in which case protocol  can
                                be specified as 0.  However, it is possible that many protocols may ex‐
                                ist, in which case a particular protocol must be specified in this man‐
                                ner.   The protocol number to use is specific to the “communication do‐
                                main” in which communication is to take place; see  protocols(5).   See
                                getprotoent(3) on how to map protocol name strings to protocol numbers.

                                Sockets  of type SOCK_STREAM are full-duplex byte streams.  They do not
                                preserve record boundaries.  A stream socket must  be  in  a  connected
                                state  before  any data may be sent or received on it.  A connection to
                                another socket is created with a connect(2) call.  Once connected, data
                                may be transferred using read(2) and write(2) calls or some variant  of
                                the  send(2)  and  recv(2)  calls.  When a session has been completed a
                                close(2) may be performed.  Out-of-band data may also be transmitted as
                                described in send(2) and received as described in recv(2).

                                ip	0	IP		# internet protocol, pseudo protocol number
                                hopopt	0	HOPOPT		# IPv6 Hop-by-Hop Option [RFC1883]
                                icmp	1	ICMP		# internet control message protocol
                                igmp	2	IGMP		# Internet Group Management
                                ggp	3	GGP		# gateway-gateway protocol
                                ipencap	4	IP-ENCAP	# IP encapsulated in IP (officially ``IP'')
                                st	5	ST		# ST datagram mode
                                tcp	6	TCP		# transmission control protocol
                                egp	8	EGP		# exterior gateway protocol
                                igp	9	IGP		# any private interior gateway (Cisco)
                                pup	12	PUP		# PARC universal packet protocol
                                udp	17	UDP		# user datagram protocol
                                hmp	20	HMP		# host monitoring protocol
                                xns-idp	22	XNS-IDP		# Xerox NS IDP
                                rdp	27	RDP		# "reliable datagram" protocol
                                iso-tp4	29	ISO-TP4		# ISO Transport Protocol class 4 [RFC905]
                                dccp	33	DCCP		# Datagram Congestion Control Prot. [RFC4340]
                                xtp	36	XTP		# Xpress Transfer Protocol
                                ddp	37	DDP		# Datagram Delivery Protocol
                                idpr-cmtp 38	IDPR-CMTP	# IDPR Control Message Transport
                                ipv6	41	IPv6		# Internet Protocol, version 6
                                ipv6-route 43	IPv6-Route	# Routing Header for IPv6
                                ipv6-frag 44	IPv6-Frag	# Fragment Header for IPv6
                                idrp	45	IDRP		# Inter-Domain Routing Protocol
                                rsvp	46	RSVP		# Reservation Protocol
                                gre	47	GRE		# General Routing Encapsulation
                                esp	50	IPSEC-ESP	# Encap Security Payload [RFC2406]
                                ah	51	IPSEC-AH	# Authentication Header [RFC2402]
                                skip	57	SKIP		# SKIP
                                ipv6-icmp 58	IPv6-ICMP	# ICMP for IPv6
                                ipv6-nonxt 59	IPv6-NoNxt	# No Next Header for IPv6
                                ipv6-opts 60	IPv6-Opts	# Destination Options for IPv6
                                rspf	73	RSPF CPHB	# Radio Shortest Path First (officially CPHB)
                                vmtp	81	VMTP		# Versatile Message Transport
                                eigrp	88	EIGRP		# Enhanced Interior Routing Protocol (Cisco)
                                ospf	89	OSPFIGP		# Open Shortest Path First IGP
                                ax.25	93	AX.25		# AX.25 frames
                                ipip	94	IPIP		# IP-within-IP Encapsulation Protocol
                                etherip	97	ETHERIP		# Ethernet-within-IP Encapsulation [RFC3378]
                                encap	98	ENCAP		# Yet Another IP encapsulation [RFC1241]
                                #	99			# any private encryption scheme
                                pim	103	PIM		# Protocol Independent Multicast
                                ipcomp	108	IPCOMP		# IP Payload Compression Protocol
                                vrrp	112	VRRP		# Virtual Router Redundancy Protocol [RFC5798]
                                l2tp	115	L2TP		# Layer Two Tunneling Protocol [RFC2661]
                                isis	124	ISIS		# IS-IS over IPv4
                                sctp	132	SCTP		# Stream Control Transmission Protocol
                                fc	133	FC		# Fibre Channel
                                mobility-header 135 Mobility-Header # Mobility Support for IPv6 [RFC3775]
                                udplite	136	UDPLite		# UDP-Lite [RFC3828]
                                mpls-in-ip 137	MPLS-in-IP	# MPLS-in-IP [RFC4023]
                                manet	138			# MANET Protocols [RFC5498]
                                hip	139	HIP		# Host Identity Protocol
                                shim6	140	Shim6		# Shim6 Protocol [RFC5533]
                                wesp	141	WESP		# Wrapped Encapsulating Security Payload
                                rohc	142	ROHC		# Robust Header Compression
                                ethernet 143	Ethernet	# Ethernet encapsulation for SRv6 [RFC8986]
    */

    if(socket_descriptor < 0)
    {
        printf("Can't create a socket\n");
        return -1;
    }

    struct sockaddr_in server_addr;

    /*  struct sokaddr_in
            struct sockaddr_in 
            {
                short int          sin_family;  //  Семейство адресов 2 bytes
                unsigned short int sin_port;    //  Номер порта 2 bytes
                struct in_addr     sin_addr;    //  IP-адрес 4 bytes
                unsigned char      sin_zero[8]; //  "Дополнение" до размера структуры sockaddr,
                                                //  нужно для совместимости,
                                                //  sockaddr более общая структура 
            };
            
            struct in_addr
            {
                unsigned long s_addr;           //  4 bytes for storage IPv4
            };
            
            struct sockaddr
            {
                unsigned short    sa_family;    //  Семейство адресов, AF_xxx   2 bytes
                char              sa_data[14];  //  14 байтов для хранения адреса
            };
    */

    /* fills all bytes struct server_addr in the symbol char(int(0)) */
    /* that is, all bytes are zero */
    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    /*  protocol family
            The  domain argument specifies a communication domain; this selects the
            protocol family which will be used for communication.   These  families
            are defined in <sys/socket.h>.  The formats currently understood by the
            Linux kernel include:
            Name         Purpose                                    Man page
            AF_UNIX      Local communication                        unix(7)
            AF_LOCAL     Synonym for AF_UNIX
            AF_INET      IPv4 Internet protocols                    ip(7)
            AF_AX25      Amateur radio AX.25 protocol               ax25(4)
            AF_IPX       IPX - Novell protocols
            AF_APPLETALK AppleTalk                                  ddp(7)
            AF_X25       ITU-T X.25 / ISO/IEC 8208 protocol         x25(7)
            AF_INET6     IPv6 Internet protocols                    ipv6(7)
            ................................................................
    */
    //  short int          sin_family;
    server_addr.sin_family = AF_INET;

    /*  unsigned short int sin_port;
        fuck htons() host to network bytes order, short
        change the byte order to big-endian
    */
    /*  uint32_t htonl(uint32_t hostlong)  :    converts the unsigned integer hostlong from
                                                host byte order to network byte order
        uint16_t htons(uint16_t hostshort) :    converts the unsigned short integer hostshort from
                                                host byte order to network byte order
        uint32_t ntohl(uint32_t netlong)   :    converts the unsigned integer netlong from
                                                network byte order to host byte order
        uint16_t ntohs(uint16_t netshort)  :    converts the unsigned short integer netshort from
                                                network byte order to host byte order
    */
    server_addr.sin_port = htons(listen_port);

    /*  struct in_addr     sin_addr;
        IP adress
        struct in_addr
            {
                unsigned long s_addr;           //  4 bytes for storage IPv4
            };
        #define INADDR_ANY ((in_addr_t) 0x00000000)
        in_addr_t equivalent to the type uint32_t as described in <inttypes.h>
    */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*  thus, all fields of the struct sockaddr_in
        are filled in
        print them
    */
    
    #define PRINT_SOCKADDR_IN
    #ifndef PRINT_SOCKADDR_IN
    printf("sockaddr_in.sin_family = %d\n", server_addr.sin_family);             //  2
    printf("sockaddr_in.sin_port = %d\n", server_addr.sin_port);                 // listen_port little-endian -> big-endian
    printf("sockaddr_in.sin_addr.s_addr = %d\n", server_addr.sin_addr.s_addr);   //  0
    printf("IP: %s\n", inet_ntoa(server_addr.sin_addr));
    #endif // PRINT_SOCKADDR_IN

    // bind socket and adress
    int res = bind(socket_descriptor, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(res < 0)
    {
        printf("Can't bind socket and adress\n");
        close(socket_descriptor);
        return -2;
    }

    /*  struct linger 
            {
                //  is there a delay when calling the func close()
                //  if l_onoff = 0 will be no delay
                //  if l_onoff = 1 will be delay
                int   l_onoff;     // задержка начата 
                // l_linger determines how many seconds the socket should remain open
                int   l_linger;    // задержать на сколько секунд 
            };
            //  Structure used to manipulate the SO_LINGER option.
    */
    struct linger linger_opt = { 1, 0 };

    /*
        int setsockopt(int sockfd,                      // socket descriptor = soket()
                        int level,                      // the level at which the option is define
                                                        // SOL_SOCKET - for socket-level options
                                                        // IPPROTO_IP or IPPROTO_IPV6 - for options
                                                        // on level IPv4 or IPv6
                        int optname,                    // name of option whose value is being set
                        const void optval[.optlen],     // pointer to the buffer where the option is located
                        socklen_t optlen);              // length of this buffer
        //  SO_LINGER: delay in closing if there is unsent information
    */
    res = setsockopt(socket_descriptor, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));
    if(res != 0)
    {
        printf("Can't set socket options (setsockopt()): level = SOL_SOCKET, optname = SO_LINGER\n");
        close(socket_descriptor);
        return -3;
    }

    int backlog_len = 16; // queue length

    /*  prepare the socket to accept incomming connections
        applies only to sockets of the type  SOCK_STREAM, SOCK_SEQPACKET
        int listen(int sockfd, int backlog)
        backlog:    The backlog argument defines the maximum length to which the queue
                    of pending connections for sockfd may grow.  If a connection
                    request arrives when the queue is full, the client may receive an
                    error with an indication of ECONNREFUSED or, if the underlying
                    protocol supports retransmission, the request may be ignored so
                    that a later reattempt at connection succeeds.
    */
    res = listen(socket_descriptor, backlog_len);
    if(res != 0)
    {
        printf("Error in func listen():\n");
        printf("Can't marks the socket referred to by sock_d(%d) as a passive socket", socket_descriptor);
        close(socket_descriptor);
        return -4;
    }

    // print_str_sockaddr_in(&server_addr);

    socklen_t len_peer_addr = sizeof(struct sockaddr_in); // 16 bytes
    struct sockaddr_in* peer_addr = (sockaddr_in*) malloc(len_peer_addr * backlog_len);

    int* s = NULL;
    pthread_t* thread = (pthread_t*) malloc(sizeof(pthread_t) * backlog_len);

    bool stop_flaf = true;
    for(int i = 0; stop_flaf; (++i) %= 16)
    {
        s = (int*) malloc(4);
        /* see man accept */
        *s = accept(socket_descriptor, (struct sockaddr*) &(peer_addr[i]), &len_peer_addr);
        if(*s == -1)
        {
            printf("Can't accept\n");
            close(socket_descriptor);
            return -5;
        }
        {
            #define PRINT_SER_CL_SOCKADDR_IN
            #ifndef PRINT_SER_CL_SOCKADDR_IN
            struct sockaddr_in run_ser_addr;
            memset(&run_ser_addr, 0, sizeof(struct sockaddr_in));
            socklen_t len = sizeof(run_ser_addr);
            getsockname(s, (struct sockaddr*) &run_ser_addr, &len);
            /* print server IP */
            print_str_sockaddr_in(&run_ser_addr);
            /* print client IP */
            print_str_sockaddr_in(&peer_addr);
            printf("\n");
            #endif // PRINT_SER_CL_SOCKADDR_IN
        }

        // res = client_processing(s);
        struct cl_thread_arg* arg = (cl_thread_arg*) malloc(sizeof(struct cl_thread_arg));
        arg -> s = s;
        arg -> stop_flag = &stop_flaf;
        pthread_create(&(thread[i]), NULL, cl_thread, arg);
    }

    free(thread);
    free(peer_addr);
    sleep(1);
    close(socket_descriptor);
    return 0;
}