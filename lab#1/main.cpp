#include <windows.h>

#define ID_BUTTON 69
#define ID_LISTBOX 70

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[] = "CodeBlocksWindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow){
  HWND hwnd;               /* This is the handle for our window */
  MSG messages;            /* Here messages to the application are saved */
  WNDCLASSEX wincl;        /* Data structure for the windowclass */

  /* The Window structure */
  wincl.hInstance = hThisInstance;
  wincl.lpszClassName = szClassName;
  wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
  wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
  wincl.cbSize = sizeof (WNDCLASSEX);

  /* Use default icon and mouse-pointer */
  wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
  wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
  wincl.lpszMenuName = NULL;                 /* No menu */
  wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
  wincl.cbWndExtra = 0;                      /* structure or the window instance */

  // Set custom background
  wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(238, 238, 238));

  /* Register the window class, and if it fails quit the program */
  if (!RegisterClassEx (&wincl))
    return 0;

  /* The class is registered, let's create the program*/
  hwnd = CreateWindowEx (0, szClassName, "WP Lab#1 example (mobile app simulation)",
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 270+16, 450+16,
    HWND_DESKTOP, NULL, hThisInstance, NULL);

  ShowWindow (hwnd, nCmdShow);

  while (GetMessage (&messages, NULL, 0, 0)){
    TranslateMessage (&messages);
    DispatchMessage (&messages);
  }

  return messages.wParam;
}

bool firstPaintColor = true;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

  HDC hdc, hdcMem;
  PAINTSTRUCT ps;
  RECT rect;

  int hMenuIndex = -1;
  HWND hwndTextareaLogin
     , hwndTextareaPassword
     , hwndButton
     , hwndListbox;
  bool firstPaint = true;

  switch (message){                  /* handle the messages */
    case WM_CREATE:

      /* Create textarea 1 */
      hwndTextareaLogin = CreateWindowEx (
        WS_EX_CLIENTEDGE, "edit", "Login",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
        60, 240, 150, 30,
        hwnd, (HMENU)(++hMenuIndex),
        (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL
      );

      /* Create textarea 2 */
      hwndTextareaPassword = CreateWindowEx (
        WS_EX_CLIENTEDGE, "edit", "Password",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
        60, 270, 150, 30,
        hwnd, (HMENU)(++hMenuIndex),
        (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL
      );

      /* Create button */
      hwndButton = CreateWindow (
        "button", "Login",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        60, 330, 150, 30,
        hwnd,(HMENU)ID_BUTTON,
        (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL
      );

      /* Create listbox */
      hwndListbox = CreateWindow (
        "listbox", "",
        WS_CHILD | WS_BORDER | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL,
        5, 5, 200, 200,
        hwnd, (HMENU)ID_LISTBOX,
        (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL
      );

      // Force paint
      InvalidateRect(hwnd, NULL, TRUE);

      break;

    case WM_PAINT:
      if (firstPaint){
        hdc = BeginPaint (hwnd, &ps);
        // Draw rectangle
        Rectangle (hdc, 60, 60, 210, 210);

        // Draw text
        SelectObject (hdc, GetStockObject(SYSTEM_FIXED_FONT));
        GetClientRect (hwnd, &rect);
        rect.top = 360;
        rect.right = 210;
        rect.bottom = 390;
        rect.left = 60;
        DrawText (hdc, "Enter your data to log in", -1, &rect, DT_VCENTER | DT_CENTER | DT_WORDBREAK);

        EndPaint (hwnd, &ps);

        firstPaint = false;
      }

      break;

    case WM_MOVE:
      RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
      break;

    case WM_CTLCOLOREDIT:
    {
      if ((HWND)lParam == GetDlgItem(hwnd, 1)){
        HBRUSH hBrush;
        if(firstPaintColor){
          hBrush = CreateSolidBrush(RGB(255,0,0));
          firstPaintColor = false;
        }else{
          hBrush = CreateSolidBrush(RGB(255,255,0));
        }
        ::SelectObject((HDC)wParam, (HGDIOBJ)hBrush);
      }

      break;
    }

    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case ID_BUTTON:
          SendDlgItemMessage(hwnd, ID_LISTBOX, LB_ADDSTRING, 0, (LPARAM)"Hi there!");

          // SendMessage(hwndText1,LB_ADDSTRING,0, (LPARAM)message);
          // SendMessage(hwndText,WM_SETTEXT,NULL, (LPARAM)"");

          // RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);

          break;

      }
      break;


    case WM_DESTROY:
      PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
      break;

    default:                      /* for messages that we don't deal with */
      return DefWindowProc (hwnd, message, wParam, lParam);
  }

  return 0;
}
