#pragma once

namespace attic {
namespace compiler {

// Forward declarations
class Lexer;
class Parser;
class ASTNode;
class CodeGenerator;

// DSL Compiler interface
class Compiler {
public:
    Compiler();
    ~Compiler();
    
    bool initialize();
    void shutdown();
    
    // Compile .physics file to target code
    bool compile_file(const char* input_path, const char* output_path);
    
    // Compile from string
    bool compile_string(const char* source, const char* output_path);
    
private:
    class Impl;
    Impl* impl_;
};

} // namespace compiler
} // namespace attic
