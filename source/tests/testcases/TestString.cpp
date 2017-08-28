#include "InternalCommon.h"
#include "inc/String.h"
using namespace mwl;

void TestString() {
    String s1;
    MWL_ASSERT(s1 == "");
    MWL_ASSERT(s1.Empty());

    s1 += "Hello";
    MWL_ASSERT(s1.Len() == 5);

    String s2(s1);
    String s3;
    s3 = s2;

    String s4("Hello world", 5);
    MWL_ASSERT(s1 == "Hello");
    MWL_ASSERT(s2 == s1);
    MWL_ASSERT(s3 == s2);
    MWL_ASSERT(s4 == s1);

    MWL_ASSERT(s1.StartsWith('H'));
    MWL_ASSERT(s1.EndsWith("lo"));
    MWL_ASSERT(s1.Contains("ell"));

    int32_t lidx = s1.Find('l');
    int32_t ridx = s1.RFind('l');
    MWL_ASSERT(lidx == 2);
    MWL_ASSERT(ridx == 3);
    lidx = s1.Find('l', 4);
    ridx = s1.RFind('l', 4);
    MWL_ASSERT(lidx < 0);
    MWL_ASSERT(ridx < 0);
    lidx = s1.Find('h');
    ridx = s1.RFind('O');
    MWL_ASSERT(lidx < 0);
    MWL_ASSERT(ridx < 0);
    
    s1.Strip("ab");
    MWL_ASSERT(s1 == s2);
    s1.Strip("Ho");
    MWL_ASSERT(s1 == "ell");
    s1.LStrip("e");
    MWL_ASSERT(s1 == "ll");
    s1.RStrip("l", 1);
    MWL_ASSERT(s1 == "l");

    s1 = s2 + " world";
    MWL_ASSERT(s1 == "Hello world");

    MWL_ASSERT(s1.SubString(0, 5) == "Hello");
    MWL_ASSERT(s1.SubString(6) == "world");
    s1.Swap(s2);
    MWL_ASSERT(s1 == "Hello");
    MWL_ASSERT(s2 == "Hello world");

    s2.Format("%d + %d = %d", 1, 2, 3);
    MWL_ASSERT(s2 == "1 + 2 = 3");
    int32_t a = 0, b = 0, c = 0;
    MWL_ASSERT(s2.Extract("%d + %d = %d", &a, &b, &c) == 3);
    MWL_ASSERT(a == 1);
    MWL_ASSERT(b == 2);
    MWL_ASSERT(c == 3);

    s1 = "Hello";
    s1.Replace("H", "H");
    MWL_ASSERT(s1 == "Hello");
    s1.Replace("", " ");
    MWL_ASSERT(s1 == " Hello ");
    s1.Replace(" ", "");
    MWL_ASSERT(s1 == "Hello");

    s1.Replace("H", "h");
    MWL_ASSERT(s1 == "hello");
    s1.Replace("ll", "l");
    MWL_ASSERT(s1 == "helo");
    s1.Replace("l", "ll");
    MWL_ASSERT(s1 == "hello");
    s1.Replace("ell", "ELL s");
    MWL_ASSERT(s1 == "hELL so");
    s1.Replace("L", "l", 1);
    MWL_ASSERT(s1 == "hElL so");

    s1 = "aaaa";
    s1.Replace("a", "ab");
    MWL_ASSERT(s1 == "abababab");
    s1.Replace("ab", "b");
    MWL_ASSERT(s1 == "bbbb");

    Array<String> words;
    s2 = s1 = "a quick fox jump over the lazy dog";
    words = s2.Split();
    return;
    MWL_ASSERT(words.Size() == 8);
    MWL_ASSERT(words[0] == "a");
    MWL_ASSERT(words[3] == "jump");
    MWL_ASSERT(words[-1] == "dog");

    words = s2.Split(" ", 2);
    MWL_ASSERT(words.Size() == 3);
    MWL_ASSERT(words[0] == "a");
    MWL_ASSERT(words[1] == "quick");

    s2 = String(" ").Join(words);
    MWL_ASSERT(s2 == s1);

    words = s2.RSplit(2);
    MWL_ASSERT(words.Size() == 3);
    MWL_ASSERT(words[0] == "a quick fox jump over the");
    MWL_ASSERT(words[1] == "lazy");
    MWL_ASSERT(words[2] == "dog");

    words = s2.RSplit("o", 2);
    MWL_ASSERT(words.Size() == 3);
    MWL_ASSERT(words[0] == "a quick fox jump ");
    MWL_ASSERT(words[1] == "ver the lazy d");
    MWL_ASSERT(words[2] == "g");
    words = s2.Split("not_exist");
    MWL_ASSERT(words.Size() == 1);
    MWL_ASSERT(words[0] == s2);

    words = s2.Partition("quick");
    MWL_ASSERT(words.Size() == 3);
    MWL_ASSERT(words[0] == "a ");
    MWL_ASSERT(words[1] == "quick");
    MWL_ASSERT(words[2] == " fox jump over the lazy dog");

    words = s2.Partition("not_exist");
    MWL_ASSERT(words.Size() == 3);
    MWL_ASSERT(words[0] == s2);
    MWL_ASSERT(words[1] == "");
    MWL_ASSERT(words[2] == "");

    s2 = "1111";
    words = s2.Split("1");
    MWL_ASSERT(words.Size() == 5);
    MWL_ASSERT(words[0] == "");
    MWL_ASSERT(words[-1] == "");
    s2 = String("1").Join(words);
    MWL_ASSERT(s2 == "1111");

    words = s2.Split("1", 3);
    MWL_ASSERT(words.Size() == 4);
    MWL_ASSERT(words[0] == "");
    MWL_ASSERT(words[-1] == "1");
    s2 = String("1").Join(words);
    MWL_ASSERT(s2 == "1111");

    words = s2.RSplit("1", 3);
    MWL_ASSERT(words.Size() == 4);
    MWL_ASSERT(words[0] == "1");
    MWL_ASSERT(words[-1] == "");
    s2 = String("1").Join(words);
    MWL_ASSERT(s2 == "1111");

    s1.Clear();
    MWL_ASSERT(s1 == "");
    MWL_ASSERT(s1.Empty());
}
