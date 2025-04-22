// codegen.cpp
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

llvm::LLVMContext context;
llvm::Module module("NoiriaModule", context);
llvm::IRBuilder<> builder(context);

void generateCode(ASTNode* node) {
    if (auto assign = dynamic_cast<AssignmentNode*>(node)) {
        // Generate LLVM IR for assignment
    } else if (auto funcCall = dynamic_cast<FunctionCallNode*>(node)) {
        // Generate LLVM IR for function call
    }
    // Handle other node types...
}
