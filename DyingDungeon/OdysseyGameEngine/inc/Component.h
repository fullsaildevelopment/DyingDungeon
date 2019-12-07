#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class SceneObject;

	class Component
	{
	public:
		virtual void initialize(SceneObject* parent) = 0;
		virtual void update(double deltaTime);
		virtual void onEnable();
		virtual void onDisable();
		virtual SceneObject* getParent();
		void setEnabled(bool enable);
		bool isEnabled();
	protected:
		bool mEnabled;
		SceneObject* mParent;
	};
}