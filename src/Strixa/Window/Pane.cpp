#include <Strixa/Window/Pane.h>

#include <Strixa/Window/Frame.h>

using Strixa::Window::Frame;
using Strixa::Window::Pane;
using Strixa::Window::UiThread;
using Strixa::Util::ThreadLocal;

/* Global Constants */
const UINT WINDOW_CREATED = RegisterWindowMessage(L"window.created");
const UINT WINDOW_DESTROYED = RegisterWindowMessage(L"window.destroyed");

/* Global Variables */
ThreadLocal<UiThread> threadlocal_ui_thread;  // Has the effect of ensuring two windows created on the same thread will share the same message pump.

/* Class Definition: UiThread */
UiThread::UiThread()
{
    this->schedule(&UiThread::run,this);
}

UiThread::~UiThread()
{
    if (this->joinable()) {
        this->join();
    }
}

void UiThread::run()
{
    MSG message;
    unsigned int window_count;


    window_count = 0;

    if (PeekMessage(&message,NULL,0,0,PM_REMOVE) == TRUE) {
        if (message.message == WINDOW_CREATED) {
            window_count++;
        } else if (message.message == WINDOW_DESTROYED) {
            window_count--;

            if (window_count == 0) {
                this->join();
            }
        } else {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

    this->schedule(&UiThread::run,this);
}

Pane::Pane(const LPTSTR class_name,UINT class_style)
    : ui_thread(threadlocal_ui_thread.get())
{
    this->background = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    this->enable_redraw = true;
    this->exitcode = 0;
    this->exitcode_future = this->exitcode_promise.get_future();
    this->extended_style = 0;
    this->hwnd = NULL;
    this->initialized = false;
    this->parent = NULL;
    this->style = 0;

    if (GetClassInfoEx(GetModuleHandle(NULL),class_name,&this->window_class) == FALSE) {
        this->window_class.cbSize = sizeof(WNDCLASSEX);
        this->window_class.style = class_style;
        this->window_class.lpfnWndProc = Pane::WndProc;
        this->window_class.cbClsExtra = 0;
        this->window_class.cbWndExtra = 0;
        this->window_class.hInstance = GetModuleHandle(NULL);
        this->window_class.hIcon = NULL;
        this->window_class.hCursor = LoadCursor(NULL,IDC_ARROW);
        this->window_class.hbrBackground = NULL;
        this->window_class.lpszMenuName = NULL;
        this->window_class.lpszClassName = class_name;
        this->window_class.hIconSm = NULL;

        if (RegisterClassEx(&this->window_class) == FALSE) {
            // TODO:  Issue some kind of error.
        }
    }
}

Pane::~Pane()
{
    if (this->isAlive()) {
        this->close();
    }
}

void Pane::addExtendedStyle(DWORD style)
{
    this->setExtendedStyle(this->getExtendedStyle() | style);
}

void Pane::addStyle(DWORD style)
{
    this->setStyle(this->getStyle() | style);
}

bool Pane::close(int exit_code,bool force)
{
    if (this->isAlive()) {
        if (force || this->isClosable()) {
            this->exitcode = exit_code;
            DestroyWindow(this->hwnd);

            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

int Pane::getBackgroundColor() const
{
    return 0;//TODO
}

int Pane::getExitCode()
{
    if (!this->initialized) {
        throw std::exception("The window must first be initialized using the 'init' method before you can attempt to retrieve an error code.");
    }

    this->exitcode_future.wait();
    return exitcode_future.get();
}

DWORD Pane::getExtendedStyle() const
{
    return GetWindowLong(this->hwnd,GWL_EXSTYLE);
}

int Pane::getHeight() const
{
    return this->size.height;
}

HWND Pane::getNativeHandle() const
{
    return this->hwnd;
}

Frame* Pane::getParent() const
{
    return this->parent;
}

int Pane::getPosX() const
{
    return this->position.x;
}

int Pane::getPosY() const
{
    return this->position.y;
}

DWORD Pane::getStyle() const
{
    return GetWindowLong(this->hwnd,GWL_STYLE);
}

int Pane::getWidth() const
{
    return this->size.width;
}

UiThread& Pane::getUiThread() const
{
    return this->ui_thread;
}

bool Pane::init()
{
    if (!this->initialized) {
        /* The window must be created on the UI thread.  Make it so, number one. */
        bool task_complete;


        task_complete = false;
        this->ui_thread.now([&](){
            this->hwnd = CreateWindowEx(
                this->extended_style | WS_CLIPCHILDREN,
                this->window_class.lpszClassName,
                L"",
                this->style,
                this->position.x,this->position.y,
                this->size.width,this->size.height,
                this->parent == NULL ? NULL : this->parent->hwnd,
                NULL,
                this->window_class.hInstance,
                this
            );

            task_complete = true;
        });
        while (!task_complete);

        /* Ensure the window was created successfully. */
        if (this->hwnd == NULL) {
            return false;
        }

        /* Notify everyone of our window's creation. */
        PostThreadMessage(GetWindowThreadProcessId(this->hwnd,NULL),WINDOW_CREATED,0,0);

        this->onCreate();
        this->initialized = true;

        ShowWindow(this->hwnd,SW_SHOWNORMAL);
        UpdateWindow(this->hwnd);
    }

    return this->initialized;
}

void Pane::invalidate()
{
    InvalidateRect(this->hwnd,NULL,true);
}

bool Pane::isAlive()
{
    return this->hwnd != NULL;
}

bool Pane::isClosable()
{
    return true;
}

bool Pane::isRedrawEnabled() const
{
    return this->enable_redraw;
}

void Pane::onClose()
{
}

void Pane::onCreate()
{
}

void Pane::onMove()
{
}

void Pane::onMoveProposed(int& x,int& y)
{
}

void Pane::onPaint(HDC device_context)
{
    RECT background_area;


    background_area.left = 0;
    background_area.top = 0;
    background_area.right = this->size.width;
    background_area.bottom = this->size.height;
    
    FillRect(device_context,&background_area,this->background);
}

void Pane::onResize()
{
}

void Pane::onResizeProposed(int& width,int& height)
{
}

void Pane::removeExtendedStyle(DWORD style)
{
    this->setExtendedStyle(this->getExtendedStyle() & (~style));
}

void Pane::removeStyle(DWORD style)
{
    this->setStyle(this->getStyle() & (~style));
}

void Pane::setBackgroundColorHSL(short hue,float saturation,float lightness)
{
    float mid_height;
    float min_height;
    float max_height;
    float rgb[3];


    /* Normalize the HSL values.
     */
    if (hue < 0) {
        hue *= -1;
    }
    hue = hue % 360;

    saturation = min(max(saturation,0.0f),1.0f);
    lightness = min(max(lightness,0.0f),1.0f);

    /* Compute the RGB equivalent values.
     */
    mid_height = 60.0f * lightness;

    min_height = mid_height - (saturation * 30.0f);
    max_height = mid_height + (saturation * 30.0f);
    if (mid_height < 30.0f) {
        min_height += 30.0f - mid_height;
        max_height += 30.0f - mid_height;
    } else {
        min_height += mid_height - 30.0f;
        max_height += mid_height - 30.0f;
    }

    for(int component = 0;component < 3;++component){
    	/* Retrieve the initial, unscaled, unbounded, and untranslated RGB values.
         */
    	rgb[component] = abs(((hue + (360 - (120 * component))) % 360) - 180.0f) - 90.0f;
    			
    	/* Scale these RGB values to the appropriate sizes.
    	 */
    	rgb[component] *= 2 * saturation * (lightness < 0.5f ? lightness : 1 - lightness);

    	/* Perform the translation.
    	 */
    	rgb[component] += mid_height;

    	/* Bound the values.
    	 */
    	rgb[component] = min(max_height, max(min_height, rgb[component])) / 60.0f;
    }

    this->setBackgroundColorRGB((short)(rgb[0] * 255),(short)(rgb[1] * 255),(short)(rgb[2] * 255));
}

void Pane::setBackgroundColorRGB(short red,short green,short blue)
{
    red = min(max(red,0),255);
    green = min(max(green,0),255);
    blue = min(max(blue,0),255);

    DeleteObject(this->background);
    this->background = CreateSolidBrush(RGB(red,green,blue));

    if (this->hwnd != NULL) {
        RedrawWindow(this->hwnd,NULL,NULL,RDW_INVALIDATE | RDW_ERASE);
    }
}

void Pane::setExtendedStyle(DWORD style)
{
    if (this->isAlive()) {
        SetWindowLong(this->hwnd,GWL_EXSTYLE,style);

        /* Per the specs, if the style change affects the border, SetWindowPos must be called with the SWP_FRAMECHANGED flag in order for the change to be shown immediately.
        */
        if ((style & (WS_EX_CLIENTEDGE | WS_EX_CONTEXTHELP | WS_EX_DLGMODALFRAME | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE)) > 0) {
            SetWindowPos(this->hwnd,NULL,0,0,0,0,SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        }
    } else {
        this->extended_style = style;
    }
}

void Pane::setParent(Frame* parent)
{
    if (parent == NULL) {
        this->removeStyle(WS_CHILD);
        this->addStyle(WS_POPUP);
    } else {
        if (this->parent != NULL) {
            throw std::exception("Cannot add the requested Window Pane to the Frame.  Pane is already set in a Frame.  Call 'removeChild' on the Pane's parent first.");
        }

        this->removeStyle(WS_POPUP);
        this->addStyle(WS_CHILD);

        this->parent = parent;
        if (this->isAlive()) {
            SetParent(this->hwnd,this->parent->hwnd);
        }
    }
}

void Pane::setPosition(long x,long y)
{
    if (this->isAlive()) {
        SetWindowPos(
            this->hwnd,
            NULL,
            x,y,
            0,0,
            SWP_NOSIZE | SWP_NOZORDER
        );
    } else {
        this->position.x = x;
        this->position.y = y;
    }
}

void Pane::setRedrawEnabled(bool enabled)
{
    this->enable_redraw = enabled;
}

void Pane::setSize(long width,long height)
{
    if (this->isAlive()){
        SetWindowPos(
            this->hwnd,
            NULL,
            0,0,
            width,height,
            SWP_NOMOVE | SWP_NOZORDER
        );
    } else {
        this->size.width = width;
        this->size.height = height;
    }
}

void Pane::setStyle(DWORD style)
{
    if (this->isAlive()) {
        SetWindowLong(this->hwnd,GWL_STYLE,style);

        /* Per the specs, if the style change affects the border, SetWindowPos must be called with the SWP_FRAMECHANGED flag in order for the change to be shown immediately.
        */
        if ((style & (WS_BORDER | WS_CAPTION | WS_DLGFRAME | WS_SIZEBOX | WS_SYSMENU | WS_THICKFRAME)) > 0) {
            SetWindowPos(this->hwnd,NULL,0,0,0,0,SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        }
    } else {
        this->style = style;
    }
}


LRESULT Pane::WndProc(HWND hwnd,UINT message,WPARAM w_param,LPARAM l_param)
{
    Pane* component = (Pane*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
    

    switch (message) {
        case WM_CREATE:
            component = (Pane*)((CREATESTRUCT*)l_param)->lpCreateParams;
            SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG)component);

            return 0;

        case WM_CLOSE:
            component->close();
                
            return 0;

        case WM_DESTROY:
            component->onClose();

            PostThreadMessage(GetWindowThreadProcessId(component->hwnd,NULL),WINDOW_DESTROYED,0,0);

            component->hwnd = NULL;
            component->exitcode_promise.set_value(component->exitcode);

            return 0;

        case WM_ENABLE:
        {int herp = 0;}

            break;

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
            if (component->enable_redraw) {
                HDC hdc;


                if ((component->window_class.style & (CS_OWNDC | CS_CLASSDC)) > 0) {
                    hdc = GetDCEx(hwnd,NULL,DCX_WINDOW);
                } else {
                    hdc = GetDCEx(hwnd,NULL,DCX_CACHE | DCX_WINDOW);
                }
                component->onPaint(hdc);
                ReleaseDC(hwnd,hdc);
            }

            return 0;
        
        case WM_SIZING:
            {
                RECT* proposed = (RECT*)l_param;
                int   proposed_x;
                int   proposed_y;
                int   proposed_height;
                int   proposed_width;


                proposed_x = proposed->left;
                proposed_y = proposed->top;
                proposed_height = proposed->bottom - proposed->top;
                proposed_width = proposed->right - proposed->left;

                if (component->position.x != proposed_x || component->position.y != proposed_y) {
                    component->onMoveProposed(proposed_x,proposed_y);
                    
                    proposed->left = component->position.x;
                    proposed->top = component->position.y;
                    proposed->bottom = proposed->top + proposed_height;
                    proposed->right = proposed->left + proposed_width;
                }
                if (component->size.height != proposed_height || component->size.width != proposed_width) {
                    component->onMoveProposed(proposed_x,proposed_y);

                    proposed->bottom = proposed->top + component->size.height;
                    proposed->right = proposed->left + component->size.width;
                }
            }

            break;

        case WM_STYLECHANGED:
            if (w_param == GWL_STYLE) {
                component->style = ((STYLESTRUCT*)l_param)->styleNew;
            } else if (w_param == GWL_EXSTYLE) {
                component->extended_style = ((STYLESTRUCT*)l_param)->styleNew;
            }

            break;

        case WM_WINDOWPOSCHANGING:
            {
                WINDOWPOS* proposed;                


                proposed = (WINDOWPOS*)l_param;
                if (
                    (proposed->flags & SWP_NOSIZE) == 0
                    &&
                    (component->size.width != proposed->cx || component->size.height != proposed->cy)
                ) {
                    component->onResizeProposed(proposed->cx,proposed->cy);
                }
                if (
                    (proposed->flags & SWP_NOMOVE) == 0
                    &&
                    (component->position.x != proposed->x || component->position.y != proposed->y)
                ){
                    component->onMoveProposed(proposed->x,proposed->y);
                }
            }

            break;

       case WM_WINDOWPOSCHANGED:
            {
                WINDOWPOS* updated;


                updated = (WINDOWPOS*)l_param;
                if ((updated->flags & SWP_NOSIZE) == 0) {
                    component->size.width = updated->cx;
                    component->size.height = updated->cy;
                    component->onResize();
                }
                if ((updated->flags & SWP_NOMOVE) == 0) {
                    component->position.x = updated->x;
                    component->position.y = updated->y;
                    component->onMove();
                }
            }

            break;
    }

    return DefWindowProc(hwnd,message,w_param,l_param);
}

bool Pane::operator==(const Pane& component) const
{
    return this->hwnd == component.hwnd;
}

bool Pane::operator!=(const Pane& component) const
{
    return this->hwnd != component.hwnd;
}