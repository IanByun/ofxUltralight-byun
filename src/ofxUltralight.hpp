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
	GPUDriver* gpu_driver;
	RefPtr<Renderer> renderer;

	std::thread web_thread;
	bool stop_flag = false;
	bool thread_done = false;
	bool loop_once = false;

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
		loop_once = true;
		while (/*loop_once*/false) {
			//wait for web_thread to loop once -> vector race condition 방지를 위한 메인 스레드와의 동기화.
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (/*loop_once*/true) {	//main 쓰레드 update 호출 시
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

			for (auto& ast : assets) {
				if (ast.view->is_bitmap_dirty()) {	//낭비 연산 최소화
					ast.bitmap_dirty = true;

					auto bmap = ast.view->bitmap();
					cv::Mat bmat = cv::Mat(bmap->height(), bmap->width(), CV_8UC4, bmap->raw_pixels());
					cv::cvtColor(bmat, ast.mat, cv::COLOR_RGBA2BGR);
				}

				//cv::imshow("ast.mat", ast.mat);
				//cv::waitKey(1);
			}

			loop_once = false; //vector race condition 방지를 위한 메인 스레드와의 동기화
		}

		for (auto& ast : assets) {
			if (ast.bitmap_dirty) {
				ast.bitmap_dirty = false;

				if (ast.tex.bAllocated() == false) {
					ast.tex.allocate(
						ast.mat.cols, ast.mat.rows,
						GL_RGB8,
						ofGetUsingArbTex(), 
						GL_BGR,
						GL_UNSIGNED_BYTE);
				}

				ast.tex.loadData(ast.mat.data, ast.mat.cols, ast.mat.rows, GL_BGR);
			}
		}
	}

	ofxUltralight();

	~ofxUltralight() {
		while (thread_done == false) {
			stop_flag = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
};