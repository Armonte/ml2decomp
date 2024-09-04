#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "anim.h"
#include "main.h"
#include "bitmap.h"

DWORD __stdcall GetBitmapBufferDetails(int bitmapHandle, DWORD *width, DWORD *height)
{
  DWORD bufferDetails;
  int colorTableSize;

  colorTableSize = 0;
  if (!bitmapHandle)
    return 0;
  GetBitmapColorTableSize(bitmapHandle, &colorTableSize);
  bufferDetails = bitmapHandle + 14 + *((DWORD *)(bitmapHandle + 14)) + 4 * colorTableSize;
  if (width)
    *width = *((DWORD *)(bitmapHandle + 18));
  if (height)
    *height = *((DWORD *)(bitmapHandle + 22));
  return bufferDetails;
}

int __stdcall flipBitmapVertically(int bitmapHandle)
{
  DWORD bufferDetails;
  DWORD adjustedWidth;
  DWORD bitmapWidth;
  int bitmapHeight;

  if (!bitmapHandle)
    return -1;
  bufferDetails = GetBitmapBufferDetails(bitmapHandle, &bitmapWidth, (DWORD*)&bitmapHeight);
  if (!bufferDetails)
    return -1;
  bitmapWidth = (bitmapWidth * *((BYTE*)(bitmapHandle + 28))) / 8
              - ((((bitmapWidth * *((BYTE*)(bitmapHandle + 28))) >> 31) ^ abs((int)(bitmapWidth * *((BYTE*)(bitmapHandle + 28)))) & 7) == (bitmapWidth * *((BYTE*)(bitmapHandle + 28))) >> 31)
              + 1;
  adjustedWidth = bitmapWidth >> 2;
  if ((bitmapWidth & 3) != 0)
    ++adjustedWidth;
  return -(flipImageVertically((void*)bufferDetails, 4 * adjustedWidth, bitmapHeight) == -1);
}

int __cdecl flipImageVertically(void *imageData, int width, int height)
{
  int remainingRows;
  char *tempBuffer;
  char *sourceRow;
  char *destRow;
  char *allocatedbuffer;

  if (!imageData || !width || !height)
    return -1;

  tempBuffer = (char *)malloc(width * height);
  if (!tempBuffer)
    return -1;

  allocatedbuffer = tempBuffer;
  sourceRow = (char *)imageData + width * (height - 1);

  for (remainingRows = height; remainingRows > 0; --remainingRows)
  {
    destRow = sourceRow;
    sourceRow -= width;
    memcpy(allocatedbuffer, destRow, width);
    allocatedbuffer += width;
  }

  memcpy(imageData, tempBuffer, width * height);
  free(tempBuffer);
  return 0;
}

HPALETTE __stdcall CreateCustomPalette(int colorTableAddress, int numEntries)
{
  int colorTablePtr;
  int entriesLeft;
  LOGPALETTE *logPalette;
  LOGPALETTE *allocatedPalette;
  BYTE *palPalEntry;
  BYTE colorValue;
  HPALETTE Palette;

  colorTablePtr = colorTableAddress;
  if (!colorTableAddress)
    return 0;
  entriesLeft = numEntries;
  if (!numEntries)
    return 0;
  logPalette = (LOGPALETTE*)malloc(4 * numEntries + 8);
  allocatedPalette = logPalette;
  if (!logPalette)
    return 0;
  logPalette->palVersion = 0x300;  // 768 in hexadecimal
  logPalette->palNumEntries = numEntries;
  if (numEntries > 0)
  {
    palPalEntry = (BYTE*)logPalette->palPalEntry;
    do
    {
      colorValue = *((BYTE*)(colorTablePtr + 2));
      colorTablePtr += 4;
      *palPalEntry = colorValue;
      palPalEntry += 4;
      --entriesLeft;
      *(palPalEntry - 3) = *((BYTE*)(colorTablePtr - 3));
      *(palPalEntry - 2) = *((BYTE*)(colorTablePtr - 4));
      *(palPalEntry - 1) = 1;
    }
    while (entriesLeft);
  }
  Palette = CreatePalette(allocatedPalette);
  free(allocatedPalette);
  return Palette;
}

int __stdcall GetBitmapColorTableSize(int bitmapHandle, int *colorCount)
{
  int colorTableOffset;
  int colorTableSize;
  int bitsPerPixel;

  if ( colorCount )
    *colorCount = 0;
  if ( !bitmapHandle )
    return 0;
  colorTableSize = *((int*)(bitmapHandle + 46));
  if ( !colorTableSize )
  {
    bitsPerPixel = *((BYTE*)(bitmapHandle + 28));
    if ( bitsPerPixel == 1 )
    {
      colorTableSize = 2;
    }
    else if ( bitsPerPixel == 4 )
    {
      colorTableSize = 16;
    }
    else
    {
      colorTableSize = bitsPerPixel == 8 ? 256 : 0;
    }
    if ( !colorTableSize )
      return 0;
  }
  colorTableOffset = bitmapHandle + 54;
  if ( bitmapHandle == -54 )
    colorTableSize = 0;
  if ( colorCount )
    *colorCount = colorTableSize;
  return colorTableOffset;
}

HPALETTE __stdcall GetBitmapPalette(int bitmapHandle)
{
  int BitmapColorTableSize;
  int colorCount;

  colorCount = 0;
  if ( !bitmapHandle )
    return 0;
  BitmapColorTableSize = GetBitmapColorTableSize(bitmapHandle, &colorCount);
  if ( BitmapColorTableSize && colorCount )
    return CreateCustomPalette(BitmapColorTableSize, colorCount);
  else
    return 0;
}


BYTE* __stdcall ProcessBitmapData(
        int bitmapHandle,
        HBITMAP *outBitmap,
        HPALETTE *outPalette,
        int *outWidth,
        int *outHeight)
{
  BYTE *buffer;
  BYTE *pixelData;
  int pixelIndex;
  HBITMAP createdBitmap;
  int height;
  int width;

  if ( !bitmapHandle )
    return 0;
  if ( outPalette )
    *outPalette = GetBitmapPalette(bitmapHandle);
  flipBitmapVertically(bitmapHandle);
  buffer = (BYTE*)GetBitmapBufferDetails(bitmapHandle, (DWORD*)&width, (DWORD*)&height);
  if ( !buffer )
    return 0;
  if ( outWidth )
    *outWidth = width;
  if ( outHeight )
    *outHeight = height;
  if ( *((BYTE*)(bitmapHandle + 28)) <= 8 )
  {
    pixelData = buffer;
    for ( pixelIndex = 0; width * height > pixelIndex; ++pixelIndex )
      *pixelData++ += 10;
  }
  if ( !outBitmap )
    return buffer;
  createdBitmap = CreateBitmap(width, height, 1, *((BYTE*)(bitmapHandle + 28)), buffer);
  *outBitmap = createdBitmap;
  return createdBitmap ? buffer : 0;
}

