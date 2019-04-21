#ifndef __MP4_H__
#define __MP4_H__
#include "log.h"

#define MP4_ASSERT(expr, msg, stmt) if (!(expr)) {PRINT_ASSERT_MSG(msg); stmt;}

#define MP4_ERROR_CODES(X) X( 0, MP4_ERR_OK, "Succes!! No error!!\n") \
	                   X(-1, MP4_ERR_FAILED, "ERROR OCCURED!! FAILED \n")          \
	                   X(-2, MP4_EINVAL, "Invalid parameter passed\n")  \
	                   X(-3, MP4_INVAL_OFFSET, "Invalid box offset\n")  \
	                   X(-3, MP4_PARSE_FAILED, "Invalid box offset\n")  \

#define MP4_ERROR_ENUM(ID, NAME, ERROR_MSG) NAME = ID,
#define MP4_ERROR_MSG(ID, NAME, ERROR_MSG) case ID: return ERROR_MSG;

#define RETURN_ON_FAIL(cond, stmt) if (!(cond)) {return stmt;}
typedef int err_t;

enum {
	MP4_ERROR_CODES(MP4_ERROR_ENUM)
};

const char *error_msg(int code);
#endif
