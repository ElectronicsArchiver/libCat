// -*- mode: c++ -*-
// vim: set ft=cpp:
#include <linux>

auto nix::open_file(char const* p_file_path, nix::OpenMode file_mode,
                    nix::OpenFlags flags) -> Result<nix::FileDescriptor> {
    // TODO: Figure out how to best support `close_exec`.
    // TODO: `large_file` should only be enabled on 64-bit targets.
    return syscall3(2, p_file_path,
                    flags | file_mode | nix::OpenFlags::large_file, file_mode);
}