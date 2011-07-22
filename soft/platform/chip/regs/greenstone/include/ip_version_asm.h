#ifndef _IP_VERSION_ASM_H_
#define _IP_VERSION_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'ip_version'."
#endif


/// This is a released IP!
#define RELEASED_IP                              (1)
#define IP_VERSION_NUMBER                        (0X12080314)

//==============================================================================
// IP_Version
//------------------------------------------------------------------------------
/// 
//==============================================================================

//IP_Version
#define IP_VERSION_MAJOR(n)         (((n)&15)<<28)
#define IP_VERSION_MINOR(n)         (((n)&15)<<24)
#define IP_VERSION_YEAR(n)          (((n)&15)<<16)
#define IP_VERSION_MONTH(n)         (((n)&0xFF)<<8)
#define IP_VERSION_DAY(n)           (((n)&0xFF)<<0)




#endif
