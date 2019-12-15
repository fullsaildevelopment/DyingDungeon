#pragma once
#include "EngineIncludes.h"
#include "RenderDevice.h"
#include "RenderPipeline.h"

namespace Odyssey
{
	class RenderPass;
	class RenderWindow;
	class Scene;

	class Application
	{
	public:
		Application();
	public:
		/**
		 *	Create a new window for rendering objects.
		 *	@param[in] title The title of the window.
		 *	@param[in] windowWidth The desired width of the window.
		 *	@param[in] windowHeight The desired height of the window.
		 *	@return shared_ptr<RenderWindow> The pointer to the created RenderWindow object.
		 */
		std::shared_ptr<RenderWindow> createRenderWindow(const std::string& title, int windowWidth, int windowHeight);

		/**
		 *	Set the active scene to be initialized and updated.
		 *	@param[in] scene The scene to set as active.
		 *	@return void
		 */
		void setActiveScene(std::shared_ptr<Scene> scene);

		/**
		 *	Add a rendering pass to the rendering pipeline.
		 *	@param[in] renderPass The rendering pass to add to the rendering pipeline.
		 *	@return void
		 */
		void addRenderPass(std::shared_ptr<RenderPass> renderPass);

		/**
		 *	Execute update logic of the Application.
		 *	@param[in] void
		 *	@return int The windows result of exiting the application.
		 */
		int run();

		/**
		 *	Stop the update logic of the Application.
		 *	@param[in] void
		 *	@return void
		 */
		void stop();

		/**
		 *	Get a raw pointer to the RenderDevice object within the Application.
		 *	The RenderDevice is used for object and resource creation.
		 *	@param[in] void
		 *	@return Render* The raw pointer to the RenderDevice object.
		 */
		RenderDevice* getRenderDevice();

		/**
		 *	Get the module handle associated with the Application.
		 *	@param[in] void
		 *	@return HINSTANCE The module handle associated with the Application.
		 */
		HINSTANCE GetModuleHandle() const;
	private:
		// Window Management
		std::vector<std::shared_ptr<RenderWindow>> mWindows;
		std::shared_ptr<RenderWindow> mActiveWindow;
		HINSTANCE mHandleInstance;
		// Scene Management
		std::vector<std::shared_ptr<Scene>> mScenes;
		std::shared_ptr<Scene> mActiveScene;
		// Rendering Management
		std::unique_ptr<RenderDevice> mRenderDevice;
		std::unique_ptr<RenderPipeline> mRenderPipeline;
		bool mIsRunning;
	};
}