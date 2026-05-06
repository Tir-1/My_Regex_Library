#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "my_reg.h"


TEST_CASE("Тесты ДКА") {
    SECTION("Positiv match") {
        auto res = search("abc", "sdfdf12abcGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "abc");
    }
    SECTION("Negative match") {
        auto res = search("cat", "mephica");
        CHECK(res.get_success() == false);
    }

    SECTION("CLINI") {
        auto res = search("ab...b", "abbbbbbbb");
        REQUIRE(res.get_success());
        CHECK(res[0] == "abbbbbbbb");

        res = search("ab...", "aGHT");
        REQUIRE(res.get_success());
        CHECK(res[0] == "a");
    }

    SECTION("REPEAT") {
        auto res = search("a{3}b", "123aaab123");
        REQUIRE(res.get_success());
        CHECK(res[0] == "aaab");

        res = search("a{3}b", "aab");
        REQUIRE(res.get_success() == false);
    }

    SECTION("OPTIONAL") {
        auto res = search("a?b", "12345aab67896");
        REQUIRE(res.get_success());
        CHECK(res[0] == "ab");
        res = search("A(KL)?B", "12345ABB67896");
        REQUIRE(res.get_success());
        CHECK(res[0] == "AB");
    }

    SECTION("Test CLINI and repeat and OPTIONAL") {
        auto res = search("HELLO(,){3} (GHT)?(MEPHI)...", "1234567896HELLO,,, MEPHIMEPHIo");
        REQUIRE(res.get_success());
        CHECK(res[0] == "HELLO,,, MEPHIMEPHI");
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
        CHECK(res["1"].empty());
        CHECK(res["2"] == "GH");
        res = search("some(<first>(GH){3})", "a_someGHGHGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "someGHGHGH");
        CHECK(res["first"] == "GHGHGH");
    }
    SECTION("CLINI") {
        auto res = search("a(<first>T|GH)...", "aTTTTGH");
        REQUIRE(res.get_success());
        CHECK(res["first"] == "GH");
        res = search("a(<first>T|GH)...", "aPAM");
        REQUIRE(res.get_success());
        CHECK(res[0] == "a");
        CHECK(res["first"].empty());
    }

    SECTION("OR groups") {
        auto res = search("(<first>ABC)|(<second>GH)|(<third> GH)", "aGH");
        REQUIRE(res.get_success());
        CHECK(res[0] == "GH");
        CHECK(res["first"].empty());
        CHECK(res["second"] == "GH");
        CHECK(res["third"].empty());
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


TEST_CASE("Compile") {
    SECTION("success compile") {
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


TEST_CASE("isomorphic") {
    SECTION("Positiv") {
        bool flag;
        auto dka = compile("abc(GH|HELLO)(KL)...").save_DKA(flag);
        CHECK( isomorphic(dka, dka) == true);
        dka = compile("").save_DKA(flag);
        CHECK( isomorphic(dka, dka) == true);
    }
    SECTION("Negativ") {
        bool flag;
        auto dka1 = compile("MIFI|cat").save_DKA(flag);
        auto dka2 = compile("MIFI|pam").save_DKA(flag);
        CHECK( isomorphic(dka1, dka2) == false);
    }
}

TEST_CASE("k_path") {
    SECTION("empty") {
        bool flag;
        auto dka = compile("").save_DKA(flag);
        CHECK(recover_reg(dka).empty());
    }
    SECTION("test clini and optional") {
        string r = "(a...|b...)?";
        auto pat = compile(r);
        bool flag;
        auto dka = pat.save_DKA(flag);
        string reg = recover_reg(dka);
        auto new_dka = compile(reg).save_DKA(flag);
        //new_dka = min_dka(new_dka);
        CHECK( isomorphic(dka, new_dka)  == true);
    }
    SECTION("test OR and Repeat") {
        string r = "gh(a|b)HELLO{4}";
        auto pat = compile(r);
        bool flag;
        auto dka = pat.save_DKA(flag);
        string reg = recover_reg(dka);
        auto new_dka = compile(reg).save_DKA(flag);
       // new_dka = min_dka(new_dka);
        CHECK( isomorphic(dka, new_dka)  == true);
    }

    SECTION("Вложенные группы") {
        string r = "((a|b)...c)?";
        auto pat = compile(r);
        bool flag;
        auto dka = pat.save_DKA(flag);
        string reg = recover_reg(dka);
        auto new_dka = compile(reg).save_DKA(flag);
        CHECK( isomorphic(dka, new_dka)  == true);
    }
}


TEST_CASE("Binary Operations") {
    SECTION("Intersection") {
        bool flag;
        auto dka1 = compile("a|b").save_DKA(flag);
        auto dka2 = compile("c|a").save_DKA(flag);
        auto dka3 = compile("a").save_DKA(flag);
        auto res = intersect(dka1, dka2);
        CHECK( isomorphic(res, dka3) == true);
        dka1 = compile("Mephi").save_DKA(flag);
        dka2 = compile("pam").save_DKA(flag);
        dka3 = get_null_dka();
        res = intersect(dka1, dka2);
        CHECK( isomorphic(res, dka3) == true);
        dka1 = compile("A|(abc)...|HELLO").save_DKA(flag);
        dka2 = compile("mephi?|(abc)...").save_DKA(flag);
        dka3 = compile("(abc)...").save_DKA(flag);
        res = intersect(dka1, dka2);
        CHECK( isomorphic(res, dka3) == true);
    }
    SECTION("Difference") {
        bool flag;
        auto dka1 = compile("hello|mephi").save_DKA(flag);
        auto dka2 = compile("hello").save_DKA(flag);
        auto dka3 = compile("mephi").save_DKA(flag);
        auto res = difference(dka1, dka2);
        CHECK( isomorphic(res, dka3) == true);
        dka1 = compile("(abc)...|Hello?").save_DKA(flag);
        dka2 = compile("pam").save_DKA(flag);
        dka3 = compile("(abc)...|Hello?").save_DKA(flag);
        res = difference(dka1, dka2);
        CHECK( isomorphic(res, dka3) == true);
        dka1 = compile("Mephi|pam|(TL)...").save_DKA(flag);
        dka2 = compile("Mephi|pam|(TL)...").save_DKA(flag);
        dka3 = get_null_dka();
        res = difference(dka1, dka2);
        CHECK( isomorphic(res, dka3) == true);
    }
}
