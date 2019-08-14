#pragma once

#include <ofMain.h>
#include <opencv.hpp>
#include <Ultralight.h>
#include "AppCore/gl/GPUDriverGL.h"

using namespace ultralight;

namespace ultralight {
	struct ViewRequest {
		int width;
		int height;
		string url;
	};

	struct ViewAsset {
		RefPtr<View> view;
		cv::Mat mat;		//cpu 픽셀
		ofTexture tex;		//gpu 텍스
		bool bitmap_dirty = false;	//다른 쓰레드에서 cpu 픽셀을 가져오면, 메인 쓰레드에서 gpu 텍스로 옮김
	};
}

class ofxUltralight {
public:
	shared_ptr<GPUDriver> gpu_driver;
	RefPtr<Renderer> renderer;

protected:
	vector<ViewRequest> requests;
	vector<ViewAsset> assets;

public:
	typedef shared_ptr<ofxUltralight> Ptr;

	const vector<ViewRequest>& getViewRequests() {
		return requests;
	}

	void requestCreateView(int width, int height, string url) {
		requests.push_back({
			width, height, url
		});
	}

	const vector<ViewAsset>& getViewAssets() {
		return assets;
	}

	void update() {
		for (const auto& msg : requests) {
			ViewAsset asset;
			asset.view = renderer->CreateView(msg.width, msg.height, false);
			asset.view->LoadURL(msg.url.c_str());
			asset.mat = cv::Mat::zeros(msg.height, msg.width, CV_8UC3);

			assets.push_back(asset);
		}

		requests.clear();

		renderer->Update(); //여기서 아마 web 엔진이 view를 업데이트 하고
		renderer->Render();	//여기서 아마 업데이트 된 view를 그리기 위한 command 를 발주
		gpu_driver->DrawCommandList(); //여기서 아마 fbo에 실제로 그리기
	}

	ofxUltralight();
};