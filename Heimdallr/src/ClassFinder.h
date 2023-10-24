#pragma once


#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/PrettyPrinter.h"
#include <sstream>


struct TypeVisitor : public clang::RecursiveASTVisitor<TypeVisitor>
{
    explicit TypeVisitor(clang::ASTContext* context)
        : m_Context(context)
    {}

    bool VisitType(clang::Type* type)
    {
        m_Leaf = type;
        return true;
    }

    clang::ASTContext* m_Context;
    clang::Type* m_Leaf;
};


class ReflectedClass
{
public:
    ReflectedClass(const std::string& fileName, const clang::CXXRecordDecl* recordDecl) :
        m_FileName(fileName),
        m_Record(recordDecl), 
        m_Fields(), 
        m_Functions(), 
        m_PrintingPolicy(clang::LangOptions{}) 
    {
        m_PrintingPolicy.Bool = true;
    };

    void AddField(const clang::FieldDecl* fieldDecl) { m_Fields.push_back(fieldDecl); };
    void AddFunction(const clang::FunctionDecl* funcDecl) { m_Functions.push_back(funcDecl); };

    void Generate(clang::ASTContext* ctx, llvm::raw_ostream& os);
    void GenerateClass(clang::ASTContext* ctx, llvm::raw_ostream& os);
    void GenerateClassStorageBody(clang::ASTContext* ctx, llvm::raw_ostream& os);
    void GenerateFunction(clang::ASTContext* ctx, llvm::raw_ostream& os, int index);
    void GenerateFunctionStorageBody(clang::ASTContext* ctx, llvm::raw_ostream& os, int index);
    std::string GetFunctionStorageTemplateParamsString(int index);
    std::string GetFunctionSignatureString(int index);
    std::string GetFunctionLambdaSignatureString(int index);
    std::string GetFunctionLambdaParamsString(int index);
    std::string GetFunctionLambdaParamsPlacementString(int index);

    // utilities
    clang::QualType GetDesugaredType(clang::ASTContext* ctx, clang::QualType t);
    std::string GetAccessSpecifierString(const clang::Decl* decl) const;
    std::string GetQualifiersString(const clang::ValueDecl* valueDecl) const;
    std::string GetTypeSpecializationString(const clang::ValueDecl* valueDecl) const;

private:
    const clang::CXXRecordDecl* m_Record;
    std::string m_FileName;
    std::vector<const clang::FieldDecl*> m_Fields;
    std::vector<const clang::FunctionDecl*> m_Functions;
    clang::PrintingPolicy m_PrintingPolicy;

    friend class ClassFinder;
};

class ReflectedEnum
{
public:
    ReflectedEnum(const std::string& fileName, const clang::EnumDecl* enumDecl) :
        m_FileName(fileName),
        m_EnumDecl(enumDecl),
        m_PrintingPolicy(clang::LangOptions{})
    {
        m_PrintingPolicy.Bool = true;
    };

    void Generate(clang::ASTContext* ctx, llvm::raw_ostream& os);
    void GenerateEnum(clang::ASTContext* ctx, llvm::raw_ostream& os);
private:
    std::string m_FileName;
    const clang::EnumDecl* m_EnumDecl;
    clang::PrintingPolicy m_PrintingPolicy;

};


class ClassFinder : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    ClassFinder();
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

    virtual void onStartOfTranslationUnit() override;
    virtual void onEndOfTranslationUnit() override;

private:
    void FoundEnum(const clang::EnumDecl* enumDecl);
    void FoundRecord(const clang::CXXRecordDecl* record);
    void FoundField(const clang::FieldDecl* field);
    void FoundFunction(const clang::FunctionDecl* function);

private:
    clang::ASTContext* m_Context;
    clang::SourceManager* m_SourceManager;

    std::string m_FileName;
    std::vector<ReflectedClass> m_Classes;
    std::vector<ReflectedEnum> m_Enums;
};

namespace Utils
{
    std::string GetScopeName(const clang::DeclContext* context);
    std::string GetFullScopeName(const clang::DeclContext* context, bool useSeparatorForAnonyms = false, const char* separator = "::");
    std::string GetFullScopeNamePrefix(const clang::DeclContext* context, bool useSeparatorForAnonyms = false, const char* separator = "::");
    std::string RemoveAnonymousTag(const std::string& str);
    std::string GetAttributes(const clang::Decl* decl);
    std::vector<std::string> GetAttributesList(const clang::Decl* decl);
    std::pair<std::string, std::string> GetParsedAttribute(const std::string& attrStr);
}
