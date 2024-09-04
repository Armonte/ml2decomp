#include "main.h"
#include "game.h"
#include "anim.h"
#include <windows.h>
#include <stddef.h>

#ifndef BITMAP_H
#define BITMAP_H


void processGraphicsAndBitmap(void);
int isGraphicsSystemInitialized(void);
DWORD __stdcall GetBitmapBufferDetails(int bitmapHandle, DWORD *width, DWORD *height);
int __stdcall flipBitmapVertically(int bitmapHandle);
HPALETTE __stdcall CreateCustomPalette(int colorTableAddress, int numEntries);
int __stdcall GetBitmapColorTableSize(int bitmapHandle, int *colorCount);
HPALETTE __stdcall GetBitmapPalette(int bitmapHandle);
BYTE* __stdcall ProcessBitmapData(
    int bitmapHandle,
    HBITMAP *outBitmap,
    HPALETTE *outPalette,
    int *outWidth,
    int *outHeight
);

int __cdecl flipImageVertically(void *imageData, int width, int height);

// Add these declarations
char *__cdecl CreateGraphicsBuffer(int startX, int startY, int width, int height);
WORD *__cdecl AllocateBitmapBuffer(
        char *sourceBuffer,
        unsigned int width,
        int height,
        const void *paletteData,
        int paletteSize,
        int colorOffset,
        unsigned int modifyZeroFlag,
        DWORD *outputSize,
        unsigned int flipFlag);
int __stdcall FindAndLoadBitmap(
        HWND windowHandle,
        const char *filter,
        char *FullPath,
        DWORD *fileHandle,
        const CHAR *defaultExtension);
DWORD __stdcall WriteBitmapToFile(LPCSTR filePath, LPCVOID buffer, LONG offset, DWORD bufferSize);
PALETTEENTRY* GetPaletteEntry(void);

// External variables
extern HWND hWndParent;
extern WORD *allocatedBuffer;
extern WORD *resultBuffer;
extern BYTE *pixelPtr;

#endif // BITMAP_H