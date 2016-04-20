//cekproc.h
//Author:sometimes.naive
//E-mail:sometimes.naive[at]hotmail.com
//Last update:Apr. 14th, 2016
#pragma once


#include "strcamp.h"
#include <windows.h>
#include <tlhelp32.h>


bool isExecExist(char const * szProgramName,DWORD *dwProcessId=NULL,DWORD *dwFProcessId=NULL){
	PROCESSENTRY32 __={sizeof(PROCESSENTRY32),0,0,0,0,0,0,0,0,{0}}; 
	HANDLE _=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	bool ___=Process32First(_,&__);
	while (___){
		if (_tstrcamp(szProgramName,__.szExeFile)){
			if (dwProcessId!=NULL && dwFProcessId!=NULL)
				return ((*dwProcessId=__.th32ProcessID,*dwFProcessId=__.th32ParentProcessID),true);
			else if (dwProcessId!=NULL && !dwFProcessId)
					return ((*dwProcessId=__.th32ProcessID),true);
				 else if (dwFProcessId!=NULL && !dwProcessId) 
				 		return ((*dwFProcessId=__.th32ParentProcessID),true);
					  else
						return true;
		}
		___=Process32Next(_,&__);
	}
	return false;
}
