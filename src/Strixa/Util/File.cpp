#include <Strixa/Util/File.h>

#include <assert.h>

using Strixa::Util::File;


/* Function Definitions */
bool Strixa::Util::fileExists(const std::string& filename)
{
    return std::fstream(filename).is_open();
}

std::string Strixa::Util::getNormalizedFilePath(const std::string& filename)
{
    return filename;
}

/* Class Definition:  File */
File::File()
{
    char temp_path[L_tmpnam];


    tmpnam(temp_path);

    this->stream.open(temp_path,std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    this->path = temp_path;
    this->readonly = !this->stream.is_open();
    this->size = 0;
}

File::File(File&& move)
{
    this->path = move.path;
    this->readonly = move.readonly;
    this->size = move.size;
    this->stream = std::move(move.stream);

    move.path.clear();
    move.readonly = true;
    move.size = 0;
}

File::File(const std::string& filename,bool readonly)
{
    this->path = Strixa::Util::getNormalizedFilePath(filename);
    
    this->stream.open(this->path.c_str(),std::fstream::in | std::fstream::out | std::fstream::binary);
    if (this->stream.is_open()) {
        this->stream.seekp(0,std::fstream::end);

        this->readonly = readonly;
        this->size = this->stream.tellp();
    } else {
        this->readonly = true;
        this->size = 0;
    }
}

File::~File()
{
    this->close();
}

void File::append(const std::string& tail)
{
    this->append(tail.c_str(),sizeof(char) * tail.size());
}

void File::append(const void* data,size_t data_length)
{
    /* General assertions. */
    assert(!this->isReadOnly());
    assert(data != nullptr);

    /* Perform the write. */
    this->stream.seekp(std::ios_base::end);
    this->stream.write((char*)data,data_length / sizeof(char));
    this->stream.flush();
    this->size += data_length;
}

void File::close()
{
    if (this->stream.is_open()) {
        this->stream.close();

        this->path.clear();
        this->readonly = true;
        this->size = 0;
    }
}

std::string File::getPath() const
{
    return this->path;
}

unsigned long long File::getSize() const
{
    return this->size;
}

bool File::isReadOnly() const
{
    return this->readonly;
}

std::string File::read(unsigned long long offset)
{
    static const size_t STRING_MAX_SIZE = std::string().max_size();


    return this->read(offset,STRING_MAX_SIZE);
}

std::string File::read(unsigned long long offset,size_t length)
{
    std::string result;


    /* General assertions. */
    assert(offset >= 0 && offset <= this->size);

    /* Perform the read. */
    if (this->size - offset > 0) {
        char*  buffer;
        size_t buffer_size;
        

        if (this->size - offset < length) {
            buffer_size = this->size - offset;
        } else {
            buffer_size = length;
        }
        buffer = new char[buffer_size];

        this->stream.seekp(offset);
        this->stream.read(buffer,buffer_size);
        result.assign(buffer,buffer_size);

        delete[] buffer;
    }

    return result;
}

void File::read(unsigned long long offset,void* buffer,size_t length)
{
    /* General assertions. */
    assert(offset >= 0 && offset <= this->size);
    assert(buffer != nullptr);

    /* Prepare the buffer. */
    memset(buffer,'\0',length);

    /* Perform the read. */
    this->stream.seekp(offset);
    this->stream.read((char*)buffer,length / sizeof(char));
}

void File::setReadOnly()
{
    this->readonly = true;
}

void File::write(unsigned long long offset,const std::string& data)
{
    this->write(offset,data.c_str(),sizeof(char) * data.size());
}

void File::write(unsigned long long offset,const void* data,size_t data_length)
{
    /* General assertions. */
    assert(!this->isReadOnly());
    assert(data != nullptr);

    /* Perform the write. */
    this->stream.seekp(offset);
    this->stream.write((char*)data,data_length / sizeof(char));
    this->stream.flush();
    this->size += data_length;
}