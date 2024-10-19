// ================================================================================
// GUISampleForVS
// �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
// Copyright (c) KOEITECMO CO., LTD. All rights reserved.
// ================================================================================


#include "stdafx.h"
#include "GUISampleForVS.h"
#include "GUIObject.h"


#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE g_hInst;								// ���݂̃C���^�[�t�F�C�X
HBITMAP   g_hBitmap = NULL;
POINTS    g_pos = { 0, 0 };
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
// �_�u���o�b�t�@�����O�ł�����΍�.
HBITMAP hDBuf = NULL;
HDC hDDc = NULL;
HPEN hPen;

LRESULT CALLBACK MyDlgProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp);

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	{
		// TODO: �����ɏ������Ɋ֌W����R�[�h��}�����Ă��������B
		srand(GetRandomSeed());
	}

	//MSG msg;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GUISAMPLEFORVS, szWindowClass, MAX_LOADSTRING);

	g_hInst = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_DIALOG1, NULL, (DLGPROC)MyDlgProc);

	return 0;
}

// �`��J�n.
// ������͕ύX�̕K�v�͂Ȃ��͂�.
void BeginRender(HWND hDlgWnd, PAINTSTRUCT& ps, HDC& hdc)
{
	
	// �o�b�t�@������΍폜.
	if (hDBuf) {
		DeleteDC(hDDc);
		DeleteObject(hDBuf);
		hDBuf = NULL;
	}

	// �_�u���o�b�t�@�쐬.
	hdc = GetDC(hDlgWnd);
	hDDc = CreateCompatibleDC(hdc);					// ���z�f�o�C�X�R���e�L�X�g�쐬.
	hDBuf = CreateCompatibleBitmap(hdc, 800, 800);	// ���z�f�o�C�X�R���e�L�X�g�pBMP�쐬.
	SelectObject(hDDc, hDBuf);
	ReleaseDC(hDlgWnd, hdc);
	hdc = hDDc;

	SelectObject(hdc, hPen);

	// �w�i�h��Ԃ�
	{
		RECT rect;
		::GetClientRect(hDlgWnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(250, 250, 250));
		FillRect(hdc, &rect, hBrush);

		DeleteObject(hBrush);
	}
}

// �`��I��.
// ������͕ύX�̕K�v�͂Ȃ��͂�.
void EndRender(HWND hDlgWnd, PAINTSTRUCT& ps)
{
	EndPaint(hDlgWnd, &ps);

	// �_�u���o�b�t�@��`��.
	if (hDDc) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlgWnd, &ps);
		BitBlt(hdc, 0, 0, 800, 800, hDDc, 0, 0, SRCCOPY);
		EndPaint(hDlgWnd, &ps);
	}
}


LRESULT CALLBACK MyDlgProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	const UINT unMY_TIMER = 12345;
	static BOOL s_bStart = false;

	// Window�̏o���ʒu���Œ肷��ɂ͉��L�̃R�[�h��L���ɂ��Ă�������
	//SetWindowPos(hDlgWnd, NULL, 0, 0, 800, 800, SWP_SHOWWINDOW);

	static UINT s_unTimer = 0;
	
	switch (msg)
	{
	case WM_INITDIALOG:
		g_hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		ResetObject();
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlgWnd, IDCLOSE);
		break;
	case WM_TIMER:
		InvalidateRect(hDlgWnd, NULL, FALSE);  // �ĕ`�施��.
		break;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case IDOK:
			EndDialog(hDlgWnd, IDOK);
			break;
		case IDSOLVE:
			// �R���W���������̏���������.
			if (!s_bStart) {
				SetTimer(hDlgWnd, unMY_TIMER, GetUpdateMsec(), NULL);
				s_bStart = !s_bStart;
			}
			else {
				KillTimer(hDlgWnd, unMY_TIMER);
				s_bStart = !s_bStart;
			}
			break;
		case IDRESET:
			// �I�u�W�F�N�g�̔z�u�����Z�b�g���鏈��������.
			if (s_bStart) {
				KillTimer(hDlgWnd, unMY_TIMER);
				s_bStart = !s_bStart;
			}
			ResetObject();
			s_unTimer = 0;
			InvalidateRect(hDlgWnd, NULL, FALSE);  // �ĕ`�施��.
			break;
		default:
			return FALSE;
		}
		break;
	case WM_PAINT:
		// �`��C�x���g.
		PAINTSTRUCT ps;
		HDC hdc;
		{
			// �`��J�n.
			BeginRender(hDlgWnd, ps, hdc);

			// �I�u�W�F�N�g�X�V.
			UpdateObject(hdc, s_unTimer);
			s_unTimer++;

			// �R�b�v�`��.
			RenderCup(hdc);

			// �`��I��.
			EndRender(hDlgWnd, ps);
		}
		break;
	case WM_LBUTTONDOWN:  // ���N���b�N�C�x���g
		g_pos = MAKEPOINTS(lp);
		{
			// TODO: ���N���b�N���̃R�[�h�������ɒǉ����Ă�������.
		}

		InvalidateRect(hDlgWnd, NULL, FALSE);  // �ĕ`�施��.
		break;
	case WM_DESTROY:
		DeleteObject(hPen);
		break;
	default:
		return FALSE;
	}
	return FALSE;
}







