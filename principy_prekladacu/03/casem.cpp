#include "casem.hpp"
#include <sstream>
#include <string>
#include <limits>
#include <vector>

namespace casem
{

    bool character = false;
    std::stringstream ss;
    int char_value;
    int char_len;

    void string_start()
    {
        character = false;
        ss.str("");
    }

    void string_append(char c)
    {
        ss << c;
    }

    std::string string_end()
    {
        std::string s = ss.str();
        return s;
    }

    void char_start()
    {
        character = true;
        char_value = 0;
        char_len = 0;
    }

    void char_append(char c)
    {
        char_value <<= 8;
        char_value |= c;
        char_len++;
    }

    bool is_char_too_long()
    {
        return char_len > 4;
    }

    int char_end()
    {
        return char_value;
    }

    int get_digit_value(char c, bool hex, bool &failed)
    {
        failed = false;
        if ('0' <= c && c <= '9')
            return c - '0';
        c = std::tolower(c);
        if ('a' <= c && c <= 'f' && hex)
            return c - 'a' + 10;
        failed = true;
        return 0;
    }

    int parse_int(std::string text, int base, bool &malformed, bool &too_long)
    {
        int value = 0;
        int add;

        too_long = false;

        for (char c : text)
        {
            add = get_digit_value(c, base == 16, malformed);
            if (malformed)
                break;

            if (value > INT_MAX / base)
                too_long = true;
            value *= base;

            if (value > INT_MAX - add)
                too_long = true;
            value += add;
        }

        return value;
    }

    int parse_int(std::string text, int base)
    {
        int value = 0;
        int add;
        bool failed;

        for (char c : text)
        {
            add = get_digit_value(c, base == 16, failed);
            if (failed)
                break;
            value *= base;
            value += add;
        }
        return value;
    }

    void append(char c)
    {
        if (character)
            char_append(c);
        else
            string_append(c);
    }

    void append_escaped_special(char c)
    {
        switch (c)
        {
        case 'a':
            append('\a');
            break;
        case 'b':
            append('\b');
            break;
        case 'f':
            append('\f');
            break;
        case 'n':
            append('\n');
            break;
        case 'r':
            append('\r');
            break;
        case 't':
            append('\t');
            break;
        case 'v':
            append('\v');
            break;
        default:
            break;
        }
    }
    void append_escaped_hex(std::string c)
    {
        append(parse_int(c, 16));
    }

    /////////////////////////////////
    // SEMANTIC ANALYSIS
    /////////////////////////////////

    inline const bool debug = false;

    DeclaratorTypeFragment::DeclaratorTypeFragment()
    {
        this->_has_type = false;
        this->is_type_def = false;
        this->name = "";
    }
    DeclaratorTypeFragment::DeclaratorTypeFragment(cecko::context *ctx) : DeclaratorTypeFragment::DeclaratorTypeFragment()
    {
        this->ctx = ctx;
    }
    DeclaratorTypeFragment::DeclaratorTypeFragment(cecko::context *ctx, cecko::CKTypeRefPack tp) : DeclaratorTypeFragment::DeclaratorTypeFragment(ctx)
    {
        this->set_type(tp);
    }
    DeclaratorTypeFragment::DeclaratorTypeFragment(cecko::context *ctx, StorageSpecifier storage_specifier) : DeclaratorTypeFragment::DeclaratorTypeFragment(ctx)
    {
        this->add_storage_specifier(storage_specifier);
    }
    DeclaratorTypeFragment::DeclaratorTypeFragment(cecko::context *ctx, TypeQualifier type_qualifier) : DeclaratorTypeFragment::DeclaratorTypeFragment(ctx)
    {
        this->add_qualifier(type_qualifier);
    }

    void DeclaratorTypeFragment::set_type(cecko::CKTypeRefPack type)
    {
        while (this->transforms.size() > 0)
        {
            type = this->transforms.back()->apply(this->ctx, type);
            this->transforms.pop_back();
        }

        debug_print(ctx, "Constructed " + type_to_string(type));

        this->_type = type;
        this->_has_type = true;
    }
    cecko::CKTypeRefPack DeclaratorTypeFragment::get_type() const
    {
        return this->_type;
    }

    bool DeclaratorTypeFragment::has_type() const
    {
        return this->_has_type;
    }

    bool DeclaratorTypeFragment::has_name() const
    {
        return this->name.length() > 0;
    }

    void DeclaratorTypeFragment::add(const DeclaratorTypeFragment &other)
    {

        debug_print(ctx, "Adding " + other.to_string() + " to " + this->to_string());

        for (auto transform : other.transforms)
        {
            this->add_transform(transform);
        }

        if (other.has_type())
            this->set_type(other.get_type());

        if (other.is_type_def)
            this->is_type_def = true;

        debug_print(ctx,"Resulting in " + this->to_string());
    }

    std::string DeclaratorTypeFragment::to_string() const
    {
        std::string s = "[";

        if (this->has_name())
            s += this->name + " ";

        if (this->has_type())
            s += type_to_string(this->_type) + " ";

        s += std::to_string(this->transforms.size()) + " transforms";
        s += "]";
        return s;
    }

    void DeclaratorTypeFragment::add_transform(std::shared_ptr<Transform> transform)
    {
        if (this->has_type())
            this->_type = transform->apply(this->ctx, this->_type);
        else
            this->transforms.push_back(transform);
    }
    

    void DeclaratorTypeFragment::add_qualifier(TypeQualifier type_qualifier)
    {
        if (type_qualifier == CONSTANT)
        {
            add_transform(std::make_shared<ConstTransform>(ConstTransform()));
        }
    }

    void DeclaratorTypeFragment::add_storage_specifier(StorageSpecifier storage_specifier)
    {
        if (storage_specifier == TYPE_DEF)
            this->is_type_def = true;
    }

    void DeclaratorTypeFragment::pointer()
    {
        debug_print(ctx, "Pointer");

        this->has_args = false;

        add_transform(std::make_shared<PointerTransform>(PointerTransform()));
    }

    void DeclaratorTypeFragment::function(const DeclaratorTypeFragmentList &args)
    {
        debug_print(ctx, "Function");

        auto trf = std::make_shared<FunctionTransform>(FunctionTransform(args));

        if (!has_args)
        {
            this->args = trf->args;
            this->has_args = true;
        }

        add_transform(trf);
    }

     void DeclaratorTypeFragment::array(cecko::CKIRConstantIntObs size)
    {
        debug_print(ctx, "Array");

        auto trf = std::make_shared<ArrayTransform>(ArrayTransform(size));

        add_transform(trf);
    }

    FunctionTransform::FunctionTransform() = default;
    FunctionTransform::FunctionTransform(const DeclaratorTypeFragmentList &args)
    {
        if (!(args.size() == 1 && args.at(0).get_type().type->is_void()))
            this->args = args;
    }

    cecko::CKTypeRefPack ConstTransform::apply(cecko::context *ctx, cecko::CKTypeRefPack &type)
    {

        debug_print(ctx,"Making const from " + type_to_string(type));

        type.is_const = true;
        return type;
    }

    cecko::CKTypeRefPack ArrayTransform::apply(cecko::context *ctx, cecko::CKTypeRefPack &type)
    {

        debug_print(ctx,"Making array from " + type_to_string(type));

        return pack_type(ctx->get_array_type(type.type, this->size));
    }

    cecko::CKTypeRefPack PointerTransform::apply(cecko::context *ctx, cecko::CKTypeRefPack &type)
    {
        debug_print(ctx, "Making pointer from " + type_to_string(type));
        return pack_type(ctx->get_pointer_type(type));
    }

    cecko::CKTypeRefPack FunctionTransform::apply(cecko::context *ctx, cecko::CKTypeRefPack &type)
    {
        debug_print(ctx, "Making function from " + type_to_string(type) + "with args " + std::to_string(this->args.size()));
        std::vector<const cecko::CIAbstractType *> types;
        for (auto decl : this->args)
        {
            types.push_back(decl.get_type().type);
        }
        return pack_type(ctx->get_function_type(type.type, types));
    }

    cecko::CKFunctionSafeObs _declare_function(cecko::context *ctx, const DeclaratorTypeFragment &declarator, cecko::loc_t location)
    {
        return ctx->declare_function(declarator.name, declarator.get_type().type, location);
    }

    void _enter_function(cecko::context *ctx, DeclaratorTypeFragment &declarator, cecko::loc_t location)
    {
        cecko::CKFunctionFormalPackArray args;

        for (auto arg : declarator.args)
        {
            args.push_back(cecko::CKFunctionFormalPack(arg.name, arg.get_type().is_const, location));
        }

        ctx->enter_function(_declare_function(ctx, declarator, location), args, location);
    }

    void process_declarations(cecko::context *ctx, const DeclaratorTypeFragment &spec, DeclaratorTypeFragmentList &init_list, cecko::loc_t location)
    {
        for (DeclaratorTypeFragment decl : init_list)
        {
            decl.add(spec);
            if (decl.get_type().type->is_function())
                _declare_function(ctx, decl, location);
            else if (decl.is_type_def)
                ctx->define_typedef(decl.name, decl.get_type(), decl.location);
            else
                ctx->define_var(decl.name, decl.get_type(), decl.location);

            debug_print(ctx, "-- declaring");
        }
    }

    cecko::CKTypeSafeObs get_etype_type(cecko::context *ctx, cecko::gt_etype etype)
    {
        switch (etype)
        {
        case cecko::gt_etype::BOOL:
            return ctx->get_bool_type();
        case cecko::gt_etype::CHAR:
            return ctx->get_char_type();
        case cecko::gt_etype::INT:
            return ctx->get_int_type();
        }
        return ctx->get_int_type();
    }

    cecko::CKTypeRefPack pack_type(cecko::CKTypeSafeObs tp)
    {
        cecko::CKTypeRefPack pack;
        pack.type = tp;
        return pack;
    }

    cecko::CKTypeRefPack pack_type_qualifier(TypeQualifier type_qualifier)
    {
        cecko::CKTypeRefPack pack;

        if (type_qualifier == CONSTANT)
        {
            pack.is_const = true;
        }

        return pack;
    }

    std::string type_to_string(const cecko::CKTypeRefPack &pack)
    {

        std::string s = "(";
        if (pack.is_const)
            s += " const";

        if (pack.type == nullptr)
            return s + ")";

        if (pack.type->is_void())
            s += "void";
        if (pack.type->is_bool())
            s += "bool";
        if (pack.type->is_char())
            s += "char";
        if (pack.type->is_int())
            s += "int";
        if (pack.type->is_enum())
            s += "enum";
        if (pack.type->is_array())
            s += "array";
        if (pack.type->is_function())
        {
            s += "fn ";

            s += type_to_string(pack_type(pack.type->get_function_return_type()));
            s += " (";

            int count = pack.type->get_function_arg_count();
            for (int i = 0; i < count; i++)
            {
                s += type_to_string(pack_type(pack.type->get_function_arg_type(i)));
                s += ", ";
            }
            if (count == 0)
                s += "void.";

            s += ") ";
        }
        if (pack.type->is_pointer())
        {
            s += "pointer to ";
            s += type_to_string(pack.type->get_pointer_points_to());
        }
        if (pack.type->is_struct())
            s += " struct";
        s += ")";
        return s;
    }

    void process_enum(cecko::context *ctx, cecko::CKEnumTypeObs type, const EnumeratorList &list)
    {

        cecko::CKConstantObsVector items;

        int value = -1;
        for (auto enumerator : list)
        {

            if (enumerator.has_value)
            {
                items.push_back(ctx->define_constant(enumerator.name, enumerator.value, enumerator.location));
                value = enumerator.value->getZExtValue();
            }
            else
            {
                value++;
                items.push_back(ctx->define_constant(enumerator.name, ctx->get_int32_constant(value), enumerator.location));
            }
        }

        ctx->define_enum_type_close(type, items);
    }

    void process_struct(cecko::context *ctx, cecko::CKStructTypeSafeObs type, DeclaratorTypeFragmentList items)
    {
        debug_print(ctx,"Making struct with items " + std::to_string(items.size()));

        cecko::CKStructItemArray struct_items;

        for (auto item : items)
        {
            debug_print(ctx, "Making struct with items " + type_to_string(item.get_type()));

            struct_items.push_back(cecko::CKStructItem(item.get_type(),item.name, item.location));
        }

        ctx->define_struct_type_close(type, struct_items);
    }


    void debug_print(cecko::context *ctx, const std::string &s)
    {
        if (debug)
            ctx->message(cecko::errors::SYNTAX, 0, s);
    }

}
