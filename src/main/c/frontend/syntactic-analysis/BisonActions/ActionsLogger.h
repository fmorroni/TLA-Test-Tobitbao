#ifndef ACTIONS_LOGGER_H
#define ACTIONS_LOGGER_H

void initializeBisonActionsModule();
void shutdownBisonActionsModule();
void logSyntacticAnalyzerAction(const char* functionName);
void logSyntacticAnalyzerPushAction(const char* functionName, const char* const format, ...);
void logActionDebug(const char* const format, ...);
void logActionError(const char* const format, ...);

#endif
