#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

/* Function pointers to hw3 functions */
void* (*mm_malloc)(size_t);
void* (*mm_realloc)(void*, size_t);
void (*mm_free)(void*);

static void* try_dlsym(void* handle, const char* symbol) {
  char* error;
  void* function = dlsym(handle, symbol);
  if ((error = dlerror())) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }
  return function;
}

static void load_alloc_functions() {
  void* handle = dlopen("hw3lib.so", RTLD_NOW);
  if (!handle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  mm_malloc = try_dlsym(handle, "mm_malloc");
  mm_realloc = try_dlsym(handle, "mm_realloc");
  mm_free = try_dlsym(handle, "mm_free");
}

int main() {
  load_alloc_functions();

  /*int* data = mm_malloc(sizeof(int));
  assert(data != NULL);
  data[0] = 0x162;
  */
  printf("sbrk pointer %p\n", (int*)sbrk(0));
  int *data1 = mm_malloc(sizeof(int));
  printf("data1 pointer %p\n",data1);
  printf("sbrk pointer %p\n",(int*)sbrk(0));
  int *data2 = mm_malloc(2*sizeof(int));
  printf("data2 pointer %p\n",data2);
  printf("sbrk pointer %p\n",(int*)sbrk(0));
  mm_free(data2);
  printf("sbrk pointer %p\n", (int*)sbrk(0));
  puts("malloc test successful!");
}
