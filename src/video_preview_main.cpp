//============================================================================
// Name        : video_preview_main.cpp
// Author      : Abhishek
// Version     :
// Copyright   : Your copyright notice
// Description : main file.
//============================================================================

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <iostream>
#include <sys/mman.h>
#include "Box.h"
#include "mp4_err.h"
#include "mp4_parser.h"
#include "log.h"

typedef	char	*caddr_t;

using namespace std;

int main(int argc, char *argv[])
{
	int fd;
	int page_size;
	void *mmap_ret;
	uint8_t *fptr = NULL;
	struct stat sbuf;

	MP4_ASSERT(2 == argc, return -1, "Usage: <filename>\n");
	fd = open(argv[1], O_RDONLY);
	MP4_ASSERT(fd >= 0, return -1, "open() for file failed\n");
	
	if (stat(argv[1], &sbuf) == -1) {
		perror("stat");
		return -1;
	}
	mmap_ret = mmap((caddr_t)0, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(mmap_ret == (void *)(-1)) {
		perror("mmap");
		return -1;
	}

	ByteBuffer fileBuffer = ByteBuffer(mmap_ret, sbuf.st_size);	
	Mp4Parser parser = Mp4Parser(fileBuffer);	
	/*ready to use mmaped file.*/
	parser.parseMp4();
	parser.print();
	VID_LOG("test", VID_INFO, "vide trak offset %lu\n", parser.searchVideTrak());

	return 0;
}

/*START
 * 1. parse Mp4
 * 2. process Meta Data
 * 3. use meta data to extract IFrames and save in new File.
 * 4. make OutPut mp4 file.
 * a) generate stbl and write in output file, use dummy info in absence of info later update it.
 * b) generate mdat and write in output file.
 * c) update boxes where dummy info was used. 
 * Final Output.
 * END
 * */
