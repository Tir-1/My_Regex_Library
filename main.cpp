#include <iostream>
#include "headers/my_reg.h"
#include <iostream>
using std::cout, std::endl;
int main() {
    string text1 = "sdfdf12abcGH";
    string r1 = "abc";
    string text2 = "mepephii";
    string r2 = "m(ep)...(ab|hi)";
    string text3 = "mephi";
    string r3 = "cat";
    string text4 = "abbbbbbbb";
    string r4 = "ab...";
    string text5 = "GHmeabcGH";
    string r5 = "me(ph)?abc";
    string text6 = "GHmephabcGH";
    string r6 = "me(ph)?abc";
    string text7 = "aaab";
    string r7 = "a{3}b";
    string text8 = "hello?Mister Anderson";
    string r8 = "hello%?%";
    string text9 = "GH";
    string r9 = "(hello||abc)GH";

    string text10 = "aaaHELLO";
        string r10 = "(<first>a...)(<second>a...)D?(<third>HELLO)";

    string text11 = "aaaaaaaaaaa";
    string r11 = "(<JK>a...)(<gh>a...)|(<2>a...)(<3>a...)(<JK2>a...)";
    string text12 = "aaabbbGH";
    string r12 = "(<first>a...(<second>bbb)GH)";
    string text13 = "aGH";
    string r13 = "a(<1>b)?(<2>GH)";
    string text14 = "helloGH";
    string r14 = "(<jk>hello)(<2>T)";
    string text15 = "aaaaaaaGHJK";
    string r15 = "(<first>GH|JK)...";
    string text16 = "aaaaaaaaT";
    string r16 = "gh|T";
    string text17 = "aassddddfGHTyesCCCCCCCCCKL...Blklklklk";
    string r17 = "GHT(<g>group|yes)?C...D...KL%...%(<null>)(<third>A|B?)";
    string text18 = "GGGGab";
    string r18 = "(a...|b...)?";
    string text19 = "";
    string r19 = "(a...)";
    string r20 = "(a|)...b";
    string text20 = "b";
    auto pat = compile(r18);
    bool flag;
    auto at = pat.save_DKA(flag);
    string gh = recover_reg(at);
    cout << gh << endl;
    Output_reg result =pat.search(text15);
    for (auto& i : result) {
        cout << i << endl;
    }
    cout << "gh";
    cout << result.get_success() << endl;
    return 0;
}
