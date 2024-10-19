// ================================================================================
// GUIObject
// �ҏW����t�@�C��
// Copyright (c) KOEITECMO CO., LTD. All rights reserved.
// ================================================================================

#include "stdafx.h"
#include "GUIObject.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#define MAX_SHAPES 10

/*
������  ������  ������  ������
��{�I�ɕҏW����͖̂{�t�@�C���݂̂ŕ����G���W�����������邱�Ƃ��ł��܂��B
�T���v���Ƃ��Đ}�`��\���������������܂����A�}�`�̕`��̕��@�������ł�����A�폜���č\���܂���B

�v���O�����Ƃ��Ă̐݌v�̗ǂ������͋C�ɂ��Ȃ��đ��v�ł��B
�O���[�o���ϐ��𑽗p���Ă��\���܂��񂵁A
C++�̕�������Ă���Ȃ�Aclass��݌v���ė��p���邩�����Ŏ������Ă���肠��܂���B

���W�͌v�Z�̊֌W��A�S��float(����)�𗘗p����悤�ɂ��Ă��������B

�֎~����
�E�O�����C�u�����𗘗p���Ă͂Ȃ�Ȃ�
�E�{�v���W�F�N�g����у\�[�X�R�[�h��GitHub���̊O���X�g���[�W�֕ۑ����Ă͂Ȃ�Ȃ�
�E���҂֏��n������A�{���������肵�Ă͂Ȃ�Ȃ�
*/


const FXY g_sCupPoint[4] = { { 100.0f, 100.0f },{ 100.0f, 600.0f },{ 600.0f, 600.0f },{ 600.0f, 100.0f } }; // �R�b�v�̎l���̍��W.


// ��F�~�̍\����.
struct S_CIRCLE {
	float x;
	float y;
	float r;
	float speed;
	float lastResetTime;
	float lastZeroSpeedTime;
	UINT rgb;
};

S_CIRCLE g_sCircle;	// �~.

struct S_RECTANGLE {
	float x;
	float y;
	float speed;
	float lastResetTime;
	float lastZeroSpeedTime;
	UINT rgb;
};

S_RECTANGLE g_sRectangle;

struct S_TRIANGLE {
	float x;
	float y;
	float speed;
	float lastResetTime;
	float lastZeroSpeedTime;
	UINT rgb;
};
S_TRIANGLE g_sTriangle;



// �����V�[�h���擾����.
// �V�[�h�l���Œ艻����ƁA���񓯂��������������܂��B
// �܂薈�񓯂��z�u��Ԃ���X�^�[�g���܂�.
UINT GetRandomSeed()
{
	UINT unSeed = 12345;

	// �������V�[�h�l���Œ艻�������ꍇ�́����R�����g�A�E�g���Ă�������.
	unSeed = static_cast<UINT>(time(NULL));	// �����炪�L�����Ɩ���قȂ闐�����������܂�.

	return unSeed;
}


// �X�V�p�x���擾����.
// �R�}����œ��삳���邱�Ƃ��ł��܂�.
UINT GetUpdateMsec()
{
	// �X�V����(1000��1�b�A�������X�V��������ΐ�����傫������).
	// �����Ԓʂ�Ɍv�Z����K�v�͂���܂���.
	return 100;	// 0.1�b�Ԋu�ōX�V.
}


// �e��I�u�W�F�N�g�̏����z�u.
// �u�����z�u���Z�b�g�v�̃{�^�����������炱�̊֐����Ă΂�܂�.
// ���̌�AUpdateObject()��unTimer == 0 �łP�񂾂��Ă΂�܂�.
// �e�}�`�̏����ʒu��������Őݒ肵�Ă�������.
void ResetObject()
{
	TRACE(_T("Reset Object\n"));	// printf�̑���ɗ��p�ł��܂��B�i�o�̓E�C���h�E�ɕ\������܂��j.

	// �~�̏����ʒu�������_���Őݒ�.
	g_sCircle.x = RandomRangeF(200.0f, 500.0f);	// 200�`500 �̊Ԃ̃����_���Ȑ���.
	g_sCircle.y = RandomRangeF(100.0f, 200.0f);
	g_sCircle.r = RandomRangeF(20.0f, 50.0f);
	g_sCircle.rgb = RGB(144, 238, 144);

	g_sRectangle.x = RandomRangeF(200.0f, 500.0f);	// 200�`500 �̊Ԃ̃����_���Ȑ���.
	g_sRectangle.y = RandomRangeF(100.0f, 200.0f);
	g_sRectangle.rgb = RGB(30, 144, 255);

	g_sTriangle.x = RandomRangeF(200.0f, 500.0f);	// 200�`500 �̊Ԃ̃����_���Ȑ���.
	g_sTriangle.y = RandomRangeF(100.0f, 200.0f);
	g_sTriangle.rgb = RGB(170, 242, 144);

}


// �e��I�u�W�F�N�g�̍X�V.
// �u�R���W���������v�̃{�^���������ƁA���Ԋu�Ŗ{�֐����Ă΂�܂�(������x�����Ǝ~�܂�܂�).
// �Ă΂��^�C�~���O��ς������Ƃ��� GetUpdateMsec() ��ύX���Ă�������(������Ԃ�0.1�b�Ԋu�ł�).
// �ʂɎ��Ԃɍ��킹�ď��������A��u�ŗ������ĉ�����ԂƂ��Ă���肠��܂���.
// �����@���ɑ����ė�������K�v������܂���(������ۂ��v�Z�ŏ\���ł�).
// �����FunTimer �Ă΂��x�ɃJ�E���g�A�b�v���܂�(0�͏������).

void UpdateObject(HDC& hdc, UINT unTimer){
	bool bReset = (unTimer == 0);
	float gravity = 0.8f;
	float restitution = 0.3f; // �����W��

	if (!bReset) {
		// �O�p�`�̑��x�ƈʒu�̍X�V
		g_sTriangle.speed += gravity;
		g_sTriangle.y += g_sTriangle.speed;

		// �l�p�`�̑��x�ƈʒu�̍X�V
		g_sRectangle.speed += gravity;
		g_sRectangle.y += g_sRectangle.speed;

		// �~�̑��x�ƈʒu�̍X�V
		g_sCircle.speed += gravity;
		g_sCircle.y += g_sCircle.speed;

		// �R�b�v�̒�Ɛ}�`�̏Փ˔���
		//���ɏՓ˂����璵�˕Ԃ�悤�Ɏ���

		if (g_sTriangle.y + 40.0f > g_sCupPoint[1].y) {
			g_sTriangle.y = g_sCupPoint[1].y -40.0f;
			g_sTriangle.speed = -g_sTriangle.speed * restitution;
		}
		if (g_sRectangle.y + 50.0f > g_sCupPoint[1].y) {
			g_sRectangle.y = g_sCupPoint[1].y - 50.0f;
			g_sRectangle.speed = -g_sRectangle.speed * restitution;
		}
		if (g_sCircle.y + g_sCircle.r > g_sCupPoint[1].y) {
			g_sCircle.y = g_sCupPoint[1].y - g_sCircle.r;
			g_sCircle.speed = -g_sCircle.speed * restitution;
		}

		// �R�b�v�̍��E�̕ǂƐ}�`�̏Փ˔���
		//������̒l��ݒ�
		if (g_sTriangle.x - 50.0f < g_sCupPoint[0].x) g_sTriangle.x = g_sCupPoint[0].x + 50.0f;
		if (g_sTriangle.x + 50.0f > g_sCupPoint[2].x) g_sTriangle.x = g_sCupPoint[2].x - 50.0f;
		if (g_sRectangle.x - 25.0f < g_sCupPoint[0].x) g_sRectangle.x = g_sCupPoint[0].x + 25.0f;
		if (g_sRectangle.x + 25.0f > g_sCupPoint[2].x) g_sRectangle.x = g_sCupPoint[2].x - 25.0f;
		if (g_sCircle.x - g_sCircle.r < g_sCupPoint[0].x) g_sCircle.x = g_sCupPoint[0].x + g_sCircle.r;
		if (g_sCircle.x + g_sCircle.r > g_sCupPoint[2].x) g_sCircle.x = g_sCupPoint[2].x - g_sCircle.r;


		// �~�Ǝl�p�`�̏Փ˔���
		float dist_cir_rec = sqrtf(powf(g_sCircle.x - g_sRectangle.x, 2) + powf(g_sCircle.y - g_sRectangle.y, 2)); //���[�N���b�h����
		if (dist_cir_rec < g_sCircle.r + 40.0f) {
			float angle = atan2f(g_sCircle.y - g_sRectangle.y, g_sCircle.x - g_sRectangle.x);

			// �~���n�ʂɐڂ��Ă���ꍇ�A�l�p�`����ɉ����o��
			if (g_sCircle.y + g_sCircle.r >= g_sCupPoint[1].y - 0.1f) {
				//g_sCircle.x = g_sRectangle.x + cosf(angle) * (g_sCircle.r + 35.0f);
				g_sCircle.y = g_sRectangle.y + sinf(angle) * (g_sCircle.r + 35.0f);
				//g_sRectangle.speed = 0;
			}
			else {
				g_sCircle.x = g_sRectangle.x + cosf(angle) * (g_sCircle.r + 35.0f);
				g_sCircle.y = g_sRectangle.y + sinf(angle) * (g_sCircle.r + 35.0f);
			}

		}
		// �~�ƎO�p�`�̏Փ˔���
		float dist_cir_tri = sqrtf(powf(g_sCircle.x - g_sTriangle.x, 2) + powf(g_sCircle.y - g_sTriangle.y, 2));
		if (dist_cir_tri < g_sCircle.r + 40.0f) {
			float angle = atan2f(g_sCircle.y - g_sTriangle.y, g_sCircle.x - g_sTriangle.x);

			// �~���n�ʂɐڂ��Ă���ꍇ�A�O�p�`����ɉ����o��
			if (g_sCircle.y + g_sCircle.r >= g_sCupPoint[1].y - 0.1f) {
				//g_sTriangle.x = g_sCircle.x + cosf(angle) * (g_sCircle.r + 40.0f);
				g_sTriangle.y = g_sCircle.y - sinf(angle) * (g_sCircle.r + 40.0f);
				//g_sTriangle.speed = 0;
			}
			else {
				g_sCircle.x = g_sTriangle.x + cosf(angle) * (g_sCircle.r + 40.0f);
				g_sCircle.y = g_sTriangle.y + sinf(angle) * (g_sCircle.r + 40.0f);
			}
		}
		// �l�p�`�ƎO�p�`�̏Փ˔���
		float dist_rec_tri = sqrtf(powf(g_sRectangle.x - g_sTriangle.x, 2) + powf(g_sRectangle.y - g_sTriangle.y, 2));
		if (dist_rec_tri < 80.0f) {
			float angle = atan2f(g_sRectangle.y - g_sTriangle.y, g_sRectangle.x - g_sTriangle.x);
			g_sRectangle.x = g_sTriangle.x + cosf(angle) * 80.0f;
			g_sRectangle.y = g_sTriangle.y + sinf(angle) * 80.0f;
			//g_sCircle.speed = g_sRectangle.speed;
		}

	}

	// �O�p�`�̕`��
	{
		FXY center = { g_sTriangle.x, g_sTriangle.y };
		float w = 100.0f;
		float h = 70.0f;
		float angle = 30.0f;
		UINT rgb = g_sTriangle.rgb;

		angle = FIT_ANGLE(angle);

		FXY point[3];
		point[0] = { center.x + w / 2.0f, center.y - h / 3.0f };
		point[1] = { center.x - w / 2.0f, center.y - h / 3.0f };
		point[2] = { center.x,            center.y + (2.0f * h) / 3.0f };

		FXY radPos[3];
		GetRotPos(radPos, center, point, COUNTOF(radPos), angle);


		RenderTriangle(hdc, radPos, rgb);
	}

	// �l�p�`�̕`��
	{
		FXY center = { g_sRectangle.x, g_sRectangle.y };
		float w = 50;
		float h = 100;
		float angle = 45.0f;
		UINT rgb = g_sRectangle.rgb;

		angle = FIT_ANGLE(angle);

		FXY point[4];
		point[0] = { center.x - w / 2.0f, center.y - h / 2.0f };
		point[1] = { center.x - w / 2.0f, center.y + h / 2.0f };
		point[2] = { center.x + w / 2.0f, center.y + h / 2.0f };
		point[3] = { center.x + w / 2.0f, center.y - h / 2.0f };

		FXY radPos[4];
		GetRotPos(radPos, center, point, COUNTOF(radPos), angle);

		RenderRectangle(hdc, point, rgb);
	}

	// �~�̕`��
	{

		RenderCircle(hdc, g_sCircle.x, g_sCircle.y, g_sCircle.r, g_sCircle.rgb);

	}
}




// �R�b�v�̕`��.
// ������͕ύX�̕K�v�͂Ȃ��͂�.
void RenderCup(HDC& hdc)
{
	// �R�b�v�̕`��
	{
		const int pointCount = sizeof(g_sCupPoint) / sizeof(g_sCupPoint[0]);
		// �`��
		HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		HGDIOBJ hOldObj = SelectObject(hdc, hPen);
		for (int i = 1; i < pointCount; i++) {
			MoveToEx(hdc, (int)g_sCupPoint[i - 1].x, (int)g_sCupPoint[i - 1].y, NULL);
			LineTo(hdc, (int)g_sCupPoint[i].x, (int)g_sCupPoint[i].y);
		}
		SelectObject(hdc, hOldObj);
		DeleteObject(hPen);
	}
}

/* ������  ������ �����܂ł�ҏW���Ă������� ������  ������ */



// �����擾.
int Random(int nMax)
{
	return rand() % (nMax + 1);
}

// �����擾.
int RandomRange(int nMin, int nMax)
{
	if (nMin >= nMax) {
		TRACE(_T("�ő�l���ŏ��l���傫���Ȃ�悤�ɐݒ肵�Ă�������\n"));
		ASSERT(false);
		return nMin;
	}
	UINT nRnd = static_cast<UINT>(nMax - nMin + 1);
	return (nMin + rand() % nRnd);
}

// �����擾.
float RandomF(float fMax)
{
	return static_cast<float>(rand() % static_cast<UINT>(fMax + 1));
}

// �����擾.
float RandomRangeF(float fMin, float fMax)
{
	if (fMin >= fMax) {
		TRACE(_T("�ő�l���ŏ��l���傫���Ȃ�悤�ɐݒ肵�Ă�������\n"));
		ASSERT(false);
		return fMin;
	}
	UINT nRnd = static_cast<UINT>(fMax - fMin + 1);
	return static_cast<float>(fMin + rand() % nRnd);
}


// �Q�n�_�Ԃ̋���.
float CalcDistance(FXY sLhs, FXY sRhs)
{
	FXY sSub = sRhs - sLhs;
	return sqrtf(sSub.x * sSub.x + sSub.y * sSub.y);
}

// �x�N�g���̃T�C�Y.
float VectorLength(FXY sXy)
{
	return sqrtf(sXy.x * sXy.x + sXy.y * sXy.y);
}

// �x�N�g���̍�
FXY VectorSub(FXY sFrom, FXY sTo)
{
	return sTo - sFrom;
}

// �x�N�g�����K��.
void VectorNormalize(FXY& sXy)
{
	float fLength = sXy.x * sXy.x + sXy.y * sXy.y;
	ASSERT(fLength);
	if (fLength > 0) {
		fLength = 1.0f / sqrtf(fLength);
		sXy.x *= fLength;
		sXy.y *= fLength;
	}
}

// �x�N�g���̓���.
float Vector2DDot(FXY sLhs, FXY sRhs)
{
	return (sLhs.x * sRhs.x) + (sLhs.y * sRhs.y);
}

// �x�N�g���̊O��.
float Vector2DCross(FXY sLhs, FXY sRhs)
{
	return (sLhs.x * sRhs.y) - (sLhs.y * sRhs.x);
}


// ��]��̍��W�擾.
// pOutput	��]�v�Z��̊e���_�̍��W�l�z��.
// center	��]�̒��S���W.
// pPos		��]�v�Z�O�̊e���_�̍��W�l�z��.
// nNum		���W�l�z��̌�(�O�p�`�Ȃ�3, �l�p�`�Ȃ�4��n��).
// fAngle	��]�p�x.
void GetRotPos(FXY* pOutput, FXY center, FXY* pPos, int nNum, float fAngle)
{
	if (!IS_FIT_ANGLE(fAngle)) {
		TRACE(_T("�p�x��0.0�`360.0�Ɏ��܂��Ă��܂���[%f]\n"), fAngle);
		ASSERT(false);	// FIT_ANGLE()��ʂ��Ė߂��ꂽ�l������ȍ~�����p����K�v������܂�.
	}
	float rad = ANGLE(fAngle);	// ���W�A��.
	float cosValue = cosf(rad);
	float sinValue = sinf(rad);
	for (int i = 0; i < nNum; i++) {
		FXY sBuf = pPos[i];
		// ��]�ϊ�
		pOutput[i].x = center.x + (sBuf.x - center.x) * cosValue - (sBuf.y - center.y) * sinValue;
		pOutput[i].y = center.y + (sBuf.x - center.x) * sinValue + (sBuf.y - center.y) * cosValue;
		// �덷�͏o�邯�ǁA���肪����Ȃ邽�ߏ����͎l�̌ܓ�����.
		pOutput[i].x = roundf(pOutput[i].x);
		pOutput[i].y = roundf(pOutput[i].y);
	}
}

// ���p�`�̕`��.
void RenderPolygon(HDC hdc, POINT* pPos, int nNum, UINT rgb)
{
	HBRUSH hBrush = CreateSolidBrush(rgb);
	HGDIOBJ hOldObj = SelectObject(hdc, hBrush);	// �h��Ԃ��F�ݒ�
	SetPolyFillMode(hdc, WINDING);
	Polygon(hdc, pPos, nNum);
	SelectObject(hdc, hOldObj);	// ���̐F�ɕ��A
	DeleteObject(hBrush);
}

// �O�p�`�̕`��.
void RenderTriangle(HDC hdc, POINT* pPos, UINT rgb)
{
	RenderPolygon(hdc, pPos, 3, rgb);
}
void RenderTriangle(HDC hdc, FXY* pPos, UINT rgb)
{
	POINT sXy[3];
	for (int i = 0; i < 3; i++) {
		sXy[i].x = static_cast<int>(pPos[i].x);
		sXy[i].y = static_cast<int>(pPos[i].y);
	}
	RenderPolygon(hdc, sXy, 3, rgb);
}

// ��]�����O�p�`�̕`��.
// center	��]�̒��S.
// pPos		�e���_���W.
// fAngle	��]�p(0.0�`360.0).
// rgb		�O�p�`�̐F.
void RenderRotTriangle(HDC hdc, FXY& center, FXY* pPos, float fAngle, UINT rgb)
{
	if (!IS_FIT_ANGLE(fAngle)) {
		TRACE(_T("�p�x��0.0�`360.0�Ɏ��܂��Ă��܂���[%f]\n"), fAngle);
		ASSERT(false);	// FIT_ANGLE()��ʂ��Ė߂��ꂽ�l������ȍ~�����p����K�v������܂�.
	}
	
	// ��]��̍��W���擾����.
	FXY radPos[3];
	GetRotPos(radPos, center, pPos, COUNTOF(radPos), fAngle);
	
	POINT drawPoint[3];		// �p�x���l�������`�掞�̍��W(�`��API�̓s���Ő����ɂȂ��Ă��܂�)
	for (int i = 0; i < 3; i++) {
		drawPoint[i] = { static_cast<LONG>(radPos[i].x), static_cast<LONG>(radPos[i].y) };
	}

	// �O�p�`�̕`��
	RenderTriangle(hdc, drawPoint, rgb);
}


// �l�p�`�̕`��.
void RenderRectangle(HDC hdc, POINT* pPos, UINT rgb)
{
	RenderPolygon(hdc, pPos, 4, rgb);
}
void RenderRectangle(HDC hdc, FXY* pPos, UINT rgb)
{
	POINT sXy[4];
	for (int i = 0; i < 4; i++) {
		sXy[i].x = static_cast<int>(pPos[i].x);
		sXy[i].y = static_cast<int>(pPos[i].y);
	}
	RenderPolygon(hdc, sXy, 4, rgb);
}

// ��]�����l�p�`�̕`��.
// center	��]�̒��S.
// pPos		�e���_���W.
// fAngle	��]�p(0.0�`360.0).
// rgb		�l�p�`�̐F.
void RenderRotRectangle(HDC hdc, FXY& center, FXY* pPos, float fAngle, UINT rgb)
{
	if (!IS_FIT_ANGLE(fAngle)) {
		TRACE(_T("�p�x��0.0�`360.0�Ɏ��܂��Ă��܂���[%f]\n"), fAngle);
		ASSERT(false);	// FIT_ANGLE()��ʂ��Ė߂��ꂽ�l������ȍ~�����p����K�v������܂�.
	}
	
	// ��]��̍��W���擾����.
	FXY radPos[4];	// �z�񐔂ɒ���.
	GetRotPos(radPos, center, pPos, COUNTOF(radPos), fAngle);

	POINT drawPoint[4];		// �p�x���l�������`�掞�̍��W
	for (int i = 0; i < 4; i++) {
		drawPoint[i] = { static_cast<LONG>(radPos[i].x), static_cast<LONG>(radPos[i].y) };
	}

	// �l�p�`�̕`��.
	RenderRectangle(hdc, drawPoint, rgb);
}


// �~�̕`��.
// ���W(x, y)�ɔ��ar�̉~��`�悷��.
void RenderCircle(HDC hdc, float x, float y, float r, UINT rgb)
{
	int nX = static_cast<int>(x);
	int nY = static_cast<int>(y);
	int nR = static_cast<int>(r);

	// �`��
	HBRUSH hBrush = CreateSolidBrush(rgb);
	HGDIOBJ hOldObj = SelectObject(hdc, hBrush);	// �h��Ԃ��F�ݒ�
	Ellipse(hdc, nX - nR, nY - nR, nX + nR, nY + nR);
	SelectObject(hdc, hOldObj);	// ���̐F�ɕ��A
	DeleteObject(hBrush);
}