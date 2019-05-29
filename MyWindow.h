#ifndef MAZESOLVER_MYWINDOW_H
#define MAZESOLVER_MYWINDOW_H
#include <SDL.h>

class MyWindow {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int windowID;
    int width, height;
    bool mouseFocus;
    bool keyboardFocus;
    bool fullScreen;
    bool minimized;
    bool shown;
public:
    MyWindow();
    ~MyWindow();

    bool init(const char* title, int width, int height);
    void handleEvent(SDL_Event& ev);

    void focus();
    void render();
    void free();

    SDL_Renderer *getRenderer();
	SDL_Window* getSDLWindow();

    int getWindowID() const;
    int getWidth() const;
    int getHeight() const;
    bool hasMouseFocus() const;
    bool hasKeyboardFocus() const;
    bool isFullScreen() const;
    bool isMinimized() const;
    bool isShown() const;
};

#endif //MAZESOLVER_MYWINDOW_H