#pragma once
#include "EngineIncludes.h"

#pragma region Component Macros

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

#pragma endregion

namespace Odyssey
{
	// Forward declarations
	class Entity;

	class Component
	{
	public: // Type identifers
		static const std::size_t Type;
		virtual bool isClassType(const std::size_t classType) const;

	public: // Rule of 3
		Component();
		~Component();

	public: // Interface
		/**
		 *	Virtual function to allow for initialization logic. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Virtual function to allow for per-frame update logic. Called once per frame while the scene is loaded.
		 *	@param[in] deltaTime The time between frames.
		 *	@return void
		 */
		virtual void update(double deltaTime);

		/**
		 *	Get the entity the component is attached to.
		 *	@param[in] void
		 *	@return Entity* A raw pointer to the entity the component is attached to
		 */
		virtual Entity* getEntity();

		/**
		 *	Set the entity the component is attached to.
		 *	@param[in] Entity The entity the component is attached to
		 *	@return void
		 */
		void setEntity(Entity* Entity);

		/**
		 *	Set the component's active state.
		 *	@param[in] enable The active state of the component.
		 *	@return void
		 */
		void setActive(bool active);

		/**
		 *	Get the component's active state.
		 *	@param[in] void
		 *	@return bool The active state of the component
		 */
		bool isActive();

	protected:
		void lock(bool isWrite);
		void unlock(bool isWrite);
	protected: // Members
		bool mActive;
		Entity* mEntity;
		SRWLOCK mComponentLock;
	};
}