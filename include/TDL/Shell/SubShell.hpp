
#ifndef TDL_SUBSHELL_HPP
    #define TDL_SUBSHELL_HPP

#include <string>

#include <termios.h>

namespace tdl {

    enum ShellType {
        BASH,
        SH,
        ZSH,
        FISH,
        TCSH,
        KSH,
        DASH,
        end /*this is the end of the enum*/
    };

    class SubShell {
        public:
            SubShell();
            ~SubShell() = default;

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
            ShellType _type; /*!< the type of the shell */
            termios raw_mode; /*!< the raw mode of the terminal */
            int _slave_pty{}; /*!< the slave pty */
            int _master_pty{}; /*!< the master pty */
            fd_set fd_in; /*!< the fd set */
            std::string _activePath; /*!< the active path */

    };
}
#endif //TDL_SUBSHELL_HPP