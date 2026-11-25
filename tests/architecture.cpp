#define LDID_NOTOOLS
#define LDID_NOSMIME
#define LDID_NOPLIST
#include <openssl/obj_mac.h>
#include "../ldid.cpp"

struct ArchitectureSeen { std::string value; };
struct RecordingProgress : ldid::Progress {
    void operator()(const std::string &value) const override { throw ArchitectureSeen{value}; }
    void operator()(double) const override {}
};
int main(int argc, char **argv) {
    if (argc != 2) return 3;
    uint32_t cpu = static_cast<uint32_t>(std::stoul(argv[1], nullptr, 0));
    uint32_t header[] = { MH_MAGIC_64, cpu, 0, MH_EXECUTE, 0, 0, 0, 0 };
    std::stringbuf output;
    RecordingProgress progress;
    try {
        ldid::Allocate(header, sizeof(header), output,
            ldid::fun([](const MachHeader &, Baton &, size_t) -> size_t { return 0; }),
            ldid::fun([](const MachHeader &, const Baton &, std::streambuf &, size_t, size_t, size_t, const std::string &, const char *, const ldid::Progress &) -> size_t { return 0; }), progress);
    } catch (const ArchitectureSeen &seen) { std::cout << seen.value << std::endl; return 0; }
      catch (const std::exception &error) { std::cerr << error.what(); return 2; }
    return 3;
}
