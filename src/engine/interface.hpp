#ifndef INTERFACE_HPP
#define INTERFACE_HPP

constexpr auto EXPECTED_SIGNATURE = 0xDEADBEEF;

extern "C" {
    int getSignature();

    // vector<string> get pkmn headers
    // vector<string> get move headers

    // init pkmn defs
    // init move defs
}

#endif // INTERFACE_HPP
