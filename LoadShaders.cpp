#include "LoadShaders.h"


static const GLchar* ReadShader(const char* filename) {
	// Abre o file 'filename' em binário, e coloca-se na última posição do file.
	ifstream file(filename, ifstream::ate | ifstream::binary);
	// Se o file foi aberto.
	if (file.is_open()) {
		// Leitura da próxima posição de leitura.
		streampos file_syze_in_bytes = file.tellg();
		// Reposiciona a leitura do file no seu início.
		file.seekg(0, ios::beg);

		// Alocação de espaço de memória para dados do file.
		GLchar* source = new GLchar[int(file_syze_in_bytes) + 1];
		// Leitura do file para o array 'source'.
		file.read(source, file_syze_in_bytes);
		// Fecha a string.
		source[file_syze_in_bytes] = 0;

		// Fecha o file.
		file.close();

		// Retorna o endereço da string alocada.
		return const_cast<const GLchar*>(source);
	}
	else {
		cerr << "Erro ao abrir o file '" << filename << "'" << endl;
	}

	return nullptr;
}

GLuint LoadShaders(ShaderInfo* shaders) {
	if (shaders == nullptr) return 0;

	// Cria um objeto de programa
	GLuint program = glCreateProgram();

	for (GLint i = 0; shaders[i].type != GL_NONE; i++) {
		// Cria um objeto shader
		shaders[i].shader = glCreateShader(shaders[i].type);

		// Efetua a leitura do código do shader
		const GLchar* source = ReadShader(shaders[i].filename);
		// Se não conseguir ler o código
		if (source == NULL) {
			// Destrói os shaders que tinham criados
			for (int j = 0; shaders[j].type != GL_NONE; j++) {
				// Se tem um shader válido (i.e., != 0)
				if(shaders[j].shader != 0)
					glDeleteShader(shaders[j].shader);
				shaders[j].shader = 0;
			}

			return 0;
		}

		// Carrega o código do shader
		glShaderSource(shaders[i].shader, 1, &source, NULL);
		delete[] source;

		// Compila o shader
		glCompileShader(shaders[i].shader);

		// Verifica o estado da compilação
		GLint compiled;
		glGetShaderiv(shaders[i].shader, GL_COMPILE_STATUS, &compiled);
		// Em caso de erro na compilação
		if (!compiled) {
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(shaders[i].shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(shaders[i].shader, len, &len, log);
			cerr << "Shader compilation failed: " << log << endl;
			delete[] log;
#endif /* DEBUG */

			// Destrói os shaders que tinham criados
			for (int j = 0; shaders[j].type != GL_NONE; j++) {
				// Se tem um shader válido (i.e., != 0)
				if (shaders[j].shader != 0)
					glDeleteShader(shaders[j].shader);
				shaders[j].shader = 0;
			}

			return 0;
		}

		// Anexa o shader ao programa
		glAttachShader(program, shaders[i].shader);
	}

	// Linka o programa
	glLinkProgram(program);

	// Verifica o estado do processo de linkagem
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	// Em caso de erro na linkagem
	if (!linked) {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		cerr << "Shader linking failed: " << log << endl;
		delete[] log;
#endif /* DEBUG */

		// Destrói os shaders que tinham criados
		for (int j = 0; shaders[j].type != GL_NONE; j++) {
			// Se tem um shader válido (i.e., != 0)
			if (shaders[j].shader != 0)
				glDeleteShader(shaders[j].shader);
			shaders[j].shader = 0;
		}

		return 0;
	}

	return program;
}
