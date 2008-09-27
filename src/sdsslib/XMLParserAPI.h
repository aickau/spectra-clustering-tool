#ifndef __XMLPARSERAPI_H__
#define __XMLPARSERAPI_H__

#ifdef _XMLDLL

#ifdef _WIN32

// microsoft specific storage-class defines
# ifdef XMLPARSER_EXPORTS
#  define XMLPARSERAPI __declspec(dllexport)
#  define EXPIMP_TEMPLATE_XMLPARSERAPI
# else
#  define XMLPARSERAPI __declspec(dllimport)
#  define EXPIMP_TEMPLATE_XMLPARSERAPI extern
# endif

#else

# define XMLPARSERAPI
# define EXPIMP_TEMPLATE_XMLPARSERAPI
#endif

#else
# define XMLPARSERAPI
#endif


#endif // __XMLPARSERAPI_H__