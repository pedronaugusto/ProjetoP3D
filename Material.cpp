#define _CRT_SECURE_NO_WARNINGS
#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"


Material::Material(const char *path)
{
	LoadMtl(path);
}


Material::~Material()
{
}

void Material::Init(GLuint program) {
	
	LoadTexture(path_to_tga);
	LoadLightningInfo(program);
}

void Material::LoadMtl(const char *path){

	ifstream file(path, ios::in);
	string line;
	
	if (file.is_open()) {
		while (getline(file, line)) {
			
			if (line.substr(0, 3) == "Ns ") {
				istringstream temp_ns(line.substr(3));
				temp_ns >> ns;
			}
			else if (line.substr(0, 3) == "Ka ") {
				istringstream temp_ka(line.substr(3));
				temp_ka >> ka.x;
				temp_ka >> ka.y;
				temp_ka >> ka.z;
			}
			else if (line.substr(0, 3) == "Kd ") {
				istringstream temp_kd(line.substr(3));
				temp_kd >> kd.x;
				temp_kd >> kd.y;
				temp_kd >> kd.z;
			}
			else if (line.substr(0, 3) == "Ks ") {
				istringstream temp_ks(line.substr(3));
				temp_ks >> ks.x;
				temp_ks >> ks.y;
				temp_ks >> ks.z;
			}
			else if (line.substr(0, 3) == "Ni ") {
				istringstream temp_ni(line.substr(3));
				temp_ni >> ni;
			}
			else if (line.substr(0, 2) == "d ") {
				istringstream temp_d(line.substr(2));
				temp_d >> d;
			}
			else if (line.substr(0, 6) == "illum ") {
				istringstream temp_illum(line.substr(6));
				temp_illum >> illum;
			}
			else if (line.substr(0, 7) == "map_Kd ") {
				string temp_path_to_tga = line.substr(7);
				strcpy(path_to_tga, "model/");
				strcat(path_to_tga, temp_path_to_tga.c_str());
			}
		}
		file.close();
		cerr << "Lido com sucesso: '" << path << "'" << endl;
	}
	else {
		cerr << "Erro ao abrir o ficheiro de material(.mtl)" << endl;
	}
}

void Material::LoadTexture(const char *path) {

	ifstream file(path, ios::in);

	if (file.is_open()) {
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glUniform1i(texture, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(true);

		int width, height, n_channels;
		unsigned char *image_data = stbi_load(path, &width, &height, &n_channels, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

		stbi_image_free(image_data);
		
		file.close();
		cerr << "Lido com sucesso: '" << path << "'" << endl;
	}
	else {
		cerr << "Erro ao abrir o ficheiro de textura" << endl;
	}
}

void Material::LoadLightningInfo(GLuint program) {
	
	glProgramUniform3fv(program, glGetUniformLocation(program, "material.emissive"), 1, &(glm::vec3(0.0f, 0.0f, 0.0f)[0]));
	glProgramUniform3fv(program, glGetUniformLocation(program, "material.ambient"), 1, &ka[0]);
	glProgramUniform3fv(program, glGetUniformLocation(program, "material.diffuse"), 1, &kd[0]);
	glProgramUniform3fv(program, glGetUniformLocation(program, "material.specular"), 1, &ks[0]);
	glProgramUniform1f(program, glGetUniformLocation(program, "material.shininess"), ns);
	
}