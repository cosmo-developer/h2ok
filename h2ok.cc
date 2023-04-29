#include "h2ok.hpp"
#include <iostream>
#include <list>



struct H2OKRunHookCallbackRegistrar_Internal : public H2OKRunHookCallbackRegistrar {
    PH2OKContext GlobalContext;
    std::list<H2OKRunHookCallback> callbacks;

    static H2OKRunHookCallbackRegistrar* GlobalRegistrar;
};

H2OKRunHookCallbackRegistrar* H2OKRunHookCallbackRegistrar_Internal::GlobalRegistrar = nullptr;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
struct H2OKContext_Platform_Win32 : public H2OKContext {
	HHOOK KeyBoardHook;
	HHOOK MouseHook;
    HANDLE InterruptEventHandle;
};

#define WIN32_CODE(code)\
	code

#define X11_CODE(code)


#else
#include <X11/Xlib.h>

#define WIN32_CODE(code)

#define X11_CODE(code)\
	code

struct H2OKContext_Platform_X11 : public H2OKContext {

};
#endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

struct GlobalContext {
    static PH2OKContext context;
};

PH2OKContext GlobalContext::context = nullptr;

H2OK_API H2OKResult CreateContext(LPH2OKContext context){



	//Win32 specific code
    WIN32_CODE(
        //Creating Mouse Hook
        HHOOK hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, [](int nCode, WPARAM wParam, LPARAM lParam)->LRESULT CALLBACK {
            
            // Call the next hook in the chain
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }, GetModuleHandle(NULL), 0);



        //In case can't create Mouse Hook
        if (!hMouseHook) return H2OKRESULT_FAILURE;


        //Create InterruptEventHandler
        HANDLE InterruptEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

        //In case can't create InterruptEventHandle
        if (!InterruptEventHandle) return H2OKRESULT_FAILURE;


        //Finally creating Context instance for win32 platform
	    H2OKContext_Platform_Win32 * ctx = new H2OKContext_Platform_Win32;


        //Setting up
        ctx->InterruptEventHandle = InterruptEventHandle;
        ctx->MouseHook = hMouseHook;

        //Assigning to output args
        *context = ctx;
    );


	//X11 specific code
	X11_CODE(
		H2OKContext_Platform_X11 * ctx = new H2OKContext_Platform_X11;
	);


	//Common Code
	return H2OKRESULT_SUCCESS;
}

H2OK_API H2OKResult DestroyContext(PH2OKContext context) {

    std::cout << "Deleting Context!" << std::endl;

    //Check if context is null or not initialized or already destroyed
    if (H2OKNULL(context)) return H2OKRESULT_FAILURE;

    WIN32_CODE(
        H2OKContext_Platform_Win32 * ctx = reinterpret_cast<H2OKContext_Platform_Win32*>(context);
        SetEvent(ctx->InterruptEventHandle);
        UnhookWindowsHookEx(ctx->MouseHook);
        CloseHandle(ctx->InterruptEventHandle);
    );


    delete[] context;

    return H2OKRESULT_SUCCESS;
}

H2OK_API H2OKResult RunHook(PH2OKContext context, H2OKRunHookCallback callback) {
    if (H2OKNULL(context)) return H2OKRESULT_FAILURE;

    if (H2OKNULL(callback)) return H2OKRESULT_FAILURE;

    WIN32_CODE(
        H2OKContext_Platform_Win32 * ctx = reinterpret_cast<H2OKContext_Platform_Win32*>(context);
     
    );

    std::list<int> what;
    
   

    return H2OKRESULT_SUCCESS;
}




H2OK_API H2OKResult GetDefaultH2OKRunHookCallbackRegistrar(LPH2OKRunHookCallbackRegistrar lpRegistrar) {
    if (H2OKFAILED(H2OKNULL(H2OKRunHookCallbackRegistrar_Internal::GlobalRegistrar))) {
        *lpRegistrar = H2OKRunHookCallbackRegistrar_Internal::GlobalRegistrar;
        return H2OKRESULT_SUCCESS;
    }

    H2OKRunHookCallbackRegistrar_Internal::GlobalRegistrar = new H2OKRunHookCallbackRegistrar_Internal;
    
    //Checking that properly intialized or stay null Rarest case check
    if (H2OKNULL(H2OKRunHookCallbackRegistrar_Internal::GlobalRegistrar)) return H2OKRESULT_FAILURE;


    *lpRegistrar = H2OKRunHookCallbackRegistrar_Internal::GlobalRegistrar;

    return H2OKRESULT_SUCCESS;
}



H2OK_API H2OKResult GetH2OKRunHookCallbackRegistrarCount(PH2OKRunHookCallbackRegistrar pRegistrar, int* pInt) {
    if (H2OKNULL(pRegistrar)) return H2OKRESULT_FAILURE;
    
    auto InternalRegistrar = reinterpret_cast<H2OKRunHookCallbackRegistrar_Internal*>(pRegistrar);

    *pInt = InternalRegistrar->callbacks.size();

    return H2OKRESULT_SUCCESS;
}



H2OK_API H2OKResult GetH2OKRunHookCallback(PH2OKRunHookCallbackRegistrar pRegistrar, int Index, H2OKRunHookCallback* pCallback) {

    int count;

    if (H2OKFAILED(GetH2OKRunHookCallbackRegistrarCount(pRegistrar, &count))) {
        return H2OKRESULT_FAILURE;
    }

    if (Index >= count ||  Index < 0) {
        return H2OKRESULT_FAILURE;
    }

    auto InternalRegistrar = reinterpret_cast<H2OKRunHookCallbackRegistrar_Internal*>(pRegistrar);


    auto iterator=InternalRegistrar->callbacks.begin();

    std::advance(iterator, Index);

    if (H2OKNULL(*iterator)) return H2OKRESULT_FAILURE;

    *pCallback = *iterator;

    std::advance(iterator, 0);

    return H2OKRESULT_SUCCESS;

}




H2OK_API H2OKResult RegisterH2OKRunHookCallback(PH2OKRunHookCallbackRegistrar pRegistrar, H2OKRunHookCallback callback) {
    if (H2OKNULL(callback) || H2OKNULL(pRegistrar)) return H2OKRESULT_FAILURE;


    auto InternalRegistrar = reinterpret_cast<H2OKRunHookCallbackRegistrar_Internal*>(pRegistrar);

    for (auto callbck : InternalRegistrar->callbacks) {
        if (H2OKSAME(callbck, callback)) return H2OKRESULT_FAILURE;
    }

    InternalRegistrar->callbacks.push_back(callback);

    std::cout << "Adding Callback" << (InternalRegistrar->callbacks.size()) << std::endl;

    return H2OKRESULT_SUCCESS;
}




H2OK_API H2OKResult DeleteH2OKRunHookCallback(PH2OKRunHookCallbackRegistrar, H2OKRunHookCallback) {

    return H2OKRESULT_SUCCESS;
}