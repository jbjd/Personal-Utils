// gcc .\rgbToHex.c
#include <stdio.h>
#include <string.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Convert three 0-255 RGB values to hex and copies it to the clipboard.\nUse like rgbToHex.exe # # #\n");
        return 0;
    }
    if (argc != 4)
    {
        printf("Please provide exactly three numbers.\n");
        return 0;
    }

    int rgb[3];
    int i, j, temp = 0;
    unsigned long long len;
    char currentChar;
    // check all three entered strings, validate them, and read in as int
    for (i = 1; i < 4; ++i)
    {
        len = strlen(argv[i]);
        if (len > 3)
        {
            goto INVALID_INPUT;
        }
        for (j = 0; j < len; ++j)
        {
            currentChar = argv[i][j];
            if (currentChar < '0' || currentChar > '9')
            {
                goto INVALID_INPUT;
            }
            // Take char * 10 since we read most signifigant number first
            // Example: 255 we read 2 -> 25 -> 255 after this loops twice
            // And - 48 to get correct int val from ascii char
            temp = (temp * 10) + (currentChar - 48);
        }
        if (temp > 255)
            goto INVALID_INPUT;
        rgb[i - 1] = temp;
        temp = 0;
    }
    // convert the three rgb ints to hex string
    char hex[8];
    hex[0] = '#';
    hex[7] = '\0';
    int index, left, right;
    for (i = 0; i < 3; ++i)
    {
        index = (i * 2) + 1;
        left = rgb[i] / 16;
        right = rgb[i] % 16;

        // either 0-9 if <= 9 or its a letter A-F
        // A starts at ascii 65 so (55 + (left or right)) to get int value
        hex[index] = left <= 9 ? 48 + left : 55 + left;
        hex[index + 1] = right <= 9 ? 48 + right : 55 + right;
    }

    printf("Hex: %s", hex);

    len = 8;
    HGLOBAL globalHandle = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(globalHandle), hex, len);

    GlobalUnlock(globalHandle);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, globalHandle);
    CloseClipboard();

    return 0;

INVALID_INPUT:
    printf("Invaid character provided, all three values should be numbers ranging from 0-255");
    return 1;
}