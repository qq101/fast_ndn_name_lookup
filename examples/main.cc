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
	cuckoohash_map<char *, int, std::hash<char *> >Table;

	char buf[256];
	char buf1[256];
	char buf2[128];
	char* fib_file_name = argv[1];
	int len, id;
	double start = gettime();
	int line = 0;

//Insert the key into cockoo hash table
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
		id = atoi(buf2);
		if (len != 0) {
			//cout << "len=" << len << " buf1=" << buf1 << " id=" << id << endl;
			Table[buf1]=id;
			line++;
		}
	}
	fib_file.close();

	cout << "insert line:" << line << "---------------";
	cout << "insert lookup:" << gettime() - start  << "s" << endl;
	return 0;
}