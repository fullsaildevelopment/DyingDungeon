#pragma once
#include "EngineIncludes.h"
#include "RenderDevice.h"
#include "RenderPipeline.h"
#include "EngineEvents.h"
#include "..\src\EngineProfiler.h"

namespace Odyssey
{
	// Forward declarations
	class RenderPass;
	class RenderWindow;
	class RenderWindowDX11;
	class SceneDX11;

	class Application
	{
	public: // Rule of 3
		Application();
		~Application();

	public: // Events
		/**
		 *	Event callback to change the active scene.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onSceneChange(SceneChangeEvent* evnt);

		/**
		 *	Event callback notifying the application to flush the EventManager's command queue.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onCommandReceive(CommandReceiveEvent* evnt);

		/**
		 *	Event callback notifying the application to shutdown.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onShutdown(EngineShutdownEvent* evnt);

		/**
		 *	Static callback for render window message handling.
		 *	@param[in] hwnd The render window's handle.
		 *	@param[in] message The windows message to handle.
		 *	@param[in] wParam Windows messaging parameter.
		 *	@param[in] lParam Windows messaging parameter.
		 *	@return void
		 */
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	public: // Interface
		/**
		 *	Get a raw pointer to the RenderDevice object associated with the application.
		 *	The RenderDevice is used for object and resource creation.
		 *	@param[in] void
		 *	@return Render* The raw pointer to the RenderDevice object.
		 */
		RenderDevice* getRenderDevice();

		/**
		 *	Create a new window for rendering objects.
		 *	@param[in] title The title of the window.
		 *	@param[in] windowWidth The desired width of the window.
		 *	@param[in] windowHeight The desired height of the window.
		 *	@return shared_ptr<RenderWindow> The pointer to the created RenderWindow object.
		 */
		std::shared_ptr<RenderWindow> createRenderWindow(const std::wstring& title, int windowWidth, int windowHeight);

		/**
		 *	Add a scene to the scene map.
		 *	@param[in] name The name identifier of the scene.
		 *	@param[in] scene The scene object.
		 *	@return void
		 */
		void addScene(std::string name, std::shared_ptr<Scene> scene);

		/**
		 *	Add a rendering pass to the rendering pipeline.
		 *	@param[in] renderPass The rendering pass to add to the rendering pipeline.
		 *	@return void
		 */
		void addRenderPass(std::shared_ptr<RenderPass> renderPass);

		/**
		 *	Set the application's multithreading state.
		 *	@param[in] active The application's multithreading state.
		 *	@return void
		 */
		void setMultithreading(bool active);

		/**
		 *	Run the application's main loop.
		 *	@param[in] void
		 *	@return int The windows result of exiting the application.
		 */
		int run();

		/**
		 *	Exit the application's main loop.
		 *	@param[in] void
		 *	@return void
		 */
		void stop();

	private: // Members
		// Threading Management
		bool mIsMultithreading;
		EngineProfiler profiler;
		bool mProcessCommands;
		// Window Management
		std::vector<std::shared_ptr<RenderWindow>> mWindows;
		std::shared_ptr<RenderWindowDX11> mActiveWindow;
		HINSTANCE mHandleInstance;
		// Scene Management
		std::map<std::string, std::shared_ptr<SceneDX11>> mSceneMap;
		std::shared_ptr<SceneDX11> mActiveScene;
		// Rendering Management
		std::unique_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<RenderPipeline> mRenderPipeline;
		bool mIsRunning;
		bool mIsShutdown;
	};
}