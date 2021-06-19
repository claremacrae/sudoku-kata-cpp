#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"

#include "Utilities/PortableRandomNumberGenerator.h"

TEST_CASE("Check limits")
{
    PortableRandomNumberGenerator random;
    // Only 1 allowed value - must always return 0
    int result = random.Next(1);
    CHECK(result == 0);
}

TEST_CASE("PortableRandomNumberGenerator")
{
    PortableRandomNumberGenerator random(5); // use fixed seed

    std::vector<int> results;
    results.reserve(1000);
    for (int i = 0; i < 10000; ++i)
    {
        int random_number = random.next(1, 6);
        CHECK(random_number >= 1);
        CHECK(random_number <= 6);
        results.push_back(random_number);
    }
    ApprovalTests::Approvals::verifyAll(results);
}
