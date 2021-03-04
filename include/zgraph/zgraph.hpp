#pragma once
#ifndef ZGRAPH_ZGRAPH_HPP
#define ZGRAPH_ZGRAPH_HPP

#include <cstdint>
#include <iostream>

namespace zgraph {
    
    class MyClass
    {
    public:
        MyClass(const uint64_t size)
        : m_size(size)
        {

        }
        
        void hello_world()
        {
            std::cout<<"Hello World!\n";
        }
    private:
        uint64_t m_size;
    };

} // end namespace zgraph


#endif // ZGRAPH_ZGRAPH_HPP