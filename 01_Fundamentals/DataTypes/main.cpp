#include <iostream>
#include <locale>
#include <cstdint>
#include <cstddef>
#include <complex>
#include <print>
#include <expected>
#include <stacktrace>

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));
    std::ios::sync_with_stdio(false);

    // ================ SIGNED DATA TYPES ================ //
    int intNum = 45;
    float floatNum = 23.78F;
    double doubleNum = 456.89;
    char charLetter = 'C';
    bool myBool = true;
    std::wstring myString = L"My name is Kostas";
    short myShort = 12;
    long myLong = 14L;
    long long myLongLong = 35LL;
    long double piPrecision = 3.141592653589793238L;
    wchar_t wideLetter = L'Ω';
    std::wstring wideString = L"Με λένε Κώστα, εσένα;";

    // ================ UNSIGNED DATA TYPES ================ //
    unsigned int uInt = 4294967295U;
    unsigned short uShort = 65535U;
    unsigned long uLong = 4294967295UL;
    unsigned long long uLongLong = 18446744073709551615ULL;
    unsigned char uChar = 255U;

    // ================ FIXED-WIDTH INTEGERS ================ //
    int8_t   i8  = 127;
    int16_t  i16 = 32767;
    int32_t  i32 = 2147483647;
    int64_t  i64 = 9223372036854775807LL;
    uint8_t  u8  = 255;
    uint16_t u16 = 65535;
    uint32_t u32 = 4294967295U;
    uint64_t u64 = 18446744073709551615ULL;

    // ================ C++17 BYTE ================ //
    std::byte myByte = std::byte{0xFF};

    // ================ POINTERS ================ //
    int* ptr = nullptr;
    std::nullptr_t nullType = nullptr;

    // ================ AUTO ================ //
    auto autoInt    = 42;
    auto autoDouble = 3.14;

    // ================ COMPLEX ================ //
    std::complex<double> c(3.0, 4.0);

    // ================ VOID POINTER ================ //
    void* rawPtr = nullptr;

    // ================ C++23: std::expected ================ //
    std::expected<int, std::wstring> goodResult = 42;
    std::expected<int, std::wstring> badResult  = std::unexpected(L"something failed");

    // ================ C++23: std::stacktrace ================ //
    auto trace = std::stacktrace::current();


    // ===================================================================
    std::wcout << L"===================== SIGNED DATA TYPES =====================\n";
    std::wcout << L"int:              " << intNum       << L'\n';
    std::wcout << L"float:            " << floatNum     << L'\n';
    std::wcout << L"double:           " << doubleNum    << L'\n';
    std::wcout << L"char:             " << charLetter   << L'\n';
    std::wcout << L"bool:             " << (myBool ? L"TRUE" : L"FALSE") << L'\n';
    std::wcout << L"string:           " << myString     << L'\n';
    std::wcout << L"short:            " << myShort      << L'\n';
    std::wcout << L"long:             " << myLong       << L'\n';
    std::wcout << L"long long:        " << myLongLong   << L'\n';
    std::wcout << L"long double:      " << piPrecision  << L'\n';
    std::wcout << L"wchar_t:          " << wideLetter   << L'\n';
    std::wcout << L"wstring:          " << wideString   << L'\n';

    std::wcout << L"\n==================== UNSIGNED DATA TYPES ====================\n";
    std::wcout << L"unsigned int:       " << uInt      << L'\n';
    std::wcout << L"unsigned short:     " << uShort    << L'\n';
    std::wcout << L"unsigned long:      " << uLong     << L'\n';
    std::wcout << L"unsigned long long: " << uLongLong << L'\n';
    std::wcout << L"unsigned char:      " << uChar     << L'\n';

    std::wcout << L"\n=================== FIXED-WIDTH INTEGERS ===================\n";
    std::wcout << L"int8_t:   " << (int)i8   << L'\n';
    std::wcout << L"int16_t:  " << i16       << L'\n';
    std::wcout << L"int32_t:  " << i32       << L'\n';
    std::wcout << L"int64_t:  " << i64       << L'\n';
    std::wcout << L"uint8_t:  " << (int)u8   << L'\n';
    std::wcout << L"uint16_t: " << u16       << L'\n';
    std::wcout << L"uint32_t: " << u32       << L'\n';
    std::wcout << L"uint64_t: " << u64       << L'\n';

    std::wcout << L"\n======================= OTHER TYPES ========================\n";
    std::wcout << L"std::byte (hex):    " << std::hex << std::to_integer<int>(myByte) << std::dec << L'\n';
    std::wcout << L"nullptr ptr:        " << (ptr == nullptr ? L"null" : L"not null") << L'\n';
    std::wcout << L"nullptr_t:          " << (nullType == nullptr ? L"null" : L"not null") << L'\n';
    std::wcout << L"auto int:           " << autoInt    << L'\n';
    std::wcout << L"auto double:        " << autoDouble << L'\n';
    std::wcout << L"complex<double>:    " << c.real() << L" + " << c.imag() << L"i\n";
    std::wcout << L"void*:              " << (rawPtr == nullptr ? L"null" : L"not null") << L'\n';

    std::wcout << L"\n====================== C++23 ADDITIONS =====================\n";
    // std::print / std::println
    std::println("std::println (C++23): int = {}, float = {:.2f}", intNum, floatNum);

    // std::expected
    if (goodResult)
        std::wcout << L"expected (good):    " << goodResult.value() << L'\n';
    if (!badResult)
        std::wcout << L"expected (bad):     " << badResult.error()  << L'\n';

    // std::stacktrace
    std::wcout << L"stacktrace depth:   " << trace.size() << L" frames\n";


    std::wcout << L"\n==================== SIZE OF DATA TYPES ====================\n";
    std::wcout << L"int:          " << sizeof(intNum)      << L'\n';
    std::wcout << L"float:        " << sizeof(floatNum)    << L'\n';
    std::wcout << L"double:       " << sizeof(doubleNum)   << L'\n';
    std::wcout << L"char:         " << sizeof(charLetter)  << L'\n';
    std::wcout << L"bool:         " << sizeof(bool)        << L'\n';
    std::wcout << L"wstring:      " << sizeof(wideString)  << L'\n';
    std::wcout << L"short:        " << sizeof(myShort)     << L'\n';
    std::wcout << L"long:         " << sizeof(myLong)      << L'\n';
    std::wcout << L"long long:    " << sizeof(myLongLong)  << L'\n';
    std::wcout << L"long double:  " << sizeof(piPrecision) << L'\n';
    std::wcout << L"wchar_t:      " << sizeof(wideLetter)  << L'\n';
    std::wcout << L"int8_t:       " << sizeof(i8)          << L'\n';
    std::wcout << L"int64_t:      " << sizeof(i64)         << L'\n';
    std::wcout << L"uint64_t:     " << sizeof(u64)         << L'\n';
    std::wcout << L"std::byte:    " << sizeof(myByte)      << L'\n';
    std::wcout << L"complex<d>:   " << sizeof(c)           << L'\n';

    return 0;
}
