#include <iostream>
#include <string>
#include <memory>
#include <stack>

// Receiver: Document
class Document {
private:
    std::string text;

public:
    void insertText(const std::string& txt) {
        text += txt;
        std::cout << "Text inserted: \"" << txt << "\"" << std::endl;
        std::cout << "Current document: \"" << text << "\"" << std::endl;
    }

    void deleteText(int length) {
        if (length > text.length()) {
            length = text.length();
        }
        text = text.substr(0, text.length() - length);
        std::cout << "Deleted " << length << " characters" << std::endl;
        std::cout << "Current document: \"" << text << "\"" << std::endl;
    }

    std::string getText() const {
        return text;
    }
};

// Command Interface
class EditorCommand {
public:
    virtual ~EditorCommand() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Concrete Command: Insert
class InsertCommand : public EditorCommand {
private:
    Document* doc;
    std::string textToInsert;

public:
    InsertCommand(Document* d, const std::string& txt)
        : doc(d), textToInsert(txt) {}

    void execute() override {
        doc->insertText(textToInsert);
    }

    void undo() override {
        doc->deleteText(textToInsert.length());
    }
};

// Concrete Command: Delete
class DeleteCommand : public EditorCommand {
private:
    Document* doc;
    int length;
    std::string deletedText;

public:
    DeleteCommand(Document* d, int len)
        : doc(d), length(len), deletedText("") {}

    void execute() override {
        std::string currentText = doc->getText();
        int actualLength = (length > currentText.length()) ? currentText.length() : length;
        deletedText = currentText.substr(currentText.length() - actualLength);
        doc->deleteText(length);
    }

    void undo() override {
        doc->insertText(deletedText);
    }
};

// Invoker: Editor
class TextEditor {
private:
    std::stack<std::shared_ptr<EditorCommand>> undoStack;
    std::stack<std::shared_ptr<EditorCommand>> redoStack;

public:
    void executeCommand(std::shared_ptr<EditorCommand> cmd) {
        cmd->execute();
        undoStack.push(cmd);
        // Clear redo stack when new command is executed
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }

    void undo() {
        if (!undoStack.empty()) {
            std::cout << "\n--- UNDO ---" << std::endl;
            auto cmd = undoStack.top();
            undoStack.pop();
            cmd->undo();
            redoStack.push(cmd);
        } else {
            std::cout << "Nothing to undo!" << std::endl;
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            std::cout << "\n--- REDO ---" << std::endl;
            auto cmd = redoStack.top();
            redoStack.pop();
            cmd->execute();
            undoStack.push(cmd);
        } else {
            std::cout << "Nothing to redo!" << std::endl;
        }
    }
};

int main() {
    Document doc;
    TextEditor editor;

    std::cout << "=== Text Editor with Command Pattern ===" << std::endl << std::endl;

    // Execute commands
    auto cmd1 = std::make_shared<InsertCommand>(&doc, "Hello ");
    editor.executeCommand(cmd1);
    std::cout << std::endl;

    auto cmd2 = std::make_shared<InsertCommand>(&doc, "World!");
    editor.executeCommand(cmd2);
    std::cout << std::endl;

    auto cmd3 = std::make_shared<InsertCommand>(&doc, " How are you?");
    editor.executeCommand(cmd3);
    std::cout << std::endl;

    // Undo operations
    editor.undo();
    std::cout << std::endl;

    editor.undo();
    std::cout << std::endl;

    // Redo operations
    editor.redo();
    std::cout << std::endl;

    // New command after redo
    auto cmd4 = std::make_shared<DeleteCommand>(&doc, 6);
    editor.executeCommand(cmd4);
    std::cout << std::endl;

    // Undo delete
    editor.undo();

    return 0;
}
