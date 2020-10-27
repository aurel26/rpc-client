#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "exorpc_h.h"
#include "RPC Client.h"

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "comctl32.lib")

HANDLE g_hHeapRpc;
HINSTANCE g_hApplication;

void
EnableRpcControls (
   _In_ HWND hDlg,
   _In_ DWORD dwCase
)
{
   switch (dwCase)
   {
      default:
      case 0:
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_ADD), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_GETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_IMPGETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_SHUTDOWN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_HASH), FALSE);
         break;

      case 1:
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_ADD), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_GETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_IMPGETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_SHUTDOWN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_HASH), FALSE);
         break;

      case 2:
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_ADD), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_GETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_IMPGETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_SHUTDOWN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_HASH), TRUE);
         break;

      case 3:
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_ADD), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_GETTOKEN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_IMPGETTOKEN), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_SHUTDOWN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_HASH), TRUE);
         break;

      case 4:
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_ADD), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_GETTOKEN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_IMPGETTOKEN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_SHUTDOWN), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RPC_HASH), TRUE);
         break;
   }
}

void
EnableAuthControls (
   _In_ HWND hDlg,
   _In_ DWORD dwCase
)
{
   switch (dwCase)
   {
      case 0:
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_ANON), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_IMP), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_EXP), TRUE);

         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_DOM), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_USER), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_PASS), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_LEVEL), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_SRV), FALSE);
         break;

      case 1:
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_ANON), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_IMP), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_EXP), TRUE);

         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_DOM), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_USER), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_PASS), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_LEVEL), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_SRV), TRUE);
         break;

      case 2:
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_ANON), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_IMP), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_EXP), TRUE);

         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_DOM), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_USER), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_PASS), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_LEVEL), TRUE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_SRV), TRUE);
         break;

      case 3:
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_ANON), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_IMP), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_RADIO_AUTH_EXP), FALSE);

         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_DOM), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_USER), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_EDIT_AUTH_PASS), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_LEVEL), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDC_COMBO_AUTH_SRV), FALSE);
         break;
   }
}

int
CALLBACK
WinMain (
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR lpCmdLine,
   _In_ int nCmdShow
)
{
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);
   UNREFERENCED_PARAMETER(nCmdShow);

   g_hApplication = hInstance;
   g_hHeapRpc = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 1024 * 1024, 0);
   DialogBox(hInstance, (LPCTSTR)IDD_DIALOG_MAIN, 0, (DLGPROC)DialogProc);
   return(0);
}

INT_PTR
CALLBACK
DialogProc (
   HWND hwndDlg,
   UINT uMsg,
   WPARAM wParam,
   LPARAM lParam
)
{
   UNREFERENCED_PARAMETER(lParam);

   DWORD dwThreadId;
   HWND hWnd;

   switch (uMsg)
   {
      case WM_INITDIALOG:
      {
         LRESULT lResult;
         INITCOMMONCONTROLSEX icc;

         // Initialisation des CommonControls
         icc.dwSize = sizeof(icc);
         icc.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES;
         InitCommonControlsEx(&icc);

         // Status Bas
         int iSize[2] = { 150, -1 };
         hWnd = GetDlgItem(hwndDlg, IDC_STATUSBAR);
         SendMessage(hWnd, SB_SETPARTS, 2, (LPARAM)iSize);
         SendMessage(hWnd, SB_SETTEXT, 0, (LPARAM)TEXT("Pas de connexion"));
         SendMessage(hWnd, SB_SETTEXT, 1, (LPARAM)TEXT(""));

         // Divers
         CheckDlgButton(hwndDlg, IDC_RADIO_AUTH_ANON, BST_CHECKED);
         hWnd = GetDlgItem(hwndDlg, IDC_BUTTON_CNX_DECONNECT);
         EnableWindow(hWnd, FALSE);
         EnableAuthControls(hwndDlg, 0);
         EnableRpcControls(hwndDlg, 0);

         // Chargement de l'icône de la fenêtre
         SetClassLongPtr(hwndDlg, GCLP_HICON, (LONG_PTR)LoadIcon(g_hApplication, MAKEINTRESOURCE(IDI_ICON_MAIN)));

         // ComboBox : Protocol
         hWnd = GetDlgItem(hwndDlg, IDC_COMBO_CNX_PROTO);
         SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("ncacn_ip_tcp"));
         SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("ncacn_np"));
         SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("ncacn_http"));
         SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("ncadg_ip_udp"));
         SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)TEXT("ncalrpc"));
         SendMessage(hWnd, CB_SETCURSEL, 0, 0);

         // ComboBox : AuthLevel
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_ADDSTRING, 0, (LPARAM)TEXT("None"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_LEVEL_NONE);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_ADDSTRING, 0, (LPARAM)TEXT("Connect"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_LEVEL_CONNECT);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_ADDSTRING, 0, (LPARAM)TEXT("Call"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_LEVEL_CALL);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_ADDSTRING, 0, (LPARAM)TEXT("Pkt"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_LEVEL_PKT);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_ADDSTRING, 0, (LPARAM)TEXT("Integrity"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_LEVEL_PKT_INTEGRITY);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_ADDSTRING, 0, (LPARAM)TEXT("Privacy"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_LEVEL_PKT_PRIVACY);
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_SETCURSEL, 1, 0);

         // ComboBox : AuthService
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_ADDSTRING, 0, (LPARAM)TEXT("None"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_NONE);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_ADDSTRING, 0, (LPARAM)TEXT("WinNT"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_WINNT);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_ADDSTRING, 0, (LPARAM)TEXT("Negociage"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_GSS_NEGOTIATE);
         lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_ADDSTRING, 0, (LPARAM)TEXT("Kerberos"));
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_SETITEMDATA, lResult, (LPARAM)RPC_C_AUTHN_GSS_KERBEROS);
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_SETCURSEL, 1, 0);
         SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_SETMINVISIBLE, 10, 0);

         // Limitation de la taille des zones de texte
         SendDlgItemMessage(hwndDlg, IDC_EDIT_CNX_ADDR, EM_LIMITTEXT, MAX_SIZE_NET_ADDR, 0);
         SendDlgItemMessage(hwndDlg, IDC_EDIT_CNX_EP, EM_LIMITTEXT, MAX_SIZE_ENDPOINT, 0);
         SendDlgItemMessage(hwndDlg, IDC_EDIT_AUTH_DOM, EM_LIMITTEXT, MAX_SIZE_NAME, 0);
         SendDlgItemMessage(hwndDlg, IDC_EDIT_AUTH_USER, EM_LIMITTEXT, MAX_SIZE_NAME, 0);
         SendDlgItemMessage(hwndDlg, IDC_EDIT_AUTH_PASS, EM_LIMITTEXT, MAX_SIZE_NAME, 0);
         SendDlgItemMessage(hwndDlg, IDC_EDIT_RPC_DATA_TOHASH, EM_LIMITTEXT, MAX_BUF_SIZE, 0);

         return TRUE;
      }

      case WM_COMMAND:
         switch (LOWORD(wParam))
         {
         case IDC_BUTTON_CNX_CONNECT:
         {
            CreateThread(NULL, 0, tConnectRpc, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_CNX_DECONNECT:
         {
            CreateThread(NULL, 0, tDisconnectRpc, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_MGMT_ISALIVE:
         {
            CreateThread(NULL, 0, tIsServerListening, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_MGMT_INQ:
         {
            CreateThread(NULL, 0, tMgmtInqIfIds, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_RPC_GET_VERSION:
         {
            CreateThread(NULL, 0, tRpcGetVersion, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_RPC_SHUTDOWN:
         {
            CreateThread(NULL, 0, tRpcShutdown, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_RPC_ADD:
         {
            CreateThread(NULL, 0, tRpcAddInteger, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_RPC_GETTOKEN:
         {
            CreateThread(NULL, 0, tRpcGetProcessToken, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_RPC_IMPGETTOKEN:
         {
            CreateThread(NULL, 0, tRpcImpGetToken, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_BUTTON_RPC_HASH:
         {
            CreateThread(NULL, 0, tRpcHash256, hwndDlg, 0, &dwThreadId);
            return TRUE;
         }

         case IDC_RADIO_AUTH_IMP:
            EnableAuthControls(hwndDlg, 1);
            return TRUE;

         case IDC_RADIO_AUTH_EXP:
            EnableAuthControls(hwndDlg, 2);
            return TRUE;

         case IDC_RADIO_AUTH_ANON:
            EnableAuthControls(hwndDlg, 0);
            return TRUE;

         case IDCANCEL:
         case IDOK:
            EndDialog(hwndDlg, 0);
            return TRUE;
         }
   }
   return FALSE;
}

_Must_inspect_result_
_Ret_maybenull_ _Post_writable_byte_size_(size)
void*
__RPC_USER
MIDL_user_allocate (
   _In_ size_t size
)
{
   __try
   {
      return HeapAlloc(g_hHeapRpc, HEAP_ZERO_MEMORY, size);
   }
#pragma warning(suppress: 6320)
   __except (EXCEPTION_EXECUTE_HANDLER)
   {
      MessageBox(0, 0, 0, 0);
      return 0;
   }
}

void
__RPC_USER
MIDL_user_free (
   _Pre_maybenull_ _Post_invalid_ void *p
)
{
   __try
   {
      HeapFree(g_hHeapRpc, 0, p);
   }
#pragma warning(suppress: 6320)
   __except (EXCEPTION_EXECUTE_HANDLER)
   {
      MessageBox(0, 0, 0, 0);
   }
}
