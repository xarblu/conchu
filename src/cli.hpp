#pragma once

class CommandLineInterface {
private:
    
public:
    /**
     * Parse CLI arguments
     * @param argc  argc forwarded from main()
     * @param argv  argv forwarded from main()
     */
    explicit CommandLineInterface(int argc, char *argv[]);

    /**
     * Print usage message
     */
    void usage() const;
};
