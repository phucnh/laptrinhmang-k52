#pragma once

//add global function in here
extern CString EncodeToUTF8(LPCTSTR szSource);
extern CString DecodeFromUTF8(LPCTSTR szSource);
extern BYTE MakeByte(TCHAR ch1, TCHAR ch2);

extern CString GetCurrentTimeStr();