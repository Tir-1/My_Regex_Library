#include <catch2/catch_test_macros.hpp>
#include "../headers/my_reg.h"


TEST_CASE("Тесты ДКА") {
    SECTION("Simple match") {
        auto res = search("abc", "sdfdf12abcGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "abc");
    }
    SECTION("Negative match") {
        auto res = search("cat", "mephi");
        CHECK_FALSE(res.get_success());
    }

    SECTION("Test CLINI and repeat and OPTIONAL") {
        auto res = search("ab...", "abbbbbbbb");
        REQUIRE(res.get_success());
        CHECK(res[0] == "abbbbbbbb");

        res = search("ab...", "aGHT");
        REQUIRE(res.get_success());
        CHECK(res[0] == "a");

        res = search("a{3}b", "aaab");
        REQUIRE(res.get_success());
        CHECK(res[0] == "aaab");

        res = search("a{3}b", "aab");
        REQUIRE(res.get_success() == false);

        res = search("a?b", "12345aab67896");
        REQUIRE(res.get_success());
        CHECK(res[0] == "ab");
    }

    SECTION("protect symbol") {
        auto res = search("hello%?%", "hello?Mister Anderson");
        REQUIRE(res.get_success());
        CHECK(res[0] == "hello?");
    }

    SECTION("OR, default and empty OR") {
        auto res = search("(MEPHI|hello)T", "GHhelloT");
        CHECK(res.get_success());
        CHECK(res[0] == "helloT");
        res = search("ab|abcdef{2}", "abcdeff");
        CHECK(res.get_success());
        CHECK(res[0] == "abcdeff");
        res = search("(hello||abc)GH", "GH");
        CHECK(res.get_success());
        CHECK(res[0] == "GH");
        res = search("hello|abc", "mephi");
        CHECK(res.get_success() == false);
    }
}

TEST_CASE("Тесты НКА для групп") {
    SECTION("Выбирает самую левую группу") {
        auto res = search("(<first>a...)(<second>a...)D?(<third>HELLO)", "aaaHELLO");
        REQUIRE(res.get_success());
        CHECK(res[0] == "aaaHELLO");
        CHECK(res["first"] == "aaa");
        CHECK(res["second"].empty());
        CHECK(res["third"] == "HELLO");
    }

    SECTION("Вложенные группы") {
        auto res = search("(<first>a...(<second>bbb)GH)", "aaabbbGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "aaabbbGH");
        CHECK(res["first"] == "aaabbbGH");
        CHECK(res["second"] == "bbb");
    }

    SECTION("Optional, repeat groups") {
        auto res = search("a(<1>b)?(<2>GH)", "aGH");
        REQUIRE(res.get_success());
        CHECK(res["1"] == "");
        CHECK(res["2"] == "GH");
        res = search("(<first>GH){3}", "aGHGHGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "GHGHGH");
        CHECK(res["first"] == "GH");
    }


    SECTION("OR groups") {
        auto res = search("(<first>ABC)|(<second>GH)|(<third> GH)", "aGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "GH");
        CHECK(res["first"] == "");
        CHECK(res["second"] == "GH");
        CHECK(res["third"] == "");
    }

    SECTION("Complex test") {
        string text = "aassddddfGHTyesCCCCCCCCCKL...Blklklklk";
        string reg = "GHT(<first>group|yes)?C...D...KL%...%(<null>)(<third>A|B?)";
        auto res = search(reg, text);
        REQUIRE(res.get_success());
        CHECK(res[0] == "GHTyesCCCCCCCCCKL...B");
        CHECK(res["first"] == "yes");
        CHECK(res["null"] == "");
        CHECK(res["third"] == "B");
    }
}

// БЛОК 3: Тесты компиляции и ошибок
TEST_CASE("Compile") {
    SECTION("") {
        string text = "aassddddfGHTyesCCCCCCCCCKKL...Blklklklk";
        string reg = "GHT(<first>group|yes)?C...D...K{2}L%...%(<null>)(<third>A|B?)";
        auto pattern = compile(reg);
        auto res = pattern.search(text);
        REQUIRE(res.get_success());
        CHECK(res[0] == "GHTyesCCCCCCCCCKKL...B");
        CHECK(res["first"] == "yes");
        CHECK(res["null"] == "");
        CHECK(res["third"] == "B");
    }

    SECTION("invalid syntax") {
        CHECK_THROWS_AS(compile("(<first>a"), std::invalid_argument);
        CHECK_THROWS_AS(compile("a{abc}b"), std::invalid_argument);
    }
}

TEST_CASE("k_path") {
    SECTION("test") {
        string r18 = "(a...|b...)?";
        auto pat = compile(r18);
        bool flag;
        auto at = pat.save_DKA(flag);
        CHECK(recover_reg(at) =="((eps)|(a|(a((eps|a)...))(eps|a)))|(b|(b((eps|b)...))(eps|b))");
    }
}