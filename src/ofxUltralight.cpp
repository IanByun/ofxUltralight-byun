#include "ofxUltralight.hpp"

char const * glErrorString(GLenum const err) noexcept {
	switch (err) {
		// OpenGL 2.0+ Errors:
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		// OpenGL 3.0+ Errors
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default: return "UNKNOWN ERROR";
	}
}

ofxUltralight::ofxUltralight() {
	auto& platform = ultralight::Platform::instance();

	Config config; {
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

void ofxUltralight::update() {
	renderer->Update(); //���⼭ �Ƹ� web ������ view�� ������Ʈ �ϰ�
	
	for (auto& asset : assets) {
		//renderer->Update() ����, renderer->Render() ���� �ҷ��� ��
		asset.needs_redraw = asset.view->needs_paint(); 
	}
	
	renderer->Render();	//���⼭ �Ƹ� ������Ʈ �� view�� �׸��� ���� command �� ����
	
	gpu_driver->DrawCommandList(); //���⼭ �Ƹ� fbo�� ������ �׸���

	auto driver = dynamic_pointer_cast<GPUDriverGL>(gpu_driver);
	auto frame_map = driver->GetFrameMap();
	auto texture_map = driver->GetTextureMap();

	for (int i = 0; i < assets.size(); i++) {
		auto& asset = assets[i];

		if (asset.needs_redraw) { //������ �ּ�ȭ
			asset.needs_redraw = false;

			auto render_target = asset.view->render_target();

			GLuint fbo_id = frame_map[render_target.render_buffer_id];
			GLuint tex_id = texture_map[render_target.texture_id];

			CopyTextureFromFBO(fbo_id, asset.tex);
			ReadTextureToPBO(tex_id, asset.pbo_id, asset.mat_rgba);
			cv::cvtColor(asset.mat_rgba, asset.mat_bgr, cv::COLOR_RGBA2BGR);
		}
	}
}

GLuint GeneratePBOReader(int width, int height, int numChannels) {
	int data_size = width * height * numChannels;
	GLuint  pbo;
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
	glBufferData(GL_PIXEL_PACK_BUFFER, data_size, NULL, GL_STATIC_READ);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	return pbo;
}

void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id[2], OUT cv::Mat & pixel_data) {
	int data_size = pixel_data.total() * pixel_data.elemSize();
	glFinish();

	//gpu �ҷ����� ����
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id[0]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		(GLvoid*)0 // buffer���� �����ϴ� byte ���� offset. cpu memory�� �ƴϴ�!
	);
	glBindTexture(GL_TEXTURE_2D, 0);

	//cpu ���� ����
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id[1]);
	GLubyte* mappedBuffer = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

	if (mappedBuffer) {
		memcpy((void*)pixel_data.data, mappedBuffer, data_size);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	std::swap(pbo_id[0], pbo_id[1]); //���� ����
}

void CopyTextureFromFBO(GLuint fbo_id, OUT ofTexture & tex) {
	int tex_id = tex.texData.textureID;
	int tex_target = tex.texData.textureTarget;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glBindTexture(tex_target, tex_id);
	glCopyTexSubImage2D(tex_target, 0, 0, 0, 0, 0, tex.getWidth(), tex.getHeight()); //sub�� �� ������?
	glBindTexture(tex_target, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
