
//  CFORM.CPP     Windows NT, MS Visual C/C++ (32-bit) 1.1
#include <windows.h>
#include "cform.h"

/**------------------------------------------------------**/
CPrinterForm::CPrinterForm(void) {
   dwForms = 0;    
   dwSize = BUF_SIZE;
   HKEY hKey;
   RegOpenKeyEx(HKEY_CURRENT_USER, "Printers", 0, 
      KEY_QUERY_VALUE, &hKey);
   RegQueryValueEx(hKey, "Default", 0, NULL, lpData, &dwSize);
   RegCloseKey(hKey);
   if (OpenPrinter((LPTSTR)lpData, &hPrinter, NULL))
      EnumForms(hPrinter, 1, (LPBYTE)lpData, BUF_SIZE,
         &dwSize, &dwForms);
} // CPrinterForm(void)

/**------------------------------------------------------**/
CPrinterForm::CPrinterForm(LPSTR pName) {
   dwForms = 0;
   if (OpenPrinter(pName, &hPrinter, NULL))
      EnumForms(hPrinter, 1, (LPBYTE)lpData, BUF_SIZE,
        &dwSize, &dwForms);
} // CPrinterForm(LPSTR)

/**------------------------------------------------------**/
CPrinterForm::~CPrinterForm(void) {
   if (hPrinter != NULL) ClosePrinter(hPrinter);
} // ~CPrinterForm

/**------------------------------------------------------**/
BOOL CPrinterForm::Enumerate(DWORD iIndex, LPSTR lpName) {
   if (iIndex == 0 || dwForms == 0) {
      EnumForms(hPrinter, 1, (LPBYTE)lpData, BUF_SIZE,
         &dwSize, &dwForms);
   }
   if (iIndex >= dwForms) return FALSE;
   pFI = (FORM_INFO_1 *)lpData;
   lstrcpy(lpName, pFI[iIndex].pName);
   return TRUE;
} // CPrinterForm::Enumerate

/**------------------------------------------------------**/
BOOL CPrinterForm::Add(LPSTR lpName, long width, long height,
      long left, long top, long right, long bottom) {
   FORM_INFO_1 fi;
   FillForm(&fi, lpName, width,height,left,top,right,bottom);
   return AddForm(hPrinter, 1, (LPBYTE)&fi);
} // CPrinterForm::Add

/**------------------------------------------------------**/
BOOL CPrinterForm::Delete(LPSTR lpName) {
   return DeleteForm(hPrinter, lpName);
} // CPrinterForm::Delete

/**------------------------------------------------------**/
BOOL CPrinterForm::Get(LPSTR lpName, long *width, long *height,
      long *left, long *top, long *right, long *bottom) {
   BYTE lpBuff[BUF_SIZE];
   FORM_INFO_1 *p;
   GetForm(hPrinter,lpName,1,(LPBYTE)lpBuff,BUF_SIZE,&dwSize);
   p = (FORM_INFO_1 *)lpBuff;
   *width  = p[0].Size.cx;
   *height = p[0].Size.cy;
   *left   = p[0].ImageableArea.left;
   *top    = p[0].ImageableArea.top;
   *right  = p[0].ImageableArea.right;
   *bottom = p[0].ImageableArea.bottom;
   return TRUE;
} // CPrinterForm::Get

/**------------------------------------------------------**/
BOOL CPrinterForm::Set(LPSTR lpName, long width, long height,
      long left, long top, long right, long bottom) {
   FORM_INFO_1 fi;
   FillForm(&fi, lpName, width,height,left,top,right,bottom);
   return SetForm(hPrinter, lpName, 1, (LPBYTE)&fi);
} // CPrinterForm::Set

/**------------------------------------------------------**/
void CPrinterForm::FillForm(FORM_INFO_1 *p, LPSTR pName,
      long dy, long dx, long x1, long y1, long x2, long y2) {
   p->pName = pName;
   p->Size.cx = dx; 
   p->Size.cy = dy;
   p->ImageableArea.left = x1;
   p->ImageableArea.top = y1;
   p->ImageableArea.right = x2;
   p->ImageableArea.bottom = y2;
} // CPrinterForm::FillForm

/* end of file */

