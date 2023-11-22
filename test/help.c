#include "syscall.h"


int main()
{
	// TT nhom
	PrintString("\t THONG TIN THANH VIEN\n");

	PrintString(" 21120221 - Huynh Phuc Dat\n");
	PrintString(" 21120221 - Son Bao Di\n");
	PrintString(" 21120222 - Hoang Thi Dieu\n");
	PrintString(" 21120238 - Nguyen Thu Ha\n");
	PrintString(" 21120579 - Phan Thi Kieu Trinh\n\n");

	// Mo ta chuong trinh
	PrintString("\t THONG TIN CHUONG TRINH\n");
	PrintString(" Chuong trinh ascii:\n");
	PrintString(" In ra bang ma ascii\n");
	PrintString(" Chay lenh:\n\t .userprog/nachos -rs 1023 -x /test/ascii\n\n");

	PrintString(" Chuong trinh sort:\n");
	PrintString(" Nhap so luong phan tu va phap tung phan tu\n");
	PrintString(" Chay lenh:\n\t .userprog/nachos -rs 1023 -x /test/bubblesort\n\n");
	
	Halt();
}
