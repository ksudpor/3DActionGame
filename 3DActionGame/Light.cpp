#include "Light.h"
#include <imgui/imgui.h>

Light::Light(IWorld* world){
	world_ = world;
	name_ = "Light";
	tag_ = "LightTag";
}

void Light::draw() const{
	// ���C�g�̃p�����[�^
	static float ambient[]{ 0.1f, 0.1f, 0.6f, 1.0f };
	static float diffuse[]{ 0.8f, 0.2f, 0.8f, 1.0f };
	static float specular[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	static float position[]{ 10.0f, 10.0f, 10.0f, 0.0f };
	static float constant_attenuation{ 1.0f };
	static float linear_attenuation{ 0.0f };
	static float quadtatic_attenuation{ 0.07f };

	// �������C�g�̃p�����[�^
	static GScolor rim_color{ 0.65f, 0.15f, 0.15f, 1.0f };
	static float rim_exponent{ 2.5f };

	// �t�H�O�̃p�����[�^
	static float fog_color[4]{ 0.5f, 0.3f, 0.7f, 1.0f };
	static float fog_start{ 5.0f };
	static float fog_end{ 150.0f };

	// �|�X�g�G�t�F�N�g�̃p�����[�^
	static GScolor color{ 1.0f, 1.0f, 1.0f, 1.0f };
	static float saturation{ 1.0f };
	static float luminance{ 1.0f };
	static float exposure{ 1.0 };

#ifdef _DEBUG
	// ���C�g�̃p�����[�^����
	ImGui::Begin("Lighting");
	ImGui::ColorEdit3("ambient", ambient, ImGuiColorEditFlags_Float);
	ImGui::ColorEdit3("diffuse", diffuse, ImGuiColorEditFlags_Float);
	ImGui::DragFloat("constant", &constant_attenuation, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("linear", &linear_attenuation, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("quadratic", &quadtatic_attenuation, 0.001f, 0.0f, 1.0f);
	ImGui::ColorEdit3("rim color", rim_color, ImGuiColorEditFlags_Float);
	ImGui::DragFloat("rim exponent", &rim_exponent, 0.01f);
	ImGui::End();

	// �t�H�O�̃p�����[�^����
	ImGui::Begin("Fog");
	ImGui::ColorEdit3("color", fog_color, ImGuiColorEditFlags_Float);
	ImGui::DragFloat("start", &fog_start);
	ImGui::DragFloat("end", &fog_end);
	ImGui::End();

	// �|�X�g�G�t�F�N�g�̃p�����[�^����
	ImGui::Begin("PostEffect");
	// �F����
	ImGui::ColorEdit3("color", color);
	// �ʓx����
	ImGui::DragFloat("saturation", &saturation, 0.01f, 0.0f, 2.0f);
	// �P�x����
	ImGui::DragFloat("luminance", &luminance, 0.01f, 0.0f, 2.0f);
	// �I�o����
	ImGui::DragFloat("exposure", &exposure, 0.01f);
	ImGui::End();
#endif
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	// ���C�g�̌����p�����[�^�̐ݒ�
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constant_attenuation);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear_attenuation);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadtatic_attenuation);
	// �������C�g�̐ݒ�
	gsSetRimLightColor(&rim_color);
	gsSetRimLightExponent(rim_exponent);
	// �⏕���C�g���o�C���h
	//gsBindAuxLight(0);
	// �t�H�O�̐ݒ�
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_START, fog_start);
	glFogi(GL_FOG_END, fog_end);
	glFogfv(GL_FOG_COLOR, fog_color);
	// �V���h�E�}�b�v�p�̃��C�g�̈ʒu��ݒ�
	GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
	//GSvector3 shadow_map_light_position{ 0.0f, 100.0f, 0.0f };
	gsSetShadowMapLightPosition(&shadow_map_light_position);
}
