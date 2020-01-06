#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	// Forward declarations
	class Mesh;
	class Material;
	class RenderState;

	class MeshRenderer : public Component
	{
		CLASS_DECLARATION(Animator)

	public: // Rule of 3
		MeshRenderer();
		MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	public: // Interface
		/**
		 *	Set the mesh renderer's mesh object.
		 *	@param[in] material The mesh renderer's mesh object.
		 *	@return void
		 */
		void setMesh(std::shared_ptr<Mesh> mesh);

		/**
		 *	Get the mesh renderer's mesh object.
		 *	@param[in] void
		 *	@return Material* A raw pointer to the mesh renderer's mesh object.
		 */
		Mesh* getMesh();

		/**
		 *	Set the mesh renderer's material object.
		 *	@param[in] material The mesh renderer's material object.
		 *	@return void
		 */
		void setMaterial(std::shared_ptr<Material> material);

		/**
		 *	Get the mesh renderer's material object.
		 *	@param[in] void
		 *	@return Material* A raw pointer to the mesh renderer's material object.
		 */
		Material* getMaterial();

		/**
		 *	Set the mesh renderer's debug state.
		 *	@param[in] The mesh renderer's debug state.
		 *	@return void 
		 */
		void setDebugEnabled(bool enabled);

		/**
		 *	Get the mesh renderer's debug state.
		 *	@param[in] void
		 *	@return bool The mesh renderer's debug state.
		 */
		bool getDebugEnabled();

		/**
		 *	Bind the mesh renderer to the rendering pipeline.
		 *	@param[in] void
		 *	@return void
		 */
		void bind();

		/**
		 *	Unbind the mesh renderer from the rendering pipeline.
		 *	@param[in] void
		 *	@return void
		 */
		void unbind();

	private: // Members
		bool mDebugEnabled;
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
	};
}