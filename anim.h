#ifndef ANIM_H
#define ANIM_H

#include <windows.h>

extern HWND g_animationWindow;
extern unsigned char *g_bitDepth;
extern void* g_bitsBuffer;
extern unsigned int maxPixelCount;
extern int g_maxScreenWidth;
extern int g_maxScreenHeight;
extern int nBitCount;

extern int g_currentFrame;
extern int g_totalFrames;
extern int g_isReverse;
extern int g_animationState;
extern int g_frameCount;


void* InitAnimationControl(HWND hwnd, int width, int height, int bitDepth);
int InitAnimParams(int frameCount, char isReverse, int totalFrames);
int __stdcall UpdateAnimationState(int pixelSize);
int __stdcall InitAnimations(void);
int DisableAnimation(void); 
int UpdateAnimations(void);




#endif
