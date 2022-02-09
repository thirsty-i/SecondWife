#include "log.h"

void g_init_log(std::string&& strAppName)
{
	std::string config_path = "../../configs/" + strAppName + ".conf";

	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);


	el::Configurations conf(config_path);
	el::Loggers::reconfigureAllLoggers(conf);
}