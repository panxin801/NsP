#include "NsPc_test.h"

#define FS_FIX                      16000
#define KSMAPLES_FIX                160

int doProcess(char* arrTempRead, char* arrTempWrite)
{
	NsHandle* NsInst;
	UWord32 fs = FS_FIX;
	int mode = 3;
	short speechFrame[160];
	short* speechFrameHB = NULL;
	short outFrame[160];
	short* outFrameHB = NULL;
	FILE * fAnsIn, *fAnsOut;


	//创建ns的一个类
	if (Ns_Create(&NsInst) != 0)
	{
		printf("ns create is erro!");
		return -1;
	}

	//初始化ns的相关参数
	if (Ns_Init(NsInst, fs) != 0)
	{
		printf("ns init is erro!");
		return -1;
	}

	//初始化ns的相关参数
	if (Ns_set_policy(NsInst, mode) != 0)
	{
		printf("ns set is erro!");
		return -1;
	}

	fAnsIn = fopen(arrTempRead, "rb");  // Input file
	char chWavHeader[44] = { 0 };
	if (fAnsIn == NULL)
	{
		printf("open %s is failed", "fAnsIn.pcm");
		return -1;
	}
	fread(chWavHeader, sizeof(char), 44, fAnsIn); // Read header from input .wav files

	fAnsOut = fopen(arrTempWrite, "wb"); // Write file
	if (fAnsOut == NULL)
	{
		printf("open %s is failed", "fAnsOut.pcm");
		return -1;
	}
	fwrite(chWavHeader, sizeof(char), 44, fAnsOut); // Write header into saveFile

	while (fread(speechFrame, 2, 160, fAnsIn) != 0)
	{
		if (Ns_Process(NsInst, speechFrame, speechFrameHB, outFrame, outFrameHB) != 0)
		{
			printf("ns process is erro!");
			return -1;
		}
		fwrite(outFrame, 2, 160, fAnsOut);
	}


	if (Ns_Free(NsInst) != 0)
	{
		printf("ns free is erro!");
		return -1;
	}

	fclose(fAnsIn);
	fclose(fAnsOut);
	return 1;
}

int main()
{
	char* pIn = "E:\\test\\";
	char* pFilter = "*.wav"; // file filter
	char arrFindIn[100]; // input path and filter name
	char* pOut = "E:\\test-out\\";
	strcpy(arrFindIn, pIn);
	strcat(arrFindIn, pFilter);

	struct _finddata_t filedir;
	long lfDir;
	if ((lfDir = _findfirst(arrFindIn, &filedir)) == -1l)
	{
		printf("No file is found\n");
	}
	else
	{
		printf("file list:\n");
		do
		{
			char arrTempRead[100]; // store each one of file
			char arrTempWrite[100];
			strcpy(arrTempRead, pIn);
			strcat(arrTempRead, filedir.name);
			strcpy(arrTempWrite, pOut);
			strcat(arrTempWrite, filedir.name);
			if (doProcess(arrTempRead,arrTempWrite))
			{
				printf("%s read and write successful.\n", filedir.name);
			}
			else
			{
				printf("Error occurs during procesing file: %s\n", filedir.name);
				return -1;
			}
		} while (_findnext(lfDir, &filedir) == 0);
	}
	_findclose(lfDir);

	system("pause");
	return 0;
}