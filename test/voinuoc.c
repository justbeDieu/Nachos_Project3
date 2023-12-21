#include "syscall.h"
#include "copyright.h"

void main()
{
	int f_Success;
	SpaceId  si_voinuoc, si_result;	// id file
	char c_readFile;	
	int v1, v2;		
	int v;			
	int flag_done_result;	

	v1 = v2 = 0;
	
	while (1)
	{
		Wait("m_vn");

		si_result = Open("result.txt", 0);
		if (si_result == -1)
		{		
			Signal("sinhvien");
			return;
		}
		while (1)
		{
			Wait("voinuoc");
			c_readFile = 0;
			si_voinuoc = Open("voinuoc.txt", 1);
			if (si_voinuoc == -1)
			{
				Close(si_result);
				Signal("sinhvien");

				return;
			}

			v = 0;
			flag_done_result = 0;
			while (1)
			{
				if (Read(&c_readFile, 1, si_voinuoc) == -2)
				{
					Close(si_voinuoc);
					break;
				}
				if (c_readFile != '*')
				{
					v = v * 10 + (c_readFile - 48);
				}
				else
				{
					flag_done_result = 1;
					Close(si_voinuoc);
					break;
				}

			}
			if (v != 0)
			{
				// Dung voi 1
				if (v1 <= v2)
				{
					v1 += v;
					Write("1", 1, si_result);
				}
				else	// Dung voi 2
				{
					v2 += v;
					Write("2", 1, si_result);

				}
			}

			if (flag_done_result == 1)
			{
				v1 = v2 = 0;
				Close(si_result);
				Signal("sinhvien");
				break;
			}

			Signal("sinhvien");
		}
	}
}