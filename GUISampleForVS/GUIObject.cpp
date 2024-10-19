// ================================================================================
// GUIObject
// 編集するファイル
// Copyright (c) KOEITECMO CO., LTD. All rights reserved.
// ================================================================================

#include "stdafx.h"
#include "GUIObject.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#define MAX_SHAPES 10

/*
▼▼▼  ▼▼▼  ▼▼▼  ▼▼▼
基本的に編集するのは本ファイルのみで物理エンジンを実装することができます。
サンプルとして図形を表示する実装がありますが、図形の描画の方法が理解できたら、削除して構いません。

プログラムとしての設計の良し悪しは気にしなくて大丈夫です。
グローバル変数を多用しても構いませんし、
C++の方が慣れているなら、classを設計して利用するかたちで実装しても問題ありません。

座標は計算の関係上、全てfloat(少数)を利用するようにしてください。

禁止事項
・外部ライブラリを利用してはならない
・本プロジェクトおよびソースコードをGitHub等の外部ストレージへ保存してはならない
・他者へ譲渡したり、閲覧させたりしてはならない
*/


const FXY g_sCupPoint[4] = { { 100.0f, 100.0f },{ 100.0f, 600.0f },{ 600.0f, 600.0f },{ 600.0f, 100.0f } }; // コップの四隅の座標.


// 例：円の構造体.
struct S_CIRCLE {
	float x;
	float y;
	float r;
	float speed;
	float lastResetTime;
	float lastZeroSpeedTime;
	UINT rgb;
};

S_CIRCLE g_sCircle;	// 円.

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



// 乱数シードを取得する.
// シード値を固定化すると、毎回同じ乱数が発生します。
// つまり毎回同じ配置状態からスタートします.
UINT GetRandomSeed()
{
	UINT unSeed = 12345;

	// もしもシード値を固定化したい場合は↓をコメントアウトしてください.
	unSeed = static_cast<UINT>(time(NULL));	// こちらが有効だと毎回異なる乱数が発生します.

	return unSeed;
}


// 更新頻度を取得する.
// コマ送りで動作させることができます.
UINT GetUpdateMsec()
{
	// 更新時間(1000で1秒、ゆっくり更新したければ数字を大きくする).
	// 実時間通りに計算する必要はありません.
	return 100;	// 0.1秒間隔で更新.
}


// 各種オブジェクトの初期配置.
// 「初期配置リセット」のボタンを押したらこの関数が呼ばれます.
// その後、UpdateObject()がunTimer == 0 で１回だけ呼ばれます.
// 各図形の初期位置をこちらで設定してください.
void ResetObject()
{
	TRACE(_T("Reset Object\n"));	// printfの代わりに利用できます。（出力ウインドウに表示されます）.

	// 円の初期位置をランダムで設定.
	g_sCircle.x = RandomRangeF(200.0f, 500.0f);	// 200～500 の間のランダムな数字.
	g_sCircle.y = RandomRangeF(100.0f, 200.0f);
	g_sCircle.r = RandomRangeF(20.0f, 50.0f);
	g_sCircle.rgb = RGB(144, 238, 144);

	g_sRectangle.x = RandomRangeF(200.0f, 500.0f);	// 200～500 の間のランダムな数字.
	g_sRectangle.y = RandomRangeF(100.0f, 200.0f);
	g_sRectangle.rgb = RGB(30, 144, 255);

	g_sTriangle.x = RandomRangeF(200.0f, 500.0f);	// 200～500 の間のランダムな数字.
	g_sTriangle.y = RandomRangeF(100.0f, 200.0f);
	g_sTriangle.rgb = RGB(170, 242, 144);

}


// 各種オブジェクトの更新.
// 「コリジョン解決」のボタンを押すと、一定間隔で本関数が呼ばれます(もう一度押すと止まります).
// 呼ばれるタイミングを変えたいときは GetUpdateMsec() を変更してください(初期状態は0.1秒間隔です).
// 別に時間に合わせて処理せず、一瞬で落下して解決状態としても問題ありません.
// 物理法則に則って落下する必要もありません(それっぽい計算で十分です).
// 引数：unTimer 呼ばれる度にカウントアップします(0は初期状態).

void UpdateObject(HDC& hdc, UINT unTimer){
	bool bReset = (unTimer == 0);
	float gravity = 0.8f;
	float restitution = 0.3f; // 反発係数

	if (!bReset) {
		// 三角形の速度と位置の更新
		g_sTriangle.speed += gravity;
		g_sTriangle.y += g_sTriangle.speed;

		// 四角形の速度と位置の更新
		g_sRectangle.speed += gravity;
		g_sRectangle.y += g_sRectangle.speed;

		// 円の速度と位置の更新
		g_sCircle.speed += gravity;
		g_sCircle.y += g_sCircle.speed;

		// コップの底と図形の衝突判定
		//床に衝突したら跳ね返るように実装

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

		// コップの左右の壁と図形の衝突判定
		//少し先の値を設定
		if (g_sTriangle.x - 50.0f < g_sCupPoint[0].x) g_sTriangle.x = g_sCupPoint[0].x + 50.0f;
		if (g_sTriangle.x + 50.0f > g_sCupPoint[2].x) g_sTriangle.x = g_sCupPoint[2].x - 50.0f;
		if (g_sRectangle.x - 25.0f < g_sCupPoint[0].x) g_sRectangle.x = g_sCupPoint[0].x + 25.0f;
		if (g_sRectangle.x + 25.0f > g_sCupPoint[2].x) g_sRectangle.x = g_sCupPoint[2].x - 25.0f;
		if (g_sCircle.x - g_sCircle.r < g_sCupPoint[0].x) g_sCircle.x = g_sCupPoint[0].x + g_sCircle.r;
		if (g_sCircle.x + g_sCircle.r > g_sCupPoint[2].x) g_sCircle.x = g_sCupPoint[2].x - g_sCircle.r;


		// 円と四角形の衝突判定
		float dist_cir_rec = sqrtf(powf(g_sCircle.x - g_sRectangle.x, 2) + powf(g_sCircle.y - g_sRectangle.y, 2)); //ユークリッド距離
		if (dist_cir_rec < g_sCircle.r + 40.0f) {
			float angle = atan2f(g_sCircle.y - g_sRectangle.y, g_sCircle.x - g_sRectangle.x);

			// 円が地面に接している場合、四角形を上に押し出す
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
		// 円と三角形の衝突判定
		float dist_cir_tri = sqrtf(powf(g_sCircle.x - g_sTriangle.x, 2) + powf(g_sCircle.y - g_sTriangle.y, 2));
		if (dist_cir_tri < g_sCircle.r + 40.0f) {
			float angle = atan2f(g_sCircle.y - g_sTriangle.y, g_sCircle.x - g_sTriangle.x);

			// 円が地面に接している場合、三角形を上に押し出す
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
		// 四角形と三角形の衝突判定
		float dist_rec_tri = sqrtf(powf(g_sRectangle.x - g_sTriangle.x, 2) + powf(g_sRectangle.y - g_sTriangle.y, 2));
		if (dist_rec_tri < 80.0f) {
			float angle = atan2f(g_sRectangle.y - g_sTriangle.y, g_sRectangle.x - g_sTriangle.x);
			g_sRectangle.x = g_sTriangle.x + cosf(angle) * 80.0f;
			g_sRectangle.y = g_sTriangle.y + sinf(angle) * 80.0f;
			//g_sCircle.speed = g_sRectangle.speed;
		}

	}

	// 三角形の描画
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

	// 四角形の描画
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

	// 円の描画
	{

		RenderCircle(hdc, g_sCircle.x, g_sCircle.y, g_sCircle.r, g_sCircle.rgb);

	}
}




// コップの描画.
// こちらは変更の必要はないはず.
void RenderCup(HDC& hdc)
{
	// コップの描画
	{
		const int pointCount = sizeof(g_sCupPoint) / sizeof(g_sCupPoint[0]);
		// 描画
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

/* ▲▲▲  ▲▲▲ ここまでを編集してください ▲▲▲  ▲▲▲ */



// 乱数取得.
int Random(int nMax)
{
	return rand() % (nMax + 1);
}

// 乱数取得.
int RandomRange(int nMin, int nMax)
{
	if (nMin >= nMax) {
		TRACE(_T("最大値が最小値より大きくなるように設定してください\n"));
		ASSERT(false);
		return nMin;
	}
	UINT nRnd = static_cast<UINT>(nMax - nMin + 1);
	return (nMin + rand() % nRnd);
}

// 乱数取得.
float RandomF(float fMax)
{
	return static_cast<float>(rand() % static_cast<UINT>(fMax + 1));
}

// 乱数取得.
float RandomRangeF(float fMin, float fMax)
{
	if (fMin >= fMax) {
		TRACE(_T("最大値が最小値より大きくなるように設定してください\n"));
		ASSERT(false);
		return fMin;
	}
	UINT nRnd = static_cast<UINT>(fMax - fMin + 1);
	return static_cast<float>(fMin + rand() % nRnd);
}


// ２地点間の距離.
float CalcDistance(FXY sLhs, FXY sRhs)
{
	FXY sSub = sRhs - sLhs;
	return sqrtf(sSub.x * sSub.x + sSub.y * sSub.y);
}

// ベクトルのサイズ.
float VectorLength(FXY sXy)
{
	return sqrtf(sXy.x * sXy.x + sXy.y * sXy.y);
}

// ベクトルの差
FXY VectorSub(FXY sFrom, FXY sTo)
{
	return sTo - sFrom;
}

// ベクトル正規化.
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

// ベクトルの内積.
float Vector2DDot(FXY sLhs, FXY sRhs)
{
	return (sLhs.x * sRhs.x) + (sLhs.y * sRhs.y);
}

// ベクトルの外積.
float Vector2DCross(FXY sLhs, FXY sRhs)
{
	return (sLhs.x * sRhs.y) - (sLhs.y * sRhs.x);
}


// 回転後の座標取得.
// pOutput	回転計算後の各頂点の座標値配列.
// center	回転の中心座標.
// pPos		回転計算前の各頂点の座標値配列.
// nNum		座標値配列の個数(三角形なら3, 四角形なら4を渡す).
// fAngle	回転角度.
void GetRotPos(FXY* pOutput, FXY center, FXY* pPos, int nNum, float fAngle)
{
	if (!IS_FIT_ANGLE(fAngle)) {
		TRACE(_T("角度が0.0～360.0に収まっていません[%f]\n"), fAngle);
		ASSERT(false);	// FIT_ANGLE()を通して戻された値を次回以降も利用する必要があります.
	}
	float rad = ANGLE(fAngle);	// ラジアン.
	float cosValue = cosf(rad);
	float sinValue = sinf(rad);
	for (int i = 0; i < nNum; i++) {
		FXY sBuf = pPos[i];
		// 回転変換
		pOutput[i].x = center.x + (sBuf.x - center.x) * cosValue - (sBuf.y - center.y) * sinValue;
		pOutput[i].y = center.y + (sBuf.x - center.x) * sinValue + (sBuf.y - center.y) * cosValue;
		// 誤差は出るけど、判定が難しくなるため少数は四捨五入する.
		pOutput[i].x = roundf(pOutput[i].x);
		pOutput[i].y = roundf(pOutput[i].y);
	}
}

// 多角形の描画.
void RenderPolygon(HDC hdc, POINT* pPos, int nNum, UINT rgb)
{
	HBRUSH hBrush = CreateSolidBrush(rgb);
	HGDIOBJ hOldObj = SelectObject(hdc, hBrush);	// 塗りつぶし色設定
	SetPolyFillMode(hdc, WINDING);
	Polygon(hdc, pPos, nNum);
	SelectObject(hdc, hOldObj);	// 元の色に復帰
	DeleteObject(hBrush);
}

// 三角形の描画.
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

// 回転した三角形の描画.
// center	回転の中心.
// pPos		各頂点座標.
// fAngle	回転角(0.0～360.0).
// rgb		三角形の色.
void RenderRotTriangle(HDC hdc, FXY& center, FXY* pPos, float fAngle, UINT rgb)
{
	if (!IS_FIT_ANGLE(fAngle)) {
		TRACE(_T("角度が0.0～360.0に収まっていません[%f]\n"), fAngle);
		ASSERT(false);	// FIT_ANGLE()を通して戻された値を次回以降も利用する必要があります.
	}
	
	// 回転後の座標を取得する.
	FXY radPos[3];
	GetRotPos(radPos, center, pPos, COUNTOF(radPos), fAngle);
	
	POINT drawPoint[3];		// 角度を考慮した描画時の座標(描画APIの都合で整数になっています)
	for (int i = 0; i < 3; i++) {
		drawPoint[i] = { static_cast<LONG>(radPos[i].x), static_cast<LONG>(radPos[i].y) };
	}

	// 三角形の描画
	RenderTriangle(hdc, drawPoint, rgb);
}


// 四角形の描画.
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

// 回転した四角形の描画.
// center	回転の中心.
// pPos		各頂点座標.
// fAngle	回転角(0.0～360.0).
// rgb		四角形の色.
void RenderRotRectangle(HDC hdc, FXY& center, FXY* pPos, float fAngle, UINT rgb)
{
	if (!IS_FIT_ANGLE(fAngle)) {
		TRACE(_T("角度が0.0～360.0に収まっていません[%f]\n"), fAngle);
		ASSERT(false);	// FIT_ANGLE()を通して戻された値を次回以降も利用する必要があります.
	}
	
	// 回転後の座標を取得する.
	FXY radPos[4];	// 配列数に注意.
	GetRotPos(radPos, center, pPos, COUNTOF(radPos), fAngle);

	POINT drawPoint[4];		// 角度を考慮した描画時の座標
	for (int i = 0; i < 4; i++) {
		drawPoint[i] = { static_cast<LONG>(radPos[i].x), static_cast<LONG>(radPos[i].y) };
	}

	// 四角形の描画.
	RenderRectangle(hdc, drawPoint, rgb);
}


// 円の描画.
// 座標(x, y)に半径rの円を描画する.
void RenderCircle(HDC hdc, float x, float y, float r, UINT rgb)
{
	int nX = static_cast<int>(x);
	int nY = static_cast<int>(y);
	int nR = static_cast<int>(r);

	// 描画
	HBRUSH hBrush = CreateSolidBrush(rgb);
	HGDIOBJ hOldObj = SelectObject(hdc, hBrush);	// 塗りつぶし色設定
	Ellipse(hdc, nX - nR, nY - nR, nX + nR, nY + nR);
	SelectObject(hdc, hOldObj);	// 元の色に復帰
	DeleteObject(hBrush);
}