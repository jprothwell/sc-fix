/**
    @file spc_mailbox.c

    The mailbox ...
*/

#include "spc_mailbox.h"

PROTECTED volatile SPC_MAILBOX_T g_mailbox __attribute__((section (".mailbox")));


