#include "interface.hpp"

extern "C" {
    int getSignature()
    {
        return EXPECTED_SIGNATURE;
    }
}

