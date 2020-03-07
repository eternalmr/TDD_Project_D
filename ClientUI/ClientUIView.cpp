
// ClientUIView.cpp: CClientUIView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ClientUI.h"
#endif

#include "ClientUIDoc.h"
#include "ClientUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientUIView

IMPLEMENT_DYNCREATE(CClientUIView, CFormView)

BEGIN_MESSAGE_MAP(CClientUIView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CClientUIView 构造/析构

CClientUIView::CClientUIView() noexcept
	: CFormView(IDD_CLIENTUI_FORM)
{
	// TODO: 在此处添加构造代码

}

CClientUIView::~CClientUIView()
{
}

void CClientUIView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CClientUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CClientUIView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CClientUIView 打印

BOOL CClientUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CClientUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CClientUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CClientUIView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此处添加自定义打印代码
}


// CClientUIView 诊断

#ifdef _DEBUG
void CClientUIView::AssertValid() const
{
	CFormView::AssertValid();
}

void CClientUIView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CClientUIDoc* CClientUIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientUIDoc)));
	return (CClientUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CClientUIView 消息处理程序
