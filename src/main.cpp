#include "..\include\mdfile.h"
#include "..\include\mdsystem.h"
#include "..\include/ui.h"
#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	MdSystem ms("sysfile\\table.txt");
	MdSystem *Ms = &ms;
	enter_main(Ms);
	
	return 0;
}
