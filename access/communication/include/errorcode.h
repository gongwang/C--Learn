#ifndef _KDI_SDK_ERRORCODE_H_
#define _KDI_SDK_ERRORCODE_H_
enum ErrorCode{
	//ERROR_EM_BEGIN = 80000,
	//ERROR_EM_END = 89999,

	EVT_EMI_SUCCESS=70000,

	//ʧ��
	EV_EMI_ERROR_DEV_ADD_EXIST_ERROR = 80000,			//����Ѿ����ڵ��豸
	EV_EMI_ERROR_DEV_DEL_NOTEXIST_ERROR,				//ɾ�������ڵ��豸	
	EV_EMI_ERROR_DEV_UPDATE_NOTEXIST_ERROR,				//���Ĳ����ڵ��豸
	EV_EMI_ERROR_DEV_CONTROL_NOTEXIST_ERROR,			//���Ʋ����ڵ��豸
	EV_EMI_ERROR_DEV_DLLWRAPPER_NOTEXIST_ERROR,         //�豸��̬���������
	EV_EMI_ERROR_DEV_DLLWRAPPER_NULL_ERROR,             //�豸��̬�����ָ��Ϊ��
	EV_EMI_ERROR_DEV_HANDLE_NULL_ERROR,                 //�豸���Ϊ��


	EV_EMI_ERROR_COM_ISUSE = 81000,							//���ڱ�ռ��
	EV_EMI_ERROR_COM_NOT_PRESENT,							//���ڲ�����
	EV_EMI_ERROR_COM_MODULE_NOT_INIT,						//����ģ��δ��ʼ��
	EV_EMI_ERROR_COM_MODULE_OPEN_ERROR,						//����ģ���ʧ��
	EV_EMI_ERROR_COM_MODULE_INIT_ERROR,						//����ģ���ʼ��ʧ��
	EV_EMI_ERROR_COM_MODULE_RELEASE_ERROR,					//����ģ���ͷ�ʧ��
	EV_EMI_ERROR_COM_SEND_ERROR,							//����COM����ʧ��
	EV_EMI_ERROR_COM_OVERFLOW,								//�ﵽCOM��������
	EV_EMI_ERROR_COM_CONFIG_ERROR,							//���ڲ������ô���

	EV_EMI_ERROR_TCP_MODULE_NOT_INIT = 81010,			//TCPģ��δ��ʼ��
	EV_EMI_ERROR_TCP_MODULE_INIT_ERROR,					//TCPģ���ʼ��ʧ��
	EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR,					//TCPģ���ʧ��
	EV_EMI_ERROR_TCP_MODULE_RELEASE_ERROR,				//TCPģ���ͷ�ʧ�� 
	EV_EMI_ERROR_TCP_SEND_ERROR,						//����TCP����ʧ��
	EV_EMI_ERROR_TCP_OVERFLOW,							//�ﵽTCP��������

	EV_EMI_ERROR_UDP_MODULE_NOT_INIT = 81020,			//UDPģ��δ��ʼ��
	EV_EMI_ERROR_UDP_MODULE_INIT_ERROR,					//UDPģ���ʼ��ʧ��
	EV_EMI_ERROR_UDP_MODULE_OPEN_ERROR,					//UDPģ���ʧ��
	EV_EMI_ERROR_UDP_MODULE_RELEASE_ERROR,				//UDPģ���ͷ�ʧ��
	EV_EMI_ERROR_UDP_SEND_ERROR,						//����UDP����ʧ��
	EV_EMI_ERROR_UDP_OVERFLOW,							//�ﵽUDP��������
	EV_EMI_ERROR_UDP_SESSION_UNAVALIBLE,                // Udp����ʧ��

	EV_EMI_ERROR_TCP_SERVER_MODULE_NOT_INIT = 81030,			//TCP ����ģ��δ��ʼ��
	EV_EMI_ERROR_TCP_SERVER_MODULE_INIT_ERROR,					//TCP ����ģ���ʼ��ʧ��
	EV_EMI_ERROR_TCP_SERVER_MODULE_OPEN_ERROR,					//TCP ����ģ���ʧ��
	EV_EMI_ERROR_TCP_SERVER_MODULE_RELEASE_ERROR,				//TCP ����ģ���ͷ�ʧ��

	EV_EMI_ERROR_DEV_NOT_INIT= 81100,					//�豸δ��ʼ��
	EV_EMI_ERROR_DEV_INIT_ERROR= 81101,					//�豸��ʼ��ʧ��
	EV_EMI_ERROR_DEV_CFG_ERROR,							//�豸���ô���
	EV_EMI_ERROR_DEV_CONFIG_OVERFLOW,					//�ﵽ�豸��������	
	EV_EMI_ERROR_DEV_OFFLINE,							//�豸������
	EV_EMI_ERROR_DEV_NOT_FIND,							//�豸������
	EV_EMI_ERROR_DEV_NOT_SUPORT,						//��֧�ֵ��豸
	EV_EMI_ERROR_NOT_SUPORT_PROTOCOL,					//��֧�ֵ�Э������
	EV_EMI_ERROR_NOT_SUPORT_CONNECT,					//��֧�ֵ����ӷ�ʽ
	EV_EMI_ERROR_DEV_CNT_ADDRESS_EXIST,					//ͬһ���ӷ�ʽ����ͬ��������ַ���豸�Ѿ�����
	EV_EMI_ERROR_DEV_CONNECT_FAIL,                      // ����ʧ��
	EV_EMI_ERROR_DEV_CLOSE_FAIL,						//�ر�����ʧ�� 
	EM_EMI_ERROR_START_TASK_FAIL,						//��������ʧ��
	EM_EMI_ERROR_DLL_LOAD_FAIL,							//���ض�̬��ʧ��

	EV_EMI_ERROR_DEV_CONTROL,							//�����豸ʧ��
	EV_EMI_ERROR_CONTROL_NOT_SUPORT,					//��֧�ֵĲ���
	EV_EMI_ERROR_CONTROL_DEV_FAILED,					//�����豸ʧ��

	EV_EMI_ERROR_DEV_NO_CFG_ERROR = 81200,				//�����豸�Ŵ���

	EV_EMI_ERROR_DSS_NOT_USE = 82000,					//dss δ����
	EV_EMI_ERROR_DSS_NOT_CONNECTED,						//dss δ����

	EV_EMI_ERROR_MEMORY_NEW,							//�ڴ��������
	EV_EMI_ERROR_DB_OPER,								//�������ݿ�ʧ��
	EV_EMI_ERROR_INVALID_PARAM,							//��Ч�Ĳ���

	EV_EMI_ERROR_PUINT_HOST_NOT_REG,					//���ܷ�����δע��
	EV_EMI_ERROR_PUINT_HOST_CONNECT_FAILED,				//���ܷ���������ʧ��
	EV_EMI_ERROR_PUINT_HOST_GET_POINT_LIST,				//��ȡ���ܷ�����б�ʧ��
	EV_EMI_ERROR_PUINT_NOT_EXIST,						//��ƽ̨���������ܵ�

	EV_EMI_ERROR_BC_FILE_NOT_EXIST,						//�����ڹ㲥�ļ�
	EV_EMI_ERROR_BC_POINT_NOT_EXIST,					//�����ڹ㲥��


	EV_EMI_ERROR_CONFIG_TIMEOUT = 83008,						  //�豸���ó�ʱ
	EV_EMI_ERROR_SMS_RUN_ERROR = 83009,							//SMS����ʧ��
	EV_EMI_ERROR_SMS_CONTROL_SEND_ERROR,                //SMS���ʹ���
	EV_EMI_ERROR_SMS_REGESTER_ERROR,                    //SMSע��ʧ��
	EV_EMI_ERROR_SMS_DLL_NOTLOAD,                       //SMS DLLδ�ҵ�
	EV_EMI_ERROR_SMS_CONTROL_SMS_INIT,                  // ����è����ʧ��
	EV_EMI_ERROR_SMS_CONTROL_WRITE_COM,                 // д����ʧ��				
	EV_EMI_ERROR_SMS_CONTROL_READ_COM,                  // ������ʧ��			
	EV_EMI_ERROR_SMS_CONTROL_CLEAR_COMM,                // �崮��ʧ��					
	EV_EMI_ERROR_SMS_CONTROL_NO_VALID_GPRS_MODEM,       // ���GSM MODEM�豸ʧ��		
	EV_EMI_ERROR_SMS_CONTROL_NO_SIM_CARD,               // ���SIM��ʧ��				
	EV_EMI_ERROR_SMS_CONTROL_NO_SINGAL,                 // ���SIM�ź�ʧ��	80040
	EV_EMI_ERROR_SMS_SNED_SMS_FAIL,						          // ���Ͷ���ʧ��
	EV_EMI_ERROR_SMS_DLL_NOT_FIND,						          //SMS DLL�Ҳ���
	EV_EMI_ERROR_SMS_DLL_INIT,							            //SMS DLL��ʼ��ʧ��
	EV_EMI_ERROR_SMS_DLL_RELEASE,						            //SMS DLL�ͷ�ʧ��
	EV_EMI_ERROR_SMS_DLL_REGESTER,						          //SMS ע��ص�ʧ��
	EV_EMI_ERROR_SMS_CLOSE,								              //SMS �ر�ʧ��
	EV_EMI_ERROR_SMS_NOT_INIT,							            //SMSδ��ʼ��

	EV_EMI_ERROR_DEV_PROTOCOL_NOT_INIT,					//�豸ģ��δ��ʼ�� 
	EV_EMI_ERROR_DEV_PROTOCOL_INIT,						//��ʼ���豸ģ��ʧ��UDP80130 
	EV_EMI_ERROR_DEV_PROTOCOL_RELEASE,					//�ͷ��豸ģ��ʧ��
	EV_EMI_ERROR_DEV_PROTOCOL_NOT_OPEN,					//�豸ģ��δ��
	EV_EMI_ERROR_DEV_PROTOCOL_CONNECT,					//�����豸ģ��ʧ��
	EV_EMI_ERROR_DEV_PROTOCOL_DISCONNECT,				//�Ͽ��豸ģ��ʧ��
	EV_EMI_ERROR_DEV_PROTOCOL_START_DATA_DETECT,		//��ʼ�豸ģ�����ݼ��ʧ��
	EV_EMI_ERROR_DEV_PROTOCOL_STOP_DATA_DETECT = 83034,			//ֹͣ�豸ģ�����ݼ��ʧ��

	EV_EMI_ERROR_SPEAK_SERIAL_NOT_INIT = 84000,					//�Խ����к�û������
	EV_EMI_ERROR_WEIGENG_DOOR_NOT_REGISTER = 84001,				//΢���Ž��ؼ�δע��
	EV_EMI_ERROR_BOSCHHK_ALARM_CREATE_LISTENPORT_ERROR = 84002, //�����澯��������ģ�鴴�������˿�ʧ��

	EV_EMI_ERROR_UNKNOWN = 89999,						//δ֪����
};
#endif