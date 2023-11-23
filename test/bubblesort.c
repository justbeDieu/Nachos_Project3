#include "syscall.h"
#define MAXSIZE 100

int main()
{
	int arr[MAXSIZE];
	int n, i, j, temp, selection;
	
	// Nhap so luong phan tu
	PrintString("Nhap do dai mang: ");
	n = ReadInt();
	if (n < 0)
	{
		PrintString("Loi\n");
		Halt();
	}
	if (n > MAXSIZE)
	{
		PrintString("Do dai toi da la 100\n");
		Halt();
	}

	// Nhap mang
	PrintString("Nhap mang:\n");
	for (i = 0; i < n; i++)
	{
		arr[i] = ReadInt();
	}
	
	PrintString("Nhap 1 de sap xep tang dan: ");
	selection = ReadInt();
	if (selection)
	{
	// Bubble sort ascending
	for (i = 0; i < n - 1; i++) {
		for ( j = 0; j < n - i -1; j++) {
			if (arr[j] > arr[j+1]) {
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
	} else
	{
		// Bubble sort descending
	for (i = 0; i < n - 1; i++) {
		for ( j = 0; j < n - i -1; j++) {
			if (arr[j] < arr[j+1]) {
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
	}
	PrintString("In mang:\n");
	for (i = 0; i < n; i++)
	{
		PrintInt(arr[i]);
		PrintChar(' ');
	}
	PrintChar('\n');
	Halt();
}
