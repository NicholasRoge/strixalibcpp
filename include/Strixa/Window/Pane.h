#ifndef Strixa_Window_Pane_H
#define Strixa_Window_Pane_H

#include <Windows.h>
#include <functional>
#include <future>
#include <map>
#include <Strixa/Graphics/Dimensions.h>
#include <Strixa/Graphics/Point.h>
#include <Strixa/Util/TaskableThread.h>
#include <Strixa/Util/ThreadObject.h>

namespace Strixa
{
    namespace Window
    {
        class Frame;


        class UiThread : public Strixa::Util::TaskableThread
        {
            /* Instance Methods */
            private:
                void run();

            public:
                UiThread();

                ~UiThread();
        };

        class Pane
        {
            /* Class Methods */
            private:
                static LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM w_param,LPARAM l_param);

            /* Instance Properties */
            private:
                HBRUSH                        background;
                bool                          enable_redraw : 1;
                DWORD                         exitcode;
                std::future<DWORD>            exitcode_future;
                std::promise<DWORD>           exitcode_promise;
                DWORD                         extended_style;
                HWND                          hwnd;
                bool                          initialized;
                RECT                          margin;
                Frame*                        parent;
                Strixa::Graphics::Point       position;
                Strixa::Graphics::Dimensions  size;
                DWORD                         style;
                UiThread&                     ui_thread;
                WNDCLASSEX                    window_class;

            /* Instance Methods*/
            protected:
                virtual void onCreate();

                virtual void onClose();

                virtual void onMove();

                virtual bool onMoveProposed(const Strixa::Graphics::Point& proposed_position);

                virtual void onPaint(HDC device_context);

                virtual void onResize();

                virtual bool onResizeProposed(const Strixa::Graphics::Dimensions& proposed_dimensions);

            public:
                Pane(const LPTSTR class_name = L"Generic Window Pane",UINT class_style = CS_HREDRAW | CS_VREDRAW);

                virtual ~Pane();

                virtual void addExtendedStyle(DWORD style);

                virtual void addStyle(DWORD style);

                bool close(int exit_code = 0,bool force = false);

                int getBackgroundColor() const;

                int getExitCode();

                DWORD getExtendedStyle() const;

                RECT getMargins() const;

                Frame* getParent() const;

                DWORD getStyle() const;

                HWND getNativeHandle() const;

                Strixa::Graphics::Point getPosition() const;

                Strixa::Graphics::Dimensions getSize() const;

                UiThread& getUiThread() const;

                virtual bool init();

                void invalidate();

                bool isAlive();

                bool isRedrawEnabled() const;

                virtual bool isClosable();

                virtual void removeExtendedStyle(DWORD style);

                virtual void removeStyle(DWORD style);

                void setBackgroundColorHSL(short hue,float saturation,float lightness);

                void setBackgroundColorRGB(short red,short green,short blue);

                virtual void setExtendedStyle(DWORD style);

                void setParent(Frame* parent);

                void setRedrawEnabled(bool enabled);

                void setPosition(long x,long y);

                virtual void setStyle(DWORD style);

                void setSize(long width,long height);

                bool operator==(const Pane& component) const;

                bool operator!=(const Pane& component) const;
        };
    }
}

#endif