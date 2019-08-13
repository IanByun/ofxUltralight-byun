#pragma once

#include <ofMain.h>
#include <opencv.hpp>
#include <Ultralight.h>

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
	std::thread web_thread;
	bool stop_flag = false;
	bool thread_done = false;
	bool loop_once = false;

protected:
	vector<ViewRequest> requests;
	vector<ViewAsset> assets;

public:
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
		while (loop_once) {
			//wait for web_thread to loop once -> vector race condition ������ ���� ���� ��������� ����ȭ.
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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