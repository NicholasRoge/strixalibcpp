#ifndef Strixa_Util_File_H
#define Strixa_Util_File_H

#include <fstream>
#include <string>


namespace Strixa
{
    namespace Util
    {
        /* Function Prototypes */
        bool fileExists(const std::string& filename);

        std::string getNormalizedFilePath(const std::string& filename);

        /* Class Prototypes */
        class File
        {
            /* Member Properties */
            private:
                bool               readonly;
                std::string        path;
                unsigned long long size;
                std::fstream       stream;

            /* Member Methods */
            public:
                /* Creates a temporary file somewhere on the system. */
                File();

                File(const File& copy) = delete;

                File(File&& move);

                File(const std::string& path,bool readonly = false);

                ~File();

                void append(const std::string& tail);

                void append(const void* data,size_t data_length);

                void close();

                /* Returns the absolute location of the file on the filesystem. */
                std::string getPath() const;

                /* Returns the size of the file in bytes. */
                unsigned long long getSize() const;

                bool isReadOnly() const;

                std::string read(unsigned long long offset);

                std::string read(unsigned long long offset,size_t length);

                void read(unsigned long long offset,void* buffer,size_t length);

                char readChar(unsigned long long offset);

                int readInt(unsigned long long offset);

                void setReadOnly();

                void write(unsigned long long offset,const std::string& data);

                void write(unsigned long long offset,const void* data,size_t data_length);
        };
    }
}

#endif