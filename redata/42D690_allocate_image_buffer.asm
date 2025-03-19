sub esp, 0xc
push ebx
push esi
push edi
push ebp
call _isGraphicsSystemInitialized
test eax, eax
jz      short .L_loc_42D6AA
xor eax, eax
pop ebp
pop edi
pop esi
pop ebx
add esp, 0xc
ret 

.L_loc_42D6AA:
mov eax, dword ptr [esp + 0x2c]
mov ecx, dword ptr [esp + 0x28]
mov ebx, dword ptr [esp + 0x24]
push eax
mov ebp, dword ptr [esp + 0x24]
push ecx
push ebx
push ebp
call _isRectangleWithinScreen
add esp, 0x10
test eax, eax
jz      short .L_loc_42D6D4
xor eax, eax
pop ebp
pop edi
pop esi
pop ebx
add esp, 0xc
ret 

.L_loc_42D6D4:
mov esi, dword ptr [esp + 0x2c]
imul esi, dword ptr [esp + 0x28]
push esi
call _malloc
mov dword ptr [esp + 0x18], eax
add esp, 4
test eax, eax
 jnz     short .L_loc_42D6F8
xor eax, eax
pop ebp
pop edi
pop esi
pop ebx
add esp, 0xc
ret 

.L_loc_42D6F8:
mov edi, dword ptr [esp + 0x14]
xor eax, eax
mov ecx, esi
shr ecx, 2
rep stosd
mov ecx, esi
and ecx, 3
rep stosb
mov edx, dword ptr [esp + 0x14]
mov eax, dword ptr [esp + 0x2c]
add eax, ebx
mov dword ptr [esp + 0x18], eax
cmp ebx, eax
jge .L_loc_42D7AD

.L_loc_42D722:
test ebx, ebx
jl      short .L_loc_42D79E
cmp ebx, dword ptr [0x6b305c]
jge     short .L_loc_42D7AD
mov ecx, ebx
mov esi, dword ptr [0x4c0788]
imul ecx, dword ptr [0x6b3060]
add esi, ecx
test ebp, ebp
jle     short .L_loc_42D74F
cmp ebp, dword ptr [0x6b3060]
jge     short .L_loc_42D74D
add esi, ebp

.L_loc_42D74D:
test ebp, ebp

.L_loc_42D74F:
jge     short .L_loc_42D763
mov ecx, dword ptr [esp + 0x28]
add ecx, ebp
mov eax, ecx
mov dword ptr [esp + 0x10], ecx
test ecx, ecx
jg      short .L_loc_42D77E
jmp     short .L_loc_42D7AD

.L_loc_42D763:
mov eax, dword ptr [esp + 0x28]
lea ecx, [ebp + eax]
mov dword ptr [esp + 0x10], ecx
cmp ecx, dword ptr [0x6b3060]
jl      short .L_loc_42D77E
mov eax, dword ptr [0x6b3060]
sub eax, ebp

.L_loc_42D77E:
test ebp, ebp
jge     short .L_loc_42D784
sub edx, ebp

.L_loc_42D784:
mov edi, edx
mov ecx, eax
shr ecx, 2
rep movsd
mov ecx, eax
and ecx, 3
rep movsb
test ebp, ebp
jge     short .L_loc_42D79E
add edx, dword ptr [esp + 0x10]
jmp     short .L_loc_42D7A2

.L_loc_42D79E:
add edx, dword ptr [esp + 0x28]


.L_loc_42D7A2:
inc ebx
cmp ebx, dword ptr [esp + 0x18]
jl      .L_loc_42D722


.L_loc_42D7AD:
mov eax, dword ptr [esp + 0x14]
pop ebp
pop edi
pop esi
pop ebx
add esp, 0xc
ret 




#include <windows.h>
extern int g_maxScreenHeight;
extern int g_maxScreenWidth;
extern int _g_bitDepth;

int  *AllocateImageBuffer(int xStart,int yStart,unsigned int width,int height)

{
  int initstatus;
  int *bufferPtr;
  unsigned int loopCounter1;
  int *currBufferPtr;
  unsigned int byteSize;
  int *srcPtr;
  int puVar1;
  int local_c;
  
  initstatus = isGraphicsSystemInitialized();
  if (initstatus != 0) {
    return (int *)0x0;
  }
  initstatus = isRectangleWithinScreen(xStart,yStart,width,height);
  if (initstatus != 0) {
    return (int *)0x0;
  }
  byteSize = height * width;
  bufferPtr = (int *)malloc(byteSize);
  if (bufferPtr == (int *)0x0) {
    return (int *)0x0;
  }
  currBufferPtr = bufferPtr;
  for (loopCounter1 = byteSize >> 2; loopCounter1 != 0; loopCounter1 = loopCounter1 - 1) {
    *currBufferPtr = 0;
    currBufferPtr = currBufferPtr + 1;
  }
  for (byteSize = byteSize & 3; byteSize != 0; byteSize = byteSize - 1) {
    *(char *)currBufferPtr = 0;
    currBufferPtr = (int *)((int)currBufferPtr + 1);
  }
  height = height + yStart;
  currBufferPtr = bufferPtr;
  if (yStart < height) {
    do {
      if (yStart < 0) {
LAB_0042d79e:
        currBufferPtr = (int *)((int)currBufferPtr + width);
      }
      else {
        if (g_maxScreenHeight <= yStart) {
          return bufferPtr;
        }
        srcPtr = (int *)(_g_bitDepth + yStart * g_maxScreenWidth);
        if ((0 < xStart) && (xStart < g_maxScreenWidth)) {
          srcPtr = (int *)((int)srcPtr + xStart);
        }
        if (xStart < 0) {
          byteSize = width + xStart;
          if ((int)(width + xStart) < 1) {
            return bufferPtr;
          }
        }
        else {
          byteSize = width;
          if (g_maxScreenWidth <= (int)(xStart + width)) {
            byteSize = g_maxScreenWidth - xStart;
          }
        }
        local_c = width + xStart;
        if (xStart < 0) {
          currBufferPtr = (int *)((int)currBufferPtr - xStart);
        }
        puVar1 = (int)currBufferPtr;
        for (loopCounter1 = byteSize >> 2; loopCounter1 != 0; loopCounter1 = loopCounter1 - 1) {
          *(int *)puVar1 = *srcPtr;
          srcPtr = srcPtr + 1;
          puVar1 = (int)(puVar1 + 4);
        }
        for (byteSize = byteSize & 3; byteSize != 0; byteSize = byteSize - 1) {
          *(char *)puVar1 = *(char *)srcPtr;
          srcPtr = (int *)((int)srcPtr + 1);
          puVar1 = puVar1 + 1;
        }
        if (-1 < xStart) goto LAB_0042d79e;
        currBufferPtr = (int *)((int)currBufferPtr + local_c);
      }
      yStart = yStart + 1;
    } while (yStart < height);
  }
  return bufferPtr;
}


