#include <iostream>
#include <string>
#include <cstring>
#include "include/string_view.hpp"

int main() {
    const char* cstr = "Hello, world!";
    std::string s = "C++ string";

    project2::string_view sv1(cstr);
    project2::string_view sv2(s);

    std::cout << "sv1: " << sv1 << " (size=" << sv1.size() << ")\n";
    std::cout << "sv2: " << sv2 << "\n";

    auto pos = sv1.find("world");
    if (pos != project2::string_view::npos)
        std::cout << "\"world\" at " << pos << "\n";

    project2::string_view part = sv1.substr(7, 5);
    std::cout << "part: " << part << "\n";

    // C++17-compatible starts/ends checks using find/rfind
    std::cout << std::boolalpha;
    bool starts = (sv1.find("Hello") == 0);
    bool ends = false;
    const char* tail = "world!";
    auto tail_len = std::strlen(tail);
    if (sv1.size() >= tail_len) {
        auto rpos = sv1.rfind(tail);
        if (rpos != project2::string_view::npos && rpos == sv1.size() - tail_len) ends = true;
    }
    std::cout << "starts_with \"Hello\": " << starts << "\n"
              << "ends_with \"world!\": " << ends << "\n";

    sv2.remove_prefix(4);
    std::cout << "sv2 after remove_prefix(4): " << sv2 << "\n";

    sv2.remove_suffix(3);
    std::cout << "sv2 after remove_suffix(3): " << sv2 << "\n";

    std::cout << "compare sv1 and \"Hello\": " << sv1.compare(project2::string_view("Hello")) << "\n";

    std::string s2(sv1.data(), sv1.size());
    std::cout << "to_string (via ctor): " << s2 << "\n";

    std::cout << "characters in sv1:";
    for (char c : sv1) std::cout << ' ' << c;
    std::cout << '\n';

    return 0;
}
