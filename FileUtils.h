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

// New function declarations
void __cdecl convertToUppercaseShiftJIS(unsigned char *inputString);
void *__cdecl ProcessAndFindMatchingEntry(
    const char *fileName,
    unsigned int fileOffset,
    unsigned long requestedSize,
    unsigned long *processedSize
);

// Add this new declaration
void* __stdcall ReadPortionOfFile(LPCSTR fileName, DWORD startOffset, DWORD bytesToRead, DWORD* bytesRead);

// Updated declaration for DecompressData
int __cdecl DecompressData(int inputSize, unsigned char *inputData, char *decompressedData, unsigned int decompressStartIndex, int decompressLength);

// External variable declarations
extern DWORD dataStructurePtr;
extern BYTE unk_67A1A8[32];
extern char LIGHTS2_NCD[33056];
extern char byte_4C0A71[259];
extern int cbMultiByte;
extern unsigned short* off_4C0858;

// New declarations
void *__cdecl PerformSpecialDataProcessing(
    unsigned char *sourceData,
    int operationType,
    int compressedSize,
    unsigned long decompressedSize,
    unsigned long offset,
    unsigned long requestedSize,
    unsigned long *processedSizeOutput
);

int __cdecl HandleDataProcessing(
    unsigned char *sourceData,
    int dataType,
    int decompressionType,
    unsigned long dataSize,
    void *destination,
    unsigned int offset,
    int length
);

int __cdecl DecompressData(
    int inputSize,
    unsigned char *inputData,
    char *decompressedData,
    unsigned int decompressStartIndex,
    int decompressLength
);

#endif // FILEUTILS_H
