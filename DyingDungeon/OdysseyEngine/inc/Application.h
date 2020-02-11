#pragma once
#include "EngineIncludes.h"
#include "RenderPipeline.h"
#include "EngineEvents.h"
#include "..\src\EngineProfiler.h"
#include "XTime.h"

namespace Odyssey
{
	// Forward declarations
	class RenderPass;
	class RenderWindow;
	class RenderWindowDX11;
	class Scene;
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

		void onShutdownApplication(ShutdownApplicationEvent* evnt);

		void onUIScale(UIScaleEvent* evnt);

		void onSpawnEntity(SpawnEntityEvent* evnt);

		void onDestroyEntity(DestroyEntityEvent* evnt);

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
		 *	Create a new window for rendering objects.
		 *	@param[in] title The title of the window.
		 *	@param[in] windowWidth The desired width of the window.
		 *	@param[in] windowHeight The desired height of the window.
		 *	@return shared_ptr<RenderWindow> The pointer to the created RenderWindow object.
		 */
		RenderWindow* createRenderWindow(const std::wstring& title, int windowWidth, int windowHeight);

		Entity* createPrefab();

		Scene* createScene(std::string name);

		Scene* createScene(std::string name, DirectX::XMFLOAT3 center, float radius);

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

		void setFrameLimit(std::string threadName, unsigned int fps);

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

	private:
		void setupRenderPipeline();
	private: // Members
		// Threading Management
		bool mIsMultithreading;
		EngineProfiler profiler;
		bool mProcessCommands;
		XTime mTimer;
		double mTickInterval;
		// Window Management
		std::vector<std::shared_ptr<RenderWindow>> mWindows;
		std::shared_ptr<RenderWindowDX11> mActiveWindow;
		HINSTANCE mHandleInstance;
		int mouseX, mouseY;
		// Scene Management
		std::map<std::string, std::shared_ptr<SceneDX11>> mSceneMap;
		std::shared_ptr<SceneDX11> mActiveScene;
		// Rendering Management
		std::shared_ptr<RenderPipeline> mRenderPipeline;
		bool mIsRunning;
		bool mIsShutdown;
		std::vector<std::shared_ptr<Entity>> mPrefabs;
	};
}