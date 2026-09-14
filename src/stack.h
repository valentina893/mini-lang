/*
stack.h

created 9/4/26 by Valentina Susarret

updated 9/9/26
- changed implementation using David Priver's guide on Template Headers.
  - Templates in C: https://www.davidpriver.com/ctemplates.html
*/

/*
example:

#define STACK_T int
#define STACK_PREFIX istack_
#define STACK_NAME istack
#include "stack.h"

// repeat above lines for any stack types

int main() {
  istack s = istack_init(10);
  istack_push(&s, 3);
  int top = istack_peek(&s);
  int pop = istack_pop(&s);
  istack_delete(&s);
  return 0;
}
*/

#ifndef STACK_HEADER_H
#define STACK_HEADER_H

#include <stdlib.h>

#define STACK_IMPL(word) STACK_COMB1(STACK_PREFIX,word)
#define STACK_COMB1(pre, word) STACK_COMB2(pre, word)
#define STACK_COMB2(pre, word) pre##word

#endif

#ifndef STACK_T
#error "STACK_T must be defined"
#endif

#ifndef STACK_NAME
#define STACK_NAME STACK_COMB1(STACK_COMB1(stack,_), STACK_T)
#endif

#ifndef STACK_PREFIX
#define STACK_PREFIX STACK_COMB1(STACK_NAME, _)
#endif

#ifndef STACK_LINKAGE
#define STACK_LINKAGE static inline
#endif

typedef struct STACK_NAME STACK_NAME;
struct STACK_NAME {
    STACK_T *data;
    size_t n;
    size_t m;
};

#define STACK_init STACK_IMPL(init)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
STACK_NAME
STACK_init(size_t m);

#else

STACK_LINKAGE
STACK_NAME
STACK_init(size_t m) {
	STACK_NAME s = (STACK_NAME){.n = 0, .m = m, .data = NULL};
  s.data = calloc(s.m, sizeof(*s.data));
  return s;
}
#endif

#define STACK_size STACK_IMPL(size)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
size_t
STACK_size(STACK_NAME *s);

#else

STACK_LINKAGE
size_t
STACK_size(STACK_NAME *s) {return s->n;}
#endif

#define STACK_ptr STACK_IMPL(ptr)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
STACK_T*
STACK_ptr(STACK_NAME *s);

#else

STACK_LINKAGE
STACK_T*
STACK_ptr(STACK_NAME *s) {return s->data;}
#endif

#define STACK_push STACK_IMPL(push)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
void
STACK_push(STACK_NAME* s, STACK_T x);

#else

STACK_LINKAGE
void
STACK_push(STACK_NAME *s, STACK_T x){
	if (s->n == s->m) {
		s->m = s->m * 2;
		s->data = realloc(s->data, sizeof(STACK_T) * s->m);
	}
	s->data[s->n++] = x;
}
#endif

#define STACK_peek STACK_IMPL(peek)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
STACK_T
STACK_peek(STACK_NAME *s);

#else

STACK_LINKAGE
STACK_T
STACK_peek(STACK_NAME *s) { return (s->n > 0) ? (s->data[s->n-1]) : (s->data[s->n]); }
#endif

#define STACK_pop STACK_IMPL(pop)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
STACK_T
STACK_pop(STACK_NAME *s);

#else

STACK_LINKAGE
STACK_T
STACK_pop(STACK_NAME *s) { return (s->n > 0) ? (s->data[--s->n]) : (s->data[s->n]); }
#endif

#define STACK_clear STACK_IMPL(clear)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
void
STACK_clear(STACK_NAME *s);

#else

STACK_LINKAGE
void
STACK_clear(STACK_NAME *s) {s->n = 0; }
#endif

#define STACK_delete STACK_IMPL(delete)

#ifdef STACK_DECLS_ONLY

STACK_LINKAGE
void
STACK_delete(STACK_NAME *s);

#else

STACK_LINKAGE
void
STACK_delete(STACK_NAME *s) { free(s->data); s->data = NULL; }
#endif

#undef STACK_T
#undef STACK_PREFIX
#undef STACK_NAME
#undef STACK_LINKAGE
#undef STACK_push
#undef STACK_pop
#ifdef STACK_DECLS_ONLY
#undef STACK_DECLS_ONLY
#endif