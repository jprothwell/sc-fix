 
/*struct for  simulator*/
 
#if 0
#define debug_test   

#ifdef MMI_ON_WIN32
typedef char			S8;			
typedef char			*PS8;		
typedef unsigned char   U8;			
typedef unsigned char	*PU8;	

typedef short			S16; 		
typedef short			*PS16; 		
typedef unsigned short  U16; 		
typedef unsigned short  *PU16;
typedef unsigned int	U32;

#define kal_uint8  U8
#define kal_uint16 U16
#define kal_uint32 U32

typedef enum {
   PHB_LND,
   PHB_LNM,
   PHB_LNR
} phb_ln_type_enum;

typedef struct
{
   kal_uint8 type;
   kal_uint8 length;
   kal_uint8 number[41];//[MAX_CC_ADDR_LEN];
} l4c_number_struct;

typedef struct l4_name_struct
{
	kal_uint8 name_length;
	kal_uint8 name_dcs;
	kal_uint8 name[32];
} l4_name_struct;
typedef struct {
    kal_uint8 rtc_sec;
    kal_uint8 rtc_min;
    kal_uint8 rtc_hour;
    kal_uint8 rtc_day;
    kal_uint8 rtc_mon;
    kal_uint8 rtc_wday;
    kal_uint8 rtc_year;
} rtc_format_struct;

typedef struct {
   l4c_number_struct tel;
   l4_name_struct alpha_id;
   rtc_format_struct time;
   kal_uint8 count;
} l4c_phb_ln_entry_struct;
typedef struct l4c_result_struct
{
   kal_uint8   flag;    /*l4c_result_enum*/
   kal_uint16   cause;

} l4c_result_struct;

typedef struct
	{
	//	LOCAL_PARA_HDR
		kal_uint8	type;
		kal_uint8	seq_id;
	} mmi_phb_get_last_number_req_struct;
	typedef struct
	{
	//	LOCAL_PARA_HDR
		l4c_result_struct	result;
		kal_uint8	no_list;
		kal_uint8	type;
		kal_uint8	more_data;
		l4c_phb_ln_entry_struct	list[10];   
		//l4c_phb_ln_entry_struct	list[1];
	} mmi_phb_get_last_number_rsp_struct;	
	typedef struct
	{
	//	LOCAL_PARA_HDR
		kal_uint8	type;
		kal_uint8	no_data;
		kal_uint8	index;
		l4c_phb_ln_entry_struct	entry;
	} mmi_phb_set_last_number_req_struct;
	typedef struct
	{
	//	LOCAL_PARA_HDR
		l4c_result_struct	result;
		kal_uint16	no_list;
		l4c_phb_ln_entry_struct	list[1];
	} mmi_phb_set_last_number_rsp_struct;
	typedef struct
	{
	//	LOCAL_PARA_HDR
		kal_uint8	type;
		kal_uint8	index;
		kal_uint8	no_data;
	} mmi_phb_del_last_number_req_struct;
	typedef struct
	{
	//	LOCAL_PARA_HDR
		l4c_result_struct	result;
		kal_uint16	no_list;
		l4c_phb_ln_entry_struct	list[10];//[1];
	} mmi_phb_del_last_number_rsp_struct;


#endif
/*struct for  simulator*/

 
void Initialization_phb_ln(void);
void * SetLastNoRsp(mmi_phb_set_last_number_rsp_struct* SetRspStruct);
void *GetLastNoRsp(mmi_phb_get_last_number_rsp_struct* GetRspEntries);
void * DeleteAllCallsRsp(mmi_phb_del_last_number_rsp_struct * DelRspEntries);
//void * IncomingCallInd(void);

void CallHistoryWriteToFile(kal_uint16 FileType,char Isupdate) ; 
void CallHistoryReadFromFile(kal_uint16 FileType);
#endif
 
