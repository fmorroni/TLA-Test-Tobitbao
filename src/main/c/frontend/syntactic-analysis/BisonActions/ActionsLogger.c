#include "../../../shared/Logger.h"
#include "../../../shared/String.h"
#include <stdarg.h>
#include <stdlib.h>

Logger* bisonActionsLogger = NULL;

void initializeBisonActionsModule() {
  bisonActionsLogger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
  if (bisonActionsLogger != NULL) {
    destroyLogger(bisonActionsLogger);
  }
}

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
void logSyntacticAnalyzerAction(const char* functionName) {
  logDebugging(bisonActionsLogger, "%s", functionName);
}

void logSyntacticAnalyzerPushAction(const char* functionName, const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  char* effectiveFormat = safeAsprintf("%s(%s)", functionName, format);
  vlogDebugging(bisonActionsLogger, effectiveFormat, arguments);
  free(effectiveFormat);
  va_end(arguments);
}

void logAlreadyDefinedError(const char* functionName, const char* id) {
  logError(bisonActionsLogger, "%s: %s already defined.", functionName, id);
}

void logUndefined(const char* functionName, const char* id) {
  logError(bisonActionsLogger, "%s: %s isn't defined.", functionName, id);
}

void logInvalidType(const char* functionName, const char* id, const char* expectedType, const char* realType) {
  logError(
    bisonActionsLogger, "%s: %s expected to be of type `%s` but was of type `%s`", functionName, id, expectedType,
    realType
  );
}

void logActionDebug(const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  vlogDebugging(bisonActionsLogger, format, arguments);
  va_end(arguments);
}

void logActionError(const char* const format, ...) {
  va_list arguments;
  va_start(arguments, format);
  vlogError(bisonActionsLogger, format, arguments);
  va_end(arguments);
}
