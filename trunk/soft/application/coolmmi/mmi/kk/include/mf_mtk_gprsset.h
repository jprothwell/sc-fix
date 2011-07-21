#include "mmidatatype.h"
#include "custom_mmi_default_value.h" 

#define MF_TRACE_ON
#define MF_GPRS_DATA_MNC_MAX 30     //需要进一步定义枚举来确认

enum
{
	DATA_ACC_ChinaMobile,
	DATA_ACC_ChinaUnicom,
	DATA_ACC_FET,
	DATA_ACC_TWM,
  DATA_ACC_CHT_Chungwa,
	DATA_ACC_VIBO,
	DATA_ACC_KGT,
	DATA_ACC_Mobiletai,
	DATA_ACC_TransAsia,
	DATA_ACC_CSL,
	DATA_ACC_PCCW,
  DATA_ACC_SmarTone_Vodafone,
	DATA_ACC_PEOPLES,
	DATA_ACC_Hutchison,
	DATA_ACC_SingTel,
	DATA_ACC_M1,
	DATA_ACC_StarHub,
	DATA_ACC_Vinaphone,
	DATA_ACC_Viettel_Mobile,
	DATA_ACC_Mobifone,
  DATA_ACC_Vietnam_Mobile,
	DATA_ACC_Beeline_VN,
	DATA_ACC_DTAC,
  DATA_ACC_TRUEMove_TAO,
	DATA_ACC_AIS,
	DATA_ACC_Telkomsel,
	DATA_ACC_Indosat,
	DATA_ACC_ID,
	DATA_ACC_Telkomsel1,
	DATA_ACC_IM3,
	DATA_ACC_Excelcom,
	DATA_ACC_TELSTRA,
	DATA_ACC_OPTUS,
	DATA_ACC_VOD,
	DATA_ACC_3AU,
	DATA_ACC_Maxis,
	DATA_ACC_Digi,
	DATA_ACC_Celcom,
	DATA_ACC_Globe,
	DATA_ACC_Smart,
	DATA_ACC_SUN,
	DATA_ACC_Airtel,
	DATA_ACC_Airtel1,
	DATA_ACC_BSNL,
	DATA_ACC_BPL,
	DATA_ACC_Vodafone_Hutch,
  DATA_ACC_Vodafone_Hutch1,
  DATA_ACC_Idea_Cellular,
  DATA_ACC_Idea_Cellular1,
	DATA_ACC_Reliance,
	DATA_ACC_Reliance1,
	DATA_ACC_TaTa,
	DATA_ACC_MTNL,
	DATA_ACC_Spice,
	DATA_ACC_Spice1,
	DATA_ACC_MTC_Vodafone_KW,
	DATA_ACC_Wataniya,
	DATA_ACC_STC,
	DATA_ACC_Mobily,
	DATA_ACC_Zain,
	DATA_ACC_Etisalat,
	DATA_ACC_du,
	DATA_ACC_Oman_Mobile,
	DATA_ACC_Nawras,
	DATA_ACC_BATELCO,
	DATA_ACC_MTC_Vodafone_BH,
	DATA_ACC_Zain1,
	DATA_ACC_Umniah,
	DATA_ACC_Sabafon,
	DATA_ACC_MobiNil,
	DATA_ACC_Vodafone,
	DATA_ACC_Etisalat1,
	DATA_ACC_AsiaCell,
	DATA_ACC_Zain_IQ,
	DATA_ACC_Djezzy,
	DATA_ACC_Qtel,
	DATA_ACC_H3G,
	DATA_ACC_O2,
	DATA_ACC_Orange,
	DATA_ACC_T_Mobile,
	DATA_ACC_Vodafone1,
	DATA_ACC_KPN,
	DATA_ACC_Orange1,
	DATA_ACC_T_Mobile1,
	DATA_ACC_Telfort,
	DATA_ACC_Vodafone2,
	DATA_ACC_MTN,
	DATA_ACC_Vodacom,
	DATA_ACC_CellC,
	DATA_ACC_Vivo,
	DATA_ACC_SCTL_GPRS,
	DATA_ACC_OI,
	DATA_ACC_Telemig_GPRS,
	DATA_ACC_TIM,
	DATA_ACC_Claro,
	DATA_ACC_BRT,
	DATA_ACC_Amazonia_GPRS,
	MF_DATA_ACCOUNT_PROFILE_NUM
};

typedef struct
{
    kal_uint8 qos_length;   /* QOS identifer bit */
    kal_uint8 unused1;      /* Unused Bit */
    kal_uint8 delay_class;
    kal_uint8 reliability_class;
    kal_uint8 peak_throughput;
    kal_uint8 unused2;      /* Unused Bit */
    kal_uint8 precedence_class;
    kal_uint8 unused3;      /* Unused Bit */
    kal_uint8 mean_throughput;
    kal_uint8 traffic_class;
    kal_uint8 delivery_order;
    kal_uint8 delivery_of_err_sdu;
    kal_uint8 max_sdu_size;
    kal_uint8 max_bitrate_up_lnk;
    kal_uint8 max_bitrate_down_lnk;
    kal_uint8 residual_bit_err_rate;
    kal_uint8 sdu_err_ratio;
    kal_uint8 transfer_delay;
    kal_uint8 traffic_hndl_priority;
    kal_uint8 guarntd_bit_rate_up_lnk;
    kal_uint8 guarntd_bit_rate_down_lnk;
} mf_nvram_editor_qos_struct;

typedef struct
{
    kal_uint8 context_id;
    kal_uint8 name_length;
    kal_uint8 name_dcs;
    kal_uint8 authentication_type;
    kal_uint8 name[32];
    kal_uint8 user_name[MAX_GPRS_USER_NAME_LEN];
    kal_uint8 password[MAX_GPRS_PASSWORD_LEN];
    kal_uint8 dns[4];
    kal_uint8 apn[MAX_GPRS_APN_LEN];
    kal_uint8 apn_length;
    mf_nvram_editor_qos_struct req_qos;
} mf_nvram_ef_abm_gprs_profile_struct;

typedef struct
{
	kal_uint8  MCC[8]; //e.g  460
	kal_uint8  MNC[MF_GPRS_DATA_MNC_MAX][4]; //e.g 00,01,02
	kal_uint8 AlterDNS[4];//alternative DNS
	mf_nvram_ef_abm_gprs_profile_struct  GprsApnProfile;
}MF_GPRS_DATA_PROFILE_STRUCT,*MF_GPRS_DATA_PROFILE_STRUCT_PTR;

typedef void (*mf_PFunction)();

