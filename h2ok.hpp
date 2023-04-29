#pragma once
#if H2OK_BUILD_MODE ==  1
#define H2OK_API extern "C" __declspec(dllexport)
#else
#define H2OK_API extern "C" __declspec(dllimport)
#endif	

typedef struct _H2OKCONTEXT {} H2OKContext, * PH2OKContext, ** LPH2OKContext;

typedef int H2OKResult;
typedef void (*H2OKRunHookCallback)(PH2OKContext);

typedef struct _H2OKRUNHOOKCALLBACKREGISTRAR {} H2OKRunHookCallbackRegistrar,*PH2OKRunHookCallbackRegistrar,**LPH2OKRunHookCallbackRegistrar;



#define H2OKRESULT_SUCCESS 1
#define H2OKRESULT_FAILURE 0

#define H2OKFAILED(result) (result==H2OKRESULT_FAILURE)
#define H2OKSUCCESS(result) (result==H2OKRESULT_SUCCESS)
#define H2OKNULL(result) (result == nullptr)
#define H2OKSAME(r1,r2) (r1==r2)


H2OK_API H2OKResult GetDefaultH2OKRunHookCallbackRegistrar(LPH2OKRunHookCallbackRegistrar);
H2OK_API H2OKResult GetH2OKRunHookCallbackRegistrarCount(PH2OKRunHookCallbackRegistrar, int*);
H2OK_API H2OKResult GetH2OKRunHookCallback(PH2OKRunHookCallbackRegistrar, int, H2OKRunHookCallback*);
H2OK_API H2OKResult RegisterH2OKRunHookCallback(PH2OKRunHookCallbackRegistrar, H2OKRunHookCallback);
H2OK_API H2OKResult DeleteH2OKRunHookCallback(PH2OKRunHookCallbackRegistrar, H2OKRunHookCallback);

H2OK_API H2OKResult CreateContext(LPH2OKContext);
H2OK_API H2OKResult RunHook(PH2OKContext);
H2OK_API H2OKResult DestroyContext(PH2OKRunHookCallbackRegistrar);
H2OK_API H2OKResult SetCurrentContext(PH2OKRunHookCallbackRegistrar,PH2OKContext);

