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
		cv::Mat mat;		//cpu �ȼ�
		ofTexture tex;		//gpu �ؽ�
		bool bitmap_dirty = false;	//�ٸ� �����忡�� cpu �ȼ��� ��������, ���� �����忡�� gpu �ؽ��� �ű�
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

		renderer->Update(); //���⼭ �Ƹ� web ������ view�� ������Ʈ �ϰ�
		renderer->Render();	//���⼭ �Ƹ� ������Ʈ �� view�� �׸��� ���� command �� ����
		gpu_driver->DrawCommandList(); //���⼭ �Ƹ� fbo�� ������ �׸���
	}

	ofxUltralight();
};