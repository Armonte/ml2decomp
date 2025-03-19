push ebx
mov edx, dword ptr [_currentPlayerIndex]
push esi
push edi
push ebp
lea eax, [edx*8]
xor edi, edi
lea eax, [eax + eax*8]
sub eax, edx
xor ebx, ebx
lea ecx, [eax*4]
mov ax, word ptr [ecx + _playerInputStateArray]
mov di, ax
mov bx, ax
and edi, 0x20
and ebx, 0x40
cmp word ptr [ecx + _PlayerDataArrayPtr], 0
jz short .L_loc_401A35
push 0xc
mov eax, dword ptr [edx*4 + _playerDataPointers]
push offset _press_A
push eax
call checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401A0C
or edi, 1



.L_loc_401A0C:
push 0xc
mov eax, dword ptr [_currentPlayerIndex]
push offset _press_B
mov ecx, dword ptr [eax*4 + _playerDataPointers]
push ecx
call _checkBytePattern
add esp, 0xc
mov edx, dword ptr [_currentPlayerIndex]
test eax, eax
jnz short .L_loc_401A35
or ebx, 1

.L_loc_401A35:
xor esi, esi
test edi, edi
jz short .L_loc_401A44
test ebx, ebx
jz short .L_loc_401A44
mov esi, 1

.L_loc_401A44:
push 3
mov eax, dword ptr [edx*4 + _playerDataPointers]
push offset _pat_C1
push eax
call _CheckBytePatternWrapper
add esp, 0xc
test eax, eax
jnz short .L_loc_401A64
mov esi, 1

.L_loc_401A64:
push 3
mov eax, dword ptr [_currentPlayerIndex]
push offset _pat_C2
mov ecx, dword ptr [eax*4 + _playerDataPointers]
push ecx
call _CheckBytePatternWrapper
add esp, 0xc
test eax, eax
jnz short .L_loc_401A89
mov esi, 1

.L_loc_401A89:
cmp dword ptr [esp + 0x18], 0
jnz .L_loc_401E30
mov ecx, dword ptr [_currentPlayerIndex]
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
cmp word ptr [eax*4 + _p1_meter_Stocks], 0
jz short .L_loc_401B22
mov eax, dword ptr [esp + _specialMoveType]
push 0x28
shl eax, 4
mov ecx, dword ptr [ecx*4 + _playerDataPointers]
add eax, offset _motion_236236
push eax
push ecx
call _checkBytePattern
add esp, 0xc
mov ecx, dword ptr [_currentPlayerIndex]
test eax, eax
jnz short .L_loc_401B22
test edi, edi
jz short .L_loc_401AFE
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0xf
ret 


.L_loc_401AFE:
test ebx, ebx
jz short .L_loc_401B22
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x10
ret 

.L_loc_401B22:
mov edx, 0x10
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
cmp word ptr [eax*4 + _gameplayflags_array], 0
jz short .L_loc_401B43
mov edx, 8

.L_loc_401B43:
mov ebp, dword ptr [esp + _specialMoveType]
push edx
shl ebp, 4
mov ecx, dword ptr [ecx*4 + _playerDataPointers]
lea eax, [ebp + 0x4b4658]
push eax
push ecx
call _checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401BE4
mov ecx, dword ptr [_currentPlayerIndex]
test esi, esi
jz short .L_loc_401B9C
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
shl eax, 2
cmp word ptr [eax + _p1_meter_Stocks], 0
jz short .L_loc_401B9C
mov word ptr [eax + _PlayerActionArray], 1
mov eax, 0x20
pop ebp
pop edi
pop esi
pop ebx
ret 

.L_loc_401B9C:
test edi, edi
jz short .L_loc_401BC0
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x1f
ret 


.L_loc_401BC0:
test ebx, ebx
jz short .L_loc_401BEA
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x1e
ret 

.L_loc_401BE4:
mov ecx, dword ptr [_currentPlayerIndex]

.L_loc_401BEA:
push 0x10
lea eax, [ebp + motion_236_214]
push eax
mov ecx, dword ptr [ecx*4 + _playerDataPointers]
push ecx
call _checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401C85
mov ecx, dword ptr [_currentPlayerIndex]
test esi, esi
jz short .L_loc_401C3D
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
shl eax, 2
cmp word ptr [eax + _p1_meter_Stocks], 0
jz short .L_loc_401C3D
mov word ptr [eax + _PlayerActionArray], 1
mov eax, 0x21
pop ebp
pop edi
pop esi
pop ebx
ret 


.L_loc_401C3D:
test edi, edi
jz short .L_loc_401C61
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x17
ret 


.L_loc_401C61:
test ebx, ebx
jz short .L_loc_401C8B
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x16
ret 


.L_loc_401C85:
mov ecx, dword ptr [_currentPlayerIndex]


.L_loc_401C8B:
push 0x10
lea eax, [ebp + _motion_214_236_632_412]
push eax
mov ecx, dword ptr [ecx*4 + _playerDataPointers]
push ecx
call _checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401D26
mov ecx, dword ptr [_currentPlayerIndex]
test esi, esi
jz short .L_loc_401CDE
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
shl eax, 2
cmp word ptr [eax + _p1_meter_Stocks], 0
jz short .L_loc_401CDE
mov word ptr [eax + _PlayerActionArray], 1
mov eax, 0x22
pop ebp
pop edi
pop esi
pop ebx
ret 


.L_loc_401CDE:
test edi, edi
jz short .L_loc_401D02
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x13
ret 


.L_loc_401D02:
test ebx, ebx
jz short .L_loc_401D2C
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x12
ret 


.L_loc_401D26:
mov ecx, dword ptr [_currentPlayerIndex]

.L_loc_401D2C:
push 0xc
mov eax, dword ptr [ecx*4 + _playerDataPointers]
push offset _motion_22
push eax
call _checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401DC5
mov ecx, dword ptr [_currentPlayerIndex]
test esi, esi
jz short .L_loc_401D7D
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
shl eax, 2
cmp word ptr [eax + _p1_meter_Stocks], 0
jz short .L_loc_401D7D
mov word ptr [eax + _PlayerActionArray], 1
mov eax, 0x30
pop ebp
pop edi
pop esi
pop ebx
ret 

.L_loc_401D7D:
test edi, edi
jz short .L_loc_401DA1
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x2f
ret 


.L_loc_401DA1
test ebx, ebx
jz short .L_loc_401DCB
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x2e
ret 


.L_loc_401DC5:
mov ecx, dword ptr [_currentPlayerIndex]


.L_loc_401DCB:
push 5
lea eax, [ebp + 0x4b4698]
push eax
mov ecx, dword ptr [ecx*4 + _playerDataPointers]
push ecx
call _checkBytePattern
add esp, 0xc
test eax, eax
jne 0x442
mov eax, 0x25
pop ebp
pop edi
pop esi
pop ebx
ret 


.L_loc_401DF2:
push 5
add ebp, offset _motion_656_454
push ebp
mov eax, dword ptr [_currentPlayerIndex]
mov ecx, dword ptr [eax*4 + _playerDataPointers]
push ecx
call _checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401E1E
mov eax, 0xd7
pop ebp
pop edi
pop esi
pop ebx
ret 


.L_loc_401E1E:
test esi, esi
jz .L_loc_401ED7
mov eax, 0xe
pop ebp
pop edi
pop esi
pop ebx
ret 


.L_loc_401E30:
mov eax, dword ptr [esp + _specialMoveType]
push 0x1e
shl eax, 4
mov ecx, dword ptr [_currentPlayerIndex]
add eax, offset motion_236_214
mov edx, dword ptr [ecx*4 + _playerDataPointers]
push eax
push edx
call _checkBytePattern
add esp, 0xc
test eax, eax
jnz short .L_loc_401ED7
mov ecx, dword ptr [_currentPlayerIndex]
test esi, esi
jz short .L_loc_401E8F
lea eax, [ecx*8]
lea eax, [eax + eax*8]
sub eax, ecx
shl eax, 2
cmp word ptr [eax + _p1_meter_Stocks], 0
jz short .L_loc_401E8F
mov word ptr [eax + _PlayerActionArray], 1
mov eax, 0x1d
pop ebp
pop edi
pop esi
pop ebx
ret 


.L_loc_401E8F:
test edi, edi
jz short .L_loc_401EB3
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x1c
ret 


.L_loc_401EB3:
test ebx, ebx
jz short .L_loc_401ED7
lea eax, [ecx*8]
pop ebp
pop edi
pop esi
lea eax, [eax + eax*8]
pop ebx
sub eax, ecx
mov word ptr [eax*4 + _PlayerActionArray], 1
mov eax, 0x1b
ret 


.L_loc_401ED7:
mov eax, 0xffffffff
pop ebp
pop edi
pop esi
pop ebx
ret 
