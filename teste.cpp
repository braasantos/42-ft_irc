#include <iostream>
#include <string>
#include <sstream>

static std::string newcomand;  // Store the accumulated command

// Function to handle incoming data
void handleIncomingData(std::string command) {
    // Check if the command contains Ctrl+D (EOT, ASCII 4)
    if (command.find("\x04") == std::string::npos) {
        // If Ctrl+D is not found, continue accumulating
        newcomand += command;
        std::cout << "new command 1: " << newcomand << std::endl;
        std::cout << "command 1: " << command << std::endl;
    } else {
        // If Ctrl+D is found, process the complete command
        newcomand += command;
        std::cout << "new command 2: " << newcomand << std::endl;
        std::cout << "command 2: " << command << std::endl;
        
        // Now process the full command
        std::cout << "Processing command: " << newcomand << std::endl;

        // Clear the buffer for the next command
        newcomand.clear();
    }
}

int main(int ac, char **av) {
    handleIncomingData(av[1]);
    return 0;
}

