#include "MyWindow.h"

int MyWindow::getWindowID() const {
    return windowID;
}

int MyWindow::getWidth() const {
    return width;
}

int MyWindow::getHeight() const {
    return height;
}

bool MyWindow::hasMouseFocus() const {
    return mouseFocus;
}

bool MyWindow::hasKeyboardFocus() const {
    return keyboardFocus;
}

bool MyWindow::isFullScreen() const {
    return fullScreen;
}

bool MyWindow::isMinimized() const {
    return minimized;
}

bool MyWindow::isShown() const {
    return shown;
}

SDL_Renderer *MyWindow::getRenderer() {
    return renderer;
}

bool MyWindow::init(const char *title, int width, int height) {
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        window = nullptr;
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    windowID = SDL_GetWindowID(window);
    this->width = width;
    this->height = height;

    shown = true;
    keyboardFocus = true;
    mouseFocus = true;

    return true;
}

void MyWindow::handleEvent(SDL_Event &ev) {
    if (ev.type == SDL_WINDOWEVENT && ev.window.windowID == windowID) {
        switch (ev.window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                shown = true;
                break;

            case SDL_WINDOWEVENT_HIDDEN:
                shown = false;
                break;

            case SDL_WINDOWEVENT_SIZE_CHANGED:
                width = ev.window.data1;
                height = ev.window.data2;
                SDL_RenderPresent(renderer);
                break;

            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(renderer);
                break;

            case SDL_WINDOWEVENT_ENTER:
                mouseFocus = true;
                break;

            case SDL_WINDOWEVENT_LEAVE:
                mouseFocus = false;
                break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
                keyboardFocus = true;
                break;

            case SDL_WINDOWEVENT_FOCUS_LOST:
                keyboardFocus = false;
                break;

            case SDL_WINDOWEVENT_MINIMIZED:
                minimized = true;
                break;

            case SDL_WINDOWEVENT_MAXIMIZED:
                minimized = false;
                break;

            case SDL_WINDOWEVENT_RESTORED:
                minimized = false;
                break;

            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(window);
                break;

            default:
                break;
        }
    }
}

void MyWindow::focus() {
    if (!shown) {
        SDL_ShowWindow(window);
    }
    SDL_RaiseWindow(window);
}

void MyWindow::render() {
    if (!minimized) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
    }
}

MyWindow::MyWindow() : shown{false},
                       keyboardFocus{false}, mouseFocus{false},
                       fullScreen{false}, minimized{false},
                       windowID{0}, width{0}, height{0} {
}

MyWindow::~MyWindow() {
    free();
}

void MyWindow::free() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
}

SDL_Window *MyWindow::getSDLWindow() {
    return window;
}
