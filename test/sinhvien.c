#include "syscall.h"
#include "copyright.h"

void main()
{
	int f_Success; 
	SpaceId si_sinhvien, si_voinuoc;	// id file
	char c_readFile;	
	int flag_VN;		
	int flag_MAIN;		
	int lengthFile;		
	int i_File;		

	Signal("m_vn");

	while (1)
	{
		lengthFile = 0;

		Wait("sinhvien");


		f_Success = CreateFile("result.txt");
		if (f_Success == -1)
		{
			Signal("main"); 
			return;
		}

		si_sinhvien = Open("sinhvien.txt", 1);
		if (si_sinhvien == -1)
		{
			Signal("main"); 
			return;
		}

		lengthFile = Seek(-1, si_sinhvien);
		Seek(0, si_sinhvien);
		i_File = 0;

		f_Success = CreateFile("voinuoc.txt");
		if (f_Success == -1)
		{
			Close(si_sinhvien);
			Signal("main"); 
			return;
		}


		si_voinuoc = Open("voinuoc.txt", 0);
		if (si_voinuoc == -1)
		{
			Close(si_sinhvien);
			Signal("main"); 
			return;
		}

		while (i_File < lengthFile)
		{
			flag_VN = 0;
			Read(&c_readFile, 1, si_sinhvien);
			if (c_readFile != ' ')
			{
				Write(&c_readFile, 1, si_voinuoc);
			}
			else
			{
				flag_VN = 1;
			}
			if (i_File == lengthFile - 1)
			{
				Write("*", 1, si_voinuoc);
				flag_VN = 1;
			}


			if (flag_VN == 1)
			{
				Close(si_voinuoc);
				Signal("voinuoc");
				Wait("sinhvien");

				f_Success = CreateFile("voinuoc.txt");
				if (f_Success == -1)
				{
					Close(si_sinhvien);
					Signal("main");
					return;
				}

				si_voinuoc = Open("voinuoc.txt", 0);
				if (si_voinuoc == -1)
				{
					Close(si_sinhvien);
					Signal("main"); 
					return;
				}

			}
			i_File++;

		}
		Signal("main");
	}
}