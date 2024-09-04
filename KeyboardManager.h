#ifndef KEYBOARD_MANAGER_H
#define KEYBOARD_MANAGER_H

#include <windows.h>

void SetupKeyboard(HWND hWND);
void ClearKeyboard(void);
int ReadKeyboard(int output[]);

unsigned long ConvertReadInputsToBitArray(
    unsigned char button1,
    unsigned char button2,
    unsigned char button3,
    unsigned char button4,
    unsigned char button5,
    unsigned char button6,
    unsigned char button7,
    unsigned char button8
);

#endif /* KEYBOARD_MANAGER_H */
