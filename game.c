#include <windows.h>
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
#include "resource.h"

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
  //  setupGameEnvironment();
    do
    {
     // if (frameCounter > 10 && (HandleP1Inputs() || HandleP2Inputs()))
       if (frameCounter > 10)
        break;
      ++frameCounter;
      updateRenderingData(0, (int)bitmapDataOffset, -1, 0, horizontalPadding, verticalPadding, outWidth, originalHeight, 1);
  //    MainGameLoop_todo();
  //    ClearGlobalAnimControl();
    }
    while (frameCounter < 240);
 //   g_UpdateObject();
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