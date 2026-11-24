/*
vector.h

created 11/18/26 by Valentina Susarret
*/

/*
example:

#define VECTOR_T int
#define VECTOR_PREFIX ivector_
#define VECTOR_NAME ivector
#include "vector.h"

// repeat above lines for any vector types

int main() {
  ivector v = ivector_init(10);
  ivector_push(&v, 3);
  int x = ivector_at(&v);
  ivector_delete(&s);
  return 0;
}
*/

#ifndef VECTOR_HEADER_H
#define VECTOR_HEADER_H

#include <stdlib.h>

#define VECTOR_IMPL(word) VECTOR_COMB1(VECTOR_PREFIX,word)
#define VECTOR_COMB1(pre, word) VECTOR_COMB2(pre, word)
#define VECTOR_COMB2(pre, word) pre##word

#endif

#ifndef VECTOR_T
#error "VECTOR_T must be defined"
#endif

#ifndef VECTOR_NAME
#define VECTOR_NAME VECTOR_COMB1(VECTOR_COMB1(vector,_), VECTOR_T)
#endif

#ifndef VECTOR_PREFIX
#define VECTOR_PREFIX VECTOR_COMB1(VECTOR_NAME, _)
#endif

#ifndef VECTOR_LINKAGE
#define VECTOR_LINKAGE static inline
#endif

typedef struct VECTOR_NAME VECTOR_NAME;
struct VECTOR_NAME {
    VECTOR_T *data;
    size_t n;
    size_t m;
};

#define VECTOR_init VECTOR_IMPL(init)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
VECTOR_NAME
VECTOR_init(size_t m);

#else

VECTOR_LINKAGE
VECTOR_NAME
VECTOR_init(size_t m) {
	VECTOR_NAME v = (VECTOR_NAME){.n = 0, .m = m, .data = NULL};
  v.data = calloc(v.m, sizeof(*v.data));
  return v;
}
#endif

#define VECTOR_init_ptr VECTOR_IMPL(init_ptr)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
VECTOR_NAME
VECTOR_init_ptr(VECTOR_T **data, size_t n);

#else

VECTOR_LINKAGE
VECTOR_NAME
VECTOR_init_ptr(VECTOR_T **data, size_t n) {
	VECTOR_NAME v = (VECTOR_NAME){.n = n, .m = n, .data = NULL};
  v.data = calloc(v.m, sizeof(*v.data));
  for (size_t i = 0; i < n; i++) {
    v.data[i] = (*data)[i];
  }
  return v;
}
#endif

#define VECTOR_size VECTOR_IMPL(size)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
size_t
VECTOR_size(VECTOR_NAME *v);

#else

VECTOR_LINKAGE
size_t
VECTOR_size(VECTOR_NAME *v) {return v->n;}
#endif

#define VECTOR_at VECTOR_IMPL(at)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
int
VECTOR_at(VECTOR_NAME *v);

#else

VECTOR_LINKAGE
int
VECTOR_at(VECTOR_NAME *v, size_t idx, VECTOR_T *out_value) {
  if (idx < v->n) {
    *out_value = v->data[idx];
    return 1;
  }
  return 0;
}
#endif

#define VECTOR_ptr VECTOR_IMPL(ptr)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
VECTOR_T*
VECTOR_ptr(VECTOR_NAME *s);

#else

VECTOR_LINKAGE
VECTOR_T*
VECTOR_ptr(VECTOR_NAME *v) {return v->data;}
#endif

#define VECTOR_push VECTOR_IMPL(push)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
void
VECTOR_push(VECTOR_NAME* s, VECTOR_T x);

#else

VECTOR_LINKAGE
void
VECTOR_push(VECTOR_NAME *v, VECTOR_T x){
	if (v->n == v->m) {
		v->m = v->m * 2;
		v->data = realloc(v->data, sizeof(VECTOR_T) * v->m);
	}
	v->data[v->n++] = x;
}
#endif

#define VECTOR_clear VECTOR_IMPL(clear)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
void
VECTOR_clear(VECTOR_NAME *v);

#else

VECTOR_LINKAGE
void
VECTOR_clear(VECTOR_NAME *v) {v->n = 0; }
#endif

#define VECTOR_delete VECTOR_IMPL(delete)

#ifdef VECTOR_DECLS_ONLY

VECTOR_LINKAGE
void
VECTOR_delete(VECTOR_NAME *v);

#else

VECTOR_LINKAGE
void
VECTOR_delete(VECTOR_NAME *v) { free(v->data); v->data = NULL; }
#endif

#undef VECTOR_T
#undef VECTOR_PREFIX
#undef VECTOR_NAME
#undef VECTOR_LINKAGE
#undef VECTOR_push
#undef VECTOR_pop
#ifdef VECTOR_DECLS_ONLY
#undef VECTOR_DECLS_ONLY
#endif