#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <stddef.h>
#include <ddraw.h>
#include "bitmap.h"

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

extern DWORD FrameTickCount_0;
extern DWORD GameMaxTickCount;
extern DWORD FrameMaxTickCount;
extern DWORD dword_4C1A80;
extern DWORD dword_4C1A84;
extern BOOL SUPERFLASH_GRAD_BMP_FLAG;
extern BOOL IsGamePaused;
extern HDC hdc;
extern HWND hWndParent;
extern BYTE byte_43F1EC;
extern DWORD LastActionTickTime;

extern int ResourceHandlerState;
extern void* bufferPointer;
extern void* colorTable1;
extern void* colorTable2;

extern LPDIRECTDRAW lpDD;
extern void* graphicsInterface;
extern BYTE g_resourceInitCount;  // Replace byte_43F1BC
extern void* g_resourceManager;   // Replace ResourceManager
extern void* g_graphicsManager;   // Replace dword_439854
extern BOOL g_isFullscreen;       // Replace isFullscreenReg
extern BOOL g_isHighResolution;   // Replace screenResolutionFlag


extern int gamestateVariable_Unk;

extern int g_isTimerDivisionActive; 
extern int g_timerInitialValue; 
extern int g_timerDivisionFactor; 
extern int g_timerDividedValue; 
short g_timercurrentValue; // Assuming this is a short based on your usage

extern int currentSize;
extern int g_gameSpeed;
extern BYTE g_gameSpeedTable[]; // Assuming this replaces byte_43F178
extern DWORD g_gameSpeedIndex; // Assuming this replaces dword_439880
extern DWORD g_frameTickCount; // Replace FrameTickCount
extern DWORD g_frameTickCount_0; // Already declared as FrameTickCount_0
extern BOOL g_isMouseActive;
extern DWORD g_someTickCount; // Replace dword_4C166C
extern DWORD LastActionTickCount;
extern NCDFileEntry ncd_array[];

extern HPALETTE g_globalPalette;
extern PALETTEENTRY tempPalette[256]; 
extern PALETTEENTRY g_gamePaletteEntries[256];




extern unsigned char *g_memoryBuffer;
extern int g_memoryBufferSize;


typedef struct {
    DWORD reserved1;
    DWORD reserved2;
    DWORD startX;
    DWORD startY;
    DWORD width;
    DWORD height;
    DWORD color;
} RectangleParams;

typedef struct {
    char *sourceBuffer;
    char baseColor;
    int renderMode;
    int startX;
    int startY;
    int width;
    int height;
    int useDirectMemoryAccess;
} RenderPixelAreaParams;

typedef struct {
    DWORD left;
    DWORD top;
    DWORD right;
    DWORD bottom;
} BufferInfo;



int __cdecl InitializeBuffer(BufferInfo *bufferInfo, int startX, int startY, int width, int height);
int initializeResourceHandlers(void);
int isGraphicsSystemInitialized(void);
int __cdecl fillRectColorWrapper(RectangleParams *params);
int __cdecl fillRectangleWithColor(char color, int startX, int startY, unsigned int width, int height);
int setPixel(char colorIndex, int x, int y);
int MessageHandlingLoop(void);

int initGame(void);
void initAndRunGame(void);

int updateGameState1(void);
int DecrementandResetVariables(void);
int resetGlobalVariables(void);
int ClearGlobalAnimControl(void); 
int ReallocateGlobalResourceArray(int newSize);
int ReallocateRenderBuffer(int newSize);
int ReallocateMemoryBuffer(int newSize);
int CustomAudioControl(void);
void* InitAnimationControl(HWND hwnd, int width, int height, int bitDepth);
int __cdecl UpdatePaletteEntries(int startIndex, UINT entryCount, char *colorData, unsigned int colorFormat);
HPALETTE createDefaultPalette(void);
HPALETTE HandleGlobalPalette(HPALETTE inputPalette);
int __cdecl UpdatePointerArray(unsigned int index, DWORD *newentry);
int __cdecl isRectangleWithinScreen(int posX, int posY, int width, int height);
int UpdateColorInformation(void);
PALETTEENTRY *GetPaletteEntry(void);
HFONT __cdecl CreateCustomFont(HDC hdc, const char *fontName, int height, int width);
void ResetGameVariable_todo(void);
void resetResourceCounter(void);
void cleanupResources(void);
int MainGameLoop_todo(void);
DWORD setupGameEnvironment(void); 
int __cdecl InitializeWindow(HDC hdc);

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

int __cdecl renderPixelArea(
    char *sourceBuffer,
    char baseColor,
    int renderMode,
    int startX,
    int startY,
    int width,
    int height,
    int useDirectMemoryAccess
);


// Function prototype
int __cdecl renderPixelAreaWrapper(RenderPixelAreaParams *params);

#endif // GAME_H

