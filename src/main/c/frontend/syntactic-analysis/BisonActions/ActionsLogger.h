#ifndef ACTIONS_LOGGER_H
#define ACTIONS_LOGGER_H

void initializeBisonActionsModule();
void shutdownBisonActionsModule();
void logSyntacticAnalyzerAction(const char* functionName);
void logSyntacticAnalyzerPushAction(const char* functionName, const char* const format, ...);
void logAlreadyDefinedError(const char* functionName, const char* id);
void logUndefined(const char* functionName, const char* id);
void logInvalidType(const char* functionName, const char* id, const char* expectedType, const char* realType);
void logActionDebug(const char* const format, ...);
void logActionError(const char* const format, ...);

#endif
