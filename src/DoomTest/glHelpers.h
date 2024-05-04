#pragma once


GLuint compile_shader(GLenum type, const char* src);
GLuint link_program(size_t num_shaders, ...);