# Architecture allocation regression

On macOS, with the AltSign OpenSSL dependency present, compile from the ldid directory:

```sh
xcrun clang++ -std=c++14 -Wno-deprecated-declarations -Wno-macro-redefined -Wno-vla-cxx-extension -I ../OpenSSL/ios/include tests/architecture.cpp -framework CoreFoundation -o /tmp/ldid-architecture-test
/tmp/ldid-architecture-test 0x0100000c
/tmp/ldid-architecture-test 0x0200000c
/tmp/ldid-architecture-test 0x7fffffff
```

The first two commands must exit 0 and report `arm64` and `arm64_32`. The unknown architecture must exit 2 with an unsupported-CPU error. The test stops at the progress callback, where the original implementation crashed converting a null architecture label to `std::string`. Before the fix, the arm64_32 case exits on SIGSEGV.
