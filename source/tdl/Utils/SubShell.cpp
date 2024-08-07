#include "tdl/Utils/SubShell.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <pty.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_BSD
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <regex>

namespace tdl {

    subShell::subShell() {
        _path = "/bin/bash";
        _type = shellType::BASH;
        tcgetattr (STDOUT_FILENO, &raw_mode);
        raw_mode.c_lflag &= ~ICANON;    /* Disable line-editing chars, etc.   */
        raw_mode.c_lflag &= ~ISIG;      /* Disable intr, quit & suspend chars */
        raw_mode.c_lflag &= ~ECHO;      /* Disable input echoing              */
        raw_mode.c_iflag &= ~IXON;      /* Pass ^S/^Q to subshell undisturbed */
        raw_mode.c_iflag &= ~ICRNL;     /* Don't translate CRs into LFs       */
        raw_mode.c_oflag &= ~OPOST;     /* Don't postprocess output           */
        raw_mode.c_cc[VTIME] = 0;       /* IE: wait forever, and return as    */
        raw_mode.c_cc[VMIN] = 1;        /* soon as a character is available   */
    }

    void subShell::openSubShell() {
    
        int rc;
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
            struct termios slave_orig_term_settings; // Saved terminal settings
            struct termios new_term_settings; // Current terminal settings

            // CHILD

            // Close the master side of the PTY
            close(_master_pty);

            // Save the defaults parameters of the slave side of the PTY
            rc = tcgetattr(_slave_pty, &slave_orig_term_settings);

            // Set RAW mode on slave side of PTY
            new_term_settings = slave_orig_term_settings;
            cfmakeraw (&new_term_settings);
            tcsetattr (_slave_pty, TCSANOW, &new_term_settings);

            // The slave side of the PTY becomes the standard input and outputs of the child process
            close(0); // Close standard input (current terminal)
            close(1); // Close standard output (current terminal)
            close(2); // Close standard error (current terminal)

            dup(_slave_pty); // PTY becomes standard input (0)
            dup(_slave_pty); // PTY becomes standard output (1)
            dup(_slave_pty); // PTY becomes standard error (2)

            // Now the original file descriptor is useless
            close(_slave_pty);

            // Make the current process a new session leader
            setsid();

            // As the child is a session leader, set the controlling terminal to be the slave side of the PTY
            // (Mandatory for programs like the shell to make them manage correctly their outputs)
            ioctl(0, TIOCSCTTY, 1);

            // Execution of the program
            {
            rc = execvp("/bin/bash", NULL);
            }
        } else {
            close(_slave_pty);


        }
    }

    void subShell::closeSubShell() {
        close(_master_pty);
        close(_slave_pty);
    }

    void subShell::writeOnSubShell(const std::string &command) {
        write(_master_pty, command.c_str(), command.size());
    }

    std::string subShell::readOnSubShell() {
        char buffer[4096];
        int rc;

        //FD_ZERO(&fd_in);
        FD_SET(_master_pty, &fd_in);

        struct timeval timeout;
        timeout.tv_sec = 0;  // Zero seconds
        timeout.tv_usec = 0; // Zero microseconds

        // Wait until data is available to read on _master_pty
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
}