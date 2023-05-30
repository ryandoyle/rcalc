#include <windows.h>
#include <stdio.h>
#include <math.h>

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char szClassName[] = "RyanCalc";

HWND numberButtons[10];

#define IDC_B0 0
#define IDC_B1 1
#define IDC_B2 2
#define IDC_B3 3
#define IDC_B4 4
#define IDC_B5 5
#define IDC_B6 6
#define IDC_B7 7
#define IDC_B8 8
#define IDC_B9 9
#define IDC_BEQUAL 10
#define IDC_BCLEAR 11
#define IDC_BPLUS 12
#define IDC_DISPLAY 13

/* TODO: Actually implement a proper RPN calculator */
typedef struct _calculatorState {
    DOUBLE accumulatedValue;
    DOUBLE storedValue;
} CalculatorState;

/* TODO: Pass this these as lparams instead of global states */
HWND displayHandle;
CalculatorState gCalculatorState = {};
char displayBuffer[100];

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    
    
    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl)) {
        return 0;
    }

    hwnd = CreateWindowEx(0, szClassName,"Ryan's Calculator",WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,CW_USEDEFAULT,320,200,HWND_DESKTOP,NULL,hThisInstance,NULL);

    numberButtons[1] = CreateWindow("BUTTON", "1", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                                            40, 50, 25, hwnd, (HMENU)IDC_B1, hThisInstance, NULL);
    numberButtons[2] = CreateWindow("BUTTON", "2", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 70,
                                            40, 50, 25, hwnd, (HMENU)IDC_B2, hThisInstance, NULL);
    numberButtons[3] = CreateWindow("BUTTON", "3", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130,
                                            40, 50, 25, hwnd, (HMENU)IDC_B3, hThisInstance, NULL);
    numberButtons[4] = CreateWindow("BUTTON", "4", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                                            70, 50, 25, hwnd, (HMENU)IDC_B4, hThisInstance, NULL);
    numberButtons[5] = CreateWindow("BUTTON", "5", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 70,
                                            70, 50, 25, hwnd, (HMENU)IDC_B5, hThisInstance, NULL);
    numberButtons[6] = CreateWindow("BUTTON", "6", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130,
                                            70, 50, 25, hwnd, (HMENU)IDC_B6, hThisInstance, NULL);
    numberButtons[7] = CreateWindow("BUTTON", "7", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                                            100, 50, 25, hwnd, (HMENU)IDC_B7, hThisInstance, NULL);
    numberButtons[8] = CreateWindow("BUTTON", "8", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 70,
                                            100, 50, 25, hwnd, (HMENU)IDC_B8, hThisInstance, NULL);
    numberButtons[9] = CreateWindow("BUTTON", "9", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130,
                                            100, 50, 25, hwnd, (HMENU)IDC_B9, hThisInstance, NULL);
    numberButtons[0] = CreateWindow("BUTTON", "0", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 70,
                                        130, 50, 25, hwnd, (HMENU)IDC_B0, hThisInstance, NULL);

    // GetDlgCtrlID(numberButtons[1]);

    displayHandle = CreateWindowEx(WS_EX_RIGHT, "STATIC", "0", WS_VISIBLE | WS_CHILD,
                                   30, 10, 150, 25, hwnd, (HMENU)IDC_DISPLAY, hThisInstance, NULL);
    HWND clearButtonHandle = CreateWindow("BUTTON", "C", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 250,
                                          40, 50, 25, hwnd, (HMENU)IDC_BCLEAR, hThisInstance, NULL);
    HWND equalButtonHandle = CreateWindow("BUTTON", "=", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 250,
                                          70, 50, 25, hwnd, (HMENU)IDC_BEQUAL, hThisInstance, NULL);
    HWND plusButtonHandle = CreateWindow("BUTTON", "+", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 250,
                                          100, 50, 25, hwnd, (HMENU)IDC_BPLUS, hThisInstance, NULL);

    // Default system font (Tahoma)
    HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(clearButtonHandle, WM_SETFONT, (WPARAM)hFont, 0);
    SendMessage(equalButtonHandle, WM_SETFONT, (WPARAM)hFont, 0);
    SendMessage(plusButtonHandle, WM_SETFONT, (WPARAM)hFont, 0);
    int i;
    for (i = 0; i < 10; i++) {
        SendMessage(numberButtons[i], WM_SETFONT, (WPARAM)hFont, 0);
    }

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nFunsterStil);

    // MessageBox(NULL, "Hello message box", "Note", MB_OK);

    gCalculatorState.accumulatedValue = 0;
    gCalculatorState.storedValue = 0;

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void HandleCalculatorNumeric(CalculatorState *state, INT number) {
    state->accumulatedValue = state->accumulatedValue * 10;
    state->accumulatedValue += number;
}

void ClearCalculator(CalculatorState *state) {
    state->accumulatedValue = 0;
    state->storedValue = 0;
}

void AddCalculator(CalculatorState *state) {
    state->storedValue = state->storedValue + state->accumulatedValue;
    state->accumulatedValue = 0;
}

void EqualsCalculator(CalculatorState *state) {
    state->accumulatedValue = state->storedValue + state->accumulatedValue;
    state->storedValue = 0;
}

DOUBLE GetCalculatorCurrentValue(CalculatorState *state) {
    return state->accumulatedValue;
}

WINBOOL IsInteger(DOUBLE toTest) {
    return floor(toTest) == toTest;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_B1:
                    HandleCalculatorNumeric(&gCalculatorState, 1);
                    break;
                case IDC_B2:
                    HandleCalculatorNumeric(&gCalculatorState, 2);
                    break;
                case IDC_B3:
                    HandleCalculatorNumeric(&gCalculatorState, 3);
                    break;
                case IDC_B4:
                    HandleCalculatorNumeric(&gCalculatorState, 4);
                    break;
                case IDC_B5:
                    HandleCalculatorNumeric(&gCalculatorState, 5);
                    break;
                case IDC_B6:
                    HandleCalculatorNumeric(&gCalculatorState, 6);
                    break;
                case IDC_B7:
                    HandleCalculatorNumeric(&gCalculatorState, 7);
                    break;
                case IDC_B8:
                    HandleCalculatorNumeric(&gCalculatorState, 8);
                    break;
                case IDC_B9:
                    HandleCalculatorNumeric(&gCalculatorState, 9);
                    break;
                case IDC_B0:
                    HandleCalculatorNumeric(&gCalculatorState, 0);
                    break;
                case IDC_BCLEAR:
                    ClearCalculator(&gCalculatorState);
                    break;
                case IDC_BPLUS:
                    AddCalculator(&gCalculatorState);
                    break;
                case IDC_BEQUAL:
                    EqualsCalculator(&gCalculatorState);
                    break;
            }

            /* Update display */
            if (IsInteger(GetCalculatorCurrentValue(&gCalculatorState))) {
                sprintf(displayBuffer, "%ld", (LONG)GetCalculatorCurrentValue(&gCalculatorState));
            } else {
                sprintf(displayBuffer, "%lf", GetCalculatorCurrentValue(&gCalculatorState));
            }
            SetWindowText(displayHandle, displayBuffer);
            return 0;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
