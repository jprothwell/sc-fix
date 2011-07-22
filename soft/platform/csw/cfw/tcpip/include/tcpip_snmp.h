/*
 * Copyright (c) 2001, 2002 Leon Woestenberg <leon.woestenberg@axon.tv>
 * Copyright (c) 2001, 2002 Axon Digital Design B.V., The Netherlands.
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
 * Author: Leon Woestenberg <leon.woestenberg@axon.tv>
 *
 */
#ifdef CFW_TCPIP_SUPPORT
#ifndef __LWIP_SNMP_H__
#define __LWIP_SNMP_H__

#include "tcpip_opt.h"

/* SNMP support available? */
#if defined(LWIP_SNMP) && (LWIP_SNMP > 0)

/* network interface */
VOID snmp_add_ifinoctets(UINT32 value);
VOID snmp_inc_ifinucastpkts(VOID);
VOID snmp_inc_ifinnucastpkts(VOID);
VOID snmp_inc_ifindiscards(VOID);
VOID snmp_add_ifoutoctets(UINT32 value);
VOID snmp_inc_ifoutucastpkts(VOID);
VOID snmp_inc_ifoutnucastpkts(VOID);
VOID snmp_inc_ifoutdiscards(VOID);

/* IP */
VOID snmp_inc_ipinreceives(VOID);
VOID snmp_inc_ipindelivers(VOID);
VOID snmp_inc_ipindiscards(VOID);
VOID snmp_inc_ipoutdiscards(VOID);
VOID snmp_inc_ipoutrequests(VOID);
VOID snmp_inc_ipunknownprotos(VOID);
VOID snmp_inc_ipnoroutes(VOID);
VOID snmp_inc_ipforwdatagrams(VOID);

/* ICMP */
VOID snmp_inc_icmpinmsgs(VOID);
VOID snmp_inc_icmpinerrors(VOID);
VOID snmp_inc_icmpindestunreachs(VOID);
VOID snmp_inc_icmpintimeexcds(VOID);
VOID snmp_inc_icmpinparmprobs(VOID);
VOID snmp_inc_icmpinsrcquenchs(VOID);
VOID snmp_inc_icmpinredirects(VOID);
VOID snmp_inc_icmpinechos(VOID);
VOID snmp_inc_icmpinechoreps(VOID);
VOID snmp_inc_icmpintimestamps(VOID);
VOID snmp_inc_icmpintimestampreps(VOID);
VOID snmp_inc_icmpinaddrmasks(VOID);
VOID snmp_inc_icmpinaddrmaskreps(VOID);
VOID snmp_inc_icmpoutmsgs(VOID);
VOID snmp_inc_icmpouterrors(VOID);
VOID snmp_inc_icmpoutdestunreachs(VOID);
VOID snmp_inc_icmpouttimeexcds(VOID);
VOID snmp_inc_icmpoutparmprobs(VOID);
VOID snmp_inc_icmpoutsrcquenchs(VOID);
VOID snmp_inc_icmpoutredirects(VOID);
VOID snmp_inc_icmpoutechos(VOID);
VOID snmp_inc_icmpoutechoreps(VOID);
VOID snmp_inc_icmpouttimestamps(VOID);
VOID snmp_inc_icmpouttimestampreps(VOID);
VOID snmp_inc_icmpoutaddrmasks(VOID);
VOID snmp_inc_icmpoutaddrmaskreps(VOID);

/* TCP */
VOID snmp_inc_tcpactiveopens(VOID);
VOID snmp_inc_tcppassiveopens(VOID);
VOID snmp_inc_tcpattemptfails(VOID);
VOID snmp_inc_tcpestabresets(VOID);
VOID snmp_inc_tcpcurrestab(VOID);
VOID snmp_inc_tcpinsegs(VOID);
VOID snmp_inc_tcpoutsegs(VOID);
VOID snmp_inc_tcpretranssegs(VOID);
VOID snmp_inc_tcpinerrs(VOID);
VOID snmp_inc_tcpoutrsts(VOID);

/* UDP */
VOID snmp_inc_udpindatagrams(VOID);
VOID snmp_inc_udpnoports(VOID);
VOID snmp_inc_udpinerrors(VOID);
VOID snmp_inc_udpoutdatagrams(VOID);

/* LWIP_SNMP support not available */
/* define everything to be empty */
#else

/* network interface */
#define snmp_add_ifinoctets(value)
#define snmp_inc_ifinucastpkts()
#define snmp_inc_ifinnucastpkts()
#define snmp_inc_ifindiscards()
#define snmp_add_ifoutoctets(value)
#define snmp_inc_ifoutucastpkts()
#define snmp_inc_ifoutnucastpkts()
#define snmp_inc_ifoutdiscards()

/* IP */
#define snmp_inc_ipinreceives()
#define snmp_inc_ipindelivers()
#define snmp_inc_ipindiscards()
#define snmp_inc_ipoutdiscards()
#define snmp_inc_ipoutrequests()
#define snmp_inc_ipunknownprotos()
#define snmp_inc_ipnoroutes()
#define snmp_inc_ipforwdatagrams()

/* ICMP */
#define snmp_inc_icmpinmsgs()
#define snmp_inc_icmpinerrors()
#define snmp_inc_icmpindestunreachs()
#define snmp_inc_icmpintimeexcds()
#define snmp_inc_icmpinparmprobs()
#define snmp_inc_icmpinsrcquenchs()
#define snmp_inc_icmpinredirects()
#define snmp_inc_icmpinechos()
#define snmp_inc_icmpinechoreps()
#define snmp_inc_icmpintimestamps()
#define snmp_inc_icmpintimestampreps()
#define snmp_inc_icmpinaddrmasks()
#define snmp_inc_icmpinaddrmaskreps()
#define snmp_inc_icmpoutmsgs()
#define snmp_inc_icmpouterrors()
#define snmp_inc_icmpoutdestunreachs()
#define snmp_inc_icmpouttimeexcds()
#define snmp_inc_icmpoutparmprobs()
#define snmp_inc_icmpoutsrcquenchs()
#define snmp_inc_icmpoutredirects()
#define snmp_inc_icmpoutechos()
#define snmp_inc_icmpoutechoreps()
#define snmp_inc_icmpouttimestamps()
#define snmp_inc_icmpouttimestampreps()
#define snmp_inc_icmpoutaddrmasks()
#define snmp_inc_icmpoutaddrmaskreps()
/* TCP */
#define snmp_inc_tcpactiveopens()
#define snmp_inc_tcppassiveopens()
#define snmp_inc_tcpattemptfails()
#define snmp_inc_tcpestabresets()
#define snmp_inc_tcpcurrestab()
#define snmp_inc_tcpinsegs()
#define snmp_inc_tcpoutsegs()
#define snmp_inc_tcpretranssegs()
#define snmp_inc_tcpinerrs()
#define snmp_inc_tcpoutrsts()

/* UDP */
#define snmp_inc_udpindatagrams()
#define snmp_inc_udpnoports()
#define snmp_inc_udpinerrors()
#define snmp_inc_udpoutdatagrams()

#endif

#endif /* __LWIP_SNMP_H__ */
#endif

