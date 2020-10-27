#define MAX_SIZE_PROTOCOL     20
#define MAX_SIZE_NET_ADDR     255
#define MAX_SIZE_ENDPOINT     255
#define MAX_SIZE_OUTPUT       11000
#define MAX_SIZE_MESSAGE      255
#define MAX_SIZE_NAME         255

//
// WinMain.cpp
//
void EnableRpcControls(_In_ HWND hDlg, _In_ DWORD dwCase);
void EnableAuthControls(_In_ HWND hDlg, _In_ DWORD dwCase);

//
// ThreadMain.cpp
//
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI tConnectRpc(LPVOID);
DWORD WINAPI tDisconnectRpc(LPVOID);
DWORD WINAPI tIsServerListening(LPVOID);
DWORD WINAPI tMgmtInqIfIds(LPVOID);
DWORD WINAPI tRpcGetVersion(LPVOID);
DWORD WINAPI tRpcShutdown(LPVOID);
DWORD WINAPI tRpcAddInteger(LPVOID);
DWORD WINAPI tRpcGetProcessToken(LPVOID);
DWORD WINAPI tRpcImpGetToken(LPVOID);
DWORD WINAPI tRpcHash256(LPVOID);