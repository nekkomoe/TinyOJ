#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace tool {
	namespace commonOpera {
		int string2int(string str) {
			return atoi(str.c_str());
		}

		string int2string(int cnt) {
			stringstream sstr;
			sstr << cnt;
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
		unsigned int getFileHash(string fileName) {
			/*
				usage: getFileHash(string fileName)
				return: int
				function: get the file ELF Hash
			*/
			ifstream t(fileName);  
			string str((istreambuf_iterator<char>(t)),
		                istreambuf_iterator<char>());
			return hash :: ELFHash(str);
		}

		bool compareFileWithELFHash(string fileA, string fileB) {
			/*
				usage: compareFileWithELFHash(string fileA, string fileB)
				return: bool
				function: compare file with ELF Hash
			*/
			return getFileHash(fileA) == getFileHash(fileB);
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
				function: execute a command
				warning: the function isn't safe
			*/

			string cmd;
			cmd = command + " & { sleep " + time + "; eval 'kill -9 $!' &> /dev/null; }";
			system(cmd.c_str());
		}
	}
}

typedef enum {
	/*
		only two status
	*/
	JD_ACCEPT,
	JD_UNACCEPT

} JD_EXIT_CODE;

void copyData(string iopath, string from, string suffix, int cnt) {
	system(("cp " + iopath + from + tool :: commonOpera :: int2string(cnt) + suffix + " " + from + suffix).c_str());
}

JD_EXIT_CODE judge(string cppName, string dataCount, string inpath, string inputName, string outputName, string outpath, string stdOutputName, string time) {
	/*
		usage: judge(string cppName, string dataCount, string inpath, string inputName, string outpath, string outputName, string stdOutputName, string time)
		return: void
		function: execute a command
		warning: the function isn't safe
	*/
	tool :: exec :: execWithTime("g++ -o " + cppName + " " + cppName + ".cpp", "2.0");
	int cnt = tool :: commonOpera :: string2int(dataCount), i;
	for(i = 1 ; i <= cnt ; ++ i) {
		copyData(inpath, inputName, ".in", i);
		tool :: exec :: execWithTime("./" + cppName, time);
		if(!tool :: file :: compareFileWithELFHash(outputName + ".out", outpath + stdOutputName + tool :: commonOpera :: int2string(i) + ".out")) {
			break;
		}
	}
	tool :: exec :: exec("rm " + inputName + ".in");
	tool :: exec :: exec("rm " + outputName + ".out");
	if(i <= cnt) {
		cout << "JD_UNACCEPT" << endl;
		return JD_UNACCEPT;
	} else {
		cout << "JD_ACCEPT" << endl;
		return JD_ACCEPT;
	}
}

int main(int argc, char *argv[]) {
	tool :: commonOpera :: closeStreamSync();
	if(argc >= 9) {
		cout << judge(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]) << endl;
	} else {
		cout << "usage: ./judge [cppName] [dataCount] [inpath] [inputName] [outputName] [outpath] [stdOutputName] [time(s)]" << endl;
	}
}
