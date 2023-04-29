#ifndef casem_hpp_
#define casem_hpp_

#include "cktables.hpp"
#include "ckcontext.hpp"
#include "ckgrptokens.hpp"
#include <string>

namespace casem
{

    void string_start();
    std::string string_end();

    void char_start();
    int char_end();
    bool is_char_too_long();

    void append(char c);
    void append_escaped_special(char c);
    void append_escaped_hex(std::string c);

    int parse_int(std::string text, int base, bool &malformed, bool &too_long);

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    class DeclaratorTypeFragment;

    using DeclaratorTypeFragmentList = std::vector<DeclaratorTypeFragment>;


    class EnumElement
    {
        public:
        cecko::CIName name;
        bool has_value;
        cecko::CKIRConstantIntObs value;
         cecko::loc_t location;

        EnumElement(cecko::CIName name, cecko::CKIRConstantIntObs value, cecko::loc_t location)
            : name(name), has_value(true), value(value), location(location)
        {
        }

        EnumElement(cecko::CIName name,   cecko::loc_t location)
            : name(name), has_value(false), value(0), location(location)
        {
        }
        
        EnumElement()
            : name(""), has_value(false), value(0), location(0)
        {
        }
    };

    using EnumeratorList = std::vector<EnumElement>;


    class Transform
    {
    public:
        virtual cecko::CKTypeRefPack apply(cecko::context *ctx, cecko::CKTypeRefPack &type) = 0;
       
    };

    class PointerTransform : public Transform
    {
    public:
        cecko::CKTypeRefPack apply(cecko::context *ctx, cecko::CKTypeRefPack &type) override;
    };

    class ConstTransform : public Transform
    {
    public:
        cecko::CKTypeRefPack apply(cecko::context *ctx, cecko::CKTypeRefPack &type) override;
    };

    class ArrayTransform : public Transform
    {
    public:
        cecko::CKIRConstantIntObs size;
        cecko::CKTypeRefPack apply(cecko::context *ctx, cecko::CKTypeRefPack &type) override;
        ArrayTransform();
        ArrayTransform(cecko::CKIRConstantIntObs size) : size(size) {}
    };

    class FunctionTransform : public Transform
    {

    public:
        DeclaratorTypeFragmentList args;
        cecko::CKTypeRefPack apply(cecko::context *ctx, cecko::CKTypeRefPack &type) override;
        FunctionTransform();
        FunctionTransform(const DeclaratorTypeFragmentList &args);
    };

    enum TypeQualifier
    {
        CONSTANT
    };

    enum StorageSpecifier
    {
        TYPE_DEF
    };

    class DeclaratorTypeFragment
    {
    private:
        bool _has_type;
        cecko::CKTypeRefPack _type;
        cecko::context *ctx;

    public:
        cecko::CIName name;


        DeclaratorTypeFragmentList args;
        bool has_args;
        
        
        bool is_type_def;
        std::vector<std::shared_ptr<Transform>> transforms;
        cecko::loc_t location;

        DeclaratorTypeFragment();
        DeclaratorTypeFragment(cecko::context *ctx);
        DeclaratorTypeFragment(cecko::context *ctx, cecko::CKTypeRefPack type);
        DeclaratorTypeFragment(cecko::context *ctx, StorageSpecifier storage_specifier);
        DeclaratorTypeFragment(cecko::context *ctx, TypeQualifier type_qualifier);

        void add_transform(std::shared_ptr<Transform> transform);

        cecko::CKTypeRefPack get_type() const;
        bool has_type() const;
        void set_type(cecko::CKTypeRefPack type);
        bool has_name() const;

        void add(const DeclaratorTypeFragment &other);
        void add_qualifier(TypeQualifier type_qualifier);
        void add_storage_specifier(StorageSpecifier add_storage_specifier);
        void pointer();
        void function(const DeclaratorTypeFragmentList &args);
        void array( cecko::CKIRConstantIntObs size);

        std::string to_string() const;
    };

    cecko::CKFunctionSafeObs _declare_function(cecko::context *ctx, const DeclaratorTypeFragment &declarator, cecko::loc_t location);
    void process_declarations(cecko::context *ctx, const DeclaratorTypeFragment &spec, DeclaratorTypeFragmentList &init_list, cecko::loc_t location);
    cecko::CKTypeSafeObs get_etype_type(cecko::context *ctx, cecko::gt_etype etype);
    cecko::CKTypeRefPack pack_type(cecko::CKTypeSafeObs tp);
    cecko::CKTypeRefPack pack_type_qualifier(TypeQualifier type_qualifier);
    std::string type_to_string(const cecko::CKTypeRefPack &pack);
    void _enter_function(cecko::context *ctx, DeclaratorTypeFragment& declarator, cecko::loc_t location);
    void process_enum(cecko::context *ctx, cecko::CKEnumTypeObs type, const EnumeratorList &spec);
    void process_struct(cecko::context *ctx, cecko::CKStructTypeSafeObs type, DeclaratorTypeFragmentList items);
    void debug_print(cecko::context *ctx, const std::string &s);
}

#endif
