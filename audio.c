#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "audio.h"
#pragma comment(lib, "winmm.lib")

int CustomAudioControl()
{
  DWORD destinationIndex;
  HANDLE heapHandle;
  struct tagMIXERCONTROLA *mixerControls;
  int controlIndex;
  DWORD *pControlType;
  DWORD *pControlStruct;
  int controlFlags;
  DWORD channelCount;
  int multipleItemCount;
  HANDLE processHeap;
  int *controlDetails;
  HANDLE tempHeapHandle;
  int controlValue;
  SIZE_T controlArraySize;
  SIZE_T detailsArraySize;
  HMIXER mixerHandle;
  struct tagMIXERLINECONTROLSA mixerLineControls;
  struct tMIXERCONTROLDETAILS mixerControlDetails;
  struct tagMIXERCAPSA mixerCapabilities;
  struct tagMIXERLINEA mixerLine;

  mixerHandle = 0;
  if (!mixerGetNumDevs())
    return -1;
  if (mixerOpen(&mixerHandle, 0, 0, 0, 0))
    mixerClose(mixerHandle);
  destinationIndex = 0;
  mixerCapabilities.cDestinations = 0;
  mixerGetDevCapsA(0, &mixerCapabilities, 0x30u);
  if (!mixerCapabilities.cDestinations)
    return -1;
  while (1)
  {
    mixerLine.cbStruct = 168;
    mixerLine.dwDestination = destinationIndex;
    if (!mixerGetLineInfoA((HMIXEROBJ)mixerHandle, &mixerLine, 0) && mixerLine.dwComponentType == 4)
      break;
    if (mixerCapabilities.cDestinations <= ++destinationIndex)
      return -1;
  }
  mixerLine.cbStruct = 168;
  if (!mixerGetLineInfoA((HMIXEROBJ)mixerHandle, &mixerLine, 2u)
    && mixerLine.cControls
    && (controlArraySize = 148 * mixerLine.cControls, heapHandle = GetProcessHeap(), (mixerControls = HeapAlloc(heapHandle, 8u, controlArraySize)) != 0))
  {
    controlIndex = 0;
    mixerLineControls.dwLineID = mixerLine.dwLineID;
    mixerLineControls.dwControlID = 0;
    mixerLineControls.cControls = mixerLine.cControls;
    mixerLineControls.pamxctrl = mixerControls;
    mixerLineControls.cbStruct = 24;
    mixerLineControls.cbmxctrl = 148;
    if (mixerGetLineControlsA((HMIXEROBJ)mixerHandle, &mixerLineControls, 0) || !mixerLineControls.cControls)
      goto CLEANUP_AND_RETURN;
    pControlType = &mixerControls->dwControlType;
    while (*pControlType != 1342373889)
    {
      pControlType += 37;
      if ((DWORD)controlIndex >= mixerLineControls.cControls)
        goto CLEANUP_AND_RETURN;
      controlIndex++;
    }
    pControlStruct = &mixerControls[controlIndex].cbStruct;
    if ((pControlStruct[2] & 0x50030001) == 0)
      goto CLEANUP_AND_RETURN;
    controlFlags = pControlStruct[3];
    if (controlFlags < 0)
      goto CLEANUP_AND_RETURN;
    channelCount = mixerLine.cChannels;
    if ((controlFlags & 1) != 0)
      channelCount = 1;
    multipleItemCount = 1;
    if ((controlFlags & 2) != 0)
      multipleItemCount = pControlStruct[4];
    detailsArraySize = 4 * channelCount * multipleItemCount;
    processHeap = GetProcessHeap();
    controlDetails = HeapAlloc(processHeap, 8u, detailsArraySize);
    if (controlDetails)
    {
      mixerControlDetails.cbStruct = 24;
      mixerControlDetails.dwControlID = pControlStruct[1];
      mixerControlDetails.cChannels = 1;
      mixerControlDetails.cMultipleItems = pControlStruct[4];
      mixerControlDetails.paDetails = controlDetails;
      mixerControlDetails.cbDetails = 4;
      if (mixerGetControlDetailsA((HMIXEROBJ)mixerHandle, &mixerControlDetails, 0))
      {
        tempHeapHandle = GetProcessHeap();
        HeapFree(tempHeapHandle, 0, mixerControls);
        tempHeapHandle = GetProcessHeap();
        HeapFree(tempHeapHandle, 0, controlDetails);
        mixerClose(mixerHandle);
        return -1;
      }
      else
      {
        controlValue = *controlDetails;
        tempHeapHandle = GetProcessHeap();
        HeapFree(tempHeapHandle, 0, mixerControls);
        tempHeapHandle = GetProcessHeap();
        HeapFree(tempHeapHandle, 0, controlDetails);
        mixerClose(mixerHandle);
        return controlValue;
      }
    }
    else
    {
CLEANUP_AND_RETURN:
      tempHeapHandle = GetProcessHeap();
      HeapFree(tempHeapHandle, 0, mixerControls);
      mixerClose(mixerHandle);
      return -1;
    }
  }
  else
  {
    mixerClose(mixerHandle);
    return -1;
  }
}