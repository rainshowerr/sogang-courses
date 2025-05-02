#include "mp3.h"

void decoding(char **argv) {
	// file open
	ifstream inputFile(argv[2]);
	if (inputFile.fail()) {
		cout << "error : file does not exist" << endl;
		exit(1);
	}

	// hufman code 읽어들이기 & tree 재생성
	for(int i = 0; i < 128; i++) {
		string s;
		getline(inputFile, s);
		hufCode[i] = s;
	}
	Node* root = rebuildHufftree(hufCode);

	// padding 읽어들이기
	string padding;
	getline(inputFile, padding);

	// 압축 해제을 위한 임시 텍스트(2진수 string) 작성
	char c;
	string binaryText;
	while (inputFile.get(c)) {
		bitset<8> bits(c);
		binaryText += bits.to_string();
	}
	inputFile.close();

	// delete padding
	binaryText.erase(binaryText.length() - 8, stoi(padding));

	// // 허프만코드에 해당하는 비트를 아스키코드로 변환
	ofstream outputFile(string(argv[2]) + ".yy");
	convertToAscii(move(outputFile), binaryText, root);
	outputFile.close();
}

Node* rebuildHufftree(vector<string>& hufCode) {
    Node* root = new Node;

    for (int i = 0; i < hufCode.size(); i++) {
        Node* n = root; // 시작 위치
        for (int j = 0; j < hufCode[i].length(); j++) {
            if (hufCode[i][j] == '0') {
                if (!n->left)
                    n->left = new Node;
                n = n->left;
            } else {
                if (!n->right)
                    n->right = new Node;
                n = n->right;
            }
        }
        n->symbol = i; // symbol 초기화
    }
    return root;
}

void convertToAscii(ofstream outputFile, string& binaryText, Node* root) {
    Node* n = root;

    for (int i = 0; i < binaryText.length(); i++) {
        if (binaryText[i] == '0')
            n = n->left;
		else
            n = n->right;
        if (n->symbol) {
            outputFile << n->symbol;
            n = root;  // 탐색 시작 위치 root로 초기화
        }
    }
}