#ifndef _H_PROCESS_ERROR_H_
#define _H_PROCESS_ERROR_H_

#include "log.h"

#define STATIC_ASSERT(__condition__) static_cast(__condition__, #__condition__);

#define PROCESS_SUCCESS(__condition__) \
	do								   \
	{								   \
		if (__condition__)			   \
		{							   \
			goto SuccessExit;		   \
		}							   \
	}while(0)

#define PROCESS_ERROR(__condition__)  \
	do								  \
	{								  \
		if(__condition__)			  \
		{							  \
									  \
		}							  \
		else						  \
		{							  \
			goto ErrorExit;			  \
		}                             \
	} while(0)	


#define LOG_PROCESS_ERROR(__condition__)  \
	do								  \
	{								  \
		if(__condition__)			  \
		{							  \
									  \
		}							  \
		else						  \
		{							  \
			ERR << #__condition__;	  \
			goto ErrorExit;			  \
		}                             \
	} while(0)

#define LOG_CHECK_ERROR(__condition__)  \
	do									\
	{									\
		if (__condition__)				\
		{								\
		}								\
		else							\
		{								\
			ERR << #__condition__;		\
		}								\
	} while (0)


#define LOG_PROCESS_WARN(__condition__)  \
	do								  \
	{								  \
		if(__condition__)			  \
		{							  \
									  \
		}							  \
		else						  \
		{							  \
			WAR << #__condition__;	  \
			goto WarnExit;			  \
		}                             \
	} while(0)

#endif // _H_PROCESS_ERROR_H_