#define MAX_SIZE_PROTOCOL     20
#define MAX_SIZE_NET_ADDR     256
#define MAX_SIZE_ENDPOINT     256
#define MAX_SIZE_OUTPUT       2048
#define MAX_SIZE_TEXTBOX      1024*1024
#define MAX_SIZE_MESSAGE      255
#define MAX_SIZE_NAME         256

//
// WinMain.cpp
//
void EnableRpcControls(_In_ HWND hDlg, _In_ DWORD dwVersion);
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