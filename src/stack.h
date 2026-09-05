/*
stack.h
created 9/4/26 by Valentina Susarret
*/

/*
example:

#include "stack.h"
int main() {
  stack_t(int) s;
  stack_init(s);
  stack_push(s, 3);
  int top = stack_peek(s);
  int pop = stack_pop(s);
  stack_destroy(s);
  return 0;
}
*/

#ifndef STACK_HEADER_H
#define STACK_HEADER_H

#include <stdlib.h>

#define STACK_INIT_SIZE 	( 256 )

#define stack_t(type) 		struct { uint64_t n, m; type *a; }
#define stack_init(s) 		( { (s).n = 0; (s).m = (STACK_INIT_SIZE); (s).a = calloc((s).m, sizeof(*(s).a)); (s); } )
#define stack_clear(s)		( { (s).n = 0; } )
#define stack_delete(s) 	( { free((s).a); (s).a = NULL; } )
#define stack_peek(s) 		( ((s).n > 0) ? ((s).a[(s).n-1]) : (0) )
#define stack_pop(s) 		  ( (s).n > 0 ? (s).a[--(s).n] : (0) )
#define stack_ptr(s)		  ( (s).a )
#define stack_size(s) 		( (s).n )

#define stack_push(s, x) do {								          \
		if ((s).n == (s).m) {								              \
			(s).m = (s).m * 2;								              \
			(s).a = realloc((s).a, sizeof(*(s).a) * (s).m);	\
		}													                        \
		(s).a[(s).n++] = (x);								              \
	} while (0)

#endif