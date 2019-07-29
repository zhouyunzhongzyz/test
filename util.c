/*
 * util.c
 *
 *  Created on: Jul 15, 2019
 *      Author: zyz
 */


#include <string.h>
#include <sys/types.h>

size_t strlcpy(char* dst, const char* src, size_t siz){

	char* d = dst;
	const char* s = src;
	size_t n = siz;

	if(n != 0){
		while( --n != 0){
			if((*d++ = *s++) == '\0')
				break;
		}
	}

	if(n == 0){
		if(siz != 0){
			*d = '\0';
		while(*s++);
		}
	}

	return (s - src -1);


}

long ltoi(const long val1,const long val2,const long val3,const long val4){
    long val = 0;
    val |= val4 << 24;
    val |= val3 << 16;
    val |= val2 << 8;
    val |= val1;
    return val;
}


long _ntol(const char* str){
	long a,b,c,d;
	sscanf(str,"%ld.%ld.%ld.%ld",&a,&b,&c,&d);
    return ltoi(a,b,c,d);

}













