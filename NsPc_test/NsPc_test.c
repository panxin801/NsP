#include "NsPc_test.h"

#define FS_FIX                      16000
#define KSMAPLES_FIX                160
#define WAV_HEADER					46

int doProcess(char* arrTempRead, char* arrTempWrite)
{
	NsHandle* NsInst;
	UWord32 fs = FS_FIX;
	int mode = 2;
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
	char chWavHeader[WAV_HEADER] = { 0 };
	if (fAnsIn == NULL)
	{
		printf("open %s is failed", "fAnsIn.pcm");
		return -1;
	}
	fread(chWavHeader, sizeof(char), WAV_HEADER, fAnsIn); // Read header from input .wav files

	fAnsOut = fopen(arrTempWrite, "wb"); // Write file
	if (fAnsOut == NULL)
	{
		printf("open %s is failed", "fAnsOut.pcm");
		return -1;
	}
	fwrite(chWavHeader, sizeof(char), WAV_HEADER, fAnsOut); // Write header into saveFile

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

int main(int argc, char* argv[])
//int main()
{
	/*if (2 > (argc - 1))
	{
		printf("Too few params. Please check.\n");
		return 1;
	}
	printf("%d\n", argc);
	printf("%s\n", argv[1]);
	printf("%s\n", argv[2]);*/


	char* pIn = "E:\\test-noise\\";
	/*char* pIn = argv[1];*/
	char* pFilter = "*.wav"; // file filter
	char arrFindIn[100]; // input path and filter name
	char* pOut = "E:\\nspcout\\";
	/*char* pOut = argv[2];*/
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
			char arrTempRead[256]; // store each one of file
			char arrTempWrite[256];
			strcpy(arrTempRead, pIn);
			strcat(arrTempRead, filedir.name);
			strcpy(arrTempWrite, pOut);
			strcat(arrTempWrite, filedir.name);
			if (doProcess(arrTempRead, arrTempWrite))
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

	printf("All processing successed!\n");
	//system("pause");
	return 0;
}