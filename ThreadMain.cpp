#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"
#include "exorpc_h.h"
#include "RPC Client.h"

#define STATUS_SUCCESS     0

RPC_BINDING_HANDLE g_hBinding = NULL;

VOID
DisplayMessageWithError (
   HWND hwndDlg,
   DWORD dwError,
   LPCTSTR szFormatMessage,
   ...
)
{
   DWORD dwReturn;
   TCHAR szMessage[MAX_SIZE_MESSAGE];
   TCHAR szOutput[MAX_SIZE_OUTPUT];
   va_list argptr;
   va_start(argptr, szFormatMessage);

   _vstprintf_s(szOutput, MAX_SIZE_OUTPUT, szFormatMessage, argptr);

   dwReturn = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, dwError, 0, szMessage, MAX_SIZE_MESSAGE, 0);
   if (dwReturn)
   {
      _tcscat_s(szOutput, MAX_SIZE_OUTPUT, TEXT("\r\n"));
      _tcscat_s(szOutput, MAX_SIZE_OUTPUT, szMessage);
   }
   SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
}

DWORD
WINAPI
tConnectRpc (
   LPVOID lpParameter
)
{
   HWND hwndDlg, hWnd;
   TCHAR szProtocol[MAX_SIZE_PROTOCOL];
   TCHAR szNetAddr[MAX_SIZE_NET_ADDR];
   TCHAR szEndPoint[MAX_SIZE_ENDPOINT];
   TCHAR szOutput[MAX_SIZE_OUTPUT];
   RPC_WSTR szStringBinding;
   RPC_STATUS status;
   DWORD dwReturn;
   LRESULT lResult;

   hwndDlg = (HWND)lpParameter;

   EnableAuthControls(hwndDlg, 3);

   if (g_hBinding)
   {
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, TEXT("La connextion est déjà établie"));
      return 0;
   }

   GetDlgItemText(hwndDlg, IDC_COMBO_CNX_PROTO, szProtocol, MAX_SIZE_PROTOCOL);
   GetDlgItemText(hwndDlg, IDC_EDIT_CNX_ADDR, szNetAddr, MAX_SIZE_NET_ADDR);
   GetDlgItemText(hwndDlg, IDC_EDIT_CNX_EP, szEndPoint, MAX_SIZE_ENDPOINT);

   status = RpcStringBindingCompose(NULL, (RPC_WSTR)szProtocol, (RPC_WSTR)szNetAddr, (RPC_WSTR)szEndPoint, NULL, &szStringBinding);
   if (status != RPC_S_OK)
   {
      _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("RpcStringBindingCompose: Error %d"), status);
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
      dwReturn = 0;
   }
   else
   {
      status = RpcBindingFromStringBinding(szStringBinding, &g_hBinding);

      if (status != RPC_S_OK)
      {
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("RpcBindingFromStringBinding: Error %d"), status);
         SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
         dwReturn = 0;
      }
      else
      {
         LRESULT lAuthLevel, lAuthSrv;

         // Affichage du nouvel état
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("Handle: %p\r\n"), g_hBinding);
         SendDlgItemMessage(hwndDlg, IDC_STATUSBAR, SB_SETTEXT, 0, (LPARAM)szOutput);
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("Chaîne de connexion: %s"), (LPTSTR)szStringBinding);
         SendDlgItemMessage(hwndDlg, IDC_STATUSBAR, SB_SETTEXT, 1, (LPARAM)szOutput);

         SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, TEXT(""));

         hWnd = GetDlgItem(hwndDlg, IDC_BUTTON_CNX_CONNECT);
         EnableWindow(hWnd, FALSE);
         hWnd = GetDlgItem(hwndDlg, IDC_BUTTON_CNX_DECONNECT);
         EnableWindow(hWnd, TRUE);

         if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_AUTH_ANON) == BST_CHECKED)
         {
            // Contexte anonyme
            status = RpcBindingSetAuthInfo(g_hBinding, (RPC_WSTR)L"", RPC_C_AUTHN_LEVEL_NONE, RPC_C_AUTHN_NONE, RPC_C_NO_CREDENTIALS, RPC_C_AUTHN_NONE);
            _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("RpcBindingSetAuthInfo(RPC_C_NO_CREDENTIALS): %d"), status);
         }
         else
         {
            // Activation de l'authentifcation

            // Récupération des paramètres d'authentification
            lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_GETCURSEL, 0, 0);
            lAuthLevel = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_LEVEL, CB_GETITEMDATA, lResult, 0);

            lResult = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_GETCURSEL, 0, 0);
            lAuthSrv = SendDlgItemMessage(hwndDlg, IDC_COMBO_AUTH_SRV, CB_GETITEMDATA, lResult, 0);

            if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_AUTH_IMP) == BST_CHECKED)
            {
               status = RpcBindingSetAuthInfo(g_hBinding, (RPC_WSTR)szNetAddr, (unsigned long)lAuthLevel, (unsigned long)lAuthSrv, NULL, RPC_C_AUTHZ_DEFAULT);
               _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("RpcBindingSetAuthInfo(NULL): %d"), status);
            }
            else if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_AUTH_EXP) == BST_CHECKED)
            {
               SEC_WINNT_AUTH_IDENTITY rpcAuth;
               UINT uUsernameSize, uPasswordSize, uDomainSize;
               TCHAR szUsername[MAX_SIZE_NAME];
               TCHAR szPassword[MAX_SIZE_NAME];
               TCHAR szDomain[MAX_SIZE_NAME];

               uUsernameSize = GetDlgItemText(hwndDlg, IDC_EDIT_AUTH_USER, szUsername, MAX_SIZE_NAME);
               uPasswordSize = GetDlgItemText(hwndDlg, IDC_EDIT_AUTH_PASS, szPassword, MAX_SIZE_NAME);
               uDomainSize = GetDlgItemText(hwndDlg, IDC_EDIT_AUTH_DOM, szDomain, MAX_SIZE_NAME);

               rpcAuth.User = (RPC_WSTR)szUsername;
               rpcAuth.UserLength = uUsernameSize;
               rpcAuth.Password = (RPC_WSTR)szPassword;
               rpcAuth.PasswordLength = uPasswordSize;
               rpcAuth.Domain = (RPC_WSTR)szDomain;
               rpcAuth.DomainLength = uDomainSize;
               rpcAuth.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

               status = RpcBindingSetAuthInfo(g_hBinding, (RPC_WSTR)szNetAddr, (unsigned long)lAuthLevel, (unsigned long)lAuthSrv, &rpcAuth, RPC_C_AUTHZ_DEFAULT);
               _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("RpcBindingSetAuthInfo(): %d"), status);
            }
         }
         SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
      }
      RpcStringFree(&szStringBinding);
      dwReturn = 1;
   }

   //RpcEpResolveBinding(

   return dwReturn;
}

DWORD
WINAPI
tDisconnectRpc (
   LPVOID lpParameter
)
{
   HWND hwndDlg, hWnd;
   RPC_STATUS status;
   TCHAR szOutput[MAX_SIZE_OUTPUT];

   hwndDlg = (HWND)lpParameter;

   status = RpcBindingFree(&g_hBinding);
   g_hBinding = NULL;

   hWnd = GetDlgItem(hwndDlg, IDC_BUTTON_CNX_CONNECT);
   EnableWindow(hWnd, TRUE);
   hWnd = GetDlgItem(hwndDlg, IDC_BUTTON_CNX_DECONNECT);
   EnableWindow(hWnd, FALSE);

   EnableRpcControls(hwndDlg, 0);
   EnableAuthControls(hwndDlg, 0);

   if (status != RPC_S_OK)
   {
      _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("RpcBindingFree: Error %d"), status);
      SetDlgItemText(hwndDlg, IDC_STATUSBAR, TEXT("Etat de connexion inconnu"));
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
      return 0;
   }
   else
   {
      SetDlgItemText(hwndDlg, IDC_STATUSBAR, TEXT("Connexion fermée"));
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, TEXT(""));
      return 1;
   }
}

DWORD
WINAPI
tIsServerListening (
   LPVOID lpParameter
)
{
   RPC_STATUS status;

   status = RpcMgmtIsServerListening(g_hBinding);
   DisplayMessageWithError((HWND)lpParameter, status, TEXT("RpcMgmtIsServerListening: %d"), status);

   return 1;
}

DWORD
WINAPI
tMgmtInqIfIds (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   RPC_STATUS status;
   TCHAR szOutput[MAX_SIZE_OUTPUT] = { 0 };
   RPC_IF_ID_VECTOR* rpcIdVect;
   LPTSTR pszString;

   hwndDlg = (HWND)lpParameter;

   status = RpcMgmtInqIfIds(g_hBinding, &rpcIdVect);
   if (status != RPC_S_OK)
   {
      DisplayMessageWithError((HWND)lpParameter, status, TEXT("RpcMgmtInqIfIds: %d"), status);
      return 0;
   }
   else
   {
      ZeroMemory(szOutput, MAX_SIZE_OUTPUT * sizeof(TCHAR));

      for (DWORD i = 0; i < rpcIdVect->Count; i++)
      {
         if (UuidToString(&rpcIdVect->IfId[i]->Uuid, (RPC_WSTR*)&pszString) == RPC_S_OK)
         {
            TCHAR szTemp[MAX_SIZE_OUTPUT] = { 0 };

            _stprintf_s(szTemp, MAX_SIZE_OUTPUT, TEXT("%ws (%u.%u)\r\n"), pszString, rpcIdVect->IfId[i]->VersMajor, rpcIdVect->IfId[i]->VersMinor);
            _tcscat_s(szOutput, MAX_SIZE_OUTPUT, szTemp);
            RpcStringFree((RPC_WSTR*)&pszString);
         }
         else
         {
            _tcscat_s(szOutput, MAX_SIZE_OUTPUT, TEXT("Échec de UuidToString\r\n"));
         }
      }

      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
      status = RpcIfIdVectorFree(&rpcIdVect);
   }

   return 1;
}

DWORD
WINAPI
tRpcGetVersion (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   ULONG ulCode;
   unsigned long Version;
   TCHAR szOutput[MAX_SIZE_OUTPUT];

   hwndDlg = (HWND)lpParameter;

   RpcTryExcept
   {
      GetExoSvcVersion(g_hBinding, &Version);
      _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("GetVersion: (%lu)"), Version);
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
      EnableRpcControls(hwndDlg, Version);
   }
#pragma warning(suppress: 6320)
   RpcExcept(EXCEPTION_EXECUTE_HANDLER)
   {
      ulCode = RpcExceptionCode();
      DisplayMessageWithError((HWND)lpParameter, ulCode, TEXT("GetVersion: Exception %ld (0x%lx)"), ulCode, ulCode);
   }
   RpcEndExcept

   return 0;
}

DWORD
WINAPI
tRpcShutdown (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   ULONG ulCode;
   int lResult;
   TCHAR szOutput[MAX_SIZE_OUTPUT];

   hwndDlg = (HWND)lpParameter;

   RpcTryExcept
   {
      lResult = Shutdown(g_hBinding);
      _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("ShutDown: (%d)"), lResult);
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
   }
#pragma warning(suppress: 6320)
   RpcExcept(EXCEPTION_EXECUTE_HANDLER)
   {
      ulCode = RpcExceptionCode();
      DisplayMessageWithError((HWND)lpParameter, ulCode, TEXT("ShutDown: Exception %ld (0x%lx)"), ulCode, ulCode);
   }
   RpcEndExcept

   return 0;
}

DWORD
WINAPI
tRpcAddInteger (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   TCHAR szOutput[MAX_SIZE_OUTPUT];
   ULONG ulCode;
   long l1, l2, lResult;

   hwndDlg = (HWND)lpParameter;

   l1 = GetDlgItemInt(hwndDlg, IDC_EDIT_RPC_ADD_1, NULL, TRUE);
   l2 = GetDlgItemInt(hwndDlg, IDC_EDIT_RPC_ADD_2, NULL, TRUE);
   SetDlgItemInt(hwndDlg, IDC_EDIT_RPC_ADD_1, l1, TRUE);
   SetDlgItemInt(hwndDlg, IDC_EDIT_RPC_ADD_2, l2, TRUE);

   RpcTryExcept
   {
      lResult = AddInteger(g_hBinding, l1, l2);
      _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("AddInteger(%ld, %ld): %ld"), l1, l2, lResult);
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
   }
#pragma warning(suppress: 6320)
   RpcExcept(EXCEPTION_EXECUTE_HANDLER)
   {
      ulCode = RpcExceptionCode();
      DisplayMessageWithError((HWND)lpParameter, ulCode, TEXT("AddInteger: Exception %ld (0x%lx)"), ulCode, ulCode);
   }
   RpcEndExcept

   return 0;
}

DWORD
WINAPI
tRpcGetProcessToken (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   ULONG ulCode;
   int lResult;
   TCHAR szOutput[MAX_SIZE_OUTPUT];
   LPWSTR szResult;

   hwndDlg = (HWND)lpParameter;

   RpcTryExcept
   {
      szResult = NULL;
      lResult = GetProcessTokenDescription(g_hBinding, &szResult);
      if (lResult == STATUS_SUCCESS)
      {
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("GetProcessTokenDescription: (%d)\r\n%s"), lResult, szResult);
         midl_user_free(szResult);
      }
      else
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("GetProcessTokenDescription: (%d)"), lResult);
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);

   }
#pragma warning(suppress: 6320)
   RpcExcept(EXCEPTION_EXECUTE_HANDLER)
   {
      ulCode = RpcExceptionCode();
      DisplayMessageWithError((HWND)lpParameter, ulCode, TEXT("GetProcessTokenDescription: Exception %ld (0x%lx)"), ulCode, ulCode);
   }
   RpcEndExcept

   return 0;
}

DWORD
WINAPI
tRpcImpGetToken (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   ULONG ulCode;
   int lResult;
   TCHAR szOutput[MAX_SIZE_OUTPUT] = { 0 };
   LPWSTR szResult = NULL;

   hwndDlg = (HWND)lpParameter;

   RpcTryExcept
   {
      lResult = ImpersonateAndGetThreadTokenDescription(g_hBinding, &szResult);
      if (lResult == STATUS_SUCCESS)
      {
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("ImpersonateAndGetThreadTokenDescription: (%d)\r\n%s"), lResult, szResult);
         midl_user_free(szResult);
      }
      else
         _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("ImpersonateAndGetThreadTokenDescription: (%d)"), lResult);
      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
   }
#pragma warning(suppress: 6320)
   RpcExcept(EXCEPTION_EXECUTE_HANDLER)
   {
      ulCode = RpcExceptionCode();
      DisplayMessageWithError((HWND)lpParameter, ulCode, TEXT("ImpersonateAndGetThreadTokenDescription: Exception %ld (0x%lx)"), ulCode, ulCode);
   }
   RpcEndExcept

   return 0;
}

DWORD
WINAPI
tRpcHash256 (
   LPVOID lpParameter
)
{
   HWND hwndDlg;
   ULONG ulCode;
   UINT uSize;
   int lResult;
   CHAR szDataToHash[MAX_BUF_SIZE];
   TCHAR szOutput[MAX_SIZE_OUTPUT];
   BYTE pbResult[HASH_256_SIZE];

   hwndDlg = (HWND)lpParameter;

   uSize = GetDlgItemTextA(hwndDlg, IDC_EDIT_RPC_DATA_TOHASH, szDataToHash, MAX_BUF_SIZE);

   RpcTryExcept
   {
      TCHAR szOctet[4];

      lResult = Hash256InWorkerProcess(g_hBinding, uSize, (PBYTE)szDataToHash, pbResult);
      _stprintf_s(szOutput, MAX_SIZE_OUTPUT, TEXT("Hash256InWorkerProcess: %d\r\n"), lResult);

      if (lResult == STATUS_SUCCESS)
      {
         _tcscat_s(szOutput, MAX_SIZE_OUTPUT, TEXT("   "));
         for (DWORD i = 0; i < HASH_256_SIZE / 2; i++)
         {
            _stprintf_s(szOctet, 4, TEXT("%.2X "), pbResult[i]);
            _tcscat_s(szOutput, MAX_SIZE_OUTPUT, szOctet);
         }

         _tcscat_s(szOutput, MAX_SIZE_OUTPUT, TEXT("\r\n   "));
         for (DWORD i = HASH_256_SIZE / 2; i < HASH_256_SIZE; i++)
         {
            _stprintf_s(szOctet, 4, TEXT("%.2X "), pbResult[i]);
            _tcscat_s(szOutput, MAX_SIZE_OUTPUT, szOctet);
         }
      }

      SetDlgItemText(hwndDlg, IDC_EDIT_OUTPUT, szOutput);
   }
#pragma warning(suppress: 6320)
   RpcExcept(EXCEPTION_EXECUTE_HANDLER)
   {
      ulCode = RpcExceptionCode();
      DisplayMessageWithError((HWND)lpParameter, ulCode, TEXT("Hash256InWorkerProcess: Exception %ld (0x%lx)"), ulCode, ulCode);
   }
   RpcEndExcept

   return 0;
}