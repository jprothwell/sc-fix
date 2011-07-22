/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifdef CFW_TCPIP_SUPPORT

#ifndef __LWIP_ARCH_H__
#define __LWIP_ARCH_H__



#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif


//#include "cc.h"
#ifndef CII_VERSION
#define 	CII_VERSION				 //temp by zouy

#endif
#define LWIP_PROVIDE_ERRNO
#define CFW_TCPIP_TS_ID       40




#ifndef PACK_STRUCT_BEGIN
#define PACK_STRUCT_BEGIN
#endif /* PACK_STRUCT_BEGIN */

#ifndef PACK_STRUCT_END
#define PACK_STRUCT_END
#endif /* PACK_STRUCT_END */

#ifndef PACK_STRUCT_FIELD
#define PACK_STRUCT_FIELD(x) x
#endif /* PACK_STRUCT_FIELD */

#ifdef LWIP_PROVIDE_ERRNO

#define  ERR_TCPIP_EPERM     1        /* Operation not permitted */
#define  ERR_TCPIP_ENOENT     2        /* No such file or directory */
#define  ERR_TCPIP_ESRCH     3        /* No such process */
#define  ERR_TCPIP_EINTR     4        /* Interrupted system call */
#define  ERR_TCPIP_EIO     5        /* I/O error */
#define  ERR_TCPIP_ENXIO     6        /* No such device or address */
#define  ERR_TCPIP_E2BIG     7        /* Arg list too long */
#define  ERR_TCPIP_ENOEXEC     8        /* Exec format error */
#define  ERR_TCPIP_EBADF     9        /* Bad file number */
#define  ERR_TCPIP_ECHILD    10        /* No child processes */
#define  ERR_TCPIP_EAGAIN    11        /* Try again */
#define  ERR_TCPIP_ENOMEM    12        /* Out of memory */
#define  ERR_TCPIP_EACCES    13        /* Permission denied */
#define  ERR_TCPIP_EFAULT    14        /* Bad address */
#define  ERR_TCPIP_ENOTBLK    15        /* Block device required */
#define  ERR_TCPIP_EBUSY    16        /* Device or resource busy */
#define  ERR_TCPIP_EEXIST    17        /* File exists */
#define  ERR_TCPIP_EXDEV    18        /* Cross-device link */
#define  ERR_TCPIP_ENODEV    19        /* No such device */
#define  ERR_TCPIP_ENOTDIR    20        /* Not a directory */
#define  ERR_TCPIP_EISDIR    21        /* Is a directory */
#define  ERR_TCPIP_EINVAL    22        /* Invalid argument */
#define  ERR_TCPIP_ENFILE    23        /* File table overflow */
#define  ERR_TCPIP_EMFILE    24        /* Too many open files */
#define  ERR_TCPIP_ENOTTY    25        /* Not a typewriter */
#define  ERR_TCPIP_ETXTBSY    26        /* Text file busy */
#define  ERR_TCPIP_EFBIG    27        /* File too large */
#define  ERR_TCPIP_ENOSPC    28        /* No space left on device */
#define  ERR_TCPIP_ESPIPE    29        /* Illegal seek */
#define  ERR_TCPIP_EROFS    30        /* Read-only file system */
#define  ERR_TCPIP_EMLINK    31        /* Too many links */
#define  ERR_TCPIP_EPIPE    32        /* Broken pipe */
#define  ERR_TCPIP_EDOM    33        /* Math argument out of domain of func */
#define  ERR_TCPIP_ERANGE    34        /* Math result not representable */
#define  ERR_TCPIP_EDEADLK    35        /* Resource deadlock would occur */
#define  ERR_TCPIP_ENAMETOOLONG  36    /* File name too long */
#define  ERR_TCPIP_ENOLCK    37        /* No record locks available */
#define  ERR_TCPIP_ENOSYS    38        /* Function not implemented */
#define  ERR_TCPIP_ENOTEMPTY  39        /* Directory not empty */
#define  ERR_TCPIP_ELOOP    40        /* Too many symbolic links encountered */
#define  ERR_TCPIP_EWOULDBLOCK  ERR_TCPIP_EAGAIN    /* Operation would block */
#define  ERR_TCPIP_ENOMSG    42        /* No message of desired type */
#define  ERR_TCPIP_EIDRM    43        /* Identifier removed */
#define  ERR_TCPIP_ECHRNG    44        /* Channel number out of range */
#define  ERR_TCPIP_EL2NSYNC  45        /* Level 2 not synchronized */
#define  ERR_TCPIP_EL3HLT    46        /* Level 3 halted */
#define  ERR_TCPIP_EL3RST    47        /* Level 3 reset */
#define  ERR_TCPIP_ELNRNG    48        /* Link number out of range */
#define  ERR_TCPIP_EUNATCH    49        /* Protocol driver not attached */
#define  ERR_TCPIP_ENOCSI    50        /* No CSI structure available */
#define  ERR_TCPIP_EL2HLT    51        /* Level 2 halted */
#define  ERR_TCPIP_EBADE    52        /* Invalid exchange */
#define  ERR_TCPIP_EBADR    53        /* Invalid request descriptor */
#define  ERR_TCPIP_EXFULL    54        /* Exchange full */
#define  ERR_TCPIP_ENOANO    55        /* No anode */
#define  ERR_TCPIP_EBADRQC    56        /* Invalid request code */
#define  ERR_TCPIP_EBADSLT    57        /* Invalid slot */

#define  ERR_TCPIP_EDEADLOCK  ERR_TCPIP_EDEADLK

#define  ERR_TCPIP_EBFONT    59        /* Bad font file format */
#define  ERR_TCPIP_ENOSTR    60        /* Device not a stream */
#define  ERR_TCPIP_ENODATA    61        /* No data available */
#define  ERR_TCPIP_ETIME    62        /* Timer expired */
#define  ERR_TCPIP_ENOSR    63        /* Out of streams resources */
#define  ERR_TCPIP_ENONET    64        /* Machine is not on the network */
#define  ERR_TCPIP_ENOPKG    65        /* Package not installed */
#define  ERR_TCPIP_EREMOTE    66        /* Object is remote */
#define  ERR_TCPIP_ENOLINK    67        /* Link has been severed */
#define  ERR_TCPIP_EADV    68        /* Advertise error */
#define  ERR_TCPIP_ESRMNT    69        /* Srmount error */
#define  ERR_TCPIP_ECOMM    70        /* Communication error on send */
#define  ERR_TCPIP_EPROTO    71        /* Protocol error */
#define  ERR_TCPIP_EMULTIHOP  72        /* Multihop attempted */
#define  ERR_TCPIP_EDOTDOT    73        /* RFS specific error */
#define  ERR_TCPIP_EBADMSG    74        /* Not a data message */
#define  ERR_TCPIP_EOVERFLOW  75        /* Value too large for defined data type */
#define  ERR_TCPIP_ENOTUNIQ  76        /* Name not unique on network */
#define  ERR_TCPIP_EBADFD    77        /* File descriptor in bad state */
#define  ERR_TCPIP_EREMCHG    78        /* Remote address changed */
#define  ERR_TCPIP_ELIBACC    79        /* Can not access a needed shared library */
#define  ERR_TCPIP_ELIBBAD    80        /* Accessing a corrupted shared library */
#define  ERR_TCPIP_ELIBSCN    81        /* .lib section in a.out corrupted */
#define  ERR_TCPIP_ELIBMAX    82        /* Attempting to link in too many shared libraries */
#define  ERR_TCPIP_ELIBEXEC  83        /* Cannot exec a shared library directly */
#define  ERR_TCPIP_EILSEQ    84        /* Illegal byte sequence */
#define  ERR_TCPIP_ERESTART  85        /* Interrupted system call should be restarted */
#define  ERR_TCPIP_ESTRPIPE  86        /* Streams pipe error */
#define  ERR_TCPIP_EUSERS    87        /* Too many users */
#define  ERR_TCPIP_ENOTSOCK  88        /* Socket operation on non-socket */
#define  ERR_TCPIP_EDESTADDRREQ  89    /* Destination address required */
#define  ERR_TCPIP_EMSGSIZE  90        /* Message too long */
#define  ERR_TCPIP_EPROTOTYPE  91        /* Protocol wrong type for socket */
#define  ERR_TCPIP_ENOPROTOOPT  92    /* Protocol not available */
#define  ERR_TCPIP_EPROTONOSUPPORT  93    /* Protocol not supported */
#define  ERR_TCPIP_ESOCKTNOSUPPORT  94    /* Socket type not supported */
#define  ERR_TCPIP_EOPNOTSUPP  95        /* Operation not supported on transport endpoint */
#define  ERR_TCPIP_EPFNOSUPPORT  96    /* Protocol family not supported */
#define  ERR_TCPIP_EAFNOSUPPORT  97    /* Address family not supported by protocol */
#define  ERR_TCPIP_EADDRINUSE  98        /* Address already in use */
#define  ERR_TCPIP_EADDRNOTAVAIL  99    /* Cannot assign requested address */
#define  ERR_TCPIP_ENETDOWN  100        /* Network is down */
#define  ERR_TCPIP_ENETUNREACH  101    /* Network is unreachable */
#define  ERR_TCPIP_ENETRESET  102        /* Network dropped connection because of reset */
#define  ERR_TCPIP_ECONNABORTED  103    /* Software caused connection abort */
#define  ERR_TCPIP_ECONNRESET  104    /* Connection reset by peer */
#define  ERR_TCPIP_ENOBUFS    105        /* No buffer space available */
#define  ERR_TCPIP_EISCONN    106        /* Transport endpoint is already connected */
#define  ERR_TCPIP_ENOTCONN  107        /* Transport endpoint is not connected */
#define  ERR_TCPIP_ESHUTDOWN  108        /* Cannot send after transport endpoint shutdown */
#define  ERR_TCPIP_ETOOMANYREFS  109    /* Too many references: cannot splice */
#define  ERR_TCPIP_ETIMEDOUT  110        /* Connection timed out */
#define  ERR_TCPIP_ECONNREFUSED  111    /* Connection refused */
#define  ERR_TCPIP_EHOSTDOWN  112        /* Host is down */
#define  ERR_TCPIP_EHOSTUNREACH  113    /* No route to host */
#define  ERR_TCPIP_EALREADY  114        /* Operation already in progress */
#define  ERR_TCPIP_EINPROGRESS  115    /* Operation now in progress */
#define  ERR_TCPIP_ESTALE    116        /* Stale NFS file handle */
#define  ERR_TCPIP_EUCLEAN    117        /* Structure needs cleaning */
#define  ERR_TCPIP_ENOTNAM    118        /* Not a XENIX named type file */
#define  ERR_TCPIP_ENAVAIL    119        /* No XENIX semaphores available */
#define  ERR_TCPIP_EISNAM    120        /* Is a named type file */
#define  ERR_TCPIP_EREMOTEIO  121        /* Remote I/O error */
#define  ERR_TCPIP_EDQUOT    122        /* Quota exceeded */

#define  ERR_TCPIP_ENOMEDIUM  123        /* No medium found */
#define  ERR_TCPIP_EMEDIUMTYPE  124    /* Wrong medium type */

#define  ERR_TCPIP_ENSROK        0    /* DNS server returned answer with no data */
#define  ERR_TCPIP_ENSRNODATA    160    /* DNS server returned answer with no data */
#define  ERR_TCPIP_ENSRFORMERR    161    /* DNS server claims query was misformatted */
#define  ERR_TCPIP_ENSRSERVFAIL 162    /* DNS server returned general failure */
#define  ERR_TCPIP_ENSRNOTFOUND 163    /* Domain name not found */
#define  ERR_TCPIP_ENSRNOTIMP    164    /* DNS server does not implement requested operation */
#define  ERR_TCPIP_ENSRREFUSED    165    /* DNS server refused query */
#define  ERR_TCPIP_ENSRBADQUERY 166    /* Misformatted DNS query */
#define  ERR_TCPIP_ENSRBADNAME    167    /* Misformatted domain name */
#define  ERR_TCPIP_ENSRBADFAMILY 168    /* Unsupported address family */
#define  ERR_TCPIP_ENSRBADRESP    169    /* Misformatted DNS reply */
#define  ERR_TCPIP_ENSRCONNREFUSED    170    /* Could not contact DNS servers */
#define  ERR_TCPIP_ENSRTIMEOUT    171    /* Timeout while contacting DNS servers */
#define  ERR_TCPIP_ENSROF        172    /* End of file */
#define  ERR_TCPIP_ENSRFILE    173    /* Error reading file */
#define  ERR_TCPIP_ENSRNOMEM    174    /* Out of memory */
#define  ERR_TCPIP_ENSRDESTRUCTION    175    /* Application terminated lookup */
#define  ERR_TCPIP_ENSRQUERYDOMAINTOOLONG    176    /* Domain name is too long */
#define  ERR_TCPIP_ENSRCNAMELOOP    177    /* Domain name is too long */


#endif /* LWIP_PROVIDE_ERRNO */

#endif /* __LWIP_ARCH_H__ */

#endif

