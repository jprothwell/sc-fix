##############################################################################
##           Copyright (C) 2005, Coolsand Technologies, Inc. 
##                        All Rights Reserved   
##############################################################################

#
# File description[�ļ����ݽ���]
#
1> cfw_sms_mmi.c  : All APIs defined for MMI using
2> cfw_sms_main.c : Main process
3> cfw_sms_tool.c : Tool APIs
4> cfw_sms_prv.h  : Tool APIs define
5> cfw_sms_db.c   : ME database api
6> cfw_sms_db.h   : ME database api define 

#
# Version description[�汾ʵ�ֹ��ܽ���]
#
V1.0.1
1> ���ͽ���һ����ż���������(�ı���PDU).
2> �Զ��Ž���Write/read/list/delete/copy
3> ͳ�ƴ洢���Ĵ洢���
4> ���ܴ洢�µĻ�༭���еĳ�������(δ���)
5> ����ͳ�Ƹ�״̬�Ķ��Ÿ���(δ���)
6> SMS��ʼ����û���Ż�(δ���)

1> cfw_sms_main.c  �Ķ�CFW_DecomposePDU��ʹ�ã� ԭ��:��readĳ��״̬�Ķ���ʱ���ܻ�����
2> cfw_sms_tool.c  �Ķ����ڳ������ŵ�nTP_UDL�ļ���,���ܼ�ȥͷ�ĳ��� 


V1.0.2
1> ͳ�ƴ洢���Ĵ洢���
2> ������SIM��ME�ϴ洢�µĳ�������;�����ܱ༭���еĳ�������
3> ��ͳ�Ƹ�״̬�Ķ��Ÿ���
4> SMS��ʼ���Ż�

1> cfw_sms_main.c  �Ķ�CFW_DecomposePDU��ʹ�ã� ԭ��:��readĳ��״̬�Ķ���ʱ���ܻ�����
2> cfw_sms_tool.c  �Ķ����ڳ������ŵ�nTP_UDL�ļ���,���ܼ�ȥͷ�ĳ��� 


2007.06.13 xueww[+]
1> cfw_sms_tool.c �Ķ�sms_mt_Parse_SmsPPReceiveInd,ʹ�÷��͸�MMI���¼�����pEvent->nParam2�ܹ���ʶ�����ŵ���Ϣ.
2> cfw_sms_main.c �Ķ�sms_ReadAoProc,ʹ�÷��͸�MMI���¼�����pEvent->nParam2�ܹ���ʶ�����ŵ���Ϣ.
