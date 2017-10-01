#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

typedef enum {
	/*
		only two status
	*/
	JD_ACCEPT,
	JD_UNACCEPT

} JD_EXIT_CODE;

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

unsigned int ELFHash(string str) {

	/*
		ELF Hash Function
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

unsigned int getFileHash(string fileName) {
	ifstream t(fileName);  
	string str((istreambuf_iterator<char>(t)),
                istreambuf_iterator<char>());
	// cout << fileName << ':' << str << ':' << ELFHash(str) << endl;
	return ELFHash(str);
}

bool compareFile(string fileA, string fileB) {
	return getFileHash(fileA) == getFileHash(fileB);
}

void exec(string command) {
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

int string2int(string str) {
	return atoi(str.c_str());
}

string int2string(int cnt) {
	stringstream sstr;
	sstr << cnt;
	return sstr.str();
}

void copyData(string iopath, string from, string suffix, int cnt) {
	system(("cp " + iopath + from + int2string(cnt) + suffix + " " + from + suffix).c_str());
}

JD_EXIT_CODE judge(string cppName, string dataCount, string inpath, string inputName, string outputName, string outpath, string stdOutputName, string time) {
	/*
		usage: judge(string cppName, string dataCount, string inpath, string inputName, string outpath, string outputName, string stdOutputName, string time)
		return: void
		function: execute a command
		warning: the function isn't safe
	*/
	exec("g++ -o " + cppName + " " + cppName + ".cpp");
	int cnt = string2int(dataCount);
	for(int i = 1 ; i <= cnt ; ++ i) {
		copyData(inpath, inputName, ".in", i);
		execWithTime("./" + cppName, time);
		cout << "Judging in case " << i << ": " << endl;
		cout << "user output(ELFHash): " << getFileHash(outputName + ".out") << endl;
		cout << "standard output(ELFHash): " << getFileHash(outpath + stdOutputName + int2string(i) + ".out") << endl;
		cout << endl;
		if(!compareFile(outputName + ".out", outpath + stdOutputName + int2string(i) + ".out")) {
			cout << inputName + ".out" << endl;
			exec("rm " + inputName + ".in");
			exec("rm " + outputName + ".out");
			cout << "JD_UNACCEPT" << endl;
			return JD_UNACCEPT;
		}
	}
	exec("rm " + inputName + ".in");
	exec("rm " + outputName + ".out");
	cout << "JD_ACCEPT" << endl;
	return JD_ACCEPT;
}

int main(int argc, char *argv[]) {
	closeStreamSync();
	if(argc >= 9) {
		judge(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
	} else {
		cout << "usage: ./judge [cppName] [dataCount] [inpath] [inputName] [outputName] [outpath] [stdOutputName] [time(s)]" << endl;
	}
	// system("open status");
}
