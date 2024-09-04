#include <windows.h>
#include <stdlib.h>
#include <string.h>

// Global variable declarations
HWND g_animationWindow;
void *g_bitDepth;
void *g_bitsBuffer;
unsigned int dword_4C0790;
int g_maxScreenWidth;
int g_maxScreenHeight;
int nBitCount;

int abs32(int n) {
    return (n < 0) ? -n : n;
}

void *__cdecl InitAnimationControl(HWND hwnd, int width, int height, int bitDepth)
{
  size_t bufferSize; // esi
  void *bitDepthBuffer; // ebp
  void *result; // eax
  unsigned int tempSize; // ebx
  void *bitsBuffer; // esi

  if ( !hwnd || width <= 0 || height <= 0 || bitDepth <= 0 )
    return 0;
  g_animationWindow = hwnd;
  bufferSize = height
     * (width * bitDepth / 8 + ((((width * bitDepth) >> 31) ^ abs32(width * bitDepth) & 7) != (width * bitDepth) >> 31));
  bitDepthBuffer = malloc(bufferSize);
  if ( !bitDepthBuffer )
    return 0;
  tempSize = bufferSize;
  memset(bitDepthBuffer, 0, bufferSize);
  if ( g_bitDepth )
    free(g_bitDepth);
  g_bitDepth = bitDepthBuffer;
  bitsBuffer = malloc(bufferSize);
  if ( !bitsBuffer )
    return 0;
  memset(bitsBuffer, 0, tempSize);
  if ( g_bitsBuffer )
    free(g_bitsBuffer);
  g_bitsBuffer = bitsBuffer;
  dword_4C0790 = tempSize;
  g_maxScreenWidth = width;
  result = g_bitDepth;
  g_maxScreenHeight = height;
  nBitCount = bitDepth;
  return result;
}