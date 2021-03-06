#pragma once

#include <string>
#include "Base\Base.h"
#include "..\Device Resources\DeviceResources.h"

namespace Renderer
{
	// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
	class TextRenderer : public Base::ISubsystem
	{
	public:
		TextRenderer(const std::shared_ptr<DeviceResources>& deviceResources);

		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void OnSuspending();
		virtual void OnResuming();

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Render(Platform::String^ Text, POINT& pos);

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DeviceResources> m_deviceResources;

		// Resources related to text rendering.
		DWRITE_TEXT_METRICS	                            m_textMetrics;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		Microsoft::WRL::ComPtr<IDWriteTextLayout>       m_textLayout;
		Microsoft::WRL::ComPtr<IDWriteTextFormat>		m_textFormat;
	};
}