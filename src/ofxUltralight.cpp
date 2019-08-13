#include "ofxUltralight.hpp"

ofxUltralight::ofxUltralight() {
	/*
	Renderer::Create() �� ��ü opengl context�� ����� ������, glut�̳� glfw�� renderer->Render()���� �浹
	���� Renderer::Create()��, renderer->Render()�� main�� �ƴ� �����忡�� �����ϰ�, �ȼ� �����͸� ����� ����
	*/
	web_thread = std::thread([&]() {
		auto& platform = ultralight::Platform::instance();

		Config config;
		{
			/*///���� ���
			config.font_family_standard = "Malgun Gothic";
			config.font_family_fixed = "Malgun Gothic";
			config.font_family_serif = "Malgun Gothic";
			config.font_family_sans_serif = "Malgun Gothic";
			*/

			//�������
			config.font_family_standard = "NanumGothic";
			config.font_family_fixed = "NanumGothicCoding";
			config.font_family_serif = "NanumMyeongjo";
			config.font_family_sans_serif = "NanumBarunGothic";

			//PC ũ�� - ���� â���� �˻��� ������ ���� ����
			//config.user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36";

			//Samsung Browser 7.2 on Android 8.1 Samsung Galaxy S9 Plus 
			config.user_agent = "Mozilla/5.0 (Linux; Android 8.1.0; SM-G965F Build/OPM2.171019.029) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/7.2 Chrome/59.0.3071.125 Mobile Safari/537.36";
		}

		platform.set_config(config);

		GPUDriver* gpu_driver = gpu_driver = DefaultGPUDriver();
		platform.set_gpu_driver(gpu_driver);

		RefPtr<Renderer> renderer = Renderer::Create(); //this overides default config with 'my' config

		while (stop_flag == false) {
			if (loop_once) {	//main ������ update ȣ�� ��
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
					if (ast.view->is_bitmap_dirty()) {	//���� ���� �ּ�ȭ
						ast.bitmap_dirty = true;

						auto bmap = ast.view->bitmap();
						cv::Mat bmat = cv::Mat(bmap->height(), bmap->width(), CV_8UC4, bmap->raw_pixels());
						cv::cvtColor(bmat, ast.mat, cv::COLOR_RGBA2BGR);
					}
				}

				loop_once = false; //vector race condition ������ ���� ���� ��������� ����ȭ
			}
		}
	
		thread_done = true;
	});
	web_thread.detach();
}
