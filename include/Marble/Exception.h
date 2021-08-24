#pragma once

#include <atomic>
#include <csetjmp>
#include <signal.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#if _WIN32
typedef struct IUnknown IUnknown; // *bad word* you objbase.h.
#include <cfloat>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN
#endif

#if (defined(_MSC_VER) && !defined(__clang__)) && defined(_WIN32)
#define __llex_msvc_win 1
#else
#define __llex_msvc_win 0
#endif

namespace Marble
{
    namespace Internal
    {
        struct JumpBuffer final
        {
            jmp_buf buffer;
        };

        struct ErrorString final
        {
            char str[1024] { 0 };
        };

        extern "C" void __cdecl signalHandler(int);
        #if _WIN32
        extern "C" void __cdecl arithmeticSignalHandler(int, int);
        LONG NTAPI vectoredExceptionHandler(_EXCEPTION_POINTERS*);
        BOOL WINAPI consoleCtrlHandler(DWORD);
        #else
        extern "C" void __cdecl interruptHandler(int);
        #endif
    }

    struct LowLevelException : public std::runtime_error
    {
        LowLevelException(const std::string& error) : std::runtime_error(error)
        {
        }
        virtual ~LowLevelException() = 0
        {
        }

        virtual const char* const what()
        {
            return ((std::runtime_error*)this)->what();
        }
    };
    class IllegalInstructionException final : public LowLevelException
    {
    public:
        IllegalInstructionException(const char* const __file, const char* const __function, int __line, void* data = nullptr) : LowLevelException([&]() -> Internal::ErrorString
        {
            #if _DEBUG
            Internal::ErrorString err;
            strcpy(err.str, "Illegal Instruction:\n\tException thrown:\n\t  in \"");
            strcpy(err.str + strlen(err.str), __file);
            strcpy(err.str + strlen(err.str), "\",\n\t  in function/ctor/dtor \"");
            strcpy(err.str + strlen(err.str), __function);
            strcpy(err.str + strlen(err.str), "\",\n\t  in try-block at line ");
            sprintf(err.str + strlen(err.str), "%i", __line);
            strcpy(err.str + strlen(err.str), ".\n\tAn illegal instruction signal was raised.\nSignal handler returned 4 (SIGILL).\nNote: This is often caused by executable file corruption.");
            return std::move(err);
            #else
            return { "Illegal instruction exception thrown.\nNote: This is often caused by executable file corruption." };
            #endif
        }().str)
        {
        }
        ~IllegalInstructionException()
        {
        }
    };
    class ArithmeticException final : public LowLevelException
    {
    public:
        ArithmeticException(const char* const __file, const char* const __function, int __line, void* data = nullptr) : LowLevelException([&]() -> Internal::ErrorString
        {
            #if _DEBUG
            Internal::ErrorString err;
            strcpy(err.str, "Arithmetic Error:\n\tException thrown:\n\t  in \"");
            strcpy(err.str + strlen(err.str), __file);
            strcpy(err.str + strlen(err.str), "\",\n\t  in function/ctor/dtor \"");
            strcpy(err.str + strlen(err.str), __function);
            strcpy(err.str + strlen(err.str), "\",\n\t  in try-block at line ");
            sprintf(err.str + strlen(err.str), "%i", __line);
            strcpy
            (
                err.str + strlen(err.str),
                ".\n\tAn arithmetic error occurred.\nSignal handler returned 8 (SIGFPE)"
                #if __llex_msvc_win
                ",\nor a vectored exception handler caught an arithmetic exception"
                #endif
                ".\nNote: This is often caused by an integer division by zero."
            );
            return std::move(err);
            #else
            return { "Arithmetic exception thrown.\nNote: This is often caused by an integer division by zero." };
            #endif
        }().str)
        {
        }
        ~ArithmeticException()
        {
        }
    };
    class SegmentationFaultException final : public LowLevelException
    {
    public:
        SegmentationFaultException(const char* const __file, const char* const __function, int __line, void* data = nullptr) : LowLevelException([&]() -> Internal::ErrorString
        {
            #if _DEBUG
            #if __llex_msvc_win
            PEXCEPTION_RECORD exrec = (PEXCEPTION_RECORD)data;
            Internal::ErrorString err;
            strcpy(err.str, "Segmentation Fault:\n\tException thrown:\n\t  in \"");
            strcpy(err.str + strlen(err.str), __file);
            strcpy(err.str + strlen(err.str), "\",\n\t  in function/ctor/dtor \"");
            strcpy(err.str + strlen(err.str), __function);
            strcpy(err.str + strlen(err.str), "\",\n\t  in try-block at line ");
            sprintf(err.str + strlen(err.str), "%i", __line);
            strcpy(err.str + strlen(err.str), ".\n\tA memory access violation was raised:\n\t  ");
            strcpy(err.str + strlen(err.str), exrec->ExceptionInformation[0] == 0 ? "read" : exrec->ExceptionInformation[0] == 1 ? "write" : "unknown");
            strcpy(err.str + strlen(err.str), " access violation of memory at ");
            sprintf(err.str + strlen(err.str), "0x%p", exrec->ExceptionAddress);
            strcpy(err.str + strlen(err.str), ",\n\t  the value of the attempted access at ");
            sprintf(err.str + strlen(err.str), "0x%p", exrec->ExceptionAddress);
            if (exrec->ExceptionInformation[1] == (uintptr_t)nullptr)
                strcpy(err.str + strlen(err.str), " was 0x00000000 (nullptr)");
            else
            {
                strcpy(err.str + strlen(err.str), " was ");
                sprintf(err.str + strlen(err.str), "0x%p", ((void*)exrec->ExceptionInformation[1]));
            }
            strcpy(err.str + strlen(err.str), ".\nSignal handler returned 11 (SIGSEGV).");
            return std::move(err);
            #else
            const char* errBeg = "Segmentation Fault:\n\tException thrown:\n\t  in \"";
            Internal::ErrorString err;
            strcpy(err.str, errBeg);
            strcpy(err.str + strlen(err.str), __file);
            strcpy(err.str + strlen(err.str), "\",\n\t  in function/ctor/dtor \"");
            strcpy(err.str + strlen(err.str), __function);
            strcpy(err.str + strlen(err.str), "\",\n\t  in try-block at line ");
            sprintf(err.str + strlen(err.str), "%i", __line);
            strcpy(err.str + strlen(err.str), ".\n\tA memory access violation was raised.\nSignal handler returned 11 (SIGSEGV).");
            return std::move(err);
            #endif
            #else
            return { "Segmentation fault exception thrown." };
            #endif
        }().str)
        {
        }
        virtual ~SegmentationFaultException()
        {
        }
    };
    class TerminateException final : public LowLevelException
    {
    public:
        TerminateException(const char* const __file, const char* const __function, int __line, void* data = nullptr) : LowLevelException([&]() -> Internal::ErrorString
        {
            #if _DEBUG
            Internal::ErrorString err;
            strcpy(err.str, "Terminate Request:\n\tException thrown:\n\t  in \"");
            strcpy(err.str + strlen(err.str), __file);
            strcpy(err.str + strlen(err.str), "\",\n\t  in function/ctor/dtor \"");
            strcpy(err.str + strlen(err.str), __function);
            strcpy(err.str + strlen(err.str), "\",\n\t  in try-block at line ");
            sprintf(err.str + strlen(err.str), "%i", __line);
            strcpy(err.str + strlen(err.str), ".\n\tA terminate request was raised.\nSignal handler returned 15 (SIGTERM).");
            return std::move(err);
            #else
            return { "Terminate request exception thrown." };
            #endif
        }().str)
        {
        }
        virtual ~TerminateException()
        {
        }
    };
    class AbortException final : public LowLevelException
    {
    public:
        AbortException(const char* const __file, const char* const __function, int __line, void* data = nullptr) : LowLevelException([&]() -> Internal::ErrorString
        {
            #if _DEBUG
            Internal::ErrorString err;
            strcpy(err.str, "Abort Signal:\n\tException thrown:\n\t  in \"");
            strcpy(err.str + strlen(err.str), __file);
            strcpy(err.str + strlen(err.str), "\",\n\t  in function/ctor/dtor \"");
            strcpy(err.str + strlen(err.str), __function);
            strcpy(err.str + strlen(err.str), "\",\n\t  in try-block at line ");
            sprintf(err.str + strlen(err.str), "%i", __line);
            strcpy(err.str + strlen(err.str), ".\n\tAn abort signal was raised.\nSignal handler returned 6/22 (SIGABRT).");
            return std::move(err);
            #else
            return { "Abort exception thrown." };
            #endif
        }().str)
        {
        }
        virtual ~AbortException()
        {
        }
    };

    class ExceptionHandler final
    {
        inline static struct Initializer final {
            Initializer()
            {
                signal(SIGILL, Marble::Internal::signalHandler);
                signal(SIGSEGV, Marble::Internal::signalHandler);
                signal(SIGTERM, Marble::Internal::signalHandler);
                signal(SIGABRT, Marble::Internal::signalHandler);

                #if _WIN32
                AddVectoredContinueHandler(1, Marble::Internal::vectoredExceptionHandler);
                SetConsoleCtrlHandler(Marble::Internal::consoleCtrlHandler, TRUE);
                signal(SIGFPE, (void(__cdecl*)(int))Marble::Internal::arithmeticSignalHandler);
                #else
                signal(SIGFPE, Marble::signalHandler);
                #endif
            }
            ~Initializer()
            {
                signal(SIGILL, SIG_IGN);
                signal(SIGFPE, SIG_IGN);
                signal(SIGSEGV, SIG_IGN);
                signal(SIGTERM, SIG_IGN);
                signal(SIGABRT, SIG_IGN);

                #if _WIN32
                RemoveVectoredContinueHandler(Marble::Internal::vectoredExceptionHandler);
                SetConsoleCtrlHandler(Marble::Internal::consoleCtrlHandler, FALSE);
                #endif
            }
        } init;
    public:
        inline static thread_local std::vector<Internal::JumpBuffer> threadBuffers { Internal::JumpBuffer() };
        inline static thread_local jmp_buf* threadBuf;
        inline static std::atomic_flag ctrlC = ATOMIC_FLAG_INIT;
        inline static std::atomic_flag ctrlBreak = ATOMIC_FLAG_INIT;

        static bool ctrlCInterruptThrown()
        {
            return ExceptionHandler::ctrlC.test(std::memory_order_acquire);
        }
        static bool ctrlBreakInterruptThrown()
        {
            return ExceptionHandler::ctrlBreak.test(std::memory_order_acquire);
        }
        static void resetCtrlCInterruptFlag()
        {
            ExceptionHandler::ctrlC.clear();
        }
        static void resetCtrlBreakInterruptFlag()
        {
            ExceptionHandler::ctrlBreak.clear();
        }

        ExceptionHandler() = delete;
    };

    namespace Internal
    {
        extern "C" void __cdecl signalHandler(int sig)
        {
            signal(sig, Marble::Internal::signalHandler);
            switch (sig)
            {
            #if !_WIN32
            case SIGFPE:
            #endif
            case SIGILL:
            case SIGSEGV:
            case SIGTERM:
            case SIGABRT:
                longjmp(*ExceptionHandler::threadBuf, sig);
                break;
            }
        }
        #if _WIN32
        extern "C" void __cdecl arithmeticSignalHandler(int sig, int err)
        {
            signal(SIGFPE, (void (__CRTDECL*)(int))Marble::Internal::arithmeticSignalHandler);
            longjmp(*ExceptionHandler::threadBuf, SIGFPE);
        }
        LONG NTAPI vectoredExceptionHandler(_EXCEPTION_POINTERS* exptr)
        {
            switch (exptr->ExceptionRecord->ExceptionCode)
            {
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                longjmp(*ExceptionHandler::threadBuf, SIGFPE);
                break;
            }
            return ExceptionContinueSearch;
        }
        BOOL WINAPI consoleCtrlHandler(DWORD ctrlEv)
        {
            switch (ctrlEv)
            {
            case CTRL_C_EVENT:
                ExceptionHandler::ctrlC.test_and_set();
                return TRUE;
                break;
            case CTRL_BREAK_EVENT:
                ExceptionHandler::ctrlBreak.test_and_set();
                return TRUE;
                break;
            }
            return FALSE;
        }
        #else
        extern "C" void __cdecl interruptHandler(int sig)
        {
            switch (sig)
            {
            case SIGINT:
                ExceptionHandler::data.ctrlC.test_and_set();
                break;
            case SIGBREAK:
                ExceptionHandler::data.ctrlBreak.test_and_set();
                break;
            }
        }
        #endif
    }
}

#if (defined (GNUC) || defined(__clang__))
#define __llex_function __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define __llex_function __FUNCSIG__
#else
#define __llex_function __func__
#endif

#if _DEBUG
#define __llex_fileData __FILE__, __llex_function, __LINE__
#if __llex_msvc_win
#define __llex_get_exrec EXCEPTION_RECORD __exrec = *((PEXCEPTION_POINTERS)_pxcptinfoptrs)->ExceptionRecord; PEXCEPTION_RECORD exrec = &__exrec
#else
#define __llex_get_exrec void* exrec = nullptr
#endif
#else
#define __llex_fileData "", "", 0
#define __llex_get_exrec void* exrec = nullptr
#endif

// Your punishment for trying to catch a nullptr exception is typing all this out everytime.
#define lowLevelExceptionsSectionBegin \
{ \
    Marble::ExceptionHandler::threadBuffers.push_back(Marble::Internal::JumpBuffer()); \
    Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
    switch (setjmp(*Marble::ExceptionHandler::threadBuf)) \
    { \
    case SIGILL: \
        { \
            Marble::ExceptionHandler::threadBuffers.pop_back(); \
            Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
            throw Marble::IllegalInstructionException(__llex_fileData); \
        } \
        break; \
    case SIGFPE: \
        { \
            Marble::ExceptionHandler::threadBuffers.pop_back(); \
            Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
            throw Marble::ArithmeticException(__llex_fileData); \
        } \
        break; \
    case SIGSEGV: \
        { \
            __llex_get_exrec; \
            Marble::ExceptionHandler::threadBuffers.pop_back(); \
            Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
            throw Marble::SegmentationFaultException(__llex_fileData, exrec); \
        } \
        break; \
    case SIGTERM: \
        { \
            Marble::ExceptionHandler::threadBuffers.pop_back(); \
            Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
            throw Marble::TerminateException(__llex_fileData); \
        } \
        break; \
    case SIGABRT: \
        { \
            Marble::ExceptionHandler::threadBuffers.pop_back(); \
            Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
            throw Marble::AbortException(__llex_fileData); \
        } \
        break; \
    }

#define lowLevelExceptionsSectionEnd \
    Marble::ExceptionHandler::threadBuffers.pop_back(); \
    Marble::ExceptionHandler::threadBuf = &Marble::ExceptionHandler::threadBuffers.back().buffer; \
}
