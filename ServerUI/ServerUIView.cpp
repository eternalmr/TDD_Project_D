
// ServerUIView.cpp: CServerUIView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ServerUI.h"
#endif

#include "ServerUIDoc.h"
#include "ServerUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerUIView

IMPLEMENT_DYNCREATE(CServerUIView, CView)

BEGIN_MESSAGE_MAP(CServerUIView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CServerUIView 构造/析构

CServerUIView::CServerUIView() noexcept
{
	// TODO: 在此处添加构造代码

}

CServerUIView::~CServerUIView()
{
}

BOOL CServerUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CServerUIView 绘图

void CServerUIView::OnDraw(CDC* /*pDC*/)
{
	CServerUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CServerUIView 打印

BOOL CServerUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CServerUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CServerUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CServerUIView 诊断

#ifdef _DEBUG
void CServerUIView::AssertValid() const
{
	CView::AssertValid();
}

void CServerUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CServerUIDoc* CServerUIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerUIDoc)));
	return (CServerUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CServerUIView 消息处理程序
