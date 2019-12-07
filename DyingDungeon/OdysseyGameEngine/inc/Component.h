#pragma once
#include "EngineIncludes.h"

#define TO_STRING( x ) #x

//****************
// CLASS_DECLARATION
//
// This macro must be included in the declaration of any subclass of Component.
// It declares variables used in type checking.
//****************
#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool isClassType( const std::size_t classType ) const override;

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define CLASS_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::isClassType( const std::size_t classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::isClassType( classType );                                       \
}  

namespace Odyssey
{
	class SceneObject;

	class Component
	{
	public:
		static const std::size_t Type;
		virtual ~Component() = default;
		virtual bool isClassType(const std::size_t classType) const;
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