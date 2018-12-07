#include "catch.hpp"
#include "IPAddress.h"

TEST_CASE("TestCaseForLocalHost","[Localhost]")
{
	IPAddress address{ "127.0.0.0" };
	REQUIRE(address.toInteger() == 2130706432);
	REQUIRE(address.toString() == "127.0.0.0");
}
TEST_CASE("TestCaseForDomain", "[Domain]")
{
	IPAddress address{ "google.com" };
	REQUIRE(address.toInteger() == 3627734862);
	REQUIRE(address.toString() == "google.com");
}