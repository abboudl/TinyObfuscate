// TinyObfuscate - By Michael Haephrati (C)2017 Michael Haephrati, Secured Globe, Inc.
// 1501 Broadway STE 1200, New York 10036, NY

#pragma once
#include "afxwin.h"


// CTinyObfuscateDlg dialog
class CTinyObfuscateDlg : public CDialogEx
{
// Construction
public:
	CTinyObfuscateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TINYOBFUSCATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit Original;
	CEdit Result;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit3();
	CEdit Varname;
};
