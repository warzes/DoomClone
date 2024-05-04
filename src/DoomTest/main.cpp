#include "stdafx.h"
#include "Renderer.h"
#include "Wad.h"
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//-----------------------------------------------------------------------------

#define WIDTH 1200
#define HEIGHT 675

void error_callback(int error, const char* description)
{
	//fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	glfwSetErrorCallback(error_callback);
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(0);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);



	wad_t wad;
	if (wad_load_from_file("doom1.wad", &wad) != 0) {
		printf("Failed to load WAD file (doom1.wad)\n");
		return 2;
	}

	printf("Loaded a WAD file of type %s with %u lumps and directory at %u\n",
		wad.id, wad.num_lumps, wad.directory_offset);

	printf("Lumps:\n");
	for (int i = 0; i < wad.num_lumps; i++) {
		printf("%8s:\t%u\t%u\n", wad.lumps[i].name, wad.lumps[i].offset,
			wad.lumps[i].size);
	}

	renderer_init(width, height);

	char title[128];
	float last = 0.0f;
	float angle = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		float now = glfwGetTime();
		float delta = now - last;
		last = now;

		angle += .7f * delta;

		glfwPollEvents();

		snprintf(title, 128, "Game | %.0f", 1.0f / delta);
		glfwSetWindowTitle(window, title);


		renderer_clear();

		renderer_draw_line({ 0.f, 0.f }, { WIDTH, HEIGHT }, 5.f, { 0.f, 0.f, 1.f, 1.f });
		renderer_draw_line({ WIDTH, 0.f }, { 0.f, HEIGHT }, 5.f, { 0.f, 0.f, 1.f, 1.f });
		renderer_draw_point({ 100.f, 100.f }, 2.f, { 1.f, 1.f, 1.f, 1.f });
		renderer_draw_quad({ 900.f, 100.f }, { 50.f, 50.f }, angle, { 1.f, 1.f, 0.f, 1.f });

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
//-----------------------------------------------------------------------------