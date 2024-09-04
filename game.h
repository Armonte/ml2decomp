#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <stddef.h>

typedef struct {
    DWORD offset;
    DWORD type;
    DWORD compressedSize;
    DWORD uncompressedSize;
    LPCSTR filename;
} NCDFileEntry;

extern short g_globalTimerValue;  // Use 'short' instead of int16_t for VS1998
extern int g_someGameState;
extern int screenPosHorizontal;
extern char byte_6B2E65;
extern char byte_6B2F74;
extern char g_flag1;
extern char g_flag2;

extern void *GlobalResourceArray;
extern int resourceArraySize;
extern void *g_renderBuffer;
extern int g_renderBufferSize;

extern HPALETTE g_globalPalette;
extern PALETTEENTRY g_gamePaletteEntries[256];

HPALETTE createDefaultPalette(void);

HPALETTE HandleGlobalPalette(HPALETTE inputPalette);
extern NCDFileEntry ncd_array[];


// Add these declarations to game.h
extern unsigned char *g_memoryBuffer;
extern int g_memoryBufferSize;

// Function declarations
int initGame(void);
int ReallocateGlobalResourceArray(int newSize);
int ReallocateRenderBuffer(int newSize);
int ReallocateMemoryBuffer(int newSize);
int CustomAudioControl(void);
void* InitAnimationControl(HWND hwnd, int width, int height, int bitDepth);


#endif // GAME_H

