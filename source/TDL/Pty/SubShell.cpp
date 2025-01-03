
#include <iostream>
#include <cstring>

#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <regex>
#include <unistd.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <pty.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "TDL/Pty/SubShell.hpp"

#define __USE_BSD

namespace tdl {

    SubShell::SubShell() {
        _path = "/bin/zsh";
        _type = ShellType::BASH;
        _activePath = getcwd(NULL, 0);
    }

    void SubShell::openSubShell(std::string const &path) {
    
        int rc;
        _path = path;
        _master_pty = posix_openpt(O_RDWR);
        if (_master_pty < 0)
        {
            fprintf(stderr, "Error %d on posix_openpt()\n", errno);
            return;
        }

        rc = grantpt(_master_pty);
        if (rc != 0)
        {
            fprintf(stderr, "Error %d on grantpt()\n", errno);
            return;
        }

        rc = unlockpt(_master_pty);
        if (rc != 0)
        {
            fprintf(stderr, "Error %d on unlockpt()\n", errno);
            return;
        }

        // Open the slave side ot the PTY
        _slave_pty = open(ptsname(_master_pty), O_RDWR);

        int pid = fork();
        if (pid == 0) {
            struct termios slave_orig_term_settings;
            struct termios new_term_settings;

            // CHILD

            close(_master_pty);
            rc = tcgetattr(_slave_pty, &slave_orig_term_settings);
            new_term_settings = slave_orig_term_settings;
            cfmakeraw (&new_term_settings);
            tcsetattr (_slave_pty, TCSANOW, &new_term_settings);

            close(0); // Close standard input (current terminal)
            close(1); // Close standard output (current terminal)
            close(2); // Close standard error (current terminal)

            dup(_slave_pty); // PTY becomes standard input (0)
            dup(_slave_pty); // PTY becomes standard output (1)
            dup(_slave_pty); // PTY becomes standard error (2)
            close(_slave_pty);
            setsid();

            // As the child is a session leader, set the controlling terminal to be the slave side of the PTY
            // (Mandatory for programs like the shell to make them manage correctly their outputs)
            ioctl(0, TIOCSCTTY, 1);
            {
            rc = execvp(_path.c_str(), NULL);
            }
        } else {
            close(_slave_pty);
        }
    }

    void SubShell::closeSubShell() {
        close(_master_pty);
        close(_slave_pty);
    }

    void SubShell::writeOnSubShell(const std::string &command) {
        write(_master_pty, command.c_str(), command.size());
    }

    std::string SubShell::readOnSubShell() {
        char buffer[4096];
        int rc;

        FD_SET(_master_pty, &fd_in);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        rc = select(_master_pty + 1, &fd_in, NULL, NULL, &timeout);

        if (rc == -1) {
            fprintf(stderr, "Error %d on select()\n", errno);
            exit(1);
        }

        if (FD_ISSET(_master_pty, &fd_in)) {
            rc = read(_master_pty, buffer, sizeof(buffer) - 1);
            if (rc > 0) {
                buffer[rc] = '\0';
                std::string str(buffer);
                return str;
            } else if (rc < 0) {
                fprintf(stderr, "Error %d on read master PTY\n", errno);
                exit(1);
            }
        }

        return "";
    }

    void SubShell::updateActivePath(std::string cmd) {
        if (cmd.find("cd") != std::string::npos) {
            if (cmd == "cd" || cmd == "cd ") {
                _activePath = getenv("HOME");
            } else {
                std::string path = cmd.substr(3);
                if (path.find("..") != std::string::npos) {
                   _activePath = _activePath.substr(0, _activePath.find_last_of("/"));
                } else {
                    //check if the path is a directory
                    struct stat sb;
                    if (stat(std::string(_activePath + "/" + path).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
                        _activePath = _activePath + "/" + path;
               }
            }
        }
    }
}