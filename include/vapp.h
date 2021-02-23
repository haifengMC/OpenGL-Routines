#ifndef __VAPP_H__
#define __VAPP_H__

#include "vgl.h"

class VermilionApplication
{
protected:
    inline VermilionApplication(void) {}
    virtual ~VermilionApplication(void) {}

    static VermilionApplication * s_app;
    GLFWwindow* m_pWindow;

#ifdef _WIN32
    ULONGLONG       m_appStartTime;
#else
    struct timeval  m_appStartTime;
#endif

    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void scroll_callback(GLFWwindow* window, double x, double y);
	static void char_callback(GLFWwindow* window, unsigned int codepoint);
    static void drop_callback(GLFWwindow* window, int num, const char** file);
    unsigned int app_time();

#ifdef _DEBUG
    static void APIENTRY DebugOutputCallback(GLenum source,
                                             GLenum type,
                                             GLuint id,
                                             GLenum severity,
                                             GLsizei length,
                                             const GLchar* message,
                                             GLvoid* userParam);
#endif

public:
    void MainLoop(void);

    virtual void Initialize(const char * title = 0);

    virtual void Display(bool auto_redraw = true)
    {
        glfwSwapBuffers(m_pWindow);
    }

    virtual void Finalize(void) {}

    virtual void Resize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void getWindowSize(int* width, int* height)
    {
        glfwGetWindowSize(m_pWindow, width, height);
    }

    hSize getWindowSize()
	{
        int width = 0, height = 0;
		glfwGetWindowSize(m_pWindow, &width, &height);
        return hSize(width, height);
	}

    virtual void OnKey(int key, int scancode, int action, int mods) { /* NOTHING */ }//键盘回调
	virtual void OnMouse(int button, int action, int mods) { /* NOTHING */ }//鼠标点击回调
	virtual void OnCursor(double x, double y) { /* NOTHING */ }//光标位置回调
	virtual void OnCursorNorm(float x, float y) { /* NOTHING */ }//光标归一化位置回调
    virtual void OnScroll(double x, double y) { /* NOTHING */ }//滚动回调
	virtual void OnChar(unsigned int codepoint) { /* NOTHING */ }
	virtual void OnDrop(int num, const char** file) { /* NOTHING */ }//拖进文件回调
};

struct VermilionUiAdaptor : hObject
{
    virtual ~VermilionUiAdaptor() {}
    virtual void loadUi() {}
    virtual void unloadUi() {}
};

struct VermilionAppAdaptor
{
	virtual ~VermilionAppAdaptor() {}
	virtual void initialize(GLuint uiBufSize) {}
	virtual void display(bool auto_redraw = true) {}
	virtual void finalize(void) {}
	virtual void resize(int width, int height) {}

	virtual void onKey(int key, int scancode, int action, int mods) { /* NOTHING */ }//键盘回调
	virtual void onMouse(int button, int action, int mods) { /* NOTHING */ }//鼠标点击回调
	virtual void onCursor(double x, double y) { /* NOTHING */ }//光标位置回调
	virtual void onCursorNorm(float x, float y) { /* NOTHING */ }//光标归一化位置回调
	virtual void onScroll(double x, double y) { /* NOTHING */ }//滚动回调
	virtual void onChar(unsigned int codepoint) { /* NOTHING */ }
	virtual void onDrop(int num, const char** file) { /* NOTHING */ }//拖进文件回调
};

#define BEGIN_UI_DECLARATION(appclass)                      \
struct appclass##Ui : public VermilionUiAdaptor

#define END_UI_DECLARATION()

#define UI_FUNCTION(appclass, appfunc)                     \
appclass##Ui::appfunc

#define BEGIN_APP_DECLARATION(appclass)                      \
struct appclass##App : public VermilionAppAdaptor

#define END_APP_DECLARATION()

#define APP_FUNCTION(appclass, appfunc)                     \
appclass##App::appfunc

#ifdef _DEBUG
//*
#define DEBUG_OUTPUT_CALLBACK                                                   \
void APIENTRY VermilionApplication::DebugOutputCallback(GLenum source,          \
                                                         GLenum type,           \
                                                         GLuint id,             \
                                                         GLenum severity,       \
                                                         GLsizei length,        \
                                                         const GLchar* message, \
                                                         GLvoid* userParam)     \
{                                                                               \
    OutputDebugStringA(message);                                                \
    OutputDebugStringA("\n");                                                   \
}
/*/
#define DEBUG_OUTPUT_CALLBACK                                                   \
void APIENTRY VermilionApplication::DebugOutputCallback(GLenum source,         \
                                                         GLenum type,           \
                                                         GLuint id,             \
                                                         GLenum severity,       \
                                                         GLsizei length,        \
                                                         const GLchar* message, \
                                                         GLvoid* userParam)     \
{                                                                               \
    printf("Debug Message: SOURCE(0x%04X), "\
                          "TYPE(0x%04X), "\
                          "ID(0x%08X), "\
                          "SEVERITY(0x%04X), \"%s\"\n",\
           source, type, id, severity, message);\
}
*/
#else
#define DEBUG_OUTPUT_CALLBACK
#endif

#ifdef _WIN32
#define MAIN_DECL int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#else
#define MAIN_DECL int main(int argc, char ** argv)
#endif

#define DECLARATION_APP(appclass)                          \
class appclass : public VermilionApplication                \
{                                                           \
    hTool::hAutoPtr<COMB(appclass,Ui)> _ui;                 \
	hTool::hAutoPtr<COMB(appclass,App)> _app;               \
public:                                                     \
	typedef class VermilionApplication base;                \
	static GLuint vaUi, vbUi, veUi;                        \
	static VermilionApplication* Create()                   \
	{                                                       \
		return s_app = new appclass;                        \
	}                                                       \
	void Initialize(const char* title = 0)                 \
	{                                                       \
		_ui.bind(new COMB(appclass,Ui));                    \
		_app.bind(new COMB(appclass,App));                  \
		base::Initialize(title);                           \
		_ui->loadUi();                                      \
		_app->initialize(_ui->getBufSize(),_ui->getEleBufSize());\
        GLuint bOffset = 0;                                 \
        GLuint eOffset = 0;                                 \
		_ui->initialize(getWindowSize(), vbUi, bOffset, veUi, eOffset);\
	}                                                       \
	void Display(bool auto_redraw = true)                   \
	{                                                       \
	    glClear(GL_COLOR_BUFFER_BIT);                       \
        glBindVertexArray(vaUi);                            \
		_app->display(auto_redraw);                         \
		_ui->display();                                     \
		base::Display(auto_redraw);                         \
	}                                                       \
	void Finalize(void)                                     \
	{                                                       \
		_app->finalize();                                   \
		_ui->unloadUi();                                    \
		_ui->hObject::finalize();                           \
		_app.destory();                                     \
		_ui.destory();                                      \
	}                                                       \
	void Resize(int width, int height)                      \
	{                                                       \
		base::Resize(width, height);                        \
		_app->resize(width, height);                        \
	}                                                       \
	void OnKey(int key, int scancode, int action, int mods) { _app->onKey(key, scancode, action, mods); }\
	void OnMouse(int button, int action, int mods) { _app->onMouse(button, action, mods); }\
	void OnCursor(double x, double y) { _app->onCursor(x, y); }\
	void OnCursorNorm(float x, float y) { _app->onCursorNorm(x, y); }\
	void OnScroll(double x, double y) { _app->onScroll(x, y); }\
	void OnChar(unsigned int codepoint) { _app->onChar(codepoint); }\
	void OnDrop(int num, const char** file) { _app->onDrop(num, file); }\
};

#define DEFINE_APP(appclass,title)                          \
VermilionApplication * VermilionApplication::s_app;         \
GLuint appclass::vaUi = 0, appclass::vbUi = 0, appclass::veUi = 0;\
                                                            \
void VermilionApplication::MainLoop(void)                   \
{                                                           \
    do                                                      \
    {                                                       \
        Display();                                          \
        glfwPollEvents();                                   \
    } while (!glfwWindowShouldClose(m_pWindow));            \
}                                                           \
                                                            \
MAIN_DECL                                                   \
{                                                           \
    VermilionApplication * app = appclass::Create();        \
                                                            \
    app->Initialize(title);                                 \
    app->MainLoop();                                        \
    app->Finalize();                                        \
    return 0;                                               \
}                                                           \
                                                            \
DEBUG_OUTPUT_CALLBACK

#endif /* __VAPP_H__ */
