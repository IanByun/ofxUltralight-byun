#include "ofMain.h"
#include "ofApp.h"

int main() {
	ofGLFWWindowSettings main_settings;
	main_settings.width = 1920;
	main_settings.height = 1080;
	main_settings.windowMode = OF_WINDOW;
	main_settings.setGLVersion(4, 5);
	auto mainWindow = ofCreateWindow(main_settings);

	shared_ptr<ofApp> mainApp = make_shared<ofApp>();

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

	/*ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());*/

	/*ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());*/
}

#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <Ultralight/Ultralight.h>

#include <iostream>
#include <sstream>
#include <opencv2\opencv.hpp>
#include <chrono>
#include <thread>

using namespace std;
using namespace ultralight;

#include <glut.h>
#include <GL/gl.h>
#include <stdio.h>

class offScreenViewer {
protected:
	RefPtr<Renderer> renderer;
	GPUDriver* gpu_driver;
	std::thread web_thread;
	bool loop_once = false;

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
			//wait for web_thread to loop once -> vector race condition 방지를 위한 메인 스레드와의 동기화.
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	offScreenViewer() {
		/*
		Renderer::Create() 는 자체 opengl context를 만들기 때문에, glut이나 glfw와 renderer->Render()에서 충돌
		따라서 Renderer::Create()도, renderer->Render()도 main이 아닌 쓰레드에서 실행하고, 픽셀 데이터만 멤버로 공유
		*/
		web_thread = std::thread([&]() {
			auto& platform = ultralight::Platform::instance();

			Config config;
			{
				/*///맑은 고딕
				config.font_family_standard = "Malgun Gothic";
				config.font_family_fixed = "Malgun Gothic";
				config.font_family_serif = "Malgun Gothic";
				config.font_family_sans_serif = "Malgun Gothic";
				*/

				//나눔고딕
				config.font_family_standard = "NanumGothic";
				config.font_family_fixed = "NanumGothicCoding";
				config.font_family_serif = "NanumMyeongjo";
				config.font_family_sans_serif = "NanumBarunGothic";

				//PC 크롬 - 구글 창에서 검색어 렌더링 오류 있음
				//config.user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36";

				//Samsung Browser 7.2 on Android 8.1 Samsung Galaxy S9 Plus 
				config.user_agent = "Mozilla/5.0 (Linux; Android 8.1.0; SM-G965F Build/OPM2.171019.029) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/7.2 Chrome/59.0.3071.125 Mobile Safari/537.36";
			}

			platform.set_config(config);

			gpu_driver = DefaultGPUDriver();
			platform.set_gpu_driver(gpu_driver);

			renderer = Renderer::Create(); //this overides default config with 'my' config

			requests.push_back(
			{
				1280, 720, "https://m.search.naver.com/search.naver?query=%EB%84%A4%EC%9D%B4%EB%B2%84+%EC%8B%9C%EA%B3%84"
			}
			);

			while (true) {
				if (loop_once) {	//main 쓰레드 update 호출 시
					for (const auto& msg : requests) {
						ViewAsset asset;
						asset.view = renderer->CreateView(msg.width, msg.height, false);
						asset.view->LoadURL(msg.url.c_str());
						asset.mat = cv::Mat::zeros(msg.height, msg.width, CV_8UC3);

						assets.push_back(asset);
					}

					requests.clear();

					renderer->Update();
					renderer->Render();

					for (auto& ast : assets) {
						if (ast.view->is_bitmap_dirty()) {
							cout << "ast.view->is_bitmap_dirty()" << endl;
							auto bmap = ast.view->bitmap();
							cv::Mat bmat = cv::Mat(bmap->height(), bmap->width(), CV_8UC4, bmap->raw_pixels());
							cv::cvtColor(bmat, ast.mat, cv::COLOR_RGBA2BGR);
						}
					}

					loop_once = false; //vector race condition 방지를 위한 메인 스레드와의 동기화
				}
			}
		});
		web_thread.detach();
	}

	~offScreenViewer() {
		if (web_thread.joinable()) {
			web_thread.join(); //wait for end
		}
	}
};

//offScreenViewer ultra;
//
//void threadedUltraLight() {
//	ultra.update();
//
//	int i = 0;
//	for (const auto& ast : ultra.getViewAssets()) {
//		cv::imshow(to_string(++i), ast.mat);
//		cv::waitKey(1);
//	}
//
//	glutSwapBuffers();
//}
//
//void timer(int value)
//{
//	glutTimerFunc(16 /*1000ms / 60fps*/, timer, 0);
//	glutPostRedisplay();
//}

//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//	glutInitWindowSize(1280, 720);
//	glutCreateWindow("windowname");
//
//	glutTimerFunc(0, timer, 0);
//	glutDisplayFunc(threadedUltraLight);
//
//	ultra.requestCreateView(
//		1280, 720, "https://www.apple.com/kr/iphone/");
//	ultra.requestCreateView(
//		1280, 720, "https://m.weather.naver.com/m/main.nhn?regionCode=09140104&lang=en");
//	ultra.requestCreateView(
//		1280, 720, "https://google.co.kr");
//	ultra.requestCreateView(
//		1280, 720, "https://www.google.com/search?q=calculator");
//
//	glutMainLoop();
//
//	system("pause"); //I think this works on windows
//
//	return 0;
//}