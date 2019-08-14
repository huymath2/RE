// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <stdio.h>
#include <windows.h>
#include <string.h>


void WrongText() {
	printf("I don't like your name! Change it :)))");
}

void anti_dbg() {
	if (IsDebuggerPresent()) {
		printf("I hate you! Are you kidding me? You dared hack me -_-\n");
		Sleep(500);
		printf("Get out of here!");
		ExitProcess(0);
	}
}

int cmp(char s1[], char s2[]) {
	for (int i = 0; i < strlen(s1); ++i) {
		if (s1[i] != s2[i]) return 0;
	}
	return 1;
}

void CheckName(char name[]) {
	int c = name[0];
	int len = strlen(name);
	if (len != 16) {
		WrongText();
		ExitProcess(0);
	}
	for (int i = 0; i < len - 1; ++i) {
		int x = (int)name[i];
		int y = (int)name[i + 1];
		if ((x < 0x41 || x > 0x5A) || (y < 0x41 || y > 0x5A)) {
			WrongText();
			ExitProcess(0);
		}
		int z = (x + y) % 26;
		for (int j = 0; j < len; ++j) {
			name[j] += z;
			name[j] %= 26;
		}
	}
	printf("%s", name);
}

int main() {
	printf("******Nobita4x4m********\n");
	Sleep(1000);
	printf("Welcome to ISP CLub!\n");
	Sleep(1000);
	printf("This is first RE challenge :))\n");
	printf("Please give me your name: ");
	char name[30];
	scanf_s("%s", &name);
	anti_dbg();
	CheckName(name);
}




