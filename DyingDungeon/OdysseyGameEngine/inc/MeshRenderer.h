#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	class Mesh;
	class Material;
	class RenderState;

	class MeshRenderer : public Component
	{
		CLASS_DECLARATION(Animator)
	public:
		MeshRenderer();
		MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

		/**
		 *	Initialize the MeshRenderer with default values. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Set the MeshRenderer's active state.
		 *	@param[in] active The new active state of the MeshRenderer.
		 *	@return void
		 */
		void setActive(bool active);

		/**
		 *	Get the MeshRenderer's active state.
		 *	@param[in] void
		 *	@return bool The active state of the MeshRenderer.
		 */
		bool getActive();

		/**
		 *	Set Mesh object associated with the MeshRenderer.
		 *	@param[in] mesh The new Mesh object to be rendered with this MeshRenderer.
		 *	@return void
		 */
		void setMesh(std::shared_ptr<Mesh> mesh);

		/**
		 *	Get the raw Mesh object associated with the MeshRenderer.
		 *	@param[in] void
		 *	@return Mesh* Raw pointer to the Mesh object associated with the MeshRenderer.
		 */
		Mesh* getMesh();

		/**
		 *	Set Material object associated with the MeshRenderer.
		 *	@param[in] material The new Material object to be rendered with this MeshRenderer.
		 *	@return void
		 */
		void setMaterial(std::shared_ptr<Material> material);

		/**
		 *	Get the raw Material object associated with the MeshRenderer.
		 *	@param[in] void
		 *	@return Material* Raw pointer to the Material object associated with the MeshRenderer.
		 */
		Material* getMaterial();

		/**
		 *	Set the MeshRenderer's debug state.
		 *	@param[in] enabled The new debug state of the MeshRenderer.
		 *	@return void
		 */
		void setDebugEnabled(bool enabled);

		/**
		 *	Get the MeshRenderer's debug state.
		 *	@param[in] void
		 *	@return bool The debug state of the MeshRenderer.
		 */
		bool getDebugEnabled();

		/**
		 *	Bind the MeshRenderer to the rendering pipeline.
		 *	@param[in] void
		 *	@return void
		 */
		void bind();

		/**
		 *	Unbind the MeshRenderer from the rendering pipeline.
		 *	@param[in] void
		 *	@return void
		 */
		void unbind();
	private:
		bool mIsActive;
		bool mDebugEnabled;
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
	};
}