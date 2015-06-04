#ifndef EXTERNAL_SORT_ERROR_HANDLING_H
#define EXTERNAL_SORT_ERROR_HANDLING_H

#include <exception>
#include <stdexcept>

#define CONCAT_DETAIL(x) #x
#define CONCAT_IT(x) CONCAT_DETAIL(x)
#define SOURCE_LOCATION __FILE__"(" CONCAT_IT(__LINE__) ") "

#define THROW_EXCEPTION(type, msg) \
        throw type(std::string(msg) + "\nRaise from " + SOURCE_LOCATION);

#endif //EXTERNAL_SORT_ERROR_HANDLING_H
