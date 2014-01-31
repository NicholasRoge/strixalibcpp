#ifndef Strixa_Util_ThreadLocal_H
#define Strixa_Util_ThreadLocal_H

#include <map>
#include <thread>

namespace Strixa
{
    namespace Util
    {
        template <typename CLASS>
        class ThreadLocal
        {
            /* Member Properties */
            private:
                std::map<std::thread::id,CLASS> instances;

            /* Member Methods */
            public:
                void destroy();
                
                template <typename ...ConstructorArguments>
                void construct(ConstructorArguments&&... arguments);

                CLASS& get();

                bool isConstructed() const;

                CLASS& operator *();

                CLASS* operator ->();
        };

        template <typename CLASS>
        template <typename ...ConstructorArguments>
        void ThreadLocal<CLASS>::construct(ConstructorArguments&&... arguments)
        {
            if (this->isConstructed()) {
                throw std::exception("Current thread's object has already been constructed.");
            }

            this->instances.emplace(std::forward(arguments...));
        }

        template <typename CLASS>
        void ThreadLocal<CLASS>::destroy()
        {
            this->instances.erase(std::this_thread::get_id());
        }

        template <typename CLASS>
        CLASS& ThreadLocal<CLASS>::get()
        {
            return this->instances[std::this_thread::get_id()];
        }

        template <typename CLASS>
        bool ThreadLocal<CLASS>::isConstructed() const
        {
            return this->instances.count(std::this_thread::get_id()) > 0;
        }

        template <typename CLASS>
        CLASS& ThreadLocal<CLASS>::operator *()
        {
            return this->get();
        }

        template <typename CLASS>
        CLASS* ThreadLocal<CLASS>::operator ->()
        {
            return &this->instances[std::this_thread::get_id()];
        }
    }
}

#endif