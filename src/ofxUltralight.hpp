#pragma once

#include <ofMain.h>
#include <opencv.hpp>
#include <Ultralight.h>
#include "AppCore/gl/GPUDriverGL.h"

#define FATAL(x) { std::stringstream str; \
  str << "[ERROR] " << __FUNCSIG__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; \
  OutputDebugStringA(str.str().c_str()); \
  std::cerr << str.str() << std::endl; \
  /*__debugbreak();*/ }

static inline char const* glErrorString(GLenum const err) noexcept;

#define CHECK_GL()  {if (GLenum err = glGetError()) FATAL(glErrorString(err)) }

GLuint GeneratePBOReader(int width, int height, int numChannels = 4);

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT std::vector<BYTE>& pixel_data);

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id[2], OUT cv::Mat& pixel_data);

void ReadTextureToPBO(IN const ofTexture& tex, GLuint pbo_id, OUT cv::Mat & pixel_data);

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
		GLuint pbo_id[2];	//gpu->cpu 읽기
		bool needs_redraw = false; 
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

	void update(); //1초간 안부르면 ultralight 타임아웃. 멀티 쓰레딩해야 하나?

	ofxUltralight();
};