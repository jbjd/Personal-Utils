// gcc hexToRgb.c -o hexToRgb -s -O2 -Wall -Wextra
#include <stdio.h>
#include <string.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Convert hex string to RGB values and copies it to the clipboard.\n");
        printf("The hex string should be 6 total characters long and NOT include a #.\n");
        return 0;
    }
    if (argc > 2 || strlen(argv[1]) != 6)
    {
        printf("Only provide a 6 digit input please.\n");
        return 1;
    }
    int r = 0, g = 0, b = 0;
    for (int i = 0; i < 6; ++i)
    {
        int hex_as_decimal;
        switch (argv[1][i])
        {
        case '0':
            hex_as_decimal = 0;
            break;
        case '1':
            hex_as_decimal = 1;
            break;
        case '2':
            hex_as_decimal = 2;
            break;
        case '3':
            hex_as_decimal = 3;
            break;
        case '4':
            hex_as_decimal = 4;
            break;
        case '5':
            hex_as_decimal = 5;
            break;
        case '6':
            hex_as_decimal = 6;
            break;
        case '7':
            hex_as_decimal = 7;
            break;
        case '8':
            hex_as_decimal = 8;
            break;
        case '9':
            hex_as_decimal = 9;
            break;
        case 'a':
        case 'A':
            hex_as_decimal = 10;
            break;
        case 'b':
        case 'B':
            hex_as_decimal = 11;
            break;
        case 'c':
        case 'C':
            hex_as_decimal = 12;
            break;
        case 'd':
        case 'D':
            hex_as_decimal = 13;
            break;
        case 'e':
        case 'E':
            hex_as_decimal = 14;
            break;
        case 'f':
        case 'F':
            hex_as_decimal = 15;
            break;
        default:
            printf("Input contained non-hex characters, exiting...\n");
            return 1;
        }

        // Check if this hex value is in the 16s digit spot
        // Example: 0xFA, F needs to be multiplied by 16
        if (i % 2 == 0)
        {
            hex_as_decimal *= 16;
        }

        switch (i)
        {
        case 0:
        case 1:
            r += hex_as_decimal;
            break;
        case 2:
        case 3:
            g += hex_as_decimal;
            break;
        case 4:
        case 5:
            b += hex_as_decimal;
            break;
        }
    }
    char rgb[16];
    sprintf(rgb, "%d %d %d", r, g, b);
    printf("RGB: %s", rgb);

    const unsigned len = strlen(rgb) + 1;
    HGLOBAL globalHandle = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(globalHandle), rgb, len);

    GlobalUnlock(globalHandle);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, globalHandle);
    CloseClipboard();

    return 0;
}