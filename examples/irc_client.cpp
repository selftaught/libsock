#include "irc_client.h"

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
    int opt          = 0;
    bool pflag       = false;
    std::string host = "";
    std::string nick = "";
    std::string pass = "";
    uint16_t port    = 0;

    while((opt = getopt(argc, argv, "h:p:")) != -1) {
        std::string optarg_str;

        if(optarg != NULL) {
            optarg_str = optarg;
        }

        switch(opt) {
            case 'n': {
                nick = optarg_str;
                break;
            }

            case 'p': {
                pass = optarg_str;
                break;
            }

            // 's' is short for server here. 'h' is reserved for help.
            case 's': {
                host = optarg_str;
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
