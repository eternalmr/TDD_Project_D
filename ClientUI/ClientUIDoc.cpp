
// ClientUIDoc.cpp: CClientUIDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ClientUI.h"
#endif

#include "ClientUIDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientUIDoc

IMPLEMENT_DYNCREATE(CClientUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientUIDoc, CDocument)
END_MESSAGE_MAP()


// CClientUIDoc 构造/析构

CClientUIDoc::CClientUIDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CClientUIDoc::~CClientUIDoc()
{
}

void CClientUIDoc::ReadConfigFile()
{
	CString configNameStr("client configuration");
	CString filePath(".\\client.config");

	int result = GetPrivateProfileInt(configNameStr, CString("HeartbeatPort"), 5555, filePath);
	m_HeartbeatPort = result;

	result = GetPrivateProfileInt(configNameStr, CString("ClientID"), 1, filePath);
	m_ClientID = result;

	result = GetPrivateProfileInt(configNameStr, CString("ControlPort"), 5556, filePath);
	m_ControlPort = result;

	result = GetPrivateProfileInt(configNameStr, CString("TaskPort"), 5557, filePath);
	m_TaskPort = result;

	result = GetPrivateProfileInt(configNameStr, CString("ResultPort"), 5558, filePath);
	m_ResultPort = result;

	GetPrivateProfileString(configNameStr, CString("IPAddress"), CString("127.0.0.1"), ipAddress.GetBuffer(MAX_PATH), MAX_PATH, filePath);
}

BOOL CClientUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	SetTitle(TEXT("并行推演管控系统客户端"));

	ReadConfigFile();

	// 将相关数据导入到client对应数据结构中去
	client.set_id(m_ClientID);
	client.heartbeat_port = m_HeartbeatPort;
	client.control_port = m_ControlPort;
	client.task_port = m_TaskPort;
	client.result_port = m_ResultPort;
	client.set_ip(string(CT2A(ipAddress)));
	client.connect_sockets_to_ip();

	return TRUE;
}




// CClientUIDoc 序列化

void CClientUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CClientUIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CClientUIDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CClientUIDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CClientUIDoc 诊断

#ifdef _DEBUG
void CClientUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClientUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CClientUIDoc 命令
