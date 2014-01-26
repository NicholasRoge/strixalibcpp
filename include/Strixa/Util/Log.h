#ifndef Strixa_Util_Log_H
#define Strixa_Util_Log_H

#include <mutex>
#include <ostream>


namespace Strixa
{
    namespace Util
    {
        class Log
        {
            /* Static Properties */
            private:
                static const char* EXCEPTION_MESSAGE_FORMAT;

            /* Member Properties */
            private:
                bool          close_stream;
                std::ostream* log_file;
                std::mutex    mutex;

            /* Member Methods */
            protected:
                virtual void write(std::string message,bool include_timestamp = true,bool escape_control_characters = true);

            public:
                Log();

                Log(const std::string& filename);

                Log(std::ostream& stream);

                ~Log();

                virtual void log(const std::exception& e);

                virtual void log(const char* format,...);

                virtual void operator()(const std::exception& e);

                virtual void operator()(const char* format,...);
        };
    }
}

#endif