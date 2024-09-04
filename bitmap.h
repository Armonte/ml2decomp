#include "main.h"
#include "game.h"
#include "anim.h"
#include <windows.h>
#include <stddef.h>

#ifndef BITMAP_H
#define BITMAP_H


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

// Update this declaration
int __cdecl flipImageVertically(void *imageData, int width, int height);

// Keep the existing declarations


#endif // BITMAP_H