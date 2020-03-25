
// ServerUIDoc.h: CServerUIDoc 类的接口
//
#pragma once



class CServerUIDoc : public CDocument
{
protected: // 仅从序列化创建
	CServerUIDoc() noexcept;
	DECLARE_DYNCREATE(CServerUIDoc)

// 特性
public:
	CPtrList m_TaskItems;
	int m_HeartbeatPort;
	int m_ControlPort;
	int m_TaskPort;
	int m_ResultPort;
	CString ipAddress;
	//BOOL IsTasksLoaded;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CServerUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	void ReadConfigFile();

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
