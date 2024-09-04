#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <windows.h>
#include "game.h"  // Include this to get the NCDFileEntry definition

void ReadFromFile(void);
void WriteToFile(void);
void LoadFile(void);

int FindCDDrive(void);
int CustomCharacterProcessing(unsigned char *inputChar);
int CustomDirectoryProcessing(void);

void _splitpath(const char *FullPath, char *Drive, char *Dir, char *Filename, char *Ext);
void _makepath (char *inputPath, const char *driveLetter, const char *directoryPath, const char *fileName, const char *fileExtension);
DWORD __stdcall customFileOperation(LPCSTR filePath, LPVOID buffer, DWORD offset, DWORD size);

// Update the function prototype to match the actual implementation
int __cdecl CustomDataCopy(LPCSTR lpFileName, NCDFileEntry* data);

// External variable declarations
extern DWORD dataStructurePtr;
extern BYTE unk_67A1A8[32];
extern char LIGHTS2_NCD[33056];

#endif // FILEUTILS_H
