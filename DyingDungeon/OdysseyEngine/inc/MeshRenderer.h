#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "Component.h"
#include "ReadWriteLock.h"

namespace Odyssey
{
	// Forward declarations
	class Mesh;
	class Material;
	class RenderState;

	class MeshRenderer : public Component
	{
		CLASS_DECLARATION(MeshRenderer)
	public:
		virtual std::shared_ptr<Component> clone() const;
	public: // Rule of 3
		MeshRenderer();
		MeshRenderer(int meshID, int materialID);

	public: // Interface
		/**
		 *	Set the mesh renderer's mesh object.
		 *	@param[in] material The mesh renderer's mesh object.
		 *	@return void
		 */
		void setMesh(int meshID);

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
		void setMaterial(int materialID);

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
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		/**
		 *	Unbind the mesh renderer from the rendering pipeline.
		 *	@param[in] void
		 *	@return void
		 */
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	private: // Members
		bool mDebugEnabled;
		int mMesh;
		int mMaterial;
		ReadWriteLock mLock;
	};
}