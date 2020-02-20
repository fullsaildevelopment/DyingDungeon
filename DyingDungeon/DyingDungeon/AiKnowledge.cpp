#include "AiKnowledge.h"

AiKnowledge& AiKnowledge::getInstance()
{
	static AiKnowledge instance;
	return instance;
}

