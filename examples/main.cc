#include "iostream"
#include "string.h"
#include "sys/time.h"
#include "assert.h"
#include "stdio.h"
#include "fstream"
#include "stdlib.h"
#include "../src/cuckoohash_map.hh"
using namespace std;

static double gettime(void) {
	struct timeval now_tv;
	gettimeofday (&now_tv, NULL);
	return ((double)now_tv.tv_sec) + ((double)now_tv.tv_usec) / 1000000.0;
}

int main(int argc, char *argv[])
{
	cuckoohash_map<std::string, int, std::hash<std::string> >Table;

	char buf[256];
	char buf1[256];
	char buf2[128];
	char* fib_file_name = argv[1];
	char* trace_file_name = argv[2];
	int len, id;
	double start = gettime();
	int line = 0;

//Insert the key into cuckoo hash table
	fstream fib_file;
	fib_file.open(fib_file_name, ios::in);
	if (!fib_file.is_open())
	{
		cout << "Error open fib_file!" << endl;
		exit(1);
	}
	while (!fib_file.eof()) {
		memset(buf, 0x00, sizeof (char) * 256);
		memset(buf1, 0x00, sizeof (char) * 256);
		fib_file.getline(buf, sizeof buf, '\n');
		sscanf(buf, "%[^,]", buf1);
		sscanf(buf, "%*[^,],%s", buf2);
		len = strlen(buf1);
		buf1[len] = '\0';
		string str1(buf1);
		id = atoi(buf2);
		if (len != 0) {
			//cout << "len=" << len << " buf1=" << str1 << " id=" << id << endl;
			Table[str1] = id;
			line++;
		}
	}
	fib_file.close();
	cout << "---------------------------------------------------------|" << endl;
	cout << endl << "insert line:" << line << "------------------";
	cout << "insert time:" << gettime() - start  << "s" << endl << endl;


//Print the cuckoo hash table
	start = gettime();
	fstream trace_file;
	trace_file.open(trace_file_name, ios::in);
	if (!trace_file.is_open())
	{
		cout << "Print: Error open trace_file!" << endl;
		exit(1);
	}
	int matched_line = 0;
	line = 0;
	while (!trace_file.eof()) {
		memset(buf, 0x00, sizeof (char) * 256);
		trace_file.getline(buf, sizeof buf, '\n');
		len = strlen(buf);
		buf[len] = '\0';
		string str2(buf);
		if (len != 0) {
			int out;
			int result = Table.find(str2, out);
			if (result) {
				matched_line++;
			}
			line++;
		}
	}
	trace_file.close();
	cout << "---------------------------------------------------------|" << endl;
	cout << "total  line" << line << endl;
	cout << "matched line:" << matched_line << "------------------";
	cout << "lookup time:" << gettime() - start  << "s" << endl;
	cout << "---------------------------------------------------------|" << endl;
	return 0;
}