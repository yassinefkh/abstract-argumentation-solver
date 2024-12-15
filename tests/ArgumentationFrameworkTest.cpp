#define CATCH_CONFIG_MAIN
#include "include/catch.hpp"
#include "ArgumentationFramework.h"
#include <set>
#include <vector>
#include <string>

void verifyCompleteExtensions(const ArgumentationFramework& af, const std::vector<std::set<std::string>>& expected, const std::set<std::string>& skepticallyExpected, const std::set<std::string>& credulouslyExpected) {
    int counter = 0;
    auto completeExtensions = af.enumerateCompleteExtensions(counter);

    REQUIRE(completeExtensions.size() == expected.size());
    for (const auto& ext : expected) {
        REQUIRE(std::find(completeExtensions.begin(), completeExtensions.end(), ext) != completeExtensions.end());
    }

    std::set<std::string> skepticallyAccepted;
    for (const auto& arg : af.getArguments()) {
        int count = 0;
        if (af.isSkepticalComplete(arg, count)) {
            skepticallyAccepted.insert(arg);
        }
    }
    REQUIRE(skepticallyAccepted == skepticallyExpected);

    std::set<std::string> credulouslyAccepted;
    for (const auto& arg : af.getArguments()) {
        int count = 0;
        if (af.isCredulousComplete(arg, count)) {
            credulouslyAccepted.insert(arg);
        }
    }
    REQUIRE(credulouslyAccepted == credulouslyExpected);
}

void verifyStableExtensions(const ArgumentationFramework& af, const std::vector<std::set<std::string>>& expected, const std::set<std::string>& skepticallyExpected, const std::set<std::string>& credulouslyExpected) {
    int counter = 0;
    auto stableExtensions = af.enumerateStableExtensions(counter);

    REQUIRE(stableExtensions.size() == expected.size());
    for (const auto& ext : expected) {
        REQUIRE(std::find(stableExtensions.begin(), stableExtensions.end(), ext) != stableExtensions.end());
    }

    std::set<std::string> skepticallyAccepted;
    for (const auto& arg : af.getArguments()) {
        int count = 0;
        if (af.isSkepticalStable(arg, count)) {
            skepticallyAccepted.insert(arg);
        }
    }
    REQUIRE(skepticallyAccepted == skepticallyExpected);

    std::set<std::string> credulouslyAccepted;
    for (const auto& arg : af.getArguments()) {
        int count = 0;
        if (af.isCredulousStable(arg, count)) {
            credulouslyAccepted.insert(arg);
        }
    }
    REQUIRE(credulouslyAccepted == credulouslyExpected);
}

TEST_CASE("Test for two graphs") {
    SECTION("Graph 1") {
  
        ArgumentationFramework af1;

        af1.addArgument("A");
        af1.addArgument("B");
        af1.addArgument("C");
        af1.addArgument("D");
        af1.addArgument("E");

        af1.addAttack("A", "B");
        af1.addAttack("B", "A");
        af1.addAttack("A", "C");
        af1.addAttack("B", "C");
        af1.addAttack("C", "D");
        af1.addAttack("D", "E");

        verifyCompleteExtensions(
            af1,
            {{}, {"B", "D"}, {"A", "D"}},
            {}, 
            {"A", "B", "D"} 
        );

        verifyStableExtensions(
            af1,
            {{"B", "D"}, {"A", "D"}},
            {"D"}, 
            {"A", "B", "D"} 
        );
    }

    SECTION("Graph 2") {

        ArgumentationFramework af2;

        af2.addArgument("A");
        af2.addArgument("B");
        af2.addArgument("C");
        af2.addArgument("D");
        af2.addArgument("E");

        af2.addAttack("A", "B");
        af2.addAttack("B", "A");
        af2.addAttack("A", "C");
        af2.addAttack("B", "C");
        af2.addAttack("C", "D");
        af2.addAttack("D", "E");
        af2.addAttack("E", "D");

        verifyCompleteExtensions(
            af2,
            {{}, {"E"}, {"B"}, {"B", "D"}, {"B", "E"}, {"A"}, {"A", "D"}, {"A", "E"}},
            {}, 
            {"A", "B", "D", "E"} 
        );

        verifyStableExtensions(
            af2,
            {{"B", "D"}, {"B", "E"}, {"A", "E"}, {"A", "D"}},
            {}, 
            {"A", "B", "D", "E"} 
        );
    }
}



/*

POUR LA COMPILATION : 
g++ -std=c++17 -o test_runner tests/ArgumentationFrameworkTest.cpp src/ArgumentationFramework.cpp src/utility.cpp -I include
puis : ./test_runner
*/