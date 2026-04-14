#include <iostream>
#include <windows.h>
#include <xinput.h>
#include <conio.h>  // For _kbhit() and _getch()

#pragma comment(lib, "xinput.lib")

int main() {
    XINPUT_STATE state;
    
    std::cout << "Xbox Controller Reader (Press ESC to exit)\n\n";
    
    while(true) {
        // Check for ESC key to exit
        if(_kbhit()) {
            int key = _getch();
            if(key == 27) break;  // ESC key
        }
        
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        
        DWORD result = XInputGetState(0, &state);
        if(result == ERROR_SUCCESS) {
            // Normalize thumbstick values (-1.0 to 1.0)
            float yaw = state.Gamepad.sThumbLX / 32768.0f;
            float throttle = state.Gamepad.sThumbLY / 32768.0f;  // Fixed: was +32768/65536
            float roll = state.Gamepad.sThumbRX / 32768.0f;
            float pitch = state.Gamepad.sThumbRY / 32768.0f;
            
            bool arm = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
            
            // Clear line and print (prevents scrolling)
            std::cout << "\rThrottle: " << std::fixed << std::setprecision(3) 
                      << throttle << " Roll: " << roll 
                      << " Pitch: " << pitch 
                      << " Yaw: " << yaw 
                      << " Arm: " << (arm ? "ON " : "OFF") << std::flush;
        } else {
            std::cout << "\rNo Xbox controller detected          " << std::flush;
        }
        Sleep(50);  // Smoother 20Hz update
    }
    std::cout << "\nExiting...\n";
    return 0;
}