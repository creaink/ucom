#include "stdafx.h"
#include "iSocket.h"

// :CAsyncSocket()
iSocket::iSocket()
{
	AfxSocketInit();
	// ��ʼΪ1�����������ٶ�̬����
	nClient.SetSize(0, 1);
}


iSocket::~iSocket()
{
}

// �ӿͻ������б������Ƴ�
void iSocket::DelFromClient(nSocket *mclient)
{
	int size = nClient.GetSize();
	for (int i = 0; i < size; i++) {
		if (nClient[i] == mclient) {
			delete nClient[i];
			nClient.RemoveAt(i);
			// ���޿ͻ��˹رտɶ�д����
			if (size == 1) {
				isSocketOpen = false;
			}
			return;
		}
	}
}

// �ӿͻ������б������Ƴ�
void iSocket::DelAllClient(void)
{
	int size = nClient.GetSize();
	// �رտɶ�д����
	isSocketOpen = false;
	for (int i = 0; i < size; i++)
	{
		nClient[i]->Close();
		delete nClient[i];
	}

	nClient.RemoveAll();
}

void iSocket::OnReceive(int nErrorCode)
{
	::PostMessage(hwTopParent, WM_MYONRECVMSG, W_NET_RECV, 0);
	CAsyncSocket::OnReceive(nErrorCode);
}


// Serverģʽ�¿ͻ�������
void iSocket::OnAccept(int nErrorCode)
{
	TRACE("On Accept:%d\n", nErrorCode);
	if (!nErrorCode) {
		// ֻҪ�о�����Ϊ����
		isSocketOpen = true;

		//����һ���µĿ��׽���
		nSocket *sockRecv = new nSocket(hwParent, hwTopParent);
		sockRecv->SetSocketHandle(sockRecv);
		//�������ӣ���ʧ��ɾ��new���׽���
		if (0 == Accept(*sockRecv))
		{
			delete sockRecv;
		}
		else
		{
			::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_SUBNET | WL_UCOM_OPEN, 0);
		}
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void iSocket::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		PostMsgIsOpen(true);
	}
	else
	{
		CString errorStr;
		// ���͹ر���Ϣ�����ر�
		PostMsgIsOpen(false);
		Close();
		switch (nErrorCode)
		{
		case WSAEADDRINUSE:
			errorStr = _T("��ַ��ռ��");break;
		case WSAEADDRNOTAVAIL:
			errorStr = _T("��ַ����������");break;
		case WSAECONNREFUSED:
			errorStr = _T("���ӱ��ܾ�");break;
		case WSAEDESTADDRREQ:
			errorStr = _T("δ�����ַ");break;
		case WSAEFAULT:
			errorStr = _T("��Ч��ַ");break;
		case WSAEINVAL:
			errorStr = _T("��Ч����");break;
		case WSAEISCONN:
			errorStr = _T("���ӱ�ռ��");break;
		case WSAENETUNREACH:
			errorStr = _T("��ַ���ɼ�"); break;
		case WSAENOBUFS:
			errorStr = _T("�׽����ڴ治��"); break;
		case WSAENOTCONN:
			errorStr = _T("δ��������\n");break;
		case WSAETIMEDOUT:
			errorStr = _T("�������ӳ�ʱ"); break;
		default:
			errorStr.Format("������-%d", nErrorCode);
			break;
		}
		AfxMessageBox(("����ʧ��:"+errorStr));
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void iSocket::OnClose(int nErrorCode)
{
	PostMsgIsOpen(false);
}

void iSocket::PostMsgIsOpen(bool isOpen)
{
	isSocketOpen = isOpen;
	if (isOpen)
	{
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_OPEN, 0);
		::SendMessage(hwParent, WM_NETDLG_MSG, WL_UCOM_OPEN, 0);
	}
	else
	{
		::PostMessage(hwTopParent, WM_MYONRECVMSG, WH_UCOM_NET | WL_UCOM_CLOSE, 0);
		::SendMessage(hwParent, WM_NETDLG_MSG, WL_UCOM_CLOSE, 0);
	}
}

int iSocket::UnblockRead(CString & dataStr)
{
	TCHAR buff[ISOCKET_READBUFF_MAX];
	int nRead;
	nRead = Receive(buff, ISOCKET_READBUFF_MAX);
	switch (nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Socket��ȡ����!"));
			Close();
		}
		break;
	default:
		dataStr.GetBufferSetLength(nRead);
		memcpy(dataStr.GetBuffer(0), buff, nRead);
	}
	return nRead;
}


int iSocket::UnblockRead(CString & dataStr, CString & infoStr)
{
	TCHAR buff[ISOCKET_READBUFF_MAX];
	int nRead;
	UINT nPort;
	nRead = ReceiveFrom(buff, ISOCKET_READBUFF_MAX, infoStr, nPort);
	infoStr.Format("\n��From: %s:%d��\n", infoStr, nPort);
	switch (nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Socket��ȡ����!"));
			Close();
		}
		break;
	default:
		dataStr.GetBufferSetLength(nRead);
		memcpy(dataStr.GetBuffer(0), buff, nRead);
	}
	return nRead;
}


/* nSocket */

nSocket::~nSocket()
{
}

void nSocket::OnReceive(int nErrorCode)
{
	// ��ʵ�������
	::PostMessage(hwTopParent, WM_MYONRECVMSG, W_SUBNET_RECV, (LPARAM)&mPara);
}

void nSocket::OnSend(int nErrorCode)
{
	TRACE("Frist Sub Send\n");
	//��һ������ʱ�򶼻���һ��send��������ʱ����ip�Ͷ˿ڣ��ر�send��Ϣ����
	//��Ȩ�ҵ����ӳɹ�����Ϣ����
	GetClientInfo();
	AsyncSelect(FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT);
	PostMsgIsOpen(true);
}


void nSocket::OnConnect(int nErrorCode)
{
	PostMsgIsOpen(!nErrorCode);
}

void nSocket::OnClose(int nErrorCode)
{
	PostMsgIsOpen(false);
}

void nSocket::GetClientInfo(void)
{
	UINT port;
	GetPeerName(mPara.strIP, port);
	mPara.strIP.Format("%s:%d", mPara.strIP, port);
}

void nSocket::PostMsgIsOpen(bool isOpen)
{
	::SendMessage(hwParent, WM_NETDLG_MSG, isOpen ? W_SUBNET_OPEN : W_SUBNET_CLOSE, (LPARAM)&mPara);
}

int nSocket::UnblockRead(CString & dataStr)
{
	TCHAR buff[ISOCKET_READBUFF_MAX];
	int nRead;
	nRead = Receive(buff, ISOCKET_READBUFF_MAX);
	switch (nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Socket��ȡ����!"));
			Close();
		}
		break;
	default:
		dataStr.GetBufferSetLength(nRead);
		memcpy(dataStr.GetBuffer(0), buff, nRead);
	}
	return nRead;
}

