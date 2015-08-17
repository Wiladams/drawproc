#if defined(BUILD_AS_DLL)
#if defined(GRC_CORE) || defined(GRC_LIB)
#define GRC_API		__declspec(dllexport)
#else
#define GRC_API		__declspec(dllimport)
#endif
#else
#define GRC_API		extern
#ifdef _WIN32
//#pragma comment(lib,"graphicc.lib")
#endif
#endif

#ifdef _MSC_VER
#include <intrin.h>
#endif

typedef float	float32;
typedef double	float64;
typedef float32 real;
typedef float coord;

typedef real real2[2];
typedef real real3[3];
typedef real real4[4];
