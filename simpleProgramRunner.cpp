#include <chrono>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <wininet.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <psapi.h>

#pragma comment(  lib, "wininet.lib" )

void myConnectChecker()
{
    int connectAttempt = 0; // Counter for the number of connection attempts
    bool connected = false; // Boolean flag to indicate if the program is connected

    while (connectAttempt < 5 && !connected) // Loop while we haven't reached the maximum number of attempts and the program is not connected
    {
        HINTERNET hInternet = InternetOpen(TEXT("program"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); // Open an internet connection handle
        if (hInternet != NULL) // If the connection handle was successfully created
        {
            InternetCloseHandle(hInternet); // Close the connection handle
            std::cout << "Program Started\n"; // Print a message indicating that the program is online
            std::system("start C:\\myProgram\\startVBSscript.vbs"); // start the VBS script file
            connected = true; // Set the connected flag to true
        }
        else { std::cout << "Program Failed, Connection Error\n"; } // Print a message indicating that the program failed to connect

        std::this_thread::sleep_for(std::chrono::seconds(60)); // Wait for 60 seconds before trying again
        connectAttempt++; // Increment the attempt counter
    }
}

std::vector<std::string> myProgramChecker()
{
    std::vector<std::string> processes; // Vector to store the names of running processes
    DWORD processIds[1024], bytesReturned; // Declare an array to store the process IDs

    // The array of process IDs is stored in "processIds" and the number of bytes returned is stored in "bytesReturned"
    if (EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) // Call the Windows API function "EnumProcesses" to get an array of process IDs
    {
        int count = bytesReturned / sizeof(DWORD); // Calculate the number of process IDs in the "processIds" array
        for (int x = 0; x < count; x++) // Iterate through the process IDs in the "processIds" array
        {
            // Call the Windows API function "OpenProcess" to get a handle to a running process
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[x]);
            if (hProcess) // A valid handle is returned
            {
                char filename[MAX_PATH]; // Array to store the filename of the running process
                // Call the Windows API function "GetModuleFileNameExA" to get the filename of the executable module associated with the process
                DWORD length = GetModuleFileNameExA(hProcess, NULL, filename, MAX_PATH);
                if (length > 0) { processes.push_back(filename); } // A valid filename is returned
                CloseHandle(hProcess);
            }
        }
    }
    return processes; // returns a vector of strings
}

int main()
{
    const int runTime = 24 * 60 * 60; // Maximum runtime in seconds ( 24 hours )
    const int waitTime = 600; // Time between each loop iteration in seconds ( 10 minutes )
    int timeElapsed = 0; // Total elapsed time in seconds

    while (timeElapsed < runTime)
    {
        std::vector<std::string> processes = myProgramChecker();
        bool nodeExeRunning = false;

        for (const auto& process : processes) // Iterate through the vector of running processes
        {
            // Check if the filename contains "node.exe"
            if (process.find("node.exe") != std::string::npos)
            {
                nodeExeRunning = true; // Set the flag to true if "node.exe" is found
                break; // Exit the loop if "node.exe" is found
            }
        }
        if (!nodeExeRunning)
        {
            std::cout << "node.exe is not running.\n";
            myConnectChecker();
        }
        // std::cout << "Running processes:\n";
        // for ( const auto& process : processes ) { std::cout << process << '\n'; }

        std::this_thread::sleep_for(std::chrono::seconds(waitTime)); // Sleep for waitTime seconds
        timeElapsed += waitTime; // Update the total elapsed time
    }

    std::cout << "Simple Program Starter has ENDED\n";
    std::cin.get(); // Hang program until user presses a key.
    return 0; // Exit the program
}

/* Alternate
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

void myConnectChecker()
{
    int x = 0; // counter for number of attempts
    bool connected = false; // flag for whether internet is connected
    while (  x < 5 && !connected ) // try to connect 5 times or until connected
    {
        if (  std::system(  "ping -n 1 www.google.com" ) == 0 ) // Connected // .com >nul
        {
            std::cout << "Online\n";
            std::system(  "start C:\\publicBot\\startBot.vbs" ); // start the bot
            connected = true; // Internet Connection Successful
        }
        else // No Connection
        {
            std::cout << "Failed\n";
            std::this_thread::sleep_for(  std::chrono::milliseconds(  60000 ) ); // wait for 1 minute before trying again
        }
        x++; // increment the attempt counter
    }
}

int main()
{
    myConnectChecker();
    std::system(  "pause" ); // pause the console window before exiting
    return 0;
}
*/