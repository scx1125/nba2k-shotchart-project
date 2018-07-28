#ifndef __D3D9RENDERER_H__
#define __D3D9RENDERER_H__

#include "hackbase.h"

class D3D9Renderer : public Renderer
{
private:
	IDirect3DDevice9 * pDevice;

	IDirect3DTexture9 *Texture_Interface;
	ID3DXSprite *Sprite_Interface;
	ID3DXLine *mLine;
	ID3DXFont *mFont;

	// ID3DXLine *mLine;
	// ID3DXFont *mFont;

	bool CreateFont();
	bool deviceLost;

public:
	D3D9Renderer(IDirect3DDevice9 *Device);
	~D3D9Renderer();
	void InitCreateFuncs();
	void BeginScene();
	void EndScene() {}
	void PreLost();
	void PostLost();
	void RefreshData(IDirect3DDevice9 *Device);
	void DrawPic(int x, int y);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawBorder(int x, int y, int w, int h, int d, Color color);
	void DrawLine(float x1, float y1, float x2, float y2, int size, bool antialias, Color color);
	void DrawTxt(int x, int y, Color FontColor, char *Text, ...);
	void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color);
	int GetWidth();
	int GetHeight();
	void loglnDebugInfo();
};

#endif