
// ClientUIView.h: CClientUIView 类的接口
//

#pragma once


class CClientUIView : public CFormView
{
protected: // 仅从序列化创建
	CClientUIView() noexcept;
	DECLARE_DYNCREATE(CClientUIView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_CLIENTUI_FORM };
#endif

// 特性
public:
	CClientUIDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CClientUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ClientUIView.cpp 中的调试版本
inline CClientUIDoc* CClientUIView::GetDocument() const
   { return reinterpret_cast<CClientUIDoc*>(m_pDocument); }
#endif

