#ifndef SENTENCE_UTILS_H
#define SENTENCE_UTILS_H

#include "../../../shared/ArrayElement.h"
#include "../AbstractSyntaxTree.h"

void initializeSentenceUtilsModule();
void shutdownSenteceUtilsModule();

void SentenceArray_freeEle(ArrayElement ele);
char* Sentence_toString(Sentence* sentence);

#endif
