// FSDManager.cpp : Defines the entry point for the console application.
//
#include "CFSDPortConnector.h"
#include "FSDCommonInclude.h"
#include "FSDCommonDefs.h"
#include "stdio.h"
#include "AutoPtr.h"
#include "FSDThreadUtils.h"

HRESULT HrMain();

#define MAX_COMMAND_LENGTH 10
#define MAX_PARAMETER_LENGTH 256

int main()
{
	HRESULT hr = HrMain();
	if (FAILED(hr))
	{
		printf("Main failed with status 0x%x\n", hr);
		return 1;
	}
	
    return 0;
}

HRESULT UserInputParser(PVOID pvContext)
{
	UNREFERENCED_PARAMETER(pvContext);

	HRESULT hr = S_OK;

	CAutoPtr<CFSDPortConnector> pConnector;
	hr = NewInstanceOf<CFSDPortConnector>(&pConnector, g_wszFSDPortName);
	RETURN_IF_FAILED(hr);

	CAutoStringW wszCommand = new WCHAR[MAX_COMMAND_LENGTH];
	RETURN_IF_FAILED_ALLOC(wszCommand);

	CAutoStringW wszParameter = new WCHAR[MAX_PARAMETER_LENGTH];
	RETURN_IF_FAILED_ALLOC(wszCommand);

	bool fExit = false;
	while (!fExit)
	{
		wscanf_s(L"%ls", wszCommand.LetPtr(), MAX_COMMAND_LENGTH);
		if (wcscmp(wszCommand.LetPtr(), L"chdir") == 0)
		{
			wscanf_s(L"%ls[/]", wszParameter.LetPtr(), MAX_FILE_NAME_LENGTH);

			FSD_MESSAGE_FORMAT aMessage;
			aMessage.aType = MESSAGE_TYPE_SET_SCAN_DIRECTORY;
			wcscpy_s(aMessage.wszFileName, MAX_FILE_NAME_LENGTH, wszParameter.LetPtr());

			printf("Changing directory to: %ls\n", wszParameter.LetPtr());

			BYTE pReply[MAX_STRING_LENGTH];
			DWORD dwReplySize = sizeof(pReply);
			hr = pConnector->SendMessage((LPVOID)&aMessage, sizeof(aMessage), pReply, &dwReplySize);
			RETURN_IF_FAILED(hr);

			if (dwReplySize > 0)
			{
				printf("Recieved response: %ls\n", (WCHAR*)pReply);
			}
		} 
		else
		if (wcscmp(wszCommand.LetPtr(), L"message") == 0)
		{
			wscanf_s(L"%ls", wszParameter.LetPtr(), MAX_FILE_NAME_LENGTH);

			FSD_MESSAGE_FORMAT aMessage;
			aMessage.aType = MESSAGE_TYPE_PRINT_STRING;
			wcscpy_s(aMessage.wszFileName, MAX_FILE_NAME_LENGTH, wszParameter.LetPtr());

			printf("Sending message: %ls\n", wszParameter.LetPtr());

			BYTE pReply[MAX_STRING_LENGTH];
			DWORD dwReplySize = sizeof(pReply);
			hr = pConnector->SendMessage((LPVOID)&aMessage, sizeof(aMessage), pReply, &dwReplySize);
			RETURN_IF_FAILED(hr);

			if (dwReplySize > 0)
			{
				printf("Recieved response: %ls\n", (WCHAR*)pReply);
			}
		}
		else
		if (wcscmp(wszCommand.LetPtr(), L"exit") == 0)
		{
			fExit = true;
			printf("Exiting FSDManager\n");
		}
		else
		{
			printf("Invalid command: %ls\n", wszCommand.LetPtr());
		}
	}

	return S_OK;
}

HRESULT HrMain()
{
	HRESULT hr = S_OK;

	CAutoHandle hInputParserThread;
	hr = UtilCreateThreadSimple(&hInputParserThread, (LPTHREAD_START_ROUTINE)UserInputParser, NULL);
	RETURN_IF_FAILED(hr);

	hr = WaitForSingleObject(hInputParserThread.LetPtr(), INFINITE);
	RETURN_IF_FAILED(hr);

	/*CFSDPortConnectorMessage aMessage = {};
	hr = aConnector.RecieveMessage(&aMessage);
	RETURN_IF_FAILED(hr);

	if (aMessage.aRecieveHeader.ReplyLength)
	{
		printf("New message recieved: %s\n", aMessage.pBuffer);
	}
	*/
	return S_OK;
}

