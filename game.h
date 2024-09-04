#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <stddef.h>

typedef struct {
    unsigned long offset;
    unsigned long compressionFlag;  // Changed from field1 to compressionFlag
    unsigned long compressedSize;
    unsigned long decompressedSize;
    char name[16];
} NCDFileEntry;

extern short g_globalTimerValue;  // Use 'short' instead of int16_t for VS1998
extern int g_someGameState;
extern int screenPosHorizontal;
extern char byte_6B2E65;
extern char byte_6B2F74;
extern char g_flag1;
extern char g_flag2;
extern unsigned char *g_bitDepth;
extern int g_maxScreenWidth;
extern int g_maxScreenHeight;

extern int g_maxRenderEntries;
extern int g_renderEntryCount;
extern void* g_renderBuffer;

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
int setPixel(char colorIndex, int x, int y);

int initGame(void);

int ReallocateGlobalResourceArray(int newSize);
int ReallocateRenderBuffer(int newSize);
int ReallocateMemoryBuffer(int newSize);
int CustomAudioControl(void);
void* InitAnimationControl(HWND hwnd, int width, int height, int bitDepth);
int __cdecl UpdatePaletteEntries(int startIndex, UINT entryCount, char *colorData, unsigned int colorFormat);

// New declaration for updateRenderingData
int __cdecl updateRenderingData(
    unsigned int imageId,
    int renderFlag,
    int xOffset,
    int yOffset,
    int xPos,
    int yPos,
    int width,
    int height,
    int renderType
);

// Function prototypes
int __cdecl UpdatePointerArray(unsigned int index, DWORD *newentry);
int __cdecl isRectangleWithinScreen(int posX, int posY, int width, int height);

#endif // GAME_H

