/*
 * DUART Enabler for 2nd generation e-dictionary.
 *
 * Copyright (C) 2021 Suguru Saito <sg.sgch07@gmail.com>
 *
 * This file is licensed in MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */

#include <windows.h>

#define APPNAME (L"DUART Enabler")

#define DEF_PROC(func_name, ret_type, ...) \
    typedef ret_type (WINAPI *func_name##Proc)(__VA_ARGS__)
#define GET_PROC(mod, fname) \
    fname##Proc fname = (fname##Proc)GetProcAddress(mod, L""#fname)

DEF_PROC(EdMmMapIoSpace, PVOID, DWORD, SIZE_T, int);
DEF_PROC(EdMmUnmapIoSpace, void, PVOID, SIZE_T);

#define REG_SET             (1)
#define REG_CLR             (2)
#define REG_TGL             (3)
#define HW_PINCTRL          (0x80018000)
#define HW_PINCTRL_MUXSEL8  (HW_PINCTRL | 0x00000170)

int main(int argc, char **argv)
{
    HMODULE hSharplib = LoadLibrary(L"SHARPLIB");
    DWORD *pinctrl_muxsel8;
    GET_PROC(hSharplib, EdMmMapIoSpace);
    GET_PROC(hSharplib, EdMmUnmapIoSpace);

    pinctrl_muxsel8 = EdMmMapIoSpace(HW_PINCTRL_MUXSEL8, 
                                                sizeof(DWORD)*4, 0);

    // Check DUART pin-mode
    if ((*(pinctrl_muxsel8) & 0x0000000f) == 0x0000000a) {
        MessageBox(NULL, L"DUART is already enabled", APPNAME, MB_OK);
    } else {
        // Bank3_PIN17 GPIO(0b11) => DUART(0b01)
        // Bank3_PIN16 GPIO(0b11) => DUART(0b01)
        *(pinctrl_muxsel8 + REG_TGL) = 0x00000005;
        MessageBox(NULL, L"DUART Enabled!", APPNAME, MB_OK);
    }
    
    EdMmUnmapIoSpace(pinctrl_muxsel8, sizeof(DWORD)*4);

    FreeLibrary(hSharplib);
    return 0;
}
