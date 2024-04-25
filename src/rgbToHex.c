// gcc rgbToHex.c -o rgbToHex -s -O2 -Wall -Wextra
#include <stdio.h>
#include <string.h>
#include <windows.h>

char decimal_to_hex(int *decimal)
{
    const char ASCII_NUMBER_START = 48;
    const char ASCII_CAPITAL_LETTER_START = 65;
    int offset = *decimal;

    return offset < 10 ? ASCII_NUMBER_START + offset : ASCII_CAPITAL_LETTER_START + offset - 10;
}

void invalid_input()
{
    printf("Invaid character provided, all three values should be numbers ranging from 0-255");
    exit(1);
}

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
    // check all three entered strings, validate them, and read in as int
    for (unsigned i = 1; i < 4; ++i)
    {
        unsigned len = strlen(argv[i]);
        if (len > 3)
        {
            invalid_input();
        }

        int rgbValue = 0;
        for (unsigned j = 0; j < len; ++j)
        {
            char currentChar = argv[i][j];
            if (currentChar < '0' || currentChar > '9')
            {
                invalid_input();
            }
            // Take char * 10 since we read most signifigant number first
            // Example: 255 we read 2 -> 25 -> 255 after this loops twice
            // And - 48 to get correct int val from ascii char
            rgbValue = (rgbValue * 10) + (currentChar - 48);
        }
        if (rgbValue > 255)
            invalid_input();

        rgb[i - 1] = rgbValue;
    }

    // convert the three rgb ints to hex string
    const unsigned outputLength = 8;
    char hex[outputLength];
    hex[0] = '#';
    hex[7] = '\0';

    for (unsigned k = 0; k < 3; ++k)
    {
        int index = (k * 2) + 1;
        int left = rgb[k] / 16;
        int right = rgb[k] % 16;

        hex[index] = decimal_to_hex(&left);
        hex[index + 1] = decimal_to_hex(&right);
    }

    printf("Hex: %s", hex);

    HGLOBAL globalHandle = GlobalAlloc(GMEM_MOVEABLE, outputLength);
    memcpy(GlobalLock(globalHandle), hex, outputLength);

    GlobalUnlock(globalHandle);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, globalHandle);
    CloseClipboard();

    return 0;
}