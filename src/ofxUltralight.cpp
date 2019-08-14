#include "ofxUltralight.hpp"

ofxUltralight::ofxUltralight() {
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

	gpu_driver = make_shared<GPUDriverGL>(1);
	platform.set_gpu_driver(gpu_driver.get());
	renderer = Renderer::Create();
}
