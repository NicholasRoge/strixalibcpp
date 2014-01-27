#include <assert.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Strixa/Util/Log.h>
#include <time.h>
#include <typeinfo>


using namespace std;
using namespace Strixa::Util;


/* Global Constants */
const char MONTH_STRING[][5] = {"JAN","FEB","MAR","APR","MAY","JUNE","JULY","AUG","SEP","OCT","NOV","DEC"};
const string FIND_AND_REPLACE_SEQUENCE[] = {
    string("\a",1), string("\\a",2),
    string("\b",1), string("\\b",2),
    string("\f",1), string("\\f",2),
    string("\n",1), string("\\n",2),
    string("\r",1), string("\\r",2),
    string("\t",1), string("\\t",2),
    string("\v",1), string("\\v",2),
    string("\0",1), string("\\0",2)
};
const string*const FIND_AND_REPLACE_SEQUENCE_END = FIND_AND_REPLACE_SEQUENCE + (sizeof(FIND_AND_REPLACE_SEQUENCE) / sizeof(string));

/* Prototypes */
string formatstring(const char* format,...);

string formatstring(const char* format,va_list arguments);

/* Definitions */
string formatstring(const char* format,...)
{
    va_list arguments;
    string  result;

    
    va_start(arguments,format);
    result = formatstring(format,arguments);
    va_end(arguments);

    return result;
}

string formatstring(const char* format,va_list arguments)
{
    char*  buffer;
    size_t buffer_size;
    string formatted_string;


    buffer_size = vsnprintf(NULL,0,format,arguments);
    buffer = new char[buffer_size + 1];
    buffer[buffer_size] = '\0';
    vsnprintf(buffer,buffer_size,format,arguments);
    formatted_string.assign(buffer,buffer_size);

    delete buffer;

    return formatted_string;
}

/* Log Class Defnitions */
const char* Log::EXCEPTION_MESSAGE_FORMAT = "An exception of type '%s' has occured.  Exception message:  %s.";


Log::Log()
{
    this->log_file = &std::cout;
    this->close_stream = false;
}

Log::Log(const string& filename)
{
    this->log_file = new ofstream(filename.c_str(),ios_base::app);
    this->close_stream = true;
}

Log::Log(std::ostream& stream)
{
    this->log_file = &stream;
    this->close_stream = false;
}

Log::~Log()
{
    if (this->close_stream) {
        delete this->log_file;
    }
}

void Log::write(std::string message,bool include_timestamp,bool escape_control_characters)
{
    string output;


    /* Get the current time for the timestamp. */
    if (include_timestamp) {
        tm*    time_info;
        time_t timestamp_raw;


        time(&timestamp_raw);
        time_info = localtime(&timestamp_raw);

        output += formatstring(
            "[%02i-%s-%04i %02i:%02i:%02i]",
            time_info->tm_mday,
            MONTH_STRING[time_info->tm_mon],
            time_info->tm_year + 1900,
            time_info->tm_hour,
            time_info->tm_min,
            time_info->tm_sec
        );
    }


    /* Replace any control characters in the message with an escaped version. */
    if (escape_control_characters) {
        const string* find_and_replace;
        size_t location;


        find_and_replace = FIND_AND_REPLACE_SEQUENCE;
        for (;find_and_replace < FIND_AND_REPLACE_SEQUENCE_END;find_and_replace += 2) {
            while ((location = message.find(*find_and_replace)) != string::npos) {
                message.replace(location,1,*(find_and_replace + 1));
            }
        }
    }

    /* Append the message to the output. */
    output.append(message);

    /* Write the log entry. */
    this->mutex.lock();
    *this->log_file << output << "\n";
    this->mutex.unlock();
}

void Log::log(const exception& e)
{
    this->write(formatstring(Log::EXCEPTION_MESSAGE_FORMAT,typeid(e).name(),e.what()));
}

void Log::log(const char* format,...)
{
    va_list arguments;

    
    va_start(arguments,format);
    this->write(formatstring(format,arguments));
    va_end(arguments);
}

void Log::operator()(const exception& e)
{
    this->log(e);
}

void Log::operator()(const char* format,...)
{
    va_list arguments;


    va_start(arguments,format);
    this->write(formatstring(format,arguments));
    va_end(arguments);
}