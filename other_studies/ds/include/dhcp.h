/********************************************************************************
Project: DHCP
Name: Netanel Shmuel
Reviewer: Jonathan  
Date: 14.12.2023
Version: 1.0
********************************************************************************/

#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h> /* size_t */

typedef unsigned long ip_ty;
typedef struct dhcp dhcp_ty;


/*
undefined behaviour: invalid net_mask_ or prefix_ according to each other
                     invalid net_mask_ if there is a zero that is between the sequence of ones digits from the beginning
return value: on failure - return NULL
time complexity: O(1)
*/
dhcp_ty *DhcpCreate(ip_ty net_mask_, ip_ty prefix_);


/*
notes: dhcp_ can be NULL pointer
time complexity: O(size)
*/
void DhcpDestroy(dhcp_ty *dhcp_);


/*
undefined behaviour: dhcp_ NULL pointer
                     requested ip not match to prefix (0 is allowed)
return value: available IP address, if there is no available address - return address 0
time complexity: O(log(size))
*/
ip_ty DhcpAllocateIP(dhcp_ty *dhcp_, ip_ty requested_ip_);


/*
undefined behaviour: dhcp_ NULL pointer
                     requested ip not match to prefix (0 is allowed)
time complexity: O(log(size))
*/
void DhcpFreeIP(dhcp_ty *dhcp_, ip_ty requested_ip_);


/*
undefined behaviour: dhcp_ NULL pointer
time complexity: O(size)
*/
size_t DhcpCountFree(const dhcp_ty *dhcp_);


/*
description: check if all IP addresses was taken
undefined behaviour: dhcp_ NULL pointer
time complexity: O(1)
*/
int DhcpIsAllTaken(const dhcp_ty *dhcp_);


#endif /* __DHCP_H__ */