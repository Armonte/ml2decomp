#ifndef ANIM_H
#define ANIM_H

#include <windows.h>

// Function declarations
void* InitAnimationControl(HWND hwnd, int width, int height, int bitDepth);
int InitAnimParams(int frameCount, char isReverse, int totalFrames);

// New function declarations from anim.c
int __stdcall UpdateAnimationState(int pixelSize);
int __stdcall InitAnimations(void);

// External declarations for global variables
extern HWND g_animationWindow;
extern unsigned char *g_bitDepth;
extern void* g_bitsBuffer;
extern unsigned int dword_4C0790;
extern int g_maxScreenWidth;
extern int g_maxScreenHeight;
extern int nBitCount;

// External declarations for additional global variables
extern int g_currentFrame;
extern int g_totalFrames;
extern int g_isReverse;  // Note: It's declared as int here
extern int g_animationState;
extern int g_frameCount;

#endif // ANIM_H
