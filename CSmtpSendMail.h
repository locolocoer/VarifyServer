#pragma once  

#include <string>  
#include <vector>  

#define SKIP_PEER_VERIFICATION  
#define SKIP_HOSTNAME_VERIFICATION  


class CSmtpSendMail {
public:
	CSmtpSendMail(const std::string& charset = "UTF-8"); // Ҳ���Դ���utf

	//����stmp�û��������롢���������˿ڣ��˿���ʵ����ָ����libcurlĬ��25���������smtps��Ĭ����465��  
	void SetSmtpServer(const std::string& username, const std::string& password, const std::string& servername, const std::string& port = "25");
	//���������������Բ���  

	void SetSendName(const std::string& sendname);

	//����������   
	void SetSendMail(const std::string& sendmail);

	//����ռ���  
	void AddRecvMail(const std::string& recvmail);

	//��������  
	void SetSubject(const std::string& subject);

	//������������  
	void SetBodyContent(const std::string& content);

	//��Ӹ���  
	void AddAttachment(const std::string& filename);

	//�����ʼ�  
	bool SendMail();
private:

	//�ص���������MIMEЭ���ƴ�ӵ��ַ�����libcurl����  
	static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* stream);

	//�����ʼ�MIME����  
	void CreatMessage();

	//��ȡ�ļ�����  
	int GetFileType(std::string const& stype);

	//�����ļ���  
	void SetFileName(const std::string& FileName);

	//�����ļ���contenttype  
	void SetContentType(std::string const& stype);

	//�õ��ļ���  
	void GetFileName(const std::string& file, std::string& filename);

	//�õ��ļ�����  
	void GetFileType(const std::string& file, std::string& stype);

private:
	std::string m_strCharset; //�ʼ�����  
	std::string m_strSubject; //�ʼ�����  
	std::string m_strContent; //�ʼ�����  
	std::string m_strFileName; //�ļ���  
	std::string m_strMessage;// ����MIMEЭ���ַ���  
	std::string m_strUserName;//�û���  
	std::string m_strPassword;//����  
	std::string m_strServerName;//smtp������  
	std::string m_strPort;//�˿�  
	std::string m_strSendName;//����������  
	std::string m_strSendMail;//����������  
	std::string m_strContentType;//����contenttype  
	std::string m_strFileContent;//��������  

	std::vector<std::string> m_vRecvMail; //�ռ�������  
	std::vector<std::string> m_vAttachMent;//��������  
};



