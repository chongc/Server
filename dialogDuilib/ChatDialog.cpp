#include "stdafx.h"
#include "ChatDialog.h"
#include <process.h>
#include "Command.h"
extern ClientConnector clientCon;
extern ChatDialog* pChatDialog;


//HANDLE bufferMutex;
//CRITICAL_SECTION cs1;

ChatDialog::ChatDialog(const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info)
:myselft_(myselft_info)
, friend_(friend_info)
{

}


ChatDialog::~ChatDialog()
{
}

LPCTSTR ChatDialog::GetWindowClassName() const
{
	return _T("ChatDialog");
}
CDuiString ChatDialog::GetSkinFile(){
	return _T("chatbox.xml");
}
CDuiString ChatDialog::GetSkinFolder()
{
	return  _T("skin\\");
}



void ChatDialog::InitWindow()
{
	CDuiString ToID =friend_.GuanXinID;
	CDuiStringtoMyString(ToID, To);
	pEidt = (CRichEditUI*)m_PaintManager.FindControl(_T("richedit1"));
	pEidt->SetFocus();
	pLabel = (CLabelUI*)m_PaintManager.FindControl(_T("titlename"));
	pLabel->SetText(friend_.Name);
	pList = (CMyList*)m_PaintManager.FindControl(_T("list"));
	pEmojiList = (CTileLayoutUI*)m_PaintManager.FindControl(_T("emojilist"));
	if (pEmojiList){
		TCHAR szPath[MAX_PATH] = { 0 };
		
		for (int i = 0; i < 120; i++)
		{
			CButtonUI *pBtn = new CButtonUI;
			_stprintf(szPath, _T("file='Face\\%d.gif' "), i);
			pBtn->SetBkImage(szPath);
		
			pBtn->SetFixedHeight(24);
			pBtn->SetFixedWidth(24);
			pBtn->SetTag(i);
			pEmojiList->Add(pBtn);
		
		}
	}
	receiveThread = (HANDLE)_beginthreadex(NULL, 0, &ReceiveMessageThread, (LPVOID)pList, 0, NULL);


}



void ChatDialog::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString name = msg.pSender->GetName();
		
		if (name == _T("sendfile") ||name== (_T("sendmsg"))){
				
			if (name == _T("sendfile")){
				
				msgType = PACKET_C2S_FILE;
				
				
			}
			else if (name == (_T("sendmsg")))
			{
				msgType = PACKET_C2S_TEXTMESSAGE;
			}
		
			sendThread = (HANDLE)_beginthreadex(NULL, 0, &SendMessageThread, (LPVOID)msgType, 0, NULL);
			
		}
		else if (name == _T("logout")){
			CloseConnect();

		}
		else if (name == _T("emojictrl")){
			if (pEmojiList->IsVisible() == FALSE)
				pEmojiList->SetVisible(TRUE);
			else{
				pEmojiList->SetVisible(FALSE);
			}
		}
		else if (pEmojiList->IsVisible() == TRUE){
			int iIndex = msg.pSender->GetTag();
			InsertGif(pEidt, iIndex);
			pEidt->SetFocus();
			pEmojiList->SetVisible(FALSE);
		}
		if (name == _T("closebtn")){
			
		DWORD dwExitCode;
		GetExitCodeThread(receiveThread, &dwExitCode);
		TerminateThread(receiveThread, dwExitCode);
		GetExitCodeThread(sendThread, &dwExitCode);
		TerminateThread(sendThread, dwExitCode);
		}
	}

	__super::Notify(msg);
}

void ChatDialog::OnFinalMessage(HWND hWnd)
{
	delete this;
}

CControlUI* ChatDialog::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("MyList")) == 0)
	{
		return new CMyList(m_PaintManager);
	}
	
	return NULL;
}

void ChatDialog::SendMessagetoAnother(CDuiString text)
{
	
	pList->AddText(TRUE, text);


}



unsigned __stdcall ReceiveMessageThread(LPVOID IpParameter)
{
	while (true)
	{
		Packet* p = clientCon.ReceivePacket();
		if (p == NULL){
			CloseConnect();
			delete p;
			break;
		}
		if (p->Type == PACKET_C2S_QUERYRECEIVEFILERESULT){
			MessageBox(pChatDialog->GetHWND(), L"对方拒绝接收文件", L"文件传输提示", MB_OK);
			delete p;
			continue;
		}
		Command* c = Command::SetCommand(p->Type);
		c->Recv(p, &clientCon);
		delete p;
		delete c;
	}
	
	/*while (true){
		int Type = 0;
		Type = (int)*client.GetData(1);
		if (Type == PACKET_S2C_FILE){
			int len = (int)*client.GetData(8);
			char* recvBuffer = client.GetData(len);
			Pack_S2C_File pSFile;
			pSFile.DeCode(recvBuffer);
			pSFile.SetFilePath(pSFile.fileName);
			FILE* fp = fopen(pSFile.fileName, "wb");
			
			CDuiString  cue = _T("正在接受文件，将保存至 ");
			USES_CONVERSION;
			LPCTSTR path = A2CW(pSFile.fileName);
			cue.Append(path);
			CMyList* MyList = (CMyList*)(LPVOID)IpParameter;
			MyList->AddText(TRUE, cue);
			UINT64 size = pSFile.fileLength;
		
			const int BUFFER_SIZE = 1024;
			int length;
			if (size > BUFFER_SIZE){
				length = BUFFER_SIZE;
			}
			else{
				length = size + 1;
			}
			char* revBuf = (char*)malloc(sizeof(char)*length);
			memset(revBuf, 0, length);
			long long ct = size;
		
			while (ct > 0){
				revBuf = client.GetData(length);
				int write_length = fwrite(revBuf, sizeof(char), strlen(revBuf), fp);
				if (write_length < len)break;
				memset(revBuf, 0, length);
				ct -= len;
			}
			fclose(fp);
			free(revBuf);
			char type = (int)*client.GetData(1);
			if (type == PACKET_S2C_FILEFINISH){
				Pack_S2C_Filefinish pff;
				int len = (int)*client.GetData(2);
				char* recvBuffer = client.GetData(len);
				pff.DeCode(recvBuffer);
				if (pff.fileFinish == 1){
					MyList->AddText(TRUE, _T("文件已接受完成！"));
				}
			}
		}
		if (Type == PACKET_S2C_TEXTMESSAGE){
			UINT16 len = (int)*client.GetData(2);
			char* recvBuffer = client.GetData(len);
			Pack_S2C_TextMessage pTextM;
			pTextM.DeCode(recvBuffer);
			
			USES_CONVERSION;
			LPCTSTR text = A2CW(pTextM.Text);
			pChatDialog->SendMessagetoAnother(text);
			
		}
	}*/
	return 0;
}

unsigned __stdcall  SendMessageThread(LPVOID pvParam)
{
	
	char type = (char)(LPVOID)pvParam;
	Command* cmd = Command::SetCommand(type);
	cmd->Send(&clientCon);
	delete cmd;
	
	return 0;
}










void ChatDialog::InsertGif(CRichEditUI* pRichEdit, int index)
{
	TCHAR szPath[MAX_PATH] = { 0 };
	_stprintf(szPath, _T("..\\debug\\skin\\Face\\%d.gif"), index);
	_bstr_t bstrFileName1 = szPath;


	InsertAniSmiley(pRichEdit, (long)GetHWND(), bstrFileName1, ::GetSysColor(COLOR_WINDOW), 0);
	emojiIndex.push_back(index);

}
bool ChatDialog::InsertAniSmiley(CRichEditUI* pRichEdit, long hwndHostWindow, BSTR bstrFileName, OLE_COLOR clr, INT cy)
{
	GifSmiley::IGifSmileyCtrl* lpAnimator;
	::CoCreateInstance(GifSmiley::CLSID_CGifSmileyCtrl, NULL, CLSCTX_INPROC, GifSmiley::IID_IGifSmileyCtrl, (LPVOID*)&lpAnimator);

	if (lpAnimator == NULL)
	{
		return false;
	}

	COLORREF backColor = (COLORREF)(clr);
	HWND hwnd = (HWND)(hwndHostWindow);
	HRESULT hr;


	IRichEditOle *pRichEditOle = pRichEdit->GetRichEditOle();
	if (NULL == pRichEditOle)
	{
		return false;
	}


	/*
	ATL::CString strPicPath(bstrFileName);
	ATL::CString strExt=strPicPath.Right(4);
	if (strExt.CompareNoCase(_T(".gif")) && strExt.CompareNoCase(_T(".jpg")) && strExt.CompareNoCase(_T(".png")) && strExt.CompareNoCase(_T(".bmp")))
	return FALSE;

	*/

	LPSTORAGE lpStorage = NULL;
	LPOLEOBJECT	lpObject = NULL;
	LPLOCKBYTES lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite = NULL;
	BSTR path = NULL;

	//Create lockbytes
	hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (FAILED(hr))
	{
		return	 false;
	}
	//use lockbytes to create storage
	SCODE sc = ::StgCreateDocfileOnILockBytes(lpLockBytes, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		lpLockBytes->Release();
		return false;
	}

	// retrieve OLE interface for richedit   and  Get site
	pRichEditOle->GetClientSite(&lpClientSite);

	try
	{
		//COM operation need BSTR, so get a BSTR
		path = bstrFileName;

		//Load the gif
		lpAnimator->LoadFromFileSized(path, cy);

		//Set back color
		OLE_COLOR oleBackColor = (OLE_COLOR)backColor;
		lpAnimator->put_BackColor(oleBackColor);


		//get the IOleObject
		hr = lpAnimator->QueryInterface(IID_IOleObject, (void**)&lpObject);
		if (FAILED(hr))
		{
			return	 false;
		}

		//Set it to be inserted
		OleSetContainedObject(lpObject, TRUE);

		//to insert into richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);

		CLSID clsid;
		hr = lpObject->GetUserClassID(&clsid);


		//set clsid
		reobject.clsid = clsid;
		//can be selected
		reobject.cp = REO_CP_SELECTION;
		//content, but not static
		reobject.dvaspect = DVASPECT_CONTENT;
		//goes in the same line of text line
		reobject.dwFlags = REO_BELOWBASELINE;
		//reobject.dwUser = (DWORD)myObject;

		//the very object
		reobject.poleobj = lpObject;
		//client site contain the object
		reobject.polesite = lpClientSite;
		//the storage 
		reobject.pstg = lpStorage;

		SIZEL sizel = { 0 };
		reobject.sizel = sizel;


		LPOLECLIENTSITE lpObjectClientSite = NULL;
		hr = lpObject->GetClientSite(&lpObjectClientSite);
		if (FAILED(hr) || lpObjectClientSite == NULL)
			lpObject->SetClientSite(lpClientSite);

		pRichEditOle->InsertObject(&reobject);
		//redraw the window to show animation
		::RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);

		if (lpClientSite)
		{
			lpClientSite->Release();
			lpClientSite = NULL;
		}
		if (lpObject)
		{
			lpObject->Release();
			lpObject = NULL;
		}
		if (lpStorage)
		{
			lpStorage->Release();
			lpStorage = NULL;
		}

		if (lpAnimator)
		{
			lpAnimator->Release();
			lpAnimator = NULL;
		}

	}
	catch (...)
	{
		if (lpClientSite)
		{
			lpClientSite->Release();
			lpClientSite = NULL;
		}
		if (lpObject)
		{
			lpObject->Release();
			lpObject = NULL;
		}
		if (lpStorage)
		{
			lpStorage->Release();
			lpStorage = NULL;
		}

		if (lpAnimator)
		{
			lpAnimator->Release();
			lpAnimator = NULL;
		}

		return false;
	}
	return true;
}
CDuiString ChatDialog::EmojitoText()
{
	//pEidt->SetSel(0, -1);
	CDuiString tmpstr = pEidt->GetTextRange(0, pEidt->GetTextLength());
	if (tmpstr.IsEmpty())return tmpstr;
	CDuiString finalstr;
	static int j = 0;
	for (int i = 0; i < tmpstr.GetLength(); i++){
		if (tmpstr[i] == ' '){
			int index = emojiIndex[j];
			j++;
			const Emojis* emoji = Emojis::Instance();
			CDuiString replacestr = emoji->m_emojis[index]->GetTag();
			finalstr += replacestr;
		}
		else{
			finalstr += tmpstr[i];
		}
	}
	return finalstr;
}



