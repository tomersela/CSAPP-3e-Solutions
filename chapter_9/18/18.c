#include "include/csapp.h"
#include "malloc/mm.h"
#include "malloc/memlib.h"

int main(void)
{
	size_t size = 32;
	mem_init();
	void *p1 = mm_malloc(size);
	printf("p1 = %#x\n", p1);
	void *p2 = mm_malloc(size);
	printf("p2 = %#x\n", p2);
	void *p3 = mm_malloc(size);
	printf("p3 = %#x\n", p3);
	mm_free(p2);
	void *p4 = mm_malloc(size);
	printf("p4 = %#x\n", p4);
	mm_free(p1);
	mm_free(p3);
	mm_free(p4);
	
	exit(0);
}