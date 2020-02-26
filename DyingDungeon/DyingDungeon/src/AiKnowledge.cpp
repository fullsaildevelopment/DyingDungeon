#include "AiKnowledge.h"
#include "Character.h"
#include "Skills.h"

AiKnowledge& AiKnowledge::getInstance()
{
	static AiKnowledge instance;
	return instance;
}

void AiKnowledge::UpdatePreviousMoves(Skills* _skill, Character* _target)
{
	if (previousMoves.size() != 4)
		for (int i = 0; i < 4; i++)
		{
			AiKnowledge::Move* temp;
			previousMoves.push_back(temp);
		}

	for (int i = previousMoves.size() - 1; i > 0; i--)
	{
		previousMoves[i]->skill = previousMoves[i - 1]->skill;
		previousMoves[i]->target = previousMoves[i - 1]->target;
	}

	previousMoves[0]->skill = _skill;
	previousMoves[0]->target = _target;
}

AiKnowledge::Move* AiKnowledge::GetMostRecentMove()
{
	if (previousMoves.size() < 1)
		return nullptr;

	return previousMoves[0];
}

AiKnowledge::Move* AiKnowledge::GetMoveAtIndex(int _index)
{
	if (_index >= 0 && _index <= previousMoves.size() - 1)
	{
		return previousMoves[_index];
	}
	
	return nullptr;
}

