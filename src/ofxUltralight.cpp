#include "ofxUltralight.hpp"

void ofxUltralight::update() {
	for (const auto& msg : requests) {
		ViewAsset asset;
		asset.view = renderer->CreateView(msg.width, msg.height, false);
		asset.view->LoadURL(msg.url.c_str());

		asset.mat_rgba = cv::Mat::zeros(msg.height, msg.width, CV_8UC4);
		asset.mat_bgr = cv::Mat::zeros(msg.height, msg.width, CV_8UC3);
		asset.pbo_id = GeneratePBOReader(msg.width, msg.height);
		asset.tex.allocate(msg.width, msg.height, GL_RGB8, ofGetUsingArbTex(), GL_RGBA, GL_UNSIGNED_BYTE);

		assets.push_back(asset);
	}

	requests.clear();

	renderer->Update(); //여기서 아마 web 엔진이 view를 업데이트 하고
	renderer->Render();	//여기서 아마 업데이트 된 view를 그리기 위한 command 를 발주
	gpu_driver->DrawCommandList(); //여기서 아마 fbo에 실제로 그리기

	auto driver = dynamic_pointer_cast<GPUDriverGL>(gpu_driver);
	auto frame_map = driver->GetFrameMap();
	auto texture_map = driver->GetTextureMap();

	for (auto& asset : assets) {
		auto render_target = asset.view->render_target();

		GLuint fbo_id = frame_map[render_target.render_buffer_id];
		GLuint tex_id = texture_map[render_target.texture_id];

		CopyTextureFromFBO(fbo_id, asset.tex);
		ReadTextureToPBO(tex_id, asset.pbo_id, asset.mat_rgba);
		cv::cvtColor(asset.mat_rgba, asset.mat_bgr, cv::COLOR_RGBA2BGR);
	}
}

ofxUltralight::ofxUltralight() {
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

	gpu_driver = make_shared<GPUDriverGL>(1);
	platform.set_gpu_driver(gpu_driver.get());
	renderer = Renderer::Create();
}

GLuint GeneratePBOReader(int width, int height, int numChannels) {
	int data_size = width * height * numChannels;
	GLuint  pbo;
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
	glBufferData(GL_PIXEL_PACK_BUFFER, data_size, NULL, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	return pbo;
}

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT std::vector<BYTE>& pixel_data) {
	int data_size = pixel_data.size(); //어차피 1byte 원소

	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		(GLvoid*)0 // buffer에서 시작하는 byte 단위 offset. cpu memory가 아니다!
	);
	glBufferData(GL_PIXEL_PACK_BUFFER, data_size, 0, GL_STREAM_READ); //GL_STREAM_READ 말고 GL_STATIC_READ 하면 깜빡깜빡!

	GLubyte* dma_ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	if (dma_ptr) {
		memcpy((void*)pixel_data.data(), dma_ptr, data_size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT cv::Mat & pixel_data) {
	int data_size = pixel_data.total() * pixel_data.elemSize();

	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		(GLvoid*)0 // buffer에서 시작하는 byte 단위 offset. cpu memory가 아니다!
	);
	glBufferData(GL_PIXEL_PACK_BUFFER, data_size, 0, GL_STREAM_READ); //GL_STREAM_READ 말고 GL_STATIC_READ 하면 깜빡깜빡!

	GLubyte* dma_ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	if (dma_ptr) {
		memcpy((void*)pixel_data.data, dma_ptr, data_size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void CopyTextureFromFBO(GLuint fbo_id, OUT ofTexture & tex) {
	int tex_id = tex.texData.textureID;
	int tex_target = tex.texData.textureTarget;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glBindTexture(tex_target, tex_id);
	glCopyTexSubImage2D(tex_target, 0, 0, 0, 0, 0, tex.getWidth(), tex.getHeight()); //sub가 더 빠르다?
	glBindTexture(tex_target, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
