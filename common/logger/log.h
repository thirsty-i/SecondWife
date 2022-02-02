#pragma once

#include "easylogging++.h"

#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE


#define LOG_CHECK_ERROR(__condition__) \
	do											  \
	{											  \
		if(__condition__)						  \
		{}										  \
		else									  \
		{										  \
			LOG(ERROR) << "LOG_CHECK_ERROR("#__condition__")"; \
		}										  \
	} while (0)	


#define LOG_PROCESS_ERROR(__condition__) \
	do											  \
	{											  \
		if(__condition__)						  \
		{}										  \
		else									  \
		{										  \
			LOG(ERROR) << "LOG_PROCESS_ERROR("#__condition__")"; \
			return;								  \
		}										  \
	} while (0)	


#define LOG_PROCESS_ERROR_RET(__condition__, __ret__) \
	do											  \
	{											  \
		if(__condition__)						  \
		{}										  \
		else									  \
		{										  \
			LOG(ERROR) << "LOG_PROCESS_ERROR_RET("#__condition__")"; \
			return __ret__;						  \
		}										  \
	} while (0)									  \

void g_init_log(std::string && strAppName);