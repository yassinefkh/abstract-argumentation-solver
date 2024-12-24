#define CATCH_CONFIG_MAIN
#include "include/catch.hpp"
#include "ArgumentationFramework.h"
#include <set>
#include <vector>
#include <string>

// fichier de tests unitaires pour vérifier les fonctionnalités de la classe ArgumentationFramework
// utilise Catch2 (fichier fourni dans tests/include) comme framework de test pour valider les extensions calculées et l'acceptation des arguments


// fonction utilitaire pour vérifier les extensions complètes calculées par le graphe
void verifyCompleteExtensions(const ArgumentationFramework &af, const std::vector<std::set<std::string>> &expected, const std::set<std::string> &skepticallyExpected, const std::set<std::string> &credulouslyExpected)
{
    int counter = 0;
    auto completeExtensions = af.enumerateCompleteExtensions(counter);
    // vérifie si le nombre d'extensions complètes correspond à l'attendu
    
    REQUIRE(completeExtensions.size() == expected.size());
    for (const auto &ext : expected)
    {   
        // vérifie que chaque extension attendue est dans les résultats calculés
        REQUIRE(std::find(completeExtensions.begin(), completeExtensions.end(), ext) != completeExtensions.end());
    }

    // vérifie les arguments acceptés sceptiquement
    std::set<std::string> skepticallyAccepted;
    for (const auto &arg : af.getArguments())
    {
        int count = 0;
        if (af.isSkepticalComplete(arg, count))
        {
            skepticallyAccepted.insert(arg);
        }
    }
    REQUIRE(skepticallyAccepted == skepticallyExpected);

    // vérifie les arguments acceptés crédulement
    std::set<std::string> credulouslyAccepted;
    for (const auto &arg : af.getArguments())
    {
        int count = 0;
        if (af.isCredulousComplete(arg, count))
        {
            credulouslyAccepted.insert(arg);
        }
    }
    REQUIRE(credulouslyAccepted == credulouslyExpected);
}

// fonction utilitaire pour vérifier les extensions stables calculées par le graphe
void verifyStableExtensions(const ArgumentationFramework &af, const std::vector<std::set<std::string>> &expected, const std::set<std::string> &skepticallyExpected, const std::set<std::string> &credulouslyExpected)
{
    int counter = 0;
    auto stableExtensions = af.enumerateStableExtensions(counter);

    REQUIRE(stableExtensions.size() == expected.size());
    for (const auto &ext : expected)
    {
        REQUIRE(std::find(stableExtensions.begin(), stableExtensions.end(), ext) != stableExtensions.end());
    }

    std::set<std::string> skepticallyAccepted;
    for (const auto &arg : af.getArguments())
    {
        int count = 0;
        if (af.isSkepticalStable(arg, count))
        {
            skepticallyAccepted.insert(arg);
        }
    }
    REQUIRE(skepticallyAccepted == skepticallyExpected);

    std::set<std::string> credulouslyAccepted;
    for (const auto &arg : af.getArguments())
    {
        int count = 0;
        if (af.isCredulousStable(arg, count))
        {
            credulouslyAccepted.insert(arg);
        }
    }
    REQUIRE(credulouslyAccepted == credulouslyExpected);
}

// tests unitaires pour différents graphes d'argumentation
TEST_CASE("Test for different graphs")
{
    SECTION("Graph 1")
    {

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
            {"A", "B", "D"});

        verifyStableExtensions(
            af1,
            {{"B", "D"}, {"A", "D"}},
            {"D"},
            {"A", "B", "D"});
    }

    SECTION("Graph 2")
    {

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
            {"A", "B", "D", "E"});

        verifyStableExtensions(
            af2,
            {{"B", "D"}, {"B", "E"}, {"A", "E"}, {"A", "D"}},
            {},
            {"A", "B", "D", "E"});
    }

    SECTION("Graph 3")
    {
        ArgumentationFramework af3;

        af3.addArgument("A");
        af3.addArgument("B");
        af3.addArgument("C");
        af3.addArgument("D");
        af3.addArgument("E");
        af3.addArgument("F");

        af3.addAttack("A", "B");
        af3.addAttack("B", "A");
        af3.addAttack("A", "C");
        af3.addAttack("B", "C");
        af3.addAttack("C", "D");
        af3.addAttack("D", "E");
        af3.addAttack("E", "D");
        af3.addAttack("F", "B");
        af3.addAttack("A", "F");

        verifyCompleteExtensions(
            af3,
            {{}, {"E"}, {"A"}, {"A", "D"}, {"A", "E"}},
            {},
            {"A", "D", "E"});

        verifyStableExtensions(
            af3,
            {{"A", "E"}, {"A", "D"}},
            {"A"},
            {"A", "D", "E"});
    }


    SECTION("Graph 4") {
    ArgumentationFramework af4;

    af4.addArgument("A");
    af4.addArgument("B");
    af4.addArgument("C");
    af4.addArgument("D");
    af4.addArgument("E");

    af4.addAttack("A", "B");
    af4.addAttack("B", "A");
    af4.addAttack("A", "C");
    af4.addAttack("C", "A");
    af4.addAttack("B", "C");
    af4.addAttack("C", "B");
    af4.addAttack("C", "D");
    af4.addAttack("D", "C");
    af4.addAttack("D", "E");
    af4.addAttack("E", "D");

    verifyCompleteExtensions(
        af4,
        {{}, {"E"}, {"D"}, {"B"}, {"A"}, {"B", "D"}, {"C", "E"}, {"A", "E"}, {"A", "D"}, {"B", "E"}},
        {},
        {"A", "B", "C", "D", "E"}
    );

    verifyStableExtensions(
        af4,
        {{"B", "D"}, {"C", "E"}, {"A", "E"}, {"A", "D"}, {"B", "E"}},
        {},
        {"A", "B", "C", "D", "E"}
    );
}

SECTION("Graph 5") {
    ArgumentationFramework af5;

    af5.addArgument("A");
    af5.addArgument("B");
    af5.addArgument("C");
    af5.addArgument("D");
    af5.addArgument("E");
    af5.addArgument("F");
    af5.addArgument("G");

    af5.addAttack("A", "B");
    af5.addAttack("B", "C");
    af5.addAttack("C", "D");
    af5.addAttack("D", "E");
    af5.addAttack("E", "F");
    af5.addAttack("F", "G");
    af5.addAttack("G", "A");
    af5.addAttack("A", "E");
    af5.addAttack("B", "A");
    af5.addAttack("C", "G");
    af5.addAttack("D", "A");
    af5.addAttack("E", "G");
    af5.addAttack("F", "B");
    af5.addAttack("G", "C");

    verifyCompleteExtensions(
        af5,
        {{}, {"A", "C", "F"}},
        {},
        {"A", "C", "F"}
    );

    verifyStableExtensions(
        af5,
        {{"A", "C", "F"}},
        {"A", "C", "F"},
        {"A", "C", "F"}
    );
}



}


/*

POUR LA COMPILATION :
g++ -std=c++17 -o test_runner tests/ArgumentationFrameworkTest.cpp src/ArgumentationFramework.cpp src/utility.cpp -I include
puis : ./test_runner
*/