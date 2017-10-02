#pragma GCC optimize(2)

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace cache {
	
	namespace widget {

		int string2int(string str) {
	
			/*
				usage: string2int(string str)
				return: int
				function: convert string to int
			*/
		
			return atoi(str.c_str());
		}

		string int2string(int val) {
	
			/*
				usage: int2string(int cnt)
				return: string
				function: convert integer to string
			*/
		
			stringstream sstr;
			sstr << val;
			return sstr.str();
		}

		void closeStreamSync() {
	
			/*
				usage: closeStreamSync()
				return: void
				function: close the stream sync, to let 'cin' and 'cout' be quicklier
			*/
		
			ios :: sync_with_stdio(0);
			cin.tie(0);
			cout.tie(0);
		}

	}

	namespace hash {

		unsigned int ELFHash(string str) {

			/*
				usgae: ELFHash(string str)
				return: int
				function: return the ELF Hash of the string
			*/

		    unsigned int hash = 0;
		    unsigned int x = 0;
		    for(int i = 0 ; str[i] ; ) {
		        hash = (hash << 4) + (str[i ++]);
		        if((x = hash & 0xF0000000L) != 0) {  
		            hash ^= (x >> 24);
		            hash &= ~x;
		        }
		    }  
		    return (hash & 0x7FFFFFFF);  
		}

	}

	namespace file {
		
		string readFile(string fileName) {
		
			/*
				usage: readFile(string fileName)
				return: string
				function: get the file content
			*/

			ifstream t(fileName);
			string str((istreambuf_iterator<char>(t)),istreambuf_iterator<char>());
			return str;
		}
		
		unsigned int getFileHash(string fileName) {
		
			/*
				usage: getFileHash(string fileName)
				return: int
				function: get the file ELF Hash
			*/
		
			return hash :: ELFHash(readFile(fileName));
		}

		bool compareFileWithELFHash(string fileA, string fileB) {
		
			/*
				usage: compareFileWithELFHash(string fileA, string fileB)
				return: bool
				function: compare file with bytes
			*/
		
			return readFile(fileA) == readFile(fileB);
		}
		
		bool exists(string fileName) {
		
			/*
				usage: exists(string fileName)
				return: bool
				function: check if file exists
			*/
		
			fstream f;
     		f.open(fileName,ios :: in);
     		return !!f;
		}
		
		void copy(string from, string to) {
		
			/*
				usage: copy(string from, string to)
				return: void
				function: copy file
			*/
		
			system(("cp " + from + " " + to).c_str());
		}

	}

	namespace exec {
		
		void exec(string command) {
		
			/*
				usage: exec(string command)
				return: void
				function: execute the command
			*/
		
			system(command.c_str());
		}
		
		void execWithTime(string command, string time = "1.0") {
		
			/*
				usage: exec(string command[string], string time[s])
				return: void
				function: execute the command
			*/

			string cmd;
			cmd = command + " & { sleep " + time + "; eval 'kill -9 $!' &> /dev/null; }";
			system(cmd.c_str());
		}

	}

}

void judge(string cppName, string dataCount, string inpath, string inputName, string outputName, string outpath, string stdOutputName, string time) {
	/*
		usage: judge(string cppName, string dataCount, string inpath, string inputName, string outpath, string outputName, string stdOutputName, string time)
		return: void
		function: judge the program
	*/
	cache :: exec :: execWithTime("g++ -o " + cppName + " " + cppName + ".cpp &> /dev/null", "1.0");
	if(!cache :: file :: exists(cppName)) {
		cout << "\033[;33mCompile Error / Compile Time Limit Exceed\033[0m" << endl;
		return;
	}

	int cnt = cache :: widget :: string2int(dataCount);
	for(int i = 1 ; i <= cnt ; ++ i) {

		string stdInputFile = inpath + inputName + cache :: widget :: int2string(i) + ".in";
		string userInputFile = inputName + ".in";

		string userOutputFile = outputName + ".out";
		string stdOutputFile = outpath + stdOutputName + cache :: widget :: int2string(i) + ".out";

		cache :: file :: copy(stdInputFile, userInputFile);
		cache :: exec :: execWithTime("./" + cppName, time);

		string userOutput = cache :: file :: readFile(userOutputFile);
		string stdOutput = cache :: file :: readFile(stdOutputFile);

		unsigned int userHash = cache :: hash :: ELFHash(userOutput);
		unsigned int stdHash = cache :: hash :: ELFHash(stdOutput);

		cout << "Judging in case " << i << ": \t";
		cout << "userELF(" << userHash << ")" << "\t" << "stdELF(" << stdHash << ")";
		
		if(userOutput == stdOutput) {
			cout << "\t\033[;32mAccept\033[0m" << endl;
		} else if(stdOutput.find(userOutput) != string :: npos) {
			cout << "\t\033[;34mTime Limit Exceed\033[0m" << endl;
		} else {
			cout << "\t\033[;31mWrong Answer\033[0m" << endl;
		}
	}

	// remove temp files
	cache :: exec :: exec("rm " + inputName + ".in");
	cache :: exec :: exec("rm " + outputName + ".out");
	cache :: exec :: exec("rm " + cppName);
}

int main(int argc, char *argv[]) {
	cache :: widget :: closeStreamSync();
	if(argc >= 9) {
		judge(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
	} else {
		cout << "usage: ./judge [cppName] [dataCount] [inpath] [inputName] [outputName] [outpath] [stdOutputName] [time(s)]" << endl;
	}
}
