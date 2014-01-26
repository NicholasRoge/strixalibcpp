#ifndef Strixa_Window_Frame_H
#define Strixa_Window_Frame_H

#include <Strixa/Window/Pane.h>

namespace Strixa
{
    namespace Window
    {
        class Frame : public Pane
        {
            /* Instance Properties */
            private:
                std::vector<Pane*> children;

            /* Instance Methods */
            public:
                Frame(const LPTSTR class_name = L"Generic Window Frame",UINT class_style = CS_HREDRAW | CS_VREDRAW);

                virtual size_t addChild(Pane& child);

                virtual void addChildren(std::vector<Pane*> children);

                std::vector<Pane*> getChildren() const;

                virtual bool isClosable();
                
                virtual void onClose();

                virtual void removeAllChildren();

                virtual void removeChild(Pane& component);

                virtual void removeChildAt(size_t offset);
        };
    }
}

#endif