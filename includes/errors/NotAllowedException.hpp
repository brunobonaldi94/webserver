#pragma once

#include <iostream>
#include <exception>

class NotAllowedException : public std::exception
    {
        private:
            std::string _objectName;
        public:
            NotAllowedException(const std::string& objectName);
            virtual ~NotAllowedException() throw();
            virtual const char* what() const throw();
    };