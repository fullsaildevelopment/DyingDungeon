#include "EngineProfiler.h"
#include "EventManager.h"
#include <iostream>
#include "InputManager.h"
#include "UICanvas.h"
#include "Rectangle2D.h"
#include "Text2D.h"

namespace Odyssey
{
	EngineProfiler::EngineProfiler()
	{
		// Subscribe to the thread tick event
		EventManager::getInstance().subscribe(this, &EngineProfiler::onThreadTick);

		// Subscribe to the opaque render event
		EventManager::getInstance().subscribe(this, &EngineProfiler::onOpaqueRender);

		EventManager::getInstance().subscribe(this, &EngineProfiler::onDebugInput);

		EventManager::getInstance().subscribe(this, &EngineProfiler::onWindowResize);

		mOutputStats = false;
	}

	void EngineProfiler::onThreadTick(ThreadTickEvent* evnt)
	{
		if (statsMap.count(evnt->threadName) == 0)
		{
			statsMap[evnt->threadName] = ThreadStats();
			statsMap[evnt->threadName].threadName = evnt->threadName;
		}

		ThreadStats& stats = statsMap[evnt->threadName];
		stats.timer.Signal();
		stats.framesPerSecond = static_cast<UINT>(1.0 / stats.timer.Delta());

		if (stats.timer.TotalTime() > 0.017)
		{
			stats.timer.Restart();

			if (mOutputStats)
			{
				updateDebugCanvas();
			}
		}
	}

	void EngineProfiler::onOpaqueRender(RenderEvent* evnt)
	{
		opaqueStats.timer.Signal();

		opaqueStats.objectsInScene = evnt->totalObjects;
		opaqueStats.numRendered = evnt->rendered;
		opaqueStats.numCulled = evnt->culled;
		opaqueStats.renderTime = opaqueStats.timer.Delta() * 1000.0;

		double time = opaqueStats.timer.TotalTime();

		if (time > 0.017)
		{
			opaqueStats.timer.Restart();

			if (mOutputStats)
			{
				updateDebugCanvas();
			}
		}
	}

	void EngineProfiler::onDebugInput(KeypressEvent* evnt)
	{
		if (evnt->keyCode == KeyCode::F7)
		{
			if (mDebugCanvas == nullptr)
			{
				createDebugCanvas();
			}
			else
			{
				mDebugCanvas->setActive(!mDebugCanvas->isActive());
			}
			mOutputStats = mDebugCanvas->isActive();
		}
	}

	void EngineProfiler::onWindowResize(WindowResizeEvent* evnt)
	{
		windowWidth = evnt->width;
		windowHeight = evnt->height;
	}

	void EngineProfiler::createDebugCanvas()
	{
		float x = (float)(windowWidth - windowWidth / 3);
		float y = (float)(windowHeight - windowHeight / 3);
		UINT width = windowWidth / 3;
		UINT height = windowHeight / 3;
		float padding = height / 10.0f;
		float indent = padding / 2.0f;

		std::wstring sceneThread = L"Scene Thread: " + std::to_wstring(statsMap["Scene Thread"].framesPerSecond) + L" fps";
		std::wstring renderThread = L"Main Thread: " + std::to_wstring(statsMap["Main Thread"].framesPerSecond) + L" fps";
		std::wstring frameTime = L"Frame Time: " + std::to_wstring(opaqueStats.renderTime) + L" ms";
		std::wstring totalObjects = L"Objects in scene: " + std::to_wstring(opaqueStats.objectsInScene);
		std::wstring renderObjects = L"Objects rendered: " + std::to_wstring(opaqueStats.numRendered);
		std::wstring culledObjects = L"Objects culled: " + std::to_wstring(opaqueStats.numCulled);

		TextProperties defaultText;
		defaultText.bold = false;
		defaultText.italic = false;
		defaultText.fontSize = (height) / 15.0f;
		defaultText.textAlignment = Odyssey::TextAlignment::Left;
		defaultText.paragraphAlignment = Odyssey::ParagraphAlignment::Left;
		defaultText.fontName = L"Verdana";

		// Create the canvas
		mDebugCanvas = std::make_shared<UICanvas>();

		// Create the background
		mDebugCanvas->addElement<Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f), windowWidth / 3, windowHeight / 3);

		// Scene thread fps
		mDebugCanvas->addElement<Text2D>(DirectX::XMFLOAT2(x + indent, y + indent), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 0.5f),
			(windowWidth / 3) - 10, (windowHeight / 3) - 10, sceneThread, defaultText);

		// Render thread fps
		mDebugCanvas->addElement<Text2D>(DirectX::XMFLOAT2(x + indent, y + padding + indent), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 0.5f),
			(windowWidth / 3) - 10, (windowHeight / 3) - 10, renderThread, defaultText);

		// Render frame time
		mDebugCanvas->addElement<Text2D>(DirectX::XMFLOAT2(x + indent, y + (2.0f * padding) + indent), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 0.5f),
			(windowWidth / 3) - 10, (windowHeight / 3) - 10, frameTime, defaultText);

		// Objects in scene
		mDebugCanvas->addElement<Text2D>(DirectX::XMFLOAT2(x + indent, y + (3.0f * padding) + indent), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 0.5f),
			(windowWidth / 3) - 10, (windowHeight / 3) - 10, totalObjects, defaultText);

		// Rendered objects
		mDebugCanvas->addElement<Text2D>(DirectX::XMFLOAT2(x + indent, y + (4.0f * padding) + indent), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 0.5f),
			(windowWidth / 3) - 10, (windowHeight / 3) - 10, renderObjects, defaultText);

		// Rendered objects
		mDebugCanvas->addElement<Text2D>(DirectX::XMFLOAT2(x + indent, y + (5.0f * padding) + indent), DirectX::XMFLOAT4(255.0f, 0.0f, 0.0f, 0.5f),
			(windowWidth / 3) - 10, (windowHeight / 3) - 10, culledObjects, defaultText);

		// Initialize the canvas
		mDebugCanvas->initialize();

		// Notify the sprite2d pass to render this debug canvas
		EventManager::getInstance().publish(new DebugEngine(mDebugCanvas));
	}

	void EngineProfiler::updateDebugCanvas()
	{
		std::wstring sceneThread = L"Scene Thread: " + std::to_wstring(statsMap["Scene Thread"].framesPerSecond) + L" fps";
		std::wstring renderThread = L"Main Thread: " + std::to_wstring(statsMap["Main Thread"].framesPerSecond) + L" fps";
		std::wstring frameTime = L"Frame Time: " + std::to_wstring(opaqueStats.renderTime) + L" ms";
		std::wstring totalObjects = L"Objects in scene: " + std::to_wstring(opaqueStats.objectsInScene);
		std::wstring renderObjects = L"Objects rendered: " + std::to_wstring(opaqueStats.numRendered);
		std::wstring culledObjects = L"Objects culled: " + std::to_wstring(opaqueStats.numCulled);

		std::vector<Text2D*> vec = mDebugCanvas->getElements<Text2D>();
		vec[0]->setText(sceneThread);
		vec[1]->setText(renderThread);
		vec[2]->setText(frameTime);
		vec[3]->setText(totalObjects);
		vec[4]->setText(renderObjects);
		vec[5]->setText(culledObjects);
	}
}