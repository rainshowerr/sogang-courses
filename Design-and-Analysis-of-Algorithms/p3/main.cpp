#include "mp3.h"

vector<string> hufCode(128);

int main(int argc, char **argv){
	if (!strcmp(argv[1], "-c"))
		incoding(argv);
	else if (!strcmp(argv[1], "-d"))
		decoding(argv);
	else {
		cout << "error : invalid option" << endl;
		exit(1);
	}
}
