//bsod.cpp
//Author:sometimes.naive
//idea from:Joakim Schicht
//E-mail:sometimes.naive[at]hotmail.com

#pragma once

#include <windows.h>
#include <tchar.h>
#include <string.h>

#define SetCriticalAndTerminateProcess(x) {\
	Func_SetProcessCritical(x,1);\
	TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,x),0);\
}
#define CallBSODNowEx Func_SetProcessCritical(GetCurrentProcessId(),1),ExitProcess(0),TerminateProcess(GetCurrentProcess(),0)

#define CallBSODNow {\
	Func_SetProcessCritical(GetCurrentProcessId(),1);\
	ExitProcess(0);\
	TerminateProcess(GetCurrentProcess(),0);\
}

void Func_SetProcessCritical(DWORD_PTR pid,DWORD flag){
	typedef struct _UNICODE_STRING {
		USHORT		Length;     
		USHORT		MaximumLength;
		PWSTR		Buffer;
	} UNICODE_STRING ,*PUNICODE_STRING;
	
	typedef struct _OBJECT_ATTRIBUTES {
		ULONG				Length;
		HANDLE				RootDirectory;
		PUNICODE_STRING			ObjectName;
		ULONG				Attributes;
		PVOID				SecurityDescriptor;
		PVOID				SecurityQualityOfService;
	}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
	
	typedef struct _CLIENT_ID
	{
		 DWORD_PTR UniqueProcess;
		 DWORD_PTR UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;
	
	typedef enum _PROCESSINFOCLASS {
		ProcessBasicInformation,
		ProcessQuotaLimits,
		ProcessIoCounters,
		ProcessVmCounters,
		ProcessTimes,
		ProcessBasePriority,
		ProcessRaisePriority,
		ProcessDebugPort,
		ProcessExceptionPort,
		ProcessAccessToken,
		ProcessLdtInformation,
		ProcessLdtSize,
		ProcessDefaultHardErrorMode,
		ProcessIoPortHandlers,
		ProcessPooledUsageAndLimits,
		ProcessWorkingSetWatch,
		ProcessUserModeIOPL,
		ProcessEnableAlignmentFaultFixup,
		ProcessPriorityClass,
		ProcessWx86Information,
		ProcessHandleCount,
		ProcessAffinityMask,
		ProcessPriorityBoost,
		ProcessDeviceMap,
		ProcessSessionInformation,
		ProcessForegroundInformation,
		ProcessWow64Information,
		ProcessImageFileName,
		ProcessLUIDDeviceMapsEnabled,
		ProcessBreakOnTermination,
		ProcessDebugObjectHandle,
		ProcessDebugFlags,
		ProcessHandleTracing,
		ProcessUnknown33,
		ProcessUnknown34,
		ProcessUnknown35,
		ProcessCookie,
		MaxProcessInfoClass
	}PROCESSINFOCLASS;
	
	typedef struct _SpecialStruct{
		DWORD x;
	} SpecialStruct, *PSpecialStruct;
	typedef DWORD NTSTATUS;
	typedef NTSTATUS (NTAPI *_NtOpenProcess)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES, PCLIENT_ID);
	typedef NTSTATUS (CALLBACK * _NtSetInformationProcess)(HANDLE,PROCESSINFOCLASS,PVOID,ULONG);
	_NtSetInformationProcess NtSetInformationProcess=(_NtSetInformationProcess)GetProcAddress(GetModuleHandle(TEXT("ntdll")),"NtSetInformationProcess");
	_NtOpenProcess NtOpenProcess=(_NtOpenProcess)GetProcAddress(GetModuleHandle(TEXT("ntdll")),"NtOpenProcess");
	OBJECT_ATTRIBUTES oa={sizeof(OBJECT_ATTRIBUTES),NULL,NULL,0x00000040,NULL,NULL};
	CLIENT_ID b={pid,0};
	SpecialStruct _={flag};
	HANDLE hd;
	NtOpenProcess(&hd,0x001F0FFF,&oa,&b);
	NtSetInformationProcess(hd,ProcessBreakOnTermination,&_,sizeof(SpecialStruct));
}
