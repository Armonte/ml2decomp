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

// Function prototype for createDefaultPalette

HPALETTE createDefaultPalette(void)
{
    // Implementation depends on your specific needs
    // This is just a placeholder
    //fun_42bb60
    return CreatePalette((LOGPALETTE*)&g_gamePaletteEntries);
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

int initGame()
{
  char customDirResult; // al
  // int processedPixelCount; // esi
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
}