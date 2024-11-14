#include "../../../shared/Array.h"
#include "../../../shared/Logger.h"
#include "../AbstractSyntaxTree.h"
#include <stdlib.h>

static Logger* _logger = NULL;

void initializeProgramUtilsModules() {
  _logger = createLogger("ProgramUtils");
}

void shutdownProgramUtilsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

void Program_free(Program* program) {
  logDebugging(_logger, "Executing destructor: %s", __func__);
  Array_free(program->sentences);
  free(program);
}
