#include <windows.h>
#include <ddraw.h>
#include <commctrl.h>
#include <stdio.h>
#include <dsound.h>
#include "reg.h"
#include "FileUtils.h"
#include "main.h"
#include "game.h"
#include <mmsystem.h>
#include "winresource.h"
#include <shlobj.h>
#define MAX_OFNA_STRUCTS 5
// Declare the missing variables
char szClass[] = "Moon Lights 2";
char FullPath[260];
char byte_687861[3]; // Assuming this is a 3-character array based on usage
int isFullscreen = 0;
int ResourceHandlerState = 0;
int dword_68A3A8 = 0;
int isAudioEnabled = 0;
int AudioHandler = 0;
int isGameStarted = 0;
LPCSTR BITDEPTH_ERROR_EN = "This game is for 256-color mode only.\nFrom Start -> Settings -> Control Panel -> Display Properties -> Display Details -> Set the color palette section to 256 colors.\nPlease reboot your machine";
LPCSTR BITDEPTH_ERROR_JP = "This game is for 256-color mode only.\nFrom Start -> Settings -> Control Panel -> Display Properties -> Display Details -> Set the color palette section to 256 colors.\nPlease reboot your machine";

extern LPDIRECTDRAW lpDD;

extern LPDIRECTDRAWSURFACE g_backBuffer;

extern LPDIRECTDRAWCLIPPER g_ddClipper;

extern int isFullscreenReg;

extern struct tagOFNA unk_6472F0[MAX_OFNA_STRUCTS];
extern struct tagOFNA stru_647310;
extern DWORD dword_4C0658;
extern LPDIRECTSOUND ppDS;

int currentGameType = 0;
BOOL isSaveRequested = FALSE;
BOOL isWindowActive = FALSE;

const char* IconName = "ICON";
const char* Caption = "Moon Lights 2";
const char* pszDriver = "Display";
void* unk_4C1560;
const char* byte_439938 = "DirectDraw initialization failed";
const char* byte_4398E0 = "DirectDraw fullscreen initialization failed";
const char* Text = "Color depth error";

HINSTANCE hInstance;
int isFullscreenReg = 0;
int dword_439878 = 0;
int screenResolutionFlag = 0;
int settings_sound_effects = 0;
int dword_439884 = 0;
int dword_439880 = 0;

int colorDepth;
int screenWidth;
int screenHeight;

GameSettings g_gameSettings;

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "ole32.lib")  // For CoTaskMemFree
#pragma comment(lib, "ddraw.lib")

LPDIRECTSOUND ppDS = NULL;
DWORD dword_4C0658 = 0;
struct tagOFNA unk_6472F0[MAX_OFNA_STRUCTS];
struct tagOFNA stru_647310;

LPDIRECTDRAWCLIPPER g_ddClipper = NULL;
LPDIRECTDRAWSURFACE g_backBuffer = NULL;

int BrowseAndSetFolderPath(HWND hwndParent, char* folderPathBuffer)
{
    BROWSEINFO bi = {0};
    LPITEMIDLIST pidl;
    
    bi.hwndOwner = hwndParent;
    bi.pszDisplayName = folderPathBuffer;
    bi.lpszTitle = "Select a folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS;

    pidl = SHBrowseForFolder(&bi);
    
    if (pidl != NULL)
    {
        SHGetPathFromIDList(pidl, folderPathBuffer);
        CoTaskMemFree(pidl);
        return 0;
    }
    
    return -1;
}

void LOL() {
    LPVOID lpMsgBuf;
    char szBuffer[256];
    DWORD dw = GetLastError();

    if (dw == 0) {
        // If there's no error, set a custom error code
        dw = 12345;
        SetLastError(dw);
    }

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Format the error message
    sprintf(szBuffer, "LOL() called. Error Code: %d\nMessage: %s", dw, lpMsgBuf ? (char*)lpMsgBuf : "No error message available");

    // Display the error message
    MessageBox(NULL, szBuffer, "LOL Function Called", MB_OK | MB_ICONINFORMATION);

    // Free the buffer allocated by FormatMessage
    if (lpMsgBuf) LocalFree(lpMsgBuf);
}

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  int cdromDriveIndex;
  HWND mainWindowHandle = NULL;

  hInstance = hInst;
  InitCommonControls();
  if ( SetWindowToForegroundIfMinimized(szClass) == -1 )
    return -1;
  memset(&FullPath, 0, sizeof(FullPath));
  cdromDriveIndex = FindCDDrive();
  if ( cdromDriveIndex != -1 )
  {
    strcpy(byte_687861, ":\\");
    FullPath[0] = cdromDriveIndex + 'A';
    FullPath[1] = '\0';
  }
  hWndParent = mainWindowHandle;
  
//regManSaveLoadKeyConfig();
  regManCheckLoadSettings(&g_gameSettings, FullPath);

  // Corrected parameter count for DialogBoxParamA
  if ( DialogBoxParamA(hInstance, MAKEINTRESOURCE(IDD_BOOT), 0, DialogFunc, 0) == -1 )
    return -1;
 regManSaveSettingsAndPath(&g_gameSettings, FullPath);

  if ( isFullscreen == 1 )
    ResourceHandlerState |= 2u;

  if ( SetWindowToForegroundIfMinimized(szClass) == -1 )
    return -1;

  dword_68A3A8 = 1;
  mainWindowHandle = CreateMainWindow(ResourceHandlerState, hInstance, nShowCmd);

  if ( !mainWindowHandle )
    return 0;

  if ( isAudioEnabled )
  {
    hWndParent = mainWindowHandle;
    AudioHandler = 0;
  }
  else
  {
    AudioHandler = -1;
  }

  isGameStarted = 0;


  initGame();
  // Call LOL() function after window is shown
  LOL();


  return 0;  // Cast to int to avoid warning
}

HWND __cdecl CreateMainWindow(int mode, HINSTANCE hInstance, int nCmdShow)
{
  HWND windowHandle = NULL;
  HDC initialContext;
  int colorDepth;
  int screenWidth, screenHeight;
  WNDCLASSA windowClass = {0};
  WNDCLASSEXA extendedWindowClass = {0};

  switch (mode)
  {
    case 0:
      extendedWindowClass.hInstance = hInstance;
      extendedWindowClass.lpszClassName = (LPCSTR)szClass;
      extendedWindowClass.lpfnWndProc = handleMenuCallHotkeys;  
      extendedWindowClass.style = 0;
      extendedWindowClass.cbSize = sizeof(WNDCLASSEXA);
      extendedWindowClass.hIcon = LoadIconA(hInstance, (LPCSTR)MAKEINTRESOURCE(0x7F00));
      extendedWindowClass.hIconSm = LoadIconA(0, (LPCSTR)MAKEINTRESOURCE(0x7F05));
      extendedWindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
      extendedWindowClass.lpszMenuName = 0;
      extendedWindowClass.cbClsExtra = 0;
      extendedWindowClass.cbWndExtra = 0;
      extendedWindowClass.hbrBackground = GetStockObject(0);
      if (RegisterClassExA(&extendedWindowClass))
      {
        windowHandle = CreateWindowExA(
                         0,
                         (LPCSTR)szClass,
                         (LPCSTR)Caption,
                         0xCF0000u,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         384,
                         320,
                         0,
                         0,
                         hInstance,
                         0);
        if (windowHandle)
        {
          initialContext = CreateICA("Display", 0, 0, 0);
          colorDepth = GetDeviceCaps(initialContext, BITSPIXEL);
          DeleteDC(initialContext);
          if (colorDepth >= 8 || colorDepth == 1)
          {
            ShowWindow(windowHandle, nCmdShow);
            UpdateWindow(windowHandle);
          }
          else
          {
            MessageBoxA(windowHandle, BITDEPTH_ERROR_EN, Caption, 0);
            DestroyWindow(windowHandle);
            windowHandle = 0;
          }
        }
      }
      break;
    case 1:
      if (InitDeviceCapabilities(&unk_4C1560) != -1)
      {
        ChangeDisplayResolutionAndDepth(8, 640, 480);
        windowClass.hInstance = hInstance;
        windowClass.lpszClassName = szClass;
        windowClass.lpfnWndProc = handleMenuCallHotkeys;
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.hIcon = LoadIconA(hInstance, (LPCSTR)MAKEINTRESOURCE(0x7F00));
        windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
        windowClass.hbrBackground = 0;
        windowClass.lpszMenuName = Caption;
        RegisterClassA(&windowClass);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        windowHandle = CreateWindowExA(WS_EX_TOPMOST, (LPCSTR)szClass, (LPCSTR)Caption, WS_POPUP, 0, 0, screenWidth, screenHeight, 0, 0, hInstance, 0);
        if (windowHandle)
        {
          initialContext = CreateICA("Display", 0, 0, 0);
          colorDepth = GetDeviceCaps(initialContext, BITSPIXEL);
          DeleteDC(initialContext);
          if (colorDepth >= 8 || colorDepth == 1)
          {
            ShowWindow(windowHandle, nCmdShow);
            UpdateWindow(windowHandle);
            SetFocus(windowHandle);
            ShowCursor(FALSE);
          }
          else
          {
            MessageBoxA(windowHandle, BITDEPTH_ERROR_EN, Caption, 0);
            DestroyWindow(windowHandle);
            windowHandle = 0;
          }
        }
      }
      break;
    case 2:
      windowClass.hInstance = hInstance;
      windowClass.lpszClassName = szClass;
      windowClass.lpfnWndProc = handleMenuCallHotkeys;
      windowClass.style = CS_HREDRAW | CS_VREDRAW;
      windowClass.hIcon = LoadIconA(hInstance, (LPCSTR)MAKEINTRESOURCE(0x7F00));
      windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName = Caption;
      RegisterClassA(&windowClass);
      windowHandle = CreateWindowExA(
                       0,
                       (LPCSTR)szClass,
                       (LPCSTR)Caption,
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       262,
                       280,
                       0,
                       0,
                       hInstance,
                       0);
      if (windowHandle)
      {
        initialContext = CreateICA("Display", 0, 0, 0);
        colorDepth = GetDeviceCaps(initialContext, BITSPIXEL);
        DeleteDC(initialContext);
        if (colorDepth >= 8 || colorDepth == 1)
        {
          ShowWindow(windowHandle, nCmdShow);
          UpdateWindow(windowHandle);
          if (!initDirectDraw(0, windowHandle))
          {
            MessageBoxA(windowHandle, byte_439938, Caption, 0);
            DestroyWindow(windowHandle);
            windowHandle = 0;
          }
        }
        else
        {
          MessageBoxA(windowHandle, BITDEPTH_ERROR_EN, Caption, 0);
          DestroyWindow(windowHandle);
          windowHandle = 0;
        }
      }
      break;
    case 3:
      windowClass.hInstance = hInstance;
      windowClass.lpszClassName = szClass;
      windowClass.lpfnWndProc = handleMenuCallHotkeys;
      windowClass.style = CS_HREDRAW | CS_VREDRAW;
      windowClass.hIcon = LoadIconA(hInstance, (LPCSTR)MAKEINTRESOURCE(0x7F00));
      windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName = Caption;
      RegisterClassA(&windowClass);
      screenHeight = GetSystemMetrics(SM_CYSCREEN);
      screenWidth = GetSystemMetrics(SM_CXSCREEN);
      windowHandle = CreateWindowExA(WS_EX_TOPMOST, (LPCSTR)szClass, (LPCSTR)Caption, WS_POPUP, 0, 0, screenWidth, screenHeight, 0, 0, hInstance, 0);
      if (windowHandle)
      {
        ShowWindow(windowHandle, nCmdShow);
        UpdateWindow(windowHandle);
        SetFocus(windowHandle);
        ShowCursor(FALSE);
        if (!initDirectDraw(1, windowHandle))
        {
          MessageBoxA(windowHandle, byte_4398E0, Caption, 0);
          DestroyWindow(windowHandle);
          windowHandle = 0;
        }
      }
      break;
    default:
      break;
  }

  return windowHandle;
}

int __cdecl SetWindowToForegroundIfMinimized(LPCSTR lpszClass)
{
  HWND Window; // esi
  WINDOWPLACEMENT wndpl; // [esp+4h] [ebp-2Ch] BYREF

  Window = FindWindowExA(0, 0, lpszClass, 0);
  if ( !Window )
    return 0;
  wndpl.length = 44;
  if ( !GetWindowPlacement(Window, &wndpl) )
    return -1;
  if ( wndpl.showCmd == 2 )
  {
    wndpl.showCmd = 1;
    if ( !SetWindowPlacement(Window, &wndpl) )
      return -1;
  }
  SetActiveWindow(Window);
  SetForegroundWindow(Window);
  return -1;
}

HWND hWndParent; 

INT_PTR CALLBACK handleKeyConfigWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

INT_PTR CALLBACK OptionsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            // Initialize dialog controls here if needed
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, LOWORD(wParam));
                    return TRUE;

                case IDC_KEYBIND_SETTINGS:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_KEYCONFIG), hDlg, handleKeyConfigWindow);
                    return TRUE;

                case IDC_DIRECTSOUND_SETTINGS:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_BOOT_DIRECT_SOUND), hDlg, DirectSoundDialogProc);
                    return TRUE;

                case IDC_DIRECTX_DISPLAY_SETTINGS:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_BOOT_DIRECTX_DISPLAY), hDlg, DXDisplayDialogProc);
                    return TRUE;

                case IDC_GAME_SPEED_SETTINGS:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_BOOT_GAME_SPEED), hDlg, GameSpeedDialogProc);
                    return TRUE;

                case IDC_SOUND_SETTINGS:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_BOOT_SOUND), hDlg, SoundDialogProc);
                    return TRUE;

                case IDC_FRAMESKIP_SETTINGS:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_BOOT_SPEED), hDlg, SpeedDialogProc);
                    return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

INT_PTR CALLBACK DirectSoundDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            if (isAudioEnabled)
            {
                CheckRadioButton(hDlg, IDC_DIRECTSOUND_ON, IDC_DIRECTSOUND_OFF, IDC_DIRECTSOUND_ON);
            }
            else
            {
                CheckRadioButton(hDlg, IDC_DIRECTSOUND_ON, IDC_DIRECTSOUND_OFF, IDC_DIRECTSOUND_OFF);
            }
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                isAudioEnabled = (IsDlgButtonChecked(hDlg, IDC_DIRECTSOUND_ON) == BST_CHECKED);
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_DIRECTSOUND_ON || LOWORD(wParam) == IDC_DIRECTSOUND_OFF)
            {
                CheckRadioButton(hDlg, IDC_DIRECTSOUND_ON, IDC_DIRECTSOUND_OFF, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

INT_PTR CALLBACK DXDisplayDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            if (screenResolutionFlag)
            {
                CheckRadioButton(hDlg, IDC_SCREEN_RESOLUTION_ON, IDC_SCREEN_RESOLUTION_OFF, IDC_SCREEN_RESOLUTION_ON);
            }
            if (dword_439878)
            {
                CheckRadioButton(hDlg, IDC_FULLSCREEN_ON, IDC_FULLSCREEN_OFF, IDC_FULLSCREEN_ON);
            }
            CheckRadioButton(hDlg, IDC_FULLSCREEN_ON, IDC_FULLSCREEN_OFF, isFullscreenReg ? IDC_FULLSCREEN_ON : IDC_FULLSCREEN_OFF);
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_FULLSCREEN_ON || LOWORD(wParam) == IDC_FULLSCREEN_OFF)
            {
                CheckRadioButton(hDlg, IDC_FULLSCREEN_ON, IDC_FULLSCREEN_OFF, LOWORD(wParam));
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_SCREEN_RESOLUTION_ON || LOWORD(wParam) == IDC_SCREEN_RESOLUTION_OFF)
            {
                CheckRadioButton(hDlg, IDC_SCREEN_RESOLUTION_ON, IDC_SCREEN_RESOLUTION_OFF, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

INT_PTR CALLBACK GameSpeedDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            // Initialize dialog controls here
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            // Handle other controls here
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

INT_PTR CALLBACK SoundDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            if (dword_439884)
            {
                CheckRadioButton(hDlg, IDC_SOUND_ON, IDC_SOUND_OFF, IDC_SOUND_ON);
            }
            if (settings_sound_effects)
            {
                CheckRadioButton(hDlg, IDC_SOUND_EFFECTS_ON, IDC_SOUND_EFFECTS_OFF, IDC_SOUND_EFFECTS_ON);
            }
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_SOUND_ON || LOWORD(wParam) == IDC_SOUND_OFF)
            {
                CheckRadioButton(hDlg, IDC_SOUND_ON, IDC_SOUND_OFF, LOWORD(wParam));
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_SOUND_EFFECTS_ON || LOWORD(wParam) == IDC_SOUND_EFFECTS_OFF)
            {
                CheckRadioButton(hDlg, IDC_SOUND_EFFECTS_ON, IDC_SOUND_EFFECTS_OFF, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

INT_PTR CALLBACK SpeedDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            // Initialize dialog controls here
            switch (dword_439880)
            {
                case 0:
                case 1:
                    CheckRadioButton(hDlg, IDC_VERY_SLOW_SPEED, IDC_VERY_HIGH_SPEED, IDC_VERY_SLOW_SPEED);
                    break;
                case 2:
                case 3:
                    CheckRadioButton(hDlg, IDC_VERY_SLOW_SPEED, IDC_VERY_HIGH_SPEED, IDC_SLOW_SPEED);
                    break;
                case 4:
                    CheckRadioButton(hDlg, IDC_VERY_SLOW_SPEED, IDC_VERY_HIGH_SPEED, IDC_NORMAL_SPEED);
                    break;
                case 5:
                case 6:
                    CheckRadioButton(hDlg, IDC_VERY_SLOW_SPEED, IDC_VERY_HIGH_SPEED, IDC_HIGH_SPEED);
                    break;
                default:
                    CheckRadioButton(hDlg, IDC_VERY_SLOW_SPEED, IDC_VERY_HIGH_SPEED, IDC_VERY_HIGH_SPEED);
                    break;
            }
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                // Save the selected speed setting
                if (IsDlgButtonChecked(hDlg, IDC_VERY_SLOW_SPEED) == BST_CHECKED)
                    dword_439880 = 0;
                else if (IsDlgButtonChecked(hDlg, IDC_SLOW_SPEED) == BST_CHECKED)
                    dword_439880 = 2;
                else if (IsDlgButtonChecked(hDlg, IDC_NORMAL_SPEED) == BST_CHECKED)
                    dword_439880 = 4;
                else if (IsDlgButtonChecked(hDlg, IDC_HIGH_SPEED) == BST_CHECKED)
                    dword_439880 = 6;
                else if (IsDlgButtonChecked(hDlg, IDC_VERY_HIGH_SPEED) == BST_CHECKED)
                    dword_439880 = 8;
                // Fall through to IDCANCEL
            }
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_VERY_SLOW_SPEED || LOWORD(wParam) == IDC_SLOW_SPEED || LOWORD(wParam) == IDC_NORMAL_SPEED || LOWORD(wParam) == IDC_HIGH_SPEED || LOWORD(wParam) == IDC_VERY_HIGH_SPEED)
            {
                CheckRadioButton(hDlg, IDC_VERY_SLOW_SPEED, IDC_VERY_HIGH_SPEED, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

INT_PTR __stdcall DialogFunc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND itemHandle;
    unsigned int strLength;
    unsigned int strIndex;
    HWND dlgItemHandle;
    char folderPath[260];

    if (message == WM_INITDIALOG)
    {
        SetDlgItemTextA(dialogHandle, 1034, FullPath);
        SendDlgItemMessageA(dialogHandle, 1034, EM_SETSEL, 0, MAKELPARAM(0, -1));
        itemHandle = GetDlgItem(dialogHandle, 1034);
        SetFocus(itemHandle);
        if (ResourceHandlerState)
            CheckRadioButton(dialogHandle, 101, 102, 101);
        else
            CheckRadioButton(dialogHandle, 101, 102, 102);
        if (isFullscreen)
            CheckRadioButton(dialogHandle, 103, 103, 103);
        return 0;
    }

    if (message != WM_COMMAND)
        return 0;

    switch (LOWORD(wParam))
    {
        case IDOK:
            EndDialog(dialogHandle, 0);
            return 0;

        case IDCANCEL:
            EndDialog(dialogHandle, -1);
            return 0;

        case 101:
            ResourceHandlerState |= 1u;
            return 0;

        case 102:
            ResourceHandlerState = 0;
            return 0;

        case 103:
            isFullscreen = (isFullscreen + 1) & 1;
            return 0;

        case IDC_SETTINGS:
            if (DialogBoxParamA(hInstance, MAKEINTRESOURCE(IDD_BOOT_OPTION), dialogHandle, OptionsDialogProc, 0) == -1)
                return 0;
            regManSaveSettingsAndPath(&g_gameSettings, FullPath);
            return 0;

        case IDC_BUTTON1:
            if (BrowseAndSetFolderPath(dialogHandle, folderPath) == -1)
                return 0;
            strLength = strlen(folderPath) + 1;
            strIndex = strLength - 1;
            if (strLength != 1 && folderPath[strIndex - 1] != '\\')
                strcat(&folderPath[strIndex], "\\");
            memcpy(FullPath, folderPath, sizeof(FullPath));
            SetDlgItemTextA(dialogHandle, 1034, FullPath);
            SendDlgItemMessageA(dialogHandle, 1034, EM_SETSEL, 0, MAKELPARAM(0, -1));
            dlgItemHandle = GetDlgItem(dialogHandle, 1034);
            SetFocus(dlgItemHandle);
            regManSaveSettingsAndPath(&g_gameSettings, FullPath);
            return 0;

        default:
            return 0;
    }
}

LRESULT CALLBACK handleMenuCallHotkeys(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;

  if (message > WM_ACTIVATEAPP)
  {
    switch (message)
    {
      case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
          DestroyWindow(windowHandle);
          return 0;
        }
        if (wParam == VK_F3) // 'r' key (F3 for "reconfigure")
        {
        //  sound_playsfxwrapper(0, 1, 10);
          if (ResourceHandlerState == 3)
            releaseDirectDrawResources();
          ShowCursor(TRUE);
          // saveKeyConfig();
          if (DialogBoxParamA(hInstance, MAKEINTRESOURCE(IDD_KEYCONFIG), windowHandle, handleKeyConfigWindow, 0) == -1)
            return -1;
          // loadKeyConfig();
          if ((ResourceHandlerState & 1) != 0)
            ShowCursor(FALSE);
          // updateJoysticks(windowHandle);
          if (ResourceHandlerState == 3)
          {
            if (initDirectDraw(1, windowHandle) && ResourceHandlerState == 3)
              UpdateColorInformation();
          }
          else
          {
            updatePalette();
          }
         // sound_playsfxwrapper(0, 1, 10);
          return 0;
        }
        if (wParam == VK_F4) // 's' key (F4 for "save")
        {
          if (currentGameType == GAME_TYPE_VERSUS || currentGameType == GAME_TYPE_TEAM || currentGameType == GAME_TYPE_TOURNAMENT)
          {
          // sound_playsfxwrapper(0, 1, 10);
            isSaveRequested = 1;
            return 0;
          }
        }
        break;
      case WM_SETFOCUS:
      case WM_ACTIVATEAPP:
        if (wParam != (WPARAM)windowHandle && ResourceHandlerState != 3)
        {
          updatePalette();
        }
        break;
      default:
        return DefWindowProcA(windowHandle, message, wParam, lParam);
    }
    return 0;
  }
  
  if (message == WM_ACTIVATEAPP)
  {
    isWindowActive = (wParam != 0);
    return 0;
  }
  else if (message == WM_DESTROY)
  {
    // resetAudio();
    // cleanupAudio();
    if (!AudioHandler)
    {
       releaseResources();
    }
    if (ResourceHandlerState == 1)
    {
      colorDepth = 8;  // Assuming 8-bit color depth
      screenWidth = GetSystemMetrics(SM_CXSCREEN);
      screenHeight = GetSystemMetrics(SM_CYSCREEN);
      ChangeDisplayResolutionAndDepth(colorDepth, screenWidth, screenHeight);
    }
     releaseDirectDrawResources();
    ShowCursor(TRUE);
    PostQuitMessage(0);
    return 0;
  }
  
  return DefWindowProcA(windowHandle, message, wParam, lParam);
}

int __cdecl initDirectDraw(int isFullScreen, HWND windowHandle)
{
  DDSURFACEDESC ddsd;
  DDSCAPS ddscaps;
//  HRESULT hr;
  
  if (DirectDrawCreate(NULL, &lpDD, NULL) != DD_OK)
    return 0;
  
  if (isFullScreen)
  {
    if (lpDD->lpVtbl->SetCooperativeLevel(lpDD, windowHandle, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK)
      return 0;
    
    if (isFullscreenReg)
    {
      if (lpDD->lpVtbl->SetDisplayMode(lpDD, 640, 480, 8) != DD_OK)
        return 0;
    }
    else if (lpDD->lpVtbl->SetDisplayMode(lpDD, 320, 240, 8) != DD_OK)
    {
      return 0;
    }
    
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    
    if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, (LPDIRECTDRAWSURFACE*)&graphicsInterface, NULL) != DD_OK)
      return 0;
    
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    if (((LPDIRECTDRAWSURFACE)graphicsInterface)->lpVtbl->GetAttachedSurface((LPDIRECTDRAWSURFACE)graphicsInterface, &ddscaps, (LPDIRECTDRAWSURFACE*)&g_backBuffer) != DD_OK)
      return 0;
    
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = 256;
    ddsd.dwHeight = 256;
    
    if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, (LPDIRECTDRAWSURFACE*)&g_resourceManager, NULL) != DD_OK)
      return 0;
  }
  else
  {
    if (lpDD->lpVtbl->SetCooperativeLevel(lpDD, windowHandle, DDSCL_NORMAL) != DD_OK)
      return 0;
    
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    
    if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, (LPDIRECTDRAWSURFACE*)&graphicsInterface, NULL) != DD_OK)
      return 0;
    
    if (lpDD->lpVtbl->CreateClipper(lpDD, 0, &g_ddClipper, NULL) != DD_OK)
      return 0;
    
    if (g_ddClipper->lpVtbl->SetHWnd(g_ddClipper, 0, windowHandle) != DD_OK)
      return 0;
    
    if (((LPDIRECTDRAWSURFACE)graphicsInterface)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)graphicsInterface, g_ddClipper) != DD_OK)
      return 0;
    
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = GetSystemMetrics(SM_CXSCREEN);
    ddsd.dwHeight = GetSystemMetrics(SM_CYSCREEN);
    
    if (lpDD->lpVtbl->CreateSurface(lpDD, &ddsd, (LPDIRECTDRAWSURFACE*)&g_graphicsManager, NULL) != DD_OK)
      return 0;
  }
  return 1;
}

int releaseResources()
{
    struct tagOFNA *currentStruct;
    struct tagOFNA *endStruct;
    void (*releaseFunc)(void*);

    currentStruct = unk_6472F0;
    endStruct = &stru_647310;

    while (currentStruct < endStruct)
    {
        if (currentStruct->lStructSize)
        {
            releaseFunc = (void (*)(void*))(*((DWORD *)currentStruct->lStructSize) + 8);
            releaseFunc((void*)currentStruct->lStructSize);
        }
        currentStruct->lStructSize = 0;
        currentStruct++;
    }

    if (dword_4C0658)
    {
        releaseFunc = (void (*)(void*))(*((DWORD *)dword_4C0658) + 8);
        releaseFunc((void*)dword_4C0658);
    }
    dword_4C0658 = 0;

    if (ppDS)
    {
        ppDS->lpVtbl->Release(ppDS);
    }
    ppDS = 0;  // Use 0 instead of NULL for VS 1998

    return 0;
}

int releaseDirectDrawResources() // Note: This function uses old-style casts
{
  if (lpDD)
  {
    if (graphicsInterface)
      (*(void (__cdecl **)(void *))(*((DWORD *)graphicsInterface) + 8))(graphicsInterface);
    
    if (g_resourceManager)
      (*(void (__cdecl **)(void *))(*((DWORD *)g_resourceManager) + 8))(g_resourceManager);
    
    if (g_graphicsManager)
      (*(void (__cdecl **)(void *))(*((DWORD *)g_graphicsManager) + 8))(g_graphicsManager);
    
    lpDD->lpVtbl->Release(lpDD);
    lpDD = NULL;
  }
  return 0;
}

int updatePalette()
{
  int result; // eax

  if ( !g_globalPalette )
    return -1;
   result = AnimatePalette(g_globalPalette, 0, 0x100u, (LPPALETTEENTRY)g_gamePaletteEntries);
if (!result)
    return -1;
  return result;
}

int ChangeDisplayResolutionAndDepth(int colorDepth, int resolutionWidth, int resolutionHeight)
{
    DEVMODE deviceMode;
    LONG result;

    // Validate input parameters
    if (colorDepth != 4 && colorDepth != 8 && colorDepth != 16 && colorDepth != 24 && colorDepth != 32)
        return -1;
    if (resolutionWidth < 640 || resolutionHeight < 480)
        return -1;

    // Initialize device capabilities
    if (InitDeviceCapabilities(unk_4C1560) != -1)
        return -1;

    // Set the new display settings
    deviceMode.dmBitsPerPel = colorDepth;
    deviceMode.dmPelsWidth = resolutionWidth;
    deviceMode.dmPelsHeight = resolutionHeight;
    deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Attempt to change the display settings
    result = ChangeDisplaySettingsA(&deviceMode, 0);

    // Return 0 if successful, -1 if failed
    return (result == DISP_CHANGE_SUCCESSFUL) ? 0 : -1;
}

int InitDeviceCapabilities(void** param)
{
    DEVMODE* deviceMode;
    HDC deviceContext;

    if (!param)
        return -1;
    
    deviceMode = (DEVMODE*)*param;
    if (!deviceMode)
    {
        *param = malloc(sizeof(DEVMODE));
        if (!*param)
            return -1;
        deviceMode = (DEVMODE*)*param;
    }
    
    ZeroMemory(deviceMode, sizeof(DEVMODE));
    deviceMode->dmSize = sizeof(DEVMODE);
    
    deviceContext = GetDC(NULL);
    if (!deviceContext)
        return -1;
    
    deviceMode->dmLogPixels = GetDeviceCaps(deviceContext, LOGPIXELSY);
    deviceMode->dmBitsPerPel = GetDeviceCaps(deviceContext, BITSPIXEL);
    deviceMode->dmPelsWidth = GetDeviceCaps(deviceContext, HORZRES);
    deviceMode->dmPelsHeight = GetDeviceCaps(deviceContext, VERTRES);
    
    ReleaseDC(NULL, deviceContext);
    return 0;
}





