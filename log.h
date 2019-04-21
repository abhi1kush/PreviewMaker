#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>
#define LOG_FILE stdout
#define VID_ERROR "ERR"
#define VID_INFO "INF"
#define VID_WARN "WARN"

#define PRINT_MSG(msg, ...) fprintf(LOG_FILE, msg, ##__VA_ARGS__);
#define PRINT_ASSERT_MSG(msg, ...) fprintf(LOG_FILE,"[ASSERT FAILED] fun: %s, line: %d" msg, __func__, __LINE__, ##__VA_ARGS__);

#define VID_LOG(moudule, prio, msg, ...) do {\
	fprintf(LOG_FILE, "[%s] : {%s : line %d : fun %s} : ", prio, __FILE__, __LINE__, __func__);\
	fprintf(LOG_FILE, msg"\n", ##__VA_ARGS__);\
	} while (0)
#endif
