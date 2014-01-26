#include <Strixa/Window/Pane.h>

#include <Strixa/Window/Frame.h>

using namespace Strixa::Graphics;
using namespace Strixa::Window;

/* Global Constants */
const UINT WINDOW_CREATED = RegisterWindowMessage(L"window.created");

/* Global Variables */
std::map<DWORD,UiThread> Pane::uithread_by_threadid;  // Has the effect of ensuring two windows created on the same thread will share the same message pump.

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
        } else if (message.message == WM_QUIT) {
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
{
    this->background = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    this->exit_code = 0;
    this->extended_style = 0;
    this->hwnd = NULL;
    this->parent = NULL;
    this->style = 0;
    this->ui_thread = &Pane::uithread_by_threadid[GetCurrentThreadId()];

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
            this->exit_code = GetLastError();
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
            this->exit_code = exit_code;
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

int Pane::getExitCode() const
{
    while (this->hwnd != NULL);

    return this->exit_code;
}

DWORD Pane::getExtendedStyle() const
{
    return GetWindowLong(this->hwnd,GWL_EXSTYLE);
}

HWND Pane::getNativeHandle() const
{
    return this->hwnd;
}

Frame* Pane::getParent() const
{
    return this->parent;
}

Point Pane::getPosition() const
{
    return this->position;
}

Dimensions Pane::getSize() const
{
    return this->size;
}

DWORD Pane::getStyle() const
{
    return GetWindowLong(this->hwnd,GWL_STYLE);
}

UiThread* Pane::getUiThread() const
{
    return this->ui_thread;
}

void Pane::init()
{
    if (this->exit_code == 0) {
        this->ui_thread->now([&](){
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

            if (this->hwnd == NULL) {
                this->exit_code = GetLastError();

                return;
            }
        });
    }

    while (this->hwnd == NULL && this->exit_code == 0);

    if (this->exit_code != 0) {
        throw std::exception("Window Pane initialization failed.",this->exit_code);
    } else {
        this->onCreate();

        PostThreadMessage(GetWindowThreadProcessId(this->hwnd,NULL),WINDOW_CREATED,0,0);

        ShowWindow(this->hwnd,SW_SHOWNORMAL);
        UpdateWindow(this->hwnd);
    }
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

void Pane::onClose()
{
}

void Pane::onCreate()
{
}

void Pane::onMove()
{
}

bool Pane::onMoveProposed(const Point& proposed_position)
{
    return true;
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

bool Pane::onResizeProposed(const Dimensions& proposed_position)
{
    return true;
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

            PostQuitMessage(component->exit_code);
            component->hwnd = NULL;

            return 0;

        case WM_ENABLE:
        {int herp = 0;}

            break;

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
            {
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
                Point      proposed_position;
                RECT*      proposed_rect = (RECT*)l_param;
                Dimensions proposed_size;


                proposed_position.x = proposed_rect->left;
                proposed_position.y = proposed_rect->top;
                proposed_size.width = proposed_rect->right - proposed_rect->left;
                proposed_size.height = proposed_rect->bottom - proposed_rect->top;
                
                if (
                    !(
                        component->position != proposed_position
                        && 
                        component->onMoveProposed(proposed_position)
                    )
                    ||
                    !component->onResizeProposed(proposed_size)
                ) {
                    proposed_rect->left = component->position.x;
                    proposed_rect->top = component->position.y;
                    proposed_rect->right = component->position.x + component->size.width;
                    proposed_rect->bottom = component->position.y + component->size.height;
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
                if ((proposed->flags & SWP_NOSIZE) == 0) {
                    Dimensions proposed_size;


                    proposed_size.width = proposed->cx;
                    proposed_size.height = proposed->cy;
                    if (
                        component->size != proposed_size
                        && 
                        !component->onResizeProposed(proposed_size)
                    ) {
                        proposed->flags |= SWP_NOSIZE;
                    }
                }
                if ((proposed->flags & SWP_NOMOVE) == 0) {
                    Point      proposed_position;


                    proposed_position.x = proposed->x;
                    proposed_position.y = proposed->y;
                    if (
                        component->position != proposed_position 
                        &&
                        !component->onMoveProposed(proposed_position)
                    ) {
                        proposed->flags |= SWP_NOMOVE;
                    }
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