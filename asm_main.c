/* int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    __asm {
        push    esi
        push    edi
        call    InitCommonControls
        lea     eax, szClass  ; Load address of szClass into eax
        push    eax           ; Push the address of szClass
        call    SetWindowToForegroundIfMinimized
        add     esp, 4
        cmp     eax, -1
        jnz     short loc_405DA4
        mov     eax, 0FFFFFFFFh
        pop     edi
        pop     esi
        retn    10h

    loc_405DA4:
        lea     edi, FullPath
        xor     eax, eax
        mov     ecx, 41h ; 'A'
        rep stosd
        call    FindFirstCDromDrive
        cmp     eax, 0FFFFFFFFh
        jz      short loc_405DD8
        mov     al, 3Ah
        mov     byte ptr [FullPath], al
        mov     al, 5Ch
        mov     byte ptr [FullPath + 1], al
        mov     al, 0
        mov     byte ptr [FullPath + 2], al

    loc_405DD8:
        mov     esi, hInstance
        call    regManSaveLoadKeyConfig
        call    regManCheckLoadSettings
        push    0               ; dwInitParam
        mov     eax, hInstance
        push    offset DialogFunc ; lpDialogFunc
        push    0               ; hWndParent
        push    offset TemplateName ; "IDD_BOOT"
        push    eax             ; hInstance
        call    DialogBoxParamA
        cmp     eax, 0FFFFFFFFh
        jnz     short loc_405E15
        mov     eax, 0FFFFFFFFh
        pop     edi
        pop     esi
        retn    10h

    loc_405E15:
        call    regManSaveSettingsAndPath
        cmp     isFullscreen, 1
        jnz     short loc_405E30
        mov     eax, ResourceHandlerState
        or      eax, 2
        mov     ResourceHandlerState, eax

    loc_405E30:
        lea     eax, szClass  ; Load address of szClass into eax
        push    eax           ; Push the address of szClass
        call    SetWindowToForegroundIfMinimized
        add     esp, 4
        cmp     eax, 0FFFFFFFFh
        jnz     short loc_405E4C
        mov     eax, 0FFFFFFFFh
        pop     edi
        pop     esi
        retn    10h

    loc_405E4C:
        mov     eax, nShowCmd
        mov     ecx, ResourceHandlerState
        mov     eax, 1
        mov     dword ptr [ResourceHandlerState], eax
        push    eax             ; nCmdShow
        push    esi             ; hInstance
        push    ecx             ; int
        call    InitWindowProc
        add     esp, 0Ch
        mov     hWndParent, eax
        test    eax, eax
        jnz     short loc_405E7B
        xor     eax, eax
        pop     edi
        pop     esi
        retn    10h

    loc_405E7B:
        cmp     isAudioEnabled, 0
        mov     hWndParent, eax
        jnz     short loc_405E95
        mov     AudioHandler, 0FFFFFFFFh
        jmp     short loc_405EA8

    loc_405E95:
        push    eax
        mov     hWndParent, eax
        call    sound_audiosetup
        add     esp, 4
        mov     AudioHandler, eax

    loc_405EA8:
        push    offset a13Zmd   ; "13.ZMD"
        call    audioSetupSong
        add     esp, 4
        call    regManScore
        call    regManSurvival
        call    regManTimeAttack
        mov     isGameStarted, 0
        call    initGame
        mov     eax, hWndParent
        push    eax             ; hWnd
        call    DestroyWindow
        xor     eax, eax
        pop     edi
        pop     esi
        retn    10h
    }

    return 0;
} */