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

// Functions
public:
	// Update previous moves. Only stores the past 4 moves.
	void UpdatePreviousMoves(Skills* _skill, Character* _target);

	// Gets the most recent move which is at index 0. If it does not exist it return nullptr
	AiKnowledge::Move* GetMostRecentMove();

	// Returns the move at that index. If it does not exist it return nullptr
	AiKnowledge::Move* GetMoveAtIndex(int _index);

// Variables
private:
	std::vector<AiKnowledge::Move*> previousMoves;

};