#ifndef Strixa_Window_Pane_H
#define Strixa_Window_Pane_H

#include <Windows.h>
#include <functional>
#include <map>
#include <Strixa/Graphics/Dimensions.h>
#include <Strixa/Graphics/Point.h>
#include <Strixa/Util/TaskableThread.h>

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
            /* Class Properties */
            private:
                static std::map<DWORD,UiThread> uithread_by_threadid;

            /* Class Methods */
            private:
                static LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM w_param,LPARAM l_param);

            /* Instance Properties */
            private:
                HBRUSH                        background;
                LRESULT                       exit_code;
                DWORD                         extended_style;
                HWND                          hwnd;
                RECT                          margin;
                Frame*                        parent;
                Strixa::Graphics::Point       position;
                Strixa::Graphics::Dimensions  size;
                DWORD                         style;
                UiThread*                     ui_thread;
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

                int getExitCode() const;

                DWORD getExtendedStyle() const;

                RECT getMargins() const;

                Frame* getParent() const;

                DWORD getStyle() const;

                HWND getNativeHandle() const;

                Strixa::Graphics::Point getPosition() const;

                Strixa::Graphics::Dimensions getSize() const;

                UiThread* getUiThread() const;

                virtual void init();

                void invalidate();

                bool isAlive();

                virtual bool isClosable();

                virtual void removeExtendedStyle(DWORD style);

                virtual void removeStyle(DWORD style);

                void setBackgroundColorHSL(short hue,float saturation,float lightness);

                void setBackgroundColorRGB(short red,short green,short blue);

                virtual void setExtendedStyle(DWORD style);

                void setParent(Frame* parent);

                void setPosition(long x,long y);

                virtual void setStyle(DWORD style);

                void setSize(long width,long height);

                bool operator==(const Pane& component) const;

                bool operator!=(const Pane& component) const;
        };
    }
}

#endif