#pragma once

namespace zgraph::detail
{
    template<class iter_t>
    class zconst_range{
    public:
        zconst_range(iter_t begin, iter_t end)
        :   m_begin(begin),
            m_end(end){
        }

        auto begin(){
            return m_begin;
        } 
        auto begin()const{
            return m_begin;
        } 

        auto end(){
            return m_end;
        } 
        auto end()const{
            return m_end;
        }

    private:
        iter_t m_begin;
        iter_t m_end;
    };
}