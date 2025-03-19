#include <windows.h>
#include <commctrl.h>  // For InitCommonControls()

// Global variables
char szClass[256];               
char FullPath[260];              
int isFullscreen = 0;
int ResourceHandlerState = 0;
int dword_68A3A8 = 0;
HWND GlobalWindowHandle;         
int GameAudioFlag = 1;           
int AudioHandler = -1;
char a13Zmd[64];                 
int isGameStarted = 0;

HINSTANCE AppInstanceHandle;      
char Moon_Lights_2[] = "Moon_Lights_2"; 

const CHAR IDD_BOOT[];

// Forward declaration for DialogFunc
BOOL FAR PASCAL DialogFunc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    int cdromDriveIndex;
    int dialogResult;
    int result;
    int i;  // Iterator for clearing FullPath
    DWORD dialogResource;  // Declare dialogResource here

    InitCommonControls();

    result = SetWindowToForegroundIfMinimized(Moon_Lights_2);
    if (result == -1) {
        return 0xFFFFFFFF; // Use 0xFFFFFFFF to match original assembly behavior
    }
    
    // Use memset to zero out FullPath instead of a loop
    memset(FullPath, 0, sizeof(FullPath));

    cdromDriveIndex = FindFirstCDromDrive();
    if (cdromDriveIndex != -1)
    {
        FullPath[0] = (char)(cdromDriveIndex + 'A'); // Set the drive letter
        FullPath[1] = ':';
        FullPath[2] = '\\';
        FullPath[3] = '\0';
    }

    AppInstanceHandle = hInstance;

    regManSaveLoadKeyConfig();
    regManCheckLoadSettings();

    // Assign the value to dialogResource and pass it

    dialogResult = DialogBoxParamA(AppInstanceHandle, (LPCSTR)IDD_BOOT, NULL, (DLGPROC)DialogFunc, 0);
    
    if (dialogResult == -1) {
        return 0xFFFFFFFF; // Use 0xFFFFFFFF to match original assembly behavior
    }

    regManSaveSettingsAndPath();

    if (isFullscreen == 1)
        ResourceHandlerState |= 2;

    result = SetWindowToForegroundIfMinimized(Moon_Lights_2);
    if (result == -1)
        return 0xFFFFFFFF; // Use 0xFFFFFFFF to match original assembly behavior

    dword_68A3A8 = 1;

    GlobalWindowHandle = (HWND)InitWindowProc(ResourceHandlerState, hInstance, nShowCmd);
    if (!GlobalWindowHandle)
        return 0;

    if (GameAudioFlag == 0) {
        AudioHandler = -1;
    } else {
        AudioHandler = sound_audiosetup(GlobalWindowHandle);
    }

    audioSetupSong(a13Zmd);
    regManScore();
    regManSurvival();
    regManTimeAttack();
    isGameStarted = 0;
    initGame();
    DestroyWindow(GlobalWindowHandle);

    // Use a standard WINAPI return without `ret $0x10`
    return 0; 
}


