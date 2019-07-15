#pragma once
#include "RtspClient.h"
#include "SipPacket.h"


typedef enum call_status
{
	INVITE_START,//���ͻ����invite��Ϣ֮ǰ
	INVITE_SEND,//����invite��Ϣ��
	INVITE_RECV,//�յ�invite��Ϣ�����磩
	INVITE_SDP_OK,//�յ����߷���200ok ��Ϣ֮��
	INVITE_ACK_OK,//�յ����߷���ack��Ϣ֮��
	INVITE_CALLING,//ͨ����
	INVITE_DISCONNECTED	//ͨ������
}CALL_STATUS;

typedef struct call_info
{
	CALL_STATUS call_status;
	CString call_id;
	CString call_name;
	CSDP *sdp;
	CNetSocket *udp_audio;
	CNetSocket *udp_video;
	HANDLE send_handle;
	HANDLE recv_handle;
	CRtpPacketCache *rtp_cache;
}CALL_INFO;


typedef void(*incoming_call_back)(CSipPacketInfo *packet_info);

class AFX_EXT_CLASS CSipClient
{
public:
	CSipClient();
	~CSipClient();

	BOOL init(const CString &strServerAddress, unsigned short usServerPort,
		const CString &strLocalAddress, unsigned short usLocalSipPort=0);

	//BOOL init(const CSDP &sdp, const CString &strServerAddress, unsigned short usServerPort,
	//	const CString &strLocalAddress, unsigned short usLocalSipPort = 0);

	BOOL register_account(const CString &strUserName, const CString &strPassword);

	BOOL make_call(const CString &strCallName, BOOL audio_media = TRUE, WORD audio_port = 0,
		BOOL video_media = TRUE, WORD video_port = 0);

	BOOL hangup(const CString &strCallName);

	BOOL call_answer(CSipPacketInfo *packet_info);

	BOOL start_rtp_transport(CSipPacketInfo *packet_info);

	void set_coming_call_function(incoming_call_back function);
	void set_send_cache(CRtpPacketCache *cache);
	void set_recv_cache(CRtpPacketCache *cache);
	void set_local_sdp(const CSDP &sdp);


	//CSDP get_sdp();
	CLIENT_STATUS get_client_status();
	CSDP get_local_sdp();

	void coming_call_back(CSipPacketInfo *packet_info);

protected:

	BOOL send_sip_packet(CSipPacket *packet);
private:
	static DWORD WINAPI ReceiveSipThread(LPVOID lpParam);
	static DWORD WINAPI SipPacketProcessThread(LPVOID lpParam);
	DWORD DoReceiveSip();
	DWORD DoSipPacketProcess();
	void proc_sip_mess(CSipPacket *sipMess);//����sip��Ϣ
	BOOL invite_ok_process(CSipPacketInfo *sipMess);//���� invite ok
	//BOOL start_rtp_transport();


	static DWORD WINAPI send_rtp_thread(LPVOID lpParam);
	static DWORD WINAPI recv_rtp_thread(LPVOID lpParam);
	//static DWORD WINAPI recv_rtp_audio_thread(LPVOID lpParam);

	DWORD do_send_rtp();
	DWORD do_recv_rtp();

private:
	CMutex		m_respond_ArrLock;
	CTypedPtrArray<CPtrArray, CSipPacket*> m_arrRespondPack;//sip��Ӧ��Ϣ����
	CMutex		m_request_info_ArrLock;
	CTypedPtrArray<CPtrArray, CSipPacketInfo*> m_arrRequest_PackInfo;//sip������Ϣ����

	//CSipPacketInfo *m_last_send_packinfo;
	DWORD m_last_send_time;
	CALL_INFO *m_call_info;

	CString m_strContactUser;
	CString m_strUserName;
	CString m_strPassword;
	CString m_strServerIP;
	unsigned short m_usServerPort;
	CString m_strLocalIP;
	unsigned short m_usLocalSipPort;

	int m_nRegisterCSeq;
	int m_nInviteCSeq;
	HANDLE m_hRecvSipThread;//����sip
	HANDLE m_hProcessSipThread;//����sip
	CNetSocket m_udpSipSock;
	CRtpPacketCache *m_send_rtp_cache;
	CSDP *m_local_sdp;
	CLIENT_STATUS m_client_status;
	BOOL m_bwork;
	incoming_call_back m_incoming_call;


	//CRtspClient m_rtsp_client;

	//unsigned short m_usLocalAudioPort;
	//unsigned short m_usLocalVideoPort;
	//CMutex		m_Request_branch_ArrLock;
	//CTypedPtrArray<CPtrArray, CSipPacketInfo*> m_arrRequestPackInfo;//���͵�������ϢժҪ����
	//sip 
	//CString m_strCallName;
	//CString m_str_call_id;
	//CNetSocket m_udpRtpAudio;
	//CNetSocket m_udpRtpVideo;
	//HANDLE m_hRecvRtpThread;//����rtp
	//HANDLE m_hSendRtpThread;//����rtp
	//CSDP m_sdp;
	//BOOL m_bRegisterOK;

};
















/*

//�����룬����try
BOOL BuildTryingMess(SIP_MESSAGE &sipMess);

//sdp�ɹ� ����ok
BOOL BuildOKMess(SIP_MESSAGE &sipMess);

BOOL BuildRegisterMess(SIP_MESSAGE *pSipMess, CString strUserName, CString strPassword,
CString strServerIP, CString strLocalIP, unsigned short usLocalSipPort, int nRegisterCSeq,
CString pViaBranch, CString pContactInstance, CString pFromTag, CString pCallId);

BOOL BuildInviteMess(SIP_MESSAGE  *sipMess, CString strCallName, CString strServerIP,
CString strLocalIP, unsigned short usLocalSipPort, CString strUserName, CString strViaBranch,
CString strFromTag, CString strCallId, int nInvCSeq, SDP_INFO *pSdpInfo);

BOOL BuildAckMess(SIP_MESSAGE *pSipMess, CString strUserName, CString strCallName,
CString strServerIP, CString strLocalIP, unsigned short usLocalSipPort, int nInviteCSeq,
CString strViaBranch, CString strFromTag, CString strCallId);

BOOL BuildByeMess(SIP_MESSAGE *pSipMess, CString strUserName, CString strCallName,
CString strServerIP, CString strLocalIP, unsigned short usLocalSipPort, int nInviteCSeq,
CString strViaBranch, CString strFromTag, CString strCallId);*/


//CMutex		m_CallInfoArrLock;
//CTypedPtrArray<CPtrArray, CALL_INFO*> m_arrCallInfo;//call info ����
//CMutex m_StatusLock;
//bool m_bWork;
//REGISTER_STATUS m_workStatus;//�ͻ���ע��״̬

//typedef enum sip_respond_status
//{
//	unknown_status = -1,
//	trying = 100,
//	ringing = 180,
//	sip_ok = 200,
//	bad_request = 400,
//	request_timeout = 408,
//}SIP_RESPOND_STATUS;

//typedef enum invite_status
//{
//
//}INVITE_STATUS;

//typedef struct sip_sdp_info
//{
//	CString strIP;
//	//��Ƶ
//	BOOL bAudioMedia;
//	unsigned short usAudioPort;
//	CString strAudioIP;
//	int nAudioLoadType;
//	CString strAudioRtpMap;
//	CString strAudioFmtp;
//	//��Ƶ
//	BOOL bVideoMedia;
//	unsigned short usVideoPort;
//	CString strVideoIP;
//	int nVideoLoadType;
//	CString strVideoRtpMap;
//	CString strVideoFmtp;
//}SIP_SDP_INFO;

//typedef struct request_info
//{
//	SIP_METHOD emMethod;
//	CString strViaBranch;
//	CString strCallId;
//	CString strCallName;
//
//
//	//CString strFromTag;
//	//SDP_INFO *sdp;//�Է�
//	//HANDLE hSendMediaThread;
//	//HANDLE hRecvMediaThread;
//	//CString strIP;
//	//unsigned short usSendAudioPort;
//	//unsigned short usSendVideoPort;
//	//unsigned short usRecvAudioPort;
//	//unsigned short usRecvVideoPort;
//	//CNetSocket *udpSockRecvAudio;//������Ƶ
//	//CNetSocket *udpSockRecvVideo;//������Ƶ
//	//CNetSocket *udpSockSendAudio;//������Ƶ
//	//CNetSocket *udpSockSendVideo;//������Ƶ
//	//CString strViaBranch;
//	//CString strContactInfo;
//	//CString strToTag;
//	//CString strRoute;
//	//CString strContactInstance;
//	//CRtpPacketCache *rtpCache;
//}REQUEST_INFO;

//typedef struct CallInfo
//{
//	//
//	BOOL bCalling;
//	CString strCallName;
//	CString strCallId;
//	SDP_INFO *pLocalSdp;
//	SDP_INFO *pSdp;
//	CNetSocket *udpRecvAudio;//������Ƶ
//	CNetSocket *udpRecvVideo;//������Ƶ
//	HANDLE hSendMediaThread;
//	HANDLE hRecvMediaThread;
//	CRtpPacketCache *rtpRecvCache;
//
//
//
//	//CRtpPacketCache *rtpSendCache;
//	//CString strViaBranch;
//	//CString strFromTag;
//	//CString strContactInstance;
//	//CString strContactInfo;
//	//CString strToTag;
//	//CString strRoute;
//	//unsigned short usRecvAudioPort;
//	//unsigned short usRecvVideoPort;
//	//
//	//CString strCallIP;
//	//unsigned short usSendAudioPort;
//	//unsigned short usSendVideoPort;
//	//unsigned short usRecvAudioPort;
//	//unsigned short usRecvVideoPort;
//	//CNetSocket *udpSendAudio;//������Ƶ
//	//CNetSocket *udpSendVideo;//������Ƶ
//
//
//}CALL_INFO;

/*
typedef enum message_type
{
sip_request,
sip_status
}MESSAGE_TYPE;

typedef enum request_method
{
other_method = -1,
SipRegister,
SipInvite,
SipAck,
SipBye,
}REQUEST_METHOD;

typedef enum status_code
{
other_status = -1,
trying = 100,
ringing = 180,
ok = 200
}STATUS_CODE;

typedef enum client_status
{
uninitialized,
init_ok,
register_ok,
inviteing,
calling,
}CLIENT_STATUS;

//typedef enum sip_head_key
//{
//	via,
//	max_forward,
//	to,
//	from,
//	contact,
//	cseq,
//	call_id
//}SIP_HEAD_KEY;

//typedef enum call_status
//{
//	INVITE_START,//���ͻ����invite��Ϣ֮ǰ
//	INVITE_SEND,//����invite��Ϣ��
//	INVITE_RECV,//�յ�invite��Ϣ�����磩
//	INVITE_SDP_OK,//�յ����߷���200ok ��Ϣ֮��
//	INVITE_ACK_OK,//�յ����߷���ack��Ϣ֮��
//	INVITE_CALLING,//ͨ����
//	INVITE_DISCONNECTED	//ͨ������
//}CALL_STATUS;

//typedef struct call_info
//{
//	CALL_STATUS call_status;
//	CString call_name;
//	CString call_id;
//	unsigned short local_audio_port;
//	unsigned short local_video_port;
//	SDP_INFO *sdp_info;
//	CNetSocket audio_socket;
//	CNetSocket video_socket;
//	HANDLE RecvRtpThread;//����rtp
//	HANDLE SendRtpThread;//����rtp
//	CRtpPacketCache *rtp_cache;
//
//}CALL_INFO;

typedef struct sip_uri
{
CString user;
CString host;
unsigned short port;
}SIP_URI;

typedef struct request_parameter
{
REQUEST_METHOD method;
SIP_URI request_uri;
}REQUEST_PARAMETER;

//typedef struct status_line
//{
//	RESPOND_STATUS status_code;
//}STATUS_LINE;

typedef struct sip_via
{
CString sent_address;
unsigned short sent_port;
CString received_address;
unsigned short recvived_port;
CString branch;

}SIP_VIA;

//typedef struct sip_max_forwards
//{
//	int forwards;
//}SIP_MAX_FORWARDS;

typedef struct sip_from
{
CString display_info;
CString from_user;
CString from_host;
CString from_tag;
}SIP_FROM;

typedef struct sip_to
{
CString display_info;
CString to_user;
CString to_host;
CString to_tag;
}SIP_TO;

typedef struct sip_contact
{
SIP_URI contact_uri;
CString parameter;
}SIP_CONTACT;

typedef struct sip_cseq
{
int cseq;
REQUEST_METHOD method;
}SIP_CSEQ;

class AFX_EXT_CLASS CSipPacket
{
public:
CSipPacket();
CSipPacket(unsigned int packet_length);
~CSipPacket();

BOOL FromBuffer(unsigned char *data, int data_len);

void build_register_pack(CString request_line, CString via, CString max_forwards, CString from,
CString to, CString contact, CString call_id, CString CSeq);

void build_invite_pack(CString request_line, CString via, CString max_forwards, CString from,
CString to, CString contact, CString call_id, CString CSeq, CString sdp);

void build_ack_pack(CString request_line, CString via, CString max_forwards, CString from,
CString to, CString call_id, CString CSeq);

void build_ok_pack(CString status_line, CString via, CString max_forwards, CString from,
CString to, CString call_id, CString CSeq, CString sdp_buf);


unsigned char* get_data();
int get_data_len();

//BOOL set_cseq(int cseq);
//BOOL build_register_pack(CString str_username, CString str_password,
//	CString server_addr, unsigned short server_port,
//	CString sent_addr, unsigned short sent_port);
//BOOL build_register_pack(REQUEST_LINE request_line, SIP_VIA *via, int via_num,
//	int max_forwards, SIP_FROM from, SIP_TO to, SIP_CONTACT *contact, int contact_num,
//	CString call_id, SIP_CSEQ CSeq);


public:
//BOOL build_packet(CString *str_line, int num);

CString generate_request_line(REQUEST_PARAMETER request);

//message line
//BOOL generate_request_line(CString & request_line, REQUEST_METHOD method, const CString &user,
//const CString &address,unsigned short port, const CString &parameter);
BOOL generate_respond_status_line(CString &status_line, RESPOND_STATUS status);
//message header
CString generate_via(const CString &sent_address, unsigned short port,
const CString &branch, const CString &rport, const CString &received);
CString generate_max_forwards(int max_f);
CString generate_from(const CString &display_name, const CString &sip_user,
const CString &sip_address, const CString &tag);
CString generate_to(const CString & display_name, const CString &to_name,
const CString &to_address, const CString &tag);
CString generate_call_id(const CString &call_id);
CString generate_cseq(int cseq, REQUEST_METHOD method);
CString generate_contact(const CString &name, const CString &address, unsigned short port,
const CString &parameter);

private:
unsigned char *m_data;
unsigned int m_data_len;
};

class AFX_EXT_CLASS CSipPacketInfo
{
public:
CSipPacketInfo();
~CSipPacketInfo();
BOOL from_packet(CSipPacket * packet);

MESSAGE_TYPE get_type();
REQUEST_PARAMETER get_request_line();
STATUS_CODE get_status_line();

CString get_call_id();
SIP_FROM get_from();
SIP_TO get_to();
SIP_CSEQ get_cseq();
int get_max_forwards();
int get_via_array_length();
SIP_VIA get_via(int index);
int get_contact_array_length();
SIP_CONTACT get_contact(int index);
CString get_content_type();
int get_content_length();

CSDP get_sdp_info();


//BOOL from_packet(CSipPacket *packet);
//REQUEST_METHOD get_method();
//RESPOND_STATUS get_status();
//CString get_via_branch();
//CString get_from_tag();
//CString get_to_tag();
//CString get_call_id();
//CString get_contact_rinstance();
//CString get_contact_user();
//CString get_contact_address();
//int get_contact_port();
//int get_cseq();

private:
MESSAGE_TYPE m_type;
REQUEST_PARAMETER m_request_parameter;
STATUS_CODE m_status_code;

SIP_FROM m_from;
SIP_TO m_to;
SIP_CSEQ m_cseq;
int m_max_forwards;
CString m_call_id;
CArray<SIP_VIA, SIP_VIA&> m_array_via;
CArray<SIP_CONTACT, SIP_CONTACT&> m_array_contact;
CString  m_content_type;
int    m_content_length;

CSDP m_sdp;



/*
MESSAGE_TYPE m_type;

CString m_status_line;
RESPOND_STATUS m_status;

CString m_request_line;
REQUEST_METHOD m_method;

CString m_via;
CString m_via_branch;

CString m_max_forwards;

CString m_from;
CString m_from_name;
CString m_from_tag;

CString m_to;
CString m_to_tag;

CString m_call_id;
CString m_call_id_value;

CString m_contact;
CString m_contact_user;
CString m_contact_address;
unsigned short  m_contact_port;
CString m_contact_rinstance;

CString m_cseq;
int m_cseq_value;

CSDP m_sdp_info;
}

//typedef enum sip_head_key
//{
//	via,
//	max_forward,
//	to,
//	from,
//	contact,
//	cseq,
//	call_id
//}SIP_HEAD_KEY;

//typedef enum call_status
//{
//	INVITE_START,//���ͻ����invite��Ϣ֮ǰ
//	INVITE_SEND,//����invite��Ϣ��
//	INVITE_RECV,//�յ�invite��Ϣ�����磩
//	INVITE_SDP_OK,//�յ����߷���200ok ��Ϣ֮��
//	INVITE_ACK_OK,//�յ����߷���ack��Ϣ֮��
//	INVITE_CALLING,//ͨ����
//	INVITE_DISCONNECTED	//ͨ������
//}CALL_STATUS;

//typedef struct call_info
//{
//	CALL_STATUS call_status;
//	CString call_name;
//	CString call_id;
//	unsigned short local_audio_port;
//	unsigned short local_video_port;
//	SDP_INFO *sdp_info;
//	CNetSocket audio_socket;
//	CNetSocket video_socket;
//	HANDLE RecvRtpThread;//����rtp
//	HANDLE SendRtpThread;//����rtp
//	CRtpPacketCache *rtp_cache;
//
//}CALL_INFO;

//typedef struct status_line
//{
//	RESPOND_STATUS status_code;
//}STATUS_LINE;
//typedef struct sip_max_forwards
//{
//	int forwards;
//}SIP_MAX_FORWARDS;

*/



