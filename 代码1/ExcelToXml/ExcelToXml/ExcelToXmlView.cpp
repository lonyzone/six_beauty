
// ExcelToXmlView.cpp : CExcelToXmlView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ExcelToXml.h"
#endif

#include "ExcelToXmlDoc.h"
#include "ExcelToXmlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExcelToXmlView

IMPLEMENT_DYNCREATE(CExcelToXmlView, CView)

BEGIN_MESSAGE_MAP(CExcelToXmlView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CExcelToXmlView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CExcelToXmlView ����/����

CExcelToXmlView::CExcelToXmlView()
{
	// TODO: �ڴ˴���ӹ������

}

CExcelToXmlView::~CExcelToXmlView()
{
}

BOOL CExcelToXmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CExcelToXmlView ����

void CExcelToXmlView::OnDraw(CDC* /*pDC*/)
{
	CExcelToXmlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CExcelToXmlView ��ӡ


void CExcelToXmlView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CExcelToXmlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CExcelToXmlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CExcelToXmlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CExcelToXmlView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExcelToXmlView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CExcelToXmlView ���

#ifdef _DEBUG
void CExcelToXmlView::AssertValid() const
{
	CView::AssertValid();
}

void CExcelToXmlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExcelToXmlDoc* CExcelToXmlView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExcelToXmlDoc)));
	return (CExcelToXmlDoc*)m_pDocument;
}
#endif //_DEBUG


// CExcelToXmlView ��Ϣ�������
