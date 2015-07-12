/*
* @Author: anchen
* @Date:   2015-05-19 10:17:54
* @Last Modified by:   anchen
* @Last Modified time: 2015-06-06 00:25:34
*/
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>  // gettimeofday
#include "src/new_art.h"
#include "src/new_art.c"
#include "src/cuckoohash_map.hh"

static double gettime(void)
{
	cuckoohash_map<int, std::string, std::hash<int> >Table;
	struct timeval now_tv;
	gettimeofday (&now_tv, NULL);
	return ((double)now_tv.tv_sec) + ((double)now_tv.tv_usec) / 1000000.0;
}
int main(int argc, char**argv) {
	art_tree t;
	int res = art_tree_init(&t);
	if (res != 0) exit(1);
	printf("init_art_size=%"PRIu64"\n", art_size(&t));
	char *prefix_file_name;
	char *trace_file_name;

// test_insert
	prefix_file_name = argv[1];
	double start = gettime();
	uintptr_t line = 1;
	int len;
	uintptr_t id;
	char buf[512];
	char buf1[256];	//储存NDN名字
	char buf2[8];	//储存相应端口号
	FILE *f0 = fopen(prefix_file_name, "r");
	//FILE *f2 = fopen("name_trace.txt", "at+");//文件f2用来收集名字
	if (f0 == NULL) {
		//printf("error!\n");
		exit(1);
	}
	while (fgets(buf, sizeof buf, f0)) {
		sscanf(buf, "%[^,]", buf1);
		sscanf(buf, "%*[^,],%s", buf2);
		len = strlen(buf1);
		buf1[len] = '\0';
		//f//printf(f2, "%s\n",buf1);
		line++;
	}
	//fclose(f2);
	double  read_file_time = gettime() - start;
	printf("total read file number: %ld , time: %f\n", line - 1, read_file_time );

//Insert_node
	rewind(f0);
	start = gettime();
	line = 0;
	while (fgets(buf, sizeof buf, f0))
	{
		//处理端口号
		sscanf(buf, "%[^,]", buf1);
		sscanf(buf, "%*[^,],%s", buf2);
		len = strlen(buf1) + 1;
		buf1[len - 1] = '\0';
		////printf("len=%d ", len);
		////printf("buf1=%s", buf1 );
		id = atoi(buf2);
		////printf(" id=%d\n", id);
		line++;
		art_insert(&t, (unsigned char*)buf1, len, (void*)(id + 1));

	}
	fclose(f0);
	printf("total insert number: %ld ,insert line number: %ld , time: %f\n", line, art_size(&t), gettime() - start);


	trace_file_name = argv[2];
	start = gettime();
	FILE *f_trace = fopen(trace_file_name, "r");
	//fseek(f_trace, 0, SEEK_SET);

// Search
	line = 0;
	int search_line = 0;
	while (fgets(buf, sizeof buf, f_trace))
	{
		len = strlen(buf);
		buf[len - 1] = '\0';
		uintptr_t val = (uintptr_t)art_search(&t, (unsigned char*)buf, len);
		if (val != 0)
		{
			search_line++;
			printf("Searched_Line: %d Port: %" PRIuPTR " Str: %s\n", search_line, val - 1, buf);
		}
		line++;
	}
	fclose(f_trace);
	printf("total lookup number: %ld , find  line number: %d , time: %f\n", line, search_line, gettime() - start );

//test_destroy
	res = art_tree_destroy(&t);
	if (res != 0) exit(1);
	printf("destroy_art_size=%"PRIu64"\n", art_size(&t));

	return 0;
}