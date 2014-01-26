#include <strixa/window/Frame.h>

using namespace Strixa::Window;

Frame::Frame(const LPTSTR class_name,UINT class_style)
: Pane(class_name,class_style)
{
}

size_t Frame::addChild(Pane& child)
{
    for (Pane* component = this;component != NULL;component = component->getParent()) {
        if (&child == component) {
            throw std::exception("Window Pane cannot be be an ancestor of itself.");  //Unless the window's title is Philip J. Fry.
        }
    }

    child.setParent(this);
    this->children.push_back(&child);

    return this->children.size();
}

void Frame::addChildren(std::vector<Pane*> children)
{
    auto child = children.begin();
    while (child < children.end()) {
        this->addChild(**child);

        ++child;
    }
}

std::vector<Pane*> Frame::getChildren() const
{
    return std::vector<Pane*>(this->children);
}

void Frame::onClose()
{
    while (this->children.size() > 0) {
        Pane& child = *this->children.back();
        
        
        DestroyWindow(child.getNativeHandle());
        this->children.pop_back();
    }

    this->Pane::onClose();
}

bool Frame::isClosable()
{
    Pane** child;


    if (this->children.size() == 0) {
        return true;
    }

    child = this->children.data();
    for (;child <= &this->children.back();++child) {
        if (!(*child)->isClosable()) {
            return false;
        }
    }

    return this->Pane::isClosable();
}

void Frame::removeAllChildren()
{
    while (this->children.size() > 0) {
        this->removeChildAt(0);
    }
}

void Frame::removeChild(Pane& component)
{
    for (size_t offset = 0;offset < this->children.size();++offset){
        if (*this->children[offset] == component) {
            this->removeChildAt(offset);
            
            return;
        }
    }

    throw std::exception("Requested component is not a descendant of this Frame.");
}

void Frame::removeChildAt(size_t offset)
{
    if (offset < 0 || offset >= this->children.size()) {
        throw std::exception("Child at the specified offset does not exist.");
    }

    this->children.at(offset)->setParent(NULL);
    this->children.erase(this->children.begin() + offset);
}