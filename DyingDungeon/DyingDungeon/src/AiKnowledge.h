#pragma once
#include <vector>

class Character;
class Skills;

class AiKnowledge
{
	struct Move
	{
		Skills* skill = nullptr;
		Character* target = nullptr;
	};

public:
	static AiKnowledge& getInstance();
	~AiKnowledge() { }

// Singleton pattern
private: 
	AiKnowledge() { }

//Functions
public:
	//Update previous moves. Only stores the past 4 moves.
	void UpdatePreviousMoves(Skills* _skill, Character* _target);
	AiKnowledge::Move* GetMostRecentMove();
	AiKnowledge::Move* GetMoveAtIndex(int _index);

//Variables
private:
	std::vector<AiKnowledge::Move*> previousMoves;

};