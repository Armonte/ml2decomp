#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <mbstring.h>
#include "game.h"

// Add these definitions
char byte_4C0A71[259] = {0}; // Initialize with zeros or appropriate values
char LIGHTS2_NCD[33056] = {0}; // Initialize with zeros or appropriate values
BYTE unk_67A1A8[32] = {0}; // Initialize with zeros or appropriate values
DWORD dataStructurePtr = 0; // Initialize with zero or an appropriate value

NCDFileEntry ncd_array[] = {
    {0x00000020, 0, 0x000012, 0x000012, "DUMMY"},
    {0x00000063, 0, 0x000946, 0x000946, "LOGO.BMP"},
    {0x000009A9, 0, 0x0079E2, 0x0079E2, "PSEL.BMP"},
    {0x0000838B, 0, 0x0026B6, 0x0026B6, "TITLE.BMP"},
    {0x0000AA41, 0, 0x00CEF0, 0x00CEF0, "TOKYO.BMP"},
    {0x00017931, 0, 0x023752, 0x023752, "SE.NCD"},
    {0x0003B083, 0, 0x00B336, 0x00B336, "MANUALBG.BMP"},
    {0x000463B9, 0, 0x004A7A, 0x004A7A, "SIGN.BMP"},
    {0x0004AE33, 0, 0x003034, 0x003034, "GRAD.BMP"},
    {0x0004DE6A, 0, 0x002234, 0x002234, "AE.FRM"},
    {0x0005009E, 0, 0x012479, 0x012479, "AE.SP"},
    {0x00062517, 0, 0x00011B, 0x00011B, "AE.PAL"},
    {0x00062632, 0, 0x0032C4, 0x0032C4, "AE.VSE"},
    {0x000658F6, 0, 0x00D4C5, 0x00D4C5, "AE.NCD"},
    {0x00072DBE, 0, 0x003F99, 0x003F99, "AS.FRM"},
    {0x00076D57, 0, 0x012BE2, 0x012BE2, "AS.SP"},
    {0x00089939, 0, 0x000128, 0x000128, "AS.PAL"},
    {0x00089A61, 0, 0x0033E6, 0x0033E6, "AS.VSE"},
    {0x0008CE47, 0, 0x0175D8, 0x0175D8, "AS.NCD"},
    {0x000A4422, 0, 0x003DA1, 0x003DA1, "CH.FRM"},
    {0x000A81C3, 0, 0x0138FD, 0x0138FD, "CH.SP"},
    {0x000BBAC0, 0, 0x000129, 0x000129, "CH.PAL"},
    {0x000BBBE9, 0, 0x0036B2, 0x0036B2, "CH.VSE"},
    {0x000BF29B, 0, 0x017C70, 0x017C70, "CH.NCD"},
    {0x000D6F0E, 0, 0x003CF0, 0x003CF0, "FU.FRM"},
    {0x000DABFE, 0, 0x00FD54, 0x00FD54, "FU.SP"},
    {0x000EA952, 0, 0x000103, 0x000103, "FU.PAL"},
    {0x000EAA55, 0, 0x0035DA, 0x0035DA, "FU.VSE"},
    {0x000EE02F, 0, 0x013172, 0x013172, "FU.NCD"},
    {0x001011A4, 0, 0x002B45, 0x002B45, "HA.FRM"},
    {0x00103CE9, 0, 0x0122A2, 0x0122A2, "HA.SP"},
    {0x00115F8B, 0, 0x0000FE, 0x0000FE, "HA.PAL"},
    {0x00116089, 0, 0x002F58, 0x002F58, "HA.VSE"},
    {0x00118FE1, 0, 0x0188C3, 0x0188C3, "HA.NCD"},
    {0x001318A7, 0, 0x004677, 0x004677, "HI.FRM"},
    {0x00135F1E, 0, 0x014895, 0x014895, "HI.SP"},
    {0x0014A7B3, 0, 0x0000EB, 0x0000EB, "HI.PAL"},
    {0x0014A89E, 0, 0x0033AC, 0x0033AC, "HI.VSE"},
    {0x0014DC4A, 0, 0x00AAB7, 0x00AAB7, "HI.NCD"},
    {0x00158704, 0, 0x004321, 0x004321, "HO.FRM"},
    {0x0015CA25, 0, 0x0126FB, 0x0126FB, "HO.SP"},
    {0x0016F120, 0, 0x0000E8, 0x0000E8, "HO.PAL"},
    {0x0016F208, 0, 0x003895, 0x003895, "HO.VSE"},
    {0x00172A9D, 0, 0x011CF4, 0x011CF4, "HO.NCD"},
    {0x00184794, 0, 0x001E7D, 0x001E7D, "KA.FRM"},
    {0x00186611, 0, 0x00AF8D, 0x00AF8D, "KA.SP"},
    {0x0019159E, 0, 0x000118, 0x000118, "KA.PAL"},
    {0x001916B6, 0, 0x002A70, 0x002A70, "KA.VSE"},
    {0x00194126, 0, 0x013D79, 0x013D79, "KA.NCD"},
    {0x001A7EA2, 0, 0x00432C, 0x00432C, "RE.FRM"},
    {0x001AC1CE, 0, 0x0117DC, 0x0117DC, "RE.SP"},
    {0x001BD9AA, 0, 0x000144, 0x000144, "RE.PAL"},
    {0x001BDAEE, 0, 0x0039A9, 0x0039A9, "RE.VSE"},
    {0x001C1497, 0, 0x00E806, 0x00E806, "RE.NCD"},
    {0x001CFCA0, 0, 0x003FF4, 0x003FF4, "RR.FRM"},
    {0x001D3C94, 0, 0x012848, 0x012848, "RR.SP"},
    {0x001E64DC, 0, 0x00011C, 0x00011C, "RR.PAL"},
    {0x001E65F8, 0, 0x0030A6, 0x0030A6, "RR.VSE"},
    {0x001E969E, 0, 0x0182EC, 0x0182EC, "RR.NCD"},
    {0x0020198D, 0, 0x002F88, 0x002F88, "RY.FRM"},
    {0x00204915, 0, 0x012A82, 0x012A82, "RY.SP"},
    {0x00217397, 0, 0x0000AF, 0x0000AF, "RY.PAL"},
    {0x00217446, 0, 0x002E6D, 0x002E6D, "RY.VSE"},
    {0x0021A2B3, 0, 0x019A27, 0x019A27, "RY.NCD"},
    {0x00233CDD, 0, 0x003470, 0x003470, "UM.FRM"},
    {0x0023714D, 0, 0x010061, 0x010061, "UM.SP"},
    {0x002471AE, 0, 0x0000F1, 0x0000F1, "UM.PAL"},
    {0x0024729F, 0, 0x002DB8, 0x002DB8, "UM.VSE"},
    {0x0024A057, 0, 0x00AE7F, 0x00AE7F, "UM.NCD"},
    {0x00254ED9, 0, 0x00386F, 0x00386F, "US.FRM"},
    {0x00258748, 0, 0x012573, 0x012573, "US.SP"},
    {0x0026ACBB, 0, 0x0000F8, 0x0000F8, "US.PAL"},
    {0x0026ADB3, 0, 0x0031A8, 0x0031A8, "US.VSE"},
    {0x0026DF5B, 0, 0x014906, 0x014906, "US.NCD"},
    {0x00282864, 0, 0x005027, 0x005027, "MA.FRM"},
    {0x0028788B, 0, 0x01ACD2, 0x01ACD2, "MA.SP"},
    {0x002A255D, 0, 0x0000E7, 0x0000E7, "MA.PAL"},
    {0x002A2644, 0, 0x00412F, 0x00412F, "MA.VSE"},
    {0x002A6773, 0, 0x017CC8, 0x017CC8, "MA.NCD"},
    {0x002BE43E, 0, 0x007d00, 0x007d00, "NUTIT.BMP"}
};

int FindCDDrive() {
    UINT offset;
    char buf[256];
    UINT drive_type;

    // this is going to be -1 almost all the time these days
    for (offset = 0; offset < 0x1a; ++offset) {
        sprintf(buf, "%c:\\", (char)(offset + 'A'));
        drive_type = GetDriveTypeA(buf);
        if (drive_type == 5) {
            return offset;
        }
    }
    return -1;
}


int __cdecl CustomCharacterProcessing(unsigned char *inputChar)
{
  unsigned char firstChar;
  char currentDirectory[520];

  if (!inputChar)
    return -1;
  firstChar = *inputChar;
  if (*inputChar >= 0x80 || firstChar <= 0x20)
    return -1;
  if (firstChar >= 0x61 && firstChar <= 0x7A)
    return firstChar - 97;
  if (firstChar >= 0x41 && firstChar <= 0x5A)
    return firstChar - 65;
  memset(currentDirectory, 0, sizeof(currentDirectory));
  if (!GetCurrentDirectoryA(520, currentDirectory))
    return -1;
  if (currentDirectory[0] <= 32)
    return -1;
  if (currentDirectory[0] >= 0x61 && currentDirectory[0] <= 0x7A)
    return currentDirectory[0] - 97;
  if (currentDirectory[0] < 0x41 || currentDirectory[0] > 0x5A)
    return -1;
  return currentDirectory[0] - 65;
}

int CustomDirectoryProcessing()
{
  CHAR Buffer[520]; // [esp+0h] [ebp-208h] BYREF

  if ( GetCurrentDirectoryA(520u, Buffer) )
    return CustomCharacterProcessing(Buffer);
  else
    return -1;
}

void __cdecl _splitpath(const char *FullPath, char *Drive, char *Dir, char *Filename, char *Ext)
{
  const char *pathCursor; // esi
  const char *lastslashPosition; // ebx
  unsigned char *charIterator; // edi
  unsigned char currentchar; // al
  size_t copyLength; // ebp
  size_t filenameCopyLength; // ebp
  size_t extensionCopyLength; // edi
  size_t fullFilenameCopyLength; // edi
  unsigned char *Source; // [esp+10h] [ebp-4h]

  pathCursor = FullPath;
  Source = 0;
  if ( FullPath[1] == ':' )
  {
    if ( Drive )
    {
      _mbsnbcpy((unsigned char*)Drive, (const unsigned char*)FullPath, 2);
      Drive[2] = 0;
    }
    pathCursor = FullPath + 2;
  }
  else if ( Drive )
  {
    *Drive = 0;
  }
  lastslashPosition = 0;
  for ( charIterator = (unsigned char*)pathCursor; *charIterator; ++charIterator )
  {
    currentchar = *charIterator;
    if ((byte_4C0A71[*charIterator] & 4) != 0)
    {
      ++charIterator;
    }
    else if ( currentchar == '/' || currentchar == '\\' )
    {
      lastslashPosition = (const char*)(charIterator + 1);
    }
    else if ( currentchar == '.' )
    {
      Source = charIterator;
    }
  }
  if ( lastslashPosition )
  {
    if ( Dir )
    {
      copyLength = lastslashPosition - pathCursor;
      if ( (lastslashPosition - pathCursor) >= 0xFF )
        copyLength = 255;
      _mbsnbcpy((unsigned char*)Dir, (const unsigned char*)pathCursor, copyLength);
      Dir[copyLength] = 0;
    }
    pathCursor = lastslashPosition;
  }
  else if ( Dir )
  {
    *Dir = 0;
  }
  if ( Source && Source >= (unsigned char*)pathCursor )
  {
    if ( Filename )
    {
      filenameCopyLength = Source - (unsigned char*)pathCursor;
      if ( (Source - (unsigned char*)pathCursor) >= 0xFF )
        filenameCopyLength = 255;
      _mbsnbcpy((unsigned char*)Filename, (const unsigned char*)pathCursor, filenameCopyLength);
      Filename[filenameCopyLength] = 0;
    }
    if ( Ext )
    {
      extensionCopyLength = charIterator - Source;
      if ( extensionCopyLength >= 0xFF )
        extensionCopyLength = 255;
      _mbsnbcpy((unsigned char*)Ext, Source, extensionCopyLength);
      Ext[extensionCopyLength] = 0;
    }
  }
  else
  {
    if ( Filename )
    {
      fullFilenameCopyLength = charIterator - (unsigned char*)pathCursor;
      if ( fullFilenameCopyLength >= 0xFF )
        fullFilenameCopyLength = 255;
      _mbsnbcpy((unsigned char*)Filename, (const unsigned char*)pathCursor, fullFilenameCopyLength);
      Filename[fullFilenameCopyLength] = 0;
    }
    if ( Ext )
      *Ext = 0;
  }
}

void __cdecl _makepath(char *inputPath, const char *driveLetter, const char *directoryPath, const char *fileName, const char *fileExtension)
{
  char *currentPosition;
  const unsigned char *dirCursor;
  char currentChar;
  unsigned char lastDirChar;
  const char *fileNameCursor;
  char extensionChar;
  unsigned char *extensionStart;
  char *extensionCursor;
  const char *extensionSource;
  char extensionChar2;
  char *extensionDestination;

  if (driveLetter)
  {
    currentPosition = inputPath;
    if (*driveLetter)
    {
      *inputPath = *driveLetter;
      inputPath[1] = ':';
      currentPosition = inputPath + 2;
    }
  }
  else
  {
    currentPosition = inputPath;
  }

  dirCursor = (const unsigned char *)directoryPath;
  if (directoryPath && *directoryPath)
  {
    do
    {
      currentChar = *dirCursor++;
      *currentPosition++ = currentChar;
    }
    while (*dirCursor);
    lastDirChar = *_mbsdec((const unsigned char *)directoryPath, dirCursor);
    if (lastDirChar != '/' && lastDirChar != '\\')
      *currentPosition++ = '\\';
  }

  fileNameCursor = fileName;
  if (fileName && *fileName)
  {
    do
    {
      currentChar = *fileNameCursor++;
      *currentPosition++ = currentChar;
    }
    while (*fileNameCursor);
  }

  if (fileExtension)
  {
    if (*fileExtension && *fileExtension != '.')
      *currentPosition++ = '.';
    extensionChar = *fileExtension;
    extensionStart = (unsigned char *)currentPosition;
    extensionCursor = currentPosition + 1;
    extensionSource = fileExtension + 1;
    *extensionStart = *fileExtension;
    if (extensionChar)
    {
      do
      {
        extensionChar2 = *extensionSource;
        extensionDestination = extensionCursor++;
        ++extensionSource;
        *extensionDestination = extensionChar2;
      }
      while (extensionChar2);
    }
  }
  else
  {
    *currentPosition = 0;
  }
}

// Implement customFileOperation here to match the declaration in the header
DWORD __stdcall customFileOperation(LPCSTR filePath, LPVOID buffer, DWORD offset, DWORD size)
{
  HANDLE fileHandle;
  DWORD fileSize;
  BOOL operationSuccess;
  DWORD bytesRead;
  DWORD remainingBytes;
  DWORD bytesReadInOperation;
  
  remainingBytes = 0;
  if ((filePath == NULL) || (buffer == NULL)) {
    return 0;
  }
  if (size == 0) {
    return 0;
  }
  fileHandle = CreateFileA(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (fileHandle == INVALID_HANDLE_VALUE) {
    return 0;
  }
  fileSize = GetFileSize(fileHandle, NULL);
  if (fileSize == 0xFFFFFFFF) {
    bytesRead = 0;
    fileSize = remainingBytes;
  }
  else if (fileSize < offset) {
    bytesRead = 0;
    fileSize = remainingBytes;
  }
  else {
    operationSuccess = LockFile(fileHandle, 0, 0, fileSize, 0);
    if (operationSuccess == 0) {
      bytesRead = 0;
      fileSize = remainingBytes;
    }
    else {
      remainingBytes = fileSize;
      if (offset != 0) {
        DWORD newFilePointer = SetFilePointer(fileHandle, offset, NULL, FILE_BEGIN);
        if (newFilePointer == 0xFFFFFFFF && GetLastError() != NO_ERROR) {
          bytesRead = 0;
          goto EXIT_FILE_OPERATION;
        }
        remainingBytes = fileSize - offset;
      }
      if (remainingBytes < size) {
        size = remainingBytes;
      }
      bytesReadInOperation = 0;
      operationSuccess = ReadFile(fileHandle, buffer, size, &bytesReadInOperation, NULL);
      bytesRead = 0;
      if (operationSuccess != 0) {
        bytesRead = bytesReadInOperation;
      }
    }
  }
EXIT_FILE_OPERATION:
  if (fileSize != 0) {
    operationSuccess = UnlockFile(fileHandle, 0, 0, fileSize, 0);
    if (operationSuccess == 0) {
      bytesRead = 0;
    }
  }
  operationSuccess = CloseHandle(fileHandle);
  if (operationSuccess == 0) {
    operationSuccess = CloseHandle(fileHandle);
    if (operationSuccess == 0) {
      bytesRead = 0;
    }
  }
  return bytesRead;
}

int __cdecl CustomDataCopy(LPCSTR lpFileName, NCDFileEntry* data)
{
  BYTE Buffer[32]; // [esp+Ch] [ebp-20h] BYREF

  if (!lpFileName || !data)
    return -1;
  if (!customFileOperation(lpFileName, Buffer, 0, 0x20))
    return -1;
  dataStructurePtr = (DWORD)data;
  memcpy(unk_67A1A8, Buffer, 0x20);
  strcpy(LIGHTS2_NCD, lpFileName);
  return 0;
}