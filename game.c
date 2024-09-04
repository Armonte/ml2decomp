#include <windows.h>
#include <ddraw.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileUtils.h"
#include "anim.h"
#include "audio.h"
#include "bitmap.h"
#include "game.h"
#include "joy.h"
#include "KeyboardManager.h"
#include "main.h"
#include "winresource.h"
#include <winuser.h>

extern int g_maxScreenWidth;
extern int g_maxScreenHeight;

PALETTEENTRY g_gamePaletteEntries[256];
HPALETTE g_globalPalette;
unsigned char *g_memoryBuffer;
int g_memoryBufferSize;
void *GlobalResourceArray;
int resourceArraySize;
void *g_renderBuffer;
int g_renderBufferSize;
int screenPosHorizontal;
short g_globalTimerValue;
char g_flag2;
char g_flag1;
int g_someGameState;
char byte_6B2F74;
char byte_6B2E65;


int g_maxRenderEntries = 0;  // Initialize with an appropriate value
int g_renderEntryCount = 0;

int gamestateVariable_Unk = 0;
void* graphicsInterface = NULL;
LPDIRECTDRAW lpDD = NULL;
void* colorTable2 = NULL;
void* colorTable1 = NULL;
void* bufferPointer = NULL;
DWORD LastActionTickTime = 0;
BYTE byte_43F1EC = 0;
HDC hdc = NULL;
BOOL SUPERFLASH_GRAD_BMP_FLAG = FALSE;
DWORD dword_4C1A84 = 0;
DWORD dword_4C1A80 = 0;
DWORD FrameMaxTickCount = 0;
DWORD GameMaxTickCount = 0;
BOOL IsGamePaused = FALSE;
DWORD FrameTickCount_0 = 0;

int currentSize = 0;

// Ensure this is only defined once
#ifndef TIMER_DEFINITIONS_H
#define TIMER_DEFINITIONS_H

int g_isTimerDivisionActive = 0; 
int g_timerInitialValue = 0; 
int g_timerDivisionFactor = 0; 
int g_timerDividedValue = 0; 
short g_timercurrentValue; // Assuming this is a short based on your usage
BOOL g_isMouseActive = FALSE;
DWORD LastActionTickCount = 0;
DWORD g_frameTickCount_0 = 0;
BYTE g_gameSpeedTable[256] = {0}; // Adjust the size as needed
DWORD g_gameSpeedIndex = 0;
DWORD g_someTickCount = 0;
DWORD g_frameTickCount = 0;
int g_gameSpeed = 0;
void* g_resourceManager = NULL;
void* g_graphicsManager = NULL;
BOOL g_isHighResolution = FALSE;
BOOL g_isFullscreen = FALSE;
BYTE g_resourceInitCount = 0;


#endif // TIMER_DEFINITIONS_H

int initializeResourceHandlers()
{
    int resourceParams[27];
    int i;
    int (*initializeResources)(void*, int, int*, int, int);
    int (*finalizeResources)(void*, int);

    if (ResourceHandlerState != 3)
        return -1;

    for (i = 0; i < 27; i++)
    {
        resourceParams[i] = 0;
    }
    resourceParams[0] = 108;  /* Magic number, consider defining a constant for this */

    /* Assuming g_resourceManager is equivalent to graphicsInterface or ResourceSubHandler1 */
    initializeResources = (int (*)(void*, int, int*, int, int))(*((int**)g_resourceManager) + 25);
    finalizeResources = (int (*)(void*, int))(*((int**)g_resourceManager) + 32);

    initializeResources(g_resourceManager, 0, resourceParams, 0, 0);
    
    /* Clear the buffer pointed to by resourceParams[9] */
    if (resourceParams[9] != 0 && resourceParams[3] != 0 && resourceParams[2] != 0) {
        memset((void*)resourceParams[9], 0, resourceParams[3] * resourceParams[2]);
    }

    return finalizeResources(g_resourceManager, 0);
}

 int ProcessScreenUpdatesAndResources()
{
    POINT screenOffset = {0, 0};
    BufferInfo horizontalBuffer = {0};
    BufferInfo fullscreenBuffer = {0};
    int resourceParams[27] = {0};
    int screenParams[25] = {0};

    resourceParams[0] = 108;

    if (ResourceHandlerState != 2)
    {
        if (g_resourceInitCount < 2)
        {
            initializeResourceHandlers();
            ++g_resourceInitCount;
        }

        ((void (*)(void*, int, int*, int, int))(*((int**)g_resourceManager) + 25))(g_resourceManager, 0, resourceParams, 0, 0);
        memcpy(&resourceParams[9], &screenPosHorizontal, 0x10000);
        ((void (*)(void*, int))(*((int**)g_resourceManager) + 32))(g_resourceManager, 0);

        InitializeBuffer(&horizontalBuffer, 0, 0, 256, 240);

        if (g_isFullscreen)
        {
            if (!g_isHighResolution)
            {
                ((void (*)(void*, int, int, void*, BufferInfo*, int))(*((int**)g_graphicsManager) + 7))(g_graphicsManager, 192, 112, g_resourceManager, &horizontalBuffer, 16);
                goto FINISH_PROCESSING;
            }
            InitializeBuffer(&fullscreenBuffer, 0, 0, 640, 480);
        }
        else
        {
            if (!g_isHighResolution)
            {
                ((void (*)(void*, int, int, void*, BufferInfo*, int))(*((int**)g_graphicsManager) + 7))(g_graphicsManager, 32, 0, g_resourceManager, &horizontalBuffer, 16);
                goto FINISH_PROCESSING;
            }
            InitializeBuffer(&fullscreenBuffer, 0, 0, 320, 240);
        }

        memset(screenParams, 0, sizeof(screenParams));
        screenParams[0] = 100;
        ((void (*)(void*, BufferInfo*, void*, BufferInfo*, int, int*))(*((int**)g_graphicsManager) + 5))(g_graphicsManager, &fullscreenBuffer, g_resourceManager, &horizontalBuffer, 0x1000000, screenParams);

FINISH_PROCESSING:
        ((void (*)(void*, int, int))(*((int**)graphicsInterface) + 11))(graphicsInterface, 0, 1);
        return 0;
    }

    ClientToScreen(hWndParent, &screenOffset);
    ((void (*)(void*, int, int*, int, int))(*((int**)graphicsInterface) + 25))(graphicsInterface, 0, resourceParams, 0, 0);

    {
        char* destBuffer;
        const char* srcBuffer;
        int i;

        destBuffer = (char*)resourceParams[9] + screenOffset.x + resourceParams[4] * screenOffset.y;
        srcBuffer = (const char*)screenPosHorizontal;

        for (i = 0; i < 256; i++)
        {
            memcpy(destBuffer, srcBuffer, 0x100);
            srcBuffer = srcBuffer + 256;
            destBuffer += resourceParams[4];
        }
    }

    ((void (*)(void*, int))(*((int**)graphicsInterface) + 32))(graphicsInterface, 0);
    return 0;
} 

int __cdecl InitializeBuffer(BufferInfo *bufferInfo, int startX, int startY, int width, int height)
{
    if (!bufferInfo)
        return -1;
    
    bufferInfo->left = startX;
    bufferInfo->top = startY;
    bufferInfo->right = startX + width;
    bufferInfo->bottom = startY + height;
    
    return 0;
}

int __cdecl initializeTimerDivision(int initialValue, int divisionFactor)
{
  if ( divisionFactor < 2 )
    return -1;
  g_timerDivisionFactor = divisionFactor;
  g_isTimerDivisionActive = 1;
  g_timerInitialValue = initialValue;
  g_timercurrentValue = g_globalTimerValue;
  g_timerDividedValue = g_globalTimerValue / divisionFactor;
  return 0;
}

int resetGlobalVariables()
{
  g_isTimerDivisionActive = 0;
  g_timerInitialValue = 0;
  g_timerDivisionFactor = 0;
  g_timerDividedValue = 0;
  g_timercurrentValue = g_globalTimerValue;
  return 0;
}

int g_UpdateObject()
{
  int loopCounter; // esi
  int result; // eax

  InitAnimParams(0, 0, 10);
  loopCounter = 11;

  initializeTimerDivision(0, 10);
  LastActionTickTime = GetTickCount();
  do
  {
    MainGameLoop_todo();
    --loopCounter;
  }
  while ( loopCounter );
//  AudioCleanup();
  DisableAnimation();
  resetGlobalVariables();
  setupGameEnvironment();
  
  result = MainGameLoop_todo();
  result = result & 0xFFFF; // Extract low word from result
  // setVolume(result);
  return 0;
}

void resetResourceCounter(void)
{
    currentSize = 0;
}

void ResetGameVariable_todo(void)
{
    extern int gamestateVariable_Unk;
    gamestateVariable_Unk = 0;
}

void cleanupResources(void)
{
    memset(GlobalResourceArray, 0, 4 * ((4 * resourceArraySize) >> 2));
    ResetGameVariable_todo();
    resetResourceCounter();
}

HFONT __cdecl CreateCustomFont(HDC hdc, const char *fontName, int height, int width)
{
  int isAscii;
  HFONT createdFont;
  LOGFONT lf;

  if ( !hdc || *fontName <= 0x20 )
    return 0;
  isAscii = *fontName < 0x80;
  memset(&lf, 0, sizeof(lf));
  lf.lfHeight = height;
  lf.lfWidth = width;
  lf.lfEscapement = 0;
  lf.lfOrientation = 0;
  lf.lfItalic = 0;
  lf.lfUnderline = 0;
  lf.lfStrikeOut = 0;
  lf.lfCharSet = 0;
  lf.lfWeight = 100;
  if ( !isAscii )
    lf.lfCharSet = 0x80;
  lf.lfOutPrecision = 0;
  lf.lfClipPrecision = 0;
  lf.lfQuality = 2;
  lf.lfPitchAndFamily = 0;
  strcpy(lf.lfFaceName, fontName);
  createdFont = CreateFontIndirect(&lf);
  if ( !createdFont )
    return 0;
  SetMapperFlags(hdc, 1);
  return createdFont;
}

PALETTEENTRY *GetPaletteEntry(void)
{
  if (g_globalPalette == NULL)
    return NULL;
  else
    return &g_gamePaletteEntries[0];
}

int UpdateColorInformation()
{
  LPDIRECTDRAWPALETTE paletteHandle; // [esp+1Ch] [ebp-4h] BYREF

  if ( ResourceHandlerState == 3 )
  {
    bufferPointer = GetPaletteEntry();
    memcpy(&colorTable1, bufferPointer, 0x28u);
    memcpy(&colorTable2, bufferPointer, 0x3D8u);
    if ( lpDD->lpVtbl->CreatePalette(lpDD, DDPCAPS_8BIT, colorTable1, &paletteHandle, NULL) )
      return -1;
    ((void (__stdcall *)(void*, LPDIRECTDRAWPALETTE))(*((DWORD *)graphicsInterface + 31)))(graphicsInterface, paletteHandle);
    ((void (__stdcall *)(LPDIRECTDRAWPALETTE))(*((DWORD *)paletteHandle + 2)))(paletteHandle);
  }
  return 0;
}



int ClearGlobalAnimControl()
{
  int bytesToClear; // eax

  if ( isGraphicsSystemInitialized() )
    return -1;
  if ( (g_maxScreenWidth * g_maxScreenHeight * nBitCount) % 8 )
    bytesToClear = (g_maxScreenWidth * g_maxScreenHeight * nBitCount) / 8 + 1;
  else
    bytesToClear = (g_maxScreenWidth * g_maxScreenHeight * nBitCount) / 8;
  if ( bytesToClear )
    memset(g_bitDepth, 0, bytesToClear);
  return 0;
}

DWORD setupGameEnvironment(void)
{
  void *customFont;
  HPALETTE defaultPalette;
  DWORD currentTickCount;

  FrameTickCount_0 = 0;
  IsGamePaused = FALSE;
  GameMaxTickCount = 0;
  FrameMaxTickCount = 0;
  dword_4C1A80 = 0;
  dword_4C1A84 = 0;
  SUPERFLASH_GRAD_BMP_FLAG = FALSE;
  ClearGlobalAnimControl();
  cleanupResources();
 // ResetJoystickInfo();
  //updateKeyboardState(&keyboardStateData);
 // setVolume(g_globalTimerValue);
  if ( !hdc )
  {
    hdc = GetDC(hWndParent);
    customFont = CreateCustomFont(hdc, &byte_43F1EC, 12, 6);
    SelectObject(hdc, customFont);
    defaultPalette = createDefaultPalette();
    SelectPalette(hdc, defaultPalette, FALSE);
    RealizePalette(hdc);
  }
  UpdateColorInformation();
  currentTickCount = GetTickCount();
  LastActionTickTime = currentTickCount;
  return currentTickCount;
}

int __cdecl UpdatePointerArray(unsigned int index, DWORD *newentry)
{
  DWORD *pointerToArrayEntry;
  int result;

  if (!GlobalResourceArray || !resourceArraySize || index >= (unsigned int)resourceArraySize || !newentry)
    return -1;
  pointerToArrayEntry = (DWORD *)((char *)GlobalResourceArray + 4 * index);
  if (*pointerToArrayEntry)
    *newentry = *pointerToArrayEntry;
  result = 0;
  *pointerToArrayEntry = (DWORD)newentry;
  return result;
}

int __cdecl isRectangleWithinScreen(int posX, int posY, int width, int height)
{
  if (width > 0
    && height > 0
    && posX <= g_maxScreenWidth
    && posY <= g_maxScreenHeight
    && posX + width > 0
    && posY + height > 0)
  {
    return 0;
  }
  
  return -1;
}

int __cdecl updateRenderingData(
        unsigned int imageId,
        int renderFlag,
        int xOffset,
        int yOffset,
        int xPos,
        int yPos,
        int width,
        int height,
        int renderType)
{
    int result;
    DWORD* renderEntry;

    if (g_renderEntryCount >= g_maxRenderEntries || !renderFlag)
        return -1;

    if (isRectangleWithinScreen(xPos, yPos, width, height))
        return -1;

    renderEntry = (DWORD*)((char*)g_renderBuffer + 44 * g_renderEntryCount);
    renderEntry[0] = 0;

    if (UpdatePointerArray(imageId, renderEntry) == -1)
        return -1;

    renderEntry[1] = 0;
    renderEntry[2] = renderFlag;
    renderEntry[7] = xOffset;
    renderEntry[8] = yOffset;
    renderEntry[3] = xPos;
    renderEntry[4] = yPos;
    renderEntry[5] = width;
    renderEntry[6] = height;
    renderEntry[9] = renderType;

    g_renderEntryCount++;
    result = 0;

    return result;
}

int setPixel(char colorIndex, int x, int y)
{
  unsigned char *pixelAddress;

  if (x < 0 || y < 0 || x >= g_maxScreenWidth || y >= g_maxScreenHeight)
    return -1;

  pixelAddress = (unsigned char *)g_bitDepth + x + g_maxScreenWidth * y;
  *pixelAddress = colorIndex;
  return 0;
}
int __cdecl UpdatePaletteEntries(int startIndex, UINT entryCount, char *colorData, unsigned int colorFormat)
{
  signed int entriesToUpdate;
  PALETTEENTRY *destEntry;
  signed int i;
  char *srcColor;
  signed int j;
  char redComponent;
  PALETTEENTRY *tempPalette;

  if (colorFormat >= 2)
    return -1;
  if (startIndex >= 256)
    return 0;
  entriesToUpdate = entryCount;
  if (!entryCount)
    return 0;
  if ((startIndex + entryCount) > 256)
    entriesToUpdate = 256 - startIndex;
  tempPalette = (PALETTEENTRY *)malloc(sizeof(PALETTEENTRY) * entriesToUpdate);
  if (!tempPalette)
    return -1;
  destEntry = tempPalette;
  if (colorFormat)
  {
    srcColor = colorData;
    if (entriesToUpdate > 0)
    {
      for (j = entriesToUpdate; j > 0; j--)
      {
        redComponent = srcColor[2];
        destEntry->peRed = redComponent;
        destEntry->peGreen = srcColor[1];
        destEntry->peBlue = srcColor[0];
        destEntry->peFlags = 1;
        ++destEntry;
        srcColor += 4;
      }
    }
  }
  else
  {
    memcpy(tempPalette, colorData, sizeof(PALETTEENTRY) * entriesToUpdate);
    for (i = 0; i < entriesToUpdate; i++)
    {
      tempPalette[i].peFlags = 1;
    }
  }
  memcpy(&g_gamePaletteEntries[startIndex], tempPalette, sizeof(PALETTEENTRY) * entriesToUpdate);
  AnimatePalette(g_globalPalette, startIndex, entriesToUpdate, tempPalette);
  free(tempPalette);
  return (g_globalPalette == 0) ? -1 : 0;
}


HPALETTE createDefaultPalette()
{
  LOGPALETTE *logPalette; // esi
  int paletteentryCount; // eax
  BYTE *palPalEntry; // edi
  HPALETTE Palette; // eax

  logPalette = (LOGPALETTE *)malloc(0x408u);
  if ( !logPalette )
    return 0;
  memset(logPalette, 0, 0x408u);
  logPalette->palVersion = 0x300; // Hexadecimal for 768
  paletteentryCount = 256;
  logPalette->palNumEntries = 256;
  palPalEntry = (BYTE *)&logPalette->palPalEntry[0];
  do
  {
    *palPalEntry = 0;
    palPalEntry += 4;
    --paletteentryCount;
    *(palPalEntry - 3) = 0;
    *(palPalEntry - 2) = 0;
    *(palPalEntry - 1) = 1; // Keeping original value instead of PC_NOCOLLAPSE
  }
  while ( paletteentryCount );
  if ( !g_globalPalette )
  {
    Palette = CreatePalette(logPalette);
    if ( !Palette )
    {
      free(logPalette);
      return 0;
    }
    g_globalPalette = Palette;
  }
  free(logPalette);
  return g_globalPalette;
}

HPALETTE __cdecl HandleGlobalPalette(HPALETTE inputPalette)
{
  HPALETTE defaultPalette;
  BYTE *pPeFlags;
  PALETTEENTRY *pLastEntry;

  defaultPalette = inputPalette;
  if (!inputPalette)
  {
    defaultPalette = createDefaultPalette();
  }
  if (defaultPalette)
  {
    g_globalPalette = defaultPalette;
  }
  memset(&g_gamePaletteEntries, 0, sizeof(PALETTEENTRY) * 256);
  pPeFlags = &g_gamePaletteEntries[0].peFlags;
  pLastEntry = &g_gamePaletteEntries[255];
  do
  {
    *pPeFlags = PC_NOCOLLAPSE;
    pPeFlags += sizeof(PALETTEENTRY);
  } while (pPeFlags <= (BYTE*)pLastEntry + offsetof(PALETTEENTRY, peFlags));
  return g_globalPalette;
}

int __cdecl ReallocateMemoryBuffer(int newSize)
{
  unsigned char *newBuffer;

  if (g_memoryBuffer)
    free(g_memoryBuffer);
  g_memoryBufferSize = 0;
  newBuffer = (unsigned char *)malloc(28 * newSize);
  g_memoryBuffer = newBuffer;
  if (!newBuffer)
    return 0;
  g_memoryBufferSize = newSize;
  memset(newBuffer, 0, 28 * newSize);
  return newSize;
}

int __cdecl ReallocateGlobalResourceArray(int newSize)
{
  void *newArray;

  if (GlobalResourceArray)
    free(GlobalResourceArray);
  resourceArraySize = 0;
  newArray = malloc(4 * newSize);
  GlobalResourceArray = newArray;
  if (!newArray)
    return 0;
  resourceArraySize = newSize;
  memset(newArray, 0, 4 * newSize);
  return newSize;
}

int __cdecl ReallocateRenderBuffer(int newSize)
{
  if (g_renderBuffer)
    free(g_renderBuffer);
  g_renderBufferSize = 0;
  g_renderBuffer = malloc(44 * newSize);
  if (!g_renderBuffer)
    return 0;
  g_renderBufferSize = newSize;
  memset(g_renderBuffer, 0, 44 * newSize);
  return newSize;
}

DWORD __cdecl PerformDelayAction(int delayAmount)
{
  DWORD currentTickCount; // eax
  DWORD targetTickCount; // esi

  currentTickCount = GetTickCount();
  targetTickCount = currentTickCount + delayAmount; // Calculate target tick count
  while (currentTickCount < targetTickCount) // Use while instead of for for clarity
  {
    currentTickCount = GetTickCount(); // Update current tick count
  }
  return currentTickCount; // Return the current tick count
}

int DecrementandResetVariables()
{
  int remainingDivisions; // edx

  if ( !g_isTimerDivisionActive )
    return -1;

  remainingDivisions = g_timerDivisionFactor - 1;
  g_timercurrentValue -= g_timerDividedValue;

  if ( g_timercurrentValue < 0 )
    g_timercurrentValue = 0;

  --g_timerDivisionFactor;

  if ( !remainingDivisions )
    resetGlobalVariables();

  return 0;
}

int __cdecl fillRectColorWrapper(RectangleParams *params)
{
  return fillRectangleWithColor(
    (char)params->color,
    (int)params->startX,
    (int)params->startY,
    (unsigned int)params->width,
    (int)params->height
  );
}

int __cdecl fillRectangleWithColor(char color, int startX, int startY, unsigned int width, int height)
{
  int currentY;
  int rowStartY;
  char *screenPtr;
  int isStartXNegative;
  unsigned int copyWidth;
  int colorValue;
  unsigned int i;

  if ( isGraphicsSystemInitialized() )
    return -1;
  currentY = startY;
  if ( isRectangleWithinScreen(startX, startY, width, height) )
    return -1;
  if ( startY + height > startY )
  {
    do
    {
      if ( currentY >= 0 )
      {
        if ( currentY >= g_maxScreenHeight )
          return 0;
        rowStartY = g_maxScreenWidth * currentY;
        screenPtr = g_bitDepth + g_maxScreenWidth * currentY;
        isStartXNegative = startX < 0;
        if ( startX > 0 )
        {
          if ( startX < g_maxScreenWidth )
            screenPtr += startX;
          isStartXNegative = startX < 0;
        }
        if ( isStartXNegative )
        {
          copyWidth = startX + width;
          if ( (int)(startX + width) <= 0 )
            return 0;
        }
        else
        {
          copyWidth = width;
          if ( (startX + width) >= (unsigned int)g_maxScreenWidth )
            copyWidth = g_maxScreenWidth - startX;
        }
        ((BYTE*)&rowStartY)[0] = color;
        ((BYTE*)&rowStartY)[1] = color;
        colorValue = rowStartY << 16;
        *(WORD*)&colorValue = (WORD)rowStartY;
        
        /* Replace memset32 with a manual loop */
        for (i = 0; i < (copyWidth >> 2); i++)
        {
          *(int*)(screenPtr + i * 4) = colorValue;
        }
        
        memset(&screenPtr[4 * (copyWidth >> 2)], color, copyWidth & 3);
      }
      ++currentY;
    }
    while ( startY + height > currentY );
  }
  return 0;
}

int isGraphicsSystemInitialized()
{
  if ( g_maxScreenWidth && g_maxScreenHeight && nBitCount && g_bitDepth && g_bitsBuffer )
    return 0;
  else
    return -1;
}
int __cdecl renderPixelAreaWrapper(RenderPixelAreaParams *params)
{
    return renderPixelArea(
        params->sourceBuffer,
        params->baseColor,
        params->renderMode,
        params->startX,
        params->startY,
        params->width,
        params->height,
        params->useDirectMemoryAccess
    );
}


int __cdecl renderPixelArea(
    char *sourceBuffer,
    char baseColor,
    int renderMode,
    int startX,
    int startY,
    int width,
    int height,
    int useDirectMemoryAccess)
{
  char *renderSourceBuffer;
  char *destinationPtr;
  char *sourcePtr;
  signed int rowWidth;
  char sourcePixel;
  char *renderDestination;
  char *sourceBufferPtr;
  signed int desinationWidth;
  char sourcePixel_1;
  unsigned char *screenPtr;
  int i;
  char *screenPtrStart;
  int isStartXNegative;
  unsigned int copyWidth;
  char *temporaryBuffer;
  char *lastRowStart;
  char *currentRowStart;
  int endYCoordinate;
  int totalHeight_1;
  int currentYCoordinate;
  int currentY;
  int currentY_1;
  char *temporaryRow;

  temporaryRow = 0;
  if ( isGraphicsSystemInitialized() )
    return -1;
  if ( isRectangleWithinScreen(startX, startY, width, height) )
    return -1;
  if ( !sourceBuffer )
    return -1;
  renderSourceBuffer = sourceBuffer;
  if ( renderMode )
  {
    renderSourceBuffer = g_bitsBuffer;
    if ((unsigned int)(width * height) > maxPixelCount)

    {
      temporaryRow = (char *)malloc(width * height);
      if ( !temporaryRow )
        return -1;
      renderSourceBuffer = temporaryRow;
    }
    destinationPtr = renderSourceBuffer;
    if ( renderMode == 1 )
    {
      if ( height > 0 )
      {
        totalHeight_1 = height;
        temporaryBuffer = &sourceBuffer[width - 1];
        do
        {
          sourcePtr = temporaryBuffer;
          if ( width > 0 )
          {
            rowWidth = width;
            do
            {
              sourcePixel = *sourcePtr--;
              *destinationPtr++ = sourcePixel;
              --rowWidth;
            }
            while ( rowWidth );
          }
          temporaryBuffer += width;
          --totalHeight_1;
        }
        while ( totalHeight_1 );
      }
    }
    else if ( renderMode == 2 )
    {
      if ( height > 0 )
      {
        currentYCoordinate = height;
        lastRowStart = &sourceBuffer[width * (height - 1)];
        do
        {
          renderDestination = destinationPtr;
          destinationPtr += width;
          memcpy(renderDestination, lastRowStart, width);
          lastRowStart -= width;
          --currentYCoordinate;
        }
        while ( currentYCoordinate );
      }
    }
    else if ( renderMode == 3 && height > 0 )
    {
      currentRowStart = &sourceBuffer[width * height - 1];
      currentY = height;
      do
      {
        sourceBufferPtr = currentRowStart;
        if ( width > 0 )
        {
          desinationWidth = width;
          do
          {
            sourcePixel_1 = *sourceBufferPtr--;
            *destinationPtr++ = sourcePixel_1;
            --desinationWidth;
          }
          while ( desinationWidth );
        }
        currentRowStart -= width;
        --currentY;
      }
      while ( currentY );
    }
  }
  currentY_1 = height + startY;
  endYCoordinate = startY;
  if ( useDirectMemoryAccess )
  {
    if ( currentY_1 > startY )
    {
      do
      {
        if ( endYCoordinate < 0 )
          goto LABEL_68;
        if ( endYCoordinate >= g_maxScreenHeight )
          break;
        screenPtrStart = g_bitDepth + g_maxScreenWidth * endYCoordinate;
        isStartXNegative = startX < 0;
        if ( startX > 0 )
        {
          if ( startX < g_maxScreenWidth )
            screenPtrStart += startX;
          isStartXNegative = startX < 0;
        }
        if ( isStartXNegative )
        {
          copyWidth = startX + width;
          if ( startX + width <= 0 )
            break;
        }
        else
        {
          copyWidth = width;
          if ( (unsigned int)(startX + width) >= (unsigned int)g_maxScreenWidth )
    copyWidth = g_maxScreenWidth - startX;
        }
        if ( startX < 0 )
          renderSourceBuffer -= startX;
        memcpy(screenPtrStart, renderSourceBuffer, copyWidth);
        if ( startX >= 0 )
LABEL_68:
          renderSourceBuffer += width;
        else
          renderSourceBuffer += startX + width;
        ++endYCoordinate;
      }
      while ( currentY_1 > endYCoordinate );
    }
  }
  else if ( currentY_1 > startY )
  {
    do
    {
      if ( endYCoordinate < 0 )
      {
        renderSourceBuffer += width;
        goto SKIP_LABEL;
      }
      if ( endYCoordinate >= g_maxScreenHeight )
        break;
      screenPtr = (unsigned char *)g_bitDepth + g_maxScreenWidth * endYCoordinate;
      if ( startX > 0 && startX < g_maxScreenWidth )
        screenPtr += startX;
      for ( i = startX; i < startX + width; ++renderSourceBuffer )
      {
        if ( i >= 0 )
        {
          if ( *renderSourceBuffer )
          {
            if ( i >= g_maxScreenWidth )
            {
              renderSourceBuffer = &renderSourceBuffer[startX + width - i];
              break;
            }
            *screenPtr = baseColor + *renderSourceBuffer;
          }
          ++screenPtr;
        }
        ++i;
      }
SKIP_LABEL:
      ++endYCoordinate;
    }
    while ( currentY_1 > endYCoordinate );
  }
  if ( temporaryRow )
    free(temporaryRow);
  return 0;
}

int __cdecl InitializeWindow(HDC hdc)
{
  HDC DC; // ebp
  HDC CompatibleDC; // ebx
  HBITMAP Bitmap; // edi
  struct tagRECT Rect; // [esp+10h] [ebp-10h] BYREF

  if ( isGraphicsSystemInitialized() )
    return -1;
  GetClientRect(g_animationWindow, &Rect);
  DC = hdc;
  if ( !hdc )
    DC = GetDC(g_animationWindow);
  CompatibleDC = CreateCompatibleDC(DC);
  Bitmap = CreateBitmap(g_maxScreenWidth, g_maxScreenHeight, 1u, nBitCount, g_bitDepth);
  SelectObject(CompatibleDC, Bitmap);
  if ( Rect.right == g_maxScreenWidth && Rect.bottom == g_maxScreenHeight )
    BitBlt(DC, 0, 0, g_maxScreenWidth, g_maxScreenHeight, CompatibleDC, 0, 0, 0xCC0020u);
  else
    StretchBlt(DC, 0, 0, Rect.right, Rect.bottom, CompatibleDC, 0, 0, g_maxScreenWidth, g_maxScreenHeight, 0xCC0020u);
  if ( !hdc )
    ReleaseDC(g_animationWindow, DC);
  DeleteDC(CompatibleDC);
  DeleteObject(Bitmap);
  return 0;
}

int updateGameState1()
{
    int index;
    int successCount;
    unsigned int i;
    DWORD **currentObjectPtr;
    DWORD *currentObject;
    int result;
    int success;

    index = 0;
    successCount = 0;
    for (i = 0; i < (unsigned int)resourceArraySize; ++i)
    {
        currentObjectPtr = (DWORD **)((char *)GlobalResourceArray + index);
        for (currentObject = *currentObjectPtr; currentObject != NULL; currentObject = (DWORD *)*currentObject)
        {
            result = currentObject[1];
            if (result)
            {
                if (result == 1)
                    success = fillRectColorWrapper((RectangleParams *)currentObject);
                else
                    success = -1;
            }
            else
            {
                success = renderPixelAreaWrapper((RenderPixelAreaParams *)currentObject);
            }
            if (!success)
                ++successCount;
        }
        index += 4;
    }
    cleanupResources();
    return successCount;
}

int MainGameLoop_todo()
{
  int inactiveFlag; // eax
  int result; // eax
  DWORD gameTickCount; // edi
  DWORD tickIncrement; // ecx
  unsigned int ticksToDecrement; // eax
  unsigned int tickQuotient; // ecx
  DWORD currentTickCount; // eax

  // updateKeyboardState(&keyboardStateData);
  if ( IsGamePaused )
  {
    UpdateAnimations();
    DecrementandResetVariables();
    inactiveFlag = IsGamePaused;
    if ( IsGamePaused > g_gameSpeed )
      inactiveFlag = g_gameSpeed;
    IsGamePaused = inactiveFlag - 1;
    if ( inactiveFlag == 1 )
    {
      cleanupResources();
      LastActionTickTime = GetTickCount();
    }
    return -1;
  }
  MessageHandlingLoop();
  gameTickCount = GetTickCount();
  updateGameState1();
  if ( isGameStarted == 1 )
  {
    isGameStarted = 0;
    processGraphicsAndBitmap();
  }
  UpdateAnimations();
  DecrementandResetVariables();
  g_frameTickCount = GetTickCount();
  if ( isFullscreen )
    ProcessScreenUpdatesAndResources();

  else
    InitializeWindow(hdc);
  g_someTickCount = GetTickCount();
  gameTickCount = GetTickCount() - LastActionTickTime;
  tickIncrement = g_gameSpeedTable[g_gameSpeedIndex];
  ticksToDecrement = gameTickCount / tickIncrement;
  LastActionTickTime += tickIncrement * (gameTickCount / tickIncrement);
  if ( gameTickCount / tickIncrement )
    --ticksToDecrement;
  IsGamePaused = ticksToDecrement;
  if ( tickIncrement - 3 > gameTickCount )
  {
    g_frameTickCount_0 = gameTickCount;
    tickQuotient = tickIncrement - gameTickCount - 3;
    if ( tickQuotient > 2 )
      PerformDelayAction(tickQuotient);
    goto LABEL_18;
  }
  IsGamePaused = ticksToDecrement;
  g_frameTickCount_0 = gameTickCount;
  if ( gameTickCount < tickIncrement )
  {
LABEL_18:
    currentTickCount = GetTickCount();
    gameTickCount = g_frameTickCount_0;
    LastActionTickCount = currentTickCount;
    ticksToDecrement = IsGamePaused;
  }
  IsGamePaused = ticksToDecrement;
  if ( GameMaxTickCount < ticksToDecrement )
    GameMaxTickCount = ticksToDecrement;
  g_frameTickCount_0 = gameTickCount;
  if ( FrameMaxTickCount < gameTickCount )
    FrameMaxTickCount = gameTickCount;
  result = 0;
  g_isMouseActive = 0;
  return result;
}

void initAndRunGame()
{
  BYTE *bitmapDataOffset;
  BYTE *BitmapColorTableSize;  // Changed from int to BYTE*
  int horizontalPadding;
  int verticalPadding;
  int frameCounter;
  void *bitmapHandle;
  int originalHeight;
  int outWidth;

  bitmapHandle = ProcessAndFindMatchingEntry("logo.bmp", 0, 0xFFFFFu, 0);
  if (bitmapHandle)
  {
    bitmapDataOffset = ProcessBitmapData((int)bitmapHandle, 0, 0, &outWidth, &originalHeight);
    BitmapColorTableSize = (BYTE*)GetBitmapColorTableSize((int)bitmapHandle, 0);
    UpdatePaletteEntries(0, 0x63u, BitmapColorTableSize, 1u);
    horizontalPadding = (256 - outWidth) >> 1;
    verticalPadding = (256 - originalHeight) >> 1;
    InitAnimations();
    frameCounter = 0;
    setupGameEnvironment();
    do
    {
     // if (frameCounter > 10 && (HandleP1Inputs() || HandleP2Inputs()))
       if (frameCounter > 10)
        break;
      ++frameCounter;
      updateRenderingData(0, (int)bitmapDataOffset, -1, 0, horizontalPadding, verticalPadding, outWidth, originalHeight, 1);
      MainGameLoop_todo();
      ClearGlobalAnimControl();
    }
    while (frameCounter < 240);
    g_UpdateObject();
    free(bitmapHandle);
  }
}

int initGame()
{
  char customDirResult; // al
  int processedPixelCount; // esi
  // unsigned char *tokyoBitmapDataPtr; // eax
  // unsigned char *gradBitmapDataPtr; // eax
  // int pixelIndex; // edx
  // int gameLoopResult; // eax
  // int *resourceArrayPointer; // eax
  // short *cpuSetPointer; // eax
  // int savedOptionTimer; // esi
  // int savedRoundsToWin; // edi
  // int gameModeResult; // edi
  // int battleResult; // eax
  // short *cpuCharPointer; // ecx
  // int loopTimer; // esi
  // int resetTimer; // esi
  // int savedRounds; // edi
  // short *charMeterPointer; // eax
  // int savedRoundsTemp; // esi
  // int bitmapWidth; // [esp+10h] [ebp-528h] BYREF
  // int bitmapHeight; // [esp+14h] [ebp-524h] BYREF
  // char tempOptionDifficultyCopy; // [esp+18h] [ebp-520h]
  // int optionDifficultyCopyTemp; // [esp+20h] [ebp-518h]
  char directoryPath[260]; // [esp+24h] [ebp-514h] BYREF
  char drivePath[260]; // [esp+128h] [ebp-410h] BYREF
  char fullPathBuffer[260]; // [esp+22Ch] [ebp-30Ch] BYREF
  char fileName[260]; // [esp+330h] [ebp-208h] BYREF
  char extension[260]; // [esp+434h] [ebp-104h] BYREF
  const char VersionString[] = "Game System Character Ver 1.01\n";

  printf("%s", VersionString);
  customDirResult = CustomDirectoryProcessing();
  byte_6B2E65 = 1;
  byte_6B2F74 = customDirResult;
  _splitpath(FullPath, drivePath, directoryPath, fileName, extension); 
  _makepath(fullPathBuffer, drivePath, directoryPath, "lights2", ".ncd");
  
  CustomDataCopy(fullPathBuffer, ncd_array);
  // Reset game state variables
  g_someGameState = 0;
  g_flag1 = 0;
  g_flag2 = 0;
  g_globalTimerValue = (short)CustomAudioControl();
  
  // Initialize animation control and set horizontal screen position
  screenPosHorizontal = (int)InitAnimationControl(hWndParent, 256, 256, 8);
  
  ReallocateGlobalResourceArray(17);
  ReallocateRenderBuffer(600);
  ReallocateMemoryBuffer(32);
  return HandleGlobalPalette(NULL) != NULL;
  processedPixelCount = 0;
  initAndRunGame();
}