#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <commctrl.h>

// Function declarations
int __cdecl SetWindowToForegroundIfMinimized(LPCSTR lpszClass);
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

// Variable declarations
extern int IDD_BOOT;
extern char szClass[];
extern char byte_687861[3];
extern int isFullscreen;
extern int dword_68A3A8;
extern int isAudioEnabled;
extern int AudioHandler;
extern int isGameStarted;
extern int dword_439884;
extern int settings_sound_effects;
extern int dword_439880;
extern HWND hWndParent;
extern const char* IconName;
extern const char* pszDriver;
extern void* unk_4C1560;
extern const char* byte_439938;
extern const char* byte_4398E0;
extern const char* Text;

extern char FullPath[260];

// Add these new variable declarations
extern int screenResolutionFlag;
extern int dword_439878;
extern int isFullscreenReg;
extern HINSTANCE hInstance;

// Function prototypes
HWND CreateMainWindow(int mode, HINSTANCE hInstance, int nShowCmd);
int InitDeviceCapabilities(void* param);
void ChangeDisplayResolutionAndDepth(int colorDepth, int width, int height);
int initDirectDraw(int mode, HWND hwnd);
LRESULT CALLBACK handleMenuCallHotkeys(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


void LOL(void);

// Add these new function prototypes

//INT_PTR CALLBACK OptionsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
//INT_PTR CALLBACK DialogFunc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK handleKeyConfigWindow(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DirectSoundDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DXDisplayDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GameSpeedDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SoundDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SpeedDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK DialogFunc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK OptionsDialogProc(HWND, UINT, WPARAM, LPARAM);

// Add these function declarations


// External variable declarations
extern int isFullscreen;
extern int isAudioEnabled;
extern int dword_43986C;
extern int g_gameSpeed;
extern int screenResolutionFlag;
extern int dword_439878;
extern int isFullscreenReg;
extern int dword_439880;
extern int dword_439884;
extern int settings_sound_effects;
extern int dword_43988C;


/* Function declarations */


/* External variable declarations */

#endif // MAIN_H