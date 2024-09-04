#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "game.h"

#include "anim.h" // Add this line to include the anim.h header

// Update these definitions to match the declarations in anim.h
int g_currentFrame = 0;
int g_totalFrames = 0;
int g_isReverse = 0;  // Change this to int to match the declaration
int g_animationState = 0;
int g_frameCount = 0;

// Remove redundant global variable declarations
// Keep only the ones not declared in anim.h
unsigned int maxPixelCount;

// Global variable declarations
HWND g_animationWindow;
unsigned char *g_bitDepth;
void *g_bitsBuffer;
int g_maxScreenWidth;
int g_maxScreenHeight;
int nBitCount;

int MessageHandlingLoop()
{
  MSG Msg;

  while ( PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) )
  {
    if ( !GetMessage(&Msg, NULL, 0, 0) ) 
    {
      PostQuitMessage(Msg.wParam); 
      return 0; 
    }
    TranslateMessage(&Msg); 
    DispatchMessage(&Msg); 
  }
  return 0; 
}

int DecrementAndResetTimer()
{
  int timerDivisionRemaining; // edx

  if ( !g_isTimerDivisionActive )
    return -1;

  timerDivisionRemaining = g_timerDivisionFactor - 1;
  g_timercurrentValue -= g_timerDividedValue;

  if ( g_timercurrentValue < 0 )
    g_timercurrentValue = 0;

  --g_timerDivisionFactor;

  if ( !timerDivisionRemaining )
    resetGlobalVariables();

  return 0;
}

int UpdateAnimations() //add decl
{
  int result; // eax

  if ( !g_bitDepth || !g_animationState )
    return -1;

  if ( g_isReverse )
  {
    if ( g_currentFrame <= 0 )
    {
      DisableAnimation();
      return -1;
    }
  }
  else if ( g_currentFrame >= g_totalFrames )
  {
    ClearGlobalAnimControl();
    return -1;
  }

  UpdateAnimationState(2);
  result = 0;

  // Ensure proper increment/decrement based on the direction of animation
  if ( g_isReverse )
    --g_currentFrame;
  else
    ++g_currentFrame;

  return result; // Return 0 to indicate success
}

int __stdcall UpdateAnimationState(int pixelSize)
{
  int totalArea;
  int xSegmentCount;
  int ySegmentCount;
  int currentX;
  int currentY;
  int tempX;
  int tempY;
  int prevX;
  int prevY;
  int initialX;
  int initialY;
  int maxScreenX;
  int pixelsToUpdate;
  int screenWidthSegments;
  int screenHeightSegments;
  int screenXCount;
  int totalPixels;
  int processedPixels;

  if ( g_totalFrames <= 1 || g_currentFrame >= g_totalFrames )
    return -1;
  if ( !g_currentFrame )
    return -1;
  totalArea = pixelSize * pixelSize;
  if ( (unsigned int)g_totalFrames >= (unsigned int)(pixelSize * pixelSize) )
  {
    if ( g_totalFrames / totalArea )
    {
      maxScreenX = g_currentFrame / (g_totalFrames / totalArea);
    }
    else
    {
      if ( g_totalFrames - totalArea > g_currentFrame )
        return 0;
      maxScreenX = totalArea + g_currentFrame - g_totalFrames;
    }
    if ( maxScreenX >= totalArea )
      return -1;
  }
  else
  {
    maxScreenX = g_currentFrame * (totalArea / g_totalFrames);
  }
  screenXCount = ((DWORD)(g_maxScreenWidth / pixelSize) + ((g_maxScreenWidth % pixelSize) != 0));
  processedPixels = ((DWORD)(g_maxScreenHeight / pixelSize) + ((g_maxScreenHeight % pixelSize) != 0));
  if ( processedPixels > 0 )
  {
    screenWidthSegments = 0;
    do
    {
      if ( screenXCount > 0 )
      {
        xSegmentCount = 0;
        totalPixels = screenXCount;
        do
        {
          ySegmentCount = 0;
          currentX = 0;
          currentY = 0;
          pixelsToUpdate = 0;
          while ( 1 )
          {
            while ( 1 )
            {
              screenHeightSegments = pixelSize - pixelsToUpdate;
              if ( pixelSize - pixelsToUpdate <= currentX )
                break;
              ++currentY;
              setPixel(0, currentX + xSegmentCount, ySegmentCount + screenWidthSegments);
              if ( currentY >= maxScreenX )
                goto LABEL_36;
              ++currentX;
            }
            if ( currentY >= maxScreenX )
              break;
            tempX = currentX - 1;
            tempY = ySegmentCount + 1;
            if ( screenHeightSegments > tempY )
            {
              do
              {
                ++currentY;
                setPixel(0, tempX + xSegmentCount, tempY + screenWidthSegments);
                if ( currentY >= maxScreenX )
                  goto LABEL_36;
              }
              while ( screenHeightSegments > ++tempY );
            }
            if ( currentY >= maxScreenX )
              break;
            prevX = tempY - 1;
            prevY = tempX - 1;
            if ( pixelsToUpdate <= prevY )
            {
              do
              {
                ++currentY;
                setPixel(0, prevY + xSegmentCount, prevX + screenWidthSegments);
                if ( currentY >= maxScreenX )
                  goto LABEL_36;
              }
              while ( pixelsToUpdate <= --prevY );
            }
            if ( currentY >= maxScreenX )
              break;
            initialX = prevY + 1;
            initialY = prevX - 1;
            if ( ++pixelsToUpdate <= initialY )
            {
              do
              {
                ++currentY;
                setPixel(0, initialX + xSegmentCount, initialY + screenWidthSegments);
                if ( currentY >= maxScreenX )
                  goto LABEL_36;
              }
              while ( pixelsToUpdate <= --initialY );
            }
            if ( currentY >= maxScreenX )
              break;
            ySegmentCount = initialY + 1;
            currentX = initialX + 1;
          }
LABEL_36:
          xSegmentCount += pixelSize;
          --totalPixels;
        }
        while ( totalPixels );
      }
      screenWidthSegments += pixelSize;
      --processedPixels;
    }
    while ( processedPixels );
  }
  return 0;
}

int __stdcall InitAnimations(void)
{
  InitAnimParams(0, 1, 10);
  return 0;
}

int InitAnimParams(int frameCount, char isReverse, int totalFrames)
{
  if ( !g_bitDepth || totalFrames < 2 )
    return -1;
  g_frameCount = frameCount;
  g_totalFrames = totalFrames - 1;
  g_animationState = 1;
  g_currentFrame = 0;
  g_isReverse = isReverse & 1;
  UpdateAnimationState(2);
  if ( g_isReverse )
    g_currentFrame = g_totalFrames;
  else
    g_currentFrame = 1;
  return 0;
}

int DisableAnimation()
{
  int result; // eax

  if ( !g_bitDepth )
    return -1;
  result = 0;
  g_animationState = 0;
  g_frameCount = 0;
  g_isReverse = 0;
  g_totalFrames = 0;
  g_currentFrame = 0;
  return result;
}

void *InitAnimationControl(HWND hwnd, int width, int height, int bitDepth)
{
  size_t bufferSize;
  void *bitDepthBuffer;
  void *result;
  unsigned int tempSize;
  void *bitsBuffer;

  if ( !hwnd || width <= 0 || height <= 0 || bitDepth <= 0 )
    return 0;
  g_animationWindow = hwnd;
  bufferSize = height
     * (width * bitDepth / 8 + ((((width * bitDepth) >> 31) ^ abs(width * bitDepth) & 7) != (width * bitDepth) >> 31));
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
  maxPixelCount = tempSize;
  g_maxScreenWidth = width;
  result = g_bitDepth;
  g_maxScreenHeight = height;
  nBitCount = bitDepth;
  return result;
}