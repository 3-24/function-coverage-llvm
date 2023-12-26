#define DEBUG_TYPE "PrintFunc"

#include <iostream>
#include <map>

#include "llvm/ADT/Statistic.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

/* PrintFunc.cpp
 *
 * Print at function entry point and return point
 * */

using namespace llvm;
using namespace std;

class PrintFunc : public ModulePass {
 public:
  static char ID;  // Pass identification, replacement for typeid

  // Fill out.

  PrintFunc() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    LLVMContext &ctx = M.getContext();
    Module *Mod = &M;

    Type *voidTy = Type::getVoidTy(ctx);
    Type *int8ptrTy = Type::getInt8PtrTy(ctx);
    Type *boolTy = Type::getInt1Ty(ctx);

    FunctionCallee init_out =
        M.getOrInsertFunction("init_out_file", voidTy);
    
    FunctionCallee print_out =
        M.getOrInsertFunction("print_func", voidTy, boolTy, int8ptrTy, int8ptrTy);

    IRBuilder<> IRB(M.getContext());

    for (auto &F : M) {
      if (F.size() == 0) {
        continue;
      }

      const string function_name = F.getName().str();
      const string demangled_name = demangle(function_name);

      IRB.SetInsertPoint(F.getEntryBlock().getFirstNonPHIOrDbgOrLifetime());

      Constant *demangled_name_global =
          gen_new_string_constant(demangled_name, &IRB, Mod);

      // If the function is main, call init_out_file
      if (function_name == "main") {
        IRB.CreateCall(init_out);
      }

      // Find the filename
      auto subp = F.getSubprogram();
      if (subp == nullptr) {
        continue;
      }

      const string dirname = subp->getDirectory().str();
      string filename = subp->getFilename().str();

      if (dirname != ""){
        filename = dirname + "/" + filename;
      }

      if (filename.find("/usr/bin") != string::npos) {
        continue;
      }

      llvm::Constant *filename_const = gen_new_string_constant(filename, &IRB, Mod);

      
      llvm::Constant *false_const = llvm::ConstantInt::get(boolTy, 0, false);
      llvm::Constant *true_const = llvm::ConstantInt::get(boolTy, 1, false);


      IRB.CreateCall(print_out, {true_const, filename_const, demangled_name_global});


      for (auto &BB : F) {
        for (auto &IN : BB) {
          if (isa<ReturnInst>(IN)) {
            IRB.SetInsertPoint(&IN);
            IRB.CreateCall(print_out, {false_const, filename_const, demangled_name_global});
          }
        }
      }
    }

    return true;
  }

  map<string, Constant *> new_string_globals;

  Constant *gen_new_string_constant(std::string name, IRBuilder<> *IRB,
                                    Module *Mod) {
    auto search = new_string_globals.find(name);

    if (search == new_string_globals.end()) {
      Constant *new_global = IRB->CreateGlobalStringPtr(name, "", 0, Mod);
      new_string_globals.insert(std::make_pair(name, new_global));
      return new_global;
    }

    return search->second;
  }
};

char PrintFunc::ID = 0;

static RegisterPass<PrintFunc> X("PrintFunc", "PrintFunc Pass", false, false);

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new PrintFunc());
}

static RegisterStandardPasses RegisterPassOpt(
    PassManagerBuilder::EP_ModuleOptimizerEarly, registerPass);

static RegisterStandardPasses RegisterPassO0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerPass);