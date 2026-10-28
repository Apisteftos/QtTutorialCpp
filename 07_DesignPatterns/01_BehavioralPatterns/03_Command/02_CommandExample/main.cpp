#include <memory>
#include <vector>
#include <string>
#include <print>

// Receiver: Light
class Light {
private:
    std::string location;
    int brightness;

public:
    Light(const std::string& loc) : location(loc), brightness(0) {}

    void on() {
        brightness = 100;
        std::println("{} light is ON (100%)", location);
    }

    void off() {
        brightness = 0;
        std::println("{} light is OFF", location);
    }

    void dim(int level) {
        brightness = level;
        std::println("{} light dimmed to {}%", location, level);
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
        std::println("{} fan speed is HIGH", location);
    }

    void medium() {
        speed = 2;
        std::println("{} fan speed is MEDIUM", location);
    }

    void low() {
        speed = 1;
        std::println("{} fan speed is LOW", location);
    }

    void off() {
        speed = 0;
        std::println("{} fan is OFF", location);
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
        std::println("Executing macro command...");
        for (auto& cmd : commands) {
            cmd->execute();
        }
    }

    void undo() override {
        std::println("Undoing macro command...");
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
            std::println("Undoing last command...");
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

    std::println("=== Smart Home Remote Control ===");
    std::println("");

    // Test individual commands
    std::println("Pressing Living Room Light ON (Slot 0):");
    remote.onButtonPressed(0);
    std::println("");

    std::println("Pressing Kitchen Light ON (Slot 1):");
    remote.onButtonPressed(1);
    std::println("");

    std::println("Pressing Fan HIGH (Slot 2):");
    remote.onButtonPressed(2);
    std::println("");

    std::println("Pressing UNDO:");
    remote.undoButtonPressed();
    std::println("");

    // Test macro command
    std::println("Pressing PARTY MODE ON (Slot 3):");
    remote.onButtonPressed(3);
    std::println("");

    std::println("Pressing UNDO (will undo entire party mode):");
    remote.undoButtonPressed();
    std::println("");

    // Test dim command
    std::println("Dimming Living Room Light (Slot 4):");
    remote.onButtonPressed(4);
    std::println("");

    // Cleanup
    delete livingRoomLight;
    delete kitchenLight;
    delete livingRoomFan;

    return 0;
}
