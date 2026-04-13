#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Receiver: Light
class Light {
private:
    std::string location;
    int brightness;

public:
    Light(const std::string& loc) : location(loc), brightness(0) {}

    void on() {
        brightness = 100;
        std::cout << location << " light is ON (100%)" << std::endl;
    }

    void off() {
        brightness = 0;
        std::cout << location << " light is OFF" << std::endl;
    }

    void dim(int level) {
        brightness = level;
        std::cout << location << " light dimmed to " << level << "%" << std::endl;
    }
};

// Receiver: Fan
class Fan {
private:
    std::string location;
    int speed;

public:
    Fan(const std::string& loc) : location(loc), speed(0) {}

    void high() {
        speed = 3;
        std::cout << location << " fan speed is HIGH" << std::endl;
    }

    void medium() {
        speed = 2;
        std::cout << location << " fan speed is MEDIUM" << std::endl;
    }

    void low() {
        speed = 1;
        std::cout << location << " fan speed is LOW" << std::endl;
    }

    void off() {
        speed = 0;
        std::cout << location << " fan is OFF" << std::endl;
    }
};

// Command Interface
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Concrete Command: Light On
class LightOnCommand : public Command {
private:
    Light* light;

public:
    LightOnCommand(Light* l) : light(l) {}

    void execute() override {
        light->on();
    }

    void undo() override {
        light->off();
    }
};

// Concrete Command: Light Off
class LightOffCommand : public Command {
private:
    Light* light;

public:
    LightOffCommand(Light* l) : light(l) {}

    void execute() override {
        light->off();
    }

    void undo() override {
        light->on();
    }
};

// Concrete Command: Light Dim
class LightDimCommand : public Command {
private:
    Light* light;
    int prevLevel;
    int newLevel;

public:
    LightDimCommand(Light* l, int level) : light(l), newLevel(level), prevLevel(100) {}

    void execute() override {
        prevLevel = 100; // Store current for undo
        light->dim(newLevel);
    }

    void undo() override {
        light->dim(prevLevel);
    }
};

// Concrete Command: Fan High
class FanHighCommand : public Command {
private:
    Fan* fan;

public:
    FanHighCommand(Fan* f) : fan(f) {}

    void execute() override {
        fan->high();
    }

    void undo() override {
        fan->off();
    }
};

// Concrete Command: Fan Off
class FanOffCommand : public Command {
private:
    Fan* fan;

public:
    FanOffCommand(Fan* f) : fan(f) {}

    void execute() override {
        fan->off();
    }

    void undo() override {
        fan->high(); // Assume it was on high before
    }
};

// Macro Command: Execute multiple commands
class MacroCommand : public Command {
private:
    std::vector<std::shared_ptr<Command>> commands;

public:
    MacroCommand(const std::vector<std::shared_ptr<Command>>& cmds) : commands(cmds) {}

    void execute() override {
        std::cout << "Executing macro command..." << std::endl;
        for (auto& cmd : commands) {
            cmd->execute();
        }
    }

    void undo() override {
        std::cout << "Undoing macro command..." << std::endl;
        // Undo in reverse order
        for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
            (*it)->undo();
        }
    }
};

// Invoker: Remote Control
class RemoteControl {
private:
    std::vector<std::shared_ptr<Command>> onCommands;
    std::vector<std::shared_ptr<Command>> offCommands;
    std::shared_ptr<Command> undoCommand;

public:
    RemoteControl() {
        // Initialize with 7 slots
        for (int i = 0; i < 7; i++) {
            onCommands.push_back(nullptr);
            offCommands.push_back(nullptr);
        }
    }

    void setCommand(int slot, std::shared_ptr<Command> onCmd, std::shared_ptr<Command> offCmd) {
        onCommands[slot] = onCmd;
        offCommands[slot] = offCmd;
    }

    void onButtonPressed(int slot) {
        if (onCommands[slot]) {
            onCommands[slot]->execute();
            undoCommand = onCommands[slot];
        }
    }

    void offButtonPressed(int slot) {
        if (offCommands[slot]) {
            offCommands[slot]->execute();
            undoCommand = offCommands[slot];
        }
    }

    void undoButtonPressed() {
        if (undoCommand) {
            std::cout << "Undoing last command..." << std::endl;
            undoCommand->undo();
        }
    }
};

int main() {
    // Create receivers
    Light* livingRoomLight = new Light("Living Room");
    Light* kitchenLight = new Light("Kitchen");
    Fan* livingRoomFan = new Fan("Living Room");

    // Create commands
    auto livingLightOn = std::make_shared<LightOnCommand>(livingRoomLight);
    auto livingLightOff = std::make_shared<LightOffCommand>(livingRoomLight);
    auto livingLightDim = std::make_shared<LightDimCommand>(livingRoomLight, 50);

    auto kitchenLightOn = std::make_shared<LightOnCommand>(kitchenLight);
    auto kitchenLightOff = std::make_shared<LightOffCommand>(kitchenLight);

    auto fanHigh = std::make_shared<FanHighCommand>(livingRoomFan);
    auto fanOff = std::make_shared<FanOffCommand>(livingRoomFan);

    // Create macro command for "party mode"
    std::vector<std::shared_ptr<Command>> partyOn = {livingLightOn, kitchenLightOn, fanHigh};
    std::vector<std::shared_ptr<Command>> partyOff = {livingLightOff, kitchenLightOff, fanOff};
    auto partyOnMacro = std::make_shared<MacroCommand>(partyOn);
    auto partyOffMacro = std::make_shared<MacroCommand>(partyOff);

    // Setup remote control
    RemoteControl remote;
    remote.setCommand(0, livingLightOn, livingLightOff);
    remote.setCommand(1, kitchenLightOn, kitchenLightOff);
    remote.setCommand(2, fanHigh, fanOff);
    remote.setCommand(3, partyOnMacro, partyOffMacro);
    remote.setCommand(4, livingLightDim, livingLightOn);

    std::cout << "=== Smart Home Remote Control ===" << std::endl << std::endl;

    // Test individual commands
    std::cout << "Pressing Living Room Light ON (Slot 0):" << std::endl;
    remote.onButtonPressed(0);
    std::cout << std::endl;

    std::cout << "Pressing Kitchen Light ON (Slot 1):" << std::endl;
    remote.onButtonPressed(1);
    std::cout << std::endl;

    std::cout << "Pressing Fan HIGH (Slot 2):" << std::endl;
    remote.onButtonPressed(2);
    std::cout << std::endl;

    std::cout << "Pressing UNDO:" << std::endl;
    remote.undoButtonPressed();
    std::cout << std::endl;

    // Test macro command
    std::cout << "Pressing PARTY MODE ON (Slot 3):" << std::endl;
    remote.onButtonPressed(3);
    std::cout << std::endl;

    std::cout << "Pressing UNDO (will undo entire party mode):" << std::endl;
    remote.undoButtonPressed();
    std::cout << std::endl;

    // Test dim command
    std::cout << "Dimming Living Room Light (Slot 4):" << std::endl;
    remote.onButtonPressed(4);
    std::cout << std::endl;

    // Cleanup
    delete livingRoomLight;
    delete kitchenLight;
    delete livingRoomFan;

    return 0;
}
