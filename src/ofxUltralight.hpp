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
		cv::Mat mat_bgr;			//cpu 픽셀
		shared_ptr<ofTexture> tex;	//gpu texture id 공유 때문에 shared
		GLuint pbo_id[2];			//gpu->cpu 읽기
		bool needs_redraw = false; 
	};
}

class ofxUltralight {
protected:
	shared_ptr<GPUDriver> gpu_driver;
	RefPtr<Renderer> renderer;
	vector<ViewAsset> assets;

public:
	typedef shared_ptr<ofxUltralight> Ptr;

	void createViewAsset(int width, int height, string url) {
		ViewAsset asset;
		asset.view = renderer->CreateView(width, height, false);
		asset.view->LoadURL(url.c_str());

		asset.mat_rgba = cv::Mat::zeros(height, width, CV_8UC4);
		asset.mat_bgr = cv::Mat::zeros(height, width, CV_8UC3);
		asset.pbo_id[0] = GeneratePBOReader(width, height);
		asset.pbo_id[1] = GeneratePBOReader(width, height);
		
		asset.tex = make_shared<ofTexture>(); //gpu texture id 공유 때문에 shared
		asset.tex->allocate(width, height, GL_RGB8, ofGetUsingArbTex(), GL_RGBA, GL_UNSIGNED_BYTE);

		assets.push_back(asset);
	}

	const vector<ViewAsset>& getViewAssets() {
		return assets;
	}

	void update(); //1초간 안부르면 ultralight 타임아웃. 멀티 쓰레딩해야 하나?

	ofxUltralight(ultralight::Config* _config = nullptr);
};