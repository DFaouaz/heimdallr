


#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>

#include "ClassFinder.h"


using namespace llvm;
using namespace clang::tooling;


static llvm::cl::OptionCategory gToolCategory("my-tool options");


int main(int argc, const char** argv)
{
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, gToolCategory);
    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),OptionsParser.getSourcePathList());

    using namespace clang::ast_matchers;
    ClassFinder classFinder;
    clang::ast_matchers::MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher enumMatcher = enumDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));
    clang::ast_matchers::DeclarationMatcher classMatcher = cxxRecordDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));
    clang::ast_matchers::DeclarationMatcher propertyMatcher = fieldDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));
    clang::ast_matchers::DeclarationMatcher functionMatcher = functionDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));

    matchFinder.addMatcher(enumMatcher, &classFinder);
    matchFinder.addMatcher(classMatcher, &classFinder);
    matchFinder.addMatcher(propertyMatcher, &classFinder);
    matchFinder.addMatcher(functionMatcher, &classFinder);

    return Tool.run(newFrontendActionFactory(&matchFinder).get());
}
