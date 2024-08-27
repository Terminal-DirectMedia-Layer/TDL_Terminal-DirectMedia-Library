
#pragma once

#include <string>
#include <termios.h>

namespace tdl {

    enum shellType {
        BASH,
        SH,
        ZSH,
        FISH,
        TCSH,
        KSH,
        DASH,
        end /*this is the end of the enum*/
    };

    class subShell {
        public:
            subShell();
            ~subShell() = default;

            void openSubShell(std::string const &path = "/bin/bash");
            void closeSubShell();
            void writeOnSubShell(const std::string &command);
            std::string readOnSubShell();
            std::string getActivePath() const { return _activePath; }
            void updateActivePath(std::string cmd);
			void setActivePath(std::string const &path) { _activePath = path; }
			bool _ispwd = false;
        private:
            std::string _path; /*!< the path to the shell */
            shellType _type; /*!< the type of the shell */
            termios raw_mode; /*!< the raw mode of the terminal */
            int _slave_pty{}; /*!< the slave pty */
            int _master_pty{}; /*!< the master pty */
            fd_set fd_in; /*!< the fd set */
            std::string _activePath; /*!< the active path */

    };
}