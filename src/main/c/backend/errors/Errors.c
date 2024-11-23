#include "../../shared/ColorMacros.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include <stdbool.h>
#include <stddef.h>

static Logger* _logger = NULL;
static bool* errors;

void initializeErrorsModule(CompilerState* state) {
  _logger = createLogger("ErrorLogger");
  errors = &state->errors;
}

void shutdownErrorsModule() {
  destroyLogger(_logger);
}

void Error_IncompatibleType(const char* functionName, const char* id) {
  *errors = true;
  logError(_logger, "%s: " COLORIZE_ID("%s") " isn't defined.", functionName, id);
}

void Error_AlreadyDefined(const char* functionName, const char* id) {
  *errors = true;
  logError(_logger, "%s: " COLORIZE_ID("%s") " already defined.", functionName, id);
}
