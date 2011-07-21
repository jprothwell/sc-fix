#ifdef JATAAYU_SUPPORT
#include <ddl.h>
#include <jcal.h>

#include "jmms_staticdefines.h"
#include "jmms_str_id.h"
JC_UINT16 MM1StatusText [] =
{
#if 0
	"Expired",
	"Retrieved",
	"Rejected",
	"Deferred",
	"Unrecognised",
	"Indeterminate",
	"Forwarded",
	"Unreachable"
#endif /* Guoj delete. It's not used now at 2009-11-4 */
    STR_ID_JMMS_DELIVERY_STATUS_EXPIRED,
    STR_ID_JMMS_DELIVERY_STATUS_RETRIEVED,
    STR_ID_JMMS_DELIVERY_STATUS_REJECTED,
    STR_ID_JMMS_DELIVERY_STATUS_DEFFERED,
    STR_ID_JMMS_DELIVERY_STATUS_UNRECOGNISED,
    STR_ID_JMMS_DELIVERY_STATUS_INDETERMINATE,
    STR_ID_JMMS_DELIVERY_STATUS_FORWARDED,
    STR_ID_JMMS_DELIVERY_STATUS_UNREACHABLE
} ;

JC_UINT16 ReadReportStatus[] =
{
#if 0
	"Read",
	"Deleted without being read"
#endif /* Guoj delete. It's not used now at 2009-11-5 */
    STR_ID_JMMS_READ_REPORT_READ,
    STR_ID_JMMS_READ_REPORT_DELETED
} ;

JC_UINT16 MessageClassStr[] =
{
    STR_ID_JMMS_PERSONEL_MSG_CLASS,
    STR_ID_JMMS_ADVERTISEMENT_MSG_CLASS,
    STR_ID_JMMS_INFORMATIONAL_MSG_CLASS,
    STR_ID_JMMS_AUTO_MSG_CLASS,
} ;

JC_UINT16 MessagePriorityStr[] =
{
	STR_ID_MMS_NONE,
	STR_ID_MMS_LOW,
	STR_ID_MMS_NORMAL,
	STR_ID_MMS_HIGH,
} ;


JC_INT8 *ReplyChargingStr[] =
{
	"Requested",
	"Requested text only",
	"Accepted",
	"Accepted text only",
	"None"
};


JC_INT8 *StoreStatusText[] =
{
	"Success",
	"Error-transient-failure",
	"Error-transient-network-problem",
	"Error-permanent-failure",
	"Error-permanent-service-denied",
	"Error-permanent-message-format-corrupt",
	"Error-permanent-message-not-found",
	"Error-permanent-mmbox-full"
} ;

JC_INT8 *ResponseStatusText[] =
{
//0
	"Ok",
	"Error-unspecified",
	"Error-service-denied",
	"Error-message-format-corrupt",
	"Error-sending-address-unresolved",
	"Error-message-not-found",
	"Error-network-problem",
	"Error-content-not-accepted",
	"Error-unsupported-message",
//8
//9
	"Error-transient-failure",
	"Error-transient-sending-address-unresolved",
	"Error-transient-message-not-found",
	"Error-transient-network-problem",
	"Error-transient-partial-success",
//13
//14
	"Error-permanent-failure",
	"Error-permanent-service-denied",
	"Error-permanent-message-format-corrupt",
	"Error-permanent-sending-address-unresolved",
	"Error-permanent-message-not-found",
	"Error-permanent-content-not-accepted",
	"Error-permanent-reply-charging-limitations-not-met",
	"Error-permanent-reply-charging-request-not-accepted",
	"Error-permanent-reply-charging-forwarding-denied",
	"Error-permanent-reply-charging-not-supported",
	"Error-permanent-address-hiding-not-supported"
//24
} ;
#endif
