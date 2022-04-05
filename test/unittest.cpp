#include "pch.h"
#include "CppUnitTest.h"
#include "../WordChain.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static int generateExample(std::vector<char*> &strVec, std::string input) {
	std::istringstream inputStream(input);
	std::string out;
	while (inputStream >> out) {
		char* tmpPtr = (char*)malloc(sizeof(char) * (out.size() + 1));
		out += '\0';
		strcpy_s(tmpPtr, out.size(), out.c_str());
		strVec.push_back(tmpPtr);
	}

	return strVec.size();
}


static bool validateNChain(char* result) {
	std::string line(result);
	std::istringstream inputStream(line);
	std::string out;
	std::vector<std::string> strVec(0);
	while (inputStream >> out) {
		strVec.push_back(out);
	}

	if (strVec.size() < 2) {
		return false;
	}

	char last = '#';
	for (auto word : strVec) {
		if (last == '#') {
			last = word[word.length() - 1];
		}
		else {
			if (last != word[0]) {
				return false;
			}
			else {
				last = word[word.length() - 1];
			}
		}
	}
	return true;
}

static bool validateChain(char* result[], int len) {
	std::vector<char*> charPtrVec(result, result + len);
	std::vector<std::string> strVec(0);

	for (auto charPtr : charPtrVec) {
		std::string tmpStr(charPtr);
		strVec.push_back(tmpStr);
	}

	if (len != strVec.size()) {
		return false;
	}

	if (strVec.size() < 2) {
		return false;
	}

	char last = '#';
	for (auto word : strVec) {
		if (last == '#') {
			last = word[word.length() - 1];
		}
		else {
			if (last != word[0]) {
				return false;
			}
			else {
				last = word[word.length() - 1];
			}
		}
	}
	return true;
}


namespace unittest
{
	TEST_CLASS(unittest)
	{
	public:
		TEST_METHOD(TestCoreN_1) {
			char* result[MAX_RESULT_LINE];
			int correctLen = 6;
			std::vector<char*> words(0);
			int inputSize = generateExample(words, "ab bc cd de");
			Assert::AreEqual(Core::gen_chains_all(&(words[0]), inputSize, result), correctLen);
			for (int i = 0; i < correctLen; i++) {
				Assert::IsTrue(validateNChain(result[i]));
			}
		}

		TEST_METHOD(TestCoreN_2) {
			char* result[MAX_RESULT_LINE];
			int correctLen = 10;
			std::vector<char*> words(0);
			int inputSize = generateExample(words, "image my encounter repeat tim");
			Assert::AreEqual(Core::gen_chains_all(&(words[0]), inputSize, result), correctLen);
			for (int i = 0; i < correctLen; i++) {
				Assert::IsTrue(validateNChain(result[i]));
			}
		}

		TEST_METHOD(TestCoreN_3) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 1;
			int inputSize = generateExample(words, "impose insufficient inpolite sucide close loss");

			Assert::AreEqual(Core::gen_chains_all(&(words[0]), inputSize, result), correctLen);
			for (int i = 0; i < correctLen; i++) {
				Assert::IsTrue(validateNChain(result[i]));
			}
		}

		TEST_METHOD(TestCoreN_4) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 10;
			int inputSize = generateExample(words, "inter release debug gel loi");

			Assert::AreEqual(Core::gen_chains_all(&(words[0]), inputSize, result), correctLen);
			for (int i = 0; i < correctLen; i++) {
				Assert::IsTrue(validateNChain(result[i]));
			}
		}

		TEST_METHOD(TestCoreN_5) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 14;
			int inputSize = generateExample(words, "test losl glg ksk sks slt tsl");

			Assert::AreEqual(Core::gen_chains_all(&(words[0]), inputSize, result), correctLen);
			for (int i = 0; i < correctLen; i++) {
				Assert::IsTrue(validateNChain(result[i]));
			}
		}

		TEST_METHOD(TestCoreN_6) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 0;
			int inputSize = generateExample(words, "test losl glg ksk sks");

			Assert::AreEqual(Core::gen_chains_all(&(words[0]), inputSize, result), correctLen);
			for (int i = 0; i < correctLen; i++) {
				Assert::IsTrue(validateNChain(result[i]));
			}
		}

		TEST_METHOD(TestCoreM_1) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 3;
			int inputSize = generateExample(words, "ab bc cd ef fg gh");

			Assert::AreEqual(Core::gen_chain_word_unique(&(words[0]), inputSize, result), correctLen);
			Assert::IsTrue(validateChain(result, correctLen));
		}

		TEST_METHOD(TestCoreM_2) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 13;
			int inputSize = generateExample(words, "ab bc cd de ee xy yz zg ef ff fg gg gh hi ij jk kl lm mn");

			Assert::AreEqual(Core::gen_chain_word_unique(&(words[0]), inputSize, result), correctLen);
			Assert::IsTrue(validateChain(result, correctLen));
		}

		TEST_METHOD(TestCoreM_3) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = -1;
			int inputSize = generateExample(words, "ab bc cd da");

			Assert::AreEqual(Core::gen_chain_word_unique(&(words[0]), inputSize, result), correctLen);
		}

		TEST_METHOD(TestCoreM_4) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 0;
			int inputSize = generateExample(words, "ab ac ad");

			Assert::AreEqual(Core::gen_chain_word_unique(&(words[0]), inputSize, result), correctLen);
		}

		TEST_METHOD(TestCoreC_1) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 0;
			int inputSize = generateExample(words, "ab bc cd de ef fg gh hz word direct tes shi information");

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 0, false), 29);
			Assert::IsTrue(validateChain(result, 7));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 'z', false), 16);
			Assert::IsTrue(validateChain(result, 8));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'd', 'i', false), 12);
			Assert::IsTrue(validateChain(result, 3));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'i', 'z', false), 0);
		}

		TEST_METHOD(TestCoreC_2) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int correctLen = 0;
			int inputSize = generateExample(words, "ab bc cd de ef fg gh word daaaaaaaaz");

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 0, false), 16);
			Assert::IsTrue(validateChain(result, 4));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 'h', false), 14);
			Assert::IsTrue(validateChain(result, 7));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'd', 'h', false), 8);
			Assert::IsTrue(validateChain(result, 4));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'i', 'z', false), 0);
		}

		TEST_METHOD(TestCoreW_1) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int inputSize = generateExample(words, "ab bc cd de ee xy yz zg ef ff fg gg gh hi ij jk kl lm mn");

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 0, false), 16);
			Assert::IsTrue(validateChain(result, 15));

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 'k', false), 13);
			Assert::IsTrue(validateChain(result, 13));

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 'e', 'k', false), 9);
			Assert::IsTrue(validateChain(result, 9));

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 'i', 'z', false), 0);
		}

		TEST_METHOD(TestCoreR_1) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int inputSize = generateExample(words, "aba alot tof fya");

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 0, true), 4);
			Assert::IsTrue(validateChain(result, 4));

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 'f', true), 4);
			Assert::IsTrue(validateChain(result,4));

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 't', 'a', true), 3);
			Assert::IsTrue(validateChain(result, 3));

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 'i', 'z', true), 0);
			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 0, false), -1);

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 0, true), 13);
			Assert::IsTrue(validateChain(result, 4));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 'f', true), 13);
			Assert::IsTrue(validateChain(result, 4));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'a', 'f', true), 10);
			Assert::IsTrue(validateChain(result, 3));

			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'i', 'z', true), 0);
			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 0, false), -1);
		}

		TEST_METHOD(TestCoreR_2) {
			char* result[MAX_RESULT_LINE];
			std::vector<char*> words(0);

			int inputSize = generateExample(words, "rabid stream public alight");

			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 0, true), 0);
			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 0, 'f', true), 0);
			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 't', 'a', true), 0);
			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 'i', 'z', true), 0);

			
			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 0, true), 0);
			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 0, 'f', true), 0);
			Assert::AreEqual(Core::gen_chain_char(&(words[0]), inputSize, result, 'a', 'f', true), 0);
			Assert::AreEqual(Core::gen_chain_word(&(words[0]), inputSize, result, 'i', 'z', true), 0);
		}

		TEST_METHOD(TestCmd_1) {
			string cmd = "wordlist.exe -n noring.txt";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("6\nab bc\nab bc cd\nab bc cd de\nbc cd\nbc cd de\ncd de\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmd_2) {
			string cmd = "wordlist.exe -c temp.txt -r";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("de\nea\nab\nbc\ncd\ndd\ndb\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmd_3) {
			string cmd = "wordlist.exe -w temp.txt -r";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("de\nea\nab\nbc\ncd\ndd\ndb\n", call_by_cmd(cmd.size(), cmdArray));
		}


		TEST_METHOD(TestCmdError_1_1) {
			string cmd = "wordlist.exe -c temp.txt -another.txt";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("multiple files found\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_1_2) {
			string cmd = "wordlist.exe -n temp.txt -g";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("-g is neither a text file nor an argument\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_2_1) {
			string cmd = "wordlist.exe -w temp.txt -h K -h g";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("multiple head\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_2_2) {
			string cmd = "wordlist.exe -w temp.txt -t k -t g";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("multiple tail\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_3) {
			string cmd = "wordlist.exe -w temp.txt another.txt";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("multiple files found\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_4) {
			string cmd = "wordlist.exe -w another.txt";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("can't find file\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_5) {
			string cmd = "wordlist.exe -w";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("no input file\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_6) {
			string cmd = "wordlist.exe temp.txt";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("unmatch type\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_7) {
			string cmd = "wordlist.exe -w temp.txt";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("has cycle\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_8_1) {
			string cmd = "wordlist.exe -w temp.txt -h";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("need letter after \'-h\'\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_8_2) {
			string cmd = "wordlist.exe -w temp.txt -t";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("need letter after \'-t\'\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdError_9) {
			string cmd = "wordlist.exe -w temp.txt -n -c";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("multiple type\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdWarning_1) {
			string cmd = "wordlist.exe -n temp.txt -r";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("warning: cycle allowance is ignored\nhas cycle\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdWarning_2) {
			string cmd = "wordlist.exe -n temp.txt -h f";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("warning: head letter specification is ignored\nhas cycle\n", call_by_cmd(cmd.size(), cmdArray));
		}

		TEST_METHOD(TestCmdWarning_3) {
			string cmd = "wordlist.exe -n temp.txt -t f";
			cmd += '\0';
			char* cmdArray = (char*)malloc(sizeof(char) * (cmd.size() + 1));
			if (cmdArray != NULL) {
				strcpy_s(cmdArray, cmd.size(), cmd.c_str());
			}
			Assert::AreEqual("warning: tail letter specification is ignored\nhas cycle\n", call_by_cmd(cmd.size(), cmdArray));
		}
	};
}
