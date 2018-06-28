#define _CRT_SECURE_NO_WARNINGS
#include "Model.h"


Model::Model(const char *path){

	LoadObj(path);

}

Model::~Model()
{

}

void Model::Init(GLuint program) {
		
	Material material(path_to_mtl);
	material.Init(program);
	
	
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	
}


void Model::Draw(GLuint program) {

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
		
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(
		2,                                
		3,                               
		GL_FLOAT,                         
		GL_FALSE,                         
		0,                                
		(void*)0                          
	);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void Model::LoadObj(const char *path) 
{
	ifstream file(path, ios::in);
	string line;
	glm::vec3 temp_vertex;
	glm::vec2 temp_uv;
	glm::vec3 temp_normal;

	string first_indices;
	string second_indices;
	string third_indices;

	if (file.is_open()) {
		while (getline(file, line)) {
			if (line.substr(0, 2) == "v ") {
				istringstream aux_vertex(line.substr(2));
				aux_vertex >> temp_vertex.x;
				aux_vertex >> temp_vertex.y;
				aux_vertex >> temp_vertex.z;
				temp_vertices.push_back(temp_vertex);
			}
			else if (line.substr(0, 3) == "vt ") {
				istringstream aux_uv(line.substr(3));
				aux_uv >> temp_uv.x;
				aux_uv >> temp_uv.y;
				temp_uvs.push_back(temp_uv);
			}
			else if (line.substr(0, 3) == "vn ") {
				istringstream aux_normal(line.substr(3));
				aux_normal >> temp_normal.x;
				aux_normal >> temp_normal.y;
				aux_normal >> temp_normal.z;
				temp_normals.push_back(temp_normal);
			}
			else if (line.substr(0, 2) == "f ") {
				istringstream temp_indices(line.substr(2));
				temp_indices >> first_indices;
				temp_indices >> second_indices;
				temp_indices >> third_indices;
				GetIndicesFromLine(first_indices, second_indices, third_indices);
			}
			else if (line.substr(0, 7) == "mtllib ") {
				string temp_path_to_mtl = line.substr(7);
				strcpy(path_to_mtl, "model/");
				strcat(path_to_mtl, temp_path_to_mtl.c_str());
			}
		}
		cerr << "Lido com sucesso: '" << path << "'" << endl;
		file.close();

		MakeIndexedVerticesUvsNormals();
	}
	else {
		cerr << "Erro ao abrir o ficheiro '" << path << "'" << endl;
	}
}

//Separate the indices and put them in their respective arrays
void Model::GetIndicesFromLine(string first_indices, string second_indices, string third_indices) {
	
	int vertex_index[3], uv_index[3], normal_index[3];
	string temp_index;

	istringstream first(first_indices);
	getline(first, temp_index, '/');
	vertex_index[0] = stoi(temp_index);
	getline(first, temp_index, '/');
	uv_index[0] = stoi(temp_index);
	getline(first, temp_index, '/');
	normal_index[0] = stoi(temp_index);

	istringstream second(second_indices);
	getline(second, temp_index, '/');
	vertex_index[1] = stoi(temp_index);
	getline(second, temp_index, '/');
	uv_index[1] = stoi(temp_index);
	getline(second, temp_index, '/');
	normal_index[1] = stoi(temp_index);

	istringstream third(third_indices);
	getline(third, temp_index, '/');
	vertex_index[2] = stoi(temp_index);
	getline(third, temp_index, '/');
	uv_index[2] = stoi(temp_index);
	getline(third, temp_index, '/');
	normal_index[2] = stoi(temp_index);

	vertex_indices.push_back(vertex_index[0]);
	vertex_indices.push_back(vertex_index[1]);
	vertex_indices.push_back(vertex_index[2]);
	uv_indices.push_back(uv_index[0]);
	uv_indices.push_back(uv_index[1]);
	uv_indices.push_back(uv_index[2]);
	normal_indices.push_back(normal_index[0]);
	normal_indices.push_back(normal_index[1]);
	normal_indices.push_back(normal_index[2]);
}

//Fill the Vertices, Uvs and Normals vectors according to the indices
void Model::MakeIndexedVerticesUvsNormals() {

	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
		unsigned int vertexIndex = vertex_indices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uv_indices.size(); i++) {
		unsigned int uvIndex = uv_indices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
	}

	for (unsigned int i = 0; i < normal_indices.size(); i++) {
		unsigned int normalIndex = normal_indices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}

}

