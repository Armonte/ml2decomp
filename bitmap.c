#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "anim.h"
#include "main.h"
#include "bitmap.h"
#include "FileUtils.h"

DWORD __stdcall WriteBitmapToFile(LPCSTR filePath, LPCVOID buffer, LONG offset, DWORD bufferSize)
{
  DWORD bytesWritten; // edi
  HANDLE fileHandle; // eax
  void *fileObject; // ebx
  DWORD bytesLocked; // ebp
  DWORD bytesWrittenLocal; // [esp+10h] [ebp-4h] BYREF

  bytesWritten = 0;
  if ( !filePath || !buffer )
    return 0;
  if ( !bufferSize )
    return 0;
  fileHandle = CreateFileA(filePath, 0x40000000u, 0, 0, 2u, 0x80u, 0);
  fileObject = fileHandle;
  if (fileHandle == INVALID_HANDLE_VALUE)
    return 0;
  if ( LockFile(fileHandle, 0, 0, bufferSize, 0) )
  {
    bytesWritten = bufferSize;
    if ( offset && SetFilePointer(fileObject, offset, 0, 0) == -1 )
    {
      bytesLocked = 0;
    }
    else
    {
      bytesLocked = 0;
      bytesWrittenLocal = 0;
      if ( WriteFile(fileObject, buffer, bufferSize, &bytesWrittenLocal, 0) )
        bytesLocked = bytesWrittenLocal;
    }
  }
  else
  {
    bytesLocked = 0;
  }
  if ( bytesWritten && !UnlockFile(fileObject, 0, 0, bytesWritten, 0) )
    bytesLocked = 0;
  if ( !CloseHandle(fileObject) && !CloseHandle(fileObject) )
    return 0;
  return bytesLocked;
}

void processGraphicsAndBitmap()
{
  void *buffer = NULL;
  char *graphicsBuffer = NULL;
  PALETTEENTRY *paletteEntry;
  DWORD nNumberOfBytesToLockLow = 0;
  char FullPath[MAX_PATH] = {0};
  char Ext[MAX_PATH] = {0};
  char Drive[_MAX_DRIVE] = {0};
  char Dir[_MAX_DIR] = {0};
  char Filename[_MAX_FNAME] = {0};

  graphicsBuffer = CreateGraphicsBuffer(0, 0, 256, 256);
  if (graphicsBuffer)
  {
    paletteEntry = GetPaletteEntry();
    buffer = AllocateBitmapBuffer(graphicsBuffer, 256, 256, (const void*)paletteEntry, 256, -10, 0, &nNumberOfBytesToLockLow, 1);
    if (buffer)
    {
      if (FindAndLoadBitmap(hWndParent, "256 color BMP file (*.bmp)|*.bmp|", FullPath, NULL, ".BMP"))
      {
        _splitpath(FullPath, Drive, Dir, Filename, Ext);
        if (strcmp(Ext, ".BMP") == 0)
        {
          WriteBitmapToFile(FullPath, buffer, 0, nNumberOfBytesToLockLow);
        }
      }
    }
    free(graphicsBuffer);
  }
  if (buffer)
  {
    free(buffer);
  }
}


int __stdcall FindAndLoadBitmap(
        HWND windowHandle,
        const char *filter,
        char *FullPath,
        DWORD *fileHandle,
        const CHAR *defaultExtension)
{
  int success; // eax
  const char *filterText; // edi
  int i; // eax
  char currentChar; // dl
  unsigned int extensionLength; // kr08_4
  unsigned int j; // edi
  int isUpperCase; // eax
//  HANDLE fileHandle; // eax
  BOOL isFileHandleValid; // zf
  char defaultFilter[20]; // [esp+10h] [ebp-5314h] BYREF
  char Ext[256]; // [esp+24h] [ebp-5300h] BYREF
  char Drive[256]; // [esp+124h] [ebp-5200h] BYREF
  char FileTitle[256]; // [esp+224h] [ebp-5100h] BYREF
  char FilterBuffer[4096]; // [esp+324h] [ebp-5000h] BYREF
  char FileNameBuffer[4096]; // [esp+1324h] [ebp-4000h] BYREF
  CHAR CurrentDir[4096]; // [esp+2324h] [ebp-3000h] BYREF
  char Filename[4096]; // [esp+3324h] [ebp-2000h] BYREF
  char Dir[4096]; // [esp+4324h] [ebp-1000h] BYREF
  OPENFILENAMEA ofn; // Use a local variable instead of global

  strcpy(defaultFilter, "All file (*.*)|*.*|");
  if ( !GetCurrentDirectoryA(0x1000u, CurrentDir) )
    return 0;
  FileNameBuffer[0] = 0;
  filterText = filter ? filter : defaultFilter;
  strcpy(FilterBuffer, filterText);
  i = 0;
  for ( currentChar = FileTitle[strlen(FilterBuffer) + 255]; FilterBuffer[i]; ++i )
  {
    if ( FilterBuffer[i] == currentChar )
      FilterBuffer[i] = 0;
  }
  memset(&ofn, 0, sizeof(OPENFILENAMEA));
  ofn.hwndOwner = windowHandle;
  ofn.hInstance = 0;
  ofn.lpstrFilter = FilterBuffer;
  ofn.lpstrCustomFilter = 0;
  ofn.lpstrFile = FileNameBuffer;
  ofn.lStructSize = sizeof(OPENFILENAMEA);
  ofn.lpstrFileTitle = FileTitle;
  ofn.lpstrInitialDir = CurrentDir;
  ofn.lpstrTitle = 0;
  ofn.nFilterIndex = 1;
  ofn.nMaxFile = sizeof(FileNameBuffer);
  ofn.nFileOffset = 0;
  ofn.nFileExtension = 0;
  ofn.lpstrDefExt = defaultExtension;
  ofn.lCustData = 0;
  ofn.lpfnHook = 0;
  ofn.lpTemplateName = 0;
  ofn.nMaxFileTitle = sizeof(FileTitle);
  ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
  if ( !GetSaveFileNameA(&ofn) )
    return 0;
  if ( FullPath )
  {
    strcpy(FullPath, ofn.lpstrFile);
    _splitpath(FullPath, Drive, Dir, Filename, Ext);
    extensionLength = strlen(Ext) + 1;
    for (j = 0; j < (extensionLength - 1); ++j)
    {
      if ( cbMultiByte <= 1 )
        isUpperCase = *(off_4C0858 + Ext[j]) & 2;
      else
        isUpperCase = _isctype(Ext[j], 2);
      if ( isUpperCase )
        Ext[j] -= 32;
    }
    _makepath(FullPath, Drive, Dir, Filename, Ext);
  }
  if ( !fileHandle )
    return 1;
  fileHandle = CreateFileA(ofn.lpstrFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  *fileHandle = (DWORD)fileHandle;
  isFileHandleValid = fileHandle == INVALID_HANDLE_VALUE;
  success = 0;
  if ( !isFileHandleValid )
    return 1;
  return success;
}

int __cdecl CopyAndManipulateMemoryBlock(char *destBuffer, int elementCount)
{
  char *currentDest; // ebx
  int remainingElements; // ebp
  BYTE *sourcePtr; // edx
  char tempByte; // al
  unsigned int totalBytes; // [esp+10h] [ebp-8h]
  void *tempBlock; // [esp+14h] [ebp-4h]

  currentDest = destBuffer;
  if (!destBuffer)
    return -1;
  remainingElements = elementCount;
  if (elementCount < 0)
    return -1;
  if (!elementCount)
    return 0;
  totalBytes = 4 * elementCount;
  sourcePtr = malloc(totalBytes);
  if (!sourcePtr)
    return -1;
  tempBlock = sourcePtr;
  memcpy(sourcePtr, destBuffer, totalBytes);
  memset(destBuffer, 0, totalBytes);
  do
  {
    tempByte = *sourcePtr;
    sourcePtr += 4;
    currentDest[2] = tempByte;
    currentDest += 4;
    --remainingElements;
    *(currentDest - 3) = *(sourcePtr - 3);
    *(currentDest - 4) = *(sourcePtr - 2);
    *(currentDest - 1) = 0;
  }
  while (remainingElements);
  free(tempBlock);
  return 0;
}

WORD *__cdecl AllocateBitmapBuffer(
        char *sourceBuffer,
        unsigned int width,
        int height,
        const void *paletteData,
        int paletteSize,
        int colorOffset,
        unsigned int modifyZeroFlag,
        DWORD *outputSize,
        unsigned int flipFlag)
{
  char *currentSource; // ebx
  char *alignedDestination; // edx
  int rowCount; // ebp
  signed int remainingPixels; // ebp
  WORD *allocatedBuffer; // eax
  BYTE *pixelPtr; // eax
  WORD *resultBuffer; // [esp+14h] [ebp-44h]
  char *tempBuffer; // [esp+18h] [ebp-40h]
  char *alignedBuffer; // [esp+1Ch] [ebp-3Ch]
  unsigned int originalWidth; // [esp+20h] [ebp-38h]
  ULONG bmpHeaderInfo[13]; // [esp+24h] [ebp-34h] BYREF

  resultBuffer = 0;
  currentSource = sourceBuffer;
  tempBuffer = 0;
  alignedBuffer = 0;
  if ( sourceBuffer && width && height && paletteData && paletteSize && flipFlag <= 1 && modifyZeroFlag <= 1 )
  {
    originalWidth = width;
    if ( (width & 3) != 0 )
    {
      width = width - (width & 3) + 4;
      alignedBuffer = malloc(width * height);
      memset(alignedBuffer, 0, width * height);
      alignedDestination = alignedBuffer;
      if ( height > 0 )
      {
        rowCount = height;
        do
        {
          memcpy(alignedDestination, currentSource, originalWidth);
          currentSource += originalWidth;
          alignedDestination += width;
          --rowCount;
        }
        while ( rowCount );
      }
      currentSource = alignedBuffer;
    }
    if ( flipFlag == 1 )
    {
      tempBuffer = malloc(width * height);
      memcpy(tempBuffer, currentSource, width * height);
      if ( flipImageVertically(tempBuffer, width, height) )
        goto CLEANUP;
      currentSource = tempBuffer;
    }
    remainingPixels = width * height;
    bmpHeaderInfo[1] = 0;
    bmpHeaderInfo[3] = 40;
    bmpHeaderInfo[2] = 4 * paletteSize + 54;
    bmpHeaderInfo[4] = width;
    bmpHeaderInfo[5] = height;
    memset(&bmpHeaderInfo[7], 0, 16);
    bmpHeaderInfo[12] = 0;
    bmpHeaderInfo[6] = 524289;
    bmpHeaderInfo[0] = width * height + bmpHeaderInfo[2];
    bmpHeaderInfo[11] = paletteSize;
    allocatedBuffer = malloc(bmpHeaderInfo[0]);
    resultBuffer = allocatedBuffer;
    if ( allocatedBuffer )
    {
      memset(allocatedBuffer, 0, bmpHeaderInfo[0]);
      *allocatedBuffer = 19778;
      memcpy(allocatedBuffer + 1, bmpHeaderInfo, 0x34u);
      memcpy(allocatedBuffer + 27, paletteData, 4 * ((4 * paletteSize) >> 2));
      CopyAndManipulateMemoryBlock((char *)(allocatedBuffer + 54), paletteSize);
      pixelPtr = (BYTE *)((char *)resultBuffer + bmpHeaderInfo[2]);
      memcpy(resultBuffer + bmpHeaderInfo[2], currentSource, remainingPixels);
      if ( colorOffset && remainingPixels > 0 )
      {
        do
        {
          if ( modifyZeroFlag || *pixelPtr )
            *pixelPtr += colorOffset;
          ++pixelPtr;
          --remainingPixels;
        }
        while ( remainingPixels );
      }
      if ( outputSize )
        *outputSize = bmpHeaderInfo[0];
    }
  }
CLEANUP:
  if ( tempBuffer )
    free(tempBuffer);
  if ( alignedBuffer )
    free(alignedBuffer);
  return resultBuffer;
}
char *__cdecl CreateGraphicsBuffer(int startX, int startY, int width, int height)
{
  int currentY; // ebx
  char *destPtr; // edx
  char *srcPtr; // esi
  int isNegativeX; // sf
  unsigned int copyWidth; // eax
  int endX; // [esp+10h] [ebp-Ch]
  char *buffer; // [esp+14h] [ebp-8h]

  if ( isGraphicsSystemInitialized() )
    return 0;
  currentY = startY;
  if ( isRectangleWithinScreen(startX, startY, width, height) )
    return 0;
  buffer = malloc(width * height);
  if ( !buffer )
    return 0;
  memset(buffer, 0, width * height);
  destPtr = buffer;
  if ( startY < startY + height )
  {
    do
    {
      if ( currentY < 0 )
        goto ADVANCE_DEST_PTR;
      if ( currentY >= g_maxScreenHeight )
        return buffer;
      srcPtr = g_bitDepth + g_maxScreenWidth * currentY;
      isNegativeX = startX < 0;
      if ( startX > 0 )
      {
        if ( startX < g_maxScreenWidth )
          srcPtr += startX;
        isNegativeX = startX < 0;
      }
      if ( isNegativeX )
      {
        copyWidth = startX + width;
        endX = startX + width;
        if ( startX + width <= 0 )
          return buffer;
      }
      else
      {
        copyWidth = width;
        endX = startX + width;
        if ( startX + width >= g_maxScreenWidth )
          copyWidth = g_maxScreenWidth - startX;
      }
      if ( startX < 0 )
        destPtr -= startX;
      memcpy(destPtr, srcPtr, copyWidth);
      if ( startX >= 0 )
ADVANCE_DEST_PTR:
        destPtr += width;
      else
        destPtr += endX;
      ++currentY;
    }
    while ( currentY < startY + height );
  }
  return buffer;
}

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





