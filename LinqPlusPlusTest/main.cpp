#include "gmock/gmock.h"

//#ifdef _DEBUG
//    #include "vld.h"
//#endif

using namespace testing;

int main(int argc, char** argv)
{
    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
