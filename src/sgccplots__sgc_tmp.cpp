
#include <type_traits>

#include <stdbool.h>

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __add__(T t, T2 t2) {
    return t + t2;
}

template <typename T>
static inline __attribute__((always_inline)) T* __add__(T* t, int t2) {
    return t + t2;
}

template <typename T>
static inline __attribute__((always_inline)) T* __add__(int t, T* t2) {
    return t + t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __sub__(T t, T2 t2) {
    return t - t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __mul__(T t, T2 t2) {
    return t * t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __div__(T t, T2 t2) {
    return t / t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __shl__(T t, T2 t2) {
    return t << t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __shr__(T t, T2 t2) {
    return t >> t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __mod__(T t, T2 t2) {
    return t % t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __or__(T t, T2 t2) {
    return t | t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __and__(T t, T2 t2) {
    return t & t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __lor__(T t, T2 t2) {
    return t || t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __land__(T t, T2 t2) {
    return t && t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __xor__(T t, T2 t2) {
    return t ^ t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __lt__(T t, T2 t2) {
    return t < t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __gt__(T t, T2 t2) {
    return t > t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __le__(T t, T2 t2) {
    return t <= t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __ge__(T t, T2 t2) {
    return t >= t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __eq__(T t, T2 t2) {
    return t == t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) typename std::common_type<T, T2>::type __neq__(T t, T2 t2) {
    return t != t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) int __eq__(T* t, T2 t2) {
    return t == t2;
}

template <typename T, typename T2>
static inline __attribute__((always_inline)) int __neq__(T* t, T2 t2) {
    return t != t2;
}

template <typename T>
static inline __attribute__((always_inline)) T* __index__(T* t, int* idx, int nidx) {
    return &t[idx[0]];
}

//Yes, this is gross and bad but I like syntax sugar. If you want performance, just do *(ptr + idx).

static inline __attribute__((always_inline)) int _n_var_args(){
    return 0;
}

template <typename T, typename... Args>
static inline __attribute__((always_inline)) int _n_var_args(T i, Args... args){
    return 1 + _n_var_args(args...);
}

extern "C" void* GC_malloc(int);
extern "C" void* GC_realloc(void*,int);
extern "C" void GC_free(void*);

template <typename T>
static inline __attribute__((always_inline)) void _vaptr_helper(T* out, int n){

}

template <typename T, typename... Args>
static inline __attribute__((always_inline)) void _vaptr_helper(T* out, int n, T i, Args... args){
    out[n] = i;
    _vaptr_helper(out,n+1,args...);
}

template <typename T, typename... Args>
static inline __attribute__((always_inline)) T* _var_args_to_ptr(T i, Args... args){
    int n = _n_var_args(i,args...);
    T* out = (T*)GC_malloc(sizeof(T) * n);
    _vaptr_helper(out,0,i,args...);
    return out;
}

//#define __index_wrapper__(var,...) (*__index__(var,(int[]){__VA_ARGS__},_n_var_args(__VA_ARGS__)))
#define __index_wrapper__(var,...) (*__index__(var,_var_args_to_ptr(__VA_ARGS__),_n_var_args(__VA_ARGS__)))

static inline const char* __str__(_Bool i){
    if (i == false)return "false";
    return "true";
}

template <typename T>
static inline const char* __str__(T a){
    return "NULL";
}

template<typename T>
static inline const char* to_str(T in);

extern "C" int printf(const char* format, ... );

static inline void print(){

}

template <typename T, typename... Args>
static inline void print(T t, Args... args) {
    printf("%s ",to_str(t));
    print(args...);
}

#define println(...) print(__VA_ARGS__);printf("\n")

extern "C" void exit(int);

#define __global__ 

#define __generate(...) 

#define warpsize 32

#define LAUNCH_KERNEL(func,numBlocks,blockSize,...) \
{\
    for (int _bidx = 0; _bidx < numBlocks; _bidx++){\
        _Pragma("omp parallel for")\
        for (int _warpidx = 0; _warpidx < blockSize/(warpsize/2); _warpidx++){\
            for (int _modid = 0; _modid < (warpsize/2); _modid++){\
                func(make_int3(_bidx,0,0),make_int3(blockSize,1,1),make_int3(_warpidx * (warpsize/2) + _modid,0,0), __VA_ARGS__);\
            }\
        }\
    }\
}

 
#define __bool_true_false_are_defined 1

#define NULL nullptr

//#ifndef NULL

//#endif

#line 3 "/Users/humzaqureshi/GitHub/sugaryc/include/../cstdlib/stdlib_macros.h"
typedef 
#line 3 "/Users/humzaqureshi/GitHub/sugaryc/include/../cstdlib/stdlib_macros.h"
unsigned long size_t;

#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"
extern "C" 
#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"
void *malloc(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"

#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"
long unsigned int);

#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"
extern "C" 
#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"
void free(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"

#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/stdlib.sgh"
void *);

#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *s);

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
float f);

#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
double f);

#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
short a);

#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
int a);

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
long int a);

#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
const 
#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
char *__str__(
#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"

#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/print.sgh"
long long int a);

#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
struct _int_tuple;

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
typedef 
#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
struct _int_tuple *int_tuple;

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int_tuple make_int_tuple(
#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int *raw, 
#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int length);

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
const 
#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int *__index__(
#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int_tuple t, 
#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int *idx, 
#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int nidx);

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int len(
#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int_tuple t);

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
const 
#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
char *__str__(
#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/int_tuple.sgh"
int_tuple t);

#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
typedef const 
#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
char *string;

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string __add__(
#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string left, 
#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string right);

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string __mul__(
#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string left, 
#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
int right);

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string __mul__(
#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
int left, 
#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 10 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string right);

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
int __eq__(
#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string left, 
#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 12 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string right);

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
int __neq__(
#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string left, 
#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string right);

#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
int len(
#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"

#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/string.sgh"
string str);

#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 20 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float x;
  
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float y;
} float2;

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float2 make_float2(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float x, 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float y)

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float2 out;
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float2 vec)

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"float2", 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 37 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float x;
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float y;
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float z;
} float3;

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float3 make_float3(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float x, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float y, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float z)

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float3 out;
  
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float3 vec)

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"float3", 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float x;
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float y;
  
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float z;
  
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float w;
} float4;

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float4 make_float4(
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float x, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float y, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float z, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float w)

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float4 out;
  
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w = 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w;
  
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
float4 vec)

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"float4", 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 20 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double x;
  
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double y;
} double2;

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double2 make_double2(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double x, 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double y)

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double2 out;
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double2 vec)

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"double2", 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 37 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double x;
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double y;
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double z;
} double3;

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double3 make_double3(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double x, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double y, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double z)

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double3 out;
  
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double3 vec)

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"double3", 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double x;
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double y;
  
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double z;
  
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double w;
} double4;

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double4 make_double4(
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double x, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double y, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double z, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double w)

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double4 out;
  
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w = 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w;
  
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
double4 vec)

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"double4", 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 20 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short x;
  
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short y;
} short2;

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short2 make_short2(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short x, 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short y)

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short2 out;
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short2 vec)

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"short2", 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 37 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short x;
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short y;
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short z;
} short3;

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short3 make_short3(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short x, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short y, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short z)

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short3 out;
  
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short3 vec)

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"short3", 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short x;
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short y;
  
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short z;
  
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short w;
} short4;

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short4 make_short4(
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short x, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short y, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short z, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short w)

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short4 out;
  
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w = 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w;
  
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
short4 vec)

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"short4", 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 20 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int x;
  
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int y;
} int2;

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int2 make_int2(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int x, 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int y)

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int2 out;
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int2 vec)

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"int2", 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 37 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int x;
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int y;
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int z;
} int3;

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int3 make_int3(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int x, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int y, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int z)

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int3 out;
  
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int3 vec)

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"int3", 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int x;
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int y;
  
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int z;
  
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int w;
} int4;

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int4 make_int4(
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int x, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int y, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int z, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int w)

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int4 out;
  
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w = 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w;
  
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
int4 vec)

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"int4", 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 20 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 21 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long x;
  
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long y;
} long2;

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long2 make_long2(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long x, 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long y)

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long2 out;
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long2 vec)

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"long2", 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 37 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long x;
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long y;
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long z;
} long3;

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long3 make_long3(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long x, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long y, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long z)

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long3 out;
  
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long3 vec)

#line 51 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"long3", 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
typedef 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
struct 
{
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long x;
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long y;
  
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long z;
  
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long w;
} long4;

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long4 make_long4(
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long x, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long y, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long z, 
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long w)

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
{
  
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long4 out;
  
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x = 
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x;
  
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y = 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y;
  
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z = 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z;
  
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out.
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w = 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w;
  
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 69 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
inline static 
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string __str__(
#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 72 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
long4 vec)

#line 72 "src/../include/raylib.sgh"
{
  
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
string out = 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
__add__(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"long4", 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"{"), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
x)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
y)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
z)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
", "), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
to_str(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
vec.
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
w)), 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
"}");
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
return 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/vector_definition.sgh"
out;
}


#line 213 "src/../include/raylib.sgh"
typedef 
#line 210 "src/../include/raylib.sgh"
struct Vector2
{
  
#line 211 "src/../include/raylib.sgh"

#line 211 "src/../include/raylib.sgh"
float x;
  
#line 212 "src/../include/raylib.sgh"

#line 212 "src/../include/raylib.sgh"
float y;
} Vector2;

#line 220 "src/../include/raylib.sgh"
typedef 
#line 216 "src/../include/raylib.sgh"
struct Vector3
{
  
#line 217 "src/../include/raylib.sgh"

#line 217 "src/../include/raylib.sgh"
float x;
  
#line 218 "src/../include/raylib.sgh"

#line 218 "src/../include/raylib.sgh"
float y;
  
#line 219 "src/../include/raylib.sgh"

#line 219 "src/../include/raylib.sgh"
float z;
} Vector3;

#line 228 "src/../include/raylib.sgh"
typedef 
#line 223 "src/../include/raylib.sgh"
struct Vector4
{
  
#line 224 "src/../include/raylib.sgh"

#line 224 "src/../include/raylib.sgh"
float x;
  
#line 225 "src/../include/raylib.sgh"

#line 225 "src/../include/raylib.sgh"
float y;
  
#line 226 "src/../include/raylib.sgh"

#line 226 "src/../include/raylib.sgh"
float z;
  
#line 227 "src/../include/raylib.sgh"

#line 227 "src/../include/raylib.sgh"
float w;
} Vector4;

#line 231 "src/../include/raylib.sgh"
typedef 
#line 231 "src/../include/raylib.sgh"
Vector4 Quaternion;

#line 239 "src/../include/raylib.sgh"
typedef 
#line 234 "src/../include/raylib.sgh"
struct Matrix
{
  
#line 235 "src/../include/raylib.sgh"

#line 235 "src/../include/raylib.sgh"
float m0;
  
#line 235 "src/../include/raylib.sgh"

#line 235 "src/../include/raylib.sgh"
float m4;
  
#line 235 "src/../include/raylib.sgh"

#line 235 "src/../include/raylib.sgh"
float m8;
  
#line 235 "src/../include/raylib.sgh"

#line 235 "src/../include/raylib.sgh"
float m12;
  
#line 236 "src/../include/raylib.sgh"

#line 236 "src/../include/raylib.sgh"
float m1;
  
#line 236 "src/../include/raylib.sgh"

#line 236 "src/../include/raylib.sgh"
float m5;
  
#line 236 "src/../include/raylib.sgh"

#line 236 "src/../include/raylib.sgh"
float m9;
  
#line 236 "src/../include/raylib.sgh"

#line 236 "src/../include/raylib.sgh"
float m13;
  
#line 237 "src/../include/raylib.sgh"

#line 237 "src/../include/raylib.sgh"
float m2;
  
#line 237 "src/../include/raylib.sgh"

#line 237 "src/../include/raylib.sgh"
float m6;
  
#line 237 "src/../include/raylib.sgh"

#line 237 "src/../include/raylib.sgh"
float m10;
  
#line 237 "src/../include/raylib.sgh"

#line 237 "src/../include/raylib.sgh"
float m14;
  
#line 238 "src/../include/raylib.sgh"

#line 238 "src/../include/raylib.sgh"
float m3;
  
#line 238 "src/../include/raylib.sgh"

#line 238 "src/../include/raylib.sgh"
float m7;
  
#line 238 "src/../include/raylib.sgh"

#line 238 "src/../include/raylib.sgh"
float m11;
  
#line 238 "src/../include/raylib.sgh"

#line 238 "src/../include/raylib.sgh"
float m15;
} Matrix;

#line 247 "src/../include/raylib.sgh"
typedef 
#line 242 "src/../include/raylib.sgh"
struct Color
{
  
#line 243 "src/../include/raylib.sgh"

#line 243 "src/../include/raylib.sgh"
unsigned char r;
  
#line 244 "src/../include/raylib.sgh"

#line 244 "src/../include/raylib.sgh"
unsigned char g;
  
#line 245 "src/../include/raylib.sgh"

#line 245 "src/../include/raylib.sgh"
unsigned char b;
  
#line 246 "src/../include/raylib.sgh"

#line 246 "src/../include/raylib.sgh"
unsigned char a;
} Color;

#line 255 "src/../include/raylib.sgh"
typedef 
#line 250 "src/../include/raylib.sgh"
struct Rectangle
{
  
#line 251 "src/../include/raylib.sgh"

#line 251 "src/../include/raylib.sgh"
float x;
  
#line 252 "src/../include/raylib.sgh"

#line 252 "src/../include/raylib.sgh"
float y;
  
#line 253 "src/../include/raylib.sgh"

#line 253 "src/../include/raylib.sgh"
float width;
  
#line 254 "src/../include/raylib.sgh"

#line 254 "src/../include/raylib.sgh"
float height;
} Rectangle;

#line 264 "src/../include/raylib.sgh"
typedef 
#line 258 "src/../include/raylib.sgh"
struct Image
{
  
#line 259 "src/../include/raylib.sgh"

#line 259 "src/../include/raylib.sgh"
void *data;
  
#line 260 "src/../include/raylib.sgh"

#line 260 "src/../include/raylib.sgh"
int width;
  
#line 261 "src/../include/raylib.sgh"

#line 261 "src/../include/raylib.sgh"
int height;
  
#line 262 "src/../include/raylib.sgh"

#line 262 "src/../include/raylib.sgh"
int mipmaps;
  
#line 263 "src/../include/raylib.sgh"

#line 263 "src/../include/raylib.sgh"
int format;
} Image;

#line 273 "src/../include/raylib.sgh"
typedef 
#line 267 "src/../include/raylib.sgh"
struct Texture
{
  
#line 268 "src/../include/raylib.sgh"

#line 268 "src/../include/raylib.sgh"
unsigned int id;
  
#line 269 "src/../include/raylib.sgh"

#line 269 "src/../include/raylib.sgh"
int width;
  
#line 270 "src/../include/raylib.sgh"

#line 270 "src/../include/raylib.sgh"
int height;
  
#line 271 "src/../include/raylib.sgh"

#line 271 "src/../include/raylib.sgh"
int mipmaps;
  
#line 272 "src/../include/raylib.sgh"

#line 272 "src/../include/raylib.sgh"
int format;
} Texture;

#line 276 "src/../include/raylib.sgh"
typedef 
#line 276 "src/../include/raylib.sgh"
Texture Texture2D;

#line 279 "src/../include/raylib.sgh"
typedef 
#line 279 "src/../include/raylib.sgh"
Texture TextureCubemap;

#line 286 "src/../include/raylib.sgh"
typedef 
#line 282 "src/../include/raylib.sgh"
struct RenderTexture
{
  
#line 283 "src/../include/raylib.sgh"

#line 283 "src/../include/raylib.sgh"
unsigned int id;
  
#line 284 "src/../include/raylib.sgh"

#line 284 "src/../include/raylib.sgh"
Texture texture;
  
#line 285 "src/../include/raylib.sgh"

#line 285 "src/../include/raylib.sgh"
Texture depth;
} RenderTexture;

#line 289 "src/../include/raylib.sgh"
typedef 
#line 289 "src/../include/raylib.sgh"
RenderTexture RenderTexture2D;

#line 299 "src/../include/raylib.sgh"
typedef 
#line 292 "src/../include/raylib.sgh"
struct NPatchInfo
{
  
#line 293 "src/../include/raylib.sgh"

#line 293 "src/../include/raylib.sgh"
Rectangle source;
  
#line 294 "src/../include/raylib.sgh"

#line 294 "src/../include/raylib.sgh"
int left;
  
#line 295 "src/../include/raylib.sgh"

#line 295 "src/../include/raylib.sgh"
int top;
  
#line 296 "src/../include/raylib.sgh"

#line 296 "src/../include/raylib.sgh"
int right;
  
#line 297 "src/../include/raylib.sgh"

#line 297 "src/../include/raylib.sgh"
int bottom;
  
#line 298 "src/../include/raylib.sgh"

#line 298 "src/../include/raylib.sgh"
int layout;
} NPatchInfo;

#line 308 "src/../include/raylib.sgh"
typedef 
#line 302 "src/../include/raylib.sgh"
struct GlyphInfo
{
  
#line 303 "src/../include/raylib.sgh"

#line 303 "src/../include/raylib.sgh"
int value;
  
#line 304 "src/../include/raylib.sgh"

#line 304 "src/../include/raylib.sgh"
int offsetX;
  
#line 305 "src/../include/raylib.sgh"

#line 305 "src/../include/raylib.sgh"
int offsetY;
  
#line 306 "src/../include/raylib.sgh"

#line 306 "src/../include/raylib.sgh"
int advanceX;
  
#line 307 "src/../include/raylib.sgh"

#line 307 "src/../include/raylib.sgh"
Image image;
} GlyphInfo;

#line 318 "src/../include/raylib.sgh"
typedef 
#line 311 "src/../include/raylib.sgh"
struct Font
{
  
#line 312 "src/../include/raylib.sgh"

#line 312 "src/../include/raylib.sgh"
int baseSize;
  
#line 313 "src/../include/raylib.sgh"

#line 313 "src/../include/raylib.sgh"
int glyphCount;
  
#line 314 "src/../include/raylib.sgh"

#line 314 "src/../include/raylib.sgh"
int glyphPadding;
  
#line 315 "src/../include/raylib.sgh"

#line 315 "src/../include/raylib.sgh"
Texture2D texture;
  
#line 316 "src/../include/raylib.sgh"

#line 316 "src/../include/raylib.sgh"
Rectangle *recs;
  
#line 317 "src/../include/raylib.sgh"

#line 317 "src/../include/raylib.sgh"
GlyphInfo *glyphs;
} Font;

#line 327 "src/../include/raylib.sgh"
typedef 
#line 321 "src/../include/raylib.sgh"
struct Camera3D
{
  
#line 322 "src/../include/raylib.sgh"

#line 322 "src/../include/raylib.sgh"
Vector3 position;
  
#line 323 "src/../include/raylib.sgh"

#line 323 "src/../include/raylib.sgh"
Vector3 target;
  
#line 324 "src/../include/raylib.sgh"

#line 324 "src/../include/raylib.sgh"
Vector3 up;
  
#line 325 "src/../include/raylib.sgh"

#line 325 "src/../include/raylib.sgh"
float fovy;
  
#line 326 "src/../include/raylib.sgh"

#line 326 "src/../include/raylib.sgh"
int projection;
} Camera3D;

#line 329 "src/../include/raylib.sgh"
typedef 
#line 329 "src/../include/raylib.sgh"
Camera3D Camera;

#line 337 "src/../include/raylib.sgh"
typedef 
#line 332 "src/../include/raylib.sgh"
struct Camera2D
{
  
#line 333 "src/../include/raylib.sgh"

#line 333 "src/../include/raylib.sgh"
Vector2 offset;
  
#line 334 "src/../include/raylib.sgh"

#line 334 "src/../include/raylib.sgh"
Vector2 target;
  
#line 335 "src/../include/raylib.sgh"

#line 335 "src/../include/raylib.sgh"
float rotation;
  
#line 336 "src/../include/raylib.sgh"

#line 336 "src/../include/raylib.sgh"
float zoom;
} Camera2D;

#line 362 "src/../include/raylib.sgh"
typedef 
#line 340 "src/../include/raylib.sgh"
struct Mesh
{
  
#line 341 "src/../include/raylib.sgh"

#line 341 "src/../include/raylib.sgh"
int vertexCount;
  
#line 342 "src/../include/raylib.sgh"

#line 342 "src/../include/raylib.sgh"
int triangleCount;
  
#line 345 "src/../include/raylib.sgh"

#line 345 "src/../include/raylib.sgh"
float *vertices;
  
#line 346 "src/../include/raylib.sgh"

#line 346 "src/../include/raylib.sgh"
float *texcoords;
  
#line 347 "src/../include/raylib.sgh"

#line 347 "src/../include/raylib.sgh"
float *texcoords2;
  
#line 348 "src/../include/raylib.sgh"

#line 348 "src/../include/raylib.sgh"
float *normals;
  
#line 349 "src/../include/raylib.sgh"

#line 349 "src/../include/raylib.sgh"
float *tangents;
  
#line 350 "src/../include/raylib.sgh"

#line 350 "src/../include/raylib.sgh"
unsigned char *colors;
  
#line 351 "src/../include/raylib.sgh"

#line 351 "src/../include/raylib.sgh"
unsigned short *indices;
  
#line 354 "src/../include/raylib.sgh"

#line 354 "src/../include/raylib.sgh"
float *animVertices;
  
#line 355 "src/../include/raylib.sgh"

#line 355 "src/../include/raylib.sgh"
float *animNormals;
  
#line 356 "src/../include/raylib.sgh"

#line 356 "src/../include/raylib.sgh"
unsigned char *boneIds;
  
#line 357 "src/../include/raylib.sgh"

#line 357 "src/../include/raylib.sgh"
float *boneWeights;
  
#line 360 "src/../include/raylib.sgh"

#line 360 "src/../include/raylib.sgh"
unsigned int vaoId;
  
#line 361 "src/../include/raylib.sgh"

#line 361 "src/../include/raylib.sgh"
unsigned int *vboId;
} Mesh;

#line 368 "src/../include/raylib.sgh"
typedef 
#line 365 "src/../include/raylib.sgh"
struct Shader
{
  
#line 366 "src/../include/raylib.sgh"

#line 366 "src/../include/raylib.sgh"
unsigned int id;
  
#line 367 "src/../include/raylib.sgh"

#line 367 "src/../include/raylib.sgh"
int *locs;
} Shader;

#line 375 "src/../include/raylib.sgh"
typedef 
#line 371 "src/../include/raylib.sgh"
struct MaterialMap
{
  
#line 372 "src/../include/raylib.sgh"

#line 372 "src/../include/raylib.sgh"
Texture2D texture;
  
#line 373 "src/../include/raylib.sgh"

#line 373 "src/../include/raylib.sgh"
Color color;
  
#line 374 "src/../include/raylib.sgh"

#line 374 "src/../include/raylib.sgh"
float value;
} MaterialMap;

#line 382 "src/../include/raylib.sgh"
typedef 
#line 378 "src/../include/raylib.sgh"
struct Material
{
  
#line 379 "src/../include/raylib.sgh"

#line 379 "src/../include/raylib.sgh"
Shader shader;
  
#line 380 "src/../include/raylib.sgh"

#line 380 "src/../include/raylib.sgh"
MaterialMap *maps;
  
#line 381 "src/../include/raylib.sgh"

#line 381 "src/../include/raylib.sgh"
float params[
#line 381 "src/../include/raylib.sgh"
4];
} Material;

#line 389 "src/../include/raylib.sgh"
typedef 
#line 385 "src/../include/raylib.sgh"
struct Transform
{
  
#line 386 "src/../include/raylib.sgh"

#line 386 "src/../include/raylib.sgh"
Vector3 translation;
  
#line 387 "src/../include/raylib.sgh"

#line 387 "src/../include/raylib.sgh"
Quaternion rotation;
  
#line 388 "src/../include/raylib.sgh"

#line 388 "src/../include/raylib.sgh"
Vector3 scale;
} Transform;

#line 395 "src/../include/raylib.sgh"
typedef 
#line 392 "src/../include/raylib.sgh"
struct BoneInfo
{
  
#line 393 "src/../include/raylib.sgh"

#line 393 "src/../include/raylib.sgh"
char name[
#line 393 "src/../include/raylib.sgh"
32];
  
#line 394 "src/../include/raylib.sgh"

#line 394 "src/../include/raylib.sgh"
int parent;
} BoneInfo;

#line 411 "src/../include/raylib.sgh"
typedef 
#line 398 "src/../include/raylib.sgh"
struct Model
{
  
#line 399 "src/../include/raylib.sgh"

#line 399 "src/../include/raylib.sgh"
Matrix transform;
  
#line 401 "src/../include/raylib.sgh"

#line 401 "src/../include/raylib.sgh"
int meshCount;
  
#line 402 "src/../include/raylib.sgh"

#line 402 "src/../include/raylib.sgh"
int materialCount;
  
#line 403 "src/../include/raylib.sgh"

#line 403 "src/../include/raylib.sgh"
Mesh *meshes;
  
#line 404 "src/../include/raylib.sgh"

#line 404 "src/../include/raylib.sgh"
Material *materials;
  
#line 405 "src/../include/raylib.sgh"

#line 405 "src/../include/raylib.sgh"
int *meshMaterial;
  
#line 408 "src/../include/raylib.sgh"

#line 408 "src/../include/raylib.sgh"
int boneCount;
  
#line 409 "src/../include/raylib.sgh"

#line 409 "src/../include/raylib.sgh"
BoneInfo *bones;
  
#line 410 "src/../include/raylib.sgh"

#line 410 "src/../include/raylib.sgh"
Transform *bindPose;
} Model;

#line 420 "src/../include/raylib.sgh"
typedef 
#line 414 "src/../include/raylib.sgh"
struct ModelAnimation
{
  
#line 415 "src/../include/raylib.sgh"

#line 415 "src/../include/raylib.sgh"
int boneCount;
  
#line 416 "src/../include/raylib.sgh"

#line 416 "src/../include/raylib.sgh"
int frameCount;
  
#line 417 "src/../include/raylib.sgh"

#line 417 "src/../include/raylib.sgh"
BoneInfo *bones;
  
#line 418 "src/../include/raylib.sgh"

#line 418 "src/../include/raylib.sgh"
Transform **framePoses;
  
#line 419 "src/../include/raylib.sgh"

#line 419 "src/../include/raylib.sgh"
char name[
#line 419 "src/../include/raylib.sgh"
32];
} ModelAnimation;

#line 426 "src/../include/raylib.sgh"
typedef 
#line 423 "src/../include/raylib.sgh"
struct Ray
{
  
#line 424 "src/../include/raylib.sgh"

#line 424 "src/../include/raylib.sgh"
Vector3 position;
  
#line 425 "src/../include/raylib.sgh"

#line 425 "src/../include/raylib.sgh"
Vector3 direction;
} Ray;

#line 434 "src/../include/raylib.sgh"
typedef 
#line 429 "src/../include/raylib.sgh"
struct RayCollision
{
  
#line 430 "src/../include/raylib.sgh"

#line 430 "src/../include/raylib.sgh"
_Bool hit;
  
#line 431 "src/../include/raylib.sgh"

#line 431 "src/../include/raylib.sgh"
float distance;
  
#line 432 "src/../include/raylib.sgh"

#line 432 "src/../include/raylib.sgh"
Vector3 point;
  
#line 433 "src/../include/raylib.sgh"

#line 433 "src/../include/raylib.sgh"
Vector3 normal;
} RayCollision;

#line 440 "src/../include/raylib.sgh"
typedef 
#line 437 "src/../include/raylib.sgh"
struct BoundingBox
{
  
#line 438 "src/../include/raylib.sgh"

#line 438 "src/../include/raylib.sgh"
Vector3 min;
  
#line 439 "src/../include/raylib.sgh"

#line 439 "src/../include/raylib.sgh"
Vector3 max;
} BoundingBox;

#line 449 "src/../include/raylib.sgh"
typedef 
#line 443 "src/../include/raylib.sgh"
struct Wave
{
  
#line 444 "src/../include/raylib.sgh"

#line 444 "src/../include/raylib.sgh"
unsigned int frameCount;
  
#line 445 "src/../include/raylib.sgh"

#line 445 "src/../include/raylib.sgh"
unsigned int sampleRate;
  
#line 446 "src/../include/raylib.sgh"

#line 446 "src/../include/raylib.sgh"
unsigned int sampleSize;
  
#line 447 "src/../include/raylib.sgh"

#line 447 "src/../include/raylib.sgh"
unsigned int channels;
  
#line 448 "src/../include/raylib.sgh"

#line 448 "src/../include/raylib.sgh"
void *data;
} Wave;

#line 453 "src/../include/raylib.sgh"
typedef 
#line 453 "src/../include/raylib.sgh"
struct rAudioBuffer rAudioBuffer;

#line 454 "src/../include/raylib.sgh"
typedef 
#line 454 "src/../include/raylib.sgh"
struct rAudioProcessor rAudioProcessor;

#line 464 "src/../include/raylib.sgh"
typedef 
#line 457 "src/../include/raylib.sgh"
struct AudioStream
{
  
#line 458 "src/../include/raylib.sgh"

#line 458 "src/../include/raylib.sgh"
rAudioBuffer *buffer;
  
#line 459 "src/../include/raylib.sgh"

#line 459 "src/../include/raylib.sgh"
rAudioProcessor *processor;
  
#line 461 "src/../include/raylib.sgh"

#line 461 "src/../include/raylib.sgh"
unsigned int sampleRate;
  
#line 462 "src/../include/raylib.sgh"

#line 462 "src/../include/raylib.sgh"
unsigned int sampleSize;
  
#line 463 "src/../include/raylib.sgh"

#line 463 "src/../include/raylib.sgh"
unsigned int channels;
} AudioStream;

#line 470 "src/../include/raylib.sgh"
typedef 
#line 467 "src/../include/raylib.sgh"
struct Sound
{
  
#line 468 "src/../include/raylib.sgh"

#line 468 "src/../include/raylib.sgh"
AudioStream stream;
  
#line 469 "src/../include/raylib.sgh"

#line 469 "src/../include/raylib.sgh"
unsigned int frameCount;
} Sound;

#line 480 "src/../include/raylib.sgh"
typedef 
#line 473 "src/../include/raylib.sgh"
struct Music
{
  
#line 474 "src/../include/raylib.sgh"

#line 474 "src/../include/raylib.sgh"
AudioStream stream;
  
#line 475 "src/../include/raylib.sgh"

#line 475 "src/../include/raylib.sgh"
unsigned int frameCount;
  
#line 476 "src/../include/raylib.sgh"

#line 476 "src/../include/raylib.sgh"
_Bool looping;
  
#line 478 "src/../include/raylib.sgh"

#line 478 "src/../include/raylib.sgh"
int ctxType;
  
#line 479 "src/../include/raylib.sgh"

#line 479 "src/../include/raylib.sgh"
void *ctxData;
} Music;

#line 493 "src/../include/raylib.sgh"
typedef 
#line 483 "src/../include/raylib.sgh"
struct VrDeviceInfo
{
  
#line 484 "src/../include/raylib.sgh"

#line 484 "src/../include/raylib.sgh"
int hResolution;
  
#line 485 "src/../include/raylib.sgh"

#line 485 "src/../include/raylib.sgh"
int vResolution;
  
#line 486 "src/../include/raylib.sgh"

#line 486 "src/../include/raylib.sgh"
float hScreenSize;
  
#line 487 "src/../include/raylib.sgh"

#line 487 "src/../include/raylib.sgh"
float vScreenSize;
  
#line 488 "src/../include/raylib.sgh"

#line 488 "src/../include/raylib.sgh"
float eyeToScreenDistance;
  
#line 489 "src/../include/raylib.sgh"

#line 489 "src/../include/raylib.sgh"
float lensSeparationDistance;
  
#line 490 "src/../include/raylib.sgh"

#line 490 "src/../include/raylib.sgh"
float interpupillaryDistance;
  
#line 491 "src/../include/raylib.sgh"

#line 491 "src/../include/raylib.sgh"
float lensDistortionValues[
#line 491 "src/../include/raylib.sgh"
4];
  
#line 492 "src/../include/raylib.sgh"

#line 492 "src/../include/raylib.sgh"
float chromaAbCorrection[
#line 492 "src/../include/raylib.sgh"
4];
} VrDeviceInfo;

#line 505 "src/../include/raylib.sgh"
typedef 
#line 496 "src/../include/raylib.sgh"
struct VrStereoConfig
{
  
#line 497 "src/../include/raylib.sgh"

#line 497 "src/../include/raylib.sgh"
Matrix projection[
#line 497 "src/../include/raylib.sgh"
2];
  
#line 498 "src/../include/raylib.sgh"

#line 498 "src/../include/raylib.sgh"
Matrix viewOffset[
#line 498 "src/../include/raylib.sgh"
2];
  
#line 499 "src/../include/raylib.sgh"

#line 499 "src/../include/raylib.sgh"
float leftLensCenter[
#line 499 "src/../include/raylib.sgh"
2];
  
#line 500 "src/../include/raylib.sgh"

#line 500 "src/../include/raylib.sgh"
float rightLensCenter[
#line 500 "src/../include/raylib.sgh"
2];
  
#line 501 "src/../include/raylib.sgh"

#line 501 "src/../include/raylib.sgh"
float leftScreenCenter[
#line 501 "src/../include/raylib.sgh"
2];
  
#line 502 "src/../include/raylib.sgh"

#line 502 "src/../include/raylib.sgh"
float rightScreenCenter[
#line 502 "src/../include/raylib.sgh"
2];
  
#line 503 "src/../include/raylib.sgh"

#line 503 "src/../include/raylib.sgh"
float scale[
#line 503 "src/../include/raylib.sgh"
2];
  
#line 504 "src/../include/raylib.sgh"

#line 504 "src/../include/raylib.sgh"
float scaleIn[
#line 504 "src/../include/raylib.sgh"
2];
} VrStereoConfig;

#line 512 "src/../include/raylib.sgh"
typedef 
#line 508 "src/../include/raylib.sgh"
struct FilePathList
{
  
#line 509 "src/../include/raylib.sgh"

#line 509 "src/../include/raylib.sgh"
unsigned int capacity;
  
#line 510 "src/../include/raylib.sgh"

#line 510 "src/../include/raylib.sgh"
unsigned int count;
  
#line 511 "src/../include/raylib.sgh"

#line 511 "src/../include/raylib.sgh"
char **paths;
} FilePathList;

#line 519 "src/../include/raylib.sgh"
typedef 
#line 515 "src/../include/raylib.sgh"
struct AutomationEvent
{
  
#line 516 "src/../include/raylib.sgh"

#line 516 "src/../include/raylib.sgh"
unsigned int frame;
  
#line 517 "src/../include/raylib.sgh"

#line 517 "src/../include/raylib.sgh"
unsigned int type;
  
#line 518 "src/../include/raylib.sgh"

#line 518 "src/../include/raylib.sgh"
int params[
#line 518 "src/../include/raylib.sgh"
4];
} AutomationEvent;

#line 526 "src/../include/raylib.sgh"
typedef 
#line 522 "src/../include/raylib.sgh"
struct AutomationEventList
{
  
#line 523 "src/../include/raylib.sgh"

#line 523 "src/../include/raylib.sgh"
unsigned int capacity;
  
#line 524 "src/../include/raylib.sgh"

#line 524 "src/../include/raylib.sgh"
unsigned int count;
  
#line 525 "src/../include/raylib.sgh"

#line 525 "src/../include/raylib.sgh"
AutomationEvent *events;
} AutomationEventList;

#line 551 "src/../include/raylib.sgh"
typedef 
#line 534 "src/../include/raylib.sgh"
enum 
{

#line 535 "src/../include/raylib.sgh"
  FLAG_VSYNC_HINT = 
#line 535 "src/../include/raylib.sgh"
0x00000040,

#line 536 "src/../include/raylib.sgh"
  FLAG_FULLSCREEN_MODE = 
#line 536 "src/../include/raylib.sgh"
0x00000002,

#line 537 "src/../include/raylib.sgh"
  FLAG_WINDOW_RESIZABLE = 
#line 537 "src/../include/raylib.sgh"
0x00000004,

#line 538 "src/../include/raylib.sgh"
  FLAG_WINDOW_UNDECORATED = 
#line 538 "src/../include/raylib.sgh"
0x00000008,

#line 539 "src/../include/raylib.sgh"
  FLAG_WINDOW_HIDDEN = 
#line 539 "src/../include/raylib.sgh"
0x00000080,

#line 540 "src/../include/raylib.sgh"
  FLAG_WINDOW_MINIMIZED = 
#line 540 "src/../include/raylib.sgh"
0x00000200,

#line 541 "src/../include/raylib.sgh"
  FLAG_WINDOW_MAXIMIZED = 
#line 541 "src/../include/raylib.sgh"
0x00000400,

#line 542 "src/../include/raylib.sgh"
  FLAG_WINDOW_UNFOCUSED = 
#line 542 "src/../include/raylib.sgh"
0x00000800,

#line 543 "src/../include/raylib.sgh"
  FLAG_WINDOW_TOPMOST = 
#line 543 "src/../include/raylib.sgh"
0x00001000,

#line 544 "src/../include/raylib.sgh"
  FLAG_WINDOW_ALWAYS_RUN = 
#line 544 "src/../include/raylib.sgh"
0x00000100,

#line 545 "src/../include/raylib.sgh"
  FLAG_WINDOW_TRANSPARENT = 
#line 545 "src/../include/raylib.sgh"
0x00000010,

#line 546 "src/../include/raylib.sgh"
  FLAG_WINDOW_HIGHDPI = 
#line 546 "src/../include/raylib.sgh"
0x00002000,

#line 547 "src/../include/raylib.sgh"
  FLAG_WINDOW_MOUSE_PASSTHROUGH = 
#line 547 "src/../include/raylib.sgh"
0x00004000,

#line 548 "src/../include/raylib.sgh"
  FLAG_BORDERLESS_WINDOWED_MODE = 
#line 548 "src/../include/raylib.sgh"
0x00008000,

#line 549 "src/../include/raylib.sgh"
  FLAG_MSAA_4X_HINT = 
#line 549 "src/../include/raylib.sgh"
0x00000020,

#line 550 "src/../include/raylib.sgh"
  FLAG_INTERLACED_HINT = 
#line 550 "src/../include/raylib.sgh"
0x00010000
} ConfigFlags;

#line 564 "src/../include/raylib.sgh"
typedef 
#line 555 "src/../include/raylib.sgh"
enum 
{

#line 556 "src/../include/raylib.sgh"
  LOG_ALL = 
#line 556 "src/../include/raylib.sgh"
0,

#line 557 "src/../include/raylib.sgh"
  LOG_TRACE,

#line 558 "src/../include/raylib.sgh"
  LOG_DEBUG,

#line 559 "src/../include/raylib.sgh"
  LOG_INFO,

#line 560 "src/../include/raylib.sgh"
  LOG_WARNING,

#line 561 "src/../include/raylib.sgh"
  LOG_ERROR,

#line 562 "src/../include/raylib.sgh"
  LOG_FATAL,

#line 563 "src/../include/raylib.sgh"
  LOG_NONE
} TraceLogLevel;

#line 684 "src/../include/raylib.sgh"
typedef 
#line 569 "src/../include/raylib.sgh"
enum 
{

#line 570 "src/../include/raylib.sgh"
  KEY_NULL = 
#line 570 "src/../include/raylib.sgh"
0,

#line 572 "src/../include/raylib.sgh"
  KEY_APOSTROPHE = 
#line 572 "src/../include/raylib.sgh"
39,

#line 573 "src/../include/raylib.sgh"
  KEY_COMMA = 
#line 573 "src/../include/raylib.sgh"
44,

#line 574 "src/../include/raylib.sgh"
  KEY_MINUS = 
#line 574 "src/../include/raylib.sgh"
45,

#line 575 "src/../include/raylib.sgh"
  KEY_PERIOD = 
#line 575 "src/../include/raylib.sgh"
46,

#line 576 "src/../include/raylib.sgh"
  KEY_SLASH = 
#line 576 "src/../include/raylib.sgh"
47,

#line 577 "src/../include/raylib.sgh"
  KEY_ZERO = 
#line 577 "src/../include/raylib.sgh"
48,

#line 578 "src/../include/raylib.sgh"
  KEY_ONE = 
#line 578 "src/../include/raylib.sgh"
49,

#line 579 "src/../include/raylib.sgh"
  KEY_TWO = 
#line 579 "src/../include/raylib.sgh"
50,

#line 580 "src/../include/raylib.sgh"
  KEY_THREE = 
#line 580 "src/../include/raylib.sgh"
51,

#line 581 "src/../include/raylib.sgh"
  KEY_FOUR = 
#line 581 "src/../include/raylib.sgh"
52,

#line 582 "src/../include/raylib.sgh"
  KEY_FIVE = 
#line 582 "src/../include/raylib.sgh"
53,

#line 583 "src/../include/raylib.sgh"
  KEY_SIX = 
#line 583 "src/../include/raylib.sgh"
54,

#line 584 "src/../include/raylib.sgh"
  KEY_SEVEN = 
#line 584 "src/../include/raylib.sgh"
55,

#line 585 "src/../include/raylib.sgh"
  KEY_EIGHT = 
#line 585 "src/../include/raylib.sgh"
56,

#line 586 "src/../include/raylib.sgh"
  KEY_NINE = 
#line 586 "src/../include/raylib.sgh"
57,

#line 587 "src/../include/raylib.sgh"
  KEY_SEMICOLON = 
#line 587 "src/../include/raylib.sgh"
59,

#line 588 "src/../include/raylib.sgh"
  KEY_EQUAL = 
#line 588 "src/../include/raylib.sgh"
61,

#line 589 "src/../include/raylib.sgh"
  KEY_A = 
#line 589 "src/../include/raylib.sgh"
65,

#line 590 "src/../include/raylib.sgh"
  KEY_B = 
#line 590 "src/../include/raylib.sgh"
66,

#line 591 "src/../include/raylib.sgh"
  KEY_C = 
#line 591 "src/../include/raylib.sgh"
67,

#line 592 "src/../include/raylib.sgh"
  KEY_D = 
#line 592 "src/../include/raylib.sgh"
68,

#line 593 "src/../include/raylib.sgh"
  KEY_E = 
#line 593 "src/../include/raylib.sgh"
69,

#line 594 "src/../include/raylib.sgh"
  KEY_F = 
#line 594 "src/../include/raylib.sgh"
70,

#line 595 "src/../include/raylib.sgh"
  KEY_G = 
#line 595 "src/../include/raylib.sgh"
71,

#line 596 "src/../include/raylib.sgh"
  KEY_H = 
#line 596 "src/../include/raylib.sgh"
72,

#line 597 "src/../include/raylib.sgh"
  KEY_I = 
#line 597 "src/../include/raylib.sgh"
73,

#line 598 "src/../include/raylib.sgh"
  KEY_J = 
#line 598 "src/../include/raylib.sgh"
74,

#line 599 "src/../include/raylib.sgh"
  KEY_K = 
#line 599 "src/../include/raylib.sgh"
75,

#line 600 "src/../include/raylib.sgh"
  KEY_L = 
#line 600 "src/../include/raylib.sgh"
76,

#line 601 "src/../include/raylib.sgh"
  KEY_M = 
#line 601 "src/../include/raylib.sgh"
77,

#line 602 "src/../include/raylib.sgh"
  KEY_N = 
#line 602 "src/../include/raylib.sgh"
78,

#line 603 "src/../include/raylib.sgh"
  KEY_O = 
#line 603 "src/../include/raylib.sgh"
79,

#line 604 "src/../include/raylib.sgh"
  KEY_P = 
#line 604 "src/../include/raylib.sgh"
80,

#line 605 "src/../include/raylib.sgh"
  KEY_Q = 
#line 605 "src/../include/raylib.sgh"
81,

#line 606 "src/../include/raylib.sgh"
  KEY_R = 
#line 606 "src/../include/raylib.sgh"
82,

#line 607 "src/../include/raylib.sgh"
  KEY_S = 
#line 607 "src/../include/raylib.sgh"
83,

#line 608 "src/../include/raylib.sgh"
  KEY_T = 
#line 608 "src/../include/raylib.sgh"
84,

#line 609 "src/../include/raylib.sgh"
  KEY_U = 
#line 609 "src/../include/raylib.sgh"
85,

#line 610 "src/../include/raylib.sgh"
  KEY_V = 
#line 610 "src/../include/raylib.sgh"
86,

#line 611 "src/../include/raylib.sgh"
  KEY_W = 
#line 611 "src/../include/raylib.sgh"
87,

#line 612 "src/../include/raylib.sgh"
  KEY_X = 
#line 612 "src/../include/raylib.sgh"
88,

#line 613 "src/../include/raylib.sgh"
  KEY_Y = 
#line 613 "src/../include/raylib.sgh"
89,

#line 614 "src/../include/raylib.sgh"
  KEY_Z = 
#line 614 "src/../include/raylib.sgh"
90,

#line 615 "src/../include/raylib.sgh"
  KEY_LEFT_BRACKET = 
#line 615 "src/../include/raylib.sgh"
91,

#line 616 "src/../include/raylib.sgh"
  KEY_BACKSLASH = 
#line 616 "src/../include/raylib.sgh"
92,

#line 617 "src/../include/raylib.sgh"
  KEY_RIGHT_BRACKET = 
#line 617 "src/../include/raylib.sgh"
93,

#line 618 "src/../include/raylib.sgh"
  KEY_GRAVE = 
#line 618 "src/../include/raylib.sgh"
96,

#line 620 "src/../include/raylib.sgh"
  KEY_SPACE = 
#line 620 "src/../include/raylib.sgh"
32,

#line 621 "src/../include/raylib.sgh"
  KEY_ESCAPE = 
#line 621 "src/../include/raylib.sgh"
256,

#line 622 "src/../include/raylib.sgh"
  KEY_ENTER = 
#line 622 "src/../include/raylib.sgh"
257,

#line 623 "src/../include/raylib.sgh"
  KEY_TAB = 
#line 623 "src/../include/raylib.sgh"
258,

#line 624 "src/../include/raylib.sgh"
  KEY_BACKSPACE = 
#line 624 "src/../include/raylib.sgh"
259,

#line 625 "src/../include/raylib.sgh"
  KEY_INSERT = 
#line 625 "src/../include/raylib.sgh"
260,

#line 626 "src/../include/raylib.sgh"
  KEY_DELETE = 
#line 626 "src/../include/raylib.sgh"
261,

#line 627 "src/../include/raylib.sgh"
  KEY_RIGHT = 
#line 627 "src/../include/raylib.sgh"
262,

#line 628 "src/../include/raylib.sgh"
  KEY_LEFT = 
#line 628 "src/../include/raylib.sgh"
263,

#line 629 "src/../include/raylib.sgh"
  KEY_DOWN = 
#line 629 "src/../include/raylib.sgh"
264,

#line 630 "src/../include/raylib.sgh"
  KEY_UP = 
#line 630 "src/../include/raylib.sgh"
265,

#line 631 "src/../include/raylib.sgh"
  KEY_PAGE_UP = 
#line 631 "src/../include/raylib.sgh"
266,

#line 632 "src/../include/raylib.sgh"
  KEY_PAGE_DOWN = 
#line 632 "src/../include/raylib.sgh"
267,

#line 633 "src/../include/raylib.sgh"
  KEY_HOME = 
#line 633 "src/../include/raylib.sgh"
268,

#line 634 "src/../include/raylib.sgh"
  KEY_END = 
#line 634 "src/../include/raylib.sgh"
269,

#line 635 "src/../include/raylib.sgh"
  KEY_CAPS_LOCK = 
#line 635 "src/../include/raylib.sgh"
280,

#line 636 "src/../include/raylib.sgh"
  KEY_SCROLL_LOCK = 
#line 636 "src/../include/raylib.sgh"
281,

#line 637 "src/../include/raylib.sgh"
  KEY_NUM_LOCK = 
#line 637 "src/../include/raylib.sgh"
282,

#line 638 "src/../include/raylib.sgh"
  KEY_PRINT_SCREEN = 
#line 638 "src/../include/raylib.sgh"
283,

#line 639 "src/../include/raylib.sgh"
  KEY_PAUSE = 
#line 639 "src/../include/raylib.sgh"
284,

#line 640 "src/../include/raylib.sgh"
  KEY_F1 = 
#line 640 "src/../include/raylib.sgh"
290,

#line 641 "src/../include/raylib.sgh"
  KEY_F2 = 
#line 641 "src/../include/raylib.sgh"
291,

#line 642 "src/../include/raylib.sgh"
  KEY_F3 = 
#line 642 "src/../include/raylib.sgh"
292,

#line 643 "src/../include/raylib.sgh"
  KEY_F4 = 
#line 643 "src/../include/raylib.sgh"
293,

#line 644 "src/../include/raylib.sgh"
  KEY_F5 = 
#line 644 "src/../include/raylib.sgh"
294,

#line 645 "src/../include/raylib.sgh"
  KEY_F6 = 
#line 645 "src/../include/raylib.sgh"
295,

#line 646 "src/../include/raylib.sgh"
  KEY_F7 = 
#line 646 "src/../include/raylib.sgh"
296,

#line 647 "src/../include/raylib.sgh"
  KEY_F8 = 
#line 647 "src/../include/raylib.sgh"
297,

#line 648 "src/../include/raylib.sgh"
  KEY_F9 = 
#line 648 "src/../include/raylib.sgh"
298,

#line 649 "src/../include/raylib.sgh"
  KEY_F10 = 
#line 649 "src/../include/raylib.sgh"
299,

#line 650 "src/../include/raylib.sgh"
  KEY_F11 = 
#line 650 "src/../include/raylib.sgh"
300,

#line 651 "src/../include/raylib.sgh"
  KEY_F12 = 
#line 651 "src/../include/raylib.sgh"
301,

#line 652 "src/../include/raylib.sgh"
  KEY_LEFT_SHIFT = 
#line 652 "src/../include/raylib.sgh"
340,

#line 653 "src/../include/raylib.sgh"
  KEY_LEFT_CONTROL = 
#line 653 "src/../include/raylib.sgh"
341,

#line 654 "src/../include/raylib.sgh"
  KEY_LEFT_ALT = 
#line 654 "src/../include/raylib.sgh"
342,

#line 655 "src/../include/raylib.sgh"
  KEY_LEFT_SUPER = 
#line 655 "src/../include/raylib.sgh"
343,

#line 656 "src/../include/raylib.sgh"
  KEY_RIGHT_SHIFT = 
#line 656 "src/../include/raylib.sgh"
344,

#line 657 "src/../include/raylib.sgh"
  KEY_RIGHT_CONTROL = 
#line 657 "src/../include/raylib.sgh"
345,

#line 658 "src/../include/raylib.sgh"
  KEY_RIGHT_ALT = 
#line 658 "src/../include/raylib.sgh"
346,

#line 659 "src/../include/raylib.sgh"
  KEY_RIGHT_SUPER = 
#line 659 "src/../include/raylib.sgh"
347,

#line 660 "src/../include/raylib.sgh"
  KEY_KB_MENU = 
#line 660 "src/../include/raylib.sgh"
348,

#line 662 "src/../include/raylib.sgh"
  KEY_KP_0 = 
#line 662 "src/../include/raylib.sgh"
320,

#line 663 "src/../include/raylib.sgh"
  KEY_KP_1 = 
#line 663 "src/../include/raylib.sgh"
321,

#line 664 "src/../include/raylib.sgh"
  KEY_KP_2 = 
#line 664 "src/../include/raylib.sgh"
322,

#line 665 "src/../include/raylib.sgh"
  KEY_KP_3 = 
#line 665 "src/../include/raylib.sgh"
323,

#line 666 "src/../include/raylib.sgh"
  KEY_KP_4 = 
#line 666 "src/../include/raylib.sgh"
324,

#line 667 "src/../include/raylib.sgh"
  KEY_KP_5 = 
#line 667 "src/../include/raylib.sgh"
325,

#line 668 "src/../include/raylib.sgh"
  KEY_KP_6 = 
#line 668 "src/../include/raylib.sgh"
326,

#line 669 "src/../include/raylib.sgh"
  KEY_KP_7 = 
#line 669 "src/../include/raylib.sgh"
327,

#line 670 "src/../include/raylib.sgh"
  KEY_KP_8 = 
#line 670 "src/../include/raylib.sgh"
328,

#line 671 "src/../include/raylib.sgh"
  KEY_KP_9 = 
#line 671 "src/../include/raylib.sgh"
329,

#line 672 "src/../include/raylib.sgh"
  KEY_KP_DECIMAL = 
#line 672 "src/../include/raylib.sgh"
330,

#line 673 "src/../include/raylib.sgh"
  KEY_KP_DIVIDE = 
#line 673 "src/../include/raylib.sgh"
331,

#line 674 "src/../include/raylib.sgh"
  KEY_KP_MULTIPLY = 
#line 674 "src/../include/raylib.sgh"
332,

#line 675 "src/../include/raylib.sgh"
  KEY_KP_SUBTRACT = 
#line 675 "src/../include/raylib.sgh"
333,

#line 676 "src/../include/raylib.sgh"
  KEY_KP_ADD = 
#line 676 "src/../include/raylib.sgh"
334,

#line 677 "src/../include/raylib.sgh"
  KEY_KP_ENTER = 
#line 677 "src/../include/raylib.sgh"
335,

#line 678 "src/../include/raylib.sgh"
  KEY_KP_EQUAL = 
#line 678 "src/../include/raylib.sgh"
336,

#line 680 "src/../include/raylib.sgh"
  KEY_BACK = 
#line 680 "src/../include/raylib.sgh"
4,

#line 681 "src/../include/raylib.sgh"
  KEY_MENU = 
#line 681 "src/../include/raylib.sgh"
5,

#line 682 "src/../include/raylib.sgh"
  KEY_VOLUME_UP = 
#line 682 "src/../include/raylib.sgh"
24,

#line 683 "src/../include/raylib.sgh"
  KEY_VOLUME_DOWN = 
#line 683 "src/../include/raylib.sgh"
25
} KeyboardKey;

#line 700 "src/../include/raylib.sgh"
typedef 
#line 692 "src/../include/raylib.sgh"
enum 
{

#line 693 "src/../include/raylib.sgh"
  MOUSE_BUTTON_LEFT = 
#line 693 "src/../include/raylib.sgh"
0,

#line 694 "src/../include/raylib.sgh"
  MOUSE_BUTTON_RIGHT = 
#line 694 "src/../include/raylib.sgh"
1,

#line 695 "src/../include/raylib.sgh"
  MOUSE_BUTTON_MIDDLE = 
#line 695 "src/../include/raylib.sgh"
2,

#line 696 "src/../include/raylib.sgh"
  MOUSE_BUTTON_SIDE = 
#line 696 "src/../include/raylib.sgh"
3,

#line 697 "src/../include/raylib.sgh"
  MOUSE_BUTTON_EXTRA = 
#line 697 "src/../include/raylib.sgh"
4,

#line 698 "src/../include/raylib.sgh"
  MOUSE_BUTTON_FORWARD = 
#line 698 "src/../include/raylib.sgh"
5,

#line 699 "src/../include/raylib.sgh"
  MOUSE_BUTTON_BACK = 
#line 699 "src/../include/raylib.sgh"
6
} MouseButton;

#line 715 "src/../include/raylib.sgh"
typedef 
#line 703 "src/../include/raylib.sgh"
enum 
{

#line 704 "src/../include/raylib.sgh"
  MOUSE_CURSOR_DEFAULT = 
#line 704 "src/../include/raylib.sgh"
0,

#line 705 "src/../include/raylib.sgh"
  MOUSE_CURSOR_ARROW = 
#line 705 "src/../include/raylib.sgh"
1,

#line 706 "src/../include/raylib.sgh"
  MOUSE_CURSOR_IBEAM = 
#line 706 "src/../include/raylib.sgh"
2,

#line 707 "src/../include/raylib.sgh"
  MOUSE_CURSOR_CROSSHAIR = 
#line 707 "src/../include/raylib.sgh"
3,

#line 708 "src/../include/raylib.sgh"
  MOUSE_CURSOR_POINTING_HAND = 
#line 708 "src/../include/raylib.sgh"
4,

#line 709 "src/../include/raylib.sgh"
  MOUSE_CURSOR_RESIZE_EW = 
#line 709 "src/../include/raylib.sgh"
5,

#line 710 "src/../include/raylib.sgh"
  MOUSE_CURSOR_RESIZE_NS = 
#line 710 "src/../include/raylib.sgh"
6,

#line 711 "src/../include/raylib.sgh"
  MOUSE_CURSOR_RESIZE_NWSE = 
#line 711 "src/../include/raylib.sgh"
7,

#line 712 "src/../include/raylib.sgh"
  MOUSE_CURSOR_RESIZE_NESW = 
#line 712 "src/../include/raylib.sgh"
8,

#line 713 "src/../include/raylib.sgh"
  MOUSE_CURSOR_RESIZE_ALL = 
#line 713 "src/../include/raylib.sgh"
9,

#line 714 "src/../include/raylib.sgh"
  MOUSE_CURSOR_NOT_ALLOWED = 
#line 714 "src/../include/raylib.sgh"
10
} MouseCursor;

#line 737 "src/../include/raylib.sgh"
typedef 
#line 718 "src/../include/raylib.sgh"
enum 
{

#line 719 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_UNKNOWN = 
#line 719 "src/../include/raylib.sgh"
0,

#line 720 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_FACE_UP,

#line 721 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_FACE_RIGHT,

#line 722 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_FACE_DOWN,

#line 723 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_FACE_LEFT,

#line 724 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_FACE_UP,

#line 725 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,

#line 726 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_FACE_DOWN,

#line 727 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_FACE_LEFT,

#line 728 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_TRIGGER_1,

#line 729 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_TRIGGER_2,

#line 730 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_TRIGGER_1,

#line 731 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_TRIGGER_2,

#line 732 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_MIDDLE_LEFT,

#line 733 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_MIDDLE,

#line 734 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_MIDDLE_RIGHT,

#line 735 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_LEFT_THUMB,

#line 736 "src/../include/raylib.sgh"
  GAMEPAD_BUTTON_RIGHT_THUMB
} GamepadButton;

#line 747 "src/../include/raylib.sgh"
typedef 
#line 740 "src/../include/raylib.sgh"
enum 
{

#line 741 "src/../include/raylib.sgh"
  GAMEPAD_AXIS_LEFT_X = 
#line 741 "src/../include/raylib.sgh"
0,

#line 742 "src/../include/raylib.sgh"
  GAMEPAD_AXIS_LEFT_Y = 
#line 742 "src/../include/raylib.sgh"
1,

#line 743 "src/../include/raylib.sgh"
  GAMEPAD_AXIS_RIGHT_X = 
#line 743 "src/../include/raylib.sgh"
2,

#line 744 "src/../include/raylib.sgh"
  GAMEPAD_AXIS_RIGHT_Y = 
#line 744 "src/../include/raylib.sgh"
3,

#line 745 "src/../include/raylib.sgh"
  GAMEPAD_AXIS_LEFT_TRIGGER = 
#line 745 "src/../include/raylib.sgh"
4,

#line 746 "src/../include/raylib.sgh"
  GAMEPAD_AXIS_RIGHT_TRIGGER = 
#line 746 "src/../include/raylib.sgh"
5
} GamepadAxis;

#line 762 "src/../include/raylib.sgh"
typedef 
#line 750 "src/../include/raylib.sgh"
enum 
{

#line 751 "src/../include/raylib.sgh"
  MATERIAL_MAP_ALBEDO = 
#line 751 "src/../include/raylib.sgh"
0,

#line 752 "src/../include/raylib.sgh"
  MATERIAL_MAP_METALNESS,

#line 753 "src/../include/raylib.sgh"
  MATERIAL_MAP_NORMAL,

#line 754 "src/../include/raylib.sgh"
  MATERIAL_MAP_ROUGHNESS,

#line 755 "src/../include/raylib.sgh"
  MATERIAL_MAP_OCCLUSION,

#line 756 "src/../include/raylib.sgh"
  MATERIAL_MAP_EMISSION,

#line 757 "src/../include/raylib.sgh"
  MATERIAL_MAP_HEIGHT,

#line 758 "src/../include/raylib.sgh"
  MATERIAL_MAP_CUBEMAP,

#line 759 "src/../include/raylib.sgh"
  MATERIAL_MAP_IRRADIANCE,

#line 760 "src/../include/raylib.sgh"
  MATERIAL_MAP_PREFILTER,

#line 761 "src/../include/raylib.sgh"
  MATERIAL_MAP_BRDF
} MaterialMapIndex;

#line 795 "src/../include/raylib.sgh"
typedef 
#line 768 "src/../include/raylib.sgh"
enum 
{

#line 769 "src/../include/raylib.sgh"
  SHADER_LOC_VERTEX_POSITION = 
#line 769 "src/../include/raylib.sgh"
0,

#line 770 "src/../include/raylib.sgh"
  SHADER_LOC_VERTEX_TEXCOORD01,

#line 771 "src/../include/raylib.sgh"
  SHADER_LOC_VERTEX_TEXCOORD02,

#line 772 "src/../include/raylib.sgh"
  SHADER_LOC_VERTEX_NORMAL,

#line 773 "src/../include/raylib.sgh"
  SHADER_LOC_VERTEX_TANGENT,

#line 774 "src/../include/raylib.sgh"
  SHADER_LOC_VERTEX_COLOR,

#line 775 "src/../include/raylib.sgh"
  SHADER_LOC_MATRIX_MVP,

#line 776 "src/../include/raylib.sgh"
  SHADER_LOC_MATRIX_VIEW,

#line 777 "src/../include/raylib.sgh"
  SHADER_LOC_MATRIX_PROJECTION,

#line 778 "src/../include/raylib.sgh"
  SHADER_LOC_MATRIX_MODEL,

#line 779 "src/../include/raylib.sgh"
  SHADER_LOC_MATRIX_NORMAL,

#line 780 "src/../include/raylib.sgh"
  SHADER_LOC_VECTOR_VIEW,

#line 781 "src/../include/raylib.sgh"
  SHADER_LOC_COLOR_DIFFUSE,

#line 782 "src/../include/raylib.sgh"
  SHADER_LOC_COLOR_SPECULAR,

#line 783 "src/../include/raylib.sgh"
  SHADER_LOC_COLOR_AMBIENT,

#line 784 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_ALBEDO,

#line 785 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_METALNESS,

#line 786 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_NORMAL,

#line 787 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_ROUGHNESS,

#line 788 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_OCCLUSION,

#line 789 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_EMISSION,

#line 790 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_HEIGHT,

#line 791 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_CUBEMAP,

#line 792 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_IRRADIANCE,

#line 793 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_PREFILTER,

#line 794 "src/../include/raylib.sgh"
  SHADER_LOC_MAP_BRDF
} ShaderLocationIndex;

#line 811 "src/../include/raylib.sgh"
typedef 
#line 801 "src/../include/raylib.sgh"
enum 
{

#line 802 "src/../include/raylib.sgh"
  SHADER_UNIFORM_FLOAT = 
#line 802 "src/../include/raylib.sgh"
0,

#line 803 "src/../include/raylib.sgh"
  SHADER_UNIFORM_VEC2,

#line 804 "src/../include/raylib.sgh"
  SHADER_UNIFORM_VEC3,

#line 805 "src/../include/raylib.sgh"
  SHADER_UNIFORM_VEC4,

#line 806 "src/../include/raylib.sgh"
  SHADER_UNIFORM_INT,

#line 807 "src/../include/raylib.sgh"
  SHADER_UNIFORM_IVEC2,

#line 808 "src/../include/raylib.sgh"
  SHADER_UNIFORM_IVEC3,

#line 809 "src/../include/raylib.sgh"
  SHADER_UNIFORM_IVEC4,

#line 810 "src/../include/raylib.sgh"
  SHADER_UNIFORM_SAMPLER2D
} ShaderUniformDataType;

#line 819 "src/../include/raylib.sgh"
typedef 
#line 814 "src/../include/raylib.sgh"
enum 
{

#line 815 "src/../include/raylib.sgh"
  SHADER_ATTRIB_FLOAT = 
#line 815 "src/../include/raylib.sgh"
0,

#line 816 "src/../include/raylib.sgh"
  SHADER_ATTRIB_VEC2,

#line 817 "src/../include/raylib.sgh"
  SHADER_ATTRIB_VEC3,

#line 818 "src/../include/raylib.sgh"
  SHADER_ATTRIB_VEC4
} ShaderAttributeDataType;

#line 848 "src/../include/raylib.sgh"
typedef 
#line 823 "src/../include/raylib.sgh"
enum 
{

#line 824 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 
#line 824 "src/../include/raylib.sgh"
1,

#line 825 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,

#line 826 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R5G6B5,

#line 827 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R8G8B8,

#line 828 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,

#line 829 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,

#line 830 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,

#line 831 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R32,

#line 832 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R32G32B32,

#line 833 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,

#line 834 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R16,

#line 835 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R16G16B16,

#line 836 "src/../include/raylib.sgh"
  PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,

#line 837 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_DXT1_RGB,

#line 838 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_DXT1_RGBA,

#line 839 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_DXT3_RGBA,

#line 840 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_DXT5_RGBA,

#line 841 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_ETC1_RGB,

#line 842 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_ETC2_RGB,

#line 843 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,

#line 844 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_PVRT_RGB,

#line 845 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_PVRT_RGBA,

#line 846 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,

#line 847 "src/../include/raylib.sgh"
  PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA
} PixelFormat;

#line 860 "src/../include/raylib.sgh"
typedef 
#line 853 "src/../include/raylib.sgh"
enum 
{

#line 854 "src/../include/raylib.sgh"
  TEXTURE_FILTER_POINT = 
#line 854 "src/../include/raylib.sgh"
0,

#line 855 "src/../include/raylib.sgh"
  TEXTURE_FILTER_BILINEAR,

#line 856 "src/../include/raylib.sgh"
  TEXTURE_FILTER_TRILINEAR,

#line 857 "src/../include/raylib.sgh"
  TEXTURE_FILTER_ANISOTROPIC_4X,

#line 858 "src/../include/raylib.sgh"
  TEXTURE_FILTER_ANISOTROPIC_8X,

#line 859 "src/../include/raylib.sgh"
  TEXTURE_FILTER_ANISOTROPIC_16X
} TextureFilter;

#line 868 "src/../include/raylib.sgh"
typedef 
#line 863 "src/../include/raylib.sgh"
enum 
{

#line 864 "src/../include/raylib.sgh"
  TEXTURE_WRAP_REPEAT = 
#line 864 "src/../include/raylib.sgh"
0,

#line 865 "src/../include/raylib.sgh"
  TEXTURE_WRAP_CLAMP,

#line 866 "src/../include/raylib.sgh"
  TEXTURE_WRAP_MIRROR_REPEAT,

#line 867 "src/../include/raylib.sgh"
  TEXTURE_WRAP_MIRROR_CLAMP
} TextureWrap;

#line 878 "src/../include/raylib.sgh"
typedef 
#line 871 "src/../include/raylib.sgh"
enum 
{

#line 872 "src/../include/raylib.sgh"
  CUBEMAP_LAYOUT_AUTO_DETECT = 
#line 872 "src/../include/raylib.sgh"
0,

#line 873 "src/../include/raylib.sgh"
  CUBEMAP_LAYOUT_LINE_VERTICAL,

#line 874 "src/../include/raylib.sgh"
  CUBEMAP_LAYOUT_LINE_HORIZONTAL,

#line 875 "src/../include/raylib.sgh"
  CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,

#line 876 "src/../include/raylib.sgh"
  CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,

#line 877 "src/../include/raylib.sgh"
  CUBEMAP_LAYOUT_PANORAMA
} CubemapLayout;

#line 885 "src/../include/raylib.sgh"
typedef 
#line 881 "src/../include/raylib.sgh"
enum 
{

#line 882 "src/../include/raylib.sgh"
  FONT_DEFAULT = 
#line 882 "src/../include/raylib.sgh"
0,

#line 883 "src/../include/raylib.sgh"
  FONT_BITMAP,

#line 884 "src/../include/raylib.sgh"
  FONT_SDF
} FontType;

#line 897 "src/../include/raylib.sgh"
typedef 
#line 888 "src/../include/raylib.sgh"
enum 
{

#line 889 "src/../include/raylib.sgh"
  BLEND_ALPHA = 
#line 889 "src/../include/raylib.sgh"
0,

#line 890 "src/../include/raylib.sgh"
  BLEND_ADDITIVE,

#line 891 "src/../include/raylib.sgh"
  BLEND_MULTIPLIED,

#line 892 "src/../include/raylib.sgh"
  BLEND_ADD_COLORS,

#line 893 "src/../include/raylib.sgh"
  BLEND_SUBTRACT_COLORS,

#line 894 "src/../include/raylib.sgh"
  BLEND_ALPHA_PREMULTIPLY,

#line 895 "src/../include/raylib.sgh"
  BLEND_CUSTOM,

#line 896 "src/../include/raylib.sgh"
  BLEND_CUSTOM_SEPARATE
} BlendMode;

#line 913 "src/../include/raylib.sgh"
typedef 
#line 901 "src/../include/raylib.sgh"
enum 
{

#line 902 "src/../include/raylib.sgh"
  GESTURE_NONE = 
#line 902 "src/../include/raylib.sgh"
0,

#line 903 "src/../include/raylib.sgh"
  GESTURE_TAP = 
#line 903 "src/../include/raylib.sgh"
1,

#line 904 "src/../include/raylib.sgh"
  GESTURE_DOUBLETAP = 
#line 904 "src/../include/raylib.sgh"
2,

#line 905 "src/../include/raylib.sgh"
  GESTURE_HOLD = 
#line 905 "src/../include/raylib.sgh"
4,

#line 906 "src/../include/raylib.sgh"
  GESTURE_DRAG = 
#line 906 "src/../include/raylib.sgh"
8,

#line 907 "src/../include/raylib.sgh"
  GESTURE_SWIPE_RIGHT = 
#line 907 "src/../include/raylib.sgh"
16,

#line 908 "src/../include/raylib.sgh"
  GESTURE_SWIPE_LEFT = 
#line 908 "src/../include/raylib.sgh"
32,

#line 909 "src/../include/raylib.sgh"
  GESTURE_SWIPE_UP = 
#line 909 "src/../include/raylib.sgh"
64,

#line 910 "src/../include/raylib.sgh"
  GESTURE_SWIPE_DOWN = 
#line 910 "src/../include/raylib.sgh"
128,

#line 911 "src/../include/raylib.sgh"
  GESTURE_PINCH_IN = 
#line 911 "src/../include/raylib.sgh"
256,

#line 912 "src/../include/raylib.sgh"
  GESTURE_PINCH_OUT = 
#line 912 "src/../include/raylib.sgh"
512
} Gesture;

#line 922 "src/../include/raylib.sgh"
typedef 
#line 916 "src/../include/raylib.sgh"
enum 
{

#line 917 "src/../include/raylib.sgh"
  CAMERA_CUSTOM = 
#line 917 "src/../include/raylib.sgh"
0,

#line 918 "src/../include/raylib.sgh"
  CAMERA_FREE,

#line 919 "src/../include/raylib.sgh"
  CAMERA_ORBITAL,

#line 920 "src/../include/raylib.sgh"
  CAMERA_FIRST_PERSON,

#line 921 "src/../include/raylib.sgh"
  CAMERA_THIRD_PERSON
} CameraMode;

#line 928 "src/../include/raylib.sgh"
typedef 
#line 925 "src/../include/raylib.sgh"
enum 
{

#line 926 "src/../include/raylib.sgh"
  CAMERA_PERSPECTIVE = 
#line 926 "src/../include/raylib.sgh"
0,

#line 927 "src/../include/raylib.sgh"
  CAMERA_ORTHOGRAPHIC
} CameraProjection;

#line 935 "src/../include/raylib.sgh"
typedef 
#line 931 "src/../include/raylib.sgh"
enum 
{

#line 932 "src/../include/raylib.sgh"
  NPATCH_NINE_PATCH = 
#line 932 "src/../include/raylib.sgh"
0,

#line 933 "src/../include/raylib.sgh"
  NPATCH_THREE_PATCH_VERTICAL,

#line 934 "src/../include/raylib.sgh"
  NPATCH_THREE_PATCH_HORIZONTAL
} NPatchLayout;

#line 940 "src/../include/raylib.sgh"
typedef 
#line 940 "src/../include/raylib.sgh"
unsigned char *(*LoadFileDataCallback)(
#line 940 "src/../include/raylib.sgh"

#line 940 "src/../include/raylib.sgh"
const 
#line 940 "src/../include/raylib.sgh"
char *fileName, 
#line 940 "src/../include/raylib.sgh"

#line 940 "src/../include/raylib.sgh"
int *dataSize);

#line 941 "src/../include/raylib.sgh"
typedef 
#line 941 "src/../include/raylib.sgh"
_Bool (*SaveFileDataCallback)(
#line 941 "src/../include/raylib.sgh"

#line 941 "src/../include/raylib.sgh"
const 
#line 941 "src/../include/raylib.sgh"
char *fileName, 
#line 941 "src/../include/raylib.sgh"

#line 941 "src/../include/raylib.sgh"
void *data, 
#line 941 "src/../include/raylib.sgh"

#line 941 "src/../include/raylib.sgh"
int dataSize);

#line 942 "src/../include/raylib.sgh"
typedef 
#line 942 "src/../include/raylib.sgh"
char *(*LoadFileTextCallback)(
#line 942 "src/../include/raylib.sgh"

#line 942 "src/../include/raylib.sgh"
const 
#line 942 "src/../include/raylib.sgh"
char *fileName);

#line 943 "src/../include/raylib.sgh"
typedef 
#line 943 "src/../include/raylib.sgh"
_Bool (*SaveFileTextCallback)(
#line 943 "src/../include/raylib.sgh"

#line 943 "src/../include/raylib.sgh"
const 
#line 943 "src/../include/raylib.sgh"
char *fileName, 
#line 943 "src/../include/raylib.sgh"

#line 943 "src/../include/raylib.sgh"
char *text);

#line 959 "src/../include/raylib.sgh"
extern "C" 
#line 959 "src/../include/raylib.sgh"
void InitWindow(
#line 959 "src/../include/raylib.sgh"

#line 959 "src/../include/raylib.sgh"

#line 959 "src/../include/raylib.sgh"
int width, 
#line 959 "src/../include/raylib.sgh"

#line 959 "src/../include/raylib.sgh"
int height, 
#line 959 "src/../include/raylib.sgh"
const 
#line 959 "src/../include/raylib.sgh"
char *title);

#line 960 "src/../include/raylib.sgh"
extern "C" 
#line 960 "src/../include/raylib.sgh"
void CloseWindow(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 960 "src/../include/raylib.sgh"
void);

#line 961 "src/../include/raylib.sgh"
extern "C" 
#line 961 "src/../include/raylib.sgh"
_Bool WindowShouldClose(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 961 "src/../include/raylib.sgh"
void);

#line 962 "src/../include/raylib.sgh"
extern "C" 
#line 962 "src/../include/raylib.sgh"
_Bool IsWindowReady(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 962 "src/../include/raylib.sgh"
void);

#line 963 "src/../include/raylib.sgh"
extern "C" 
#line 963 "src/../include/raylib.sgh"
_Bool IsWindowFullscreen(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 963 "src/../include/raylib.sgh"
void);

#line 964 "src/../include/raylib.sgh"
extern "C" 
#line 964 "src/../include/raylib.sgh"
_Bool IsWindowHidden(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 964 "src/../include/raylib.sgh"
void);

#line 965 "src/../include/raylib.sgh"
extern "C" 
#line 965 "src/../include/raylib.sgh"
_Bool IsWindowMinimized(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 965 "src/../include/raylib.sgh"
void);

#line 966 "src/../include/raylib.sgh"
extern "C" 
#line 966 "src/../include/raylib.sgh"
_Bool IsWindowMaximized(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 966 "src/../include/raylib.sgh"
void);

#line 967 "src/../include/raylib.sgh"
extern "C" 
#line 967 "src/../include/raylib.sgh"
_Bool IsWindowFocused(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 967 "src/../include/raylib.sgh"
void);

#line 968 "src/../include/raylib.sgh"
extern "C" 
#line 968 "src/../include/raylib.sgh"
_Bool IsWindowResized(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 968 "src/../include/raylib.sgh"
void);

#line 969 "src/../include/raylib.sgh"
extern "C" 
#line 969 "src/../include/raylib.sgh"
_Bool IsWindowState(
#line 969 "src/../include/raylib.sgh"

#line 969 "src/../include/raylib.sgh"

#line 969 "src/../include/raylib.sgh"
unsigned int flag);

#line 970 "src/../include/raylib.sgh"
extern "C" 
#line 970 "src/../include/raylib.sgh"
void SetWindowState(
#line 970 "src/../include/raylib.sgh"

#line 970 "src/../include/raylib.sgh"

#line 970 "src/../include/raylib.sgh"
unsigned int flags);

#line 971 "src/../include/raylib.sgh"
extern "C" 
#line 971 "src/../include/raylib.sgh"
void ClearWindowState(
#line 971 "src/../include/raylib.sgh"

#line 971 "src/../include/raylib.sgh"

#line 971 "src/../include/raylib.sgh"
unsigned int flags);

#line 972 "src/../include/raylib.sgh"
extern "C" 
#line 972 "src/../include/raylib.sgh"
void ToggleFullscreen(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 972 "src/../include/raylib.sgh"
void);

#line 973 "src/../include/raylib.sgh"
extern "C" 
#line 973 "src/../include/raylib.sgh"
void ToggleBorderlessWindowed(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 973 "src/../include/raylib.sgh"
void);

#line 974 "src/../include/raylib.sgh"
extern "C" 
#line 974 "src/../include/raylib.sgh"
void MaximizeWindow(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 974 "src/../include/raylib.sgh"
void);

#line 975 "src/../include/raylib.sgh"
extern "C" 
#line 975 "src/../include/raylib.sgh"
void MinimizeWindow(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 975 "src/../include/raylib.sgh"
void);

#line 976 "src/../include/raylib.sgh"
extern "C" 
#line 976 "src/../include/raylib.sgh"
void RestoreWindow(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 976 "src/../include/raylib.sgh"
void);

#line 977 "src/../include/raylib.sgh"
extern "C" 
#line 977 "src/../include/raylib.sgh"
void SetWindowIcon(
#line 977 "src/../include/raylib.sgh"

#line 977 "src/../include/raylib.sgh"

#line 977 "src/../include/raylib.sgh"
Image image);

#line 978 "src/../include/raylib.sgh"
extern "C" 
#line 978 "src/../include/raylib.sgh"
void SetWindowIcons(
#line 978 "src/../include/raylib.sgh"

#line 978 "src/../include/raylib.sgh"

#line 978 "src/../include/raylib.sgh"
Image *images, 
#line 978 "src/../include/raylib.sgh"

#line 978 "src/../include/raylib.sgh"
int count);

#line 979 "src/../include/raylib.sgh"
extern "C" 
#line 979 "src/../include/raylib.sgh"
void SetWindowTitle(
#line 979 "src/../include/raylib.sgh"

#line 979 "src/../include/raylib.sgh"
const 
#line 979 "src/../include/raylib.sgh"
char *title);

#line 980 "src/../include/raylib.sgh"
extern "C" 
#line 980 "src/../include/raylib.sgh"
void SetWindowPosition(
#line 980 "src/../include/raylib.sgh"

#line 980 "src/../include/raylib.sgh"

#line 980 "src/../include/raylib.sgh"
int x, 
#line 980 "src/../include/raylib.sgh"

#line 980 "src/../include/raylib.sgh"
int y);

#line 981 "src/../include/raylib.sgh"
extern "C" 
#line 981 "src/../include/raylib.sgh"
void SetWindowMonitor(
#line 981 "src/../include/raylib.sgh"

#line 981 "src/../include/raylib.sgh"

#line 981 "src/../include/raylib.sgh"
int monitor);

#line 982 "src/../include/raylib.sgh"
extern "C" 
#line 982 "src/../include/raylib.sgh"
void SetWindowMinSize(
#line 982 "src/../include/raylib.sgh"

#line 982 "src/../include/raylib.sgh"

#line 982 "src/../include/raylib.sgh"
int width, 
#line 982 "src/../include/raylib.sgh"

#line 982 "src/../include/raylib.sgh"
int height);

#line 983 "src/../include/raylib.sgh"
extern "C" 
#line 983 "src/../include/raylib.sgh"
void SetWindowMaxSize(
#line 983 "src/../include/raylib.sgh"

#line 983 "src/../include/raylib.sgh"

#line 983 "src/../include/raylib.sgh"
int width, 
#line 983 "src/../include/raylib.sgh"

#line 983 "src/../include/raylib.sgh"
int height);

#line 984 "src/../include/raylib.sgh"
extern "C" 
#line 984 "src/../include/raylib.sgh"
void SetWindowSize(
#line 984 "src/../include/raylib.sgh"

#line 984 "src/../include/raylib.sgh"

#line 984 "src/../include/raylib.sgh"
int width, 
#line 984 "src/../include/raylib.sgh"

#line 984 "src/../include/raylib.sgh"
int height);

#line 985 "src/../include/raylib.sgh"
extern "C" 
#line 985 "src/../include/raylib.sgh"
void SetWindowOpacity(
#line 985 "src/../include/raylib.sgh"

#line 985 "src/../include/raylib.sgh"

#line 985 "src/../include/raylib.sgh"
float opacity);

#line 986 "src/../include/raylib.sgh"
extern "C" 
#line 986 "src/../include/raylib.sgh"
void SetWindowFocused(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 986 "src/../include/raylib.sgh"
void);

#line 987 "src/../include/raylib.sgh"
extern "C" 
#line 987 "src/../include/raylib.sgh"
void *GetWindowHandle(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 987 "src/../include/raylib.sgh"
void);

#line 988 "src/../include/raylib.sgh"
extern "C" 
#line 988 "src/../include/raylib.sgh"
int GetScreenWidth(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 988 "src/../include/raylib.sgh"
void);

#line 989 "src/../include/raylib.sgh"
extern "C" 
#line 989 "src/../include/raylib.sgh"
int GetScreenHeight(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 989 "src/../include/raylib.sgh"
void);

#line 990 "src/../include/raylib.sgh"
extern "C" 
#line 990 "src/../include/raylib.sgh"
int GetRenderWidth(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 990 "src/../include/raylib.sgh"
void);

#line 991 "src/../include/raylib.sgh"
extern "C" 
#line 991 "src/../include/raylib.sgh"
int GetRenderHeight(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 991 "src/../include/raylib.sgh"
void);

#line 992 "src/../include/raylib.sgh"
extern "C" 
#line 992 "src/../include/raylib.sgh"
int GetMonitorCount(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 992 "src/../include/raylib.sgh"
void);

#line 993 "src/../include/raylib.sgh"
extern "C" 
#line 993 "src/../include/raylib.sgh"
int GetCurrentMonitor(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 993 "src/../include/raylib.sgh"
void);

#line 994 "src/../include/raylib.sgh"
extern "C" 
#line 994 "src/../include/raylib.sgh"
Vector2 GetMonitorPosition(
#line 994 "src/../include/raylib.sgh"

#line 994 "src/../include/raylib.sgh"

#line 994 "src/../include/raylib.sgh"
int monitor);

#line 995 "src/../include/raylib.sgh"
extern "C" 
#line 995 "src/../include/raylib.sgh"
int GetMonitorWidth(
#line 995 "src/../include/raylib.sgh"

#line 995 "src/../include/raylib.sgh"

#line 995 "src/../include/raylib.sgh"
int monitor);

#line 996 "src/../include/raylib.sgh"
extern "C" 
#line 996 "src/../include/raylib.sgh"
int GetMonitorHeight(
#line 996 "src/../include/raylib.sgh"

#line 996 "src/../include/raylib.sgh"

#line 996 "src/../include/raylib.sgh"
int monitor);

#line 997 "src/../include/raylib.sgh"
extern "C" 
#line 997 "src/../include/raylib.sgh"
int GetMonitorPhysicalWidth(
#line 997 "src/../include/raylib.sgh"

#line 997 "src/../include/raylib.sgh"

#line 997 "src/../include/raylib.sgh"
int monitor);

#line 998 "src/../include/raylib.sgh"
extern "C" 
#line 998 "src/../include/raylib.sgh"
int GetMonitorPhysicalHeight(
#line 998 "src/../include/raylib.sgh"

#line 998 "src/../include/raylib.sgh"

#line 998 "src/../include/raylib.sgh"
int monitor);

#line 999 "src/../include/raylib.sgh"
extern "C" 
#line 999 "src/../include/raylib.sgh"
int GetMonitorRefreshRate(
#line 999 "src/../include/raylib.sgh"

#line 999 "src/../include/raylib.sgh"

#line 999 "src/../include/raylib.sgh"
int monitor);

#line 1000 "src/../include/raylib.sgh"
extern "C" 
#line 1000 "src/../include/raylib.sgh"
Vector2 GetWindowPosition(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1000 "src/../include/raylib.sgh"
void);

#line 1001 "src/../include/raylib.sgh"
extern "C" 
#line 1001 "src/../include/raylib.sgh"
Vector2 GetWindowScaleDPI(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1001 "src/../include/raylib.sgh"
void);

#line 1002 "src/../include/raylib.sgh"
extern "C" const 
#line 1002 "src/../include/raylib.sgh"
char *GetMonitorName(
#line 1002 "src/../include/raylib.sgh"

#line 1002 "src/../include/raylib.sgh"

#line 1002 "src/../include/raylib.sgh"
int monitor);

#line 1003 "src/../include/raylib.sgh"
extern "C" 
#line 1003 "src/../include/raylib.sgh"
void SetClipboardText(
#line 1003 "src/../include/raylib.sgh"

#line 1003 "src/../include/raylib.sgh"
const 
#line 1003 "src/../include/raylib.sgh"
char *text);

#line 1004 "src/../include/raylib.sgh"
extern "C" const 
#line 1004 "src/../include/raylib.sgh"
char *GetClipboardText(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1004 "src/../include/raylib.sgh"
void);

#line 1005 "src/../include/raylib.sgh"
extern "C" 
#line 1005 "src/../include/raylib.sgh"
void EnableEventWaiting(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1005 "src/../include/raylib.sgh"
void);

#line 1006 "src/../include/raylib.sgh"
extern "C" 
#line 1006 "src/../include/raylib.sgh"
void DisableEventWaiting(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1006 "src/../include/raylib.sgh"
void);

#line 1009 "src/../include/raylib.sgh"
extern "C" 
#line 1009 "src/../include/raylib.sgh"
void ShowCursor(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1009 "src/../include/raylib.sgh"
void);

#line 1010 "src/../include/raylib.sgh"
extern "C" 
#line 1010 "src/../include/raylib.sgh"
void HideCursor(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1010 "src/../include/raylib.sgh"
void);

#line 1011 "src/../include/raylib.sgh"
extern "C" 
#line 1011 "src/../include/raylib.sgh"
_Bool IsCursorHidden(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1011 "src/../include/raylib.sgh"
void);

#line 1012 "src/../include/raylib.sgh"
extern "C" 
#line 1012 "src/../include/raylib.sgh"
void EnableCursor(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1012 "src/../include/raylib.sgh"
void);

#line 1013 "src/../include/raylib.sgh"
extern "C" 
#line 1013 "src/../include/raylib.sgh"
void DisableCursor(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1013 "src/../include/raylib.sgh"
void);

#line 1014 "src/../include/raylib.sgh"
extern "C" 
#line 1014 "src/../include/raylib.sgh"
_Bool IsCursorOnScreen(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1014 "src/../include/raylib.sgh"
void);

#line 1017 "src/../include/raylib.sgh"
extern "C" 
#line 1017 "src/../include/raylib.sgh"
void ClearBackground(
#line 1017 "src/../include/raylib.sgh"

#line 1017 "src/../include/raylib.sgh"

#line 1017 "src/../include/raylib.sgh"
Color color);

#line 1018 "src/../include/raylib.sgh"
extern "C" 
#line 1018 "src/../include/raylib.sgh"
void BeginDrawing(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1018 "src/../include/raylib.sgh"
void);

#line 1019 "src/../include/raylib.sgh"
extern "C" 
#line 1019 "src/../include/raylib.sgh"
void EndDrawing(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1019 "src/../include/raylib.sgh"
void);

#line 1020 "src/../include/raylib.sgh"
extern "C" 
#line 1020 "src/../include/raylib.sgh"
void BeginMode2D(
#line 1020 "src/../include/raylib.sgh"

#line 1020 "src/../include/raylib.sgh"

#line 1020 "src/../include/raylib.sgh"
Camera2D camera);

#line 1021 "src/../include/raylib.sgh"
extern "C" 
#line 1021 "src/../include/raylib.sgh"
void EndMode2D(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1021 "src/../include/raylib.sgh"
void);

#line 1022 "src/../include/raylib.sgh"
extern "C" 
#line 1022 "src/../include/raylib.sgh"
void BeginMode3D(
#line 1022 "src/../include/raylib.sgh"

#line 1022 "src/../include/raylib.sgh"

#line 1022 "src/../include/raylib.sgh"
Camera3D camera);

#line 1023 "src/../include/raylib.sgh"
extern "C" 
#line 1023 "src/../include/raylib.sgh"
void EndMode3D(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1023 "src/../include/raylib.sgh"
void);

#line 1024 "src/../include/raylib.sgh"
extern "C" 
#line 1024 "src/../include/raylib.sgh"
void BeginTextureMode(
#line 1024 "src/../include/raylib.sgh"

#line 1024 "src/../include/raylib.sgh"

#line 1024 "src/../include/raylib.sgh"
RenderTexture2D target);

#line 1025 "src/../include/raylib.sgh"
extern "C" 
#line 1025 "src/../include/raylib.sgh"
void EndTextureMode(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1025 "src/../include/raylib.sgh"
void);

#line 1026 "src/../include/raylib.sgh"
extern "C" 
#line 1026 "src/../include/raylib.sgh"
void BeginShaderMode(
#line 1026 "src/../include/raylib.sgh"

#line 1026 "src/../include/raylib.sgh"

#line 1026 "src/../include/raylib.sgh"
Shader shader);

#line 1027 "src/../include/raylib.sgh"
extern "C" 
#line 1027 "src/../include/raylib.sgh"
void EndShaderMode(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1027 "src/../include/raylib.sgh"
void);

#line 1028 "src/../include/raylib.sgh"
extern "C" 
#line 1028 "src/../include/raylib.sgh"
void BeginBlendMode(
#line 1028 "src/../include/raylib.sgh"

#line 1028 "src/../include/raylib.sgh"

#line 1028 "src/../include/raylib.sgh"
int mode);

#line 1029 "src/../include/raylib.sgh"
extern "C" 
#line 1029 "src/../include/raylib.sgh"
void EndBlendMode(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1029 "src/../include/raylib.sgh"
void);

#line 1030 "src/../include/raylib.sgh"
extern "C" 
#line 1030 "src/../include/raylib.sgh"
void BeginScissorMode(
#line 1030 "src/../include/raylib.sgh"

#line 1030 "src/../include/raylib.sgh"

#line 1030 "src/../include/raylib.sgh"
int x, 
#line 1030 "src/../include/raylib.sgh"

#line 1030 "src/../include/raylib.sgh"
int y, 
#line 1030 "src/../include/raylib.sgh"

#line 1030 "src/../include/raylib.sgh"
int width, 
#line 1030 "src/../include/raylib.sgh"

#line 1030 "src/../include/raylib.sgh"
int height);

#line 1031 "src/../include/raylib.sgh"
extern "C" 
#line 1031 "src/../include/raylib.sgh"
void EndScissorMode(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1031 "src/../include/raylib.sgh"
void);

#line 1032 "src/../include/raylib.sgh"
extern "C" 
#line 1032 "src/../include/raylib.sgh"
void BeginVrStereoMode(
#line 1032 "src/../include/raylib.sgh"

#line 1032 "src/../include/raylib.sgh"

#line 1032 "src/../include/raylib.sgh"
VrStereoConfig config);

#line 1033 "src/../include/raylib.sgh"
extern "C" 
#line 1033 "src/../include/raylib.sgh"
void EndVrStereoMode(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1033 "src/../include/raylib.sgh"
void);

#line 1036 "src/../include/raylib.sgh"
extern "C" 
#line 1036 "src/../include/raylib.sgh"
VrStereoConfig LoadVrStereoConfig(
#line 1036 "src/../include/raylib.sgh"

#line 1036 "src/../include/raylib.sgh"

#line 1036 "src/../include/raylib.sgh"
VrDeviceInfo device);

#line 1037 "src/../include/raylib.sgh"
extern "C" 
#line 1037 "src/../include/raylib.sgh"
void UnloadVrStereoConfig(
#line 1037 "src/../include/raylib.sgh"

#line 1037 "src/../include/raylib.sgh"

#line 1037 "src/../include/raylib.sgh"
VrStereoConfig config);

#line 1041 "src/../include/raylib.sgh"
extern "C" 
#line 1041 "src/../include/raylib.sgh"
Shader LoadShader(
#line 1041 "src/../include/raylib.sgh"

#line 1041 "src/../include/raylib.sgh"
const 
#line 1041 "src/../include/raylib.sgh"
char *vsFileName, 
#line 1041 "src/../include/raylib.sgh"
const 
#line 1041 "src/../include/raylib.sgh"
char *fsFileName);

#line 1042 "src/../include/raylib.sgh"
extern "C" 
#line 1042 "src/../include/raylib.sgh"
Shader LoadShaderFromMemory(
#line 1042 "src/../include/raylib.sgh"

#line 1042 "src/../include/raylib.sgh"
const 
#line 1042 "src/../include/raylib.sgh"
char *vsCode, 
#line 1042 "src/../include/raylib.sgh"
const 
#line 1042 "src/../include/raylib.sgh"
char *fsCode);

#line 1043 "src/../include/raylib.sgh"
extern "C" 
#line 1043 "src/../include/raylib.sgh"
_Bool IsShaderReady(
#line 1043 "src/../include/raylib.sgh"

#line 1043 "src/../include/raylib.sgh"

#line 1043 "src/../include/raylib.sgh"
Shader shader);

#line 1044 "src/../include/raylib.sgh"
extern "C" 
#line 1044 "src/../include/raylib.sgh"
int GetShaderLocation(
#line 1044 "src/../include/raylib.sgh"

#line 1044 "src/../include/raylib.sgh"

#line 1044 "src/../include/raylib.sgh"
Shader shader, 
#line 1044 "src/../include/raylib.sgh"
const 
#line 1044 "src/../include/raylib.sgh"
char *uniformName);

#line 1045 "src/../include/raylib.sgh"
extern "C" 
#line 1045 "src/../include/raylib.sgh"
int GetShaderLocationAttrib(
#line 1045 "src/../include/raylib.sgh"

#line 1045 "src/../include/raylib.sgh"

#line 1045 "src/../include/raylib.sgh"
Shader shader, 
#line 1045 "src/../include/raylib.sgh"
const 
#line 1045 "src/../include/raylib.sgh"
char *attribName);

#line 1046 "src/../include/raylib.sgh"
extern "C" 
#line 1046 "src/../include/raylib.sgh"
void SetShaderValue(
#line 1046 "src/../include/raylib.sgh"

#line 1046 "src/../include/raylib.sgh"

#line 1046 "src/../include/raylib.sgh"
Shader shader, 
#line 1046 "src/../include/raylib.sgh"

#line 1046 "src/../include/raylib.sgh"
int locIndex, 
#line 1046 "src/../include/raylib.sgh"
const 
#line 1046 "src/../include/raylib.sgh"
void *value, 
#line 1046 "src/../include/raylib.sgh"

#line 1046 "src/../include/raylib.sgh"
int uniformType);

#line 1047 "src/../include/raylib.sgh"
extern "C" 
#line 1047 "src/../include/raylib.sgh"
void SetShaderValueV(
#line 1047 "src/../include/raylib.sgh"

#line 1047 "src/../include/raylib.sgh"

#line 1047 "src/../include/raylib.sgh"
Shader shader, 
#line 1047 "src/../include/raylib.sgh"

#line 1047 "src/../include/raylib.sgh"
int locIndex, 
#line 1047 "src/../include/raylib.sgh"
const 
#line 1047 "src/../include/raylib.sgh"
void *value, 
#line 1047 "src/../include/raylib.sgh"

#line 1047 "src/../include/raylib.sgh"
int uniformType, 
#line 1047 "src/../include/raylib.sgh"

#line 1047 "src/../include/raylib.sgh"
int count);

#line 1048 "src/../include/raylib.sgh"
extern "C" 
#line 1048 "src/../include/raylib.sgh"
void SetShaderValueMatrix(
#line 1048 "src/../include/raylib.sgh"

#line 1048 "src/../include/raylib.sgh"

#line 1048 "src/../include/raylib.sgh"
Shader shader, 
#line 1048 "src/../include/raylib.sgh"

#line 1048 "src/../include/raylib.sgh"
int locIndex, 
#line 1048 "src/../include/raylib.sgh"

#line 1048 "src/../include/raylib.sgh"
Matrix mat);

#line 1049 "src/../include/raylib.sgh"
extern "C" 
#line 1049 "src/../include/raylib.sgh"
void SetShaderValueTexture(
#line 1049 "src/../include/raylib.sgh"

#line 1049 "src/../include/raylib.sgh"

#line 1049 "src/../include/raylib.sgh"
Shader shader, 
#line 1049 "src/../include/raylib.sgh"

#line 1049 "src/../include/raylib.sgh"
int locIndex, 
#line 1049 "src/../include/raylib.sgh"

#line 1049 "src/../include/raylib.sgh"
Texture2D texture);

#line 1050 "src/../include/raylib.sgh"
extern "C" 
#line 1050 "src/../include/raylib.sgh"
void UnloadShader(
#line 1050 "src/../include/raylib.sgh"

#line 1050 "src/../include/raylib.sgh"

#line 1050 "src/../include/raylib.sgh"
Shader shader);

#line 1053 "src/../include/raylib.sgh"
extern "C" 
#line 1053 "src/../include/raylib.sgh"
Ray GetMouseRay(
#line 1053 "src/../include/raylib.sgh"

#line 1053 "src/../include/raylib.sgh"

#line 1053 "src/../include/raylib.sgh"
Vector2 mousePosition, 
#line 1053 "src/../include/raylib.sgh"

#line 1053 "src/../include/raylib.sgh"
Camera camera);

#line 1054 "src/../include/raylib.sgh"
extern "C" 
#line 1054 "src/../include/raylib.sgh"
Matrix GetCameraMatrix(
#line 1054 "src/../include/raylib.sgh"

#line 1054 "src/../include/raylib.sgh"

#line 1054 "src/../include/raylib.sgh"
Camera camera);

#line 1055 "src/../include/raylib.sgh"
extern "C" 
#line 1055 "src/../include/raylib.sgh"
Matrix GetCameraMatrix2D(
#line 1055 "src/../include/raylib.sgh"

#line 1055 "src/../include/raylib.sgh"

#line 1055 "src/../include/raylib.sgh"
Camera2D camera);

#line 1056 "src/../include/raylib.sgh"
extern "C" 
#line 1056 "src/../include/raylib.sgh"
Vector2 GetWorldToScreen(
#line 1056 "src/../include/raylib.sgh"

#line 1056 "src/../include/raylib.sgh"

#line 1056 "src/../include/raylib.sgh"
Vector3 position, 
#line 1056 "src/../include/raylib.sgh"

#line 1056 "src/../include/raylib.sgh"
Camera camera);

#line 1057 "src/../include/raylib.sgh"
extern "C" 
#line 1057 "src/../include/raylib.sgh"
Vector2 GetScreenToWorld2D(
#line 1057 "src/../include/raylib.sgh"

#line 1057 "src/../include/raylib.sgh"

#line 1057 "src/../include/raylib.sgh"
Vector2 position, 
#line 1057 "src/../include/raylib.sgh"

#line 1057 "src/../include/raylib.sgh"
Camera2D camera);

#line 1058 "src/../include/raylib.sgh"
extern "C" 
#line 1058 "src/../include/raylib.sgh"
Vector2 GetWorldToScreenEx(
#line 1058 "src/../include/raylib.sgh"

#line 1058 "src/../include/raylib.sgh"

#line 1058 "src/../include/raylib.sgh"
Vector3 position, 
#line 1058 "src/../include/raylib.sgh"

#line 1058 "src/../include/raylib.sgh"
Camera camera, 
#line 1058 "src/../include/raylib.sgh"

#line 1058 "src/../include/raylib.sgh"
int width, 
#line 1058 "src/../include/raylib.sgh"

#line 1058 "src/../include/raylib.sgh"
int height);

#line 1059 "src/../include/raylib.sgh"
extern "C" 
#line 1059 "src/../include/raylib.sgh"
Vector2 GetWorldToScreen2D(
#line 1059 "src/../include/raylib.sgh"

#line 1059 "src/../include/raylib.sgh"

#line 1059 "src/../include/raylib.sgh"
Vector2 position, 
#line 1059 "src/../include/raylib.sgh"

#line 1059 "src/../include/raylib.sgh"
Camera2D camera);

#line 1062 "src/../include/raylib.sgh"
extern "C" 
#line 1062 "src/../include/raylib.sgh"
void SetTargetFPS(
#line 1062 "src/../include/raylib.sgh"

#line 1062 "src/../include/raylib.sgh"

#line 1062 "src/../include/raylib.sgh"
int fps);

#line 1063 "src/../include/raylib.sgh"
extern "C" 
#line 1063 "src/../include/raylib.sgh"
float GetFrameTime(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1063 "src/../include/raylib.sgh"
void);

#line 1064 "src/../include/raylib.sgh"
extern "C" 
#line 1064 "src/../include/raylib.sgh"
double GetTime(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1064 "src/../include/raylib.sgh"
void);

#line 1065 "src/../include/raylib.sgh"
extern "C" 
#line 1065 "src/../include/raylib.sgh"
int GetFPS(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1065 "src/../include/raylib.sgh"
void);

#line 1071 "src/../include/raylib.sgh"
extern "C" 
#line 1071 "src/../include/raylib.sgh"
void SwapScreenBuffer(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1071 "src/../include/raylib.sgh"
void);

#line 1072 "src/../include/raylib.sgh"
extern "C" 
#line 1072 "src/../include/raylib.sgh"
void PollInputEvents(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1072 "src/../include/raylib.sgh"
void);

#line 1073 "src/../include/raylib.sgh"
extern "C" 
#line 1073 "src/../include/raylib.sgh"
void WaitTime(
#line 1073 "src/../include/raylib.sgh"

#line 1073 "src/../include/raylib.sgh"

#line 1073 "src/../include/raylib.sgh"
double seconds);

#line 1076 "src/../include/raylib.sgh"
extern "C" 
#line 1076 "src/../include/raylib.sgh"
void SetRandomSeed(
#line 1076 "src/../include/raylib.sgh"

#line 1076 "src/../include/raylib.sgh"

#line 1076 "src/../include/raylib.sgh"
unsigned int seed);

#line 1077 "src/../include/raylib.sgh"
extern "C" 
#line 1077 "src/../include/raylib.sgh"
int GetRandomValue(
#line 1077 "src/../include/raylib.sgh"

#line 1077 "src/../include/raylib.sgh"

#line 1077 "src/../include/raylib.sgh"
int min, 
#line 1077 "src/../include/raylib.sgh"

#line 1077 "src/../include/raylib.sgh"
int max);

#line 1078 "src/../include/raylib.sgh"
extern "C" 
#line 1078 "src/../include/raylib.sgh"
int *LoadRandomSequence(
#line 1078 "src/../include/raylib.sgh"

#line 1078 "src/../include/raylib.sgh"

#line 1078 "src/../include/raylib.sgh"
unsigned int count, 
#line 1078 "src/../include/raylib.sgh"

#line 1078 "src/../include/raylib.sgh"
int min, 
#line 1078 "src/../include/raylib.sgh"

#line 1078 "src/../include/raylib.sgh"
int max);

#line 1079 "src/../include/raylib.sgh"
extern "C" 
#line 1079 "src/../include/raylib.sgh"
void UnloadRandomSequence(
#line 1079 "src/../include/raylib.sgh"

#line 1079 "src/../include/raylib.sgh"

#line 1079 "src/../include/raylib.sgh"
int *sequence);

#line 1082 "src/../include/raylib.sgh"
extern "C" 
#line 1082 "src/../include/raylib.sgh"
void TakeScreenshot(
#line 1082 "src/../include/raylib.sgh"

#line 1082 "src/../include/raylib.sgh"
const 
#line 1082 "src/../include/raylib.sgh"
char *fileName);

#line 1083 "src/../include/raylib.sgh"
extern "C" 
#line 1083 "src/../include/raylib.sgh"
void SetConfigFlags(
#line 1083 "src/../include/raylib.sgh"

#line 1083 "src/../include/raylib.sgh"

#line 1083 "src/../include/raylib.sgh"
unsigned int flags);

#line 1084 "src/../include/raylib.sgh"
extern "C" 
#line 1084 "src/../include/raylib.sgh"
void OpenURL(
#line 1084 "src/../include/raylib.sgh"

#line 1084 "src/../include/raylib.sgh"
const 
#line 1084 "src/../include/raylib.sgh"
char *url);

#line 1088 "src/../include/raylib.sgh"
extern "C" 
#line 1088 "src/../include/raylib.sgh"
void TraceLog(
#line 1088 "src/../include/raylib.sgh"

#line 1088 "src/../include/raylib.sgh"

#line 1088 "src/../include/raylib.sgh"
int logLevel, 
#line 1088 "src/../include/raylib.sgh"
const 
#line 1088 "src/../include/raylib.sgh"
char *text, 
#line 1088 "src/../include/raylib.sgh"
...);

#line 1089 "src/../include/raylib.sgh"
extern "C" 
#line 1089 "src/../include/raylib.sgh"
void SetTraceLogLevel(
#line 1089 "src/../include/raylib.sgh"

#line 1089 "src/../include/raylib.sgh"

#line 1089 "src/../include/raylib.sgh"
int logLevel);

#line 1090 "src/../include/raylib.sgh"
extern "C" 
#line 1090 "src/../include/raylib.sgh"
void *MemAlloc(
#line 1090 "src/../include/raylib.sgh"

#line 1090 "src/../include/raylib.sgh"

#line 1090 "src/../include/raylib.sgh"
unsigned int size);

#line 1091 "src/../include/raylib.sgh"
extern "C" 
#line 1091 "src/../include/raylib.sgh"
void *MemRealloc(
#line 1091 "src/../include/raylib.sgh"

#line 1091 "src/../include/raylib.sgh"

#line 1091 "src/../include/raylib.sgh"
void *ptr, 
#line 1091 "src/../include/raylib.sgh"

#line 1091 "src/../include/raylib.sgh"
unsigned int size);

#line 1092 "src/../include/raylib.sgh"
extern "C" 
#line 1092 "src/../include/raylib.sgh"
void MemFree(
#line 1092 "src/../include/raylib.sgh"

#line 1092 "src/../include/raylib.sgh"

#line 1092 "src/../include/raylib.sgh"
void *ptr);

#line 1103 "src/../include/raylib.sgh"
extern "C" 
#line 1103 "src/../include/raylib.sgh"
unsigned char *LoadFileData(
#line 1103 "src/../include/raylib.sgh"

#line 1103 "src/../include/raylib.sgh"
const 
#line 1103 "src/../include/raylib.sgh"
char *fileName, 
#line 1103 "src/../include/raylib.sgh"

#line 1103 "src/../include/raylib.sgh"
int *dataSize);

#line 1104 "src/../include/raylib.sgh"
extern "C" 
#line 1104 "src/../include/raylib.sgh"
void UnloadFileData(
#line 1104 "src/../include/raylib.sgh"

#line 1104 "src/../include/raylib.sgh"

#line 1104 "src/../include/raylib.sgh"
unsigned char *data);

#line 1105 "src/../include/raylib.sgh"
extern "C" 
#line 1105 "src/../include/raylib.sgh"
_Bool SaveFileData(
#line 1105 "src/../include/raylib.sgh"

#line 1105 "src/../include/raylib.sgh"
const 
#line 1105 "src/../include/raylib.sgh"
char *fileName, 
#line 1105 "src/../include/raylib.sgh"

#line 1105 "src/../include/raylib.sgh"
void *data, 
#line 1105 "src/../include/raylib.sgh"

#line 1105 "src/../include/raylib.sgh"
int dataSize);

#line 1106 "src/../include/raylib.sgh"
extern "C" 
#line 1106 "src/../include/raylib.sgh"
_Bool ExportDataAsCode(
#line 1106 "src/../include/raylib.sgh"

#line 1106 "src/../include/raylib.sgh"
const 
#line 1106 "src/../include/raylib.sgh"
unsigned char *data, 
#line 1106 "src/../include/raylib.sgh"

#line 1106 "src/../include/raylib.sgh"
int dataSize, 
#line 1106 "src/../include/raylib.sgh"
const 
#line 1106 "src/../include/raylib.sgh"
char *fileName);

#line 1107 "src/../include/raylib.sgh"
extern "C" 
#line 1107 "src/../include/raylib.sgh"
char *LoadFileText(
#line 1107 "src/../include/raylib.sgh"

#line 1107 "src/../include/raylib.sgh"
const 
#line 1107 "src/../include/raylib.sgh"
char *fileName);

#line 1108 "src/../include/raylib.sgh"
extern "C" 
#line 1108 "src/../include/raylib.sgh"
void UnloadFileText(
#line 1108 "src/../include/raylib.sgh"

#line 1108 "src/../include/raylib.sgh"

#line 1108 "src/../include/raylib.sgh"
char *text);

#line 1109 "src/../include/raylib.sgh"
extern "C" 
#line 1109 "src/../include/raylib.sgh"
_Bool SaveFileText(
#line 1109 "src/../include/raylib.sgh"

#line 1109 "src/../include/raylib.sgh"
const 
#line 1109 "src/../include/raylib.sgh"
char *fileName, 
#line 1109 "src/../include/raylib.sgh"

#line 1109 "src/../include/raylib.sgh"
char *text);

#line 1113 "src/../include/raylib.sgh"
extern "C" 
#line 1113 "src/../include/raylib.sgh"
_Bool FileExists(
#line 1113 "src/../include/raylib.sgh"

#line 1113 "src/../include/raylib.sgh"
const 
#line 1113 "src/../include/raylib.sgh"
char *fileName);

#line 1114 "src/../include/raylib.sgh"
extern "C" 
#line 1114 "src/../include/raylib.sgh"
_Bool DirectoryExists(
#line 1114 "src/../include/raylib.sgh"

#line 1114 "src/../include/raylib.sgh"
const 
#line 1114 "src/../include/raylib.sgh"
char *dirPath);

#line 1115 "src/../include/raylib.sgh"
extern "C" 
#line 1115 "src/../include/raylib.sgh"
_Bool IsFileExtension(
#line 1115 "src/../include/raylib.sgh"

#line 1115 "src/../include/raylib.sgh"
const 
#line 1115 "src/../include/raylib.sgh"
char *fileName, 
#line 1115 "src/../include/raylib.sgh"
const 
#line 1115 "src/../include/raylib.sgh"
char *ext);

#line 1116 "src/../include/raylib.sgh"
extern "C" 
#line 1116 "src/../include/raylib.sgh"
int GetFileLength(
#line 1116 "src/../include/raylib.sgh"

#line 1116 "src/../include/raylib.sgh"
const 
#line 1116 "src/../include/raylib.sgh"
char *fileName);

#line 1117 "src/../include/raylib.sgh"
extern "C" const 
#line 1117 "src/../include/raylib.sgh"
char *GetFileExtension(
#line 1117 "src/../include/raylib.sgh"

#line 1117 "src/../include/raylib.sgh"
const 
#line 1117 "src/../include/raylib.sgh"
char *fileName);

#line 1118 "src/../include/raylib.sgh"
extern "C" const 
#line 1118 "src/../include/raylib.sgh"
char *GetFileName(
#line 1118 "src/../include/raylib.sgh"

#line 1118 "src/../include/raylib.sgh"
const 
#line 1118 "src/../include/raylib.sgh"
char *filePath);

#line 1119 "src/../include/raylib.sgh"
extern "C" const 
#line 1119 "src/../include/raylib.sgh"
char *GetFileNameWithoutExt(
#line 1119 "src/../include/raylib.sgh"

#line 1119 "src/../include/raylib.sgh"
const 
#line 1119 "src/../include/raylib.sgh"
char *filePath);

#line 1120 "src/../include/raylib.sgh"
extern "C" const 
#line 1120 "src/../include/raylib.sgh"
char *GetDirectoryPath(
#line 1120 "src/../include/raylib.sgh"

#line 1120 "src/../include/raylib.sgh"
const 
#line 1120 "src/../include/raylib.sgh"
char *filePath);

#line 1121 "src/../include/raylib.sgh"
extern "C" const 
#line 1121 "src/../include/raylib.sgh"
char *GetPrevDirectoryPath(
#line 1121 "src/../include/raylib.sgh"

#line 1121 "src/../include/raylib.sgh"
const 
#line 1121 "src/../include/raylib.sgh"
char *dirPath);

#line 1122 "src/../include/raylib.sgh"
extern "C" const 
#line 1122 "src/../include/raylib.sgh"
char *GetWorkingDirectory(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1122 "src/../include/raylib.sgh"
void);

#line 1123 "src/../include/raylib.sgh"
extern "C" const 
#line 1123 "src/../include/raylib.sgh"
char *GetApplicationDirectory(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1123 "src/../include/raylib.sgh"
void);

#line 1124 "src/../include/raylib.sgh"
extern "C" 
#line 1124 "src/../include/raylib.sgh"
_Bool ChangeDirectory(
#line 1124 "src/../include/raylib.sgh"

#line 1124 "src/../include/raylib.sgh"
const 
#line 1124 "src/../include/raylib.sgh"
char *dir);

#line 1125 "src/../include/raylib.sgh"
extern "C" 
#line 1125 "src/../include/raylib.sgh"
_Bool IsPathFile(
#line 1125 "src/../include/raylib.sgh"

#line 1125 "src/../include/raylib.sgh"
const 
#line 1125 "src/../include/raylib.sgh"
char *path);

#line 1126 "src/../include/raylib.sgh"
extern "C" 
#line 1126 "src/../include/raylib.sgh"
FilePathList LoadDirectoryFiles(
#line 1126 "src/../include/raylib.sgh"

#line 1126 "src/../include/raylib.sgh"
const 
#line 1126 "src/../include/raylib.sgh"
char *dirPath);

#line 1127 "src/../include/raylib.sgh"
extern "C" 
#line 1127 "src/../include/raylib.sgh"
FilePathList LoadDirectoryFilesEx(
#line 1127 "src/../include/raylib.sgh"

#line 1127 "src/../include/raylib.sgh"
const 
#line 1127 "src/../include/raylib.sgh"
char *basePath, 
#line 1127 "src/../include/raylib.sgh"
const 
#line 1127 "src/../include/raylib.sgh"
char *filter, 
#line 1127 "src/../include/raylib.sgh"

#line 1127 "src/../include/raylib.sgh"
_Bool scanSubdirs);

#line 1128 "src/../include/raylib.sgh"
extern "C" 
#line 1128 "src/../include/raylib.sgh"
void UnloadDirectoryFiles(
#line 1128 "src/../include/raylib.sgh"

#line 1128 "src/../include/raylib.sgh"

#line 1128 "src/../include/raylib.sgh"
FilePathList files);

#line 1129 "src/../include/raylib.sgh"
extern "C" 
#line 1129 "src/../include/raylib.sgh"
_Bool IsFileDropped(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1129 "src/../include/raylib.sgh"
void);

#line 1130 "src/../include/raylib.sgh"
extern "C" 
#line 1130 "src/../include/raylib.sgh"
FilePathList LoadDroppedFiles(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1130 "src/../include/raylib.sgh"
void);

#line 1131 "src/../include/raylib.sgh"
extern "C" 
#line 1131 "src/../include/raylib.sgh"
void UnloadDroppedFiles(
#line 1131 "src/../include/raylib.sgh"

#line 1131 "src/../include/raylib.sgh"

#line 1131 "src/../include/raylib.sgh"
FilePathList files);

#line 1132 "src/../include/raylib.sgh"
extern "C" 
#line 1132 "src/../include/raylib.sgh"
long GetFileModTime(
#line 1132 "src/../include/raylib.sgh"

#line 1132 "src/../include/raylib.sgh"
const 
#line 1132 "src/../include/raylib.sgh"
char *fileName);

#line 1135 "src/../include/raylib.sgh"
extern "C" 
#line 1135 "src/../include/raylib.sgh"
unsigned char *CompressData(
#line 1135 "src/../include/raylib.sgh"

#line 1135 "src/../include/raylib.sgh"
const 
#line 1135 "src/../include/raylib.sgh"
unsigned char *data, 
#line 1135 "src/../include/raylib.sgh"

#line 1135 "src/../include/raylib.sgh"
int dataSize, 
#line 1135 "src/../include/raylib.sgh"

#line 1135 "src/../include/raylib.sgh"
int *compDataSize);

#line 1136 "src/../include/raylib.sgh"
extern "C" 
#line 1136 "src/../include/raylib.sgh"
unsigned char *DecompressData(
#line 1136 "src/../include/raylib.sgh"

#line 1136 "src/../include/raylib.sgh"
const 
#line 1136 "src/../include/raylib.sgh"
unsigned char *compData, 
#line 1136 "src/../include/raylib.sgh"

#line 1136 "src/../include/raylib.sgh"
int compDataSize, 
#line 1136 "src/../include/raylib.sgh"

#line 1136 "src/../include/raylib.sgh"
int *dataSize);

#line 1137 "src/../include/raylib.sgh"
extern "C" 
#line 1137 "src/../include/raylib.sgh"
char *EncodeDataBase64(
#line 1137 "src/../include/raylib.sgh"

#line 1137 "src/../include/raylib.sgh"
const 
#line 1137 "src/../include/raylib.sgh"
unsigned char *data, 
#line 1137 "src/../include/raylib.sgh"

#line 1137 "src/../include/raylib.sgh"
int dataSize, 
#line 1137 "src/../include/raylib.sgh"

#line 1137 "src/../include/raylib.sgh"
int *outputSize);

#line 1138 "src/../include/raylib.sgh"
extern "C" 
#line 1138 "src/../include/raylib.sgh"
unsigned char *DecodeDataBase64(
#line 1138 "src/../include/raylib.sgh"

#line 1138 "src/../include/raylib.sgh"
const 
#line 1138 "src/../include/raylib.sgh"
unsigned char *data, 
#line 1138 "src/../include/raylib.sgh"

#line 1138 "src/../include/raylib.sgh"
int *outputSize);

#line 1141 "src/../include/raylib.sgh"
extern "C" 
#line 1141 "src/../include/raylib.sgh"
AutomationEventList LoadAutomationEventList(
#line 1141 "src/../include/raylib.sgh"

#line 1141 "src/../include/raylib.sgh"
const 
#line 1141 "src/../include/raylib.sgh"
char *fileName);

#line 1142 "src/../include/raylib.sgh"
extern "C" 
#line 1142 "src/../include/raylib.sgh"
void UnloadAutomationEventList(
#line 1142 "src/../include/raylib.sgh"

#line 1142 "src/../include/raylib.sgh"

#line 1142 "src/../include/raylib.sgh"
AutomationEventList list);

#line 1143 "src/../include/raylib.sgh"
extern "C" 
#line 1143 "src/../include/raylib.sgh"
_Bool ExportAutomationEventList(
#line 1143 "src/../include/raylib.sgh"

#line 1143 "src/../include/raylib.sgh"

#line 1143 "src/../include/raylib.sgh"
AutomationEventList list, 
#line 1143 "src/../include/raylib.sgh"
const 
#line 1143 "src/../include/raylib.sgh"
char *fileName);

#line 1144 "src/../include/raylib.sgh"
extern "C" 
#line 1144 "src/../include/raylib.sgh"
void SetAutomationEventList(
#line 1144 "src/../include/raylib.sgh"

#line 1144 "src/../include/raylib.sgh"

#line 1144 "src/../include/raylib.sgh"
AutomationEventList *list);

#line 1145 "src/../include/raylib.sgh"
extern "C" 
#line 1145 "src/../include/raylib.sgh"
void SetAutomationEventBaseFrame(
#line 1145 "src/../include/raylib.sgh"

#line 1145 "src/../include/raylib.sgh"

#line 1145 "src/../include/raylib.sgh"
int frame);

#line 1146 "src/../include/raylib.sgh"
extern "C" 
#line 1146 "src/../include/raylib.sgh"
void StartAutomationEventRecording(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1146 "src/../include/raylib.sgh"
void);

#line 1147 "src/../include/raylib.sgh"
extern "C" 
#line 1147 "src/../include/raylib.sgh"
void StopAutomationEventRecording(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1147 "src/../include/raylib.sgh"
void);

#line 1148 "src/../include/raylib.sgh"
extern "C" 
#line 1148 "src/../include/raylib.sgh"
void PlayAutomationEvent(
#line 1148 "src/../include/raylib.sgh"

#line 1148 "src/../include/raylib.sgh"

#line 1148 "src/../include/raylib.sgh"
AutomationEvent event);

#line 1155 "src/../include/raylib.sgh"
extern "C" 
#line 1155 "src/../include/raylib.sgh"
_Bool IsKeyPressed(
#line 1155 "src/../include/raylib.sgh"

#line 1155 "src/../include/raylib.sgh"

#line 1155 "src/../include/raylib.sgh"
int key);

#line 1156 "src/../include/raylib.sgh"
extern "C" 
#line 1156 "src/../include/raylib.sgh"
_Bool IsKeyPressedRepeat(
#line 1156 "src/../include/raylib.sgh"

#line 1156 "src/../include/raylib.sgh"

#line 1156 "src/../include/raylib.sgh"
int key);

#line 1157 "src/../include/raylib.sgh"
extern "C" 
#line 1157 "src/../include/raylib.sgh"
_Bool IsKeyDown(
#line 1157 "src/../include/raylib.sgh"

#line 1157 "src/../include/raylib.sgh"

#line 1157 "src/../include/raylib.sgh"
int key);

#line 1158 "src/../include/raylib.sgh"
extern "C" 
#line 1158 "src/../include/raylib.sgh"
_Bool IsKeyReleased(
#line 1158 "src/../include/raylib.sgh"

#line 1158 "src/../include/raylib.sgh"

#line 1158 "src/../include/raylib.sgh"
int key);

#line 1159 "src/../include/raylib.sgh"
extern "C" 
#line 1159 "src/../include/raylib.sgh"
_Bool IsKeyUp(
#line 1159 "src/../include/raylib.sgh"

#line 1159 "src/../include/raylib.sgh"

#line 1159 "src/../include/raylib.sgh"
int key);

#line 1160 "src/../include/raylib.sgh"
extern "C" 
#line 1160 "src/../include/raylib.sgh"
int GetKeyPressed(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1160 "src/../include/raylib.sgh"
void);

#line 1161 "src/../include/raylib.sgh"
extern "C" 
#line 1161 "src/../include/raylib.sgh"
int GetCharPressed(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1161 "src/../include/raylib.sgh"
void);

#line 1162 "src/../include/raylib.sgh"
extern "C" 
#line 1162 "src/../include/raylib.sgh"
void SetExitKey(
#line 1162 "src/../include/raylib.sgh"

#line 1162 "src/../include/raylib.sgh"

#line 1162 "src/../include/raylib.sgh"
int key);

#line 1165 "src/../include/raylib.sgh"
extern "C" 
#line 1165 "src/../include/raylib.sgh"
_Bool IsGamepadAvailable(
#line 1165 "src/../include/raylib.sgh"

#line 1165 "src/../include/raylib.sgh"

#line 1165 "src/../include/raylib.sgh"
int gamepad);

#line 1166 "src/../include/raylib.sgh"
extern "C" const 
#line 1166 "src/../include/raylib.sgh"
char *GetGamepadName(
#line 1166 "src/../include/raylib.sgh"

#line 1166 "src/../include/raylib.sgh"

#line 1166 "src/../include/raylib.sgh"
int gamepad);

#line 1167 "src/../include/raylib.sgh"
extern "C" 
#line 1167 "src/../include/raylib.sgh"
_Bool IsGamepadButtonPressed(
#line 1167 "src/../include/raylib.sgh"

#line 1167 "src/../include/raylib.sgh"

#line 1167 "src/../include/raylib.sgh"
int gamepad, 
#line 1167 "src/../include/raylib.sgh"

#line 1167 "src/../include/raylib.sgh"
int button);

#line 1168 "src/../include/raylib.sgh"
extern "C" 
#line 1168 "src/../include/raylib.sgh"
_Bool IsGamepadButtonDown(
#line 1168 "src/../include/raylib.sgh"

#line 1168 "src/../include/raylib.sgh"

#line 1168 "src/../include/raylib.sgh"
int gamepad, 
#line 1168 "src/../include/raylib.sgh"

#line 1168 "src/../include/raylib.sgh"
int button);

#line 1169 "src/../include/raylib.sgh"
extern "C" 
#line 1169 "src/../include/raylib.sgh"
_Bool IsGamepadButtonReleased(
#line 1169 "src/../include/raylib.sgh"

#line 1169 "src/../include/raylib.sgh"

#line 1169 "src/../include/raylib.sgh"
int gamepad, 
#line 1169 "src/../include/raylib.sgh"

#line 1169 "src/../include/raylib.sgh"
int button);

#line 1170 "src/../include/raylib.sgh"
extern "C" 
#line 1170 "src/../include/raylib.sgh"
_Bool IsGamepadButtonUp(
#line 1170 "src/../include/raylib.sgh"

#line 1170 "src/../include/raylib.sgh"

#line 1170 "src/../include/raylib.sgh"
int gamepad, 
#line 1170 "src/../include/raylib.sgh"

#line 1170 "src/../include/raylib.sgh"
int button);

#line 1171 "src/../include/raylib.sgh"
extern "C" 
#line 1171 "src/../include/raylib.sgh"
int GetGamepadButtonPressed(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1171 "src/../include/raylib.sgh"
void);

#line 1172 "src/../include/raylib.sgh"
extern "C" 
#line 1172 "src/../include/raylib.sgh"
int GetGamepadAxisCount(
#line 1172 "src/../include/raylib.sgh"

#line 1172 "src/../include/raylib.sgh"

#line 1172 "src/../include/raylib.sgh"
int gamepad);

#line 1173 "src/../include/raylib.sgh"
extern "C" 
#line 1173 "src/../include/raylib.sgh"
float GetGamepadAxisMovement(
#line 1173 "src/../include/raylib.sgh"

#line 1173 "src/../include/raylib.sgh"

#line 1173 "src/../include/raylib.sgh"
int gamepad, 
#line 1173 "src/../include/raylib.sgh"

#line 1173 "src/../include/raylib.sgh"
int axis);

#line 1174 "src/../include/raylib.sgh"
extern "C" 
#line 1174 "src/../include/raylib.sgh"
int SetGamepadMappings(
#line 1174 "src/../include/raylib.sgh"

#line 1174 "src/../include/raylib.sgh"
const 
#line 1174 "src/../include/raylib.sgh"
char *mappings);

#line 1177 "src/../include/raylib.sgh"
extern "C" 
#line 1177 "src/../include/raylib.sgh"
_Bool IsMouseButtonPressed(
#line 1177 "src/../include/raylib.sgh"

#line 1177 "src/../include/raylib.sgh"

#line 1177 "src/../include/raylib.sgh"
int button);

#line 1178 "src/../include/raylib.sgh"
extern "C" 
#line 1178 "src/../include/raylib.sgh"
_Bool IsMouseButtonDown(
#line 1178 "src/../include/raylib.sgh"

#line 1178 "src/../include/raylib.sgh"

#line 1178 "src/../include/raylib.sgh"
int button);

#line 1179 "src/../include/raylib.sgh"
extern "C" 
#line 1179 "src/../include/raylib.sgh"
_Bool IsMouseButtonReleased(
#line 1179 "src/../include/raylib.sgh"

#line 1179 "src/../include/raylib.sgh"

#line 1179 "src/../include/raylib.sgh"
int button);

#line 1180 "src/../include/raylib.sgh"
extern "C" 
#line 1180 "src/../include/raylib.sgh"
_Bool IsMouseButtonUp(
#line 1180 "src/../include/raylib.sgh"

#line 1180 "src/../include/raylib.sgh"

#line 1180 "src/../include/raylib.sgh"
int button);

#line 1181 "src/../include/raylib.sgh"
extern "C" 
#line 1181 "src/../include/raylib.sgh"
int GetMouseX(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1181 "src/../include/raylib.sgh"
void);

#line 1182 "src/../include/raylib.sgh"
extern "C" 
#line 1182 "src/../include/raylib.sgh"
int GetMouseY(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1182 "src/../include/raylib.sgh"
void);

#line 1183 "src/../include/raylib.sgh"
extern "C" 
#line 1183 "src/../include/raylib.sgh"
Vector2 GetMousePosition(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1183 "src/../include/raylib.sgh"
void);

#line 1184 "src/../include/raylib.sgh"
extern "C" 
#line 1184 "src/../include/raylib.sgh"
Vector2 GetMouseDelta(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1184 "src/../include/raylib.sgh"
void);

#line 1185 "src/../include/raylib.sgh"
extern "C" 
#line 1185 "src/../include/raylib.sgh"
void SetMousePosition(
#line 1185 "src/../include/raylib.sgh"

#line 1185 "src/../include/raylib.sgh"

#line 1185 "src/../include/raylib.sgh"
int x, 
#line 1185 "src/../include/raylib.sgh"

#line 1185 "src/../include/raylib.sgh"
int y);

#line 1186 "src/../include/raylib.sgh"
extern "C" 
#line 1186 "src/../include/raylib.sgh"
void SetMouseOffset(
#line 1186 "src/../include/raylib.sgh"

#line 1186 "src/../include/raylib.sgh"

#line 1186 "src/../include/raylib.sgh"
int offsetX, 
#line 1186 "src/../include/raylib.sgh"

#line 1186 "src/../include/raylib.sgh"
int offsetY);

#line 1187 "src/../include/raylib.sgh"
extern "C" 
#line 1187 "src/../include/raylib.sgh"
void SetMouseScale(
#line 1187 "src/../include/raylib.sgh"

#line 1187 "src/../include/raylib.sgh"

#line 1187 "src/../include/raylib.sgh"
float scaleX, 
#line 1187 "src/../include/raylib.sgh"

#line 1187 "src/../include/raylib.sgh"
float scaleY);

#line 1188 "src/../include/raylib.sgh"
extern "C" 
#line 1188 "src/../include/raylib.sgh"
float GetMouseWheelMove(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1188 "src/../include/raylib.sgh"
void);

#line 1189 "src/../include/raylib.sgh"
extern "C" 
#line 1189 "src/../include/raylib.sgh"
Vector2 GetMouseWheelMoveV(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1189 "src/../include/raylib.sgh"
void);

#line 1190 "src/../include/raylib.sgh"
extern "C" 
#line 1190 "src/../include/raylib.sgh"
void SetMouseCursor(
#line 1190 "src/../include/raylib.sgh"

#line 1190 "src/../include/raylib.sgh"

#line 1190 "src/../include/raylib.sgh"
int cursor);

#line 1193 "src/../include/raylib.sgh"
extern "C" 
#line 1193 "src/../include/raylib.sgh"
int GetTouchX(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1193 "src/../include/raylib.sgh"
void);

#line 1194 "src/../include/raylib.sgh"
extern "C" 
#line 1194 "src/../include/raylib.sgh"
int GetTouchY(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1194 "src/../include/raylib.sgh"
void);

#line 1195 "src/../include/raylib.sgh"
extern "C" 
#line 1195 "src/../include/raylib.sgh"
Vector2 GetTouchPosition(
#line 1195 "src/../include/raylib.sgh"

#line 1195 "src/../include/raylib.sgh"

#line 1195 "src/../include/raylib.sgh"
int index);

#line 1196 "src/../include/raylib.sgh"
extern "C" 
#line 1196 "src/../include/raylib.sgh"
int GetTouchPointId(
#line 1196 "src/../include/raylib.sgh"

#line 1196 "src/../include/raylib.sgh"

#line 1196 "src/../include/raylib.sgh"
int index);

#line 1197 "src/../include/raylib.sgh"
extern "C" 
#line 1197 "src/../include/raylib.sgh"
int GetTouchPointCount(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1197 "src/../include/raylib.sgh"
void);

#line 1202 "src/../include/raylib.sgh"
extern "C" 
#line 1202 "src/../include/raylib.sgh"
void SetGesturesEnabled(
#line 1202 "src/../include/raylib.sgh"

#line 1202 "src/../include/raylib.sgh"

#line 1202 "src/../include/raylib.sgh"
unsigned int flags);

#line 1203 "src/../include/raylib.sgh"
extern "C" 
#line 1203 "src/../include/raylib.sgh"
_Bool IsGestureDetected(
#line 1203 "src/../include/raylib.sgh"

#line 1203 "src/../include/raylib.sgh"

#line 1203 "src/../include/raylib.sgh"
unsigned int gesture);

#line 1204 "src/../include/raylib.sgh"
extern "C" 
#line 1204 "src/../include/raylib.sgh"
int GetGestureDetected(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1204 "src/../include/raylib.sgh"
void);

#line 1205 "src/../include/raylib.sgh"
extern "C" 
#line 1205 "src/../include/raylib.sgh"
float GetGestureHoldDuration(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1205 "src/../include/raylib.sgh"
void);

#line 1206 "src/../include/raylib.sgh"
extern "C" 
#line 1206 "src/../include/raylib.sgh"
Vector2 GetGestureDragVector(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1206 "src/../include/raylib.sgh"
void);

#line 1207 "src/../include/raylib.sgh"
extern "C" 
#line 1207 "src/../include/raylib.sgh"
float GetGestureDragAngle(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1207 "src/../include/raylib.sgh"
void);

#line 1208 "src/../include/raylib.sgh"
extern "C" 
#line 1208 "src/../include/raylib.sgh"
Vector2 GetGesturePinchVector(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1208 "src/../include/raylib.sgh"
void);

#line 1209 "src/../include/raylib.sgh"
extern "C" 
#line 1209 "src/../include/raylib.sgh"
float GetGesturePinchAngle(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1209 "src/../include/raylib.sgh"
void);

#line 1214 "src/../include/raylib.sgh"
extern "C" 
#line 1214 "src/../include/raylib.sgh"
void UpdateCamera(
#line 1214 "src/../include/raylib.sgh"

#line 1214 "src/../include/raylib.sgh"

#line 1214 "src/../include/raylib.sgh"
Camera *camera, 
#line 1214 "src/../include/raylib.sgh"

#line 1214 "src/../include/raylib.sgh"
int mode);

#line 1215 "src/../include/raylib.sgh"
extern "C" 
#line 1215 "src/../include/raylib.sgh"
void UpdateCameraPro(
#line 1215 "src/../include/raylib.sgh"

#line 1215 "src/../include/raylib.sgh"

#line 1215 "src/../include/raylib.sgh"
Camera *camera, 
#line 1215 "src/../include/raylib.sgh"

#line 1215 "src/../include/raylib.sgh"
Vector3 movement, 
#line 1215 "src/../include/raylib.sgh"

#line 1215 "src/../include/raylib.sgh"
Vector3 rotation, 
#line 1215 "src/../include/raylib.sgh"

#line 1215 "src/../include/raylib.sgh"
float zoom);

#line 1223 "src/../include/raylib.sgh"
extern "C" 
#line 1223 "src/../include/raylib.sgh"
void SetShapesTexture(
#line 1223 "src/../include/raylib.sgh"

#line 1223 "src/../include/raylib.sgh"

#line 1223 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1223 "src/../include/raylib.sgh"

#line 1223 "src/../include/raylib.sgh"
Rectangle source);

#line 1224 "src/../include/raylib.sgh"
extern "C" 
#line 1224 "src/../include/raylib.sgh"
Texture2D GetShapesTexture(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1224 "src/../include/raylib.sgh"
void);

#line 1225 "src/../include/raylib.sgh"
extern "C" 
#line 1225 "src/../include/raylib.sgh"
Rectangle GetShapesTextureRectangle(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1225 "src/../include/raylib.sgh"
void);

#line 1228 "src/../include/raylib.sgh"
extern "C" 
#line 1228 "src/../include/raylib.sgh"
void DrawPixel(
#line 1228 "src/../include/raylib.sgh"

#line 1228 "src/../include/raylib.sgh"

#line 1228 "src/../include/raylib.sgh"
int posX, 
#line 1228 "src/../include/raylib.sgh"

#line 1228 "src/../include/raylib.sgh"
int posY, 
#line 1228 "src/../include/raylib.sgh"

#line 1228 "src/../include/raylib.sgh"
Color color);

#line 1229 "src/../include/raylib.sgh"
extern "C" 
#line 1229 "src/../include/raylib.sgh"
void DrawPixelV(
#line 1229 "src/../include/raylib.sgh"

#line 1229 "src/../include/raylib.sgh"

#line 1229 "src/../include/raylib.sgh"
Vector2 position, 
#line 1229 "src/../include/raylib.sgh"

#line 1229 "src/../include/raylib.sgh"
Color color);

#line 1230 "src/../include/raylib.sgh"
extern "C" 
#line 1230 "src/../include/raylib.sgh"
void DrawLine(
#line 1230 "src/../include/raylib.sgh"

#line 1230 "src/../include/raylib.sgh"

#line 1230 "src/../include/raylib.sgh"
int startPosX, 
#line 1230 "src/../include/raylib.sgh"

#line 1230 "src/../include/raylib.sgh"
int startPosY, 
#line 1230 "src/../include/raylib.sgh"

#line 1230 "src/../include/raylib.sgh"
int endPosX, 
#line 1230 "src/../include/raylib.sgh"

#line 1230 "src/../include/raylib.sgh"
int endPosY, 
#line 1230 "src/../include/raylib.sgh"

#line 1230 "src/../include/raylib.sgh"
Color color);

#line 1231 "src/../include/raylib.sgh"
extern "C" 
#line 1231 "src/../include/raylib.sgh"
void DrawLineV(
#line 1231 "src/../include/raylib.sgh"

#line 1231 "src/../include/raylib.sgh"

#line 1231 "src/../include/raylib.sgh"
Vector2 startPos, 
#line 1231 "src/../include/raylib.sgh"

#line 1231 "src/../include/raylib.sgh"
Vector2 endPos, 
#line 1231 "src/../include/raylib.sgh"

#line 1231 "src/../include/raylib.sgh"
Color color);

#line 1232 "src/../include/raylib.sgh"
extern "C" 
#line 1232 "src/../include/raylib.sgh"
void DrawLineEx(
#line 1232 "src/../include/raylib.sgh"

#line 1232 "src/../include/raylib.sgh"

#line 1232 "src/../include/raylib.sgh"
Vector2 startPos, 
#line 1232 "src/../include/raylib.sgh"

#line 1232 "src/../include/raylib.sgh"
Vector2 endPos, 
#line 1232 "src/../include/raylib.sgh"

#line 1232 "src/../include/raylib.sgh"
float thick, 
#line 1232 "src/../include/raylib.sgh"

#line 1232 "src/../include/raylib.sgh"
Color color);

#line 1233 "src/../include/raylib.sgh"
extern "C" 
#line 1233 "src/../include/raylib.sgh"
void DrawLineStrip(
#line 1233 "src/../include/raylib.sgh"

#line 1233 "src/../include/raylib.sgh"

#line 1233 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1233 "src/../include/raylib.sgh"

#line 1233 "src/../include/raylib.sgh"
int pointCount, 
#line 1233 "src/../include/raylib.sgh"

#line 1233 "src/../include/raylib.sgh"
Color color);

#line 1234 "src/../include/raylib.sgh"
extern "C" 
#line 1234 "src/../include/raylib.sgh"
void DrawLineBezier(
#line 1234 "src/../include/raylib.sgh"

#line 1234 "src/../include/raylib.sgh"

#line 1234 "src/../include/raylib.sgh"
Vector2 startPos, 
#line 1234 "src/../include/raylib.sgh"

#line 1234 "src/../include/raylib.sgh"
Vector2 endPos, 
#line 1234 "src/../include/raylib.sgh"

#line 1234 "src/../include/raylib.sgh"
float thick, 
#line 1234 "src/../include/raylib.sgh"

#line 1234 "src/../include/raylib.sgh"
Color color);

#line 1235 "src/../include/raylib.sgh"
extern "C" 
#line 1235 "src/../include/raylib.sgh"
void DrawCircle(
#line 1235 "src/../include/raylib.sgh"

#line 1235 "src/../include/raylib.sgh"

#line 1235 "src/../include/raylib.sgh"
int centerX, 
#line 1235 "src/../include/raylib.sgh"

#line 1235 "src/../include/raylib.sgh"
int centerY, 
#line 1235 "src/../include/raylib.sgh"

#line 1235 "src/../include/raylib.sgh"
float radius, 
#line 1235 "src/../include/raylib.sgh"

#line 1235 "src/../include/raylib.sgh"
Color color);

#line 1236 "src/../include/raylib.sgh"
extern "C" 
#line 1236 "src/../include/raylib.sgh"
void DrawCircleSector(
#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"
Vector2 center, 
#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"
float radius, 
#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"
float startAngle, 
#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"
float endAngle, 
#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"
int segments, 
#line 1236 "src/../include/raylib.sgh"

#line 1236 "src/../include/raylib.sgh"
Color color);

#line 1237 "src/../include/raylib.sgh"
extern "C" 
#line 1237 "src/../include/raylib.sgh"
void DrawCircleSectorLines(
#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"
Vector2 center, 
#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"
float radius, 
#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"
float startAngle, 
#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"
float endAngle, 
#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"
int segments, 
#line 1237 "src/../include/raylib.sgh"

#line 1237 "src/../include/raylib.sgh"
Color color);

#line 1238 "src/../include/raylib.sgh"
extern "C" 
#line 1238 "src/../include/raylib.sgh"
void DrawCircleGradient(
#line 1238 "src/../include/raylib.sgh"

#line 1238 "src/../include/raylib.sgh"

#line 1238 "src/../include/raylib.sgh"
int centerX, 
#line 1238 "src/../include/raylib.sgh"

#line 1238 "src/../include/raylib.sgh"
int centerY, 
#line 1238 "src/../include/raylib.sgh"

#line 1238 "src/../include/raylib.sgh"
float radius, 
#line 1238 "src/../include/raylib.sgh"

#line 1238 "src/../include/raylib.sgh"
Color color1, 
#line 1238 "src/../include/raylib.sgh"

#line 1238 "src/../include/raylib.sgh"
Color color2);

#line 1239 "src/../include/raylib.sgh"
extern "C" 
#line 1239 "src/../include/raylib.sgh"
void DrawCircleV(
#line 1239 "src/../include/raylib.sgh"

#line 1239 "src/../include/raylib.sgh"

#line 1239 "src/../include/raylib.sgh"
Vector2 center, 
#line 1239 "src/../include/raylib.sgh"

#line 1239 "src/../include/raylib.sgh"
float radius, 
#line 1239 "src/../include/raylib.sgh"

#line 1239 "src/../include/raylib.sgh"
Color color);

#line 1240 "src/../include/raylib.sgh"
extern "C" 
#line 1240 "src/../include/raylib.sgh"
void DrawCircleLines(
#line 1240 "src/../include/raylib.sgh"

#line 1240 "src/../include/raylib.sgh"

#line 1240 "src/../include/raylib.sgh"
int centerX, 
#line 1240 "src/../include/raylib.sgh"

#line 1240 "src/../include/raylib.sgh"
int centerY, 
#line 1240 "src/../include/raylib.sgh"

#line 1240 "src/../include/raylib.sgh"
float radius, 
#line 1240 "src/../include/raylib.sgh"

#line 1240 "src/../include/raylib.sgh"
Color color);

#line 1241 "src/../include/raylib.sgh"
extern "C" 
#line 1241 "src/../include/raylib.sgh"
void DrawCircleLinesV(
#line 1241 "src/../include/raylib.sgh"

#line 1241 "src/../include/raylib.sgh"

#line 1241 "src/../include/raylib.sgh"
Vector2 center, 
#line 1241 "src/../include/raylib.sgh"

#line 1241 "src/../include/raylib.sgh"
float radius, 
#line 1241 "src/../include/raylib.sgh"

#line 1241 "src/../include/raylib.sgh"
Color color);

#line 1242 "src/../include/raylib.sgh"
extern "C" 
#line 1242 "src/../include/raylib.sgh"
void DrawEllipse(
#line 1242 "src/../include/raylib.sgh"

#line 1242 "src/../include/raylib.sgh"

#line 1242 "src/../include/raylib.sgh"
int centerX, 
#line 1242 "src/../include/raylib.sgh"

#line 1242 "src/../include/raylib.sgh"
int centerY, 
#line 1242 "src/../include/raylib.sgh"

#line 1242 "src/../include/raylib.sgh"
float radiusH, 
#line 1242 "src/../include/raylib.sgh"

#line 1242 "src/../include/raylib.sgh"
float radiusV, 
#line 1242 "src/../include/raylib.sgh"

#line 1242 "src/../include/raylib.sgh"
Color color);

#line 1243 "src/../include/raylib.sgh"
extern "C" 
#line 1243 "src/../include/raylib.sgh"
void DrawEllipseLines(
#line 1243 "src/../include/raylib.sgh"

#line 1243 "src/../include/raylib.sgh"

#line 1243 "src/../include/raylib.sgh"
int centerX, 
#line 1243 "src/../include/raylib.sgh"

#line 1243 "src/../include/raylib.sgh"
int centerY, 
#line 1243 "src/../include/raylib.sgh"

#line 1243 "src/../include/raylib.sgh"
float radiusH, 
#line 1243 "src/../include/raylib.sgh"

#line 1243 "src/../include/raylib.sgh"
float radiusV, 
#line 1243 "src/../include/raylib.sgh"

#line 1243 "src/../include/raylib.sgh"
Color color);

#line 1244 "src/../include/raylib.sgh"
extern "C" 
#line 1244 "src/../include/raylib.sgh"
void DrawRing(
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
Vector2 center, 
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
float innerRadius, 
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
float outerRadius, 
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
float startAngle, 
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
float endAngle, 
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
int segments, 
#line 1244 "src/../include/raylib.sgh"

#line 1244 "src/../include/raylib.sgh"
Color color);

#line 1245 "src/../include/raylib.sgh"
extern "C" 
#line 1245 "src/../include/raylib.sgh"
void DrawRingLines(
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
Vector2 center, 
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
float innerRadius, 
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
float outerRadius, 
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
float startAngle, 
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
float endAngle, 
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
int segments, 
#line 1245 "src/../include/raylib.sgh"

#line 1245 "src/../include/raylib.sgh"
Color color);

#line 1246 "src/../include/raylib.sgh"
extern "C" 
#line 1246 "src/../include/raylib.sgh"
void DrawRectangle(
#line 1246 "src/../include/raylib.sgh"

#line 1246 "src/../include/raylib.sgh"

#line 1246 "src/../include/raylib.sgh"
int posX, 
#line 1246 "src/../include/raylib.sgh"

#line 1246 "src/../include/raylib.sgh"
int posY, 
#line 1246 "src/../include/raylib.sgh"

#line 1246 "src/../include/raylib.sgh"
int width, 
#line 1246 "src/../include/raylib.sgh"

#line 1246 "src/../include/raylib.sgh"
int height, 
#line 1246 "src/../include/raylib.sgh"

#line 1246 "src/../include/raylib.sgh"
Color color);

#line 1247 "src/../include/raylib.sgh"
extern "C" 
#line 1247 "src/../include/raylib.sgh"
void DrawRectangleV(
#line 1247 "src/../include/raylib.sgh"

#line 1247 "src/../include/raylib.sgh"

#line 1247 "src/../include/raylib.sgh"
Vector2 position, 
#line 1247 "src/../include/raylib.sgh"

#line 1247 "src/../include/raylib.sgh"
Vector2 size, 
#line 1247 "src/../include/raylib.sgh"

#line 1247 "src/../include/raylib.sgh"
Color color);

#line 1248 "src/../include/raylib.sgh"
extern "C" 
#line 1248 "src/../include/raylib.sgh"
void DrawRectangleRec(
#line 1248 "src/../include/raylib.sgh"

#line 1248 "src/../include/raylib.sgh"

#line 1248 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1248 "src/../include/raylib.sgh"

#line 1248 "src/../include/raylib.sgh"
Color color);

#line 1249 "src/../include/raylib.sgh"
extern "C" 
#line 1249 "src/../include/raylib.sgh"
void DrawRectanglePro(
#line 1249 "src/../include/raylib.sgh"

#line 1249 "src/../include/raylib.sgh"

#line 1249 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1249 "src/../include/raylib.sgh"

#line 1249 "src/../include/raylib.sgh"
Vector2 origin, 
#line 1249 "src/../include/raylib.sgh"

#line 1249 "src/../include/raylib.sgh"
float rotation, 
#line 1249 "src/../include/raylib.sgh"

#line 1249 "src/../include/raylib.sgh"
Color color);

#line 1250 "src/../include/raylib.sgh"
extern "C" 
#line 1250 "src/../include/raylib.sgh"
void DrawRectangleGradientV(
#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"
int posX, 
#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"
int posY, 
#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"
int width, 
#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"
int height, 
#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"
Color color1, 
#line 1250 "src/../include/raylib.sgh"

#line 1250 "src/../include/raylib.sgh"
Color color2);

#line 1251 "src/../include/raylib.sgh"
extern "C" 
#line 1251 "src/../include/raylib.sgh"
void DrawRectangleGradientH(
#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"
int posX, 
#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"
int posY, 
#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"
int width, 
#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"
int height, 
#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"
Color color1, 
#line 1251 "src/../include/raylib.sgh"

#line 1251 "src/../include/raylib.sgh"
Color color2);

#line 1252 "src/../include/raylib.sgh"
extern "C" 
#line 1252 "src/../include/raylib.sgh"
void DrawRectangleGradientEx(
#line 1252 "src/../include/raylib.sgh"

#line 1252 "src/../include/raylib.sgh"

#line 1252 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1252 "src/../include/raylib.sgh"

#line 1252 "src/../include/raylib.sgh"
Color col1, 
#line 1252 "src/../include/raylib.sgh"

#line 1252 "src/../include/raylib.sgh"
Color col2, 
#line 1252 "src/../include/raylib.sgh"

#line 1252 "src/../include/raylib.sgh"
Color col3, 
#line 1252 "src/../include/raylib.sgh"

#line 1252 "src/../include/raylib.sgh"
Color col4);

#line 1253 "src/../include/raylib.sgh"
extern "C" 
#line 1253 "src/../include/raylib.sgh"
void DrawRectangleLines(
#line 1253 "src/../include/raylib.sgh"

#line 1253 "src/../include/raylib.sgh"

#line 1253 "src/../include/raylib.sgh"
int posX, 
#line 1253 "src/../include/raylib.sgh"

#line 1253 "src/../include/raylib.sgh"
int posY, 
#line 1253 "src/../include/raylib.sgh"

#line 1253 "src/../include/raylib.sgh"
int width, 
#line 1253 "src/../include/raylib.sgh"

#line 1253 "src/../include/raylib.sgh"
int height, 
#line 1253 "src/../include/raylib.sgh"

#line 1253 "src/../include/raylib.sgh"
Color color);

#line 1254 "src/../include/raylib.sgh"
extern "C" 
#line 1254 "src/../include/raylib.sgh"
void DrawRectangleLinesEx(
#line 1254 "src/../include/raylib.sgh"

#line 1254 "src/../include/raylib.sgh"

#line 1254 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1254 "src/../include/raylib.sgh"

#line 1254 "src/../include/raylib.sgh"
float lineThick, 
#line 1254 "src/../include/raylib.sgh"

#line 1254 "src/../include/raylib.sgh"
Color color);

#line 1255 "src/../include/raylib.sgh"
extern "C" 
#line 1255 "src/../include/raylib.sgh"
void DrawRectangleRounded(
#line 1255 "src/../include/raylib.sgh"

#line 1255 "src/../include/raylib.sgh"

#line 1255 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1255 "src/../include/raylib.sgh"

#line 1255 "src/../include/raylib.sgh"
float roundness, 
#line 1255 "src/../include/raylib.sgh"

#line 1255 "src/../include/raylib.sgh"
int segments, 
#line 1255 "src/../include/raylib.sgh"

#line 1255 "src/../include/raylib.sgh"
Color color);

#line 1256 "src/../include/raylib.sgh"
extern "C" 
#line 1256 "src/../include/raylib.sgh"
void DrawRectangleRoundedLines(
#line 1256 "src/../include/raylib.sgh"

#line 1256 "src/../include/raylib.sgh"

#line 1256 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1256 "src/../include/raylib.sgh"

#line 1256 "src/../include/raylib.sgh"
float roundness, 
#line 1256 "src/../include/raylib.sgh"

#line 1256 "src/../include/raylib.sgh"
int segments, 
#line 1256 "src/../include/raylib.sgh"

#line 1256 "src/../include/raylib.sgh"
float lineThick, 
#line 1256 "src/../include/raylib.sgh"

#line 1256 "src/../include/raylib.sgh"
Color color);

#line 1257 "src/../include/raylib.sgh"
extern "C" 
#line 1257 "src/../include/raylib.sgh"
void DrawTriangle(
#line 1257 "src/../include/raylib.sgh"

#line 1257 "src/../include/raylib.sgh"

#line 1257 "src/../include/raylib.sgh"
Vector2 v1, 
#line 1257 "src/../include/raylib.sgh"

#line 1257 "src/../include/raylib.sgh"
Vector2 v2, 
#line 1257 "src/../include/raylib.sgh"

#line 1257 "src/../include/raylib.sgh"
Vector2 v3, 
#line 1257 "src/../include/raylib.sgh"

#line 1257 "src/../include/raylib.sgh"
Color color);

#line 1258 "src/../include/raylib.sgh"
extern "C" 
#line 1258 "src/../include/raylib.sgh"
void DrawTriangleLines(
#line 1258 "src/../include/raylib.sgh"

#line 1258 "src/../include/raylib.sgh"

#line 1258 "src/../include/raylib.sgh"
Vector2 v1, 
#line 1258 "src/../include/raylib.sgh"

#line 1258 "src/../include/raylib.sgh"
Vector2 v2, 
#line 1258 "src/../include/raylib.sgh"

#line 1258 "src/../include/raylib.sgh"
Vector2 v3, 
#line 1258 "src/../include/raylib.sgh"

#line 1258 "src/../include/raylib.sgh"
Color color);

#line 1259 "src/../include/raylib.sgh"
extern "C" 
#line 1259 "src/../include/raylib.sgh"
void DrawTriangleFan(
#line 1259 "src/../include/raylib.sgh"

#line 1259 "src/../include/raylib.sgh"

#line 1259 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1259 "src/../include/raylib.sgh"

#line 1259 "src/../include/raylib.sgh"
int pointCount, 
#line 1259 "src/../include/raylib.sgh"

#line 1259 "src/../include/raylib.sgh"
Color color);

#line 1260 "src/../include/raylib.sgh"
extern "C" 
#line 1260 "src/../include/raylib.sgh"
void DrawTriangleStrip(
#line 1260 "src/../include/raylib.sgh"

#line 1260 "src/../include/raylib.sgh"

#line 1260 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1260 "src/../include/raylib.sgh"

#line 1260 "src/../include/raylib.sgh"
int pointCount, 
#line 1260 "src/../include/raylib.sgh"

#line 1260 "src/../include/raylib.sgh"
Color color);

#line 1261 "src/../include/raylib.sgh"
extern "C" 
#line 1261 "src/../include/raylib.sgh"
void DrawPoly(
#line 1261 "src/../include/raylib.sgh"

#line 1261 "src/../include/raylib.sgh"

#line 1261 "src/../include/raylib.sgh"
Vector2 center, 
#line 1261 "src/../include/raylib.sgh"

#line 1261 "src/../include/raylib.sgh"
int sides, 
#line 1261 "src/../include/raylib.sgh"

#line 1261 "src/../include/raylib.sgh"
float radius, 
#line 1261 "src/../include/raylib.sgh"

#line 1261 "src/../include/raylib.sgh"
float rotation, 
#line 1261 "src/../include/raylib.sgh"

#line 1261 "src/../include/raylib.sgh"
Color color);

#line 1262 "src/../include/raylib.sgh"
extern "C" 
#line 1262 "src/../include/raylib.sgh"
void DrawPolyLines(
#line 1262 "src/../include/raylib.sgh"

#line 1262 "src/../include/raylib.sgh"

#line 1262 "src/../include/raylib.sgh"
Vector2 center, 
#line 1262 "src/../include/raylib.sgh"

#line 1262 "src/../include/raylib.sgh"
int sides, 
#line 1262 "src/../include/raylib.sgh"

#line 1262 "src/../include/raylib.sgh"
float radius, 
#line 1262 "src/../include/raylib.sgh"

#line 1262 "src/../include/raylib.sgh"
float rotation, 
#line 1262 "src/../include/raylib.sgh"

#line 1262 "src/../include/raylib.sgh"
Color color);

#line 1263 "src/../include/raylib.sgh"
extern "C" 
#line 1263 "src/../include/raylib.sgh"
void DrawPolyLinesEx(
#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"
Vector2 center, 
#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"
int sides, 
#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"
float radius, 
#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"
float rotation, 
#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"
float lineThick, 
#line 1263 "src/../include/raylib.sgh"

#line 1263 "src/../include/raylib.sgh"
Color color);

#line 1266 "src/../include/raylib.sgh"
extern "C" 
#line 1266 "src/../include/raylib.sgh"
void DrawSplineLinear(
#line 1266 "src/../include/raylib.sgh"

#line 1266 "src/../include/raylib.sgh"

#line 1266 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1266 "src/../include/raylib.sgh"

#line 1266 "src/../include/raylib.sgh"
int pointCount, 
#line 1266 "src/../include/raylib.sgh"

#line 1266 "src/../include/raylib.sgh"
float thick, 
#line 1266 "src/../include/raylib.sgh"

#line 1266 "src/../include/raylib.sgh"
Color color);

#line 1267 "src/../include/raylib.sgh"
extern "C" 
#line 1267 "src/../include/raylib.sgh"
void DrawSplineBasis(
#line 1267 "src/../include/raylib.sgh"

#line 1267 "src/../include/raylib.sgh"

#line 1267 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1267 "src/../include/raylib.sgh"

#line 1267 "src/../include/raylib.sgh"
int pointCount, 
#line 1267 "src/../include/raylib.sgh"

#line 1267 "src/../include/raylib.sgh"
float thick, 
#line 1267 "src/../include/raylib.sgh"

#line 1267 "src/../include/raylib.sgh"
Color color);

#line 1268 "src/../include/raylib.sgh"
extern "C" 
#line 1268 "src/../include/raylib.sgh"
void DrawSplineCatmullRom(
#line 1268 "src/../include/raylib.sgh"

#line 1268 "src/../include/raylib.sgh"

#line 1268 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1268 "src/../include/raylib.sgh"

#line 1268 "src/../include/raylib.sgh"
int pointCount, 
#line 1268 "src/../include/raylib.sgh"

#line 1268 "src/../include/raylib.sgh"
float thick, 
#line 1268 "src/../include/raylib.sgh"

#line 1268 "src/../include/raylib.sgh"
Color color);

#line 1269 "src/../include/raylib.sgh"
extern "C" 
#line 1269 "src/../include/raylib.sgh"
void DrawSplineBezierQuadratic(
#line 1269 "src/../include/raylib.sgh"

#line 1269 "src/../include/raylib.sgh"

#line 1269 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1269 "src/../include/raylib.sgh"

#line 1269 "src/../include/raylib.sgh"
int pointCount, 
#line 1269 "src/../include/raylib.sgh"

#line 1269 "src/../include/raylib.sgh"
float thick, 
#line 1269 "src/../include/raylib.sgh"

#line 1269 "src/../include/raylib.sgh"
Color color);

#line 1270 "src/../include/raylib.sgh"
extern "C" 
#line 1270 "src/../include/raylib.sgh"
void DrawSplineBezierCubic(
#line 1270 "src/../include/raylib.sgh"

#line 1270 "src/../include/raylib.sgh"

#line 1270 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1270 "src/../include/raylib.sgh"

#line 1270 "src/../include/raylib.sgh"
int pointCount, 
#line 1270 "src/../include/raylib.sgh"

#line 1270 "src/../include/raylib.sgh"
float thick, 
#line 1270 "src/../include/raylib.sgh"

#line 1270 "src/../include/raylib.sgh"
Color color);

#line 1271 "src/../include/raylib.sgh"
extern "C" 
#line 1271 "src/../include/raylib.sgh"
void DrawSplineSegmentLinear(
#line 1271 "src/../include/raylib.sgh"

#line 1271 "src/../include/raylib.sgh"

#line 1271 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1271 "src/../include/raylib.sgh"

#line 1271 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1271 "src/../include/raylib.sgh"

#line 1271 "src/../include/raylib.sgh"
float thick, 
#line 1271 "src/../include/raylib.sgh"

#line 1271 "src/../include/raylib.sgh"
Color color);

#line 1272 "src/../include/raylib.sgh"
extern "C" 
#line 1272 "src/../include/raylib.sgh"
void DrawSplineSegmentBasis(
#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"
Vector2 p3, 
#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"
Vector2 p4, 
#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"
float thick, 
#line 1272 "src/../include/raylib.sgh"

#line 1272 "src/../include/raylib.sgh"
Color color);

#line 1273 "src/../include/raylib.sgh"
extern "C" 
#line 1273 "src/../include/raylib.sgh"
void DrawSplineSegmentCatmullRom(
#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"
Vector2 p3, 
#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"
Vector2 p4, 
#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"
float thick, 
#line 1273 "src/../include/raylib.sgh"

#line 1273 "src/../include/raylib.sgh"
Color color);

#line 1274 "src/../include/raylib.sgh"
extern "C" 
#line 1274 "src/../include/raylib.sgh"
void DrawSplineSegmentBezierQuadratic(
#line 1274 "src/../include/raylib.sgh"

#line 1274 "src/../include/raylib.sgh"

#line 1274 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1274 "src/../include/raylib.sgh"

#line 1274 "src/../include/raylib.sgh"
Vector2 c2, 
#line 1274 "src/../include/raylib.sgh"

#line 1274 "src/../include/raylib.sgh"
Vector2 p3, 
#line 1274 "src/../include/raylib.sgh"

#line 1274 "src/../include/raylib.sgh"
float thick, 
#line 1274 "src/../include/raylib.sgh"

#line 1274 "src/../include/raylib.sgh"
Color color);

#line 1275 "src/../include/raylib.sgh"
extern "C" 
#line 1275 "src/../include/raylib.sgh"
void DrawSplineSegmentBezierCubic(
#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"
Vector2 c2, 
#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"
Vector2 c3, 
#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"
Vector2 p4, 
#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"
float thick, 
#line 1275 "src/../include/raylib.sgh"

#line 1275 "src/../include/raylib.sgh"
Color color);

#line 1278 "src/../include/raylib.sgh"
extern "C" 
#line 1278 "src/../include/raylib.sgh"
Vector2 GetSplinePointLinear(
#line 1278 "src/../include/raylib.sgh"

#line 1278 "src/../include/raylib.sgh"

#line 1278 "src/../include/raylib.sgh"
Vector2 startPos, 
#line 1278 "src/../include/raylib.sgh"

#line 1278 "src/../include/raylib.sgh"
Vector2 endPos, 
#line 1278 "src/../include/raylib.sgh"

#line 1278 "src/../include/raylib.sgh"
float t);

#line 1279 "src/../include/raylib.sgh"
extern "C" 
#line 1279 "src/../include/raylib.sgh"
Vector2 GetSplinePointBasis(
#line 1279 "src/../include/raylib.sgh"

#line 1279 "src/../include/raylib.sgh"

#line 1279 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1279 "src/../include/raylib.sgh"

#line 1279 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1279 "src/../include/raylib.sgh"

#line 1279 "src/../include/raylib.sgh"
Vector2 p3, 
#line 1279 "src/../include/raylib.sgh"

#line 1279 "src/../include/raylib.sgh"
Vector2 p4, 
#line 1279 "src/../include/raylib.sgh"

#line 1279 "src/../include/raylib.sgh"
float t);

#line 1280 "src/../include/raylib.sgh"
extern "C" 
#line 1280 "src/../include/raylib.sgh"
Vector2 GetSplinePointCatmullRom(
#line 1280 "src/../include/raylib.sgh"

#line 1280 "src/../include/raylib.sgh"

#line 1280 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1280 "src/../include/raylib.sgh"

#line 1280 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1280 "src/../include/raylib.sgh"

#line 1280 "src/../include/raylib.sgh"
Vector2 p3, 
#line 1280 "src/../include/raylib.sgh"

#line 1280 "src/../include/raylib.sgh"
Vector2 p4, 
#line 1280 "src/../include/raylib.sgh"

#line 1280 "src/../include/raylib.sgh"
float t);

#line 1281 "src/../include/raylib.sgh"
extern "C" 
#line 1281 "src/../include/raylib.sgh"
Vector2 GetSplinePointBezierQuad(
#line 1281 "src/../include/raylib.sgh"

#line 1281 "src/../include/raylib.sgh"

#line 1281 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1281 "src/../include/raylib.sgh"

#line 1281 "src/../include/raylib.sgh"
Vector2 c2, 
#line 1281 "src/../include/raylib.sgh"

#line 1281 "src/../include/raylib.sgh"
Vector2 p3, 
#line 1281 "src/../include/raylib.sgh"

#line 1281 "src/../include/raylib.sgh"
float t);

#line 1282 "src/../include/raylib.sgh"
extern "C" 
#line 1282 "src/../include/raylib.sgh"
Vector2 GetSplinePointBezierCubic(
#line 1282 "src/../include/raylib.sgh"

#line 1282 "src/../include/raylib.sgh"

#line 1282 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1282 "src/../include/raylib.sgh"

#line 1282 "src/../include/raylib.sgh"
Vector2 c2, 
#line 1282 "src/../include/raylib.sgh"

#line 1282 "src/../include/raylib.sgh"
Vector2 c3, 
#line 1282 "src/../include/raylib.sgh"

#line 1282 "src/../include/raylib.sgh"
Vector2 p4, 
#line 1282 "src/../include/raylib.sgh"

#line 1282 "src/../include/raylib.sgh"
float t);

#line 1285 "src/../include/raylib.sgh"
extern "C" 
#line 1285 "src/../include/raylib.sgh"
_Bool CheckCollisionRecs(
#line 1285 "src/../include/raylib.sgh"

#line 1285 "src/../include/raylib.sgh"

#line 1285 "src/../include/raylib.sgh"
Rectangle rec1, 
#line 1285 "src/../include/raylib.sgh"

#line 1285 "src/../include/raylib.sgh"
Rectangle rec2);

#line 1286 "src/../include/raylib.sgh"
extern "C" 
#line 1286 "src/../include/raylib.sgh"
_Bool CheckCollisionCircles(
#line 1286 "src/../include/raylib.sgh"

#line 1286 "src/../include/raylib.sgh"

#line 1286 "src/../include/raylib.sgh"
Vector2 center1, 
#line 1286 "src/../include/raylib.sgh"

#line 1286 "src/../include/raylib.sgh"
float radius1, 
#line 1286 "src/../include/raylib.sgh"

#line 1286 "src/../include/raylib.sgh"
Vector2 center2, 
#line 1286 "src/../include/raylib.sgh"

#line 1286 "src/../include/raylib.sgh"
float radius2);

#line 1287 "src/../include/raylib.sgh"
extern "C" 
#line 1287 "src/../include/raylib.sgh"
_Bool CheckCollisionCircleRec(
#line 1287 "src/../include/raylib.sgh"

#line 1287 "src/../include/raylib.sgh"

#line 1287 "src/../include/raylib.sgh"
Vector2 center, 
#line 1287 "src/../include/raylib.sgh"

#line 1287 "src/../include/raylib.sgh"
float radius, 
#line 1287 "src/../include/raylib.sgh"

#line 1287 "src/../include/raylib.sgh"
Rectangle rec);

#line 1288 "src/../include/raylib.sgh"
extern "C" 
#line 1288 "src/../include/raylib.sgh"
_Bool CheckCollisionPointRec(
#line 1288 "src/../include/raylib.sgh"

#line 1288 "src/../include/raylib.sgh"

#line 1288 "src/../include/raylib.sgh"
Vector2 point, 
#line 1288 "src/../include/raylib.sgh"

#line 1288 "src/../include/raylib.sgh"
Rectangle rec);

#line 1289 "src/../include/raylib.sgh"
extern "C" 
#line 1289 "src/../include/raylib.sgh"
_Bool CheckCollisionPointCircle(
#line 1289 "src/../include/raylib.sgh"

#line 1289 "src/../include/raylib.sgh"

#line 1289 "src/../include/raylib.sgh"
Vector2 point, 
#line 1289 "src/../include/raylib.sgh"

#line 1289 "src/../include/raylib.sgh"
Vector2 center, 
#line 1289 "src/../include/raylib.sgh"

#line 1289 "src/../include/raylib.sgh"
float radius);

#line 1290 "src/../include/raylib.sgh"
extern "C" 
#line 1290 "src/../include/raylib.sgh"
_Bool CheckCollisionPointTriangle(
#line 1290 "src/../include/raylib.sgh"

#line 1290 "src/../include/raylib.sgh"

#line 1290 "src/../include/raylib.sgh"
Vector2 point, 
#line 1290 "src/../include/raylib.sgh"

#line 1290 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1290 "src/../include/raylib.sgh"

#line 1290 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1290 "src/../include/raylib.sgh"

#line 1290 "src/../include/raylib.sgh"
Vector2 p3);

#line 1291 "src/../include/raylib.sgh"
extern "C" 
#line 1291 "src/../include/raylib.sgh"
_Bool CheckCollisionPointPoly(
#line 1291 "src/../include/raylib.sgh"

#line 1291 "src/../include/raylib.sgh"

#line 1291 "src/../include/raylib.sgh"
Vector2 point, 
#line 1291 "src/../include/raylib.sgh"

#line 1291 "src/../include/raylib.sgh"
Vector2 *points, 
#line 1291 "src/../include/raylib.sgh"

#line 1291 "src/../include/raylib.sgh"
int pointCount);

#line 1292 "src/../include/raylib.sgh"
extern "C" 
#line 1292 "src/../include/raylib.sgh"
_Bool CheckCollisionLines(
#line 1292 "src/../include/raylib.sgh"

#line 1292 "src/../include/raylib.sgh"

#line 1292 "src/../include/raylib.sgh"
Vector2 startPos1, 
#line 1292 "src/../include/raylib.sgh"

#line 1292 "src/../include/raylib.sgh"
Vector2 endPos1, 
#line 1292 "src/../include/raylib.sgh"

#line 1292 "src/../include/raylib.sgh"
Vector2 startPos2, 
#line 1292 "src/../include/raylib.sgh"

#line 1292 "src/../include/raylib.sgh"
Vector2 endPos2, 
#line 1292 "src/../include/raylib.sgh"

#line 1292 "src/../include/raylib.sgh"
Vector2 *collisionPoint);

#line 1293 "src/../include/raylib.sgh"
extern "C" 
#line 1293 "src/../include/raylib.sgh"
_Bool CheckCollisionPointLine(
#line 1293 "src/../include/raylib.sgh"

#line 1293 "src/../include/raylib.sgh"

#line 1293 "src/../include/raylib.sgh"
Vector2 point, 
#line 1293 "src/../include/raylib.sgh"

#line 1293 "src/../include/raylib.sgh"
Vector2 p1, 
#line 1293 "src/../include/raylib.sgh"

#line 1293 "src/../include/raylib.sgh"
Vector2 p2, 
#line 1293 "src/../include/raylib.sgh"

#line 1293 "src/../include/raylib.sgh"
int threshold);

#line 1294 "src/../include/raylib.sgh"
extern "C" 
#line 1294 "src/../include/raylib.sgh"
Rectangle GetCollisionRec(
#line 1294 "src/../include/raylib.sgh"

#line 1294 "src/../include/raylib.sgh"

#line 1294 "src/../include/raylib.sgh"
Rectangle rec1, 
#line 1294 "src/../include/raylib.sgh"

#line 1294 "src/../include/raylib.sgh"
Rectangle rec2);

#line 1302 "src/../include/raylib.sgh"
extern "C" 
#line 1302 "src/../include/raylib.sgh"
Image LoadImage(
#line 1302 "src/../include/raylib.sgh"

#line 1302 "src/../include/raylib.sgh"
const 
#line 1302 "src/../include/raylib.sgh"
char *fileName);

#line 1303 "src/../include/raylib.sgh"
extern "C" 
#line 1303 "src/../include/raylib.sgh"
Image LoadImageRaw(
#line 1303 "src/../include/raylib.sgh"

#line 1303 "src/../include/raylib.sgh"
const 
#line 1303 "src/../include/raylib.sgh"
char *fileName, 
#line 1303 "src/../include/raylib.sgh"

#line 1303 "src/../include/raylib.sgh"
int width, 
#line 1303 "src/../include/raylib.sgh"

#line 1303 "src/../include/raylib.sgh"
int height, 
#line 1303 "src/../include/raylib.sgh"

#line 1303 "src/../include/raylib.sgh"
int format, 
#line 1303 "src/../include/raylib.sgh"

#line 1303 "src/../include/raylib.sgh"
int headerSize);

#line 1304 "src/../include/raylib.sgh"
extern "C" 
#line 1304 "src/../include/raylib.sgh"
Image LoadImageSvg(
#line 1304 "src/../include/raylib.sgh"

#line 1304 "src/../include/raylib.sgh"
const 
#line 1304 "src/../include/raylib.sgh"
char *fileNameOrString, 
#line 1304 "src/../include/raylib.sgh"

#line 1304 "src/../include/raylib.sgh"
int width, 
#line 1304 "src/../include/raylib.sgh"

#line 1304 "src/../include/raylib.sgh"
int height);

#line 1305 "src/../include/raylib.sgh"
extern "C" 
#line 1305 "src/../include/raylib.sgh"
Image LoadImageAnim(
#line 1305 "src/../include/raylib.sgh"

#line 1305 "src/../include/raylib.sgh"
const 
#line 1305 "src/../include/raylib.sgh"
char *fileName, 
#line 1305 "src/../include/raylib.sgh"

#line 1305 "src/../include/raylib.sgh"
int *frames);

#line 1306 "src/../include/raylib.sgh"
extern "C" 
#line 1306 "src/../include/raylib.sgh"
Image LoadImageAnimFromMemory(
#line 1306 "src/../include/raylib.sgh"

#line 1306 "src/../include/raylib.sgh"
const 
#line 1306 "src/../include/raylib.sgh"
char *fileType, 
#line 1306 "src/../include/raylib.sgh"
const 
#line 1306 "src/../include/raylib.sgh"
unsigned char *fileData, 
#line 1306 "src/../include/raylib.sgh"

#line 1306 "src/../include/raylib.sgh"
int dataSize, 
#line 1306 "src/../include/raylib.sgh"

#line 1306 "src/../include/raylib.sgh"
int *frames);

#line 1307 "src/../include/raylib.sgh"
extern "C" 
#line 1307 "src/../include/raylib.sgh"
Image LoadImageFromMemory(
#line 1307 "src/../include/raylib.sgh"

#line 1307 "src/../include/raylib.sgh"
const 
#line 1307 "src/../include/raylib.sgh"
char *fileType, 
#line 1307 "src/../include/raylib.sgh"
const 
#line 1307 "src/../include/raylib.sgh"
unsigned char *fileData, 
#line 1307 "src/../include/raylib.sgh"

#line 1307 "src/../include/raylib.sgh"
int dataSize);

#line 1308 "src/../include/raylib.sgh"
extern "C" 
#line 1308 "src/../include/raylib.sgh"
Image LoadImageFromTexture(
#line 1308 "src/../include/raylib.sgh"

#line 1308 "src/../include/raylib.sgh"

#line 1308 "src/../include/raylib.sgh"
Texture2D texture);

#line 1309 "src/../include/raylib.sgh"
extern "C" 
#line 1309 "src/../include/raylib.sgh"
Image LoadImageFromScreen(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1309 "src/../include/raylib.sgh"
void);

#line 1310 "src/../include/raylib.sgh"
extern "C" 
#line 1310 "src/../include/raylib.sgh"
_Bool IsImageReady(
#line 1310 "src/../include/raylib.sgh"

#line 1310 "src/../include/raylib.sgh"

#line 1310 "src/../include/raylib.sgh"
Image image);

#line 1311 "src/../include/raylib.sgh"
extern "C" 
#line 1311 "src/../include/raylib.sgh"
void UnloadImage(
#line 1311 "src/../include/raylib.sgh"

#line 1311 "src/../include/raylib.sgh"

#line 1311 "src/../include/raylib.sgh"
Image image);

#line 1312 "src/../include/raylib.sgh"
extern "C" 
#line 1312 "src/../include/raylib.sgh"
_Bool ExportImage(
#line 1312 "src/../include/raylib.sgh"

#line 1312 "src/../include/raylib.sgh"

#line 1312 "src/../include/raylib.sgh"
Image image, 
#line 1312 "src/../include/raylib.sgh"
const 
#line 1312 "src/../include/raylib.sgh"
char *fileName);

#line 1313 "src/../include/raylib.sgh"
extern "C" 
#line 1313 "src/../include/raylib.sgh"
unsigned char *ExportImageToMemory(
#line 1313 "src/../include/raylib.sgh"

#line 1313 "src/../include/raylib.sgh"

#line 1313 "src/../include/raylib.sgh"
Image image, 
#line 1313 "src/../include/raylib.sgh"
const 
#line 1313 "src/../include/raylib.sgh"
char *fileType, 
#line 1313 "src/../include/raylib.sgh"

#line 1313 "src/../include/raylib.sgh"
int *fileSize);

#line 1314 "src/../include/raylib.sgh"
extern "C" 
#line 1314 "src/../include/raylib.sgh"
_Bool ExportImageAsCode(
#line 1314 "src/../include/raylib.sgh"

#line 1314 "src/../include/raylib.sgh"

#line 1314 "src/../include/raylib.sgh"
Image image, 
#line 1314 "src/../include/raylib.sgh"
const 
#line 1314 "src/../include/raylib.sgh"
char *fileName);

#line 1317 "src/../include/raylib.sgh"
extern "C" 
#line 1317 "src/../include/raylib.sgh"
Image GenImageColor(
#line 1317 "src/../include/raylib.sgh"

#line 1317 "src/../include/raylib.sgh"

#line 1317 "src/../include/raylib.sgh"
int width, 
#line 1317 "src/../include/raylib.sgh"

#line 1317 "src/../include/raylib.sgh"
int height, 
#line 1317 "src/../include/raylib.sgh"

#line 1317 "src/../include/raylib.sgh"
Color color);

#line 1318 "src/../include/raylib.sgh"
extern "C" 
#line 1318 "src/../include/raylib.sgh"
Image GenImageGradientLinear(
#line 1318 "src/../include/raylib.sgh"

#line 1318 "src/../include/raylib.sgh"

#line 1318 "src/../include/raylib.sgh"
int width, 
#line 1318 "src/../include/raylib.sgh"

#line 1318 "src/../include/raylib.sgh"
int height, 
#line 1318 "src/../include/raylib.sgh"

#line 1318 "src/../include/raylib.sgh"
int direction, 
#line 1318 "src/../include/raylib.sgh"

#line 1318 "src/../include/raylib.sgh"
Color start, 
#line 1318 "src/../include/raylib.sgh"

#line 1318 "src/../include/raylib.sgh"
Color end);

#line 1319 "src/../include/raylib.sgh"
extern "C" 
#line 1319 "src/../include/raylib.sgh"
Image GenImageGradientRadial(
#line 1319 "src/../include/raylib.sgh"

#line 1319 "src/../include/raylib.sgh"

#line 1319 "src/../include/raylib.sgh"
int width, 
#line 1319 "src/../include/raylib.sgh"

#line 1319 "src/../include/raylib.sgh"
int height, 
#line 1319 "src/../include/raylib.sgh"

#line 1319 "src/../include/raylib.sgh"
float density, 
#line 1319 "src/../include/raylib.sgh"

#line 1319 "src/../include/raylib.sgh"
Color inner, 
#line 1319 "src/../include/raylib.sgh"

#line 1319 "src/../include/raylib.sgh"
Color outer);

#line 1320 "src/../include/raylib.sgh"
extern "C" 
#line 1320 "src/../include/raylib.sgh"
Image GenImageGradientSquare(
#line 1320 "src/../include/raylib.sgh"

#line 1320 "src/../include/raylib.sgh"

#line 1320 "src/../include/raylib.sgh"
int width, 
#line 1320 "src/../include/raylib.sgh"

#line 1320 "src/../include/raylib.sgh"
int height, 
#line 1320 "src/../include/raylib.sgh"

#line 1320 "src/../include/raylib.sgh"
float density, 
#line 1320 "src/../include/raylib.sgh"

#line 1320 "src/../include/raylib.sgh"
Color inner, 
#line 1320 "src/../include/raylib.sgh"

#line 1320 "src/../include/raylib.sgh"
Color outer);

#line 1321 "src/../include/raylib.sgh"
extern "C" 
#line 1321 "src/../include/raylib.sgh"
Image GenImageChecked(
#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"
int width, 
#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"
int height, 
#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"
int checksX, 
#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"
int checksY, 
#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"
Color col1, 
#line 1321 "src/../include/raylib.sgh"

#line 1321 "src/../include/raylib.sgh"
Color col2);

#line 1322 "src/../include/raylib.sgh"
extern "C" 
#line 1322 "src/../include/raylib.sgh"
Image GenImageWhiteNoise(
#line 1322 "src/../include/raylib.sgh"

#line 1322 "src/../include/raylib.sgh"

#line 1322 "src/../include/raylib.sgh"
int width, 
#line 1322 "src/../include/raylib.sgh"

#line 1322 "src/../include/raylib.sgh"
int height, 
#line 1322 "src/../include/raylib.sgh"

#line 1322 "src/../include/raylib.sgh"
float factor);

#line 1323 "src/../include/raylib.sgh"
extern "C" 
#line 1323 "src/../include/raylib.sgh"
Image GenImagePerlinNoise(
#line 1323 "src/../include/raylib.sgh"

#line 1323 "src/../include/raylib.sgh"

#line 1323 "src/../include/raylib.sgh"
int width, 
#line 1323 "src/../include/raylib.sgh"

#line 1323 "src/../include/raylib.sgh"
int height, 
#line 1323 "src/../include/raylib.sgh"

#line 1323 "src/../include/raylib.sgh"
int offsetX, 
#line 1323 "src/../include/raylib.sgh"

#line 1323 "src/../include/raylib.sgh"
int offsetY, 
#line 1323 "src/../include/raylib.sgh"

#line 1323 "src/../include/raylib.sgh"
float scale);

#line 1324 "src/../include/raylib.sgh"
extern "C" 
#line 1324 "src/../include/raylib.sgh"
Image GenImageCellular(
#line 1324 "src/../include/raylib.sgh"

#line 1324 "src/../include/raylib.sgh"

#line 1324 "src/../include/raylib.sgh"
int width, 
#line 1324 "src/../include/raylib.sgh"

#line 1324 "src/../include/raylib.sgh"
int height, 
#line 1324 "src/../include/raylib.sgh"

#line 1324 "src/../include/raylib.sgh"
int tileSize);

#line 1325 "src/../include/raylib.sgh"
extern "C" 
#line 1325 "src/../include/raylib.sgh"
Image GenImageText(
#line 1325 "src/../include/raylib.sgh"

#line 1325 "src/../include/raylib.sgh"

#line 1325 "src/../include/raylib.sgh"
int width, 
#line 1325 "src/../include/raylib.sgh"

#line 1325 "src/../include/raylib.sgh"
int height, 
#line 1325 "src/../include/raylib.sgh"
const 
#line 1325 "src/../include/raylib.sgh"
char *text);

#line 1328 "src/../include/raylib.sgh"
extern "C" 
#line 1328 "src/../include/raylib.sgh"
Image ImageCopy(
#line 1328 "src/../include/raylib.sgh"

#line 1328 "src/../include/raylib.sgh"

#line 1328 "src/../include/raylib.sgh"
Image image);

#line 1329 "src/../include/raylib.sgh"
extern "C" 
#line 1329 "src/../include/raylib.sgh"
Image ImageFromImage(
#line 1329 "src/../include/raylib.sgh"

#line 1329 "src/../include/raylib.sgh"

#line 1329 "src/../include/raylib.sgh"
Image image, 
#line 1329 "src/../include/raylib.sgh"

#line 1329 "src/../include/raylib.sgh"
Rectangle rec);

#line 1330 "src/../include/raylib.sgh"
extern "C" 
#line 1330 "src/../include/raylib.sgh"
Image ImageText(
#line 1330 "src/../include/raylib.sgh"

#line 1330 "src/../include/raylib.sgh"
const 
#line 1330 "src/../include/raylib.sgh"
char *text, 
#line 1330 "src/../include/raylib.sgh"

#line 1330 "src/../include/raylib.sgh"
int fontSize, 
#line 1330 "src/../include/raylib.sgh"

#line 1330 "src/../include/raylib.sgh"
Color color);

#line 1331 "src/../include/raylib.sgh"
extern "C" 
#line 1331 "src/../include/raylib.sgh"
Image ImageTextEx(
#line 1331 "src/../include/raylib.sgh"

#line 1331 "src/../include/raylib.sgh"

#line 1331 "src/../include/raylib.sgh"
Font font, 
#line 1331 "src/../include/raylib.sgh"
const 
#line 1331 "src/../include/raylib.sgh"
char *text, 
#line 1331 "src/../include/raylib.sgh"

#line 1331 "src/../include/raylib.sgh"
float fontSize, 
#line 1331 "src/../include/raylib.sgh"

#line 1331 "src/../include/raylib.sgh"
float spacing, 
#line 1331 "src/../include/raylib.sgh"

#line 1331 "src/../include/raylib.sgh"
Color tint);

#line 1332 "src/../include/raylib.sgh"
extern "C" 
#line 1332 "src/../include/raylib.sgh"
void ImageFormat(
#line 1332 "src/../include/raylib.sgh"

#line 1332 "src/../include/raylib.sgh"

#line 1332 "src/../include/raylib.sgh"
Image *image, 
#line 1332 "src/../include/raylib.sgh"

#line 1332 "src/../include/raylib.sgh"
int newFormat);

#line 1333 "src/../include/raylib.sgh"
extern "C" 
#line 1333 "src/../include/raylib.sgh"
void ImageToPOT(
#line 1333 "src/../include/raylib.sgh"

#line 1333 "src/../include/raylib.sgh"

#line 1333 "src/../include/raylib.sgh"
Image *image, 
#line 1333 "src/../include/raylib.sgh"

#line 1333 "src/../include/raylib.sgh"
Color fill);

#line 1334 "src/../include/raylib.sgh"
extern "C" 
#line 1334 "src/../include/raylib.sgh"
void ImageCrop(
#line 1334 "src/../include/raylib.sgh"

#line 1334 "src/../include/raylib.sgh"

#line 1334 "src/../include/raylib.sgh"
Image *image, 
#line 1334 "src/../include/raylib.sgh"

#line 1334 "src/../include/raylib.sgh"
Rectangle crop);

#line 1335 "src/../include/raylib.sgh"
extern "C" 
#line 1335 "src/../include/raylib.sgh"
void ImageAlphaCrop(
#line 1335 "src/../include/raylib.sgh"

#line 1335 "src/../include/raylib.sgh"

#line 1335 "src/../include/raylib.sgh"
Image *image, 
#line 1335 "src/../include/raylib.sgh"

#line 1335 "src/../include/raylib.sgh"
float threshold);

#line 1336 "src/../include/raylib.sgh"
extern "C" 
#line 1336 "src/../include/raylib.sgh"
void ImageAlphaClear(
#line 1336 "src/../include/raylib.sgh"

#line 1336 "src/../include/raylib.sgh"

#line 1336 "src/../include/raylib.sgh"
Image *image, 
#line 1336 "src/../include/raylib.sgh"

#line 1336 "src/../include/raylib.sgh"
Color color, 
#line 1336 "src/../include/raylib.sgh"

#line 1336 "src/../include/raylib.sgh"
float threshold);

#line 1337 "src/../include/raylib.sgh"
extern "C" 
#line 1337 "src/../include/raylib.sgh"
void ImageAlphaMask(
#line 1337 "src/../include/raylib.sgh"

#line 1337 "src/../include/raylib.sgh"

#line 1337 "src/../include/raylib.sgh"
Image *image, 
#line 1337 "src/../include/raylib.sgh"

#line 1337 "src/../include/raylib.sgh"
Image alphaMask);

#line 1338 "src/../include/raylib.sgh"
extern "C" 
#line 1338 "src/../include/raylib.sgh"
void ImageAlphaPremultiply(
#line 1338 "src/../include/raylib.sgh"

#line 1338 "src/../include/raylib.sgh"

#line 1338 "src/../include/raylib.sgh"
Image *image);

#line 1339 "src/../include/raylib.sgh"
extern "C" 
#line 1339 "src/../include/raylib.sgh"
void ImageBlurGaussian(
#line 1339 "src/../include/raylib.sgh"

#line 1339 "src/../include/raylib.sgh"

#line 1339 "src/../include/raylib.sgh"
Image *image, 
#line 1339 "src/../include/raylib.sgh"

#line 1339 "src/../include/raylib.sgh"
int blurSize);

#line 1340 "src/../include/raylib.sgh"
extern "C" 
#line 1340 "src/../include/raylib.sgh"
void ImageKernelConvolution(
#line 1340 "src/../include/raylib.sgh"

#line 1340 "src/../include/raylib.sgh"

#line 1340 "src/../include/raylib.sgh"
Image *image, 
#line 1340 "src/../include/raylib.sgh"

#line 1340 "src/../include/raylib.sgh"
float *kernel, 
#line 1340 "src/../include/raylib.sgh"

#line 1340 "src/../include/raylib.sgh"
int kernelSize);

#line 1341 "src/../include/raylib.sgh"
extern "C" 
#line 1341 "src/../include/raylib.sgh"
void ImageResize(
#line 1341 "src/../include/raylib.sgh"

#line 1341 "src/../include/raylib.sgh"

#line 1341 "src/../include/raylib.sgh"
Image *image, 
#line 1341 "src/../include/raylib.sgh"

#line 1341 "src/../include/raylib.sgh"
int newWidth, 
#line 1341 "src/../include/raylib.sgh"

#line 1341 "src/../include/raylib.sgh"
int newHeight);

#line 1342 "src/../include/raylib.sgh"
extern "C" 
#line 1342 "src/../include/raylib.sgh"
void ImageResizeNN(
#line 1342 "src/../include/raylib.sgh"

#line 1342 "src/../include/raylib.sgh"

#line 1342 "src/../include/raylib.sgh"
Image *image, 
#line 1342 "src/../include/raylib.sgh"

#line 1342 "src/../include/raylib.sgh"
int newWidth, 
#line 1342 "src/../include/raylib.sgh"

#line 1342 "src/../include/raylib.sgh"
int newHeight);

#line 1343 "src/../include/raylib.sgh"
extern "C" 
#line 1343 "src/../include/raylib.sgh"
void ImageResizeCanvas(
#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"
Image *image, 
#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"
int newWidth, 
#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"
int newHeight, 
#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"
int offsetX, 
#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"
int offsetY, 
#line 1343 "src/../include/raylib.sgh"

#line 1343 "src/../include/raylib.sgh"
Color fill);

#line 1344 "src/../include/raylib.sgh"
extern "C" 
#line 1344 "src/../include/raylib.sgh"
void ImageMipmaps(
#line 1344 "src/../include/raylib.sgh"

#line 1344 "src/../include/raylib.sgh"

#line 1344 "src/../include/raylib.sgh"
Image *image);

#line 1345 "src/../include/raylib.sgh"
extern "C" 
#line 1345 "src/../include/raylib.sgh"
void ImageDither(
#line 1345 "src/../include/raylib.sgh"

#line 1345 "src/../include/raylib.sgh"

#line 1345 "src/../include/raylib.sgh"
Image *image, 
#line 1345 "src/../include/raylib.sgh"

#line 1345 "src/../include/raylib.sgh"
int rBpp, 
#line 1345 "src/../include/raylib.sgh"

#line 1345 "src/../include/raylib.sgh"
int gBpp, 
#line 1345 "src/../include/raylib.sgh"

#line 1345 "src/../include/raylib.sgh"
int bBpp, 
#line 1345 "src/../include/raylib.sgh"

#line 1345 "src/../include/raylib.sgh"
int aBpp);

#line 1346 "src/../include/raylib.sgh"
extern "C" 
#line 1346 "src/../include/raylib.sgh"
void ImageFlipVertical(
#line 1346 "src/../include/raylib.sgh"

#line 1346 "src/../include/raylib.sgh"

#line 1346 "src/../include/raylib.sgh"
Image *image);

#line 1347 "src/../include/raylib.sgh"
extern "C" 
#line 1347 "src/../include/raylib.sgh"
void ImageFlipHorizontal(
#line 1347 "src/../include/raylib.sgh"

#line 1347 "src/../include/raylib.sgh"

#line 1347 "src/../include/raylib.sgh"
Image *image);

#line 1348 "src/../include/raylib.sgh"
extern "C" 
#line 1348 "src/../include/raylib.sgh"
void ImageRotate(
#line 1348 "src/../include/raylib.sgh"

#line 1348 "src/../include/raylib.sgh"

#line 1348 "src/../include/raylib.sgh"
Image *image, 
#line 1348 "src/../include/raylib.sgh"

#line 1348 "src/../include/raylib.sgh"
int degrees);

#line 1349 "src/../include/raylib.sgh"
extern "C" 
#line 1349 "src/../include/raylib.sgh"
void ImageRotateCW(
#line 1349 "src/../include/raylib.sgh"

#line 1349 "src/../include/raylib.sgh"

#line 1349 "src/../include/raylib.sgh"
Image *image);

#line 1350 "src/../include/raylib.sgh"
extern "C" 
#line 1350 "src/../include/raylib.sgh"
void ImageRotateCCW(
#line 1350 "src/../include/raylib.sgh"

#line 1350 "src/../include/raylib.sgh"

#line 1350 "src/../include/raylib.sgh"
Image *image);

#line 1351 "src/../include/raylib.sgh"
extern "C" 
#line 1351 "src/../include/raylib.sgh"
void ImageColorTint(
#line 1351 "src/../include/raylib.sgh"

#line 1351 "src/../include/raylib.sgh"

#line 1351 "src/../include/raylib.sgh"
Image *image, 
#line 1351 "src/../include/raylib.sgh"

#line 1351 "src/../include/raylib.sgh"
Color color);

#line 1352 "src/../include/raylib.sgh"
extern "C" 
#line 1352 "src/../include/raylib.sgh"
void ImageColorInvert(
#line 1352 "src/../include/raylib.sgh"

#line 1352 "src/../include/raylib.sgh"

#line 1352 "src/../include/raylib.sgh"
Image *image);

#line 1353 "src/../include/raylib.sgh"
extern "C" 
#line 1353 "src/../include/raylib.sgh"
void ImageColorGrayscale(
#line 1353 "src/../include/raylib.sgh"

#line 1353 "src/../include/raylib.sgh"

#line 1353 "src/../include/raylib.sgh"
Image *image);

#line 1354 "src/../include/raylib.sgh"
extern "C" 
#line 1354 "src/../include/raylib.sgh"
void ImageColorContrast(
#line 1354 "src/../include/raylib.sgh"

#line 1354 "src/../include/raylib.sgh"

#line 1354 "src/../include/raylib.sgh"
Image *image, 
#line 1354 "src/../include/raylib.sgh"

#line 1354 "src/../include/raylib.sgh"
float contrast);

#line 1355 "src/../include/raylib.sgh"
extern "C" 
#line 1355 "src/../include/raylib.sgh"
void ImageColorBrightness(
#line 1355 "src/../include/raylib.sgh"

#line 1355 "src/../include/raylib.sgh"

#line 1355 "src/../include/raylib.sgh"
Image *image, 
#line 1355 "src/../include/raylib.sgh"

#line 1355 "src/../include/raylib.sgh"
int brightness);

#line 1356 "src/../include/raylib.sgh"
extern "C" 
#line 1356 "src/../include/raylib.sgh"
void ImageColorReplace(
#line 1356 "src/../include/raylib.sgh"

#line 1356 "src/../include/raylib.sgh"

#line 1356 "src/../include/raylib.sgh"
Image *image, 
#line 1356 "src/../include/raylib.sgh"

#line 1356 "src/../include/raylib.sgh"
Color color, 
#line 1356 "src/../include/raylib.sgh"

#line 1356 "src/../include/raylib.sgh"
Color replace);

#line 1357 "src/../include/raylib.sgh"
extern "C" 
#line 1357 "src/../include/raylib.sgh"
Color *LoadImageColors(
#line 1357 "src/../include/raylib.sgh"

#line 1357 "src/../include/raylib.sgh"

#line 1357 "src/../include/raylib.sgh"
Image image);

#line 1358 "src/../include/raylib.sgh"
extern "C" 
#line 1358 "src/../include/raylib.sgh"
Color *LoadImagePalette(
#line 1358 "src/../include/raylib.sgh"

#line 1358 "src/../include/raylib.sgh"

#line 1358 "src/../include/raylib.sgh"
Image image, 
#line 1358 "src/../include/raylib.sgh"

#line 1358 "src/../include/raylib.sgh"
int maxPaletteSize, 
#line 1358 "src/../include/raylib.sgh"

#line 1358 "src/../include/raylib.sgh"
int *colorCount);

#line 1359 "src/../include/raylib.sgh"
extern "C" 
#line 1359 "src/../include/raylib.sgh"
void UnloadImageColors(
#line 1359 "src/../include/raylib.sgh"

#line 1359 "src/../include/raylib.sgh"

#line 1359 "src/../include/raylib.sgh"
Color *colors);

#line 1360 "src/../include/raylib.sgh"
extern "C" 
#line 1360 "src/../include/raylib.sgh"
void UnloadImagePalette(
#line 1360 "src/../include/raylib.sgh"

#line 1360 "src/../include/raylib.sgh"

#line 1360 "src/../include/raylib.sgh"
Color *colors);

#line 1361 "src/../include/raylib.sgh"
extern "C" 
#line 1361 "src/../include/raylib.sgh"
Rectangle GetImageAlphaBorder(
#line 1361 "src/../include/raylib.sgh"

#line 1361 "src/../include/raylib.sgh"

#line 1361 "src/../include/raylib.sgh"
Image image, 
#line 1361 "src/../include/raylib.sgh"

#line 1361 "src/../include/raylib.sgh"
float threshold);

#line 1362 "src/../include/raylib.sgh"
extern "C" 
#line 1362 "src/../include/raylib.sgh"
Color GetImageColor(
#line 1362 "src/../include/raylib.sgh"

#line 1362 "src/../include/raylib.sgh"

#line 1362 "src/../include/raylib.sgh"
Image image, 
#line 1362 "src/../include/raylib.sgh"

#line 1362 "src/../include/raylib.sgh"
int x, 
#line 1362 "src/../include/raylib.sgh"

#line 1362 "src/../include/raylib.sgh"
int y);

#line 1366 "src/../include/raylib.sgh"
extern "C" 
#line 1366 "src/../include/raylib.sgh"
void ImageClearBackground(
#line 1366 "src/../include/raylib.sgh"

#line 1366 "src/../include/raylib.sgh"

#line 1366 "src/../include/raylib.sgh"
Image *dst, 
#line 1366 "src/../include/raylib.sgh"

#line 1366 "src/../include/raylib.sgh"
Color color);

#line 1367 "src/../include/raylib.sgh"
extern "C" 
#line 1367 "src/../include/raylib.sgh"
void ImageDrawPixel(
#line 1367 "src/../include/raylib.sgh"

#line 1367 "src/../include/raylib.sgh"

#line 1367 "src/../include/raylib.sgh"
Image *dst, 
#line 1367 "src/../include/raylib.sgh"

#line 1367 "src/../include/raylib.sgh"
int posX, 
#line 1367 "src/../include/raylib.sgh"

#line 1367 "src/../include/raylib.sgh"
int posY, 
#line 1367 "src/../include/raylib.sgh"

#line 1367 "src/../include/raylib.sgh"
Color color);

#line 1368 "src/../include/raylib.sgh"
extern "C" 
#line 1368 "src/../include/raylib.sgh"
void ImageDrawPixelV(
#line 1368 "src/../include/raylib.sgh"

#line 1368 "src/../include/raylib.sgh"

#line 1368 "src/../include/raylib.sgh"
Image *dst, 
#line 1368 "src/../include/raylib.sgh"

#line 1368 "src/../include/raylib.sgh"
Vector2 position, 
#line 1368 "src/../include/raylib.sgh"

#line 1368 "src/../include/raylib.sgh"
Color color);

#line 1369 "src/../include/raylib.sgh"
extern "C" 
#line 1369 "src/../include/raylib.sgh"
void ImageDrawLine(
#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"
Image *dst, 
#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"
int startPosX, 
#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"
int startPosY, 
#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"
int endPosX, 
#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"
int endPosY, 
#line 1369 "src/../include/raylib.sgh"

#line 1369 "src/../include/raylib.sgh"
Color color);

#line 1370 "src/../include/raylib.sgh"
extern "C" 
#line 1370 "src/../include/raylib.sgh"
void ImageDrawLineV(
#line 1370 "src/../include/raylib.sgh"

#line 1370 "src/../include/raylib.sgh"

#line 1370 "src/../include/raylib.sgh"
Image *dst, 
#line 1370 "src/../include/raylib.sgh"

#line 1370 "src/../include/raylib.sgh"
Vector2 start, 
#line 1370 "src/../include/raylib.sgh"

#line 1370 "src/../include/raylib.sgh"
Vector2 end, 
#line 1370 "src/../include/raylib.sgh"

#line 1370 "src/../include/raylib.sgh"
Color color);

#line 1371 "src/../include/raylib.sgh"
extern "C" 
#line 1371 "src/../include/raylib.sgh"
void ImageDrawCircle(
#line 1371 "src/../include/raylib.sgh"

#line 1371 "src/../include/raylib.sgh"

#line 1371 "src/../include/raylib.sgh"
Image *dst, 
#line 1371 "src/../include/raylib.sgh"

#line 1371 "src/../include/raylib.sgh"
int centerX, 
#line 1371 "src/../include/raylib.sgh"

#line 1371 "src/../include/raylib.sgh"
int centerY, 
#line 1371 "src/../include/raylib.sgh"

#line 1371 "src/../include/raylib.sgh"
int radius, 
#line 1371 "src/../include/raylib.sgh"

#line 1371 "src/../include/raylib.sgh"
Color color);

#line 1372 "src/../include/raylib.sgh"
extern "C" 
#line 1372 "src/../include/raylib.sgh"
void ImageDrawCircleV(
#line 1372 "src/../include/raylib.sgh"

#line 1372 "src/../include/raylib.sgh"

#line 1372 "src/../include/raylib.sgh"
Image *dst, 
#line 1372 "src/../include/raylib.sgh"

#line 1372 "src/../include/raylib.sgh"
Vector2 center, 
#line 1372 "src/../include/raylib.sgh"

#line 1372 "src/../include/raylib.sgh"
int radius, 
#line 1372 "src/../include/raylib.sgh"

#line 1372 "src/../include/raylib.sgh"
Color color);

#line 1373 "src/../include/raylib.sgh"
extern "C" 
#line 1373 "src/../include/raylib.sgh"
void ImageDrawCircleLines(
#line 1373 "src/../include/raylib.sgh"

#line 1373 "src/../include/raylib.sgh"

#line 1373 "src/../include/raylib.sgh"
Image *dst, 
#line 1373 "src/../include/raylib.sgh"

#line 1373 "src/../include/raylib.sgh"
int centerX, 
#line 1373 "src/../include/raylib.sgh"

#line 1373 "src/../include/raylib.sgh"
int centerY, 
#line 1373 "src/../include/raylib.sgh"

#line 1373 "src/../include/raylib.sgh"
int radius, 
#line 1373 "src/../include/raylib.sgh"

#line 1373 "src/../include/raylib.sgh"
Color color);

#line 1374 "src/../include/raylib.sgh"
extern "C" 
#line 1374 "src/../include/raylib.sgh"
void ImageDrawCircleLinesV(
#line 1374 "src/../include/raylib.sgh"

#line 1374 "src/../include/raylib.sgh"

#line 1374 "src/../include/raylib.sgh"
Image *dst, 
#line 1374 "src/../include/raylib.sgh"

#line 1374 "src/../include/raylib.sgh"
Vector2 center, 
#line 1374 "src/../include/raylib.sgh"

#line 1374 "src/../include/raylib.sgh"
int radius, 
#line 1374 "src/../include/raylib.sgh"

#line 1374 "src/../include/raylib.sgh"
Color color);

#line 1375 "src/../include/raylib.sgh"
extern "C" 
#line 1375 "src/../include/raylib.sgh"
void ImageDrawRectangle(
#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"
Image *dst, 
#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"
int posX, 
#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"
int posY, 
#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"
int width, 
#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"
int height, 
#line 1375 "src/../include/raylib.sgh"

#line 1375 "src/../include/raylib.sgh"
Color color);

#line 1376 "src/../include/raylib.sgh"
extern "C" 
#line 1376 "src/../include/raylib.sgh"
void ImageDrawRectangleV(
#line 1376 "src/../include/raylib.sgh"

#line 1376 "src/../include/raylib.sgh"

#line 1376 "src/../include/raylib.sgh"
Image *dst, 
#line 1376 "src/../include/raylib.sgh"

#line 1376 "src/../include/raylib.sgh"
Vector2 position, 
#line 1376 "src/../include/raylib.sgh"

#line 1376 "src/../include/raylib.sgh"
Vector2 size, 
#line 1376 "src/../include/raylib.sgh"

#line 1376 "src/../include/raylib.sgh"
Color color);

#line 1377 "src/../include/raylib.sgh"
extern "C" 
#line 1377 "src/../include/raylib.sgh"
void ImageDrawRectangleRec(
#line 1377 "src/../include/raylib.sgh"

#line 1377 "src/../include/raylib.sgh"

#line 1377 "src/../include/raylib.sgh"
Image *dst, 
#line 1377 "src/../include/raylib.sgh"

#line 1377 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1377 "src/../include/raylib.sgh"

#line 1377 "src/../include/raylib.sgh"
Color color);

#line 1378 "src/../include/raylib.sgh"
extern "C" 
#line 1378 "src/../include/raylib.sgh"
void ImageDrawRectangleLines(
#line 1378 "src/../include/raylib.sgh"

#line 1378 "src/../include/raylib.sgh"

#line 1378 "src/../include/raylib.sgh"
Image *dst, 
#line 1378 "src/../include/raylib.sgh"

#line 1378 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1378 "src/../include/raylib.sgh"

#line 1378 "src/../include/raylib.sgh"
int thick, 
#line 1378 "src/../include/raylib.sgh"

#line 1378 "src/../include/raylib.sgh"
Color color);

#line 1379 "src/../include/raylib.sgh"
extern "C" 
#line 1379 "src/../include/raylib.sgh"
void ImageDraw(
#line 1379 "src/../include/raylib.sgh"

#line 1379 "src/../include/raylib.sgh"

#line 1379 "src/../include/raylib.sgh"
Image *dst, 
#line 1379 "src/../include/raylib.sgh"

#line 1379 "src/../include/raylib.sgh"
Image src, 
#line 1379 "src/../include/raylib.sgh"

#line 1379 "src/../include/raylib.sgh"
Rectangle srcRec, 
#line 1379 "src/../include/raylib.sgh"

#line 1379 "src/../include/raylib.sgh"
Rectangle dstRec, 
#line 1379 "src/../include/raylib.sgh"

#line 1379 "src/../include/raylib.sgh"
Color tint);

#line 1380 "src/../include/raylib.sgh"
extern "C" 
#line 1380 "src/../include/raylib.sgh"
void ImageDrawText(
#line 1380 "src/../include/raylib.sgh"

#line 1380 "src/../include/raylib.sgh"

#line 1380 "src/../include/raylib.sgh"
Image *dst, 
#line 1380 "src/../include/raylib.sgh"
const 
#line 1380 "src/../include/raylib.sgh"
char *text, 
#line 1380 "src/../include/raylib.sgh"

#line 1380 "src/../include/raylib.sgh"
int posX, 
#line 1380 "src/../include/raylib.sgh"

#line 1380 "src/../include/raylib.sgh"
int posY, 
#line 1380 "src/../include/raylib.sgh"

#line 1380 "src/../include/raylib.sgh"
int fontSize, 
#line 1380 "src/../include/raylib.sgh"

#line 1380 "src/../include/raylib.sgh"
Color color);

#line 1381 "src/../include/raylib.sgh"
extern "C" 
#line 1381 "src/../include/raylib.sgh"
void ImageDrawTextEx(
#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"
Image *dst, 
#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"
Font font, 
#line 1381 "src/../include/raylib.sgh"
const 
#line 1381 "src/../include/raylib.sgh"
char *text, 
#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"
Vector2 position, 
#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"
float fontSize, 
#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"
float spacing, 
#line 1381 "src/../include/raylib.sgh"

#line 1381 "src/../include/raylib.sgh"
Color tint);

#line 1385 "src/../include/raylib.sgh"
extern "C" 
#line 1385 "src/../include/raylib.sgh"
Texture2D LoadTexture(
#line 1385 "src/../include/raylib.sgh"

#line 1385 "src/../include/raylib.sgh"
const 
#line 1385 "src/../include/raylib.sgh"
char *fileName);

#line 1386 "src/../include/raylib.sgh"
extern "C" 
#line 1386 "src/../include/raylib.sgh"
Texture2D LoadTextureFromImage(
#line 1386 "src/../include/raylib.sgh"

#line 1386 "src/../include/raylib.sgh"

#line 1386 "src/../include/raylib.sgh"
Image image);

#line 1387 "src/../include/raylib.sgh"
extern "C" 
#line 1387 "src/../include/raylib.sgh"
TextureCubemap LoadTextureCubemap(
#line 1387 "src/../include/raylib.sgh"

#line 1387 "src/../include/raylib.sgh"

#line 1387 "src/../include/raylib.sgh"
Image image, 
#line 1387 "src/../include/raylib.sgh"

#line 1387 "src/../include/raylib.sgh"
int layout);

#line 1388 "src/../include/raylib.sgh"
extern "C" 
#line 1388 "src/../include/raylib.sgh"
RenderTexture2D LoadRenderTexture(
#line 1388 "src/../include/raylib.sgh"

#line 1388 "src/../include/raylib.sgh"

#line 1388 "src/../include/raylib.sgh"
int width, 
#line 1388 "src/../include/raylib.sgh"

#line 1388 "src/../include/raylib.sgh"
int height);

#line 1389 "src/../include/raylib.sgh"
extern "C" 
#line 1389 "src/../include/raylib.sgh"
_Bool IsTextureReady(
#line 1389 "src/../include/raylib.sgh"

#line 1389 "src/../include/raylib.sgh"

#line 1389 "src/../include/raylib.sgh"
Texture2D texture);

#line 1390 "src/../include/raylib.sgh"
extern "C" 
#line 1390 "src/../include/raylib.sgh"
void UnloadTexture(
#line 1390 "src/../include/raylib.sgh"

#line 1390 "src/../include/raylib.sgh"

#line 1390 "src/../include/raylib.sgh"
Texture2D texture);

#line 1391 "src/../include/raylib.sgh"
extern "C" 
#line 1391 "src/../include/raylib.sgh"
_Bool IsRenderTextureReady(
#line 1391 "src/../include/raylib.sgh"

#line 1391 "src/../include/raylib.sgh"

#line 1391 "src/../include/raylib.sgh"
RenderTexture2D target);

#line 1392 "src/../include/raylib.sgh"
extern "C" 
#line 1392 "src/../include/raylib.sgh"
void UnloadRenderTexture(
#line 1392 "src/../include/raylib.sgh"

#line 1392 "src/../include/raylib.sgh"

#line 1392 "src/../include/raylib.sgh"
RenderTexture2D target);

#line 1393 "src/../include/raylib.sgh"
extern "C" 
#line 1393 "src/../include/raylib.sgh"
void UpdateTexture(
#line 1393 "src/../include/raylib.sgh"

#line 1393 "src/../include/raylib.sgh"

#line 1393 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1393 "src/../include/raylib.sgh"
const 
#line 1393 "src/../include/raylib.sgh"
void *pixels);

#line 1394 "src/../include/raylib.sgh"
extern "C" 
#line 1394 "src/../include/raylib.sgh"
void UpdateTextureRec(
#line 1394 "src/../include/raylib.sgh"

#line 1394 "src/../include/raylib.sgh"

#line 1394 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1394 "src/../include/raylib.sgh"

#line 1394 "src/../include/raylib.sgh"
Rectangle rec, 
#line 1394 "src/../include/raylib.sgh"
const 
#line 1394 "src/../include/raylib.sgh"
void *pixels);

#line 1397 "src/../include/raylib.sgh"
extern "C" 
#line 1397 "src/../include/raylib.sgh"
void GenTextureMipmaps(
#line 1397 "src/../include/raylib.sgh"

#line 1397 "src/../include/raylib.sgh"

#line 1397 "src/../include/raylib.sgh"
Texture2D *texture);

#line 1398 "src/../include/raylib.sgh"
extern "C" 
#line 1398 "src/../include/raylib.sgh"
void SetTextureFilter(
#line 1398 "src/../include/raylib.sgh"

#line 1398 "src/../include/raylib.sgh"

#line 1398 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1398 "src/../include/raylib.sgh"

#line 1398 "src/../include/raylib.sgh"
int filter);

#line 1399 "src/../include/raylib.sgh"
extern "C" 
#line 1399 "src/../include/raylib.sgh"
void SetTextureWrap(
#line 1399 "src/../include/raylib.sgh"

#line 1399 "src/../include/raylib.sgh"

#line 1399 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1399 "src/../include/raylib.sgh"

#line 1399 "src/../include/raylib.sgh"
int wrap);

#line 1402 "src/../include/raylib.sgh"
extern "C" 
#line 1402 "src/../include/raylib.sgh"
void DrawTexture(
#line 1402 "src/../include/raylib.sgh"

#line 1402 "src/../include/raylib.sgh"

#line 1402 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1402 "src/../include/raylib.sgh"

#line 1402 "src/../include/raylib.sgh"
int posX, 
#line 1402 "src/../include/raylib.sgh"

#line 1402 "src/../include/raylib.sgh"
int posY, 
#line 1402 "src/../include/raylib.sgh"

#line 1402 "src/../include/raylib.sgh"
Color tint);

#line 1403 "src/../include/raylib.sgh"
extern "C" 
#line 1403 "src/../include/raylib.sgh"
void DrawTextureV(
#line 1403 "src/../include/raylib.sgh"

#line 1403 "src/../include/raylib.sgh"

#line 1403 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1403 "src/../include/raylib.sgh"

#line 1403 "src/../include/raylib.sgh"
Vector2 position, 
#line 1403 "src/../include/raylib.sgh"

#line 1403 "src/../include/raylib.sgh"
Color tint);

#line 1404 "src/../include/raylib.sgh"
extern "C" 
#line 1404 "src/../include/raylib.sgh"
void DrawTextureEx(
#line 1404 "src/../include/raylib.sgh"

#line 1404 "src/../include/raylib.sgh"

#line 1404 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1404 "src/../include/raylib.sgh"

#line 1404 "src/../include/raylib.sgh"
Vector2 position, 
#line 1404 "src/../include/raylib.sgh"

#line 1404 "src/../include/raylib.sgh"
float rotation, 
#line 1404 "src/../include/raylib.sgh"

#line 1404 "src/../include/raylib.sgh"
float scale, 
#line 1404 "src/../include/raylib.sgh"

#line 1404 "src/../include/raylib.sgh"
Color tint);

#line 1405 "src/../include/raylib.sgh"
extern "C" 
#line 1405 "src/../include/raylib.sgh"
void DrawTextureRec(
#line 1405 "src/../include/raylib.sgh"

#line 1405 "src/../include/raylib.sgh"

#line 1405 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1405 "src/../include/raylib.sgh"

#line 1405 "src/../include/raylib.sgh"
Rectangle source, 
#line 1405 "src/../include/raylib.sgh"

#line 1405 "src/../include/raylib.sgh"
Vector2 position, 
#line 1405 "src/../include/raylib.sgh"

#line 1405 "src/../include/raylib.sgh"
Color tint);

#line 1406 "src/../include/raylib.sgh"
extern "C" 
#line 1406 "src/../include/raylib.sgh"
void DrawTexturePro(
#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"
Rectangle source, 
#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"
Rectangle dest, 
#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"
Vector2 origin, 
#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"
float rotation, 
#line 1406 "src/../include/raylib.sgh"

#line 1406 "src/../include/raylib.sgh"
Color tint);

#line 1407 "src/../include/raylib.sgh"
extern "C" 
#line 1407 "src/../include/raylib.sgh"
void DrawTextureNPatch(
#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"
NPatchInfo nPatchInfo, 
#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"
Rectangle dest, 
#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"
Vector2 origin, 
#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"
float rotation, 
#line 1407 "src/../include/raylib.sgh"

#line 1407 "src/../include/raylib.sgh"
Color tint);

#line 1410 "src/../include/raylib.sgh"
extern "C" 
#line 1410 "src/../include/raylib.sgh"
Color Fade(
#line 1410 "src/../include/raylib.sgh"

#line 1410 "src/../include/raylib.sgh"

#line 1410 "src/../include/raylib.sgh"
Color color, 
#line 1410 "src/../include/raylib.sgh"

#line 1410 "src/../include/raylib.sgh"
float alpha);

#line 1411 "src/../include/raylib.sgh"
extern "C" 
#line 1411 "src/../include/raylib.sgh"
int ColorToInt(
#line 1411 "src/../include/raylib.sgh"

#line 1411 "src/../include/raylib.sgh"

#line 1411 "src/../include/raylib.sgh"
Color color);

#line 1412 "src/../include/raylib.sgh"
extern "C" 
#line 1412 "src/../include/raylib.sgh"
Vector4 ColorNormalize(
#line 1412 "src/../include/raylib.sgh"

#line 1412 "src/../include/raylib.sgh"

#line 1412 "src/../include/raylib.sgh"
Color color);

#line 1413 "src/../include/raylib.sgh"
extern "C" 
#line 1413 "src/../include/raylib.sgh"
Color ColorFromNormalized(
#line 1413 "src/../include/raylib.sgh"

#line 1413 "src/../include/raylib.sgh"

#line 1413 "src/../include/raylib.sgh"
Vector4 normalized);

#line 1414 "src/../include/raylib.sgh"
extern "C" 
#line 1414 "src/../include/raylib.sgh"
Vector3 ColorToHSV(
#line 1414 "src/../include/raylib.sgh"

#line 1414 "src/../include/raylib.sgh"

#line 1414 "src/../include/raylib.sgh"
Color color);

#line 1415 "src/../include/raylib.sgh"
extern "C" 
#line 1415 "src/../include/raylib.sgh"
Color ColorFromHSV(
#line 1415 "src/../include/raylib.sgh"

#line 1415 "src/../include/raylib.sgh"

#line 1415 "src/../include/raylib.sgh"
float hue, 
#line 1415 "src/../include/raylib.sgh"

#line 1415 "src/../include/raylib.sgh"
float saturation, 
#line 1415 "src/../include/raylib.sgh"

#line 1415 "src/../include/raylib.sgh"
float value);

#line 1416 "src/../include/raylib.sgh"
extern "C" 
#line 1416 "src/../include/raylib.sgh"
Color ColorTint(
#line 1416 "src/../include/raylib.sgh"

#line 1416 "src/../include/raylib.sgh"

#line 1416 "src/../include/raylib.sgh"
Color color, 
#line 1416 "src/../include/raylib.sgh"

#line 1416 "src/../include/raylib.sgh"
Color tint);

#line 1417 "src/../include/raylib.sgh"
extern "C" 
#line 1417 "src/../include/raylib.sgh"
Color ColorBrightness(
#line 1417 "src/../include/raylib.sgh"

#line 1417 "src/../include/raylib.sgh"

#line 1417 "src/../include/raylib.sgh"
Color color, 
#line 1417 "src/../include/raylib.sgh"

#line 1417 "src/../include/raylib.sgh"
float factor);

#line 1418 "src/../include/raylib.sgh"
extern "C" 
#line 1418 "src/../include/raylib.sgh"
Color ColorContrast(
#line 1418 "src/../include/raylib.sgh"

#line 1418 "src/../include/raylib.sgh"

#line 1418 "src/../include/raylib.sgh"
Color color, 
#line 1418 "src/../include/raylib.sgh"

#line 1418 "src/../include/raylib.sgh"
float contrast);

#line 1419 "src/../include/raylib.sgh"
extern "C" 
#line 1419 "src/../include/raylib.sgh"
Color ColorAlpha(
#line 1419 "src/../include/raylib.sgh"

#line 1419 "src/../include/raylib.sgh"

#line 1419 "src/../include/raylib.sgh"
Color color, 
#line 1419 "src/../include/raylib.sgh"

#line 1419 "src/../include/raylib.sgh"
float alpha);

#line 1420 "src/../include/raylib.sgh"
extern "C" 
#line 1420 "src/../include/raylib.sgh"
Color ColorAlphaBlend(
#line 1420 "src/../include/raylib.sgh"

#line 1420 "src/../include/raylib.sgh"

#line 1420 "src/../include/raylib.sgh"
Color dst, 
#line 1420 "src/../include/raylib.sgh"

#line 1420 "src/../include/raylib.sgh"
Color src, 
#line 1420 "src/../include/raylib.sgh"

#line 1420 "src/../include/raylib.sgh"
Color tint);

#line 1421 "src/../include/raylib.sgh"
extern "C" 
#line 1421 "src/../include/raylib.sgh"
Color GetColor(
#line 1421 "src/../include/raylib.sgh"

#line 1421 "src/../include/raylib.sgh"

#line 1421 "src/../include/raylib.sgh"
unsigned int hexValue);

#line 1422 "src/../include/raylib.sgh"
extern "C" 
#line 1422 "src/../include/raylib.sgh"
Color GetPixelColor(
#line 1422 "src/../include/raylib.sgh"

#line 1422 "src/../include/raylib.sgh"

#line 1422 "src/../include/raylib.sgh"
void *srcPtr, 
#line 1422 "src/../include/raylib.sgh"

#line 1422 "src/../include/raylib.sgh"
int format);

#line 1423 "src/../include/raylib.sgh"
extern "C" 
#line 1423 "src/../include/raylib.sgh"
void SetPixelColor(
#line 1423 "src/../include/raylib.sgh"

#line 1423 "src/../include/raylib.sgh"

#line 1423 "src/../include/raylib.sgh"
void *dstPtr, 
#line 1423 "src/../include/raylib.sgh"

#line 1423 "src/../include/raylib.sgh"
Color color, 
#line 1423 "src/../include/raylib.sgh"

#line 1423 "src/../include/raylib.sgh"
int format);

#line 1424 "src/../include/raylib.sgh"
extern "C" 
#line 1424 "src/../include/raylib.sgh"
int GetPixelDataSize(
#line 1424 "src/../include/raylib.sgh"

#line 1424 "src/../include/raylib.sgh"

#line 1424 "src/../include/raylib.sgh"
int width, 
#line 1424 "src/../include/raylib.sgh"

#line 1424 "src/../include/raylib.sgh"
int height, 
#line 1424 "src/../include/raylib.sgh"

#line 1424 "src/../include/raylib.sgh"
int format);

#line 1431 "src/../include/raylib.sgh"
extern "C" 
#line 1431 "src/../include/raylib.sgh"
Font GetFontDefault(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1431 "src/../include/raylib.sgh"
void);

#line 1432 "src/../include/raylib.sgh"
extern "C" 
#line 1432 "src/../include/raylib.sgh"
Font LoadFont(
#line 1432 "src/../include/raylib.sgh"

#line 1432 "src/../include/raylib.sgh"
const 
#line 1432 "src/../include/raylib.sgh"
char *fileName);

#line 1433 "src/../include/raylib.sgh"
extern "C" 
#line 1433 "src/../include/raylib.sgh"
Font LoadFontEx(
#line 1433 "src/../include/raylib.sgh"

#line 1433 "src/../include/raylib.sgh"
const 
#line 1433 "src/../include/raylib.sgh"
char *fileName, 
#line 1433 "src/../include/raylib.sgh"

#line 1433 "src/../include/raylib.sgh"
int fontSize, 
#line 1433 "src/../include/raylib.sgh"

#line 1433 "src/../include/raylib.sgh"
int *codepoints, 
#line 1433 "src/../include/raylib.sgh"

#line 1433 "src/../include/raylib.sgh"
int codepointCount);

#line 1434 "src/../include/raylib.sgh"
extern "C" 
#line 1434 "src/../include/raylib.sgh"
Font LoadFontFromImage(
#line 1434 "src/../include/raylib.sgh"

#line 1434 "src/../include/raylib.sgh"

#line 1434 "src/../include/raylib.sgh"
Image image, 
#line 1434 "src/../include/raylib.sgh"

#line 1434 "src/../include/raylib.sgh"
Color key, 
#line 1434 "src/../include/raylib.sgh"

#line 1434 "src/../include/raylib.sgh"
int firstChar);

#line 1435 "src/../include/raylib.sgh"
extern "C" 
#line 1435 "src/../include/raylib.sgh"
Font LoadFontFromMemory(
#line 1435 "src/../include/raylib.sgh"

#line 1435 "src/../include/raylib.sgh"
const 
#line 1435 "src/../include/raylib.sgh"
char *fileType, 
#line 1435 "src/../include/raylib.sgh"
const 
#line 1435 "src/../include/raylib.sgh"
unsigned char *fileData, 
#line 1435 "src/../include/raylib.sgh"

#line 1435 "src/../include/raylib.sgh"
int dataSize, 
#line 1435 "src/../include/raylib.sgh"

#line 1435 "src/../include/raylib.sgh"
int fontSize, 
#line 1435 "src/../include/raylib.sgh"

#line 1435 "src/../include/raylib.sgh"
int *codepoints, 
#line 1435 "src/../include/raylib.sgh"

#line 1435 "src/../include/raylib.sgh"
int codepointCount);

#line 1436 "src/../include/raylib.sgh"
extern "C" 
#line 1436 "src/../include/raylib.sgh"
_Bool IsFontReady(
#line 1436 "src/../include/raylib.sgh"

#line 1436 "src/../include/raylib.sgh"

#line 1436 "src/../include/raylib.sgh"
Font font);

#line 1437 "src/../include/raylib.sgh"
extern "C" 
#line 1437 "src/../include/raylib.sgh"
GlyphInfo *LoadFontData(
#line 1437 "src/../include/raylib.sgh"

#line 1437 "src/../include/raylib.sgh"
const 
#line 1437 "src/../include/raylib.sgh"
unsigned char *fileData, 
#line 1437 "src/../include/raylib.sgh"

#line 1437 "src/../include/raylib.sgh"
int dataSize, 
#line 1437 "src/../include/raylib.sgh"

#line 1437 "src/../include/raylib.sgh"
int fontSize, 
#line 1437 "src/../include/raylib.sgh"

#line 1437 "src/../include/raylib.sgh"
int *codepoints, 
#line 1437 "src/../include/raylib.sgh"

#line 1437 "src/../include/raylib.sgh"
int codepointCount, 
#line 1437 "src/../include/raylib.sgh"

#line 1437 "src/../include/raylib.sgh"
int type);

#line 1438 "src/../include/raylib.sgh"
extern "C" 
#line 1438 "src/../include/raylib.sgh"
Image GenImageFontAtlas(
#line 1438 "src/../include/raylib.sgh"

#line 1438 "src/../include/raylib.sgh"
const 
#line 1438 "src/../include/raylib.sgh"
GlyphInfo *glyphs, 
#line 1438 "src/../include/raylib.sgh"

#line 1438 "src/../include/raylib.sgh"
Rectangle **glyphRecs, 
#line 1438 "src/../include/raylib.sgh"

#line 1438 "src/../include/raylib.sgh"
int glyphCount, 
#line 1438 "src/../include/raylib.sgh"

#line 1438 "src/../include/raylib.sgh"
int fontSize, 
#line 1438 "src/../include/raylib.sgh"

#line 1438 "src/../include/raylib.sgh"
int padding, 
#line 1438 "src/../include/raylib.sgh"

#line 1438 "src/../include/raylib.sgh"
int packMethod);

#line 1439 "src/../include/raylib.sgh"
extern "C" 
#line 1439 "src/../include/raylib.sgh"
void UnloadFontData(
#line 1439 "src/../include/raylib.sgh"

#line 1439 "src/../include/raylib.sgh"

#line 1439 "src/../include/raylib.sgh"
GlyphInfo *glyphs, 
#line 1439 "src/../include/raylib.sgh"

#line 1439 "src/../include/raylib.sgh"
int glyphCount);

#line 1440 "src/../include/raylib.sgh"
extern "C" 
#line 1440 "src/../include/raylib.sgh"
void UnloadFont(
#line 1440 "src/../include/raylib.sgh"

#line 1440 "src/../include/raylib.sgh"

#line 1440 "src/../include/raylib.sgh"
Font font);

#line 1441 "src/../include/raylib.sgh"
extern "C" 
#line 1441 "src/../include/raylib.sgh"
_Bool ExportFontAsCode(
#line 1441 "src/../include/raylib.sgh"

#line 1441 "src/../include/raylib.sgh"

#line 1441 "src/../include/raylib.sgh"
Font font, 
#line 1441 "src/../include/raylib.sgh"
const 
#line 1441 "src/../include/raylib.sgh"
char *fileName);

#line 1444 "src/../include/raylib.sgh"
extern "C" 
#line 1444 "src/../include/raylib.sgh"
void DrawFPS(
#line 1444 "src/../include/raylib.sgh"

#line 1444 "src/../include/raylib.sgh"

#line 1444 "src/../include/raylib.sgh"
int posX, 
#line 1444 "src/../include/raylib.sgh"

#line 1444 "src/../include/raylib.sgh"
int posY);

#line 1445 "src/../include/raylib.sgh"
extern "C" 
#line 1445 "src/../include/raylib.sgh"
void DrawText(
#line 1445 "src/../include/raylib.sgh"

#line 1445 "src/../include/raylib.sgh"
const 
#line 1445 "src/../include/raylib.sgh"
char *text, 
#line 1445 "src/../include/raylib.sgh"

#line 1445 "src/../include/raylib.sgh"
int posX, 
#line 1445 "src/../include/raylib.sgh"

#line 1445 "src/../include/raylib.sgh"
int posY, 
#line 1445 "src/../include/raylib.sgh"

#line 1445 "src/../include/raylib.sgh"
int fontSize, 
#line 1445 "src/../include/raylib.sgh"

#line 1445 "src/../include/raylib.sgh"
Color color);

#line 1446 "src/../include/raylib.sgh"
extern "C" 
#line 1446 "src/../include/raylib.sgh"
void DrawTextEx(
#line 1446 "src/../include/raylib.sgh"

#line 1446 "src/../include/raylib.sgh"

#line 1446 "src/../include/raylib.sgh"
Font font, 
#line 1446 "src/../include/raylib.sgh"
const 
#line 1446 "src/../include/raylib.sgh"
char *text, 
#line 1446 "src/../include/raylib.sgh"

#line 1446 "src/../include/raylib.sgh"
Vector2 position, 
#line 1446 "src/../include/raylib.sgh"

#line 1446 "src/../include/raylib.sgh"
float fontSize, 
#line 1446 "src/../include/raylib.sgh"

#line 1446 "src/../include/raylib.sgh"
float spacing, 
#line 1446 "src/../include/raylib.sgh"

#line 1446 "src/../include/raylib.sgh"
Color tint);

#line 1447 "src/../include/raylib.sgh"
extern "C" 
#line 1447 "src/../include/raylib.sgh"
void DrawTextPro(
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
Font font, 
#line 1447 "src/../include/raylib.sgh"
const 
#line 1447 "src/../include/raylib.sgh"
char *text, 
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
Vector2 position, 
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
Vector2 origin, 
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
float rotation, 
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
float fontSize, 
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
float spacing, 
#line 1447 "src/../include/raylib.sgh"

#line 1447 "src/../include/raylib.sgh"
Color tint);

#line 1448 "src/../include/raylib.sgh"
extern "C" 
#line 1448 "src/../include/raylib.sgh"
void DrawTextCodepoint(
#line 1448 "src/../include/raylib.sgh"

#line 1448 "src/../include/raylib.sgh"

#line 1448 "src/../include/raylib.sgh"
Font font, 
#line 1448 "src/../include/raylib.sgh"

#line 1448 "src/../include/raylib.sgh"
int codepoint, 
#line 1448 "src/../include/raylib.sgh"

#line 1448 "src/../include/raylib.sgh"
Vector2 position, 
#line 1448 "src/../include/raylib.sgh"

#line 1448 "src/../include/raylib.sgh"
float fontSize, 
#line 1448 "src/../include/raylib.sgh"

#line 1448 "src/../include/raylib.sgh"
Color tint);

#line 1449 "src/../include/raylib.sgh"
extern "C" 
#line 1449 "src/../include/raylib.sgh"
void DrawTextCodepoints(
#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"
Font font, 
#line 1449 "src/../include/raylib.sgh"
const 
#line 1449 "src/../include/raylib.sgh"
int *codepoints, 
#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"
int codepointCount, 
#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"
Vector2 position, 
#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"
float fontSize, 
#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"
float spacing, 
#line 1449 "src/../include/raylib.sgh"

#line 1449 "src/../include/raylib.sgh"
Color tint);

#line 1452 "src/../include/raylib.sgh"
extern "C" 
#line 1452 "src/../include/raylib.sgh"
void SetTextLineSpacing(
#line 1452 "src/../include/raylib.sgh"

#line 1452 "src/../include/raylib.sgh"

#line 1452 "src/../include/raylib.sgh"
int spacing);

#line 1453 "src/../include/raylib.sgh"
extern "C" 
#line 1453 "src/../include/raylib.sgh"
int MeasureText(
#line 1453 "src/../include/raylib.sgh"

#line 1453 "src/../include/raylib.sgh"
const 
#line 1453 "src/../include/raylib.sgh"
char *text, 
#line 1453 "src/../include/raylib.sgh"

#line 1453 "src/../include/raylib.sgh"
int fontSize);

#line 1454 "src/../include/raylib.sgh"
extern "C" 
#line 1454 "src/../include/raylib.sgh"
Vector2 MeasureTextEx(
#line 1454 "src/../include/raylib.sgh"

#line 1454 "src/../include/raylib.sgh"

#line 1454 "src/../include/raylib.sgh"
Font font, 
#line 1454 "src/../include/raylib.sgh"
const 
#line 1454 "src/../include/raylib.sgh"
char *text, 
#line 1454 "src/../include/raylib.sgh"

#line 1454 "src/../include/raylib.sgh"
float fontSize, 
#line 1454 "src/../include/raylib.sgh"

#line 1454 "src/../include/raylib.sgh"
float spacing);

#line 1455 "src/../include/raylib.sgh"
extern "C" 
#line 1455 "src/../include/raylib.sgh"
int GetGlyphIndex(
#line 1455 "src/../include/raylib.sgh"

#line 1455 "src/../include/raylib.sgh"

#line 1455 "src/../include/raylib.sgh"
Font font, 
#line 1455 "src/../include/raylib.sgh"

#line 1455 "src/../include/raylib.sgh"
int codepoint);

#line 1456 "src/../include/raylib.sgh"
extern "C" 
#line 1456 "src/../include/raylib.sgh"
GlyphInfo GetGlyphInfo(
#line 1456 "src/../include/raylib.sgh"

#line 1456 "src/../include/raylib.sgh"

#line 1456 "src/../include/raylib.sgh"
Font font, 
#line 1456 "src/../include/raylib.sgh"

#line 1456 "src/../include/raylib.sgh"
int codepoint);

#line 1457 "src/../include/raylib.sgh"
extern "C" 
#line 1457 "src/../include/raylib.sgh"
Rectangle GetGlyphAtlasRec(
#line 1457 "src/../include/raylib.sgh"

#line 1457 "src/../include/raylib.sgh"

#line 1457 "src/../include/raylib.sgh"
Font font, 
#line 1457 "src/../include/raylib.sgh"

#line 1457 "src/../include/raylib.sgh"
int codepoint);

#line 1460 "src/../include/raylib.sgh"
extern "C" 
#line 1460 "src/../include/raylib.sgh"
char *LoadUTF8(
#line 1460 "src/../include/raylib.sgh"

#line 1460 "src/../include/raylib.sgh"
const 
#line 1460 "src/../include/raylib.sgh"
int *codepoints, 
#line 1460 "src/../include/raylib.sgh"

#line 1460 "src/../include/raylib.sgh"
int length);

#line 1461 "src/../include/raylib.sgh"
extern "C" 
#line 1461 "src/../include/raylib.sgh"
void UnloadUTF8(
#line 1461 "src/../include/raylib.sgh"

#line 1461 "src/../include/raylib.sgh"

#line 1461 "src/../include/raylib.sgh"
char *text);

#line 1462 "src/../include/raylib.sgh"
extern "C" 
#line 1462 "src/../include/raylib.sgh"
int *LoadCodepoints(
#line 1462 "src/../include/raylib.sgh"

#line 1462 "src/../include/raylib.sgh"
const 
#line 1462 "src/../include/raylib.sgh"
char *text, 
#line 1462 "src/../include/raylib.sgh"

#line 1462 "src/../include/raylib.sgh"
int *count);

#line 1463 "src/../include/raylib.sgh"
extern "C" 
#line 1463 "src/../include/raylib.sgh"
void UnloadCodepoints(
#line 1463 "src/../include/raylib.sgh"

#line 1463 "src/../include/raylib.sgh"

#line 1463 "src/../include/raylib.sgh"
int *codepoints);

#line 1464 "src/../include/raylib.sgh"
extern "C" 
#line 1464 "src/../include/raylib.sgh"
int GetCodepointCount(
#line 1464 "src/../include/raylib.sgh"

#line 1464 "src/../include/raylib.sgh"
const 
#line 1464 "src/../include/raylib.sgh"
char *text);

#line 1465 "src/../include/raylib.sgh"
extern "C" 
#line 1465 "src/../include/raylib.sgh"
int GetCodepoint(
#line 1465 "src/../include/raylib.sgh"

#line 1465 "src/../include/raylib.sgh"
const 
#line 1465 "src/../include/raylib.sgh"
char *text, 
#line 1465 "src/../include/raylib.sgh"

#line 1465 "src/../include/raylib.sgh"
int *codepointSize);

#line 1466 "src/../include/raylib.sgh"
extern "C" 
#line 1466 "src/../include/raylib.sgh"
int GetCodepointNext(
#line 1466 "src/../include/raylib.sgh"

#line 1466 "src/../include/raylib.sgh"
const 
#line 1466 "src/../include/raylib.sgh"
char *text, 
#line 1466 "src/../include/raylib.sgh"

#line 1466 "src/../include/raylib.sgh"
int *codepointSize);

#line 1467 "src/../include/raylib.sgh"
extern "C" 
#line 1467 "src/../include/raylib.sgh"
int GetCodepointPrevious(
#line 1467 "src/../include/raylib.sgh"

#line 1467 "src/../include/raylib.sgh"
const 
#line 1467 "src/../include/raylib.sgh"
char *text, 
#line 1467 "src/../include/raylib.sgh"

#line 1467 "src/../include/raylib.sgh"
int *codepointSize);

#line 1468 "src/../include/raylib.sgh"
extern "C" const 
#line 1468 "src/../include/raylib.sgh"
char *CodepointToUTF8(
#line 1468 "src/../include/raylib.sgh"

#line 1468 "src/../include/raylib.sgh"

#line 1468 "src/../include/raylib.sgh"
int codepoint, 
#line 1468 "src/../include/raylib.sgh"

#line 1468 "src/../include/raylib.sgh"
int *utf8Size);

#line 1472 "src/../include/raylib.sgh"
extern "C" 
#line 1472 "src/../include/raylib.sgh"
int TextCopy(
#line 1472 "src/../include/raylib.sgh"

#line 1472 "src/../include/raylib.sgh"

#line 1472 "src/../include/raylib.sgh"
char *dst, 
#line 1472 "src/../include/raylib.sgh"
const 
#line 1472 "src/../include/raylib.sgh"
char *src);

#line 1473 "src/../include/raylib.sgh"
extern "C" 
#line 1473 "src/../include/raylib.sgh"
_Bool TextIsEqual(
#line 1473 "src/../include/raylib.sgh"

#line 1473 "src/../include/raylib.sgh"
const 
#line 1473 "src/../include/raylib.sgh"
char *text1, 
#line 1473 "src/../include/raylib.sgh"
const 
#line 1473 "src/../include/raylib.sgh"
char *text2);

#line 1474 "src/../include/raylib.sgh"
extern "C" 
#line 1474 "src/../include/raylib.sgh"
unsigned int TextLength(
#line 1474 "src/../include/raylib.sgh"

#line 1474 "src/../include/raylib.sgh"
const 
#line 1474 "src/../include/raylib.sgh"
char *text);

#line 1475 "src/../include/raylib.sgh"
extern "C" const 
#line 1475 "src/../include/raylib.sgh"
char *TextFormat(
#line 1475 "src/../include/raylib.sgh"

#line 1475 "src/../include/raylib.sgh"
const 
#line 1475 "src/../include/raylib.sgh"
char *text, 
#line 1475 "src/../include/raylib.sgh"
...);

#line 1476 "src/../include/raylib.sgh"
extern "C" const 
#line 1476 "src/../include/raylib.sgh"
char *TextSubtext(
#line 1476 "src/../include/raylib.sgh"

#line 1476 "src/../include/raylib.sgh"
const 
#line 1476 "src/../include/raylib.sgh"
char *text, 
#line 1476 "src/../include/raylib.sgh"

#line 1476 "src/../include/raylib.sgh"
int position, 
#line 1476 "src/../include/raylib.sgh"

#line 1476 "src/../include/raylib.sgh"
int length);

#line 1477 "src/../include/raylib.sgh"
extern "C" 
#line 1477 "src/../include/raylib.sgh"
char *TextReplace(
#line 1477 "src/../include/raylib.sgh"

#line 1477 "src/../include/raylib.sgh"
const 
#line 1477 "src/../include/raylib.sgh"
char *text, 
#line 1477 "src/../include/raylib.sgh"
const 
#line 1477 "src/../include/raylib.sgh"
char *replace, 
#line 1477 "src/../include/raylib.sgh"
const 
#line 1477 "src/../include/raylib.sgh"
char *by);

#line 1478 "src/../include/raylib.sgh"
extern "C" 
#line 1478 "src/../include/raylib.sgh"
char *TextInsert(
#line 1478 "src/../include/raylib.sgh"

#line 1478 "src/../include/raylib.sgh"
const 
#line 1478 "src/../include/raylib.sgh"
char *text, 
#line 1478 "src/../include/raylib.sgh"
const 
#line 1478 "src/../include/raylib.sgh"
char *insert, 
#line 1478 "src/../include/raylib.sgh"

#line 1478 "src/../include/raylib.sgh"
int position);

#line 1479 "src/../include/raylib.sgh"
extern "C" const 
#line 1479 "src/../include/raylib.sgh"
char *TextJoin(
#line 1479 "src/../include/raylib.sgh"

#line 1479 "src/../include/raylib.sgh"
const 
#line 1479 "src/../include/raylib.sgh"
char **textList, 
#line 1479 "src/../include/raylib.sgh"

#line 1479 "src/../include/raylib.sgh"
int count, 
#line 1479 "src/../include/raylib.sgh"
const 
#line 1479 "src/../include/raylib.sgh"
char *delimiter);

#line 1480 "src/../include/raylib.sgh"
extern "C" const 
#line 1480 "src/../include/raylib.sgh"
char **TextSplit(
#line 1480 "src/../include/raylib.sgh"

#line 1480 "src/../include/raylib.sgh"
const 
#line 1480 "src/../include/raylib.sgh"
char *text, 
#line 1480 "src/../include/raylib.sgh"

#line 1480 "src/../include/raylib.sgh"
char delimiter, 
#line 1480 "src/../include/raylib.sgh"

#line 1480 "src/../include/raylib.sgh"
int *count);

#line 1481 "src/../include/raylib.sgh"
extern "C" 
#line 1481 "src/../include/raylib.sgh"
void TextAppend(
#line 1481 "src/../include/raylib.sgh"

#line 1481 "src/../include/raylib.sgh"

#line 1481 "src/../include/raylib.sgh"
char *text, 
#line 1481 "src/../include/raylib.sgh"
const 
#line 1481 "src/../include/raylib.sgh"
char *append, 
#line 1481 "src/../include/raylib.sgh"

#line 1481 "src/../include/raylib.sgh"
int *position);

#line 1482 "src/../include/raylib.sgh"
extern "C" 
#line 1482 "src/../include/raylib.sgh"
int TextFindIndex(
#line 1482 "src/../include/raylib.sgh"

#line 1482 "src/../include/raylib.sgh"
const 
#line 1482 "src/../include/raylib.sgh"
char *text, 
#line 1482 "src/../include/raylib.sgh"
const 
#line 1482 "src/../include/raylib.sgh"
char *find);

#line 1483 "src/../include/raylib.sgh"
extern "C" const 
#line 1483 "src/../include/raylib.sgh"
char *TextToUpper(
#line 1483 "src/../include/raylib.sgh"

#line 1483 "src/../include/raylib.sgh"
const 
#line 1483 "src/../include/raylib.sgh"
char *text);

#line 1484 "src/../include/raylib.sgh"
extern "C" const 
#line 1484 "src/../include/raylib.sgh"
char *TextToLower(
#line 1484 "src/../include/raylib.sgh"

#line 1484 "src/../include/raylib.sgh"
const 
#line 1484 "src/../include/raylib.sgh"
char *text);

#line 1485 "src/../include/raylib.sgh"
extern "C" const 
#line 1485 "src/../include/raylib.sgh"
char *TextToPascal(
#line 1485 "src/../include/raylib.sgh"

#line 1485 "src/../include/raylib.sgh"
const 
#line 1485 "src/../include/raylib.sgh"
char *text);

#line 1486 "src/../include/raylib.sgh"
extern "C" 
#line 1486 "src/../include/raylib.sgh"
int TextToInteger(
#line 1486 "src/../include/raylib.sgh"

#line 1486 "src/../include/raylib.sgh"
const 
#line 1486 "src/../include/raylib.sgh"
char *text);

#line 1487 "src/../include/raylib.sgh"
extern "C" 
#line 1487 "src/../include/raylib.sgh"
float TextToFloat(
#line 1487 "src/../include/raylib.sgh"

#line 1487 "src/../include/raylib.sgh"
const 
#line 1487 "src/../include/raylib.sgh"
char *text);

#line 1494 "src/../include/raylib.sgh"
extern "C" 
#line 1494 "src/../include/raylib.sgh"
void DrawLine3D(
#line 1494 "src/../include/raylib.sgh"

#line 1494 "src/../include/raylib.sgh"

#line 1494 "src/../include/raylib.sgh"
Vector3 startPos, 
#line 1494 "src/../include/raylib.sgh"

#line 1494 "src/../include/raylib.sgh"
Vector3 endPos, 
#line 1494 "src/../include/raylib.sgh"

#line 1494 "src/../include/raylib.sgh"
Color color);

#line 1495 "src/../include/raylib.sgh"
extern "C" 
#line 1495 "src/../include/raylib.sgh"
void DrawPoint3D(
#line 1495 "src/../include/raylib.sgh"

#line 1495 "src/../include/raylib.sgh"

#line 1495 "src/../include/raylib.sgh"
Vector3 position, 
#line 1495 "src/../include/raylib.sgh"

#line 1495 "src/../include/raylib.sgh"
Color color);

#line 1496 "src/../include/raylib.sgh"
extern "C" 
#line 1496 "src/../include/raylib.sgh"
void DrawCircle3D(
#line 1496 "src/../include/raylib.sgh"

#line 1496 "src/../include/raylib.sgh"

#line 1496 "src/../include/raylib.sgh"
Vector3 center, 
#line 1496 "src/../include/raylib.sgh"

#line 1496 "src/../include/raylib.sgh"
float radius, 
#line 1496 "src/../include/raylib.sgh"

#line 1496 "src/../include/raylib.sgh"
Vector3 rotationAxis, 
#line 1496 "src/../include/raylib.sgh"

#line 1496 "src/../include/raylib.sgh"
float rotationAngle, 
#line 1496 "src/../include/raylib.sgh"

#line 1496 "src/../include/raylib.sgh"
Color color);

#line 1497 "src/../include/raylib.sgh"
extern "C" 
#line 1497 "src/../include/raylib.sgh"
void DrawTriangle3D(
#line 1497 "src/../include/raylib.sgh"

#line 1497 "src/../include/raylib.sgh"

#line 1497 "src/../include/raylib.sgh"
Vector3 v1, 
#line 1497 "src/../include/raylib.sgh"

#line 1497 "src/../include/raylib.sgh"
Vector3 v2, 
#line 1497 "src/../include/raylib.sgh"

#line 1497 "src/../include/raylib.sgh"
Vector3 v3, 
#line 1497 "src/../include/raylib.sgh"

#line 1497 "src/../include/raylib.sgh"
Color color);

#line 1498 "src/../include/raylib.sgh"
extern "C" 
#line 1498 "src/../include/raylib.sgh"
void DrawTriangleStrip3D(
#line 1498 "src/../include/raylib.sgh"

#line 1498 "src/../include/raylib.sgh"

#line 1498 "src/../include/raylib.sgh"
Vector3 *points, 
#line 1498 "src/../include/raylib.sgh"

#line 1498 "src/../include/raylib.sgh"
int pointCount, 
#line 1498 "src/../include/raylib.sgh"

#line 1498 "src/../include/raylib.sgh"
Color color);

#line 1499 "src/../include/raylib.sgh"
extern "C" 
#line 1499 "src/../include/raylib.sgh"
void DrawCube(
#line 1499 "src/../include/raylib.sgh"

#line 1499 "src/../include/raylib.sgh"

#line 1499 "src/../include/raylib.sgh"
Vector3 position, 
#line 1499 "src/../include/raylib.sgh"

#line 1499 "src/../include/raylib.sgh"
float width, 
#line 1499 "src/../include/raylib.sgh"

#line 1499 "src/../include/raylib.sgh"
float height, 
#line 1499 "src/../include/raylib.sgh"

#line 1499 "src/../include/raylib.sgh"
float length, 
#line 1499 "src/../include/raylib.sgh"

#line 1499 "src/../include/raylib.sgh"
Color color);

#line 1500 "src/../include/raylib.sgh"
extern "C" 
#line 1500 "src/../include/raylib.sgh"
void DrawCubeV(
#line 1500 "src/../include/raylib.sgh"

#line 1500 "src/../include/raylib.sgh"

#line 1500 "src/../include/raylib.sgh"
Vector3 position, 
#line 1500 "src/../include/raylib.sgh"

#line 1500 "src/../include/raylib.sgh"
Vector3 size, 
#line 1500 "src/../include/raylib.sgh"

#line 1500 "src/../include/raylib.sgh"
Color color);

#line 1501 "src/../include/raylib.sgh"
extern "C" 
#line 1501 "src/../include/raylib.sgh"
void DrawCubeWires(
#line 1501 "src/../include/raylib.sgh"

#line 1501 "src/../include/raylib.sgh"

#line 1501 "src/../include/raylib.sgh"
Vector3 position, 
#line 1501 "src/../include/raylib.sgh"

#line 1501 "src/../include/raylib.sgh"
float width, 
#line 1501 "src/../include/raylib.sgh"

#line 1501 "src/../include/raylib.sgh"
float height, 
#line 1501 "src/../include/raylib.sgh"

#line 1501 "src/../include/raylib.sgh"
float length, 
#line 1501 "src/../include/raylib.sgh"

#line 1501 "src/../include/raylib.sgh"
Color color);

#line 1502 "src/../include/raylib.sgh"
extern "C" 
#line 1502 "src/../include/raylib.sgh"
void DrawCubeWiresV(
#line 1502 "src/../include/raylib.sgh"

#line 1502 "src/../include/raylib.sgh"

#line 1502 "src/../include/raylib.sgh"
Vector3 position, 
#line 1502 "src/../include/raylib.sgh"

#line 1502 "src/../include/raylib.sgh"
Vector3 size, 
#line 1502 "src/../include/raylib.sgh"

#line 1502 "src/../include/raylib.sgh"
Color color);

#line 1503 "src/../include/raylib.sgh"
extern "C" 
#line 1503 "src/../include/raylib.sgh"
void DrawSphere(
#line 1503 "src/../include/raylib.sgh"

#line 1503 "src/../include/raylib.sgh"

#line 1503 "src/../include/raylib.sgh"
Vector3 centerPos, 
#line 1503 "src/../include/raylib.sgh"

#line 1503 "src/../include/raylib.sgh"
float radius, 
#line 1503 "src/../include/raylib.sgh"

#line 1503 "src/../include/raylib.sgh"
Color color);

#line 1504 "src/../include/raylib.sgh"
extern "C" 
#line 1504 "src/../include/raylib.sgh"
void DrawSphereEx(
#line 1504 "src/../include/raylib.sgh"

#line 1504 "src/../include/raylib.sgh"

#line 1504 "src/../include/raylib.sgh"
Vector3 centerPos, 
#line 1504 "src/../include/raylib.sgh"

#line 1504 "src/../include/raylib.sgh"
float radius, 
#line 1504 "src/../include/raylib.sgh"

#line 1504 "src/../include/raylib.sgh"
int rings, 
#line 1504 "src/../include/raylib.sgh"

#line 1504 "src/../include/raylib.sgh"
int slices, 
#line 1504 "src/../include/raylib.sgh"

#line 1504 "src/../include/raylib.sgh"
Color color);

#line 1505 "src/../include/raylib.sgh"
extern "C" 
#line 1505 "src/../include/raylib.sgh"
void DrawSphereWires(
#line 1505 "src/../include/raylib.sgh"

#line 1505 "src/../include/raylib.sgh"

#line 1505 "src/../include/raylib.sgh"
Vector3 centerPos, 
#line 1505 "src/../include/raylib.sgh"

#line 1505 "src/../include/raylib.sgh"
float radius, 
#line 1505 "src/../include/raylib.sgh"

#line 1505 "src/../include/raylib.sgh"
int rings, 
#line 1505 "src/../include/raylib.sgh"

#line 1505 "src/../include/raylib.sgh"
int slices, 
#line 1505 "src/../include/raylib.sgh"

#line 1505 "src/../include/raylib.sgh"
Color color);

#line 1506 "src/../include/raylib.sgh"
extern "C" 
#line 1506 "src/../include/raylib.sgh"
void DrawCylinder(
#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"
Vector3 position, 
#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"
float radiusTop, 
#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"
float radiusBottom, 
#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"
float height, 
#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"
int slices, 
#line 1506 "src/../include/raylib.sgh"

#line 1506 "src/../include/raylib.sgh"
Color color);

#line 1507 "src/../include/raylib.sgh"
extern "C" 
#line 1507 "src/../include/raylib.sgh"
void DrawCylinderEx(
#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"
Vector3 startPos, 
#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"
Vector3 endPos, 
#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"
float startRadius, 
#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"
float endRadius, 
#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"
int sides, 
#line 1507 "src/../include/raylib.sgh"

#line 1507 "src/../include/raylib.sgh"
Color color);

#line 1508 "src/../include/raylib.sgh"
extern "C" 
#line 1508 "src/../include/raylib.sgh"
void DrawCylinderWires(
#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"
Vector3 position, 
#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"
float radiusTop, 
#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"
float radiusBottom, 
#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"
float height, 
#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"
int slices, 
#line 1508 "src/../include/raylib.sgh"

#line 1508 "src/../include/raylib.sgh"
Color color);

#line 1509 "src/../include/raylib.sgh"
extern "C" 
#line 1509 "src/../include/raylib.sgh"
void DrawCylinderWiresEx(
#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"
Vector3 startPos, 
#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"
Vector3 endPos, 
#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"
float startRadius, 
#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"
float endRadius, 
#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"
int sides, 
#line 1509 "src/../include/raylib.sgh"

#line 1509 "src/../include/raylib.sgh"
Color color);

#line 1510 "src/../include/raylib.sgh"
extern "C" 
#line 1510 "src/../include/raylib.sgh"
void DrawCapsule(
#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"
Vector3 startPos, 
#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"
Vector3 endPos, 
#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"
float radius, 
#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"
int slices, 
#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"
int rings, 
#line 1510 "src/../include/raylib.sgh"

#line 1510 "src/../include/raylib.sgh"
Color color);

#line 1511 "src/../include/raylib.sgh"
extern "C" 
#line 1511 "src/../include/raylib.sgh"
void DrawCapsuleWires(
#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"
Vector3 startPos, 
#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"
Vector3 endPos, 
#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"
float radius, 
#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"
int slices, 
#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"
int rings, 
#line 1511 "src/../include/raylib.sgh"

#line 1511 "src/../include/raylib.sgh"
Color color);

#line 1512 "src/../include/raylib.sgh"
extern "C" 
#line 1512 "src/../include/raylib.sgh"
void DrawPlane(
#line 1512 "src/../include/raylib.sgh"

#line 1512 "src/../include/raylib.sgh"

#line 1512 "src/../include/raylib.sgh"
Vector3 centerPos, 
#line 1512 "src/../include/raylib.sgh"

#line 1512 "src/../include/raylib.sgh"
Vector2 size, 
#line 1512 "src/../include/raylib.sgh"

#line 1512 "src/../include/raylib.sgh"
Color color);

#line 1513 "src/../include/raylib.sgh"
extern "C" 
#line 1513 "src/../include/raylib.sgh"
void DrawRay(
#line 1513 "src/../include/raylib.sgh"

#line 1513 "src/../include/raylib.sgh"

#line 1513 "src/../include/raylib.sgh"
Ray ray, 
#line 1513 "src/../include/raylib.sgh"

#line 1513 "src/../include/raylib.sgh"
Color color);

#line 1514 "src/../include/raylib.sgh"
extern "C" 
#line 1514 "src/../include/raylib.sgh"
void DrawGrid(
#line 1514 "src/../include/raylib.sgh"

#line 1514 "src/../include/raylib.sgh"

#line 1514 "src/../include/raylib.sgh"
int slices, 
#line 1514 "src/../include/raylib.sgh"

#line 1514 "src/../include/raylib.sgh"
float spacing);

#line 1521 "src/../include/raylib.sgh"
extern "C" 
#line 1521 "src/../include/raylib.sgh"
Model LoadModel(
#line 1521 "src/../include/raylib.sgh"

#line 1521 "src/../include/raylib.sgh"
const 
#line 1521 "src/../include/raylib.sgh"
char *fileName);

#line 1522 "src/../include/raylib.sgh"
extern "C" 
#line 1522 "src/../include/raylib.sgh"
Model LoadModelFromMesh(
#line 1522 "src/../include/raylib.sgh"

#line 1522 "src/../include/raylib.sgh"

#line 1522 "src/../include/raylib.sgh"
Mesh mesh);

#line 1523 "src/../include/raylib.sgh"
extern "C" 
#line 1523 "src/../include/raylib.sgh"
_Bool IsModelReady(
#line 1523 "src/../include/raylib.sgh"

#line 1523 "src/../include/raylib.sgh"

#line 1523 "src/../include/raylib.sgh"
Model model);

#line 1524 "src/../include/raylib.sgh"
extern "C" 
#line 1524 "src/../include/raylib.sgh"
void UnloadModel(
#line 1524 "src/../include/raylib.sgh"

#line 1524 "src/../include/raylib.sgh"

#line 1524 "src/../include/raylib.sgh"
Model model);

#line 1525 "src/../include/raylib.sgh"
extern "C" 
#line 1525 "src/../include/raylib.sgh"
BoundingBox GetModelBoundingBox(
#line 1525 "src/../include/raylib.sgh"

#line 1525 "src/../include/raylib.sgh"

#line 1525 "src/../include/raylib.sgh"
Model model);

#line 1528 "src/../include/raylib.sgh"
extern "C" 
#line 1528 "src/../include/raylib.sgh"
void DrawModel(
#line 1528 "src/../include/raylib.sgh"

#line 1528 "src/../include/raylib.sgh"

#line 1528 "src/../include/raylib.sgh"
Model model, 
#line 1528 "src/../include/raylib.sgh"

#line 1528 "src/../include/raylib.sgh"
Vector3 position, 
#line 1528 "src/../include/raylib.sgh"

#line 1528 "src/../include/raylib.sgh"
float scale, 
#line 1528 "src/../include/raylib.sgh"

#line 1528 "src/../include/raylib.sgh"
Color tint);

#line 1529 "src/../include/raylib.sgh"
extern "C" 
#line 1529 "src/../include/raylib.sgh"
void DrawModelEx(
#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"
Model model, 
#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"
Vector3 position, 
#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"
Vector3 rotationAxis, 
#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"
float rotationAngle, 
#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"
Vector3 scale, 
#line 1529 "src/../include/raylib.sgh"

#line 1529 "src/../include/raylib.sgh"
Color tint);

#line 1530 "src/../include/raylib.sgh"
extern "C" 
#line 1530 "src/../include/raylib.sgh"
void DrawModelWires(
#line 1530 "src/../include/raylib.sgh"

#line 1530 "src/../include/raylib.sgh"

#line 1530 "src/../include/raylib.sgh"
Model model, 
#line 1530 "src/../include/raylib.sgh"

#line 1530 "src/../include/raylib.sgh"
Vector3 position, 
#line 1530 "src/../include/raylib.sgh"

#line 1530 "src/../include/raylib.sgh"
float scale, 
#line 1530 "src/../include/raylib.sgh"

#line 1530 "src/../include/raylib.sgh"
Color tint);

#line 1531 "src/../include/raylib.sgh"
extern "C" 
#line 1531 "src/../include/raylib.sgh"
void DrawModelWiresEx(
#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"
Model model, 
#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"
Vector3 position, 
#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"
Vector3 rotationAxis, 
#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"
float rotationAngle, 
#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"
Vector3 scale, 
#line 1531 "src/../include/raylib.sgh"

#line 1531 "src/../include/raylib.sgh"
Color tint);

#line 1532 "src/../include/raylib.sgh"
extern "C" 
#line 1532 "src/../include/raylib.sgh"
void DrawBoundingBox(
#line 1532 "src/../include/raylib.sgh"

#line 1532 "src/../include/raylib.sgh"

#line 1532 "src/../include/raylib.sgh"
BoundingBox box, 
#line 1532 "src/../include/raylib.sgh"

#line 1532 "src/../include/raylib.sgh"
Color color);

#line 1533 "src/../include/raylib.sgh"
extern "C" 
#line 1533 "src/../include/raylib.sgh"
void DrawBillboard(
#line 1533 "src/../include/raylib.sgh"

#line 1533 "src/../include/raylib.sgh"

#line 1533 "src/../include/raylib.sgh"
Camera camera, 
#line 1533 "src/../include/raylib.sgh"

#line 1533 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1533 "src/../include/raylib.sgh"

#line 1533 "src/../include/raylib.sgh"
Vector3 position, 
#line 1533 "src/../include/raylib.sgh"

#line 1533 "src/../include/raylib.sgh"
float size, 
#line 1533 "src/../include/raylib.sgh"

#line 1533 "src/../include/raylib.sgh"
Color tint);

#line 1534 "src/../include/raylib.sgh"
extern "C" 
#line 1534 "src/../include/raylib.sgh"
void DrawBillboardRec(
#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"
Camera camera, 
#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"
Rectangle source, 
#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"
Vector3 position, 
#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"
Vector2 size, 
#line 1534 "src/../include/raylib.sgh"

#line 1534 "src/../include/raylib.sgh"
Color tint);

#line 1535 "src/../include/raylib.sgh"
extern "C" 
#line 1535 "src/../include/raylib.sgh"
void DrawBillboardPro(
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Camera camera, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Texture2D texture, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Rectangle source, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Vector3 position, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Vector3 up, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Vector2 size, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Vector2 origin, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
float rotation, 
#line 1535 "src/../include/raylib.sgh"

#line 1535 "src/../include/raylib.sgh"
Color tint);

#line 1538 "src/../include/raylib.sgh"
extern "C" 
#line 1538 "src/../include/raylib.sgh"
void UploadMesh(
#line 1538 "src/../include/raylib.sgh"

#line 1538 "src/../include/raylib.sgh"

#line 1538 "src/../include/raylib.sgh"
Mesh *mesh, 
#line 1538 "src/../include/raylib.sgh"

#line 1538 "src/../include/raylib.sgh"
_Bool dynamic);

#line 1539 "src/../include/raylib.sgh"
extern "C" 
#line 1539 "src/../include/raylib.sgh"
void UpdateMeshBuffer(
#line 1539 "src/../include/raylib.sgh"

#line 1539 "src/../include/raylib.sgh"

#line 1539 "src/../include/raylib.sgh"
Mesh mesh, 
#line 1539 "src/../include/raylib.sgh"

#line 1539 "src/../include/raylib.sgh"
int index, 
#line 1539 "src/../include/raylib.sgh"
const 
#line 1539 "src/../include/raylib.sgh"
void *data, 
#line 1539 "src/../include/raylib.sgh"

#line 1539 "src/../include/raylib.sgh"
int dataSize, 
#line 1539 "src/../include/raylib.sgh"

#line 1539 "src/../include/raylib.sgh"
int offset);

#line 1540 "src/../include/raylib.sgh"
extern "C" 
#line 1540 "src/../include/raylib.sgh"
void UnloadMesh(
#line 1540 "src/../include/raylib.sgh"

#line 1540 "src/../include/raylib.sgh"

#line 1540 "src/../include/raylib.sgh"
Mesh mesh);

#line 1541 "src/../include/raylib.sgh"
extern "C" 
#line 1541 "src/../include/raylib.sgh"
void DrawMesh(
#line 1541 "src/../include/raylib.sgh"

#line 1541 "src/../include/raylib.sgh"

#line 1541 "src/../include/raylib.sgh"
Mesh mesh, 
#line 1541 "src/../include/raylib.sgh"

#line 1541 "src/../include/raylib.sgh"
Material material, 
#line 1541 "src/../include/raylib.sgh"

#line 1541 "src/../include/raylib.sgh"
Matrix transform);

#line 1542 "src/../include/raylib.sgh"
extern "C" 
#line 1542 "src/../include/raylib.sgh"
void DrawMeshInstanced(
#line 1542 "src/../include/raylib.sgh"

#line 1542 "src/../include/raylib.sgh"

#line 1542 "src/../include/raylib.sgh"
Mesh mesh, 
#line 1542 "src/../include/raylib.sgh"

#line 1542 "src/../include/raylib.sgh"
Material material, 
#line 1542 "src/../include/raylib.sgh"
const 
#line 1542 "src/../include/raylib.sgh"
Matrix *transforms, 
#line 1542 "src/../include/raylib.sgh"

#line 1542 "src/../include/raylib.sgh"
int instances);

#line 1543 "src/../include/raylib.sgh"
extern "C" 
#line 1543 "src/../include/raylib.sgh"
BoundingBox GetMeshBoundingBox(
#line 1543 "src/../include/raylib.sgh"

#line 1543 "src/../include/raylib.sgh"

#line 1543 "src/../include/raylib.sgh"
Mesh mesh);

#line 1544 "src/../include/raylib.sgh"
extern "C" 
#line 1544 "src/../include/raylib.sgh"
void GenMeshTangents(
#line 1544 "src/../include/raylib.sgh"

#line 1544 "src/../include/raylib.sgh"

#line 1544 "src/../include/raylib.sgh"
Mesh *mesh);

#line 1545 "src/../include/raylib.sgh"
extern "C" 
#line 1545 "src/../include/raylib.sgh"
_Bool ExportMesh(
#line 1545 "src/../include/raylib.sgh"

#line 1545 "src/../include/raylib.sgh"

#line 1545 "src/../include/raylib.sgh"
Mesh mesh, 
#line 1545 "src/../include/raylib.sgh"
const 
#line 1545 "src/../include/raylib.sgh"
char *fileName);

#line 1546 "src/../include/raylib.sgh"
extern "C" 
#line 1546 "src/../include/raylib.sgh"
_Bool ExportMeshAsCode(
#line 1546 "src/../include/raylib.sgh"

#line 1546 "src/../include/raylib.sgh"

#line 1546 "src/../include/raylib.sgh"
Mesh mesh, 
#line 1546 "src/../include/raylib.sgh"
const 
#line 1546 "src/../include/raylib.sgh"
char *fileName);

#line 1549 "src/../include/raylib.sgh"
extern "C" 
#line 1549 "src/../include/raylib.sgh"
Mesh GenMeshPoly(
#line 1549 "src/../include/raylib.sgh"

#line 1549 "src/../include/raylib.sgh"

#line 1549 "src/../include/raylib.sgh"
int sides, 
#line 1549 "src/../include/raylib.sgh"

#line 1549 "src/../include/raylib.sgh"
float radius);

#line 1550 "src/../include/raylib.sgh"
extern "C" 
#line 1550 "src/../include/raylib.sgh"
Mesh GenMeshPlane(
#line 1550 "src/../include/raylib.sgh"

#line 1550 "src/../include/raylib.sgh"

#line 1550 "src/../include/raylib.sgh"
float width, 
#line 1550 "src/../include/raylib.sgh"

#line 1550 "src/../include/raylib.sgh"
float length, 
#line 1550 "src/../include/raylib.sgh"

#line 1550 "src/../include/raylib.sgh"
int resX, 
#line 1550 "src/../include/raylib.sgh"

#line 1550 "src/../include/raylib.sgh"
int resZ);

#line 1551 "src/../include/raylib.sgh"
extern "C" 
#line 1551 "src/../include/raylib.sgh"
Mesh GenMeshCube(
#line 1551 "src/../include/raylib.sgh"

#line 1551 "src/../include/raylib.sgh"

#line 1551 "src/../include/raylib.sgh"
float width, 
#line 1551 "src/../include/raylib.sgh"

#line 1551 "src/../include/raylib.sgh"
float height, 
#line 1551 "src/../include/raylib.sgh"

#line 1551 "src/../include/raylib.sgh"
float length);

#line 1552 "src/../include/raylib.sgh"
extern "C" 
#line 1552 "src/../include/raylib.sgh"
Mesh GenMeshSphere(
#line 1552 "src/../include/raylib.sgh"

#line 1552 "src/../include/raylib.sgh"

#line 1552 "src/../include/raylib.sgh"
float radius, 
#line 1552 "src/../include/raylib.sgh"

#line 1552 "src/../include/raylib.sgh"
int rings, 
#line 1552 "src/../include/raylib.sgh"

#line 1552 "src/../include/raylib.sgh"
int slices);

#line 1553 "src/../include/raylib.sgh"
extern "C" 
#line 1553 "src/../include/raylib.sgh"
Mesh GenMeshHemiSphere(
#line 1553 "src/../include/raylib.sgh"

#line 1553 "src/../include/raylib.sgh"

#line 1553 "src/../include/raylib.sgh"
float radius, 
#line 1553 "src/../include/raylib.sgh"

#line 1553 "src/../include/raylib.sgh"
int rings, 
#line 1553 "src/../include/raylib.sgh"

#line 1553 "src/../include/raylib.sgh"
int slices);

#line 1554 "src/../include/raylib.sgh"
extern "C" 
#line 1554 "src/../include/raylib.sgh"
Mesh GenMeshCylinder(
#line 1554 "src/../include/raylib.sgh"

#line 1554 "src/../include/raylib.sgh"

#line 1554 "src/../include/raylib.sgh"
float radius, 
#line 1554 "src/../include/raylib.sgh"

#line 1554 "src/../include/raylib.sgh"
float height, 
#line 1554 "src/../include/raylib.sgh"

#line 1554 "src/../include/raylib.sgh"
int slices);

#line 1555 "src/../include/raylib.sgh"
extern "C" 
#line 1555 "src/../include/raylib.sgh"
Mesh GenMeshCone(
#line 1555 "src/../include/raylib.sgh"

#line 1555 "src/../include/raylib.sgh"

#line 1555 "src/../include/raylib.sgh"
float radius, 
#line 1555 "src/../include/raylib.sgh"

#line 1555 "src/../include/raylib.sgh"
float height, 
#line 1555 "src/../include/raylib.sgh"

#line 1555 "src/../include/raylib.sgh"
int slices);

#line 1556 "src/../include/raylib.sgh"
extern "C" 
#line 1556 "src/../include/raylib.sgh"
Mesh GenMeshTorus(
#line 1556 "src/../include/raylib.sgh"

#line 1556 "src/../include/raylib.sgh"

#line 1556 "src/../include/raylib.sgh"
float radius, 
#line 1556 "src/../include/raylib.sgh"

#line 1556 "src/../include/raylib.sgh"
float size, 
#line 1556 "src/../include/raylib.sgh"

#line 1556 "src/../include/raylib.sgh"
int radSeg, 
#line 1556 "src/../include/raylib.sgh"

#line 1556 "src/../include/raylib.sgh"
int sides);

#line 1557 "src/../include/raylib.sgh"
extern "C" 
#line 1557 "src/../include/raylib.sgh"
Mesh GenMeshKnot(
#line 1557 "src/../include/raylib.sgh"

#line 1557 "src/../include/raylib.sgh"

#line 1557 "src/../include/raylib.sgh"
float radius, 
#line 1557 "src/../include/raylib.sgh"

#line 1557 "src/../include/raylib.sgh"
float size, 
#line 1557 "src/../include/raylib.sgh"

#line 1557 "src/../include/raylib.sgh"
int radSeg, 
#line 1557 "src/../include/raylib.sgh"

#line 1557 "src/../include/raylib.sgh"
int sides);

#line 1558 "src/../include/raylib.sgh"
extern "C" 
#line 1558 "src/../include/raylib.sgh"
Mesh GenMeshHeightmap(
#line 1558 "src/../include/raylib.sgh"

#line 1558 "src/../include/raylib.sgh"

#line 1558 "src/../include/raylib.sgh"
Image heightmap, 
#line 1558 "src/../include/raylib.sgh"

#line 1558 "src/../include/raylib.sgh"
Vector3 size);

#line 1559 "src/../include/raylib.sgh"
extern "C" 
#line 1559 "src/../include/raylib.sgh"
Mesh GenMeshCubicmap(
#line 1559 "src/../include/raylib.sgh"

#line 1559 "src/../include/raylib.sgh"

#line 1559 "src/../include/raylib.sgh"
Image cubicmap, 
#line 1559 "src/../include/raylib.sgh"

#line 1559 "src/../include/raylib.sgh"
Vector3 cubeSize);

#line 1562 "src/../include/raylib.sgh"
extern "C" 
#line 1562 "src/../include/raylib.sgh"
Material *LoadMaterials(
#line 1562 "src/../include/raylib.sgh"

#line 1562 "src/../include/raylib.sgh"
const 
#line 1562 "src/../include/raylib.sgh"
char *fileName, 
#line 1562 "src/../include/raylib.sgh"

#line 1562 "src/../include/raylib.sgh"
int *materialCount);

#line 1563 "src/../include/raylib.sgh"
extern "C" 
#line 1563 "src/../include/raylib.sgh"
Material LoadMaterialDefault(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1563 "src/../include/raylib.sgh"
void);

#line 1564 "src/../include/raylib.sgh"
extern "C" 
#line 1564 "src/../include/raylib.sgh"
_Bool IsMaterialReady(
#line 1564 "src/../include/raylib.sgh"

#line 1564 "src/../include/raylib.sgh"

#line 1564 "src/../include/raylib.sgh"
Material material);

#line 1565 "src/../include/raylib.sgh"
extern "C" 
#line 1565 "src/../include/raylib.sgh"
void UnloadMaterial(
#line 1565 "src/../include/raylib.sgh"

#line 1565 "src/../include/raylib.sgh"

#line 1565 "src/../include/raylib.sgh"
Material material);

#line 1566 "src/../include/raylib.sgh"
extern "C" 
#line 1566 "src/../include/raylib.sgh"
void SetMaterialTexture(
#line 1566 "src/../include/raylib.sgh"

#line 1566 "src/../include/raylib.sgh"

#line 1566 "src/../include/raylib.sgh"
Material *material, 
#line 1566 "src/../include/raylib.sgh"

#line 1566 "src/../include/raylib.sgh"
int mapType, 
#line 1566 "src/../include/raylib.sgh"

#line 1566 "src/../include/raylib.sgh"
Texture2D texture);

#line 1567 "src/../include/raylib.sgh"
extern "C" 
#line 1567 "src/../include/raylib.sgh"
void SetModelMeshMaterial(
#line 1567 "src/../include/raylib.sgh"

#line 1567 "src/../include/raylib.sgh"

#line 1567 "src/../include/raylib.sgh"
Model *model, 
#line 1567 "src/../include/raylib.sgh"

#line 1567 "src/../include/raylib.sgh"
int meshId, 
#line 1567 "src/../include/raylib.sgh"

#line 1567 "src/../include/raylib.sgh"
int materialId);

#line 1570 "src/../include/raylib.sgh"
extern "C" 
#line 1570 "src/../include/raylib.sgh"
ModelAnimation *LoadModelAnimations(
#line 1570 "src/../include/raylib.sgh"

#line 1570 "src/../include/raylib.sgh"
const 
#line 1570 "src/../include/raylib.sgh"
char *fileName, 
#line 1570 "src/../include/raylib.sgh"

#line 1570 "src/../include/raylib.sgh"
int *animCount);

#line 1571 "src/../include/raylib.sgh"
extern "C" 
#line 1571 "src/../include/raylib.sgh"
void UpdateModelAnimation(
#line 1571 "src/../include/raylib.sgh"

#line 1571 "src/../include/raylib.sgh"

#line 1571 "src/../include/raylib.sgh"
Model model, 
#line 1571 "src/../include/raylib.sgh"

#line 1571 "src/../include/raylib.sgh"
ModelAnimation anim, 
#line 1571 "src/../include/raylib.sgh"

#line 1571 "src/../include/raylib.sgh"
int frame);

#line 1572 "src/../include/raylib.sgh"
extern "C" 
#line 1572 "src/../include/raylib.sgh"
void UnloadModelAnimation(
#line 1572 "src/../include/raylib.sgh"

#line 1572 "src/../include/raylib.sgh"

#line 1572 "src/../include/raylib.sgh"
ModelAnimation anim);

#line 1573 "src/../include/raylib.sgh"
extern "C" 
#line 1573 "src/../include/raylib.sgh"
void UnloadModelAnimations(
#line 1573 "src/../include/raylib.sgh"

#line 1573 "src/../include/raylib.sgh"

#line 1573 "src/../include/raylib.sgh"
ModelAnimation *animations, 
#line 1573 "src/../include/raylib.sgh"

#line 1573 "src/../include/raylib.sgh"
int animCount);

#line 1574 "src/../include/raylib.sgh"
extern "C" 
#line 1574 "src/../include/raylib.sgh"
_Bool IsModelAnimationValid(
#line 1574 "src/../include/raylib.sgh"

#line 1574 "src/../include/raylib.sgh"

#line 1574 "src/../include/raylib.sgh"
Model model, 
#line 1574 "src/../include/raylib.sgh"

#line 1574 "src/../include/raylib.sgh"
ModelAnimation anim);

#line 1577 "src/../include/raylib.sgh"
extern "C" 
#line 1577 "src/../include/raylib.sgh"
_Bool CheckCollisionSpheres(
#line 1577 "src/../include/raylib.sgh"

#line 1577 "src/../include/raylib.sgh"

#line 1577 "src/../include/raylib.sgh"
Vector3 center1, 
#line 1577 "src/../include/raylib.sgh"

#line 1577 "src/../include/raylib.sgh"
float radius1, 
#line 1577 "src/../include/raylib.sgh"

#line 1577 "src/../include/raylib.sgh"
Vector3 center2, 
#line 1577 "src/../include/raylib.sgh"

#line 1577 "src/../include/raylib.sgh"
float radius2);

#line 1578 "src/../include/raylib.sgh"
extern "C" 
#line 1578 "src/../include/raylib.sgh"
_Bool CheckCollisionBoxes(
#line 1578 "src/../include/raylib.sgh"

#line 1578 "src/../include/raylib.sgh"

#line 1578 "src/../include/raylib.sgh"
BoundingBox box1, 
#line 1578 "src/../include/raylib.sgh"

#line 1578 "src/../include/raylib.sgh"
BoundingBox box2);

#line 1579 "src/../include/raylib.sgh"
extern "C" 
#line 1579 "src/../include/raylib.sgh"
_Bool CheckCollisionBoxSphere(
#line 1579 "src/../include/raylib.sgh"

#line 1579 "src/../include/raylib.sgh"

#line 1579 "src/../include/raylib.sgh"
BoundingBox box, 
#line 1579 "src/../include/raylib.sgh"

#line 1579 "src/../include/raylib.sgh"
Vector3 center, 
#line 1579 "src/../include/raylib.sgh"

#line 1579 "src/../include/raylib.sgh"
float radius);

#line 1580 "src/../include/raylib.sgh"
extern "C" 
#line 1580 "src/../include/raylib.sgh"
RayCollision GetRayCollisionSphere(
#line 1580 "src/../include/raylib.sgh"

#line 1580 "src/../include/raylib.sgh"

#line 1580 "src/../include/raylib.sgh"
Ray ray, 
#line 1580 "src/../include/raylib.sgh"

#line 1580 "src/../include/raylib.sgh"
Vector3 center, 
#line 1580 "src/../include/raylib.sgh"

#line 1580 "src/../include/raylib.sgh"
float radius);

#line 1581 "src/../include/raylib.sgh"
extern "C" 
#line 1581 "src/../include/raylib.sgh"
RayCollision GetRayCollisionBox(
#line 1581 "src/../include/raylib.sgh"

#line 1581 "src/../include/raylib.sgh"

#line 1581 "src/../include/raylib.sgh"
Ray ray, 
#line 1581 "src/../include/raylib.sgh"

#line 1581 "src/../include/raylib.sgh"
BoundingBox box);

#line 1582 "src/../include/raylib.sgh"
extern "C" 
#line 1582 "src/../include/raylib.sgh"
RayCollision GetRayCollisionMesh(
#line 1582 "src/../include/raylib.sgh"

#line 1582 "src/../include/raylib.sgh"

#line 1582 "src/../include/raylib.sgh"
Ray ray, 
#line 1582 "src/../include/raylib.sgh"

#line 1582 "src/../include/raylib.sgh"
Mesh mesh, 
#line 1582 "src/../include/raylib.sgh"

#line 1582 "src/../include/raylib.sgh"
Matrix transform);

#line 1583 "src/../include/raylib.sgh"
extern "C" 
#line 1583 "src/../include/raylib.sgh"
RayCollision GetRayCollisionTriangle(
#line 1583 "src/../include/raylib.sgh"

#line 1583 "src/../include/raylib.sgh"

#line 1583 "src/../include/raylib.sgh"
Ray ray, 
#line 1583 "src/../include/raylib.sgh"

#line 1583 "src/../include/raylib.sgh"
Vector3 p1, 
#line 1583 "src/../include/raylib.sgh"

#line 1583 "src/../include/raylib.sgh"
Vector3 p2, 
#line 1583 "src/../include/raylib.sgh"

#line 1583 "src/../include/raylib.sgh"
Vector3 p3);

#line 1584 "src/../include/raylib.sgh"
extern "C" 
#line 1584 "src/../include/raylib.sgh"
RayCollision GetRayCollisionQuad(
#line 1584 "src/../include/raylib.sgh"

#line 1584 "src/../include/raylib.sgh"

#line 1584 "src/../include/raylib.sgh"
Ray ray, 
#line 1584 "src/../include/raylib.sgh"

#line 1584 "src/../include/raylib.sgh"
Vector3 p1, 
#line 1584 "src/../include/raylib.sgh"

#line 1584 "src/../include/raylib.sgh"
Vector3 p2, 
#line 1584 "src/../include/raylib.sgh"

#line 1584 "src/../include/raylib.sgh"
Vector3 p3, 
#line 1584 "src/../include/raylib.sgh"

#line 1584 "src/../include/raylib.sgh"
Vector3 p4);

#line 1589 "src/../include/raylib.sgh"
typedef 
#line 1589 "src/../include/raylib.sgh"
void (*AudioCallback)(
#line 1589 "src/../include/raylib.sgh"

#line 1589 "src/../include/raylib.sgh"

#line 1589 "src/../include/raylib.sgh"
void *bufferData, 
#line 1589 "src/../include/raylib.sgh"

#line 1589 "src/../include/raylib.sgh"
unsigned int frames);

#line 1592 "src/../include/raylib.sgh"
extern "C" 
#line 1592 "src/../include/raylib.sgh"
void InitAudioDevice(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1592 "src/../include/raylib.sgh"
void);

#line 1593 "src/../include/raylib.sgh"
extern "C" 
#line 1593 "src/../include/raylib.sgh"
void CloseAudioDevice(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1593 "src/../include/raylib.sgh"
void);

#line 1594 "src/../include/raylib.sgh"
extern "C" 
#line 1594 "src/../include/raylib.sgh"
_Bool IsAudioDeviceReady(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1594 "src/../include/raylib.sgh"
void);

#line 1595 "src/../include/raylib.sgh"
extern "C" 
#line 1595 "src/../include/raylib.sgh"
void SetMasterVolume(
#line 1595 "src/../include/raylib.sgh"

#line 1595 "src/../include/raylib.sgh"

#line 1595 "src/../include/raylib.sgh"
float volume);

#line 1596 "src/../include/raylib.sgh"
extern "C" 
#line 1596 "src/../include/raylib.sgh"
float GetMasterVolume(
#line 0 "src/../include/raylib.sgh"

#line 0 "src/../include/raylib.sgh"

#line 1596 "src/../include/raylib.sgh"
void);

#line 1599 "src/../include/raylib.sgh"
extern "C" 
#line 1599 "src/../include/raylib.sgh"
Wave LoadWave(
#line 1599 "src/../include/raylib.sgh"

#line 1599 "src/../include/raylib.sgh"
const 
#line 1599 "src/../include/raylib.sgh"
char *fileName);

#line 1600 "src/../include/raylib.sgh"
extern "C" 
#line 1600 "src/../include/raylib.sgh"
Wave LoadWaveFromMemory(
#line 1600 "src/../include/raylib.sgh"

#line 1600 "src/../include/raylib.sgh"
const 
#line 1600 "src/../include/raylib.sgh"
char *fileType, 
#line 1600 "src/../include/raylib.sgh"
const 
#line 1600 "src/../include/raylib.sgh"
unsigned char *fileData, 
#line 1600 "src/../include/raylib.sgh"

#line 1600 "src/../include/raylib.sgh"
int dataSize);

#line 1601 "src/../include/raylib.sgh"
extern "C" 
#line 1601 "src/../include/raylib.sgh"
_Bool IsWaveReady(
#line 1601 "src/../include/raylib.sgh"

#line 1601 "src/../include/raylib.sgh"

#line 1601 "src/../include/raylib.sgh"
Wave wave);

#line 1602 "src/../include/raylib.sgh"
extern "C" 
#line 1602 "src/../include/raylib.sgh"
Sound LoadSound(
#line 1602 "src/../include/raylib.sgh"

#line 1602 "src/../include/raylib.sgh"
const 
#line 1602 "src/../include/raylib.sgh"
char *fileName);

#line 1603 "src/../include/raylib.sgh"
extern "C" 
#line 1603 "src/../include/raylib.sgh"
Sound LoadSoundFromWave(
#line 1603 "src/../include/raylib.sgh"

#line 1603 "src/../include/raylib.sgh"

#line 1603 "src/../include/raylib.sgh"
Wave wave);

#line 1604 "src/../include/raylib.sgh"
extern "C" 
#line 1604 "src/../include/raylib.sgh"
Sound LoadSoundAlias(
#line 1604 "src/../include/raylib.sgh"

#line 1604 "src/../include/raylib.sgh"

#line 1604 "src/../include/raylib.sgh"
Sound source);

#line 1605 "src/../include/raylib.sgh"
extern "C" 
#line 1605 "src/../include/raylib.sgh"
_Bool IsSoundReady(
#line 1605 "src/../include/raylib.sgh"

#line 1605 "src/../include/raylib.sgh"

#line 1605 "src/../include/raylib.sgh"
Sound sound);

#line 1606 "src/../include/raylib.sgh"
extern "C" 
#line 1606 "src/../include/raylib.sgh"
void UpdateSound(
#line 1606 "src/../include/raylib.sgh"

#line 1606 "src/../include/raylib.sgh"

#line 1606 "src/../include/raylib.sgh"
Sound sound, 
#line 1606 "src/../include/raylib.sgh"
const 
#line 1606 "src/../include/raylib.sgh"
void *data, 
#line 1606 "src/../include/raylib.sgh"

#line 1606 "src/../include/raylib.sgh"
int sampleCount);

#line 1607 "src/../include/raylib.sgh"
extern "C" 
#line 1607 "src/../include/raylib.sgh"
void UnloadWave(
#line 1607 "src/../include/raylib.sgh"

#line 1607 "src/../include/raylib.sgh"

#line 1607 "src/../include/raylib.sgh"
Wave wave);

#line 1608 "src/../include/raylib.sgh"
extern "C" 
#line 1608 "src/../include/raylib.sgh"
void UnloadSound(
#line 1608 "src/../include/raylib.sgh"

#line 1608 "src/../include/raylib.sgh"

#line 1608 "src/../include/raylib.sgh"
Sound sound);

#line 1609 "src/../include/raylib.sgh"
extern "C" 
#line 1609 "src/../include/raylib.sgh"
void UnloadSoundAlias(
#line 1609 "src/../include/raylib.sgh"

#line 1609 "src/../include/raylib.sgh"

#line 1609 "src/../include/raylib.sgh"
Sound alias);

#line 1610 "src/../include/raylib.sgh"
extern "C" 
#line 1610 "src/../include/raylib.sgh"
_Bool ExportWave(
#line 1610 "src/../include/raylib.sgh"

#line 1610 "src/../include/raylib.sgh"

#line 1610 "src/../include/raylib.sgh"
Wave wave, 
#line 1610 "src/../include/raylib.sgh"
const 
#line 1610 "src/../include/raylib.sgh"
char *fileName);

#line 1611 "src/../include/raylib.sgh"
extern "C" 
#line 1611 "src/../include/raylib.sgh"
_Bool ExportWaveAsCode(
#line 1611 "src/../include/raylib.sgh"

#line 1611 "src/../include/raylib.sgh"

#line 1611 "src/../include/raylib.sgh"
Wave wave, 
#line 1611 "src/../include/raylib.sgh"
const 
#line 1611 "src/../include/raylib.sgh"
char *fileName);

#line 1614 "src/../include/raylib.sgh"
extern "C" 
#line 1614 "src/../include/raylib.sgh"
void PlaySound(
#line 1614 "src/../include/raylib.sgh"

#line 1614 "src/../include/raylib.sgh"

#line 1614 "src/../include/raylib.sgh"
Sound sound);

#line 1615 "src/../include/raylib.sgh"
extern "C" 
#line 1615 "src/../include/raylib.sgh"
void StopSound(
#line 1615 "src/../include/raylib.sgh"

#line 1615 "src/../include/raylib.sgh"

#line 1615 "src/../include/raylib.sgh"
Sound sound);

#line 1616 "src/../include/raylib.sgh"
extern "C" 
#line 1616 "src/../include/raylib.sgh"
void PauseSound(
#line 1616 "src/../include/raylib.sgh"

#line 1616 "src/../include/raylib.sgh"

#line 1616 "src/../include/raylib.sgh"
Sound sound);

#line 1617 "src/../include/raylib.sgh"
extern "C" 
#line 1617 "src/../include/raylib.sgh"
void ResumeSound(
#line 1617 "src/../include/raylib.sgh"

#line 1617 "src/../include/raylib.sgh"

#line 1617 "src/../include/raylib.sgh"
Sound sound);

#line 1618 "src/../include/raylib.sgh"
extern "C" 
#line 1618 "src/../include/raylib.sgh"
_Bool IsSoundPlaying(
#line 1618 "src/../include/raylib.sgh"

#line 1618 "src/../include/raylib.sgh"

#line 1618 "src/../include/raylib.sgh"
Sound sound);

#line 1619 "src/../include/raylib.sgh"
extern "C" 
#line 1619 "src/../include/raylib.sgh"
void SetSoundVolume(
#line 1619 "src/../include/raylib.sgh"

#line 1619 "src/../include/raylib.sgh"

#line 1619 "src/../include/raylib.sgh"
Sound sound, 
#line 1619 "src/../include/raylib.sgh"

#line 1619 "src/../include/raylib.sgh"
float volume);

#line 1620 "src/../include/raylib.sgh"
extern "C" 
#line 1620 "src/../include/raylib.sgh"
void SetSoundPitch(
#line 1620 "src/../include/raylib.sgh"

#line 1620 "src/../include/raylib.sgh"

#line 1620 "src/../include/raylib.sgh"
Sound sound, 
#line 1620 "src/../include/raylib.sgh"

#line 1620 "src/../include/raylib.sgh"
float pitch);

#line 1621 "src/../include/raylib.sgh"
extern "C" 
#line 1621 "src/../include/raylib.sgh"
void SetSoundPan(
#line 1621 "src/../include/raylib.sgh"

#line 1621 "src/../include/raylib.sgh"

#line 1621 "src/../include/raylib.sgh"
Sound sound, 
#line 1621 "src/../include/raylib.sgh"

#line 1621 "src/../include/raylib.sgh"
float pan);

#line 1622 "src/../include/raylib.sgh"
extern "C" 
#line 1622 "src/../include/raylib.sgh"
Wave WaveCopy(
#line 1622 "src/../include/raylib.sgh"

#line 1622 "src/../include/raylib.sgh"

#line 1622 "src/../include/raylib.sgh"
Wave wave);

#line 1623 "src/../include/raylib.sgh"
extern "C" 
#line 1623 "src/../include/raylib.sgh"
void WaveCrop(
#line 1623 "src/../include/raylib.sgh"

#line 1623 "src/../include/raylib.sgh"

#line 1623 "src/../include/raylib.sgh"
Wave *wave, 
#line 1623 "src/../include/raylib.sgh"

#line 1623 "src/../include/raylib.sgh"
int initSample, 
#line 1623 "src/../include/raylib.sgh"

#line 1623 "src/../include/raylib.sgh"
int finalSample);

#line 1624 "src/../include/raylib.sgh"
extern "C" 
#line 1624 "src/../include/raylib.sgh"
void WaveFormat(
#line 1624 "src/../include/raylib.sgh"

#line 1624 "src/../include/raylib.sgh"

#line 1624 "src/../include/raylib.sgh"
Wave *wave, 
#line 1624 "src/../include/raylib.sgh"

#line 1624 "src/../include/raylib.sgh"
int sampleRate, 
#line 1624 "src/../include/raylib.sgh"

#line 1624 "src/../include/raylib.sgh"
int sampleSize, 
#line 1624 "src/../include/raylib.sgh"

#line 1624 "src/../include/raylib.sgh"
int channels);

#line 1625 "src/../include/raylib.sgh"
extern "C" 
#line 1625 "src/../include/raylib.sgh"
float *LoadWaveSamples(
#line 1625 "src/../include/raylib.sgh"

#line 1625 "src/../include/raylib.sgh"

#line 1625 "src/../include/raylib.sgh"
Wave wave);

#line 1626 "src/../include/raylib.sgh"
extern "C" 
#line 1626 "src/../include/raylib.sgh"
void UnloadWaveSamples(
#line 1626 "src/../include/raylib.sgh"

#line 1626 "src/../include/raylib.sgh"

#line 1626 "src/../include/raylib.sgh"
float *samples);

#line 1629 "src/../include/raylib.sgh"
extern "C" 
#line 1629 "src/../include/raylib.sgh"
Music LoadMusicStream(
#line 1629 "src/../include/raylib.sgh"

#line 1629 "src/../include/raylib.sgh"
const 
#line 1629 "src/../include/raylib.sgh"
char *fileName);

#line 1630 "src/../include/raylib.sgh"
extern "C" 
#line 1630 "src/../include/raylib.sgh"
Music LoadMusicStreamFromMemory(
#line 1630 "src/../include/raylib.sgh"

#line 1630 "src/../include/raylib.sgh"
const 
#line 1630 "src/../include/raylib.sgh"
char *fileType, 
#line 1630 "src/../include/raylib.sgh"
const 
#line 1630 "src/../include/raylib.sgh"
unsigned char *data, 
#line 1630 "src/../include/raylib.sgh"

#line 1630 "src/../include/raylib.sgh"
int dataSize);

#line 1631 "src/../include/raylib.sgh"
extern "C" 
#line 1631 "src/../include/raylib.sgh"
_Bool IsMusicReady(
#line 1631 "src/../include/raylib.sgh"

#line 1631 "src/../include/raylib.sgh"

#line 1631 "src/../include/raylib.sgh"
Music music);

#line 1632 "src/../include/raylib.sgh"
extern "C" 
#line 1632 "src/../include/raylib.sgh"
void UnloadMusicStream(
#line 1632 "src/../include/raylib.sgh"

#line 1632 "src/../include/raylib.sgh"

#line 1632 "src/../include/raylib.sgh"
Music music);

#line 1633 "src/../include/raylib.sgh"
extern "C" 
#line 1633 "src/../include/raylib.sgh"
void PlayMusicStream(
#line 1633 "src/../include/raylib.sgh"

#line 1633 "src/../include/raylib.sgh"

#line 1633 "src/../include/raylib.sgh"
Music music);

#line 1634 "src/../include/raylib.sgh"
extern "C" 
#line 1634 "src/../include/raylib.sgh"
_Bool IsMusicStreamPlaying(
#line 1634 "src/../include/raylib.sgh"

#line 1634 "src/../include/raylib.sgh"

#line 1634 "src/../include/raylib.sgh"
Music music);

#line 1635 "src/../include/raylib.sgh"
extern "C" 
#line 1635 "src/../include/raylib.sgh"
void UpdateMusicStream(
#line 1635 "src/../include/raylib.sgh"

#line 1635 "src/../include/raylib.sgh"

#line 1635 "src/../include/raylib.sgh"
Music music);

#line 1636 "src/../include/raylib.sgh"
extern "C" 
#line 1636 "src/../include/raylib.sgh"
void StopMusicStream(
#line 1636 "src/../include/raylib.sgh"

#line 1636 "src/../include/raylib.sgh"

#line 1636 "src/../include/raylib.sgh"
Music music);

#line 1637 "src/../include/raylib.sgh"
extern "C" 
#line 1637 "src/../include/raylib.sgh"
void PauseMusicStream(
#line 1637 "src/../include/raylib.sgh"

#line 1637 "src/../include/raylib.sgh"

#line 1637 "src/../include/raylib.sgh"
Music music);

#line 1638 "src/../include/raylib.sgh"
extern "C" 
#line 1638 "src/../include/raylib.sgh"
void ResumeMusicStream(
#line 1638 "src/../include/raylib.sgh"

#line 1638 "src/../include/raylib.sgh"

#line 1638 "src/../include/raylib.sgh"
Music music);

#line 1639 "src/../include/raylib.sgh"
extern "C" 
#line 1639 "src/../include/raylib.sgh"
void SeekMusicStream(
#line 1639 "src/../include/raylib.sgh"

#line 1639 "src/../include/raylib.sgh"

#line 1639 "src/../include/raylib.sgh"
Music music, 
#line 1639 "src/../include/raylib.sgh"

#line 1639 "src/../include/raylib.sgh"
float position);

#line 1640 "src/../include/raylib.sgh"
extern "C" 
#line 1640 "src/../include/raylib.sgh"
void SetMusicVolume(
#line 1640 "src/../include/raylib.sgh"

#line 1640 "src/../include/raylib.sgh"

#line 1640 "src/../include/raylib.sgh"
Music music, 
#line 1640 "src/../include/raylib.sgh"

#line 1640 "src/../include/raylib.sgh"
float volume);

#line 1641 "src/../include/raylib.sgh"
extern "C" 
#line 1641 "src/../include/raylib.sgh"
void SetMusicPitch(
#line 1641 "src/../include/raylib.sgh"

#line 1641 "src/../include/raylib.sgh"

#line 1641 "src/../include/raylib.sgh"
Music music, 
#line 1641 "src/../include/raylib.sgh"

#line 1641 "src/../include/raylib.sgh"
float pitch);

#line 1642 "src/../include/raylib.sgh"
extern "C" 
#line 1642 "src/../include/raylib.sgh"
void SetMusicPan(
#line 1642 "src/../include/raylib.sgh"

#line 1642 "src/../include/raylib.sgh"

#line 1642 "src/../include/raylib.sgh"
Music music, 
#line 1642 "src/../include/raylib.sgh"

#line 1642 "src/../include/raylib.sgh"
float pan);

#line 1643 "src/../include/raylib.sgh"
extern "C" 
#line 1643 "src/../include/raylib.sgh"
float GetMusicTimeLength(
#line 1643 "src/../include/raylib.sgh"

#line 1643 "src/../include/raylib.sgh"

#line 1643 "src/../include/raylib.sgh"
Music music);

#line 1644 "src/../include/raylib.sgh"
extern "C" 
#line 1644 "src/../include/raylib.sgh"
float GetMusicTimePlayed(
#line 1644 "src/../include/raylib.sgh"

#line 1644 "src/../include/raylib.sgh"

#line 1644 "src/../include/raylib.sgh"
Music music);

#line 1647 "src/../include/raylib.sgh"
extern "C" 
#line 1647 "src/../include/raylib.sgh"
AudioStream LoadAudioStream(
#line 1647 "src/../include/raylib.sgh"

#line 1647 "src/../include/raylib.sgh"

#line 1647 "src/../include/raylib.sgh"
unsigned int sampleRate, 
#line 1647 "src/../include/raylib.sgh"

#line 1647 "src/../include/raylib.sgh"
unsigned int sampleSize, 
#line 1647 "src/../include/raylib.sgh"

#line 1647 "src/../include/raylib.sgh"
unsigned int channels);

#line 1648 "src/../include/raylib.sgh"
extern "C" 
#line 1648 "src/../include/raylib.sgh"
_Bool IsAudioStreamReady(
#line 1648 "src/../include/raylib.sgh"

#line 1648 "src/../include/raylib.sgh"

#line 1648 "src/../include/raylib.sgh"
AudioStream stream);

#line 1649 "src/../include/raylib.sgh"
extern "C" 
#line 1649 "src/../include/raylib.sgh"
void UnloadAudioStream(
#line 1649 "src/../include/raylib.sgh"

#line 1649 "src/../include/raylib.sgh"

#line 1649 "src/../include/raylib.sgh"
AudioStream stream);

#line 1650 "src/../include/raylib.sgh"
extern "C" 
#line 1650 "src/../include/raylib.sgh"
void UpdateAudioStream(
#line 1650 "src/../include/raylib.sgh"

#line 1650 "src/../include/raylib.sgh"

#line 1650 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1650 "src/../include/raylib.sgh"
const 
#line 1650 "src/../include/raylib.sgh"
void *data, 
#line 1650 "src/../include/raylib.sgh"

#line 1650 "src/../include/raylib.sgh"
int frameCount);

#line 1651 "src/../include/raylib.sgh"
extern "C" 
#line 1651 "src/../include/raylib.sgh"
_Bool IsAudioStreamProcessed(
#line 1651 "src/../include/raylib.sgh"

#line 1651 "src/../include/raylib.sgh"

#line 1651 "src/../include/raylib.sgh"
AudioStream stream);

#line 1652 "src/../include/raylib.sgh"
extern "C" 
#line 1652 "src/../include/raylib.sgh"
void PlayAudioStream(
#line 1652 "src/../include/raylib.sgh"

#line 1652 "src/../include/raylib.sgh"

#line 1652 "src/../include/raylib.sgh"
AudioStream stream);

#line 1653 "src/../include/raylib.sgh"
extern "C" 
#line 1653 "src/../include/raylib.sgh"
void PauseAudioStream(
#line 1653 "src/../include/raylib.sgh"

#line 1653 "src/../include/raylib.sgh"

#line 1653 "src/../include/raylib.sgh"
AudioStream stream);

#line 1654 "src/../include/raylib.sgh"
extern "C" 
#line 1654 "src/../include/raylib.sgh"
void ResumeAudioStream(
#line 1654 "src/../include/raylib.sgh"

#line 1654 "src/../include/raylib.sgh"

#line 1654 "src/../include/raylib.sgh"
AudioStream stream);

#line 1655 "src/../include/raylib.sgh"
extern "C" 
#line 1655 "src/../include/raylib.sgh"
_Bool IsAudioStreamPlaying(
#line 1655 "src/../include/raylib.sgh"

#line 1655 "src/../include/raylib.sgh"

#line 1655 "src/../include/raylib.sgh"
AudioStream stream);

#line 1656 "src/../include/raylib.sgh"
extern "C" 
#line 1656 "src/../include/raylib.sgh"
void StopAudioStream(
#line 1656 "src/../include/raylib.sgh"

#line 1656 "src/../include/raylib.sgh"

#line 1656 "src/../include/raylib.sgh"
AudioStream stream);

#line 1657 "src/../include/raylib.sgh"
extern "C" 
#line 1657 "src/../include/raylib.sgh"
void SetAudioStreamVolume(
#line 1657 "src/../include/raylib.sgh"

#line 1657 "src/../include/raylib.sgh"

#line 1657 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1657 "src/../include/raylib.sgh"

#line 1657 "src/../include/raylib.sgh"
float volume);

#line 1658 "src/../include/raylib.sgh"
extern "C" 
#line 1658 "src/../include/raylib.sgh"
void SetAudioStreamPitch(
#line 1658 "src/../include/raylib.sgh"

#line 1658 "src/../include/raylib.sgh"

#line 1658 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1658 "src/../include/raylib.sgh"

#line 1658 "src/../include/raylib.sgh"
float pitch);

#line 1659 "src/../include/raylib.sgh"
extern "C" 
#line 1659 "src/../include/raylib.sgh"
void SetAudioStreamPan(
#line 1659 "src/../include/raylib.sgh"

#line 1659 "src/../include/raylib.sgh"

#line 1659 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1659 "src/../include/raylib.sgh"

#line 1659 "src/../include/raylib.sgh"
float pan);

#line 1660 "src/../include/raylib.sgh"
extern "C" 
#line 1660 "src/../include/raylib.sgh"
void SetAudioStreamBufferSizeDefault(
#line 1660 "src/../include/raylib.sgh"

#line 1660 "src/../include/raylib.sgh"

#line 1660 "src/../include/raylib.sgh"
int size);

#line 1661 "src/../include/raylib.sgh"
extern "C" 
#line 1661 "src/../include/raylib.sgh"
void SetAudioStreamCallback(
#line 1661 "src/../include/raylib.sgh"

#line 1661 "src/../include/raylib.sgh"

#line 1661 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1661 "src/../include/raylib.sgh"

#line 1661 "src/../include/raylib.sgh"
AudioCallback callback);

#line 1663 "src/../include/raylib.sgh"
extern "C" 
#line 1663 "src/../include/raylib.sgh"
void AttachAudioStreamProcessor(
#line 1663 "src/../include/raylib.sgh"

#line 1663 "src/../include/raylib.sgh"

#line 1663 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1663 "src/../include/raylib.sgh"

#line 1663 "src/../include/raylib.sgh"
AudioCallback processor);

#line 1664 "src/../include/raylib.sgh"
extern "C" 
#line 1664 "src/../include/raylib.sgh"
void DetachAudioStreamProcessor(
#line 1664 "src/../include/raylib.sgh"

#line 1664 "src/../include/raylib.sgh"

#line 1664 "src/../include/raylib.sgh"
AudioStream stream, 
#line 1664 "src/../include/raylib.sgh"

#line 1664 "src/../include/raylib.sgh"
AudioCallback processor);

#line 1666 "src/../include/raylib.sgh"
extern "C" 
#line 1666 "src/../include/raylib.sgh"
void AttachAudioMixedProcessor(
#line 1666 "src/../include/raylib.sgh"

#line 1666 "src/../include/raylib.sgh"

#line 1666 "src/../include/raylib.sgh"
AudioCallback processor);

#line 1667 "src/../include/raylib.sgh"
extern "C" 
#line 1667 "src/../include/raylib.sgh"
void DetachAudioMixedProcessor(
#line 1667 "src/../include/raylib.sgh"

#line 1667 "src/../include/raylib.sgh"

#line 1667 "src/../include/raylib.sgh"
AudioCallback processor);

#line 4 "src/sgccplots.sgc"

#line 4 "src/sgccplots.sgc"
struct drawer_struct;

#line 5 "src/sgccplots.sgc"
typedef 
#line 5 "src/sgccplots.sgc"
struct drawer_struct *drawer;

#line 6 "src/../include/sgccplots.sgh"

#line 6 "src/../include/sgccplots.sgh"
enum generic_arg_t
{

#line 7 "src/../include/sgccplots.sgh"
  ARG_COLOR,

#line 8 "src/../include/sgccplots.sgh"
  ARG_RADIUS,

#line 9 "src/../include/sgccplots.sgh"
  ARG_THICKNESS,

#line 10 "src/../include/sgccplots.sgh"
  ARG_STYLE
};

#line 19 "src/../include/sgccplots.sgh"
typedef 
#line 13 "src/../include/sgccplots.sgh"
struct 
{
  
#line 14 "src/../include/sgccplots.sgh"

#line 14 "src/../include/sgccplots.sgh"
enum generic_arg_t t;
  
#line 18 "src/../include/sgccplots.sgh"

#line 15 "src/../include/sgccplots.sgh"
union 
  {
    
#line 16 "src/../include/sgccplots.sgh"

#line 16 "src/../include/sgccplots.sgh"
float f;
    
#line 17 "src/../include/sgccplots.sgh"
const 
#line 17 "src/../include/sgccplots.sgh"
char *s;
  } data;
} plot_arg;

#line 9 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
typedef 
#line 4 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
struct 
{
  
#line 5 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 5 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int start;
  
#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 6 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int stop;
  
#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 7 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int step;
  
#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 8 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int cur;
} range_t;

#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
typedef 
#line 11 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_t *range_obj;

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj __iter__(
#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj r)

#line 13 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
{
  
#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 14 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r;
}


#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int __start__(
#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj r)

#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
{
  
#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
cur = 
#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
start;
  
#line 19 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 19 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 19 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 19 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
cur;
}


#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
_Bool __done__(
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj r)

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
{
  
#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
__ge__(
#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
cur, 
#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 23 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop);
}


#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int __next__(
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj r)

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
{
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
cur = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
__add__(
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
cur, 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
step);
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
r->
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
cur;
}


#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj range(
#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int stop)

#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
{
  
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj out = 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj) 
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
GC_malloc(
#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 32 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_t));
  
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
start = 
#line 33 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
0;
  
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop = 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop;
  
#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
step = 
#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
1;
  
#line 36 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 36 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out;
}


#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj range(
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int start, 
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int stop)

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
{
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj out = 
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
(
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj) 
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
GC_malloc(
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_t));
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
start = 
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
start;
  
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop = 
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop;
  
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
step = 
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
1;
  
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out;
}


#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
inline static 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj range(
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int start, 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int stop, 
#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
int step)

#line 45 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj out = 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
(
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_obj) 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
GC_malloc(
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
range_t));
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
start = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
start;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop = 
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
stop;
  
#line 49 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 49 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"

#line 49 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out->
#line 49 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
step = 
#line 49 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
step;
  
#line 50 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
return 
#line 50 "/Users/humzaqureshi/GitHub/sugaryc/include/range.sgh"
out;
}


#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 15 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__int
{
  
#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 16 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *raw;
  
#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 17 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int length;
  
#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 18 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n_allocated;
  
#line 19 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 19 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int cur;
};

#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
typedef 
#line 22 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__int *std__list__template__int;

#line 24 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 24 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 24 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int std__make_list__template__int(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 24 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 24 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int out = 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int) 
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 25 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__int));
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out != NULL");
    
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 26 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length = 
#line 27 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 28 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10;
  
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *) 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int), 
#line 29 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10));
  
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out->raw != NULL");
    
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 30 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 31 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int __init____template__std__list__template__int(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 34 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 35 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__make_list__template__int();
}


#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void append(
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst, 
#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int val)

#line 38 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 39 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length >= 0");
    
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 40 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 41 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 42 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *) 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int), 
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 43 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 44 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length < lst->n_allocated");
    
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 46 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length) = 
#line 47 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val;
  
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 48 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
  
#line 49 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return;
}


#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void __append__(
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst, 
#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int val)

#line 52 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
append(
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 53 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val);
}


#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int pop(
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst, 
#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx)

#line 56 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 57 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 58 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int out = 
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 59 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
  
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx; 
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 60 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i) = 
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 61 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
  }

  
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 63 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length--;
  
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length), 
#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10))

#line 64 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 65 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated > 0");
      
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 66 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated >= lst->length");
      
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 67 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *) 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int), 
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 68 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
  }
  
#line 70 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 70 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int pop(
#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 73 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 74 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length > 0");
    
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 75 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
pop(
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 76 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
}


#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int len(
#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 79 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 80 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 81 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 81 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 81 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 81 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length;
}


#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void insert(
#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst, 
#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx, 
#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int value)

#line 84 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 85 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 86 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1), 
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 87 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 88 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *) 
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int), 
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 89 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 90 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1); 
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx); 
#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i--)

#line 92 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)) = 
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 93 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i);
  }

  
#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx) = 
#line 95 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
value;
  
#line 96 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 96 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 96 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 96 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
}


#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *__index__(
#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst, 
#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *idxs, 
#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n)

#line 100 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 101 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n, 
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))

#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"n == 1");
    
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 102 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 103 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 103 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx = 
#line 103 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 103 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 103 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idxs, 
#line 103 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0);
  
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 104 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
    
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 105 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return &
#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 106 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
}


#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string __str__(
#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 109 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 110 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 110 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string out = 
#line 110 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"[";
  
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0; 
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 111 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i)), 
#line 112 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
", "));
  }

  
#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 114 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 115 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))));
  }
  
#line 117 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 117 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 117 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 117 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 117 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"]");
  
#line 118 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 118 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int __iter__(
#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 121 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 122 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 122 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst;
}


#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int __start__(
#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 125 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 126 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 126 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 126 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 126 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur = 
#line 126 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 127 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 128 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 128 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int empty;
    
#line 129 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 129 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
empty;
  }
  
#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 131 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
_Bool __done__(
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur, 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length);
}


#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int __next__(
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur++;
  
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void destroy(
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int lst)

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw);
  
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst);
}


#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__plot_arg
{
  
#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg *raw;
  
#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int length;
  
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n_allocated;
  
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int cur;
};

#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
typedef 
#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__plot_arg *std__list__template__plot_arg;

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg std__make_list__template__plot_arg(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg out = 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg) 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__plot_arg));
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out != NULL");
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length = 
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10;
  
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg *) 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg), 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10));
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out->raw != NULL");
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg __init____template__std__list__template__plot_arg(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__make_list__template__plot_arg();
}


#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void append(
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst, 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg val)

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length >= 0");
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg *) 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg), 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length < lst->n_allocated");
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length) = 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val;
  
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
  
#line 185 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return;
}


#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void __append__(
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst, 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg val)

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
append(
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val);
}


#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg pop(
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst, 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx)

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg out = 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
  
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx; 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i) = 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
  }

  
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length--;
  
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length), 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10))

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated > 0");
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated >= lst->length");
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg *) 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg), 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
  }
  
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg pop(
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length > 0");
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
pop(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
}


#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int len(
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length;
}


#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void insert(
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg value)

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1), 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg *) 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg), 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i--)

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)) = 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i);
  }

  
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx) = 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
value;
  
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
}


#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg *__index__(
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *idxs, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n)

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n, 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"n == 1");
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx = 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idxs, 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0);
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return &
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
}


#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string __str__(
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string out = 
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"[";
  
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0; 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i)), 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
", "));
  }

  
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))));
  }
  
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"]");
  
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg __iter__(
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst;
}


#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg __start__(
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur = 
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg empty;
    
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
empty;
  }
  
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
_Bool __done__(
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur, 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length);
}


#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
plot_arg __next__(
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur++;
  
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void destroy(
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__plot_arg lst)

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw);
  
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst);
}


#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__double
{
  
#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double *raw;
  
#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int length;
  
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n_allocated;
  
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int cur;
};

#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
typedef 
#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__double *std__list__template__double;

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double std__make_list__template__double(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double out = 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double) 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__double));
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out != NULL");
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length = 
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10;
  
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double *) 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double), 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10));
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out->raw != NULL");
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double __init____template__std__list__template__double(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__make_list__template__double();
}


#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void append(
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst, 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double val)

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length >= 0");
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double *) 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double), 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length < lst->n_allocated");
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length) = 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val;
  
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
  
#line 185 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return;
}


#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void __append__(
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst, 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double val)

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
append(
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val);
}


#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double pop(
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst, 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx)

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double out = 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
  
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx; 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i) = 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
  }

  
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length--;
  
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length), 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10))

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated > 0");
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated >= lst->length");
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double *) 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double), 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
  }
  
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double pop(
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length > 0");
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
pop(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
}


#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int len(
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length;
}


#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void insert(
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double value)

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1), 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double *) 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double), 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i--)

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)) = 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i);
  }

  
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx) = 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
value;
  
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
}


#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double *__index__(
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *idxs, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n)

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n, 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"n == 1");
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx = 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idxs, 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0);
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return &
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
}


#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string __str__(
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string out = 
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"[";
  
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0; 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i)), 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
", "));
  }

  
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))));
  }
  
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"]");
  
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double __iter__(
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst;
}


#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double __start__(
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur = 
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double empty;
    
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
empty;
  }
  
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
_Bool __done__(
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur, 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length);
}


#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
double __next__(
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur++;
  
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void destroy(
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__double lst)

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw);
  
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst);
}


#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__drawer
{
  
#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer *raw;
  
#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int length;
  
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n_allocated;
  
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int cur;
};

#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
typedef 
#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__drawer *std__list__template__drawer;

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer std__make_list__template__drawer(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer out = 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer) 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__drawer));
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out != NULL");
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length = 
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10;
  
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer *) 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer), 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10));
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out->raw != NULL");
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer __init____template__std__list__template__drawer(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__make_list__template__drawer();
}


#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void append(
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst, 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer val)

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length >= 0");
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer *) 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer), 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length < lst->n_allocated");
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length) = 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val;
  
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
  
#line 185 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return;
}


#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void __append__(
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst, 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer val)

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
append(
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val);
}


#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer pop(
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst, 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx)

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer out = 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
  
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx; 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i) = 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
  }

  
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length--;
  
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length), 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10))

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated > 0");
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated >= lst->length");
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer *) 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer), 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
  }
  
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer pop(
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length > 0");
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
pop(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
}


#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int len(
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length;
}


#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void insert(
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer value)

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1), 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer *) 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer), 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i--)

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)) = 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i);
  }

  
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx) = 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
value;
  
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
}


#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer *__index__(
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *idxs, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n)

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n, 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"n == 1");
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx = 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idxs, 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0);
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return &
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
}


#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string __str__(
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string out = 
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"[";
  
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0; 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i)), 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
", "));
  }

  
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))));
  }
  
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"]");
  
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer __iter__(
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst;
}


#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer __start__(
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur = 
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer empty;
    
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
empty;
  }
  
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
_Bool __done__(
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur, 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length);
}


#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
drawer __next__(
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur++;
  
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void destroy(
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__drawer lst)

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw);
  
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst);
}


#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 151 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__float
{
  
#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float *raw;
  
#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int length;
  
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n_allocated;
  
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int cur;
};

#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
typedef 
#line 158 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__float *std__list__template__float;

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float std__make_list__template__float(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 160 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float out = 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float) 
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 161 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
struct list_struct__template__float));
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out != NULL");
    
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 162 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length = 
#line 163 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 164 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10;
  
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float *) 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_malloc(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float), 
#line 165 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10));
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out->
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"out->raw != NULL");
    
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 166 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 167 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float __init____template__std__list__template__float(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void)

#line 170 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 171 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__make_list__template__float();
}


#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void append(
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst, 
#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float val)

#line 174 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 175 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length >= 0");
    
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 176 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 177 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 178 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float *) 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float), 
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 179 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 180 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length < lst->n_allocated");
    
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 182 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length) = 
#line 183 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val;
  
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 184 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
  
#line 185 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return;
}


#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void __append__(
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst, 
#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float val)

#line 188 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
append(
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 189 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
val);
}


#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float pop(
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst, 
#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx)

#line 192 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 193 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 194 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float out = 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 195 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
  
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx; 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 196 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i) = 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 197 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
  }

  
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 199 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length--;
  
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length), 
#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10))

#line 200 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 201 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated > 0");
      
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 202 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->n_allocated >= lst->length");
      
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 203 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
    
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float *) 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float), 
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 204 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
  }
  
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 206 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float pop(
#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 209 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 210 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->length > 0");
    
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 211 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
pop(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 212 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1));
}


#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int len(
#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 215 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 216 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 217 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length;
}


#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void insert(
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx, 
#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float value)

#line 220 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 221 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 222 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1), 
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated))

#line 223 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated, 
#line 224 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
10);
    
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw = 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float *) 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_realloc(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__mul__(
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
sizeof(
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float), 
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 225 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n_allocated));
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
    {
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
      
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
    }
    
#line 226 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  }
  
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx); 
#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i--)

#line 228 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)) = 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 229 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i);
  }

  
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx) = 
#line 231 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
value;
  
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 232 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length++;
}


#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float *__index__(
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int *idxs, 
#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int n)

#line 236 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst, 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst != NULL");
    
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 237 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
n, 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"n == 1");
    
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 238 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int idx = 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idxs, 
#line 239 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0);
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx, 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length))

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"idx < lst->length");
    
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 240 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (!
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__neq__(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
nullptr))

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
printf(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"%s: FAILED\n", 
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"lst->raw != NULL");
    
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
exit(
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1);
  }
  
#line 241 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
;
  
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return &
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 242 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
idx);
}


#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string __str__(
#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 245 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
string out = 
#line 246 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"[";
  
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
int i = 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0; 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__lt__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1)); 
#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i++)

#line 247 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i)), 
#line 248 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
", "));
  }

  
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__gt__(
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 250 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
to_str(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__sub__(
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 251 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
1))));
  }
  
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out = 
#line 0 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__add__(
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 253 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
"]");
  
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 254 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float __iter__(
#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 257 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 258 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst;
}


#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float __start__(
#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 261 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur = 
#line 262 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0;
  
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
if (
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__eq__(
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length, 
#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
0))

#line 263 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 264 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float empty;
    
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 265 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
empty;
  }
  
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 267 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
_Bool __done__(
#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 134 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__ge__(
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur, 
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 135 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
length);
}


#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
float __next__(
#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 138 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 139 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur++;
  
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__index_wrapper__(
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw, 
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 140 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
cur);
}


#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
void destroy(
#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__float lst)

#line 143 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
{
  
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst->
#line 144 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
raw);
  
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
GC_free(
#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 145 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
lst);
}


#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
inline static 
#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int to_list(
#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 152 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
range_obj obj)

#line 152 "src/../include/sgccplots.sgh"
{
  
#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__list__template__int out = 
#line 153 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
std__make_list__template__int();
  
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
auto __iter_foreach_1 = 
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__iter__(
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
obj);
  
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
for (
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
auto i = 
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__start__(
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__iter_foreach_1); !
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__done__(
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__iter_foreach_1); 
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i = 
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__next__(
#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
__iter_foreach_1))

#line 154 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
  {
    
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
append(
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"

#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out, 
#line 155 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
i);
  }

  
#line 157 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
return 
#line 157 "/Users/humzaqureshi/GitHub/sugaryc/include/lists.sgh"
out;
}


#line 23 "src/../include/sgccplots.sgh"
typedef 
#line 23 "src/../include/sgccplots.sgh"
std__list__template__plot_arg plot_arg_list;

#line 25 "src/../include/sgccplots.sgh"

#line 25 "src/../include/sgccplots.sgh"
plot_arg_list __add__(
#line 25 "src/../include/sgccplots.sgh"

#line 25 "src/../include/sgccplots.sgh"

#line 25 "src/../include/sgccplots.sgh"
plot_arg a, 
#line 25 "src/../include/sgccplots.sgh"

#line 25 "src/../include/sgccplots.sgh"
plot_arg b);

#line 27 "src/../include/sgccplots.sgh"

#line 27 "src/../include/sgccplots.sgh"
plot_arg_list __add__(
#line 27 "src/../include/sgccplots.sgh"

#line 27 "src/../include/sgccplots.sgh"

#line 27 "src/../include/sgccplots.sgh"
plot_arg_list a, 
#line 27 "src/../include/sgccplots.sgh"

#line 27 "src/../include/sgccplots.sgh"
plot_arg b);

#line 29 "src/../include/sgccplots.sgh"

#line 29 "src/../include/sgccplots.sgh"
struct plot_struct;

#line 30 "src/../include/sgccplots.sgh"
typedef 
#line 30 "src/../include/sgccplots.sgh"
struct plot_struct *plot;

#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
plot make_plot(
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
int screenWidth, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
int screenHeight, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
double x_start, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
double y_start, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
double x_end, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
double y_end, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
double padx, 
#line 32 "src/../include/sgccplots.sgh"

#line 32 "src/../include/sgccplots.sgh"
double pady);

#line 34 "src/../include/sgccplots.sgh"

#line 34 "src/../include/sgccplots.sgh"
void show(
#line 34 "src/../include/sgccplots.sgh"

#line 34 "src/../include/sgccplots.sgh"

#line 34 "src/../include/sgccplots.sgh"
plot plt);

#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"
void line(
#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"
plot plt, 
#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"
float thickness, 
#line 38 "src/../include/sgccplots.sgh"

#line 38 "src/../include/sgccplots.sgh"
string color);

#line 40 "src/../include/sgccplots.sgh"

#line 40 "src/../include/sgccplots.sgh"
void line(
#line 40 "src/../include/sgccplots.sgh"

#line 40 "src/../include/sgccplots.sgh"

#line 40 "src/../include/sgccplots.sgh"
plot plt, 
#line 40 "src/../include/sgccplots.sgh"

#line 40 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 40 "src/../include/sgccplots.sgh"

#line 40 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 40 "src/../include/sgccplots.sgh"

#line 40 "src/../include/sgccplots.sgh"
float thickness);

#line 42 "src/../include/sgccplots.sgh"

#line 42 "src/../include/sgccplots.sgh"
void line(
#line 42 "src/../include/sgccplots.sgh"

#line 42 "src/../include/sgccplots.sgh"

#line 42 "src/../include/sgccplots.sgh"
plot plt, 
#line 42 "src/../include/sgccplots.sgh"

#line 42 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 42 "src/../include/sgccplots.sgh"

#line 42 "src/../include/sgccplots.sgh"
std__list__template__int ys);

#line 44 "src/../include/sgccplots.sgh"

#line 44 "src/../include/sgccplots.sgh"
void line(
#line 44 "src/../include/sgccplots.sgh"

#line 44 "src/../include/sgccplots.sgh"

#line 44 "src/../include/sgccplots.sgh"
plot plt, 
#line 44 "src/../include/sgccplots.sgh"

#line 44 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 44 "src/../include/sgccplots.sgh"

#line 44 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 44 "src/../include/sgccplots.sgh"

#line 44 "src/../include/sgccplots.sgh"
plot_arg arg0);

#line 46 "src/../include/sgccplots.sgh"

#line 46 "src/../include/sgccplots.sgh"
void line(
#line 46 "src/../include/sgccplots.sgh"

#line 46 "src/../include/sgccplots.sgh"

#line 46 "src/../include/sgccplots.sgh"
plot plt, 
#line 46 "src/../include/sgccplots.sgh"

#line 46 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 46 "src/../include/sgccplots.sgh"

#line 46 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 46 "src/../include/sgccplots.sgh"

#line 46 "src/../include/sgccplots.sgh"
plot_arg_list args);

#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
void scatter(
#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
plot plt, 
#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
string style, 
#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
float radius, 
#line 48 "src/../include/sgccplots.sgh"

#line 48 "src/../include/sgccplots.sgh"
string color);

#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"
void scatter(
#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"
plot plt, 
#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"
string style, 
#line 50 "src/../include/sgccplots.sgh"

#line 50 "src/../include/sgccplots.sgh"
float radius);

#line 52 "src/../include/sgccplots.sgh"

#line 52 "src/../include/sgccplots.sgh"
void scatter(
#line 52 "src/../include/sgccplots.sgh"

#line 52 "src/../include/sgccplots.sgh"

#line 52 "src/../include/sgccplots.sgh"
plot plt, 
#line 52 "src/../include/sgccplots.sgh"

#line 52 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 52 "src/../include/sgccplots.sgh"

#line 52 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 52 "src/../include/sgccplots.sgh"

#line 52 "src/../include/sgccplots.sgh"
string style);

#line 54 "src/../include/sgccplots.sgh"

#line 54 "src/../include/sgccplots.sgh"
void scatter(
#line 54 "src/../include/sgccplots.sgh"

#line 54 "src/../include/sgccplots.sgh"

#line 54 "src/../include/sgccplots.sgh"
plot plt, 
#line 54 "src/../include/sgccplots.sgh"

#line 54 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 54 "src/../include/sgccplots.sgh"

#line 54 "src/../include/sgccplots.sgh"
std__list__template__int ys);

#line 56 "src/../include/sgccplots.sgh"

#line 56 "src/../include/sgccplots.sgh"
void scatter(
#line 56 "src/../include/sgccplots.sgh"

#line 56 "src/../include/sgccplots.sgh"

#line 56 "src/../include/sgccplots.sgh"
plot plt, 
#line 56 "src/../include/sgccplots.sgh"

#line 56 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 56 "src/../include/sgccplots.sgh"

#line 56 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 56 "src/../include/sgccplots.sgh"

#line 56 "src/../include/sgccplots.sgh"
plot_arg arg0);

#line 58 "src/../include/sgccplots.sgh"

#line 58 "src/../include/sgccplots.sgh"
void scatter(
#line 58 "src/../include/sgccplots.sgh"

#line 58 "src/../include/sgccplots.sgh"

#line 58 "src/../include/sgccplots.sgh"
plot plt, 
#line 58 "src/../include/sgccplots.sgh"

#line 58 "src/../include/sgccplots.sgh"
std__list__template__int xs, 
#line 58 "src/../include/sgccplots.sgh"

#line 58 "src/../include/sgccplots.sgh"
std__list__template__int ys, 
#line 58 "src/../include/sgccplots.sgh"

#line 58 "src/../include/sgccplots.sgh"
plot_arg_list args);

#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"
void line(
#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"
plot plt, 
#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"
float thickness, 
#line 63 "src/../include/sgccplots.sgh"

#line 63 "src/../include/sgccplots.sgh"
string color);

#line 65 "src/../include/sgccplots.sgh"

#line 65 "src/../include/sgccplots.sgh"
void line(
#line 65 "src/../include/sgccplots.sgh"

#line 65 "src/../include/sgccplots.sgh"

#line 65 "src/../include/sgccplots.sgh"
plot plt, 
#line 65 "src/../include/sgccplots.sgh"

#line 65 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 65 "src/../include/sgccplots.sgh"

#line 65 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 65 "src/../include/sgccplots.sgh"

#line 65 "src/../include/sgccplots.sgh"
float thickness);

#line 67 "src/../include/sgccplots.sgh"

#line 67 "src/../include/sgccplots.sgh"
void line(
#line 67 "src/../include/sgccplots.sgh"

#line 67 "src/../include/sgccplots.sgh"

#line 67 "src/../include/sgccplots.sgh"
plot plt, 
#line 67 "src/../include/sgccplots.sgh"

#line 67 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 67 "src/../include/sgccplots.sgh"

#line 67 "src/../include/sgccplots.sgh"
std__list__template__double ys);

#line 69 "src/../include/sgccplots.sgh"

#line 69 "src/../include/sgccplots.sgh"
void line(
#line 69 "src/../include/sgccplots.sgh"

#line 69 "src/../include/sgccplots.sgh"

#line 69 "src/../include/sgccplots.sgh"
plot plt, 
#line 69 "src/../include/sgccplots.sgh"

#line 69 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 69 "src/../include/sgccplots.sgh"

#line 69 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 69 "src/../include/sgccplots.sgh"

#line 69 "src/../include/sgccplots.sgh"
plot_arg arg0);

#line 71 "src/../include/sgccplots.sgh"

#line 71 "src/../include/sgccplots.sgh"
void line(
#line 71 "src/../include/sgccplots.sgh"

#line 71 "src/../include/sgccplots.sgh"

#line 71 "src/../include/sgccplots.sgh"
plot plt, 
#line 71 "src/../include/sgccplots.sgh"

#line 71 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 71 "src/../include/sgccplots.sgh"

#line 71 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 71 "src/../include/sgccplots.sgh"

#line 71 "src/../include/sgccplots.sgh"
plot_arg_list args);

#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
void scatter(
#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
plot plt, 
#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
string style, 
#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
float radius, 
#line 73 "src/../include/sgccplots.sgh"

#line 73 "src/../include/sgccplots.sgh"
string color);

#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"
void scatter(
#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"
plot plt, 
#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"
string style, 
#line 75 "src/../include/sgccplots.sgh"

#line 75 "src/../include/sgccplots.sgh"
float radius);

#line 77 "src/../include/sgccplots.sgh"

#line 77 "src/../include/sgccplots.sgh"
void scatter(
#line 77 "src/../include/sgccplots.sgh"

#line 77 "src/../include/sgccplots.sgh"

#line 77 "src/../include/sgccplots.sgh"
plot plt, 
#line 77 "src/../include/sgccplots.sgh"

#line 77 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 77 "src/../include/sgccplots.sgh"

#line 77 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 77 "src/../include/sgccplots.sgh"

#line 77 "src/../include/sgccplots.sgh"
string style);

#line 79 "src/../include/sgccplots.sgh"

#line 79 "src/../include/sgccplots.sgh"
void scatter(
#line 79 "src/../include/sgccplots.sgh"

#line 79 "src/../include/sgccplots.sgh"

#line 79 "src/../include/sgccplots.sgh"
plot plt, 
#line 79 "src/../include/sgccplots.sgh"

#line 79 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 79 "src/../include/sgccplots.sgh"

#line 79 "src/../include/sgccplots.sgh"
std__list__template__double ys);

#line 81 "src/../include/sgccplots.sgh"

#line 81 "src/../include/sgccplots.sgh"
void scatter(
#line 81 "src/../include/sgccplots.sgh"

#line 81 "src/../include/sgccplots.sgh"

#line 81 "src/../include/sgccplots.sgh"
plot plt, 
#line 81 "src/../include/sgccplots.sgh"

#line 81 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 81 "src/../include/sgccplots.sgh"

#line 81 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 81 "src/../include/sgccplots.sgh"

#line 81 "src/../include/sgccplots.sgh"
plot_arg arg0);

#line 83 "src/../include/sgccplots.sgh"

#line 83 "src/../include/sgccplots.sgh"
void scatter(
#line 83 "src/../include/sgccplots.sgh"

#line 83 "src/../include/sgccplots.sgh"

#line 83 "src/../include/sgccplots.sgh"
plot plt, 
#line 83 "src/../include/sgccplots.sgh"

#line 83 "src/../include/sgccplots.sgh"
std__list__template__double xs, 
#line 83 "src/../include/sgccplots.sgh"

#line 83 "src/../include/sgccplots.sgh"
std__list__template__double ys, 
#line 83 "src/../include/sgccplots.sgh"

#line 83 "src/../include/sgccplots.sgh"
plot_arg_list args);

#line 88 "src/../include/sgccplots.sgh"

#line 88 "src/../include/sgccplots.sgh"
plot_arg gColor(
#line 88 "src/../include/sgccplots.sgh"

#line 88 "src/../include/sgccplots.sgh"

#line 88 "src/../include/sgccplots.sgh"
string color);

#line 90 "src/../include/sgccplots.sgh"

#line 90 "src/../include/sgccplots.sgh"
plot_arg gRadius(
#line 90 "src/../include/sgccplots.sgh"

#line 90 "src/../include/sgccplots.sgh"

#line 90 "src/../include/sgccplots.sgh"
float radius);

#line 92 "src/../include/sgccplots.sgh"

#line 92 "src/../include/sgccplots.sgh"
plot_arg gThickness(
#line 92 "src/../include/sgccplots.sgh"

#line 92 "src/../include/sgccplots.sgh"

#line 92 "src/../include/sgccplots.sgh"
float thickness);

#line 94 "src/../include/sgccplots.sgh"

#line 94 "src/../include/sgccplots.sgh"
plot_arg gStyle(
#line 94 "src/../include/sgccplots.sgh"

#line 94 "src/../include/sgccplots.sgh"

#line 94 "src/../include/sgccplots.sgh"
string style);

#line 96 "src/../include/sgccplots.sgh"

#line 96 "src/../include/sgccplots.sgh"
void y_axis(
#line 96 "src/../include/sgccplots.sgh"

#line 96 "src/../include/sgccplots.sgh"

#line 96 "src/../include/sgccplots.sgh"
plot plt, 
#line 96 "src/../include/sgccplots.sgh"

#line 96 "src/../include/sgccplots.sgh"
float radius, 
#line 96 "src/../include/sgccplots.sgh"

#line 96 "src/../include/sgccplots.sgh"
string color);

#line 98 "src/../include/sgccplots.sgh"

#line 98 "src/../include/sgccplots.sgh"
void x_axis(
#line 98 "src/../include/sgccplots.sgh"

#line 98 "src/../include/sgccplots.sgh"

#line 98 "src/../include/sgccplots.sgh"
plot plt, 
#line 98 "src/../include/sgccplots.sgh"

#line 98 "src/../include/sgccplots.sgh"
float radius, 
#line 98 "src/../include/sgccplots.sgh"

#line 98 "src/../include/sgccplots.sgh"
string color);

#line 100 "src/../include/sgccplots.sgh"

#line 100 "src/../include/sgccplots.sgh"
void axes(
#line 100 "src/../include/sgccplots.sgh"

#line 100 "src/../include/sgccplots.sgh"

#line 100 "src/../include/sgccplots.sgh"
plot plt, 
#line 100 "src/../include/sgccplots.sgh"

#line 100 "src/../include/sgccplots.sgh"
float radius, 
#line 100 "src/../include/sgccplots.sgh"

#line 100 "src/../include/sgccplots.sgh"
string color);

#line 102 "src/../include/sgccplots.sgh"

#line 102 "src/../include/sgccplots.sgh"
void y_axis(
#line 102 "src/../include/sgccplots.sgh"

#line 102 "src/../include/sgccplots.sgh"

#line 102 "src/../include/sgccplots.sgh"
plot plt, 
#line 102 "src/../include/sgccplots.sgh"

#line 102 "src/../include/sgccplots.sgh"
float radius);

#line 104 "src/../include/sgccplots.sgh"

#line 104 "src/../include/sgccplots.sgh"
void x_axis(
#line 104 "src/../include/sgccplots.sgh"

#line 104 "src/../include/sgccplots.sgh"

#line 104 "src/../include/sgccplots.sgh"
plot plt, 
#line 104 "src/../include/sgccplots.sgh"

#line 104 "src/../include/sgccplots.sgh"
float radius);

#line 106 "src/../include/sgccplots.sgh"

#line 106 "src/../include/sgccplots.sgh"
void axes(
#line 106 "src/../include/sgccplots.sgh"

#line 106 "src/../include/sgccplots.sgh"

#line 106 "src/../include/sgccplots.sgh"
plot plt, 
#line 106 "src/../include/sgccplots.sgh"

#line 106 "src/../include/sgccplots.sgh"
float radius);

#line 108 "src/../include/sgccplots.sgh"

#line 108 "src/../include/sgccplots.sgh"
void y_axis(
#line 108 "src/../include/sgccplots.sgh"

#line 108 "src/../include/sgccplots.sgh"

#line 108 "src/../include/sgccplots.sgh"
plot plt);

#line 110 "src/../include/sgccplots.sgh"

#line 110 "src/../include/sgccplots.sgh"
void x_axis(
#line 110 "src/../include/sgccplots.sgh"

#line 110 "src/../include/sgccplots.sgh"

#line 110 "src/../include/sgccplots.sgh"
plot plt);

#line 112 "src/../include/sgccplots.sgh"

#line 112 "src/../include/sgccplots.sgh"
void axes(
#line 112 "src/../include/sgccplots.sgh"

#line 112 "src/../include/sgccplots.sgh"

#line 112 "src/../include/sgccplots.sgh"
plot plt);

#line 13 "src/sgccplots.sgc"

#line 13 "src/sgccplots.sgc"
string color_cycle[] = {
#line 14 "src/sgccplots.sgc"

#line 14 "src/sgccplots.sgc"
"blue", 
#line 15 "src/sgccplots.sgc"
"orange", 
#line 16 "src/sgccplots.sgc"
"green", 
#line 17 "src/sgccplots.sgc"
"purple", 
#line 18 "src/sgccplots.sgc"
"skyblue", 
#line 19 "src/sgccplots.sgc"
"red", 
#line 20 "src/sgccplots.sgc"
"brown"};

#line 23 "src/sgccplots.sgc"

#line 23 "src/sgccplots.sgc"
int n_cycle = 
#line 23 "src/sgccplots.sgc"
7;

#line 25 "src/sgccplots.sgc"

#line 25 "src/sgccplots.sgc"
struct plot_struct
{
  
#line 26 "src/sgccplots.sgc"

#line 26 "src/sgccplots.sgc"
int screenWidth;
  
#line 27 "src/sgccplots.sgc"

#line 27 "src/sgccplots.sgc"
int screenHeight;
  
#line 28 "src/sgccplots.sgc"

#line 28 "src/sgccplots.sgc"
double x_start;
  
#line 29 "src/sgccplots.sgc"

#line 29 "src/sgccplots.sgc"
double y_start;
  
#line 30 "src/sgccplots.sgc"

#line 30 "src/sgccplots.sgc"
double dataWidth;
  
#line 31 "src/sgccplots.sgc"

#line 31 "src/sgccplots.sgc"
double dataHeight;
  
#line 32 "src/sgccplots.sgc"

#line 32 "src/sgccplots.sgc"
std__list__template__drawer drawers;
  
#line 33 "src/sgccplots.sgc"

#line 33 "src/sgccplots.sgc"
int color_cycle;
};

#line 36 "src/sgccplots.sgc"
typedef 
#line 36 "src/sgccplots.sgc"
struct plot_struct *plot;

#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
plot make_plot(
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
int screenWidth, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
int screenHeight, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
double x_start, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
double y_start, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
double x_end, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
double y_end, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
double padx, 
#line 38 "src/sgccplots.sgc"

#line 38 "src/sgccplots.sgc"
double pady)

#line 38 "src/sgccplots.sgc"
{
  
#line 39 "src/sgccplots.sgc"

#line 39 "src/sgccplots.sgc"
plot out = 
#line 39 "src/sgccplots.sgc"
(
#line 39 "src/sgccplots.sgc"
plot) 
#line 39 "src/sgccplots.sgc"
GC_malloc(
#line 39 "src/sgccplots.sgc"

#line 39 "src/sgccplots.sgc"
sizeof(
#line 0 "src/sgccplots.sgc"

#line 39 "src/sgccplots.sgc"
struct plot_struct));
  
#line 40 "src/sgccplots.sgc"

#line 40 "src/sgccplots.sgc"

#line 40 "src/sgccplots.sgc"
out->
#line 40 "src/sgccplots.sgc"
screenWidth = 
#line 40 "src/sgccplots.sgc"
screenWidth;
  
#line 41 "src/sgccplots.sgc"

#line 41 "src/sgccplots.sgc"

#line 41 "src/sgccplots.sgc"
out->
#line 41 "src/sgccplots.sgc"
screenHeight = 
#line 41 "src/sgccplots.sgc"
screenHeight;
  
#line 42 "src/sgccplots.sgc"

#line 42 "src/sgccplots.sgc"

#line 42 "src/sgccplots.sgc"
out->
#line 42 "src/sgccplots.sgc"
x_start = 
#line 42 "src/sgccplots.sgc"
x_start;
  
#line 43 "src/sgccplots.sgc"

#line 43 "src/sgccplots.sgc"

#line 43 "src/sgccplots.sgc"
out->
#line 43 "src/sgccplots.sgc"
y_start = 
#line 43 "src/sgccplots.sgc"
y_start;
  
#line 44 "src/sgccplots.sgc"

#line 44 "src/sgccplots.sgc"

#line 44 "src/sgccplots.sgc"
out->
#line 44 "src/sgccplots.sgc"
dataWidth = 
#line 44 "src/sgccplots.sgc"
__sub__(
#line 44 "src/sgccplots.sgc"

#line 44 "src/sgccplots.sgc"
x_end, 
#line 44 "src/sgccplots.sgc"
x_start);
  
#line 45 "src/sgccplots.sgc"

#line 45 "src/sgccplots.sgc"

#line 45 "src/sgccplots.sgc"
out->
#line 45 "src/sgccplots.sgc"
dataHeight = 
#line 45 "src/sgccplots.sgc"
__sub__(
#line 45 "src/sgccplots.sgc"

#line 45 "src/sgccplots.sgc"
y_end, 
#line 45 "src/sgccplots.sgc"
y_start);
  
#line 46 "src/sgccplots.sgc"

#line 46 "src/sgccplots.sgc"
double pad_width = 
#line 46 "src/sgccplots.sgc"
__mul__(
#line 46 "src/sgccplots.sgc"
__div__(
#line 46 "src/sgccplots.sgc"

#line 46 "src/sgccplots.sgc"

#line 46 "src/sgccplots.sgc"
out->
#line 46 "src/sgccplots.sgc"
dataWidth, 
#line 46 "src/sgccplots.sgc"
(
#line 46 "src/sgccplots.sgc"
double) 
#line 46 "src/sgccplots.sgc"

#line 46 "src/sgccplots.sgc"
out->
#line 46 "src/sgccplots.sgc"
screenWidth), 
#line 46 "src/sgccplots.sgc"
padx);
  
#line 47 "src/sgccplots.sgc"

#line 47 "src/sgccplots.sgc"
double pad_height = 
#line 47 "src/sgccplots.sgc"
__mul__(
#line 47 "src/sgccplots.sgc"
__div__(
#line 47 "src/sgccplots.sgc"

#line 47 "src/sgccplots.sgc"

#line 47 "src/sgccplots.sgc"
out->
#line 47 "src/sgccplots.sgc"
dataHeight, 
#line 47 "src/sgccplots.sgc"
(
#line 47 "src/sgccplots.sgc"
double) 
#line 47 "src/sgccplots.sgc"

#line 47 "src/sgccplots.sgc"
out->
#line 47 "src/sgccplots.sgc"
screenHeight), 
#line 47 "src/sgccplots.sgc"
pady);
  
#line 48 "src/sgccplots.sgc"

#line 48 "src/sgccplots.sgc"

#line 48 "src/sgccplots.sgc"
out->
#line 48 "src/sgccplots.sgc"
x_start = 
#line 0 "src/sgccplots.sgc"
__sub__(
#line 48 "src/sgccplots.sgc"

#line 48 "src/sgccplots.sgc"

#line 48 "src/sgccplots.sgc"
out->
#line 48 "src/sgccplots.sgc"
x_start, 
#line 48 "src/sgccplots.sgc"
pad_width);
  
#line 49 "src/sgccplots.sgc"

#line 49 "src/sgccplots.sgc"

#line 49 "src/sgccplots.sgc"
out->
#line 49 "src/sgccplots.sgc"
dataWidth = 
#line 0 "src/sgccplots.sgc"
__add__(
#line 49 "src/sgccplots.sgc"

#line 49 "src/sgccplots.sgc"

#line 49 "src/sgccplots.sgc"
out->
#line 49 "src/sgccplots.sgc"
dataWidth, 
#line 49 "src/sgccplots.sgc"
__mul__(
#line 49 "src/sgccplots.sgc"

#line 49 "src/sgccplots.sgc"
pad_width, 
#line 49 "src/sgccplots.sgc"
2));
  
#line 50 "src/sgccplots.sgc"

#line 50 "src/sgccplots.sgc"

#line 50 "src/sgccplots.sgc"
out->
#line 50 "src/sgccplots.sgc"
y_start = 
#line 0 "src/sgccplots.sgc"
__sub__(
#line 50 "src/sgccplots.sgc"

#line 50 "src/sgccplots.sgc"

#line 50 "src/sgccplots.sgc"
out->
#line 50 "src/sgccplots.sgc"
y_start, 
#line 50 "src/sgccplots.sgc"
pad_height);
  
#line 51 "src/sgccplots.sgc"

#line 51 "src/sgccplots.sgc"

#line 51 "src/sgccplots.sgc"
out->
#line 51 "src/sgccplots.sgc"
dataHeight = 
#line 0 "src/sgccplots.sgc"
__add__(
#line 51 "src/sgccplots.sgc"

#line 51 "src/sgccplots.sgc"

#line 51 "src/sgccplots.sgc"
out->
#line 51 "src/sgccplots.sgc"
dataHeight, 
#line 51 "src/sgccplots.sgc"
__mul__(
#line 51 "src/sgccplots.sgc"

#line 51 "src/sgccplots.sgc"
pad_height, 
#line 51 "src/sgccplots.sgc"
2));
  
#line 52 "src/sgccplots.sgc"

#line 52 "src/sgccplots.sgc"

#line 52 "src/sgccplots.sgc"
out->
#line 52 "src/sgccplots.sgc"
drawers = 
#line 52 "src/sgccplots.sgc"
std__make_list__template__drawer();
  
#line 53 "src/sgccplots.sgc"

#line 53 "src/sgccplots.sgc"

#line 53 "src/sgccplots.sgc"
out->
#line 53 "src/sgccplots.sgc"
color_cycle = 
#line 53 "src/sgccplots.sgc"
0;
  
#line 54 "src/sgccplots.sgc"
return 
#line 54 "src/sgccplots.sgc"
out;
}


#line 57 "src/sgccplots.sgc"

#line 57 "src/sgccplots.sgc"
inline static 
#line 57 "src/sgccplots.sgc"
float2 world2screen(
#line 57 "src/sgccplots.sgc"

#line 57 "src/sgccplots.sgc"

#line 57 "src/sgccplots.sgc"
plot plt, 
#line 57 "src/sgccplots.sgc"

#line 57 "src/sgccplots.sgc"
double x, 
#line 57 "src/sgccplots.sgc"

#line 57 "src/sgccplots.sgc"
double y)

#line 57 "src/sgccplots.sgc"
{
  
#line 58 "src/sgccplots.sgc"
return 
#line 58 "src/sgccplots.sgc"
make_float2(
#line 58 "src/sgccplots.sgc"
__mul__(
#line 58 "src/sgccplots.sgc"
__div__(
#line 58 "src/sgccplots.sgc"
__sub__(
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
x, 
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
plt->
#line 58 "src/sgccplots.sgc"
x_start), 
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
plt->
#line 58 "src/sgccplots.sgc"
dataWidth), 
#line 58 "src/sgccplots.sgc"
(
#line 58 "src/sgccplots.sgc"
double) 
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
plt->
#line 58 "src/sgccplots.sgc"
screenWidth), 
#line 58 "src/sgccplots.sgc"
__mul__(
#line 58 "src/sgccplots.sgc"
__sub__(
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
1, 
#line 58 "src/sgccplots.sgc"
__div__(
#line 58 "src/sgccplots.sgc"
__sub__(
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
y, 
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
plt->
#line 58 "src/sgccplots.sgc"
y_start), 
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
plt->
#line 58 "src/sgccplots.sgc"
dataHeight)), 
#line 58 "src/sgccplots.sgc"
(
#line 58 "src/sgccplots.sgc"
double) 
#line 58 "src/sgccplots.sgc"

#line 58 "src/sgccplots.sgc"
plt->
#line 58 "src/sgccplots.sgc"
screenHeight));
}


#line 61 "src/sgccplots.sgc"

#line 61 "src/sgccplots.sgc"
inline static 
#line 61 "src/sgccplots.sgc"
void init_window(
#line 61 "src/sgccplots.sgc"

#line 61 "src/sgccplots.sgc"

#line 61 "src/sgccplots.sgc"
plot plt)

#line 61 "src/sgccplots.sgc"
{
  
#line 62 "src/sgccplots.sgc"
InitWindow(
#line 62 "src/sgccplots.sgc"

#line 62 "src/sgccplots.sgc"

#line 62 "src/sgccplots.sgc"
plt->
#line 62 "src/sgccplots.sgc"
screenWidth, 
#line 62 "src/sgccplots.sgc"

#line 62 "src/sgccplots.sgc"
plt->
#line 62 "src/sgccplots.sgc"
screenHeight, 
#line 62 "src/sgccplots.sgc"
"plot");
  
#line 63 "src/sgccplots.sgc"
SetTargetFPS(
#line 63 "src/sgccplots.sgc"

#line 63 "src/sgccplots.sgc"
60);
}


#line 66 "src/sgccplots.sgc"

#line 66 "src/sgccplots.sgc"
inline static 
#line 66 "src/sgccplots.sgc"
void close(
#line 66 "src/sgccplots.sgc"

#line 66 "src/sgccplots.sgc"

#line 66 "src/sgccplots.sgc"
plot plt)

#line 66 "src/sgccplots.sgc"
{
  
#line 67 "src/sgccplots.sgc"
CloseWindow();
}


#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"
inline static 
#line 70 "src/sgccplots.sgc"
void draw_point(
#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"
plot plt, 
#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"
float2 pix, 
#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"
float radius_, 
#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"
Color color, 
#line 70 "src/sgccplots.sgc"

#line 70 "src/sgccplots.sgc"
string style)

#line 70 "src/sgccplots.sgc"
{
  
#line 71 "src/sgccplots.sgc"

#line 71 "src/sgccplots.sgc"
float radius = 
#line 71 "src/sgccplots.sgc"
__mul__(
#line 71 "src/sgccplots.sgc"
__div__(
#line 71 "src/sgccplots.sgc"

#line 71 "src/sgccplots.sgc"
radius_, 
#line 71 "src/sgccplots.sgc"

#line 71 "src/sgccplots.sgc"
plt->
#line 71 "src/sgccplots.sgc"
dataWidth), 
#line 71 "src/sgccplots.sgc"
(
#line 71 "src/sgccplots.sgc"
float) 
#line 71 "src/sgccplots.sgc"

#line 71 "src/sgccplots.sgc"
plt->
#line 71 "src/sgccplots.sgc"
screenWidth);
  
#line 72 "src/sgccplots.sgc"

#line 72 "src/sgccplots.sgc"
Vector2 point;
  
#line 73 "src/sgccplots.sgc"

#line 73 "src/sgccplots.sgc"

#line 73 "src/sgccplots.sgc"
point.
#line 73 "src/sgccplots.sgc"
x = 
#line 73 "src/sgccplots.sgc"

#line 73 "src/sgccplots.sgc"
pix.
#line 73 "src/sgccplots.sgc"
x;
  
#line 74 "src/sgccplots.sgc"

#line 74 "src/sgccplots.sgc"

#line 74 "src/sgccplots.sgc"
point.
#line 74 "src/sgccplots.sgc"
y = 
#line 74 "src/sgccplots.sgc"

#line 74 "src/sgccplots.sgc"
pix.
#line 74 "src/sgccplots.sgc"
y;
  
#line 75 "src/sgccplots.sgc"
if (
#line 75 "src/sgccplots.sgc"
__eq__(
#line 75 "src/sgccplots.sgc"

#line 75 "src/sgccplots.sgc"
style, 
#line 75 "src/sgccplots.sgc"
"o"))

#line 75 "src/sgccplots.sgc"
  {
    
#line 76 "src/sgccplots.sgc"
DrawCircleV(
#line 76 "src/sgccplots.sgc"

#line 76 "src/sgccplots.sgc"
point, 
#line 76 "src/sgccplots.sgc"
radius, 
#line 76 "src/sgccplots.sgc"
color);
    
#line 77 "src/sgccplots.sgc"
return;
  }
  
#line 79 "src/sgccplots.sgc"
if (
#line 79 "src/sgccplots.sgc"
__eq__(
#line 79 "src/sgccplots.sgc"

#line 79 "src/sgccplots.sgc"
style, 
#line 79 "src/sgccplots.sgc"
"."))

#line 79 "src/sgccplots.sgc"
  {
    
#line 80 "src/sgccplots.sgc"
DrawPixel(
#line 80 "src/sgccplots.sgc"

#line 80 "src/sgccplots.sgc"

#line 80 "src/sgccplots.sgc"
point.
#line 80 "src/sgccplots.sgc"
x, 
#line 80 "src/sgccplots.sgc"

#line 80 "src/sgccplots.sgc"
point.
#line 80 "src/sgccplots.sgc"
y, 
#line 80 "src/sgccplots.sgc"
color);
    
#line 81 "src/sgccplots.sgc"
return;
  }
}


#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
inline static 
#line 86 "src/sgccplots.sgc"
void draw_points(
#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
plot plt, 
#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
std__list__template__double x, 
#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
std__list__template__double y, 
#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
float radius, 
#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
Color color, 
#line 86 "src/sgccplots.sgc"

#line 86 "src/sgccplots.sgc"
string style)

#line 86 "src/sgccplots.sgc"
{
  
#line 87 "src/sgccplots.sgc"
if (!
#line 87 "src/sgccplots.sgc"
__eq__(
#line 87 "src/sgccplots.sgc"
len(
#line 87 "src/sgccplots.sgc"

#line 87 "src/sgccplots.sgc"
x), 
#line 87 "src/sgccplots.sgc"
len(
#line 87 "src/sgccplots.sgc"

#line 87 "src/sgccplots.sgc"
y)))

#line 87 "src/sgccplots.sgc"
  {
    
#line 87 "src/sgccplots.sgc"
printf(
#line 87 "src/sgccplots.sgc"

#line 87 "src/sgccplots.sgc"
"%s: FAILED\n", 
#line 87 "src/sgccplots.sgc"
"len(x) == len(y)");
    
#line 87 "src/sgccplots.sgc"
exit(
#line 87 "src/sgccplots.sgc"

#line 87 "src/sgccplots.sgc"
1);
  }
  
#line 87 "src/sgccplots.sgc"
;
  
#line 88 "src/sgccplots.sgc"
for (
#line 88 "src/sgccplots.sgc"

#line 88 "src/sgccplots.sgc"

#line 88 "src/sgccplots.sgc"
int i = 
#line 88 "src/sgccplots.sgc"
0; 
#line 88 "src/sgccplots.sgc"
__lt__(
#line 88 "src/sgccplots.sgc"

#line 88 "src/sgccplots.sgc"
i, 
#line 88 "src/sgccplots.sgc"
len(
#line 88 "src/sgccplots.sgc"

#line 88 "src/sgccplots.sgc"
x)); 
#line 88 "src/sgccplots.sgc"

#line 88 "src/sgccplots.sgc"
i++)

#line 88 "src/sgccplots.sgc"
  {
    
#line 89 "src/sgccplots.sgc"
draw_point(
#line 89 "src/sgccplots.sgc"
plt, 
#line 89 "src/sgccplots.sgc"
world2screen(
#line 89 "src/sgccplots.sgc"
plt, 
#line 89 "src/sgccplots.sgc"
__index_wrapper__(
#line 89 "src/sgccplots.sgc"

#line 89 "src/sgccplots.sgc"
x, 
#line 89 "src/sgccplots.sgc"
i), 
#line 89 "src/sgccplots.sgc"
__index_wrapper__(
#line 89 "src/sgccplots.sgc"

#line 89 "src/sgccplots.sgc"
y, 
#line 89 "src/sgccplots.sgc"
i)), 
#line 89 "src/sgccplots.sgc"
radius, 
#line 89 "src/sgccplots.sgc"
color, 
#line 89 "src/sgccplots.sgc"
style);
  }

}


#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"
inline static 
#line 93 "src/sgccplots.sgc"
void draw_line_segment(
#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"
plot plt, 
#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"
float2 start_, 
#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"
float2 end_, 
#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"
float thickness_, 
#line 93 "src/sgccplots.sgc"

#line 93 "src/sgccplots.sgc"
Color color)

#line 93 "src/sgccplots.sgc"
{
  
#line 94 "src/sgccplots.sgc"

#line 94 "src/sgccplots.sgc"
float thickness = 
#line 94 "src/sgccplots.sgc"
__mul__(
#line 94 "src/sgccplots.sgc"
__div__(
#line 94 "src/sgccplots.sgc"

#line 94 "src/sgccplots.sgc"
thickness_, 
#line 94 "src/sgccplots.sgc"

#line 94 "src/sgccplots.sgc"
plt->
#line 94 "src/sgccplots.sgc"
dataWidth), 
#line 94 "src/sgccplots.sgc"
(
#line 94 "src/sgccplots.sgc"
float) 
#line 94 "src/sgccplots.sgc"

#line 94 "src/sgccplots.sgc"
plt->
#line 94 "src/sgccplots.sgc"
screenWidth);
  
#line 95 "src/sgccplots.sgc"

#line 95 "src/sgccplots.sgc"
Vector2 start;
  
#line 96 "src/sgccplots.sgc"

#line 96 "src/sgccplots.sgc"

#line 96 "src/sgccplots.sgc"
start.
#line 96 "src/sgccplots.sgc"
x = 
#line 96 "src/sgccplots.sgc"

#line 96 "src/sgccplots.sgc"
start_.
#line 96 "src/sgccplots.sgc"
x;
  
#line 97 "src/sgccplots.sgc"

#line 97 "src/sgccplots.sgc"

#line 97 "src/sgccplots.sgc"
start.
#line 97 "src/sgccplots.sgc"
y = 
#line 97 "src/sgccplots.sgc"

#line 97 "src/sgccplots.sgc"
start_.
#line 97 "src/sgccplots.sgc"
y;
  
#line 98 "src/sgccplots.sgc"

#line 98 "src/sgccplots.sgc"
Vector2 end;
  
#line 99 "src/sgccplots.sgc"

#line 99 "src/sgccplots.sgc"

#line 99 "src/sgccplots.sgc"
end.
#line 99 "src/sgccplots.sgc"
x = 
#line 99 "src/sgccplots.sgc"

#line 99 "src/sgccplots.sgc"
end_.
#line 99 "src/sgccplots.sgc"
x;
  
#line 100 "src/sgccplots.sgc"

#line 100 "src/sgccplots.sgc"

#line 100 "src/sgccplots.sgc"
end.
#line 100 "src/sgccplots.sgc"
y = 
#line 100 "src/sgccplots.sgc"

#line 100 "src/sgccplots.sgc"
end_.
#line 100 "src/sgccplots.sgc"
y;
  
#line 101 "src/sgccplots.sgc"
DrawLineEx(
#line 101 "src/sgccplots.sgc"

#line 101 "src/sgccplots.sgc"
start, 
#line 101 "src/sgccplots.sgc"
end, 
#line 101 "src/sgccplots.sgc"
thickness, 
#line 101 "src/sgccplots.sgc"
color);
}


#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"
inline static 
#line 104 "src/sgccplots.sgc"
void draw_line(
#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"
plot plt, 
#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"
std__list__template__double x, 
#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"
std__list__template__double y, 
#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"
float thickness, 
#line 104 "src/sgccplots.sgc"

#line 104 "src/sgccplots.sgc"
Color color)

#line 104 "src/sgccplots.sgc"
{
  
#line 105 "src/sgccplots.sgc"
if (!
#line 105 "src/sgccplots.sgc"
__eq__(
#line 105 "src/sgccplots.sgc"
len(
#line 105 "src/sgccplots.sgc"

#line 105 "src/sgccplots.sgc"
x), 
#line 105 "src/sgccplots.sgc"
len(
#line 105 "src/sgccplots.sgc"

#line 105 "src/sgccplots.sgc"
y)))

#line 105 "src/sgccplots.sgc"
  {
    
#line 105 "src/sgccplots.sgc"
printf(
#line 105 "src/sgccplots.sgc"

#line 105 "src/sgccplots.sgc"
"%s: FAILED\n", 
#line 105 "src/sgccplots.sgc"
"len(x) == len(y)");
    
#line 105 "src/sgccplots.sgc"
exit(
#line 105 "src/sgccplots.sgc"

#line 105 "src/sgccplots.sgc"
1);
  }
  
#line 105 "src/sgccplots.sgc"
;
  
#line 106 "src/sgccplots.sgc"
for (
#line 106 "src/sgccplots.sgc"

#line 106 "src/sgccplots.sgc"

#line 106 "src/sgccplots.sgc"
int i = 
#line 106 "src/sgccplots.sgc"
0; 
#line 106 "src/sgccplots.sgc"
__lt__(
#line 106 "src/sgccplots.sgc"

#line 106 "src/sgccplots.sgc"
i, 
#line 106 "src/sgccplots.sgc"
__sub__(
#line 106 "src/sgccplots.sgc"
len(
#line 106 "src/sgccplots.sgc"

#line 106 "src/sgccplots.sgc"
x), 
#line 106 "src/sgccplots.sgc"
1)); 
#line 106 "src/sgccplots.sgc"

#line 106 "src/sgccplots.sgc"
i++)

#line 106 "src/sgccplots.sgc"
  {
    
#line 107 "src/sgccplots.sgc"
draw_line_segment(
#line 107 "src/sgccplots.sgc"
plt, 
#line 107 "src/sgccplots.sgc"
world2screen(
#line 107 "src/sgccplots.sgc"
plt, 
#line 107 "src/sgccplots.sgc"
__index_wrapper__(
#line 107 "src/sgccplots.sgc"

#line 107 "src/sgccplots.sgc"
x, 
#line 107 "src/sgccplots.sgc"
i), 
#line 107 "src/sgccplots.sgc"
__index_wrapper__(
#line 107 "src/sgccplots.sgc"

#line 107 "src/sgccplots.sgc"
y, 
#line 107 "src/sgccplots.sgc"
i)), 
#line 107 "src/sgccplots.sgc"
world2screen(
#line 107 "src/sgccplots.sgc"
plt, 
#line 107 "src/sgccplots.sgc"
__index_wrapper__(
#line 107 "src/sgccplots.sgc"
x, 
#line 107 "src/sgccplots.sgc"
__add__(
#line 107 "src/sgccplots.sgc"

#line 107 "src/sgccplots.sgc"
i, 
#line 107 "src/sgccplots.sgc"
1)), 
#line 107 "src/sgccplots.sgc"
__index_wrapper__(
#line 107 "src/sgccplots.sgc"
y, 
#line 107 "src/sgccplots.sgc"
__add__(
#line 107 "src/sgccplots.sgc"

#line 107 "src/sgccplots.sgc"
i, 
#line 107 "src/sgccplots.sgc"
1))), 
#line 107 "src/sgccplots.sgc"
thickness, 
#line 107 "src/sgccplots.sgc"
color);
  }

}


#line 111 "src/sgccplots.sgc"

#line 111 "src/sgccplots.sgc"
inline static 
#line 111 "src/sgccplots.sgc"
void draw_y_axis(
#line 111 "src/sgccplots.sgc"

#line 111 "src/sgccplots.sgc"

#line 111 "src/sgccplots.sgc"
plot plt, 
#line 111 "src/sgccplots.sgc"

#line 111 "src/sgccplots.sgc"
float thickness, 
#line 111 "src/sgccplots.sgc"

#line 111 "src/sgccplots.sgc"
Color color)

#line 111 "src/sgccplots.sgc"
{
  
#line 112 "src/sgccplots.sgc"

#line 112 "src/sgccplots.sgc"
float2 zero = 
#line 112 "src/sgccplots.sgc"
world2screen(
#line 112 "src/sgccplots.sgc"

#line 112 "src/sgccplots.sgc"
plt, 
#line 112 "src/sgccplots.sgc"
0, 
#line 112 "src/sgccplots.sgc"
0);
  
#line 113 "src/sgccplots.sgc"

#line 113 "src/sgccplots.sgc"
float2 start = 
#line 113 "src/sgccplots.sgc"
make_float2(
#line 113 "src/sgccplots.sgc"

#line 113 "src/sgccplots.sgc"

#line 113 "src/sgccplots.sgc"
zero.
#line 113 "src/sgccplots.sgc"
x, 
#line 113 "src/sgccplots.sgc"
0);
  
#line 114 "src/sgccplots.sgc"

#line 114 "src/sgccplots.sgc"
float2 end = 
#line 114 "src/sgccplots.sgc"
make_float2(
#line 114 "src/sgccplots.sgc"

#line 114 "src/sgccplots.sgc"

#line 114 "src/sgccplots.sgc"
zero.
#line 114 "src/sgccplots.sgc"
x, 
#line 114 "src/sgccplots.sgc"

#line 114 "src/sgccplots.sgc"
plt->
#line 114 "src/sgccplots.sgc"
screenHeight);
  
#line 115 "src/sgccplots.sgc"
draw_line_segment(
#line 115 "src/sgccplots.sgc"

#line 115 "src/sgccplots.sgc"
plt, 
#line 115 "src/sgccplots.sgc"
start, 
#line 115 "src/sgccplots.sgc"
end, 
#line 115 "src/sgccplots.sgc"
thickness, 
#line 115 "src/sgccplots.sgc"
color);
}


#line 118 "src/sgccplots.sgc"

#line 118 "src/sgccplots.sgc"
inline static 
#line 118 "src/sgccplots.sgc"
void draw_x_axis(
#line 118 "src/sgccplots.sgc"

#line 118 "src/sgccplots.sgc"

#line 118 "src/sgccplots.sgc"
plot plt, 
#line 118 "src/sgccplots.sgc"

#line 118 "src/sgccplots.sgc"
float thickness, 
#line 118 "src/sgccplots.sgc"

#line 118 "src/sgccplots.sgc"
Color color)

#line 118 "src/sgccplots.sgc"
{
  
#line 119 "src/sgccplots.sgc"

#line 119 "src/sgccplots.sgc"
float2 zero = 
#line 119 "src/sgccplots.sgc"
world2screen(
#line 119 "src/sgccplots.sgc"

#line 119 "src/sgccplots.sgc"
plt, 
#line 119 "src/sgccplots.sgc"
0, 
#line 119 "src/sgccplots.sgc"
0);
  
#line 120 "src/sgccplots.sgc"

#line 120 "src/sgccplots.sgc"
float2 start = 
#line 120 "src/sgccplots.sgc"
make_float2(
#line 120 "src/sgccplots.sgc"

#line 120 "src/sgccplots.sgc"
0, 
#line 120 "src/sgccplots.sgc"

#line 120 "src/sgccplots.sgc"
zero.
#line 120 "src/sgccplots.sgc"
y);
  
#line 121 "src/sgccplots.sgc"

#line 121 "src/sgccplots.sgc"
float2 end = 
#line 121 "src/sgccplots.sgc"
make_float2(
#line 121 "src/sgccplots.sgc"

#line 121 "src/sgccplots.sgc"

#line 121 "src/sgccplots.sgc"
plt->
#line 121 "src/sgccplots.sgc"
screenWidth, 
#line 121 "src/sgccplots.sgc"

#line 121 "src/sgccplots.sgc"
zero.
#line 121 "src/sgccplots.sgc"
y);
  
#line 122 "src/sgccplots.sgc"
draw_line_segment(
#line 122 "src/sgccplots.sgc"

#line 122 "src/sgccplots.sgc"
plt, 
#line 122 "src/sgccplots.sgc"
start, 
#line 122 "src/sgccplots.sgc"
end, 
#line 122 "src/sgccplots.sgc"
thickness, 
#line 122 "src/sgccplots.sgc"
color);
}


#line 125 "src/sgccplots.sgc"

#line 125 "src/sgccplots.sgc"
struct line_drawer_struct
{
  
#line 126 "src/sgccplots.sgc"

#line 126 "src/sgccplots.sgc"
std__list__template__double xs;
  
#line 127 "src/sgccplots.sgc"

#line 127 "src/sgccplots.sgc"
std__list__template__double ys;
  
#line 128 "src/sgccplots.sgc"

#line 128 "src/sgccplots.sgc"
float thickness;
  
#line 129 "src/sgccplots.sgc"

#line 129 "src/sgccplots.sgc"
Color color;
};

#line 132 "src/sgccplots.sgc"

#line 132 "src/sgccplots.sgc"
struct scatter_drawer_struct
{
  
#line 133 "src/sgccplots.sgc"

#line 133 "src/sgccplots.sgc"
std__list__template__double xs;
  
#line 134 "src/sgccplots.sgc"

#line 134 "src/sgccplots.sgc"
std__list__template__double ys;
  
#line 135 "src/sgccplots.sgc"

#line 135 "src/sgccplots.sgc"
float radius;
  
#line 136 "src/sgccplots.sgc"

#line 136 "src/sgccplots.sgc"
Color color;
  
#line 137 "src/sgccplots.sgc"

#line 137 "src/sgccplots.sgc"
string style;
};

#line 140 "src/sgccplots.sgc"

#line 140 "src/sgccplots.sgc"
struct x_axis_drawer_struct
{
  
#line 141 "src/sgccplots.sgc"

#line 141 "src/sgccplots.sgc"
float thickness;
  
#line 142 "src/sgccplots.sgc"

#line 142 "src/sgccplots.sgc"
Color color;
};

#line 145 "src/sgccplots.sgc"

#line 145 "src/sgccplots.sgc"
struct y_axis_drawer_struct
{
  
#line 146 "src/sgccplots.sgc"

#line 146 "src/sgccplots.sgc"
float thickness;
  
#line 147 "src/sgccplots.sgc"

#line 147 "src/sgccplots.sgc"
Color color;
};

#line 150 "src/sgccplots.sgc"

#line 150 "src/sgccplots.sgc"
enum drawer_t
{

#line 151 "src/sgccplots.sgc"
  LINE_DRAWER,

#line 152 "src/sgccplots.sgc"
  SCATTER_DRAWER,

#line 153 "src/sgccplots.sgc"
  X_AXIS_DRAWER,

#line 154 "src/sgccplots.sgc"
  Y_AXIS_DRAWER
};

#line 157 "src/sgccplots.sgc"

#line 157 "src/sgccplots.sgc"
struct drawer_struct
{
  
#line 158 "src/sgccplots.sgc"

#line 158 "src/sgccplots.sgc"
enum drawer_t t;
  
#line 164 "src/sgccplots.sgc"

#line 159 "src/sgccplots.sgc"
union 
  {
    
#line 160 "src/sgccplots.sgc"

#line 160 "src/sgccplots.sgc"
struct line_drawer_struct line_drawer;
    
#line 161 "src/sgccplots.sgc"

#line 161 "src/sgccplots.sgc"
struct scatter_drawer_struct scatter_drawer;
    
#line 162 "src/sgccplots.sgc"

#line 162 "src/sgccplots.sgc"
struct x_axis_drawer_struct x_axis_drawer;
    
#line 163 "src/sgccplots.sgc"

#line 163 "src/sgccplots.sgc"
struct y_axis_drawer_struct y_axis_drawer;
  } drawers;
};

#line 167 "src/sgccplots.sgc"

#line 167 "src/sgccplots.sgc"
inline static 
#line 167 "src/sgccplots.sgc"
drawer make_line_drawer(
#line 167 "src/sgccplots.sgc"

#line 167 "src/sgccplots.sgc"

#line 167 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 167 "src/sgccplots.sgc"

#line 167 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 167 "src/sgccplots.sgc"

#line 167 "src/sgccplots.sgc"
float thickness, 
#line 167 "src/sgccplots.sgc"

#line 167 "src/sgccplots.sgc"
Color color)

#line 167 "src/sgccplots.sgc"
{
  
#line 168 "src/sgccplots.sgc"

#line 168 "src/sgccplots.sgc"
struct line_drawer_struct ldraw;
  
#line 169 "src/sgccplots.sgc"

#line 169 "src/sgccplots.sgc"

#line 169 "src/sgccplots.sgc"
ldraw.
#line 169 "src/sgccplots.sgc"
xs = 
#line 169 "src/sgccplots.sgc"
xs;
  
#line 170 "src/sgccplots.sgc"

#line 170 "src/sgccplots.sgc"

#line 170 "src/sgccplots.sgc"
ldraw.
#line 170 "src/sgccplots.sgc"
ys = 
#line 170 "src/sgccplots.sgc"
ys;
  
#line 171 "src/sgccplots.sgc"

#line 171 "src/sgccplots.sgc"

#line 171 "src/sgccplots.sgc"
ldraw.
#line 171 "src/sgccplots.sgc"
thickness = 
#line 171 "src/sgccplots.sgc"
thickness;
  
#line 172 "src/sgccplots.sgc"

#line 172 "src/sgccplots.sgc"

#line 172 "src/sgccplots.sgc"
ldraw.
#line 172 "src/sgccplots.sgc"
color = 
#line 172 "src/sgccplots.sgc"
color;
  
#line 173 "src/sgccplots.sgc"

#line 173 "src/sgccplots.sgc"
drawer out = 
#line 173 "src/sgccplots.sgc"
(
#line 173 "src/sgccplots.sgc"
drawer) 
#line 173 "src/sgccplots.sgc"
GC_malloc(
#line 173 "src/sgccplots.sgc"

#line 173 "src/sgccplots.sgc"
sizeof(
#line 0 "src/sgccplots.sgc"

#line 173 "src/sgccplots.sgc"
struct drawer_struct));
  
#line 174 "src/sgccplots.sgc"

#line 174 "src/sgccplots.sgc"

#line 174 "src/sgccplots.sgc"
out->
#line 174 "src/sgccplots.sgc"
t = 
#line 174 "src/sgccplots.sgc"
LINE_DRAWER;
  
#line 175 "src/sgccplots.sgc"

#line 175 "src/sgccplots.sgc"

#line 175 "src/sgccplots.sgc"

#line 175 "src/sgccplots.sgc"
out->
#line 175 "src/sgccplots.sgc"
drawers.
#line 175 "src/sgccplots.sgc"
line_drawer = 
#line 175 "src/sgccplots.sgc"
ldraw;
  
#line 176 "src/sgccplots.sgc"
return 
#line 176 "src/sgccplots.sgc"
out;
}


#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"
inline static 
#line 179 "src/sgccplots.sgc"
drawer make_scatter_drawer(
#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"
float radius, 
#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"
Color color, 
#line 179 "src/sgccplots.sgc"

#line 179 "src/sgccplots.sgc"
string style)

#line 179 "src/sgccplots.sgc"
{
  
#line 180 "src/sgccplots.sgc"

#line 180 "src/sgccplots.sgc"
struct scatter_drawer_struct ldraw;
  
#line 181 "src/sgccplots.sgc"

#line 181 "src/sgccplots.sgc"

#line 181 "src/sgccplots.sgc"
ldraw.
#line 181 "src/sgccplots.sgc"
xs = 
#line 181 "src/sgccplots.sgc"
xs;
  
#line 182 "src/sgccplots.sgc"

#line 182 "src/sgccplots.sgc"

#line 182 "src/sgccplots.sgc"
ldraw.
#line 182 "src/sgccplots.sgc"
ys = 
#line 182 "src/sgccplots.sgc"
ys;
  
#line 183 "src/sgccplots.sgc"

#line 183 "src/sgccplots.sgc"

#line 183 "src/sgccplots.sgc"
ldraw.
#line 183 "src/sgccplots.sgc"
radius = 
#line 183 "src/sgccplots.sgc"
radius;
  
#line 184 "src/sgccplots.sgc"

#line 184 "src/sgccplots.sgc"

#line 184 "src/sgccplots.sgc"
ldraw.
#line 184 "src/sgccplots.sgc"
color = 
#line 184 "src/sgccplots.sgc"
color;
  
#line 185 "src/sgccplots.sgc"

#line 185 "src/sgccplots.sgc"

#line 185 "src/sgccplots.sgc"
ldraw.
#line 185 "src/sgccplots.sgc"
style = 
#line 185 "src/sgccplots.sgc"
style;
  
#line 186 "src/sgccplots.sgc"

#line 186 "src/sgccplots.sgc"
drawer out = 
#line 186 "src/sgccplots.sgc"
(
#line 186 "src/sgccplots.sgc"
drawer) 
#line 186 "src/sgccplots.sgc"
GC_malloc(
#line 186 "src/sgccplots.sgc"

#line 186 "src/sgccplots.sgc"
sizeof(
#line 0 "src/sgccplots.sgc"

#line 186 "src/sgccplots.sgc"
struct drawer_struct));
  
#line 187 "src/sgccplots.sgc"

#line 187 "src/sgccplots.sgc"

#line 187 "src/sgccplots.sgc"
out->
#line 187 "src/sgccplots.sgc"
t = 
#line 187 "src/sgccplots.sgc"
SCATTER_DRAWER;
  
#line 188 "src/sgccplots.sgc"

#line 188 "src/sgccplots.sgc"

#line 188 "src/sgccplots.sgc"

#line 188 "src/sgccplots.sgc"
out->
#line 188 "src/sgccplots.sgc"
drawers.
#line 188 "src/sgccplots.sgc"
scatter_drawer = 
#line 188 "src/sgccplots.sgc"
ldraw;
  
#line 189 "src/sgccplots.sgc"
return 
#line 189 "src/sgccplots.sgc"
out;
}


#line 192 "src/sgccplots.sgc"

#line 192 "src/sgccplots.sgc"
inline static 
#line 192 "src/sgccplots.sgc"
drawer make_x_axis_drawer(
#line 192 "src/sgccplots.sgc"

#line 192 "src/sgccplots.sgc"

#line 192 "src/sgccplots.sgc"
float thickness, 
#line 192 "src/sgccplots.sgc"

#line 192 "src/sgccplots.sgc"
Color color)

#line 192 "src/sgccplots.sgc"
{
  
#line 193 "src/sgccplots.sgc"

#line 193 "src/sgccplots.sgc"
struct x_axis_drawer_struct ldraw;
  
#line 194 "src/sgccplots.sgc"

#line 194 "src/sgccplots.sgc"

#line 194 "src/sgccplots.sgc"
ldraw.
#line 194 "src/sgccplots.sgc"
thickness = 
#line 194 "src/sgccplots.sgc"
thickness;
  
#line 195 "src/sgccplots.sgc"

#line 195 "src/sgccplots.sgc"

#line 195 "src/sgccplots.sgc"
ldraw.
#line 195 "src/sgccplots.sgc"
color = 
#line 195 "src/sgccplots.sgc"
color;
  
#line 196 "src/sgccplots.sgc"

#line 196 "src/sgccplots.sgc"
drawer out = 
#line 196 "src/sgccplots.sgc"
(
#line 196 "src/sgccplots.sgc"
drawer) 
#line 196 "src/sgccplots.sgc"
GC_malloc(
#line 196 "src/sgccplots.sgc"

#line 196 "src/sgccplots.sgc"
sizeof(
#line 0 "src/sgccplots.sgc"

#line 196 "src/sgccplots.sgc"
struct drawer_struct));
  
#line 197 "src/sgccplots.sgc"

#line 197 "src/sgccplots.sgc"

#line 197 "src/sgccplots.sgc"
out->
#line 197 "src/sgccplots.sgc"
t = 
#line 197 "src/sgccplots.sgc"
X_AXIS_DRAWER;
  
#line 198 "src/sgccplots.sgc"

#line 198 "src/sgccplots.sgc"

#line 198 "src/sgccplots.sgc"

#line 198 "src/sgccplots.sgc"
out->
#line 198 "src/sgccplots.sgc"
drawers.
#line 198 "src/sgccplots.sgc"
x_axis_drawer = 
#line 198 "src/sgccplots.sgc"
ldraw;
  
#line 199 "src/sgccplots.sgc"
return 
#line 199 "src/sgccplots.sgc"
out;
}


#line 202 "src/sgccplots.sgc"

#line 202 "src/sgccplots.sgc"
inline static 
#line 202 "src/sgccplots.sgc"
drawer make_y_axis_drawer(
#line 202 "src/sgccplots.sgc"

#line 202 "src/sgccplots.sgc"

#line 202 "src/sgccplots.sgc"
float thickness, 
#line 202 "src/sgccplots.sgc"

#line 202 "src/sgccplots.sgc"
Color color)

#line 202 "src/sgccplots.sgc"
{
  
#line 203 "src/sgccplots.sgc"

#line 203 "src/sgccplots.sgc"
struct y_axis_drawer_struct ldraw;
  
#line 204 "src/sgccplots.sgc"

#line 204 "src/sgccplots.sgc"

#line 204 "src/sgccplots.sgc"
ldraw.
#line 204 "src/sgccplots.sgc"
thickness = 
#line 204 "src/sgccplots.sgc"
thickness;
  
#line 205 "src/sgccplots.sgc"

#line 205 "src/sgccplots.sgc"

#line 205 "src/sgccplots.sgc"
ldraw.
#line 205 "src/sgccplots.sgc"
color = 
#line 205 "src/sgccplots.sgc"
color;
  
#line 206 "src/sgccplots.sgc"

#line 206 "src/sgccplots.sgc"
drawer out = 
#line 206 "src/sgccplots.sgc"
(
#line 206 "src/sgccplots.sgc"
drawer) 
#line 206 "src/sgccplots.sgc"
GC_malloc(
#line 206 "src/sgccplots.sgc"

#line 206 "src/sgccplots.sgc"
sizeof(
#line 0 "src/sgccplots.sgc"

#line 206 "src/sgccplots.sgc"
struct drawer_struct));
  
#line 207 "src/sgccplots.sgc"

#line 207 "src/sgccplots.sgc"

#line 207 "src/sgccplots.sgc"
out->
#line 207 "src/sgccplots.sgc"
t = 
#line 207 "src/sgccplots.sgc"
Y_AXIS_DRAWER;
  
#line 208 "src/sgccplots.sgc"

#line 208 "src/sgccplots.sgc"

#line 208 "src/sgccplots.sgc"

#line 208 "src/sgccplots.sgc"
out->
#line 208 "src/sgccplots.sgc"
drawers.
#line 208 "src/sgccplots.sgc"
y_axis_drawer = 
#line 208 "src/sgccplots.sgc"
ldraw;
  
#line 209 "src/sgccplots.sgc"
return 
#line 209 "src/sgccplots.sgc"
out;
}


#line 212 "src/sgccplots.sgc"

#line 212 "src/sgccplots.sgc"
inline static 
#line 212 "src/sgccplots.sgc"
void draw_elem(
#line 212 "src/sgccplots.sgc"

#line 212 "src/sgccplots.sgc"

#line 212 "src/sgccplots.sgc"
plot plt, 
#line 212 "src/sgccplots.sgc"

#line 212 "src/sgccplots.sgc"
drawer d)

#line 212 "src/sgccplots.sgc"
{
  
#line 213 "src/sgccplots.sgc"
switch (
#line 213 "src/sgccplots.sgc"

#line 213 "src/sgccplots.sgc"
d->
#line 213 "src/sgccplots.sgc"
t)

#line 213 "src/sgccplots.sgc"
  {
    
#line 214 "src/sgccplots.sgc"
case 
#line 214 "src/sgccplots.sgc"
LINE_DRAWER:
      
#line 215 "src/sgccplots.sgc"
draw_line(
#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"
plt, 
#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"
d->
#line 215 "src/sgccplots.sgc"
drawers.
#line 215 "src/sgccplots.sgc"
line_drawer.
#line 215 "src/sgccplots.sgc"
xs, 
#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"
d->
#line 215 "src/sgccplots.sgc"
drawers.
#line 215 "src/sgccplots.sgc"
line_drawer.
#line 215 "src/sgccplots.sgc"
ys, 
#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"
d->
#line 215 "src/sgccplots.sgc"
drawers.
#line 215 "src/sgccplots.sgc"
line_drawer.
#line 215 "src/sgccplots.sgc"
thickness, 
#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"

#line 215 "src/sgccplots.sgc"
d->
#line 215 "src/sgccplots.sgc"
drawers.
#line 215 "src/sgccplots.sgc"
line_drawer.
#line 215 "src/sgccplots.sgc"
color);
      
#line 216 "src/sgccplots.sgc"
break;

    
#line 217 "src/sgccplots.sgc"
case 
#line 217 "src/sgccplots.sgc"
SCATTER_DRAWER:
      
#line 218 "src/sgccplots.sgc"
draw_points(
#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"
plt, 
#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"
d->
#line 218 "src/sgccplots.sgc"
drawers.
#line 218 "src/sgccplots.sgc"
scatter_drawer.
#line 218 "src/sgccplots.sgc"
xs, 
#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"
d->
#line 218 "src/sgccplots.sgc"
drawers.
#line 218 "src/sgccplots.sgc"
scatter_drawer.
#line 218 "src/sgccplots.sgc"
ys, 
#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"
d->
#line 218 "src/sgccplots.sgc"
drawers.
#line 218 "src/sgccplots.sgc"
scatter_drawer.
#line 218 "src/sgccplots.sgc"
radius, 
#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"
d->
#line 218 "src/sgccplots.sgc"
drawers.
#line 218 "src/sgccplots.sgc"
scatter_drawer.
#line 218 "src/sgccplots.sgc"
color, 
#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"

#line 218 "src/sgccplots.sgc"
d->
#line 218 "src/sgccplots.sgc"
drawers.
#line 218 "src/sgccplots.sgc"
scatter_drawer.
#line 218 "src/sgccplots.sgc"
style);
      
#line 219 "src/sgccplots.sgc"
break;

    
#line 220 "src/sgccplots.sgc"
case 
#line 220 "src/sgccplots.sgc"
X_AXIS_DRAWER:
      
#line 221 "src/sgccplots.sgc"
draw_x_axis(
#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"
plt, 
#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"
d->
#line 221 "src/sgccplots.sgc"
drawers.
#line 221 "src/sgccplots.sgc"
x_axis_drawer.
#line 221 "src/sgccplots.sgc"
thickness, 
#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"

#line 221 "src/sgccplots.sgc"
d->
#line 221 "src/sgccplots.sgc"
drawers.
#line 221 "src/sgccplots.sgc"
x_axis_drawer.
#line 221 "src/sgccplots.sgc"
color);
      
#line 222 "src/sgccplots.sgc"
break;

    
#line 223 "src/sgccplots.sgc"
case 
#line 223 "src/sgccplots.sgc"
Y_AXIS_DRAWER:
      
#line 224 "src/sgccplots.sgc"
draw_y_axis(
#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"
plt, 
#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"
d->
#line 224 "src/sgccplots.sgc"
drawers.
#line 224 "src/sgccplots.sgc"
y_axis_drawer.
#line 224 "src/sgccplots.sgc"
thickness, 
#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"

#line 224 "src/sgccplots.sgc"
d->
#line 224 "src/sgccplots.sgc"
drawers.
#line 224 "src/sgccplots.sgc"
y_axis_drawer.
#line 224 "src/sgccplots.sgc"
color);
      
#line 225 "src/sgccplots.sgc"
break;

    
#line 226 "src/sgccplots.sgc"
default:
      
#line 227 "src/sgccplots.sgc"
break;

  }

}


#line 231 "src/sgccplots.sgc"

#line 231 "src/sgccplots.sgc"
inline static 
#line 231 "src/sgccplots.sgc"
void draw_all(
#line 231 "src/sgccplots.sgc"

#line 231 "src/sgccplots.sgc"

#line 231 "src/sgccplots.sgc"
plot plt)

#line 231 "src/sgccplots.sgc"
{
  
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
auto __iter_foreach_2 = 
#line 232 "src/sgccplots.sgc"
__iter__(
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
plt->
#line 232 "src/sgccplots.sgc"
drawers);
  
#line 232 "src/sgccplots.sgc"
for (
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
auto i = 
#line 232 "src/sgccplots.sgc"
__start__(
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
__iter_foreach_2); !
#line 232 "src/sgccplots.sgc"
__done__(
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
__iter_foreach_2); 
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
i = 
#line 232 "src/sgccplots.sgc"
__next__(
#line 232 "src/sgccplots.sgc"

#line 232 "src/sgccplots.sgc"
__iter_foreach_2))

#line 232 "src/sgccplots.sgc"
  {
    
#line 233 "src/sgccplots.sgc"
draw_elem(
#line 233 "src/sgccplots.sgc"

#line 233 "src/sgccplots.sgc"
plt, 
#line 233 "src/sgccplots.sgc"
i);
  }

}


#line 237 "src/sgccplots.sgc"

#line 237 "src/sgccplots.sgc"
inline static 
#line 237 "src/sgccplots.sgc"
Color string2color(
#line 237 "src/sgccplots.sgc"

#line 237 "src/sgccplots.sgc"

#line 237 "src/sgccplots.sgc"
string col)

#line 237 "src/sgccplots.sgc"
{
  
#line 238 "src/sgccplots.sgc"
if (
#line 238 "src/sgccplots.sgc"
__eq__(
#line 238 "src/sgccplots.sgc"

#line 238 "src/sgccplots.sgc"
col, 
#line 238 "src/sgccplots.sgc"
"lightgray"))
    
#line 238 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 238 "src/sgccplots.sgc"
Color){
#line 238 "src/sgccplots.sgc"

#line 238 "src/sgccplots.sgc"
200, 
#line 238 "src/sgccplots.sgc"
200, 
#line 238 "src/sgccplots.sgc"
200, 
#line 238 "src/sgccplots.sgc"
255};
  
#line 239 "src/sgccplots.sgc"
if (
#line 239 "src/sgccplots.sgc"
__eq__(
#line 239 "src/sgccplots.sgc"

#line 239 "src/sgccplots.sgc"
col, 
#line 239 "src/sgccplots.sgc"
"gray"))
    
#line 239 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 239 "src/sgccplots.sgc"
Color){
#line 239 "src/sgccplots.sgc"

#line 239 "src/sgccplots.sgc"
130, 
#line 239 "src/sgccplots.sgc"
130, 
#line 239 "src/sgccplots.sgc"
130, 
#line 239 "src/sgccplots.sgc"
255};
  
#line 240 "src/sgccplots.sgc"
if (
#line 240 "src/sgccplots.sgc"
__eq__(
#line 240 "src/sgccplots.sgc"

#line 240 "src/sgccplots.sgc"
col, 
#line 240 "src/sgccplots.sgc"
"darkgray"))
    
#line 240 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 240 "src/sgccplots.sgc"
Color){
#line 240 "src/sgccplots.sgc"

#line 240 "src/sgccplots.sgc"
80, 
#line 240 "src/sgccplots.sgc"
80, 
#line 240 "src/sgccplots.sgc"
80, 
#line 240 "src/sgccplots.sgc"
255};
  
#line 241 "src/sgccplots.sgc"
if (
#line 241 "src/sgccplots.sgc"
__eq__(
#line 241 "src/sgccplots.sgc"

#line 241 "src/sgccplots.sgc"
col, 
#line 241 "src/sgccplots.sgc"
"blue"))
    
#line 241 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 241 "src/sgccplots.sgc"
Color){
#line 241 "src/sgccplots.sgc"

#line 241 "src/sgccplots.sgc"
0, 
#line 241 "src/sgccplots.sgc"
121, 
#line 241 "src/sgccplots.sgc"
241, 
#line 241 "src/sgccplots.sgc"
255};
  
#line 242 "src/sgccplots.sgc"
if (
#line 242 "src/sgccplots.sgc"
__eq__(
#line 242 "src/sgccplots.sgc"

#line 242 "src/sgccplots.sgc"
col, 
#line 242 "src/sgccplots.sgc"
"red"))
    
#line 242 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 242 "src/sgccplots.sgc"
Color){
#line 242 "src/sgccplots.sgc"

#line 242 "src/sgccplots.sgc"
230, 
#line 242 "src/sgccplots.sgc"
41, 
#line 242 "src/sgccplots.sgc"
55, 
#line 242 "src/sgccplots.sgc"
255};
  
#line 243 "src/sgccplots.sgc"
if (
#line 243 "src/sgccplots.sgc"
__eq__(
#line 243 "src/sgccplots.sgc"

#line 243 "src/sgccplots.sgc"
col, 
#line 243 "src/sgccplots.sgc"
"orange"))
    
#line 243 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 243 "src/sgccplots.sgc"
Color){
#line 243 "src/sgccplots.sgc"

#line 243 "src/sgccplots.sgc"
255, 
#line 243 "src/sgccplots.sgc"
161, 
#line 243 "src/sgccplots.sgc"
0, 
#line 243 "src/sgccplots.sgc"
255};
  
#line 244 "src/sgccplots.sgc"
if (
#line 244 "src/sgccplots.sgc"
__eq__(
#line 244 "src/sgccplots.sgc"

#line 244 "src/sgccplots.sgc"
col, 
#line 244 "src/sgccplots.sgc"
"green"))
    
#line 244 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 244 "src/sgccplots.sgc"
Color){
#line 244 "src/sgccplots.sgc"

#line 244 "src/sgccplots.sgc"
0, 
#line 244 "src/sgccplots.sgc"
228, 
#line 244 "src/sgccplots.sgc"
48, 
#line 244 "src/sgccplots.sgc"
255};
  
#line 245 "src/sgccplots.sgc"
if (
#line 245 "src/sgccplots.sgc"
__eq__(
#line 245 "src/sgccplots.sgc"

#line 245 "src/sgccplots.sgc"
col, 
#line 245 "src/sgccplots.sgc"
"purple"))
    
#line 245 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 245 "src/sgccplots.sgc"
Color){
#line 245 "src/sgccplots.sgc"

#line 245 "src/sgccplots.sgc"
200, 
#line 245 "src/sgccplots.sgc"
122, 
#line 245 "src/sgccplots.sgc"
255, 
#line 245 "src/sgccplots.sgc"
255};
  
#line 246 "src/sgccplots.sgc"
if (
#line 246 "src/sgccplots.sgc"
__eq__(
#line 246 "src/sgccplots.sgc"

#line 246 "src/sgccplots.sgc"
col, 
#line 246 "src/sgccplots.sgc"
"skyblue"))
    
#line 246 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 246 "src/sgccplots.sgc"
Color){
#line 246 "src/sgccplots.sgc"

#line 246 "src/sgccplots.sgc"
102, 
#line 246 "src/sgccplots.sgc"
191, 
#line 246 "src/sgccplots.sgc"
255, 
#line 246 "src/sgccplots.sgc"
255};
  
#line 247 "src/sgccplots.sgc"
if (
#line 247 "src/sgccplots.sgc"
__eq__(
#line 247 "src/sgccplots.sgc"

#line 247 "src/sgccplots.sgc"
col, 
#line 247 "src/sgccplots.sgc"
"brown"))
    
#line 247 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 247 "src/sgccplots.sgc"
Color){
#line 247 "src/sgccplots.sgc"

#line 247 "src/sgccplots.sgc"
127, 
#line 247 "src/sgccplots.sgc"
106, 
#line 247 "src/sgccplots.sgc"
79, 
#line 247 "src/sgccplots.sgc"
255};
  
#line 248 "src/sgccplots.sgc"
if (
#line 248 "src/sgccplots.sgc"
__eq__(
#line 248 "src/sgccplots.sgc"

#line 248 "src/sgccplots.sgc"
col, 
#line 248 "src/sgccplots.sgc"
"black"))
    
#line 248 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 248 "src/sgccplots.sgc"
Color){
#line 248 "src/sgccplots.sgc"

#line 248 "src/sgccplots.sgc"
0, 
#line 248 "src/sgccplots.sgc"
0, 
#line 248 "src/sgccplots.sgc"
0, 
#line 248 "src/sgccplots.sgc"
255};
  
#line 249 "src/sgccplots.sgc"
return (
#line 0 "src/sgccplots.sgc"

#line 249 "src/sgccplots.sgc"
Color){
#line 249 "src/sgccplots.sgc"

#line 249 "src/sgccplots.sgc"
0, 
#line 249 "src/sgccplots.sgc"
0, 
#line 249 "src/sgccplots.sgc"
0, 
#line 249 "src/sgccplots.sgc"
255};
}


#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"
void line(
#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"
plot plt, 
#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"
float thickness, 
#line 252 "src/sgccplots.sgc"

#line 252 "src/sgccplots.sgc"
string color)

#line 252 "src/sgccplots.sgc"
{
  
#line 253 "src/sgccplots.sgc"
println(
#line 253 "src/sgccplots.sgc"

#line 253 "src/sgccplots.sgc"
color);
  
#line 254 "src/sgccplots.sgc"
append(
#line 254 "src/sgccplots.sgc"

#line 254 "src/sgccplots.sgc"
plt->
#line 254 "src/sgccplots.sgc"
drawers, 
#line 254 "src/sgccplots.sgc"
make_line_drawer(
#line 254 "src/sgccplots.sgc"

#line 254 "src/sgccplots.sgc"
xs, 
#line 254 "src/sgccplots.sgc"
ys, 
#line 254 "src/sgccplots.sgc"
thickness, 
#line 254 "src/sgccplots.sgc"
string2color(
#line 254 "src/sgccplots.sgc"

#line 254 "src/sgccplots.sgc"
color)));
}


#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
void scatter(
#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
plot plt, 
#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
string style, 
#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
float radius, 
#line 257 "src/sgccplots.sgc"

#line 257 "src/sgccplots.sgc"
string color)

#line 257 "src/sgccplots.sgc"
{
  
#line 258 "src/sgccplots.sgc"
println(
#line 258 "src/sgccplots.sgc"

#line 258 "src/sgccplots.sgc"
color);
  
#line 259 "src/sgccplots.sgc"
append(
#line 259 "src/sgccplots.sgc"

#line 259 "src/sgccplots.sgc"
plt->
#line 259 "src/sgccplots.sgc"
drawers, 
#line 259 "src/sgccplots.sgc"
make_scatter_drawer(
#line 259 "src/sgccplots.sgc"

#line 259 "src/sgccplots.sgc"
xs, 
#line 259 "src/sgccplots.sgc"
ys, 
#line 259 "src/sgccplots.sgc"
radius, 
#line 259 "src/sgccplots.sgc"
string2color(
#line 259 "src/sgccplots.sgc"

#line 259 "src/sgccplots.sgc"
color), 
#line 259 "src/sgccplots.sgc"
style));
}


#line 262 "src/sgccplots.sgc"

#line 262 "src/sgccplots.sgc"

#line 262 "src/sgccplots.sgc"
void y_axis(
#line 262 "src/sgccplots.sgc"

#line 262 "src/sgccplots.sgc"

#line 262 "src/sgccplots.sgc"
plot plt, 
#line 262 "src/sgccplots.sgc"

#line 262 "src/sgccplots.sgc"
float radius, 
#line 262 "src/sgccplots.sgc"

#line 262 "src/sgccplots.sgc"
string color)

#line 262 "src/sgccplots.sgc"
{
  
#line 263 "src/sgccplots.sgc"
append(
#line 263 "src/sgccplots.sgc"

#line 263 "src/sgccplots.sgc"
plt->
#line 263 "src/sgccplots.sgc"
drawers, 
#line 263 "src/sgccplots.sgc"
make_y_axis_drawer(
#line 263 "src/sgccplots.sgc"

#line 263 "src/sgccplots.sgc"
radius, 
#line 263 "src/sgccplots.sgc"
string2color(
#line 263 "src/sgccplots.sgc"

#line 263 "src/sgccplots.sgc"
color)));
}


#line 266 "src/sgccplots.sgc"

#line 266 "src/sgccplots.sgc"

#line 266 "src/sgccplots.sgc"
void x_axis(
#line 266 "src/sgccplots.sgc"

#line 266 "src/sgccplots.sgc"

#line 266 "src/sgccplots.sgc"
plot plt, 
#line 266 "src/sgccplots.sgc"

#line 266 "src/sgccplots.sgc"
float radius, 
#line 266 "src/sgccplots.sgc"

#line 266 "src/sgccplots.sgc"
string color)

#line 266 "src/sgccplots.sgc"
{
  
#line 267 "src/sgccplots.sgc"
append(
#line 267 "src/sgccplots.sgc"

#line 267 "src/sgccplots.sgc"
plt->
#line 267 "src/sgccplots.sgc"
drawers, 
#line 267 "src/sgccplots.sgc"
make_x_axis_drawer(
#line 267 "src/sgccplots.sgc"

#line 267 "src/sgccplots.sgc"
radius, 
#line 267 "src/sgccplots.sgc"
string2color(
#line 267 "src/sgccplots.sgc"

#line 267 "src/sgccplots.sgc"
color)));
}


#line 270 "src/sgccplots.sgc"

#line 270 "src/sgccplots.sgc"

#line 270 "src/sgccplots.sgc"
void y_axis(
#line 270 "src/sgccplots.sgc"

#line 270 "src/sgccplots.sgc"

#line 270 "src/sgccplots.sgc"
plot plt, 
#line 270 "src/sgccplots.sgc"

#line 270 "src/sgccplots.sgc"
float radius)

#line 270 "src/sgccplots.sgc"
{
  
#line 271 "src/sgccplots.sgc"
y_axis(
#line 271 "src/sgccplots.sgc"

#line 271 "src/sgccplots.sgc"
plt, 
#line 271 "src/sgccplots.sgc"
radius, 
#line 271 "src/sgccplots.sgc"
"black");
}


#line 274 "src/sgccplots.sgc"

#line 274 "src/sgccplots.sgc"

#line 274 "src/sgccplots.sgc"
void x_axis(
#line 274 "src/sgccplots.sgc"

#line 274 "src/sgccplots.sgc"

#line 274 "src/sgccplots.sgc"
plot plt, 
#line 274 "src/sgccplots.sgc"

#line 274 "src/sgccplots.sgc"
float radius)

#line 274 "src/sgccplots.sgc"
{
  
#line 275 "src/sgccplots.sgc"
x_axis(
#line 275 "src/sgccplots.sgc"

#line 275 "src/sgccplots.sgc"
plt, 
#line 275 "src/sgccplots.sgc"
radius, 
#line 275 "src/sgccplots.sgc"
"black");
}


#line 278 "src/sgccplots.sgc"

#line 278 "src/sgccplots.sgc"

#line 278 "src/sgccplots.sgc"
void y_axis(
#line 278 "src/sgccplots.sgc"

#line 278 "src/sgccplots.sgc"

#line 278 "src/sgccplots.sgc"
plot plt)

#line 278 "src/sgccplots.sgc"
{
  
#line 279 "src/sgccplots.sgc"
y_axis(
#line 279 "src/sgccplots.sgc"

#line 279 "src/sgccplots.sgc"
plt, 
#line 279 "src/sgccplots.sgc"
0.5, 
#line 279 "src/sgccplots.sgc"
"black");
}


#line 282 "src/sgccplots.sgc"

#line 282 "src/sgccplots.sgc"

#line 282 "src/sgccplots.sgc"
void x_axis(
#line 282 "src/sgccplots.sgc"

#line 282 "src/sgccplots.sgc"

#line 282 "src/sgccplots.sgc"
plot plt)

#line 282 "src/sgccplots.sgc"
{
  
#line 283 "src/sgccplots.sgc"
x_axis(
#line 283 "src/sgccplots.sgc"

#line 283 "src/sgccplots.sgc"
plt, 
#line 283 "src/sgccplots.sgc"
0.5, 
#line 283 "src/sgccplots.sgc"
"black");
}


#line 286 "src/sgccplots.sgc"

#line 286 "src/sgccplots.sgc"

#line 286 "src/sgccplots.sgc"
void axes(
#line 286 "src/sgccplots.sgc"

#line 286 "src/sgccplots.sgc"

#line 286 "src/sgccplots.sgc"
plot plt, 
#line 286 "src/sgccplots.sgc"

#line 286 "src/sgccplots.sgc"
float radius, 
#line 286 "src/sgccplots.sgc"

#line 286 "src/sgccplots.sgc"
string color)

#line 286 "src/sgccplots.sgc"
{
  
#line 287 "src/sgccplots.sgc"
y_axis(
#line 287 "src/sgccplots.sgc"

#line 287 "src/sgccplots.sgc"
plt, 
#line 287 "src/sgccplots.sgc"
radius, 
#line 287 "src/sgccplots.sgc"
color);
  
#line 288 "src/sgccplots.sgc"
x_axis(
#line 288 "src/sgccplots.sgc"

#line 288 "src/sgccplots.sgc"
plt, 
#line 288 "src/sgccplots.sgc"
radius, 
#line 288 "src/sgccplots.sgc"
color);
}


#line 291 "src/sgccplots.sgc"

#line 291 "src/sgccplots.sgc"

#line 291 "src/sgccplots.sgc"
void axes(
#line 291 "src/sgccplots.sgc"

#line 291 "src/sgccplots.sgc"

#line 291 "src/sgccplots.sgc"
plot plt, 
#line 291 "src/sgccplots.sgc"

#line 291 "src/sgccplots.sgc"
float radius)

#line 291 "src/sgccplots.sgc"
{
  
#line 292 "src/sgccplots.sgc"
axes(
#line 292 "src/sgccplots.sgc"

#line 292 "src/sgccplots.sgc"
plt, 
#line 292 "src/sgccplots.sgc"
radius, 
#line 292 "src/sgccplots.sgc"
"black");
}


#line 295 "src/sgccplots.sgc"

#line 295 "src/sgccplots.sgc"

#line 295 "src/sgccplots.sgc"
void axes(
#line 295 "src/sgccplots.sgc"

#line 295 "src/sgccplots.sgc"

#line 295 "src/sgccplots.sgc"
plot plt)

#line 295 "src/sgccplots.sgc"
{
  
#line 296 "src/sgccplots.sgc"
axes(
#line 296 "src/sgccplots.sgc"

#line 296 "src/sgccplots.sgc"
plt, 
#line 296 "src/sgccplots.sgc"
0.5, 
#line 296 "src/sgccplots.sgc"
"black");
}


#line 301 "src/sgccplots.sgc"

#line 301 "src/sgccplots.sgc"

#line 301 "src/sgccplots.sgc"
std__list__template__double to_double_list(
#line 301 "src/sgccplots.sgc"

#line 301 "src/sgccplots.sgc"

#line 301 "src/sgccplots.sgc"
std__list__template__float lst)

#line 301 "src/sgccplots.sgc"
{
  
#line 302 "src/sgccplots.sgc"

#line 302 "src/sgccplots.sgc"
auto out = 
#line 302 "src/sgccplots.sgc"
std__make_list__template__double();
  
#line 303 "src/sgccplots.sgc"
for (
#line 303 "src/sgccplots.sgc"

#line 303 "src/sgccplots.sgc"

#line 303 "src/sgccplots.sgc"
int i = 
#line 303 "src/sgccplots.sgc"
0; 
#line 303 "src/sgccplots.sgc"
__lt__(
#line 303 "src/sgccplots.sgc"

#line 303 "src/sgccplots.sgc"
i, 
#line 303 "src/sgccplots.sgc"
len(
#line 303 "src/sgccplots.sgc"

#line 303 "src/sgccplots.sgc"
lst)); 
#line 303 "src/sgccplots.sgc"

#line 303 "src/sgccplots.sgc"
i++)

#line 303 "src/sgccplots.sgc"
  {
    
#line 304 "src/sgccplots.sgc"
append(
#line 304 "src/sgccplots.sgc"
out, 
#line 304 "src/sgccplots.sgc"
__index_wrapper__(
#line 304 "src/sgccplots.sgc"

#line 304 "src/sgccplots.sgc"
lst, 
#line 304 "src/sgccplots.sgc"
i));
  }

  
#line 306 "src/sgccplots.sgc"
return 
#line 306 "src/sgccplots.sgc"
out;
}


#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"
void line(
#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"
plot plt, 
#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"
float thickness, 
#line 309 "src/sgccplots.sgc"

#line 309 "src/sgccplots.sgc"
string color)

#line 309 "src/sgccplots.sgc"
{
  
#line 310 "src/sgccplots.sgc"
line(
#line 310 "src/sgccplots.sgc"

#line 310 "src/sgccplots.sgc"
plt, 
#line 310 "src/sgccplots.sgc"
to_double_list(
#line 310 "src/sgccplots.sgc"

#line 310 "src/sgccplots.sgc"
xs), 
#line 310 "src/sgccplots.sgc"
to_double_list(
#line 310 "src/sgccplots.sgc"

#line 310 "src/sgccplots.sgc"
ys), 
#line 310 "src/sgccplots.sgc"
thickness, 
#line 310 "src/sgccplots.sgc"
color);
}


#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
void scatter(
#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
plot plt, 
#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
string style, 
#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
float radius, 
#line 313 "src/sgccplots.sgc"

#line 313 "src/sgccplots.sgc"
string color)

#line 313 "src/sgccplots.sgc"
{
  
#line 314 "src/sgccplots.sgc"
scatter(
#line 314 "src/sgccplots.sgc"

#line 314 "src/sgccplots.sgc"
plt, 
#line 314 "src/sgccplots.sgc"
to_double_list(
#line 314 "src/sgccplots.sgc"

#line 314 "src/sgccplots.sgc"
xs), 
#line 314 "src/sgccplots.sgc"
to_double_list(
#line 314 "src/sgccplots.sgc"

#line 314 "src/sgccplots.sgc"
ys), 
#line 314 "src/sgccplots.sgc"
style, 
#line 314 "src/sgccplots.sgc"
radius, 
#line 314 "src/sgccplots.sgc"
color);
}


#line 319 "src/sgccplots.sgc"

#line 319 "src/sgccplots.sgc"

#line 319 "src/sgccplots.sgc"
std__list__template__double to_double_list(
#line 319 "src/sgccplots.sgc"

#line 319 "src/sgccplots.sgc"

#line 319 "src/sgccplots.sgc"
std__list__template__int lst)

#line 319 "src/sgccplots.sgc"
{
  
#line 320 "src/sgccplots.sgc"

#line 320 "src/sgccplots.sgc"
auto out = 
#line 320 "src/sgccplots.sgc"
std__make_list__template__double();
  
#line 321 "src/sgccplots.sgc"
for (
#line 321 "src/sgccplots.sgc"

#line 321 "src/sgccplots.sgc"

#line 321 "src/sgccplots.sgc"
int i = 
#line 321 "src/sgccplots.sgc"
0; 
#line 321 "src/sgccplots.sgc"
__lt__(
#line 321 "src/sgccplots.sgc"

#line 321 "src/sgccplots.sgc"
i, 
#line 321 "src/sgccplots.sgc"
len(
#line 321 "src/sgccplots.sgc"

#line 321 "src/sgccplots.sgc"
lst)); 
#line 321 "src/sgccplots.sgc"

#line 321 "src/sgccplots.sgc"
i++)

#line 321 "src/sgccplots.sgc"
  {
    
#line 322 "src/sgccplots.sgc"
append(
#line 322 "src/sgccplots.sgc"
out, 
#line 322 "src/sgccplots.sgc"
__index_wrapper__(
#line 322 "src/sgccplots.sgc"

#line 322 "src/sgccplots.sgc"
lst, 
#line 322 "src/sgccplots.sgc"
i));
  }

  
#line 324 "src/sgccplots.sgc"
return 
#line 324 "src/sgccplots.sgc"
out;
}


#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"
void line(
#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"
plot plt, 
#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"
float thickness, 
#line 327 "src/sgccplots.sgc"

#line 327 "src/sgccplots.sgc"
string color)

#line 327 "src/sgccplots.sgc"
{
  
#line 328 "src/sgccplots.sgc"
line(
#line 328 "src/sgccplots.sgc"

#line 328 "src/sgccplots.sgc"
plt, 
#line 328 "src/sgccplots.sgc"
to_double_list(
#line 328 "src/sgccplots.sgc"

#line 328 "src/sgccplots.sgc"
xs), 
#line 328 "src/sgccplots.sgc"
to_double_list(
#line 328 "src/sgccplots.sgc"

#line 328 "src/sgccplots.sgc"
ys), 
#line 328 "src/sgccplots.sgc"
thickness, 
#line 328 "src/sgccplots.sgc"
color);
}


#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
void scatter(
#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
plot plt, 
#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
string style, 
#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
float radius, 
#line 331 "src/sgccplots.sgc"

#line 331 "src/sgccplots.sgc"
string color)

#line 331 "src/sgccplots.sgc"
{
  
#line 332 "src/sgccplots.sgc"
scatter(
#line 332 "src/sgccplots.sgc"

#line 332 "src/sgccplots.sgc"
plt, 
#line 332 "src/sgccplots.sgc"
to_double_list(
#line 332 "src/sgccplots.sgc"

#line 332 "src/sgccplots.sgc"
xs), 
#line 332 "src/sgccplots.sgc"
to_double_list(
#line 332 "src/sgccplots.sgc"

#line 332 "src/sgccplots.sgc"
ys), 
#line 332 "src/sgccplots.sgc"
style, 
#line 332 "src/sgccplots.sgc"
radius, 
#line 332 "src/sgccplots.sgc"
color);
}


#line 337 "src/sgccplots.sgc"

#line 337 "src/sgccplots.sgc"

#line 337 "src/sgccplots.sgc"
plot_arg gColor(
#line 337 "src/sgccplots.sgc"

#line 337 "src/sgccplots.sgc"

#line 337 "src/sgccplots.sgc"
string color)

#line 337 "src/sgccplots.sgc"
{
  
#line 338 "src/sgccplots.sgc"

#line 338 "src/sgccplots.sgc"
plot_arg out;
  
#line 339 "src/sgccplots.sgc"

#line 339 "src/sgccplots.sgc"

#line 339 "src/sgccplots.sgc"
out.
#line 339 "src/sgccplots.sgc"
t = 
#line 339 "src/sgccplots.sgc"
ARG_COLOR;
  
#line 340 "src/sgccplots.sgc"

#line 340 "src/sgccplots.sgc"

#line 340 "src/sgccplots.sgc"

#line 340 "src/sgccplots.sgc"
out.
#line 340 "src/sgccplots.sgc"
data.
#line 340 "src/sgccplots.sgc"
s = 
#line 340 "src/sgccplots.sgc"
color;
  
#line 341 "src/sgccplots.sgc"
return 
#line 341 "src/sgccplots.sgc"
out;
}


#line 344 "src/sgccplots.sgc"

#line 344 "src/sgccplots.sgc"

#line 344 "src/sgccplots.sgc"
plot_arg gRadius(
#line 344 "src/sgccplots.sgc"

#line 344 "src/sgccplots.sgc"

#line 344 "src/sgccplots.sgc"
float radius)

#line 344 "src/sgccplots.sgc"
{
  
#line 345 "src/sgccplots.sgc"

#line 345 "src/sgccplots.sgc"
plot_arg out;
  
#line 346 "src/sgccplots.sgc"

#line 346 "src/sgccplots.sgc"

#line 346 "src/sgccplots.sgc"
out.
#line 346 "src/sgccplots.sgc"
t = 
#line 346 "src/sgccplots.sgc"
ARG_RADIUS;
  
#line 347 "src/sgccplots.sgc"

#line 347 "src/sgccplots.sgc"

#line 347 "src/sgccplots.sgc"

#line 347 "src/sgccplots.sgc"
out.
#line 347 "src/sgccplots.sgc"
data.
#line 347 "src/sgccplots.sgc"
f = 
#line 347 "src/sgccplots.sgc"
radius;
  
#line 348 "src/sgccplots.sgc"
return 
#line 348 "src/sgccplots.sgc"
out;
}


#line 351 "src/sgccplots.sgc"

#line 351 "src/sgccplots.sgc"

#line 351 "src/sgccplots.sgc"
plot_arg gThickness(
#line 351 "src/sgccplots.sgc"

#line 351 "src/sgccplots.sgc"

#line 351 "src/sgccplots.sgc"
float thickness)

#line 351 "src/sgccplots.sgc"
{
  
#line 352 "src/sgccplots.sgc"

#line 352 "src/sgccplots.sgc"
plot_arg out;
  
#line 353 "src/sgccplots.sgc"

#line 353 "src/sgccplots.sgc"

#line 353 "src/sgccplots.sgc"
out.
#line 353 "src/sgccplots.sgc"
t = 
#line 353 "src/sgccplots.sgc"
ARG_THICKNESS;
  
#line 354 "src/sgccplots.sgc"

#line 354 "src/sgccplots.sgc"

#line 354 "src/sgccplots.sgc"

#line 354 "src/sgccplots.sgc"
out.
#line 354 "src/sgccplots.sgc"
data.
#line 354 "src/sgccplots.sgc"
f = 
#line 354 "src/sgccplots.sgc"
thickness;
  
#line 355 "src/sgccplots.sgc"
return 
#line 355 "src/sgccplots.sgc"
out;
}


#line 358 "src/sgccplots.sgc"

#line 358 "src/sgccplots.sgc"

#line 358 "src/sgccplots.sgc"
plot_arg gStyle(
#line 358 "src/sgccplots.sgc"

#line 358 "src/sgccplots.sgc"

#line 358 "src/sgccplots.sgc"
string style)

#line 358 "src/sgccplots.sgc"
{
  
#line 359 "src/sgccplots.sgc"

#line 359 "src/sgccplots.sgc"
plot_arg out;
  
#line 360 "src/sgccplots.sgc"

#line 360 "src/sgccplots.sgc"

#line 360 "src/sgccplots.sgc"
out.
#line 360 "src/sgccplots.sgc"
t = 
#line 360 "src/sgccplots.sgc"
ARG_STYLE;
  
#line 361 "src/sgccplots.sgc"

#line 361 "src/sgccplots.sgc"

#line 361 "src/sgccplots.sgc"

#line 361 "src/sgccplots.sgc"
out.
#line 361 "src/sgccplots.sgc"
data.
#line 361 "src/sgccplots.sgc"
s = 
#line 361 "src/sgccplots.sgc"
style;
  
#line 362 "src/sgccplots.sgc"
return 
#line 362 "src/sgccplots.sgc"
out;
}


#line 365 "src/sgccplots.sgc"

#line 365 "src/sgccplots.sgc"

#line 365 "src/sgccplots.sgc"
plot_arg_list __add__(
#line 365 "src/sgccplots.sgc"

#line 365 "src/sgccplots.sgc"

#line 365 "src/sgccplots.sgc"
plot_arg a, 
#line 365 "src/sgccplots.sgc"

#line 365 "src/sgccplots.sgc"
plot_arg b)

#line 365 "src/sgccplots.sgc"
{
  
#line 366 "src/sgccplots.sgc"

#line 366 "src/sgccplots.sgc"
plot_arg_list out = 
#line 366 "src/sgccplots.sgc"
std__make_list__template__plot_arg();
  
#line 367 "src/sgccplots.sgc"
append(
#line 367 "src/sgccplots.sgc"

#line 367 "src/sgccplots.sgc"
out, 
#line 367 "src/sgccplots.sgc"
a);
  
#line 368 "src/sgccplots.sgc"
append(
#line 368 "src/sgccplots.sgc"

#line 368 "src/sgccplots.sgc"
out, 
#line 368 "src/sgccplots.sgc"
b);
  
#line 369 "src/sgccplots.sgc"
return 
#line 369 "src/sgccplots.sgc"
out;
}


#line 372 "src/sgccplots.sgc"

#line 372 "src/sgccplots.sgc"

#line 372 "src/sgccplots.sgc"
plot_arg_list __add__(
#line 372 "src/sgccplots.sgc"

#line 372 "src/sgccplots.sgc"

#line 372 "src/sgccplots.sgc"
plot_arg_list a, 
#line 372 "src/sgccplots.sgc"

#line 372 "src/sgccplots.sgc"
plot_arg b)

#line 372 "src/sgccplots.sgc"
{
  
#line 373 "src/sgccplots.sgc"
append(
#line 373 "src/sgccplots.sgc"

#line 373 "src/sgccplots.sgc"
a, 
#line 373 "src/sgccplots.sgc"
b);
  
#line 374 "src/sgccplots.sgc"
return 
#line 374 "src/sgccplots.sgc"
a;
}


#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"
void line(
#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"
plot plt, 
#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 379 "src/sgccplots.sgc"

#line 379 "src/sgccplots.sgc"
float thickness)

#line 379 "src/sgccplots.sgc"
{
  
#line 380 "src/sgccplots.sgc"
line(
#line 380 "src/sgccplots.sgc"

#line 380 "src/sgccplots.sgc"
plt, 
#line 380 "src/sgccplots.sgc"
xs, 
#line 380 "src/sgccplots.sgc"
ys, 
#line 380 "src/sgccplots.sgc"
thickness, 
#line 380 "src/sgccplots.sgc"
__index_wrapper__(
#line 380 "src/sgccplots.sgc"
color_cycle, 
#line 380 "src/sgccplots.sgc"
__mod__(
#line 380 "src/sgccplots.sgc"

#line 380 "src/sgccplots.sgc"

#line 380 "src/sgccplots.sgc"
plt->
#line 380 "src/sgccplots.sgc"
color_cycle, 
#line 380 "src/sgccplots.sgc"
n_cycle)));
  
#line 381 "src/sgccplots.sgc"

#line 381 "src/sgccplots.sgc"

#line 381 "src/sgccplots.sgc"
plt->
#line 381 "src/sgccplots.sgc"
color_cycle++;
}


#line 384 "src/sgccplots.sgc"

#line 384 "src/sgccplots.sgc"

#line 384 "src/sgccplots.sgc"
void line(
#line 384 "src/sgccplots.sgc"

#line 384 "src/sgccplots.sgc"

#line 384 "src/sgccplots.sgc"
plot plt, 
#line 384 "src/sgccplots.sgc"

#line 384 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 384 "src/sgccplots.sgc"

#line 384 "src/sgccplots.sgc"
std__list__template__double ys)

#line 384 "src/sgccplots.sgc"
{
  
#line 385 "src/sgccplots.sgc"
line(
#line 385 "src/sgccplots.sgc"

#line 385 "src/sgccplots.sgc"
plt, 
#line 385 "src/sgccplots.sgc"
xs, 
#line 385 "src/sgccplots.sgc"
ys, 
#line 385 "src/sgccplots.sgc"
1);
}


#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"
void line(
#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"
plot plt, 
#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 388 "src/sgccplots.sgc"

#line 388 "src/sgccplots.sgc"
plot_arg arg0)

#line 388 "src/sgccplots.sgc"
{
  
#line 389 "src/sgccplots.sgc"

#line 389 "src/sgccplots.sgc"
float thickness = 
#line 389 "src/sgccplots.sgc"
1;
  
#line 390 "src/sgccplots.sgc"

#line 390 "src/sgccplots.sgc"
string color = 
#line 390 "src/sgccplots.sgc"
__index_wrapper__(
#line 390 "src/sgccplots.sgc"
color_cycle, 
#line 390 "src/sgccplots.sgc"
__mod__(
#line 390 "src/sgccplots.sgc"

#line 390 "src/sgccplots.sgc"

#line 390 "src/sgccplots.sgc"
plt->
#line 390 "src/sgccplots.sgc"
color_cycle, 
#line 390 "src/sgccplots.sgc"
n_cycle));
  
#line 391 "src/sgccplots.sgc"
switch (
#line 391 "src/sgccplots.sgc"

#line 391 "src/sgccplots.sgc"
arg0.
#line 391 "src/sgccplots.sgc"
t)

#line 391 "src/sgccplots.sgc"
  {
    
#line 392 "src/sgccplots.sgc"
case 
#line 392 "src/sgccplots.sgc"
ARG_THICKNESS:
      
#line 393 "src/sgccplots.sgc"

#line 393 "src/sgccplots.sgc"
thickness = 
#line 393 "src/sgccplots.sgc"

#line 393 "src/sgccplots.sgc"

#line 393 "src/sgccplots.sgc"
arg0.
#line 393 "src/sgccplots.sgc"
data.
#line 393 "src/sgccplots.sgc"
f;
      
#line 394 "src/sgccplots.sgc"
break;

    
#line 395 "src/sgccplots.sgc"
case 
#line 395 "src/sgccplots.sgc"
ARG_COLOR:
      
#line 396 "src/sgccplots.sgc"

#line 396 "src/sgccplots.sgc"
color = 
#line 396 "src/sgccplots.sgc"

#line 396 "src/sgccplots.sgc"

#line 396 "src/sgccplots.sgc"
arg0.
#line 396 "src/sgccplots.sgc"
data.
#line 396 "src/sgccplots.sgc"
s;
      
#line 397 "src/sgccplots.sgc"
break;

    
#line 398 "src/sgccplots.sgc"
default:
      
#line 399 "src/sgccplots.sgc"
break;

  }

  
#line 401 "src/sgccplots.sgc"
line(
#line 401 "src/sgccplots.sgc"

#line 401 "src/sgccplots.sgc"
plt, 
#line 401 "src/sgccplots.sgc"
xs, 
#line 401 "src/sgccplots.sgc"
ys, 
#line 401 "src/sgccplots.sgc"
thickness, 
#line 401 "src/sgccplots.sgc"
color);
}


#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"
void line(
#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"
plot plt, 
#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 404 "src/sgccplots.sgc"

#line 404 "src/sgccplots.sgc"
plot_arg_list args)

#line 404 "src/sgccplots.sgc"
{
  
#line 405 "src/sgccplots.sgc"

#line 405 "src/sgccplots.sgc"
float thickness = 
#line 405 "src/sgccplots.sgc"
1;
  
#line 406 "src/sgccplots.sgc"

#line 406 "src/sgccplots.sgc"
string color = 
#line 406 "src/sgccplots.sgc"
__index_wrapper__(
#line 406 "src/sgccplots.sgc"
color_cycle, 
#line 406 "src/sgccplots.sgc"
__mod__(
#line 406 "src/sgccplots.sgc"

#line 406 "src/sgccplots.sgc"

#line 406 "src/sgccplots.sgc"
plt->
#line 406 "src/sgccplots.sgc"
color_cycle, 
#line 406 "src/sgccplots.sgc"
n_cycle));
  
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
auto __iter_foreach_3 = 
#line 407 "src/sgccplots.sgc"
__iter__(
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
args);
  
#line 407 "src/sgccplots.sgc"
for (
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
auto arg0 = 
#line 407 "src/sgccplots.sgc"
__start__(
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
__iter_foreach_3); !
#line 407 "src/sgccplots.sgc"
__done__(
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
__iter_foreach_3); 
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
arg0 = 
#line 407 "src/sgccplots.sgc"
__next__(
#line 407 "src/sgccplots.sgc"

#line 407 "src/sgccplots.sgc"
__iter_foreach_3))

#line 407 "src/sgccplots.sgc"
  {
    
#line 408 "src/sgccplots.sgc"
switch (
#line 408 "src/sgccplots.sgc"

#line 408 "src/sgccplots.sgc"
arg0.
#line 408 "src/sgccplots.sgc"
t)

#line 408 "src/sgccplots.sgc"
    {
      
#line 409 "src/sgccplots.sgc"
case 
#line 409 "src/sgccplots.sgc"
ARG_THICKNESS:
        
#line 410 "src/sgccplots.sgc"

#line 410 "src/sgccplots.sgc"
thickness = 
#line 410 "src/sgccplots.sgc"

#line 410 "src/sgccplots.sgc"

#line 410 "src/sgccplots.sgc"
arg0.
#line 410 "src/sgccplots.sgc"
data.
#line 410 "src/sgccplots.sgc"
f;
        
#line 411 "src/sgccplots.sgc"
break;

      
#line 412 "src/sgccplots.sgc"
case 
#line 412 "src/sgccplots.sgc"
ARG_COLOR:
        
#line 413 "src/sgccplots.sgc"

#line 413 "src/sgccplots.sgc"
color = 
#line 413 "src/sgccplots.sgc"

#line 413 "src/sgccplots.sgc"

#line 413 "src/sgccplots.sgc"
arg0.
#line 413 "src/sgccplots.sgc"
data.
#line 413 "src/sgccplots.sgc"
s;
        
#line 414 "src/sgccplots.sgc"
break;

      
#line 415 "src/sgccplots.sgc"
default:
        
#line 416 "src/sgccplots.sgc"
break;

    }

  }

  
#line 419 "src/sgccplots.sgc"
line(
#line 419 "src/sgccplots.sgc"

#line 419 "src/sgccplots.sgc"
plt, 
#line 419 "src/sgccplots.sgc"
xs, 
#line 419 "src/sgccplots.sgc"
ys, 
#line 419 "src/sgccplots.sgc"
thickness, 
#line 419 "src/sgccplots.sgc"
color);
}


#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"
void scatter(
#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"
plot plt, 
#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"
string style, 
#line 422 "src/sgccplots.sgc"

#line 422 "src/sgccplots.sgc"
float radius)

#line 422 "src/sgccplots.sgc"
{
  
#line 423 "src/sgccplots.sgc"
scatter(
#line 423 "src/sgccplots.sgc"

#line 423 "src/sgccplots.sgc"
plt, 
#line 423 "src/sgccplots.sgc"
xs, 
#line 423 "src/sgccplots.sgc"
ys, 
#line 423 "src/sgccplots.sgc"
style, 
#line 423 "src/sgccplots.sgc"
radius, 
#line 423 "src/sgccplots.sgc"
__index_wrapper__(
#line 423 "src/sgccplots.sgc"
color_cycle, 
#line 423 "src/sgccplots.sgc"
__mod__(
#line 423 "src/sgccplots.sgc"

#line 423 "src/sgccplots.sgc"

#line 423 "src/sgccplots.sgc"
plt->
#line 423 "src/sgccplots.sgc"
color_cycle, 
#line 423 "src/sgccplots.sgc"
n_cycle)));
  
#line 424 "src/sgccplots.sgc"

#line 424 "src/sgccplots.sgc"

#line 424 "src/sgccplots.sgc"
plt->
#line 424 "src/sgccplots.sgc"
color_cycle++;
}


#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"
void scatter(
#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"
plot plt, 
#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 427 "src/sgccplots.sgc"

#line 427 "src/sgccplots.sgc"
string style)

#line 427 "src/sgccplots.sgc"
{
  
#line 428 "src/sgccplots.sgc"
scatter(
#line 428 "src/sgccplots.sgc"

#line 428 "src/sgccplots.sgc"
plt, 
#line 428 "src/sgccplots.sgc"
xs, 
#line 428 "src/sgccplots.sgc"
ys, 
#line 428 "src/sgccplots.sgc"
style, 
#line 428 "src/sgccplots.sgc"
1);
}


#line 431 "src/sgccplots.sgc"

#line 431 "src/sgccplots.sgc"

#line 431 "src/sgccplots.sgc"
void scatter(
#line 431 "src/sgccplots.sgc"

#line 431 "src/sgccplots.sgc"

#line 431 "src/sgccplots.sgc"
plot plt, 
#line 431 "src/sgccplots.sgc"

#line 431 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 431 "src/sgccplots.sgc"

#line 431 "src/sgccplots.sgc"
std__list__template__double ys)

#line 431 "src/sgccplots.sgc"
{
  
#line 432 "src/sgccplots.sgc"
scatter(
#line 432 "src/sgccplots.sgc"

#line 432 "src/sgccplots.sgc"
plt, 
#line 432 "src/sgccplots.sgc"
xs, 
#line 432 "src/sgccplots.sgc"
ys, 
#line 432 "src/sgccplots.sgc"
"o");
}


#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"
void scatter(
#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"
plot plt, 
#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 435 "src/sgccplots.sgc"

#line 435 "src/sgccplots.sgc"
plot_arg arg0)

#line 435 "src/sgccplots.sgc"
{
  
#line 436 "src/sgccplots.sgc"

#line 436 "src/sgccplots.sgc"
float radius = 
#line 436 "src/sgccplots.sgc"
1;
  
#line 437 "src/sgccplots.sgc"

#line 437 "src/sgccplots.sgc"
string color = 
#line 437 "src/sgccplots.sgc"
__index_wrapper__(
#line 437 "src/sgccplots.sgc"
color_cycle, 
#line 437 "src/sgccplots.sgc"
__mod__(
#line 437 "src/sgccplots.sgc"

#line 437 "src/sgccplots.sgc"

#line 437 "src/sgccplots.sgc"
plt->
#line 437 "src/sgccplots.sgc"
color_cycle, 
#line 437 "src/sgccplots.sgc"
n_cycle));
  
#line 438 "src/sgccplots.sgc"

#line 438 "src/sgccplots.sgc"
string style = 
#line 438 "src/sgccplots.sgc"
"o";
  
#line 439 "src/sgccplots.sgc"
switch (
#line 439 "src/sgccplots.sgc"

#line 439 "src/sgccplots.sgc"
arg0.
#line 439 "src/sgccplots.sgc"
t)

#line 439 "src/sgccplots.sgc"
  {
    
#line 440 "src/sgccplots.sgc"
case 
#line 440 "src/sgccplots.sgc"
ARG_STYLE:
      
#line 441 "src/sgccplots.sgc"

#line 441 "src/sgccplots.sgc"
style = 
#line 441 "src/sgccplots.sgc"

#line 441 "src/sgccplots.sgc"

#line 441 "src/sgccplots.sgc"
arg0.
#line 441 "src/sgccplots.sgc"
data.
#line 441 "src/sgccplots.sgc"
s;
      
#line 442 "src/sgccplots.sgc"
break;

    
#line 443 "src/sgccplots.sgc"
case 
#line 443 "src/sgccplots.sgc"
ARG_RADIUS:
      
#line 444 "src/sgccplots.sgc"

#line 444 "src/sgccplots.sgc"
radius = 
#line 444 "src/sgccplots.sgc"

#line 444 "src/sgccplots.sgc"

#line 444 "src/sgccplots.sgc"
arg0.
#line 444 "src/sgccplots.sgc"
data.
#line 444 "src/sgccplots.sgc"
f;
      
#line 445 "src/sgccplots.sgc"
break;

    
#line 446 "src/sgccplots.sgc"
case 
#line 446 "src/sgccplots.sgc"
ARG_COLOR:
      
#line 447 "src/sgccplots.sgc"

#line 447 "src/sgccplots.sgc"
color = 
#line 447 "src/sgccplots.sgc"

#line 447 "src/sgccplots.sgc"

#line 447 "src/sgccplots.sgc"
arg0.
#line 447 "src/sgccplots.sgc"
data.
#line 447 "src/sgccplots.sgc"
s;
      
#line 448 "src/sgccplots.sgc"
break;

    
#line 449 "src/sgccplots.sgc"
default:
      
#line 450 "src/sgccplots.sgc"
break;

  }

  
#line 452 "src/sgccplots.sgc"
scatter(
#line 452 "src/sgccplots.sgc"

#line 452 "src/sgccplots.sgc"
plt, 
#line 452 "src/sgccplots.sgc"
xs, 
#line 452 "src/sgccplots.sgc"
ys, 
#line 452 "src/sgccplots.sgc"
style, 
#line 452 "src/sgccplots.sgc"
radius, 
#line 452 "src/sgccplots.sgc"
color);
}


#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"
void scatter(
#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"
plot plt, 
#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"
std__list__template__double xs, 
#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"
std__list__template__double ys, 
#line 455 "src/sgccplots.sgc"

#line 455 "src/sgccplots.sgc"
plot_arg_list args)

#line 455 "src/sgccplots.sgc"
{
  
#line 456 "src/sgccplots.sgc"

#line 456 "src/sgccplots.sgc"
float radius = 
#line 456 "src/sgccplots.sgc"
1;
  
#line 457 "src/sgccplots.sgc"

#line 457 "src/sgccplots.sgc"
string color = 
#line 457 "src/sgccplots.sgc"
__index_wrapper__(
#line 457 "src/sgccplots.sgc"
color_cycle, 
#line 457 "src/sgccplots.sgc"
__mod__(
#line 457 "src/sgccplots.sgc"

#line 457 "src/sgccplots.sgc"

#line 457 "src/sgccplots.sgc"
plt->
#line 457 "src/sgccplots.sgc"
color_cycle, 
#line 457 "src/sgccplots.sgc"
n_cycle));
  
#line 458 "src/sgccplots.sgc"

#line 458 "src/sgccplots.sgc"
string style = 
#line 458 "src/sgccplots.sgc"
"o";
  
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
auto __iter_foreach_4 = 
#line 459 "src/sgccplots.sgc"
__iter__(
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
args);
  
#line 459 "src/sgccplots.sgc"
for (
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
auto arg0 = 
#line 459 "src/sgccplots.sgc"
__start__(
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
__iter_foreach_4); !
#line 459 "src/sgccplots.sgc"
__done__(
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
__iter_foreach_4); 
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
arg0 = 
#line 459 "src/sgccplots.sgc"
__next__(
#line 459 "src/sgccplots.sgc"

#line 459 "src/sgccplots.sgc"
__iter_foreach_4))

#line 459 "src/sgccplots.sgc"
  {
    
#line 460 "src/sgccplots.sgc"
switch (
#line 460 "src/sgccplots.sgc"

#line 460 "src/sgccplots.sgc"
arg0.
#line 460 "src/sgccplots.sgc"
t)

#line 460 "src/sgccplots.sgc"
    {
      
#line 461 "src/sgccplots.sgc"
case 
#line 461 "src/sgccplots.sgc"
ARG_STYLE:
        
#line 462 "src/sgccplots.sgc"

#line 462 "src/sgccplots.sgc"
style = 
#line 462 "src/sgccplots.sgc"

#line 462 "src/sgccplots.sgc"

#line 462 "src/sgccplots.sgc"
arg0.
#line 462 "src/sgccplots.sgc"
data.
#line 462 "src/sgccplots.sgc"
s;
        
#line 463 "src/sgccplots.sgc"
break;

      
#line 464 "src/sgccplots.sgc"
case 
#line 464 "src/sgccplots.sgc"
ARG_RADIUS:
        
#line 465 "src/sgccplots.sgc"

#line 465 "src/sgccplots.sgc"
radius = 
#line 465 "src/sgccplots.sgc"

#line 465 "src/sgccplots.sgc"

#line 465 "src/sgccplots.sgc"
arg0.
#line 465 "src/sgccplots.sgc"
data.
#line 465 "src/sgccplots.sgc"
f;
        
#line 466 "src/sgccplots.sgc"
break;

      
#line 467 "src/sgccplots.sgc"
case 
#line 467 "src/sgccplots.sgc"
ARG_COLOR:
        
#line 468 "src/sgccplots.sgc"

#line 468 "src/sgccplots.sgc"
color = 
#line 468 "src/sgccplots.sgc"

#line 468 "src/sgccplots.sgc"

#line 468 "src/sgccplots.sgc"
arg0.
#line 468 "src/sgccplots.sgc"
data.
#line 468 "src/sgccplots.sgc"
s;
        
#line 469 "src/sgccplots.sgc"
break;

      
#line 470 "src/sgccplots.sgc"
default:
        
#line 471 "src/sgccplots.sgc"
break;

    }

  }

  
#line 474 "src/sgccplots.sgc"
scatter(
#line 474 "src/sgccplots.sgc"

#line 474 "src/sgccplots.sgc"
plt, 
#line 474 "src/sgccplots.sgc"
xs, 
#line 474 "src/sgccplots.sgc"
ys, 
#line 474 "src/sgccplots.sgc"
style, 
#line 474 "src/sgccplots.sgc"
radius, 
#line 474 "src/sgccplots.sgc"
color);
}


#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"
void line(
#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"
plot plt, 
#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 480 "src/sgccplots.sgc"

#line 480 "src/sgccplots.sgc"
float thickness)

#line 480 "src/sgccplots.sgc"
{
  
#line 481 "src/sgccplots.sgc"
line(
#line 481 "src/sgccplots.sgc"

#line 481 "src/sgccplots.sgc"
plt, 
#line 481 "src/sgccplots.sgc"
xs, 
#line 481 "src/sgccplots.sgc"
ys, 
#line 481 "src/sgccplots.sgc"
thickness, 
#line 481 "src/sgccplots.sgc"
__index_wrapper__(
#line 481 "src/sgccplots.sgc"
color_cycle, 
#line 481 "src/sgccplots.sgc"
__mod__(
#line 481 "src/sgccplots.sgc"

#line 481 "src/sgccplots.sgc"

#line 481 "src/sgccplots.sgc"
plt->
#line 481 "src/sgccplots.sgc"
color_cycle, 
#line 481 "src/sgccplots.sgc"
n_cycle)));
  
#line 482 "src/sgccplots.sgc"

#line 482 "src/sgccplots.sgc"

#line 482 "src/sgccplots.sgc"
plt->
#line 482 "src/sgccplots.sgc"
color_cycle++;
}


#line 485 "src/sgccplots.sgc"

#line 485 "src/sgccplots.sgc"

#line 485 "src/sgccplots.sgc"
void line(
#line 485 "src/sgccplots.sgc"

#line 485 "src/sgccplots.sgc"

#line 485 "src/sgccplots.sgc"
plot plt, 
#line 485 "src/sgccplots.sgc"

#line 485 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 485 "src/sgccplots.sgc"

#line 485 "src/sgccplots.sgc"
std__list__template__float ys)

#line 485 "src/sgccplots.sgc"
{
  
#line 486 "src/sgccplots.sgc"
line(
#line 486 "src/sgccplots.sgc"

#line 486 "src/sgccplots.sgc"
plt, 
#line 486 "src/sgccplots.sgc"
xs, 
#line 486 "src/sgccplots.sgc"
ys, 
#line 486 "src/sgccplots.sgc"
1);
}


#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"
void line(
#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"
plot plt, 
#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 489 "src/sgccplots.sgc"

#line 489 "src/sgccplots.sgc"
plot_arg arg0)

#line 489 "src/sgccplots.sgc"
{
  
#line 490 "src/sgccplots.sgc"

#line 490 "src/sgccplots.sgc"
float thickness = 
#line 490 "src/sgccplots.sgc"
1;
  
#line 491 "src/sgccplots.sgc"

#line 491 "src/sgccplots.sgc"
string color = 
#line 491 "src/sgccplots.sgc"
__index_wrapper__(
#line 491 "src/sgccplots.sgc"
color_cycle, 
#line 491 "src/sgccplots.sgc"
__mod__(
#line 491 "src/sgccplots.sgc"

#line 491 "src/sgccplots.sgc"

#line 491 "src/sgccplots.sgc"
plt->
#line 491 "src/sgccplots.sgc"
color_cycle, 
#line 491 "src/sgccplots.sgc"
n_cycle));
  
#line 492 "src/sgccplots.sgc"
switch (
#line 492 "src/sgccplots.sgc"

#line 492 "src/sgccplots.sgc"
arg0.
#line 492 "src/sgccplots.sgc"
t)

#line 492 "src/sgccplots.sgc"
  {
    
#line 493 "src/sgccplots.sgc"
case 
#line 493 "src/sgccplots.sgc"
ARG_THICKNESS:
      
#line 494 "src/sgccplots.sgc"

#line 494 "src/sgccplots.sgc"
thickness = 
#line 494 "src/sgccplots.sgc"

#line 494 "src/sgccplots.sgc"

#line 494 "src/sgccplots.sgc"
arg0.
#line 494 "src/sgccplots.sgc"
data.
#line 494 "src/sgccplots.sgc"
f;
      
#line 495 "src/sgccplots.sgc"
break;

    
#line 496 "src/sgccplots.sgc"
case 
#line 496 "src/sgccplots.sgc"
ARG_COLOR:
      
#line 497 "src/sgccplots.sgc"

#line 497 "src/sgccplots.sgc"
color = 
#line 497 "src/sgccplots.sgc"

#line 497 "src/sgccplots.sgc"

#line 497 "src/sgccplots.sgc"
arg0.
#line 497 "src/sgccplots.sgc"
data.
#line 497 "src/sgccplots.sgc"
s;
      
#line 498 "src/sgccplots.sgc"
break;

    
#line 499 "src/sgccplots.sgc"
default:
      
#line 500 "src/sgccplots.sgc"
break;

  }

  
#line 502 "src/sgccplots.sgc"
line(
#line 502 "src/sgccplots.sgc"

#line 502 "src/sgccplots.sgc"
plt, 
#line 502 "src/sgccplots.sgc"
xs, 
#line 502 "src/sgccplots.sgc"
ys, 
#line 502 "src/sgccplots.sgc"
thickness, 
#line 502 "src/sgccplots.sgc"
color);
}


#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"
void line(
#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"
plot plt, 
#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 505 "src/sgccplots.sgc"

#line 505 "src/sgccplots.sgc"
plot_arg_list args)

#line 505 "src/sgccplots.sgc"
{
  
#line 506 "src/sgccplots.sgc"

#line 506 "src/sgccplots.sgc"
float thickness = 
#line 506 "src/sgccplots.sgc"
1;
  
#line 507 "src/sgccplots.sgc"

#line 507 "src/sgccplots.sgc"
string color = 
#line 507 "src/sgccplots.sgc"
__index_wrapper__(
#line 507 "src/sgccplots.sgc"
color_cycle, 
#line 507 "src/sgccplots.sgc"
__mod__(
#line 507 "src/sgccplots.sgc"

#line 507 "src/sgccplots.sgc"

#line 507 "src/sgccplots.sgc"
plt->
#line 507 "src/sgccplots.sgc"
color_cycle, 
#line 507 "src/sgccplots.sgc"
n_cycle));
  
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
auto __iter_foreach_5 = 
#line 508 "src/sgccplots.sgc"
__iter__(
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
args);
  
#line 508 "src/sgccplots.sgc"
for (
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
auto arg0 = 
#line 508 "src/sgccplots.sgc"
__start__(
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
__iter_foreach_5); !
#line 508 "src/sgccplots.sgc"
__done__(
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
__iter_foreach_5); 
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
arg0 = 
#line 508 "src/sgccplots.sgc"
__next__(
#line 508 "src/sgccplots.sgc"

#line 508 "src/sgccplots.sgc"
__iter_foreach_5))

#line 508 "src/sgccplots.sgc"
  {
    
#line 509 "src/sgccplots.sgc"
switch (
#line 509 "src/sgccplots.sgc"

#line 509 "src/sgccplots.sgc"
arg0.
#line 509 "src/sgccplots.sgc"
t)

#line 509 "src/sgccplots.sgc"
    {
      
#line 510 "src/sgccplots.sgc"
case 
#line 510 "src/sgccplots.sgc"
ARG_THICKNESS:
        
#line 511 "src/sgccplots.sgc"

#line 511 "src/sgccplots.sgc"
thickness = 
#line 511 "src/sgccplots.sgc"

#line 511 "src/sgccplots.sgc"

#line 511 "src/sgccplots.sgc"
arg0.
#line 511 "src/sgccplots.sgc"
data.
#line 511 "src/sgccplots.sgc"
f;
        
#line 512 "src/sgccplots.sgc"
break;

      
#line 513 "src/sgccplots.sgc"
case 
#line 513 "src/sgccplots.sgc"
ARG_COLOR:
        
#line 514 "src/sgccplots.sgc"

#line 514 "src/sgccplots.sgc"
color = 
#line 514 "src/sgccplots.sgc"

#line 514 "src/sgccplots.sgc"

#line 514 "src/sgccplots.sgc"
arg0.
#line 514 "src/sgccplots.sgc"
data.
#line 514 "src/sgccplots.sgc"
s;
        
#line 515 "src/sgccplots.sgc"
break;

      
#line 516 "src/sgccplots.sgc"
default:
        
#line 517 "src/sgccplots.sgc"
break;

    }

  }

  
#line 520 "src/sgccplots.sgc"
line(
#line 520 "src/sgccplots.sgc"

#line 520 "src/sgccplots.sgc"
plt, 
#line 520 "src/sgccplots.sgc"
xs, 
#line 520 "src/sgccplots.sgc"
ys, 
#line 520 "src/sgccplots.sgc"
thickness, 
#line 520 "src/sgccplots.sgc"
color);
}


#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"
void scatter(
#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"
plot plt, 
#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"
string style, 
#line 523 "src/sgccplots.sgc"

#line 523 "src/sgccplots.sgc"
float radius)

#line 523 "src/sgccplots.sgc"
{
  
#line 524 "src/sgccplots.sgc"
scatter(
#line 524 "src/sgccplots.sgc"

#line 524 "src/sgccplots.sgc"
plt, 
#line 524 "src/sgccplots.sgc"
xs, 
#line 524 "src/sgccplots.sgc"
ys, 
#line 524 "src/sgccplots.sgc"
style, 
#line 524 "src/sgccplots.sgc"
radius, 
#line 524 "src/sgccplots.sgc"
__index_wrapper__(
#line 524 "src/sgccplots.sgc"
color_cycle, 
#line 524 "src/sgccplots.sgc"
__mod__(
#line 524 "src/sgccplots.sgc"

#line 524 "src/sgccplots.sgc"

#line 524 "src/sgccplots.sgc"
plt->
#line 524 "src/sgccplots.sgc"
color_cycle, 
#line 524 "src/sgccplots.sgc"
n_cycle)));
  
#line 525 "src/sgccplots.sgc"

#line 525 "src/sgccplots.sgc"

#line 525 "src/sgccplots.sgc"
plt->
#line 525 "src/sgccplots.sgc"
color_cycle++;
}


#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"
void scatter(
#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"
plot plt, 
#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 528 "src/sgccplots.sgc"

#line 528 "src/sgccplots.sgc"
string style)

#line 528 "src/sgccplots.sgc"
{
  
#line 529 "src/sgccplots.sgc"
scatter(
#line 529 "src/sgccplots.sgc"

#line 529 "src/sgccplots.sgc"
plt, 
#line 529 "src/sgccplots.sgc"
xs, 
#line 529 "src/sgccplots.sgc"
ys, 
#line 529 "src/sgccplots.sgc"
style, 
#line 529 "src/sgccplots.sgc"
1);
}


#line 532 "src/sgccplots.sgc"

#line 532 "src/sgccplots.sgc"

#line 532 "src/sgccplots.sgc"
void scatter(
#line 532 "src/sgccplots.sgc"

#line 532 "src/sgccplots.sgc"

#line 532 "src/sgccplots.sgc"
plot plt, 
#line 532 "src/sgccplots.sgc"

#line 532 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 532 "src/sgccplots.sgc"

#line 532 "src/sgccplots.sgc"
std__list__template__float ys)

#line 532 "src/sgccplots.sgc"
{
  
#line 533 "src/sgccplots.sgc"
scatter(
#line 533 "src/sgccplots.sgc"

#line 533 "src/sgccplots.sgc"
plt, 
#line 533 "src/sgccplots.sgc"
xs, 
#line 533 "src/sgccplots.sgc"
ys, 
#line 533 "src/sgccplots.sgc"
"o");
}


#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"
void scatter(
#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"
plot plt, 
#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 536 "src/sgccplots.sgc"

#line 536 "src/sgccplots.sgc"
plot_arg arg0)

#line 536 "src/sgccplots.sgc"
{
  
#line 537 "src/sgccplots.sgc"

#line 537 "src/sgccplots.sgc"
float radius = 
#line 537 "src/sgccplots.sgc"
1;
  
#line 538 "src/sgccplots.sgc"

#line 538 "src/sgccplots.sgc"
string color = 
#line 538 "src/sgccplots.sgc"
__index_wrapper__(
#line 538 "src/sgccplots.sgc"
color_cycle, 
#line 538 "src/sgccplots.sgc"
__mod__(
#line 538 "src/sgccplots.sgc"

#line 538 "src/sgccplots.sgc"

#line 538 "src/sgccplots.sgc"
plt->
#line 538 "src/sgccplots.sgc"
color_cycle, 
#line 538 "src/sgccplots.sgc"
n_cycle));
  
#line 539 "src/sgccplots.sgc"

#line 539 "src/sgccplots.sgc"
string style = 
#line 539 "src/sgccplots.sgc"
"o";
  
#line 540 "src/sgccplots.sgc"
switch (
#line 540 "src/sgccplots.sgc"

#line 540 "src/sgccplots.sgc"
arg0.
#line 540 "src/sgccplots.sgc"
t)

#line 540 "src/sgccplots.sgc"
  {
    
#line 541 "src/sgccplots.sgc"
case 
#line 541 "src/sgccplots.sgc"
ARG_STYLE:
      
#line 542 "src/sgccplots.sgc"

#line 542 "src/sgccplots.sgc"
style = 
#line 542 "src/sgccplots.sgc"

#line 542 "src/sgccplots.sgc"

#line 542 "src/sgccplots.sgc"
arg0.
#line 542 "src/sgccplots.sgc"
data.
#line 542 "src/sgccplots.sgc"
s;
      
#line 543 "src/sgccplots.sgc"
break;

    
#line 544 "src/sgccplots.sgc"
case 
#line 544 "src/sgccplots.sgc"
ARG_RADIUS:
      
#line 545 "src/sgccplots.sgc"

#line 545 "src/sgccplots.sgc"
radius = 
#line 545 "src/sgccplots.sgc"

#line 545 "src/sgccplots.sgc"

#line 545 "src/sgccplots.sgc"
arg0.
#line 545 "src/sgccplots.sgc"
data.
#line 545 "src/sgccplots.sgc"
f;
      
#line 546 "src/sgccplots.sgc"
break;

    
#line 547 "src/sgccplots.sgc"
case 
#line 547 "src/sgccplots.sgc"
ARG_COLOR:
      
#line 548 "src/sgccplots.sgc"

#line 548 "src/sgccplots.sgc"
color = 
#line 548 "src/sgccplots.sgc"

#line 548 "src/sgccplots.sgc"

#line 548 "src/sgccplots.sgc"
arg0.
#line 548 "src/sgccplots.sgc"
data.
#line 548 "src/sgccplots.sgc"
s;
      
#line 549 "src/sgccplots.sgc"
break;

    
#line 550 "src/sgccplots.sgc"
default:
      
#line 551 "src/sgccplots.sgc"
break;

  }

  
#line 553 "src/sgccplots.sgc"
scatter(
#line 553 "src/sgccplots.sgc"

#line 553 "src/sgccplots.sgc"
plt, 
#line 553 "src/sgccplots.sgc"
xs, 
#line 553 "src/sgccplots.sgc"
ys, 
#line 553 "src/sgccplots.sgc"
style, 
#line 553 "src/sgccplots.sgc"
radius, 
#line 553 "src/sgccplots.sgc"
color);
}


#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"
void scatter(
#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"
plot plt, 
#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"
std__list__template__float xs, 
#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"
std__list__template__float ys, 
#line 556 "src/sgccplots.sgc"

#line 556 "src/sgccplots.sgc"
plot_arg_list args)

#line 556 "src/sgccplots.sgc"
{
  
#line 557 "src/sgccplots.sgc"

#line 557 "src/sgccplots.sgc"
float radius = 
#line 557 "src/sgccplots.sgc"
1;
  
#line 558 "src/sgccplots.sgc"

#line 558 "src/sgccplots.sgc"
string color = 
#line 558 "src/sgccplots.sgc"
__index_wrapper__(
#line 558 "src/sgccplots.sgc"
color_cycle, 
#line 558 "src/sgccplots.sgc"
__mod__(
#line 558 "src/sgccplots.sgc"

#line 558 "src/sgccplots.sgc"

#line 558 "src/sgccplots.sgc"
plt->
#line 558 "src/sgccplots.sgc"
color_cycle, 
#line 558 "src/sgccplots.sgc"
n_cycle));
  
#line 559 "src/sgccplots.sgc"

#line 559 "src/sgccplots.sgc"
string style = 
#line 559 "src/sgccplots.sgc"
"o";
  
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
auto __iter_foreach_6 = 
#line 560 "src/sgccplots.sgc"
__iter__(
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
args);
  
#line 560 "src/sgccplots.sgc"
for (
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
auto arg0 = 
#line 560 "src/sgccplots.sgc"
__start__(
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
__iter_foreach_6); !
#line 560 "src/sgccplots.sgc"
__done__(
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
__iter_foreach_6); 
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
arg0 = 
#line 560 "src/sgccplots.sgc"
__next__(
#line 560 "src/sgccplots.sgc"

#line 560 "src/sgccplots.sgc"
__iter_foreach_6))

#line 560 "src/sgccplots.sgc"
  {
    
#line 561 "src/sgccplots.sgc"
switch (
#line 561 "src/sgccplots.sgc"

#line 561 "src/sgccplots.sgc"
arg0.
#line 561 "src/sgccplots.sgc"
t)

#line 561 "src/sgccplots.sgc"
    {
      
#line 562 "src/sgccplots.sgc"
case 
#line 562 "src/sgccplots.sgc"
ARG_STYLE:
        
#line 563 "src/sgccplots.sgc"

#line 563 "src/sgccplots.sgc"
style = 
#line 563 "src/sgccplots.sgc"

#line 563 "src/sgccplots.sgc"

#line 563 "src/sgccplots.sgc"
arg0.
#line 563 "src/sgccplots.sgc"
data.
#line 563 "src/sgccplots.sgc"
s;
        
#line 564 "src/sgccplots.sgc"
break;

      
#line 565 "src/sgccplots.sgc"
case 
#line 565 "src/sgccplots.sgc"
ARG_RADIUS:
        
#line 566 "src/sgccplots.sgc"

#line 566 "src/sgccplots.sgc"
radius = 
#line 566 "src/sgccplots.sgc"

#line 566 "src/sgccplots.sgc"

#line 566 "src/sgccplots.sgc"
arg0.
#line 566 "src/sgccplots.sgc"
data.
#line 566 "src/sgccplots.sgc"
f;
        
#line 567 "src/sgccplots.sgc"
break;

      
#line 568 "src/sgccplots.sgc"
case 
#line 568 "src/sgccplots.sgc"
ARG_COLOR:
        
#line 569 "src/sgccplots.sgc"

#line 569 "src/sgccplots.sgc"
color = 
#line 569 "src/sgccplots.sgc"

#line 569 "src/sgccplots.sgc"

#line 569 "src/sgccplots.sgc"
arg0.
#line 569 "src/sgccplots.sgc"
data.
#line 569 "src/sgccplots.sgc"
s;
        
#line 570 "src/sgccplots.sgc"
break;

      
#line 571 "src/sgccplots.sgc"
default:
        
#line 572 "src/sgccplots.sgc"
break;

    }

  }

  
#line 575 "src/sgccplots.sgc"
scatter(
#line 575 "src/sgccplots.sgc"

#line 575 "src/sgccplots.sgc"
plt, 
#line 575 "src/sgccplots.sgc"
xs, 
#line 575 "src/sgccplots.sgc"
ys, 
#line 575 "src/sgccplots.sgc"
style, 
#line 575 "src/sgccplots.sgc"
radius, 
#line 575 "src/sgccplots.sgc"
color);
}


#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"
void line(
#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"
plot plt, 
#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 581 "src/sgccplots.sgc"

#line 581 "src/sgccplots.sgc"
float thickness)

#line 581 "src/sgccplots.sgc"
{
  
#line 582 "src/sgccplots.sgc"
line(
#line 582 "src/sgccplots.sgc"

#line 582 "src/sgccplots.sgc"
plt, 
#line 582 "src/sgccplots.sgc"
xs, 
#line 582 "src/sgccplots.sgc"
ys, 
#line 582 "src/sgccplots.sgc"
thickness, 
#line 582 "src/sgccplots.sgc"
__index_wrapper__(
#line 582 "src/sgccplots.sgc"
color_cycle, 
#line 582 "src/sgccplots.sgc"
__mod__(
#line 582 "src/sgccplots.sgc"

#line 582 "src/sgccplots.sgc"

#line 582 "src/sgccplots.sgc"
plt->
#line 582 "src/sgccplots.sgc"
color_cycle, 
#line 582 "src/sgccplots.sgc"
n_cycle)));
  
#line 583 "src/sgccplots.sgc"

#line 583 "src/sgccplots.sgc"

#line 583 "src/sgccplots.sgc"
plt->
#line 583 "src/sgccplots.sgc"
color_cycle++;
}


#line 586 "src/sgccplots.sgc"

#line 586 "src/sgccplots.sgc"

#line 586 "src/sgccplots.sgc"
void line(
#line 586 "src/sgccplots.sgc"

#line 586 "src/sgccplots.sgc"

#line 586 "src/sgccplots.sgc"
plot plt, 
#line 586 "src/sgccplots.sgc"

#line 586 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 586 "src/sgccplots.sgc"

#line 586 "src/sgccplots.sgc"
std__list__template__int ys)

#line 586 "src/sgccplots.sgc"
{
  
#line 587 "src/sgccplots.sgc"
line(
#line 587 "src/sgccplots.sgc"

#line 587 "src/sgccplots.sgc"
plt, 
#line 587 "src/sgccplots.sgc"
xs, 
#line 587 "src/sgccplots.sgc"
ys, 
#line 587 "src/sgccplots.sgc"
1);
}


#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"
void line(
#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"
plot plt, 
#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 590 "src/sgccplots.sgc"

#line 590 "src/sgccplots.sgc"
plot_arg arg0)

#line 590 "src/sgccplots.sgc"
{
  
#line 591 "src/sgccplots.sgc"

#line 591 "src/sgccplots.sgc"
float thickness = 
#line 591 "src/sgccplots.sgc"
1;
  
#line 592 "src/sgccplots.sgc"

#line 592 "src/sgccplots.sgc"
string color = 
#line 592 "src/sgccplots.sgc"
__index_wrapper__(
#line 592 "src/sgccplots.sgc"
color_cycle, 
#line 592 "src/sgccplots.sgc"
__mod__(
#line 592 "src/sgccplots.sgc"

#line 592 "src/sgccplots.sgc"

#line 592 "src/sgccplots.sgc"
plt->
#line 592 "src/sgccplots.sgc"
color_cycle, 
#line 592 "src/sgccplots.sgc"
n_cycle));
  
#line 593 "src/sgccplots.sgc"
switch (
#line 593 "src/sgccplots.sgc"

#line 593 "src/sgccplots.sgc"
arg0.
#line 593 "src/sgccplots.sgc"
t)

#line 593 "src/sgccplots.sgc"
  {
    
#line 594 "src/sgccplots.sgc"
case 
#line 594 "src/sgccplots.sgc"
ARG_THICKNESS:
      
#line 595 "src/sgccplots.sgc"

#line 595 "src/sgccplots.sgc"
thickness = 
#line 595 "src/sgccplots.sgc"

#line 595 "src/sgccplots.sgc"

#line 595 "src/sgccplots.sgc"
arg0.
#line 595 "src/sgccplots.sgc"
data.
#line 595 "src/sgccplots.sgc"
f;
      
#line 596 "src/sgccplots.sgc"
break;

    
#line 597 "src/sgccplots.sgc"
case 
#line 597 "src/sgccplots.sgc"
ARG_COLOR:
      
#line 598 "src/sgccplots.sgc"

#line 598 "src/sgccplots.sgc"
color = 
#line 598 "src/sgccplots.sgc"

#line 598 "src/sgccplots.sgc"

#line 598 "src/sgccplots.sgc"
arg0.
#line 598 "src/sgccplots.sgc"
data.
#line 598 "src/sgccplots.sgc"
s;
      
#line 599 "src/sgccplots.sgc"
break;

    
#line 600 "src/sgccplots.sgc"
default:
      
#line 601 "src/sgccplots.sgc"
break;

  }

  
#line 603 "src/sgccplots.sgc"
line(
#line 603 "src/sgccplots.sgc"

#line 603 "src/sgccplots.sgc"
plt, 
#line 603 "src/sgccplots.sgc"
xs, 
#line 603 "src/sgccplots.sgc"
ys, 
#line 603 "src/sgccplots.sgc"
thickness, 
#line 603 "src/sgccplots.sgc"
color);
}


#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"
void line(
#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"
plot plt, 
#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 606 "src/sgccplots.sgc"

#line 606 "src/sgccplots.sgc"
plot_arg_list args)

#line 606 "src/sgccplots.sgc"
{
  
#line 607 "src/sgccplots.sgc"

#line 607 "src/sgccplots.sgc"
float thickness = 
#line 607 "src/sgccplots.sgc"
1;
  
#line 608 "src/sgccplots.sgc"

#line 608 "src/sgccplots.sgc"
string color = 
#line 608 "src/sgccplots.sgc"
__index_wrapper__(
#line 608 "src/sgccplots.sgc"
color_cycle, 
#line 608 "src/sgccplots.sgc"
__mod__(
#line 608 "src/sgccplots.sgc"

#line 608 "src/sgccplots.sgc"

#line 608 "src/sgccplots.sgc"
plt->
#line 608 "src/sgccplots.sgc"
color_cycle, 
#line 608 "src/sgccplots.sgc"
n_cycle));
  
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
auto __iter_foreach_7 = 
#line 609 "src/sgccplots.sgc"
__iter__(
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
args);
  
#line 609 "src/sgccplots.sgc"
for (
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
auto arg0 = 
#line 609 "src/sgccplots.sgc"
__start__(
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
__iter_foreach_7); !
#line 609 "src/sgccplots.sgc"
__done__(
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
__iter_foreach_7); 
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
arg0 = 
#line 609 "src/sgccplots.sgc"
__next__(
#line 609 "src/sgccplots.sgc"

#line 609 "src/sgccplots.sgc"
__iter_foreach_7))

#line 609 "src/sgccplots.sgc"
  {
    
#line 610 "src/sgccplots.sgc"
switch (
#line 610 "src/sgccplots.sgc"

#line 610 "src/sgccplots.sgc"
arg0.
#line 610 "src/sgccplots.sgc"
t)

#line 610 "src/sgccplots.sgc"
    {
      
#line 611 "src/sgccplots.sgc"
case 
#line 611 "src/sgccplots.sgc"
ARG_THICKNESS:
        
#line 612 "src/sgccplots.sgc"

#line 612 "src/sgccplots.sgc"
thickness = 
#line 612 "src/sgccplots.sgc"

#line 612 "src/sgccplots.sgc"

#line 612 "src/sgccplots.sgc"
arg0.
#line 612 "src/sgccplots.sgc"
data.
#line 612 "src/sgccplots.sgc"
f;
        
#line 613 "src/sgccplots.sgc"
break;

      
#line 614 "src/sgccplots.sgc"
case 
#line 614 "src/sgccplots.sgc"
ARG_COLOR:
        
#line 615 "src/sgccplots.sgc"

#line 615 "src/sgccplots.sgc"
color = 
#line 615 "src/sgccplots.sgc"

#line 615 "src/sgccplots.sgc"

#line 615 "src/sgccplots.sgc"
arg0.
#line 615 "src/sgccplots.sgc"
data.
#line 615 "src/sgccplots.sgc"
s;
        
#line 616 "src/sgccplots.sgc"
break;

      
#line 617 "src/sgccplots.sgc"
default:
        
#line 618 "src/sgccplots.sgc"
break;

    }

  }

  
#line 621 "src/sgccplots.sgc"
line(
#line 621 "src/sgccplots.sgc"

#line 621 "src/sgccplots.sgc"
plt, 
#line 621 "src/sgccplots.sgc"
xs, 
#line 621 "src/sgccplots.sgc"
ys, 
#line 621 "src/sgccplots.sgc"
thickness, 
#line 621 "src/sgccplots.sgc"
color);
}


#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"
void scatter(
#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"
plot plt, 
#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"
string style, 
#line 624 "src/sgccplots.sgc"

#line 624 "src/sgccplots.sgc"
float radius)

#line 624 "src/sgccplots.sgc"
{
  
#line 625 "src/sgccplots.sgc"
scatter(
#line 625 "src/sgccplots.sgc"

#line 625 "src/sgccplots.sgc"
plt, 
#line 625 "src/sgccplots.sgc"
xs, 
#line 625 "src/sgccplots.sgc"
ys, 
#line 625 "src/sgccplots.sgc"
style, 
#line 625 "src/sgccplots.sgc"
radius, 
#line 625 "src/sgccplots.sgc"
__index_wrapper__(
#line 625 "src/sgccplots.sgc"
color_cycle, 
#line 625 "src/sgccplots.sgc"
__mod__(
#line 625 "src/sgccplots.sgc"

#line 625 "src/sgccplots.sgc"

#line 625 "src/sgccplots.sgc"
plt->
#line 625 "src/sgccplots.sgc"
color_cycle, 
#line 625 "src/sgccplots.sgc"
n_cycle)));
  
#line 626 "src/sgccplots.sgc"

#line 626 "src/sgccplots.sgc"

#line 626 "src/sgccplots.sgc"
plt->
#line 626 "src/sgccplots.sgc"
color_cycle++;
}


#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"
void scatter(
#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"
plot plt, 
#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 629 "src/sgccplots.sgc"

#line 629 "src/sgccplots.sgc"
string style)

#line 629 "src/sgccplots.sgc"
{
  
#line 630 "src/sgccplots.sgc"
scatter(
#line 630 "src/sgccplots.sgc"

#line 630 "src/sgccplots.sgc"
plt, 
#line 630 "src/sgccplots.sgc"
xs, 
#line 630 "src/sgccplots.sgc"
ys, 
#line 630 "src/sgccplots.sgc"
style, 
#line 630 "src/sgccplots.sgc"
1);
}


#line 633 "src/sgccplots.sgc"

#line 633 "src/sgccplots.sgc"

#line 633 "src/sgccplots.sgc"
void scatter(
#line 633 "src/sgccplots.sgc"

#line 633 "src/sgccplots.sgc"

#line 633 "src/sgccplots.sgc"
plot plt, 
#line 633 "src/sgccplots.sgc"

#line 633 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 633 "src/sgccplots.sgc"

#line 633 "src/sgccplots.sgc"
std__list__template__int ys)

#line 633 "src/sgccplots.sgc"
{
  
#line 634 "src/sgccplots.sgc"
scatter(
#line 634 "src/sgccplots.sgc"

#line 634 "src/sgccplots.sgc"
plt, 
#line 634 "src/sgccplots.sgc"
xs, 
#line 634 "src/sgccplots.sgc"
ys, 
#line 634 "src/sgccplots.sgc"
"o");
}


#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"
void scatter(
#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"
plot plt, 
#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 637 "src/sgccplots.sgc"

#line 637 "src/sgccplots.sgc"
plot_arg arg0)

#line 637 "src/sgccplots.sgc"
{
  
#line 638 "src/sgccplots.sgc"

#line 638 "src/sgccplots.sgc"
float radius = 
#line 638 "src/sgccplots.sgc"
1;
  
#line 639 "src/sgccplots.sgc"

#line 639 "src/sgccplots.sgc"
string color = 
#line 639 "src/sgccplots.sgc"
__index_wrapper__(
#line 639 "src/sgccplots.sgc"
color_cycle, 
#line 639 "src/sgccplots.sgc"
__mod__(
#line 639 "src/sgccplots.sgc"

#line 639 "src/sgccplots.sgc"

#line 639 "src/sgccplots.sgc"
plt->
#line 639 "src/sgccplots.sgc"
color_cycle, 
#line 639 "src/sgccplots.sgc"
n_cycle));
  
#line 640 "src/sgccplots.sgc"

#line 640 "src/sgccplots.sgc"
string style = 
#line 640 "src/sgccplots.sgc"
"o";
  
#line 641 "src/sgccplots.sgc"
switch (
#line 641 "src/sgccplots.sgc"

#line 641 "src/sgccplots.sgc"
arg0.
#line 641 "src/sgccplots.sgc"
t)

#line 641 "src/sgccplots.sgc"
  {
    
#line 642 "src/sgccplots.sgc"
case 
#line 642 "src/sgccplots.sgc"
ARG_STYLE:
      
#line 643 "src/sgccplots.sgc"

#line 643 "src/sgccplots.sgc"
style = 
#line 643 "src/sgccplots.sgc"

#line 643 "src/sgccplots.sgc"

#line 643 "src/sgccplots.sgc"
arg0.
#line 643 "src/sgccplots.sgc"
data.
#line 643 "src/sgccplots.sgc"
s;
      
#line 644 "src/sgccplots.sgc"
break;

    
#line 645 "src/sgccplots.sgc"
case 
#line 645 "src/sgccplots.sgc"
ARG_RADIUS:
      
#line 646 "src/sgccplots.sgc"

#line 646 "src/sgccplots.sgc"
radius = 
#line 646 "src/sgccplots.sgc"

#line 646 "src/sgccplots.sgc"

#line 646 "src/sgccplots.sgc"
arg0.
#line 646 "src/sgccplots.sgc"
data.
#line 646 "src/sgccplots.sgc"
f;
      
#line 647 "src/sgccplots.sgc"
break;

    
#line 648 "src/sgccplots.sgc"
case 
#line 648 "src/sgccplots.sgc"
ARG_COLOR:
      
#line 649 "src/sgccplots.sgc"

#line 649 "src/sgccplots.sgc"
color = 
#line 649 "src/sgccplots.sgc"

#line 649 "src/sgccplots.sgc"

#line 649 "src/sgccplots.sgc"
arg0.
#line 649 "src/sgccplots.sgc"
data.
#line 649 "src/sgccplots.sgc"
s;
      
#line 650 "src/sgccplots.sgc"
break;

    
#line 651 "src/sgccplots.sgc"
default:
      
#line 652 "src/sgccplots.sgc"
break;

  }

  
#line 654 "src/sgccplots.sgc"
scatter(
#line 654 "src/sgccplots.sgc"

#line 654 "src/sgccplots.sgc"
plt, 
#line 654 "src/sgccplots.sgc"
xs, 
#line 654 "src/sgccplots.sgc"
ys, 
#line 654 "src/sgccplots.sgc"
style, 
#line 654 "src/sgccplots.sgc"
radius, 
#line 654 "src/sgccplots.sgc"
color);
}


#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"
void scatter(
#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"
plot plt, 
#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"
std__list__template__int xs, 
#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"
std__list__template__int ys, 
#line 657 "src/sgccplots.sgc"

#line 657 "src/sgccplots.sgc"
plot_arg_list args)

#line 657 "src/sgccplots.sgc"
{
  
#line 658 "src/sgccplots.sgc"

#line 658 "src/sgccplots.sgc"
float radius = 
#line 658 "src/sgccplots.sgc"
1;
  
#line 659 "src/sgccplots.sgc"

#line 659 "src/sgccplots.sgc"
string color = 
#line 659 "src/sgccplots.sgc"
__index_wrapper__(
#line 659 "src/sgccplots.sgc"
color_cycle, 
#line 659 "src/sgccplots.sgc"
__mod__(
#line 659 "src/sgccplots.sgc"

#line 659 "src/sgccplots.sgc"

#line 659 "src/sgccplots.sgc"
plt->
#line 659 "src/sgccplots.sgc"
color_cycle, 
#line 659 "src/sgccplots.sgc"
n_cycle));
  
#line 660 "src/sgccplots.sgc"

#line 660 "src/sgccplots.sgc"
string style = 
#line 660 "src/sgccplots.sgc"
"o";
  
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
auto __iter_foreach_8 = 
#line 661 "src/sgccplots.sgc"
__iter__(
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
args);
  
#line 661 "src/sgccplots.sgc"
for (
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
auto arg0 = 
#line 661 "src/sgccplots.sgc"
__start__(
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
__iter_foreach_8); !
#line 661 "src/sgccplots.sgc"
__done__(
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
__iter_foreach_8); 
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
arg0 = 
#line 661 "src/sgccplots.sgc"
__next__(
#line 661 "src/sgccplots.sgc"

#line 661 "src/sgccplots.sgc"
__iter_foreach_8))

#line 661 "src/sgccplots.sgc"
  {
    
#line 662 "src/sgccplots.sgc"
switch (
#line 662 "src/sgccplots.sgc"

#line 662 "src/sgccplots.sgc"
arg0.
#line 662 "src/sgccplots.sgc"
t)

#line 662 "src/sgccplots.sgc"
    {
      
#line 663 "src/sgccplots.sgc"
case 
#line 663 "src/sgccplots.sgc"
ARG_STYLE:
        
#line 664 "src/sgccplots.sgc"

#line 664 "src/sgccplots.sgc"
style = 
#line 664 "src/sgccplots.sgc"

#line 664 "src/sgccplots.sgc"

#line 664 "src/sgccplots.sgc"
arg0.
#line 664 "src/sgccplots.sgc"
data.
#line 664 "src/sgccplots.sgc"
s;
        
#line 665 "src/sgccplots.sgc"
break;

      
#line 666 "src/sgccplots.sgc"
case 
#line 666 "src/sgccplots.sgc"
ARG_RADIUS:
        
#line 667 "src/sgccplots.sgc"

#line 667 "src/sgccplots.sgc"
radius = 
#line 667 "src/sgccplots.sgc"

#line 667 "src/sgccplots.sgc"

#line 667 "src/sgccplots.sgc"
arg0.
#line 667 "src/sgccplots.sgc"
data.
#line 667 "src/sgccplots.sgc"
f;
        
#line 668 "src/sgccplots.sgc"
break;

      
#line 669 "src/sgccplots.sgc"
case 
#line 669 "src/sgccplots.sgc"
ARG_COLOR:
        
#line 670 "src/sgccplots.sgc"

#line 670 "src/sgccplots.sgc"
color = 
#line 670 "src/sgccplots.sgc"

#line 670 "src/sgccplots.sgc"

#line 670 "src/sgccplots.sgc"
arg0.
#line 670 "src/sgccplots.sgc"
data.
#line 670 "src/sgccplots.sgc"
s;
        
#line 671 "src/sgccplots.sgc"
break;

      
#line 672 "src/sgccplots.sgc"
default:
        
#line 673 "src/sgccplots.sgc"
break;

    }

  }

  
#line 676 "src/sgccplots.sgc"
scatter(
#line 676 "src/sgccplots.sgc"

#line 676 "src/sgccplots.sgc"
plt, 
#line 676 "src/sgccplots.sgc"
xs, 
#line 676 "src/sgccplots.sgc"
ys, 
#line 676 "src/sgccplots.sgc"
style, 
#line 676 "src/sgccplots.sgc"
radius, 
#line 676 "src/sgccplots.sgc"
color);
}


#line 682 "src/sgccplots.sgc"

#line 682 "src/sgccplots.sgc"

#line 682 "src/sgccplots.sgc"
void show(
#line 682 "src/sgccplots.sgc"

#line 682 "src/sgccplots.sgc"

#line 682 "src/sgccplots.sgc"
plot plt)

#line 682 "src/sgccplots.sgc"
{
  
#line 683 "src/sgccplots.sgc"
init_window(
#line 683 "src/sgccplots.sgc"

#line 683 "src/sgccplots.sgc"
plt);
  
#line 685 "src/sgccplots.sgc"
while (!
#line 685 "src/sgccplots.sgc"
WindowShouldClose())

#line 686 "src/sgccplots.sgc"
  {
    
#line 687 "src/sgccplots.sgc"
BeginDrawing();
    
#line 689 "src/sgccplots.sgc"
ClearBackground((
#line 0 "src/sgccplots.sgc"

#line 689 "src/sgccplots.sgc"
Color){
#line 689 "src/sgccplots.sgc"

#line 689 "src/sgccplots.sgc"
245, 
#line 689 "src/sgccplots.sgc"
245, 
#line 689 "src/sgccplots.sgc"
245, 
#line 689 "src/sgccplots.sgc"
255});
    
#line 691 "src/sgccplots.sgc"
draw_all(
#line 691 "src/sgccplots.sgc"

#line 691 "src/sgccplots.sgc"
plt);
    
#line 693 "src/sgccplots.sgc"
EndDrawing();
  }

  
#line 696 "src/sgccplots.sgc"
close(
#line 696 "src/sgccplots.sgc"

#line 696 "src/sgccplots.sgc"
plt);
}


template<typename T>
static inline const char* to_str(T in){
    return __str__(in);
}
