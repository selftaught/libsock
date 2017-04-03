#include "irc_server.h"

/**
 * [] Command handler
 * [] Connection handler
 * [] Server response handler
 * [] Private messages
 * [] Channels
 * [] Nicknames
 * [] Login authentication
 */

int main(int argc, char** argv) {
    int opt             = 0;
    bool pflag          = false;
    std::string channel = "";
    uint16_t port       = 0;

    while((opt = getopt(argc, argv, "h:p:")) != -1) {
        std::string optarg_str;

        if(optarg != NULL) {
            optarg_str = optarg;
        }

        switch(opt) {
            case 'c': {
                channel = optarg_str;
                break;
            }

            default: {
                help();
                break;
            }
        }
    }

    return 0;
}

/**
 * @TODO: print options and descriptions for each.
 */
void help() {

}
