#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "dhcp.h"
#include "trie.h"
#include "utils.h"


struct dhcp
{
    trie_ty *ip_trie;
    ip_ty net_mask;
    ip_ty prefix;
};


static int ValidateNetMaskIMP(ip_ty net_mask_) /* TODO: do it in DEBUG_ONLY */
{
    ip_ty net_mask_dop = net_mask_;
    ip_ty mask = 1;

    /* assert */
    assert(0 != net_mask_);

    /* while 0 == mask & net_mask_dop */
    while (0 == (mask & net_mask_dop))
    {
        /* net_mask_dop >>= 1 */
        net_mask_dop >>= 1;
    }

    /* while 1 == mask & net_mask_dop */
    while (1 == (mask & net_mask_dop))
    {
        /* net_mask_dop >>= 1 */
        net_mask_dop >>= 1;
    }

    /* if 0 != net_mask_dop */
    if (0 != net_mask_dop)
    {
        /* return false */
        return 0;
    }

    /* return true */
    return 1;
}

static size_t CalcTrieDepthIMP(ip_ty net_mask_)
{
    ip_ty net_mask_dop = net_mask_;
    size_t ret = 0;

    /* assert */
    assert(ValidateNetMaskIMP(net_mask_)); /* TODO: also check when the net_mask starts */

    /* while 0 == mask & net_mask_dop */
    while (!(1UL & net_mask_dop))
    {
        /* ++zero_part_count */
        ++ret;

        /* net_mask_dop >>= 1 */
        net_mask_dop >>= 1;
    }

    /* return zero_part_count */
    return ret;
}

dhcp_ty *DhcpCreate(ip_ty net_mask_, ip_ty prefix_)
{
    dhcp_ty *dhcp = NULL;
    size_t trie_depth = 0;

    /* assert */
    assert(ValidateNetMaskIMP(net_mask_));
    assert((net_mask_ | prefix_) == net_mask_);
    assert(1 < CalcTrieDepthIMP(net_mask_));

    /* allocate memory for dhcp instance */
    dhcp = (dhcp_ty *)malloc(sizeof(dhcp_ty));

    /* if allocation failed */
    if (NULL == dhcp)
    {
        /* return NULL */
        return NULL;
    }

    /* calculate trie depth according to parameters */
    trie_depth = CalcTrieDepthIMP(net_mask_);

    /* create the dhcp ip_trie */
    dhcp->ip_trie = TrieCreate(trie_depth);

    /* if creation failed */
    if (NULL == dhcp->ip_trie)
    {
        /* Destroy dhcp */
        DhcpDestroy(dhcp);

        /* return NULL */
        return NULL;
    }

    /* initialize other dhcp fields with input parameters */
    dhcp->net_mask = net_mask_;
    dhcp->prefix = prefix_;

    /* allocate 3 IP addresses of the dhcp */
    DhcpAllocateIP(dhcp, prefix_ | 0);                   /* TODO: check it the allocation failed */
    DhcpAllocateIP(dhcp, prefix_ & (~(ip_ty)0));
    DhcpAllocateIP(dhcp, prefix_ & (~(ip_ty)0 - 1));

    /* return dhcp */
    return dhcp;
}

void DhcpDestroy(dhcp_ty *dhcp_)
{
    if (NULL == dhcp_)
    {
        return;
    }

    /* Destroy trie */
    TrieDestroy(dhcp_->ip_trie);

    /* DEBUG_ONLY - corrupt dhcp_ */
    DEBUG_ONLY(dhcp_->ip_trie = BAD_MEM(trie_ty););

    /* free dhcp_ */
    free(dhcp_);
}

ip_ty DhcpAllocateIP(dhcp_ty *dhcp_, ip_ty requested_ip_)
{
    size_t trie_depth = 0;

    /* assert */
    assert(NULL != dhcp_);
    assert((requested_ip_ & dhcp_->net_mask) == dhcp_->prefix); /* TODO: do it in function */

    trie_depth = CalcTrieDepthIMP(dhcp_->net_mask);

    /* return TrieGet(dhcp_ trie, requested_ip_) */
    return  (dhcp_->prefix | TrieGet(dhcp_->ip_trie, ((1<<trie_depth)-1) & requested_ip_)); /* TODO: do it with ~net_mask & req_ip */
}

void DhcpFreeIP(dhcp_ty *dhcp_, ip_ty requested_ip_)
{
    size_t trie_depth = 0;

    /* assert */
    assert(NULL != dhcp_);                                         /* TODO: print to user if ip_ to free was not allocated */
    assert((requested_ip_ & dhcp_->net_mask) == dhcp_->prefix);

    trie_depth = CalcTrieDepthIMP(dhcp_->net_mask);

    /* TrieFree(dhcp_ trie, requested_ip_) */
    TrieFree(dhcp_->ip_trie, ((1<<trie_depth)-1) & requested_ip_);
}

size_t DhcpCountFree(const dhcp_ty *dhcp_)
{
    /* assert */
    assert(NULL != dhcp_);

    /* return TrieCountAvailable(dhcp_ trie) */
    return TrieCountAvailable(dhcp_->ip_trie);
}

int DhcpIsAllTaken(const dhcp_ty *dhcp_)
{
    /* assert */
    assert(NULL != dhcp_);

    /* return TrieIsAllTaken(dhcp_ trie) */
    return TrieIsAllTaken(dhcp_->ip_trie);
}

