#include "syscall.h"
#include "copyright.h"

#define MAX_LENGTH 32


int main()
{
	int f_Success; 
	SpaceId si_input, si_output, si_sinhvien, si_result;	// id file
	int SLTD;	
	char c_readFile;

	//Semaphore
	f_Success = CreateSemaphore("main", 0);
	if (f_Success == -1)
		return 1;
	f_Success = CreateSemaphore("sinhvien", 0);
	if (f_Success == -1)
		return 1;
	f_Success = CreateSemaphore("voinuoc", 0);
	if (f_Success == -1)
		return 1;
	f_Success = CreateSemaphore("m_vn", 0);
	if (f_Success == -1)
		return 1;

	// Tao file output.txt 
	f_Success = CreateFile("output.txt");
	if (f_Success == -1)
		return 1;

	// Mo file input.txt 
	si_input = Open("input.txt", 1);
	if (si_input == -1)
		return 1;

	// Mo file output.txt 
	si_output = Open("output.txt", 0);
	if (si_output == -1)
	{
		Close(si_input);
		return 1;
	}

	SLTD = 0;
	while (1)
	{
		Read(&c_readFile, 1, si_input);
		if (c_readFile != '\n')
		{
			if (c_readFile >= '0' && c_readFile <= '9')
				SLTD = SLTD * 10 + (c_readFile - 48);
		}
		else
			break;
	}

	f_Success = Exec("./test/sinhvien");
	if (f_Success == -1)
	{
		Close(si_input);
		Close(si_output);
		return 1;
	}

	f_Success = Exec("./test/voinuoc");
	if (f_Success == -1)
	{
		Close(si_input);
		Close(si_output);
		return 1;
	}

	while (SLTD--)
	{
		f_Success = CreateFile("sinhvien.txt");
		if (f_Success == -1)
		{
			Close(si_input);
			Close(si_output);
			return 1;
		}

		si_sinhvien = Open("sinhvien.txt", 0);
		if (si_sinhvien == -1)
		{
			Close(si_input);
			Close(si_output);
			return 1;
		}
		while (1)
		{
			if (Read(&c_readFile, 1, si_input) < 1)
			{
				break;
			}
			if (c_readFile != '\n')
			{
				Write(&c_readFile, 1, si_sinhvien);
			}
			else
				break;

		}
		Close(si_sinhvien);

		Down("sinhvien");

		Up("main");

		si_result = Open("result.txt", 1);
		if (si_result == -1)
		{
			Close(si_input);
			Close(si_output);
			return 1;
		}

		PrintString("\n Lan thu: ");
		PrintInt(SLTD);
		PrintString("\n");


		si_sinhvien = Open("sinhvien.txt", 0);
		if (si_sinhvien == -1)
		{
			Close(si_input);
			Close(si_output);
			return 1;
		}

		while (1)
		{
			if (Read(&c_readFile, 1, si_sinhvien) < 1)
			{
				Write("\r\n", 2, si_output);
				Close(si_result);
				Down("m_vn");
				break;
			}

			Write(&c_readFile, 1, si_output);

			if (Read(&c_readFile, 1, si_result) < 1)
			{
				Write("\r\n", 2, si_output);
				Close(si_result);
				Down("m_vn");
				break;
			}

			Write(&c_readFile, 1, si_output);
			Write(" ", 1, si_output);

		}

	}

	Close(si_sinhvien);
	Close(si_input);
	Close(si_output);
	return 0;

}
