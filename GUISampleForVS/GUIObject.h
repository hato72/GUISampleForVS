// ================================================================================
// GUIObject
// �ҏW����t�@�C��
// Copyright (c) KOEITECMO CO., LTD. All rights reserved.
// ================================================================================

#pragma once

struct FXY : public POINTFLOAT
{
public:
	// ������.
	FXY() { x = y = 0.0f; }
	FXY(float _x, float _y) { x = _x; y = _y; }
	// �L���X�g���Z�q(for KTGL).
	operator POINTFLOAT&() { return *this; }
	operator const POINTFLOAT&() const { return *this; }
	// �A�N�Z�X�֐�.
	FXY& Set(float _x, float _y) { x = _x; y = _y; return *this; }
	FXY& Zero() { return Set(0.0f, 0.0f); }
	FXY& One() { return Set(1.0f, 1.0f); }
	FXY& Invalidate() { return Set(-1.0f, -1.0f); }
	bool IsValid() { return x >= 0.0f; }
	// ���Z�q
	FXY& operator+=(const FXY& _xy) { x += _xy.x; y += _xy.y; return *this; }			// this += FXY
	FXY& operator-=(const FXY& _xy) { x -= _xy.x; y -= _xy.y; return *this; }			// this -= FXY
	FXY& operator*=(float f) { x *= f; y *= f; return *this; }							// this *= f
	FXY& operator*=(const FXY& _xy) { x *= _xy.x; y *= _xy.y; return *this; }			// this *= FXY
	FXY& operator/=(float f) { x /= f; y /= f; return *this; }							// this /= f
	FXY& operator/=(const FXY& _xy) { x /= _xy.x; y /= _xy.y; return *this; }			// this /= FXY
	FXY operator+(const FXY& _xy) const { return FXY(x + _xy.x, y + _xy.y); }			// FXY = this + FXY
	FXY operator-() const { return FXY(-x, -y); }										// FXY = - this
	FXY operator-(const FXY& _xy) const { return FXY(x - _xy.x, y - _xy.y); }			// FXY = this - FXY
	FXY operator*(float f) const { return FXY(x * f, y * f); }							// FXY = this * f
	FXY operator*(const FXY& _xy) const { return FXY(x * _xy.x, y * _xy.y); }			// FXY = this * FXY
	FXY operator/(float f) const { return FXY(x / f, y / f); }							// FXY = this / f
	FXY operator/(const FXY& _xy) const { return FXY(x / _xy.x, y / _xy.y); }			// FXY = this / FXY
};

#define TRACE(str, ...) \
	{ \
		TCHAR c[256]; \
		_stprintf_s(c, 256, str, __VA_ARGS__); \
		OutputDebugString(c); \
	}

#define MIN(a, b)		((a) > (b))? (b): (a)
#define MAX(a, b)		((a) >= (b))? (a): (b)
#define COUNTOF(a)		((sizeof a)/(sizeof a[0]))


// �p�x�v�Z.
#define F_PI			((float)M_PI)
#define F_2PI			((float)(M_PI*2.0))
#define RAD2DEG(a)		(((a)*180.0f/F_PI))														// ���W�A�����x.
#define DEG2RAD(a)		(((a)*F_PI/180.0f))														// �x�����W�A��.
#define ANGLE(a)		DEG2RAD(a)																// �x�����W�A��.
#define FIT_DEG(x)		(((x)>180.0f) ? ((x)-360.0f) : (((x)<=-180.0f) ? ((x)+360.0f) : (x)))	// �p�x�␳( -180.0 <= x <= 180.0 ).
#define FIT_RAD(x)		(((x)>F_PI) ? ((x)-F_2PI) : (((x)<=-F_PI) ? ((x)+F_2PI) : (x)))			// ���W�A���␳( -F_PI <= x <= F_PI ).
#define FIT_ANGLE(x)	(((x)>=360.0f) ? ((x)-360.0f) : ((x) < 0.0f) ? ((x)+360.0f) : (x))		// �p�x�␳( 0.0 <= x <= 360.0 ).
#define IS_FIT_ANGLE(x)	(((x)<=360.0f) && ((x)>=0.0f))											// �p�x�`�F�b�N.


// �v���g�^�C�v�錾.
extern UINT GetRandomSeed();
extern UINT GetUpdateMsec();
extern void ResetObject();
extern void UpdateObject(HDC& hdc, UINT unTimer);
extern void RenderCup(HDC& hdc);
extern int Random(int nMax);
extern int RandomRange(int nMin, int nMax);
extern float RandomF(float fMax);
extern float RandomRangeF(float fMin, float fMax);

// �x�N�g��.
extern float CalcDistance(FXY sLhs, FXY sRhs);
extern float VectorLength(FXY sXy);
extern FXY VectorSub(FXY sFrom, FXY sTo);
extern void VectorNormalize(FXY& sXy);
extern float Vector2DDot(FXY sLhs, FXY sRhs);
extern float Vector2DCross(FXY sLhs, FXY sRhs);

// �`��n.
extern void GetRotPos(FXY* pOutput, FXY center, FXY* pPos, int nNum, float fAngle);
extern void RenderPolygon(HDC hdc, POINT* pPos, int nNum, UINT rgb);
extern void RenderTriangle(HDC hdc, POINT* pPos, UINT rgb);
extern void RenderTriangle(HDC hdc, FXY* pPos, UINT rgb);
extern void RenderRotTriangle(HDC hdc, FXY& center, FXY* pPos, float fAngle, UINT rgb);
extern void RenderRectangle(HDC hdc, POINT* pPos, UINT rgb);
extern void RenderRectangle(HDC hdc, FXY* pPos, UINT rgb);
extern void RenderRotRectangle(HDC hdc, FXY& center, FXY* pPos, float fAngle, UINT rgb);
extern void RenderCircle(HDC hdc, float x, float y, float r, UINT rgb);
