// TinyObfuscate - By Michael Haephrati (C)2017 Michael Haephrati, Secured Globe, Inc.
// 1501 Broadway STE 1200, New York 10036, NY

#include "stdafx.h"
#include "TinyObfuscate.h"
#include "TinyObfuscateDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <vector>
#include <ctime>
#include "random.h"

#define RANDOM_DIGIT (RandomIntFromRange(1,9))
#define RANDOM_WCHAR (WCHAR)(RandomIntFromRange(L'a',L'z'))
#define RANDOM_INT_LARGER_THAN(n) (int)(RandomIntFromRange(n,122))
#define RANDOM_INT_SMALLER_THAN(n) (int)((n>48)?RandomIntFromRange(48, n):n)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int arr[1028];

int RandomIntFromRange(int From, int To)
{
	int result;
	std::random_device rd;
	pcg rand(rd);
	std::uniform_int_distribution<> u(From, To);
	result = u(rand);
	return result;
}

bool find(const std::vector<int>& vec, int index)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		if (index == vec[i])
			return true;
	}

	return false;
}
void shuffle(int array[], const int size)
{
	const int n_size = size;
	int temp[1028];
	std::vector<int> indices;

	for (int i(0); i < size; ++i)
		temp[i] = array[i];

	int index = rand() % size;
	indices.push_back(index);

	for (int i = 0; i < size; ++i)
	{
		if (i == 0)
			array[i] = temp[index];
		else
		{
			while (find(indices, index))
				index = rand() % size;

			indices.push_back(index);
			array[i] = temp[index];
		}
	}

}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTinyObfuscateDlg dialog



CTinyObfuscateDlg::CTinyObfuscateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTinyObfuscateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTinyObfuscateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, Original);
	DDX_Control(pDX, IDC_EDIT1, Result);
	DDX_Control(pDX, IDC_EDIT3, Varname);
}

BEGIN_MESSAGE_MAP(CTinyObfuscateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTinyObfuscateDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CTinyObfuscateDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT3, &CTinyObfuscateDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CTinyObfuscateDlg message handlers

BOOL CTinyObfuscateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTinyObfuscateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTinyObfuscateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTinyObfuscateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTinyObfuscateDlg::OnBnClickedOk()
{
	CString OriginalText,ResultText,ResultTextLine,VarnameText;
	Original.GetWindowTextW(OriginalText);
	Varname.GetWindowTextW(VarnameText);
	int i;
	int d;
	int z;
	int x;
	int choice;
	WCHAR c;
	CString Formula;
	CString TextWithJunk;
	TextWithJunk = OriginalText;
	int Length = OriginalText.GetLength();
	TextWithJunk += (CString)L" ";
	for (i = Length + 1; i < Length * 2; i++)
	{
		WCHAR result = RANDOM_WCHAR;
		TextWithJunk += (CString)(result);
	}
	ResultTextLine.Format(L"WCHAR %s[%d];", VarnameText,Length * 2);
	ResultText = ResultText + ResultTextLine + L"\r\n";
	TextWithJunk.SetAt(Length, L'\0');
	Length = Length * 2;
	for (i = 0; i<Length; i++) arr[i]=i;
	shuffle(arr,Length);
	for (i = 0; i < Length; i++)
	{
		c = TextWithJunk[arr[i]];
		// Select obfuscation option for this character
		choice = RANDOM_DIGIT;
		x = TextWithJunk[arr[i]];

		switch (choice)
		{
			case 10:
			case 1:
				// x = z - y
				// Calculate Z
				z = RANDOM_INT_LARGER_THAN(x);
				// Calculate the difference
				d = z-x;
				Formula.Format(L"%d - %d",z,d);
				break;
			case 2:
			case 3:
				// x = z + y
				// Calculate Z
				z = RANDOM_INT_SMALLER_THAN(x);
				// Calculate the difference
				d = x - z;
				Formula.Format(L"%d + %d", z, d);
				break;
			case 4:
			case 5:
				// x = 'z' - y
				// Calculate Z
				z = RANDOM_INT_LARGER_THAN(x);
				// Calculate the difference
				d = z - x;
				Formula.Format(L"L'%c' - %d", z, d);
				break;
			case 6:
			case 7:
				// x = 'z' + y
				// Calculate Z
				z = RANDOM_INT_SMALLER_THAN(x);
				// Calculate the difference
				d = x - z;
				Formula.Format(L"L'%c' + %d", z, d);
				break;
			case 8:
			case 9:
				// x = 'z'
				Formula.Format(L"%d",x);
				break;
		}
		ResultTextLine.Format(L" %s[%d] = %s;",VarnameText, arr[i],Formula);
		int ccc = ResultTextLine.Find(L'\\');
		if (ccc>-1)
		{
			ResultTextLine = ResultTextLine.Left(ccc) + L"\\" + ResultTextLine.Mid(ccc);
		}

		ResultText = ResultText + ResultTextLine + L"\r\n";
		
	}
	Result.SetWindowTextW(ResultText);
}


void CTinyObfuscateDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CTinyObfuscateDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
