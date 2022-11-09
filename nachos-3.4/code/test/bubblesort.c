#include "syscall.h"
int main()
{
	int i, j;
	int n;
	int arr[100];
	PrintString("\n=============== Chuong trinh sort ===============\n");

	// User input
	while(1)
	{
		PrintString("Xin moi nhap so luong phan tu (n <= 100):\nn = ");
		// Đầu tiên, ta cho người dùng nhập vào biến n bằng hàm ReadInt() có sẵn
		n = ReadInt();

		if( n <= 0 || n > 100)
		{
			PrintString("So luong phan tu phai la so nguyen tu 1 -> 100. Moi thu lai!\n");
			continue;
		}
		else
			break;
	}

	// lưu các giá trị của mảng cũng thông qua hàm ReadInt(). 
	for (i = 0; i < n; i++) {
		PrintString("Phan tu thu ");
		// hàm PrintInt(), PrintString() để xuất ra mảng vừa nhập.
		PrintInt(i);
		PrintString(": ");
		arr[i] = ReadInt();
	}

	// Bubble Sort
	for (i = 0; i < n-1; i++) {
		for (j = 0; j < n-i-1; j++) {
			if (arr[j] > arr[j+1]) {
				// Swap arr[j] and arr[j+1]
				arr[j] = arr[j] + arr[j+1];
				arr[j+1] = arr[j] - arr[j+1];
				arr[j] = arr[j] - arr[j+1];
			}
		}
	}
	// Print new order
	PrintString("\nSau khi sap xep: ");
	for (i = 0; i < n; i++) {
		if (i != 0) {PrintString(", ");}
		PrintInt(arr[i]);
	}
	PrintString("\n==================================================\n\n");
	return 0;
}