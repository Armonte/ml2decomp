#include "KeyboardManager.h"
#include <windows.h>

HWND window;
unsigned char last_frame_inputs_00679da8[256];
unsigned char pressed_two_frames_ago_00679ea8[256];
unsigned char lpKeyState_00679fa8[256];
unsigned char processed_inputs_0067a0a8[256];

void SetupKeyboard(HWND hWND) {
    window = hWND;
    ClearKeyboard();
}

void ClearKeyboard(void) {
    int i;
    for (i = 0; i < 0x100; ++i) {
        /* This counts down in the original but that's probably due to compiler optimization */
        /* It's also getting bulk stored for the same reason */
        last_frame_inputs_00679da8[i] = 0;
        /* This is in a separate loop in the original */
        processed_inputs_0067a0a8[i] = 0;
    }
}

int ReadKeyboard(int output[]) {
    /* Returns a 256 byte array where entries are
       0x80 if the button was pressed this frame and not last frame
       0x01 if the button was pressed last frame and not this frame
       0xc1 if the button was pressed last frame and this frame (held)
       0x00 if the button was neither pressed last frame or this frame */
    unsigned char pressed_last_frame;
    unsigned char pressed_this_frame;
    int i;

    if (!GetKeyboardState(lpKeyState_00679fa8)) {
        return -1;
    }
    for (i = 0; i < 0x100; ++i) {
        pressed_last_frame = last_frame_inputs_00679da8[i];
        pressed_this_frame = lpKeyState_00679fa8[i] & 0x80;
        lpKeyState_00679fa8[i] = pressed_this_frame;
        pressed_two_frames_ago_00679ea8[i] = pressed_last_frame;
        last_frame_inputs_00679da8[i] = pressed_this_frame;
        /* Some of the following is probably less idiomatic than it should be as a straight conversion of the asm */
        lpKeyState_00679fa8[i] = (unsigned char)(~pressed_last_frame & pressed_this_frame); /* 0x80 if button this frame and not last frame else 0 */
        if ((pressed_last_frame & 0x80) != 0) {
            lpKeyState_00679fa8[i] |= 1;
        }
        if (pressed_this_frame) {
            lpKeyState_00679fa8[i] |= 0x40;
        }
        processed_inputs_0067a0a8[i] = lpKeyState_00679fa8[i]; /* This was optimized into a second loop originally */
    }
    if (output) {
        for (i = 0; i < 0x100; ++i) {
            output[i] = lpKeyState_00679fa8[i];
        }
    }
    return 0;
}

unsigned long ConvertReadInputsToBitArray(
    unsigned char button1,
    unsigned char button2,
    unsigned char button3,
    unsigned char button4,
    unsigned char button5,
    unsigned char button6,
    unsigned char button7,
    unsigned char button8
) {
    /* FUN_0042f120 */
    /* Returns a bit array with flags set based on the buttons being pressed */
    /* 0000 0000 0000 0000 0000 0000 0000 0000 */
    /*                     Held      This turn */
    /*                     Dir  Btn  Dir  Btn */
    unsigned long bit_array = 0;
    /* Pressed this turn */
    if ((processed_inputs_0067a0a8[button1] & 0x80) != 0) {
        bit_array = 0x80;
    }
    if ((processed_inputs_0067a0a8[button2] & 0x80) != 0) {
        bit_array = bit_array | 0x40;
    }
    if ((processed_inputs_0067a0a8[button3] & 0x80) != 0) {
        bit_array = bit_array | 0x20;
    }
    if ((processed_inputs_0067a0a8[button4] & 0x80) != 0) {
        bit_array = bit_array | 0x10;
    }
    if ((processed_inputs_0067a0a8[button5] & 0x80) != 0) {
        bit_array = bit_array | 8;
    }
    if ((processed_inputs_0067a0a8[button6] & 0x80) != 0) {
        bit_array = bit_array | 4;
    }
    if ((processed_inputs_0067a0a8[button7] & 0x80) != 0) {
        bit_array = bit_array | 2;
    }
    if ((processed_inputs_0067a0a8[button8] & 0x80) != 0) {
        bit_array = bit_array | 1;
    }
    /* Held */
    if ((processed_inputs_0067a0a8[button1] & 0x40) != 0) {
        bit_array = bit_array | 0x8000;
    }
    if ((processed_inputs_0067a0a8[button2] & 0x40) != 0) {
        bit_array = bit_array | 0x4000;
    }
    if ((processed_inputs_0067a0a8[button3] & 0x40) != 0) {
        bit_array = bit_array | 0x2000;
    }
    if ((processed_inputs_0067a0a8[button4] & 0x40) != 0) {
        bit_array = bit_array | 0x1000;
    }
    if ((processed_inputs_0067a0a8[button5] & 0x40) != 0) {
        bit_array = bit_array | 0x800;
    }
    if ((processed_inputs_0067a0a8[button6] & 0x40) != 0) {
        bit_array = bit_array | 0x400;
    }
    if ((processed_inputs_0067a0a8[button7] & 0x40) != 0) {
        bit_array = bit_array | 0x200;
    }
    if ((processed_inputs_0067a0a8[button8] & 0x40) != 0) {
        bit_array = bit_array | 0x100;
    }
    /* SOCD */
    if (((bit_array & 0x8000) != 0) && ((bit_array & 0x4000) != 0)) {
        bit_array = bit_array ^ 0x4000;
    }
    if (((bit_array & 0x2000) != 0) && ((bit_array & 0x1000) != 0)) {
        bit_array = bit_array ^ 0x1000;
    }
    if (((bit_array & 0x80) != 0) && ((bit_array & 0x40) != 0)) {
        bit_array = bit_array ^ 0x40;
    }
    if (((bit_array & 0x20) != 0) && ((bit_array & 0x10) != 0)) {
        bit_array = bit_array ^ 0x10;
    }
    return bit_array;
}

