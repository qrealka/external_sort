#ifndef EXTERNAL_SORT_ERROR_HANDLING_H
#define EXTERNAL_SORT_ERROR_HANDLING_H

#include <exception>
#include <stdexcept>

#define CONCAT_DETAIL(x) #x
#define CONCAT_IT(x) CONCAT_DETAIL(x)
#define SOURCE_LOCATION __FILE__"(" CONCAT_IT(__LINE__) ") "

// check 'condition' and throw exception if false
#define CHECK_CONTRACT(condition, msg) \
        if (false == (condition)) throw std::runtime_error(std::string(msg) + "\nSource location: " + SOURCE_LOCATION);

#endif //EXTERNAL_SORT_ERROR_HANDLING_H
