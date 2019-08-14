#pragma once

#include <ofMain.h>
#include <opencv.hpp>
#include <Ultralight.h>
#include "AppCore/gl/GPUDriverGL.h"

GLuint GeneratePBOReader(int width, int height, int numChannels = 4);

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT std::vector<BYTE>& pixel_data);

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT cv::Mat& pixel_data);

void CopyTextureFromFBO(GLuint fbo_id, OUT ofTexture& tex);

using namespace ultralight;

namespace ultralight {
	struct ViewRequest {
		int width;
		int height;
		string url;
	};

	struct ViewAsset {
		RefPtr<View> view;
		cv::Mat mat_rgba;	
		cv::Mat mat_bgr;	//cpu 픽셀
		ofTexture tex;		//gpu 텍스처
		
		GLuint pbo_id;		//gpu->cpu 읽기
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

	void update();

	ofxUltralight();
};