#ifndef ANIM_H
#define ANIM_H

#include <windows.h>

// Function declarations
void* InitAnimationControl(HWND hwnd, int width, int height, int bitDepth);
int abs32(int n);

// External declarations for global variables
extern HWND g_animationWindow;
extern void* g_bitDepth;
extern void* g_bitsBuffer;
extern unsigned int dword_4C0790;
extern int g_maxScreenWidth;
extern int g_maxScreenHeight;
extern int nBitCount;

#endif // ANIM_H
