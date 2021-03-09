#include <doctest.h>

#include "zgraph/zgraph.hpp"
#include "zgraph/zgraph_config.hpp"


#include "zgraph/graph/zadjancency_list_ugraph.hpp"



TEST_SUITE_BEGIN("core");

TEST_CASE("check version"){

    #ifndef ZGRAPH_VERSION_MAJOR
        #error "ZGRAPH_VERSION_MAJOR is undefined"
    #endif
    

    #ifndef ZGRAPH_VERSION_MINOR
        #error "ZGRAPH_VERSION_MINOR is undefined"
    #endif


    #ifndef ZGRAPH_VERSION_PATCH
        #error "ZGRAPH_VERSION_PATCH is undefined"
    #endif

    CHECK_EQ(ZGRAPH_VERSION_MAJOR , 0);
    CHECK_EQ(ZGRAPH_VERSION_MINOR , 1);
    CHECK_EQ(ZGRAPH_VERSION_PATCH , 0);
}



TEST_SUITE_END(); // end of testsuite core
