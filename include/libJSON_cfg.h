/*********************************************************************************************
*	Name		: libJSON_cfg.h
*	Description	: Configuration of the project
********************************************************************************************/

#pragma once
#ifndef _LIBJSON_CFG_H_
#define _LIBJSON_CFG_H_

//If the solution is a dinamic library (dll), we need the next macro
#define LIBJSON_DLL

//IMPORTANT: the project who exports must have the preprocessor macro LIBJSON_EXPORTS

//see http://gcc.gnu.org/wiki/Visibility

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define LIBJSON_HELPER_DLL_IMPORT __declspec(dllimport)
#define LIBJSON_HELPER_DLL_EXPORT __declspec(dllexport)
#define LIBJSON_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define LIBJSON_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define LIBJSON_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define LIBJSON_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define LIBJSON_HELPER_DLL_IMPORT
#define LIBJSON_HELPER_DLL_EXPORT
#define LIBJSON_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define LIBJSON_API and LIBJSON_LOCAL.
// LIBJSON_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// LIBJSON_LOCAL is used for non-api symbols.

#ifdef LIBJSON_DLL // defined if LIBJSON is compiled as a DLL
#ifdef LIBJSON_EXPORTS // defined if we are building the LIBJSON DLL (instead of using it)
#define LIBJSON_API LIBJSON_HELPER_DLL_EXPORT
#else
#define LIBJSON_API LIBJSON_HELPER_DLL_IMPORT
#endif // LIBJSON_EXPORTS
#define LIBJSON_LOCAL LIBJSON_HELPER_DLL_LOCAL
#else // LIBJSON_DLL is not defined: this means LIBJSON is a static lib.
#define LIBJSON_API
#define LIBJSON_LOCAL
#endif // LIBJSON_DLL




#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif


#endif //_LIBJSON_CFG_H_
