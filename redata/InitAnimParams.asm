
; =============== S U B R O U T I N E =======================================


; int __cdecl InitAnimParams(int frameCount, char isReverse, int totalFrames)
InitAnimParams  proc near               ; CODE XREF: InitAnimations+6↑p
                                        ; g_UpdateObject+7↑p

frameCount      = dword ptr  4
isReverse       = dword ptr  8
totalFrames     = dword ptr  0Ch

                cmp     g_bitDepth, 0
                jz      short loc_42F30F
                mov     ecx, [esp+totalFrames]
                cmp     ecx, 2
                jl      short loc_42F30F
                mov     eax, [esp+frameCount]
                dec     ecx
                mov     g_frameCount, eax
                mov     g_totalFrames, ecx
                mov     eax, [esp+isReverse]
                push    2
                mov     g_animationState, 1
                mov     g_currentFrame, 0
                and     eax, 1
                mov     g_isReverse, eax
                call    UpdateAnimationState
                add     esp, 4
                cmp     g_isReverse, 0
                jz      short loc_42F302
                mov     eax, g_totalFrames
                mov     g_currentFrame, eax
                xor     eax, eax
                retn
; ---------------------------------------------------------------------------

loc_42F302:                             ; CODE XREF: InitAnimParams+53↑j
                mov     g_currentFrame, 1
                xor     eax, eax
                retn
; ---------------------------------------------------------------------------

loc_42F30F:                             ; CODE XREF: InitAnimParams+7↑j
                                        ; InitAnimParams+10↑j
                mov     eax, 0FFFFFFFFh
                retn
InitAnimParams  endp

; ---------------------------------------------------------------------------
