/*
   File name:  compile.cc
   Date:       2010/04/12 13:41
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <memory>
#include <list>

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>
#include <xc/pool.h>

#include "xc/template/compile.h"
#include "xc/template/fs.h"
#include "xc/template/scanner.h"
#include "xc/template/page.h"
#include "xc/template/preprocess.h"

namespace {

    using namespace xc::templ;

    class bc_t
    {
    public:
        bc_t(xc::pool_t& pool) : _pool(pool), _total(0), _size(0) {
        }

        ~bc_t() {}
        void add_simple(inscode_t code) {
            ins_t& ins = create<ins_t>();
            ins.code = code;
        }

        void add_data(inscode_t code, uint32_t ptr, size_t size) {
            ins_data_t& ins = create<ins_data_t>();
            ins.code = code;
            ins.data.ptr = ptr;
            ins.data.size = size;
        }

        void add_page(inscode_t code, const xc::string& name) {
            ins_page_t& ins = create<ins_page_t>(name.data(), name.size());
            ins.code = code;
            ins.name_size = name.size();
        }

        void add_dict(const xc::string& name) {
            ins_dict_t& ins = create<ins_dict_t>(name.data(), name.size());
            ins.code = TI_DICT;
            ins.name_size = name.size();
        }

        void add_value(const xc::string& name) {
            ins_value_t& ins = create<ins_value_t>(name.data(), name.size());
            ins.code = TI_VALUE;
            ins.name_size = name.size();
        }

        void add_fragment(const xc::string& name, bc_t* frag) {
            ins_frag_t& ins = create<ins_frag_t>(name.data(), name.size());
            ins.code = TI_FRAG;
            ins.name_size = name.size();
            ins.addr = 0;
            ins.def = 0;
            _list.back().code = frag;

            _total += frag->total_size();
        }

        size_t size() const {
            return _size;
        }

        size_t total_size() const {
            return _total;
        }

        uint32_t link(bytecode_t* bc, uint32_t& size) const;
    protected:

        template<typename T>
        T& create(const char* data = 0, size_t size = 0) {
            T* t = new (this->_pool.alloc(sizeof(T) + size)) T();
            if (data)
                ::memcpy(&t[1], data, size);
            _list.push_back(_ins_info_t(t, sizeof(T) + size));
            _size += sizeof(T) + size;
            _total += sizeof(T) + size;
            return *t;
        }


    private:
        bc_t(const bc_t&);
        bc_t& operator=(const bc_t&);

        struct _ins_info_t {
            ins_t* ins;
            size_t size;
            bc_t* code;

            _ins_info_t(ins_t* _ins, size_t _size) : ins(_ins), size(_size), code(0) {}

            ~_ins_info_t() {
                if (code)
                    delete code;
            }

            void write(bytecode_t* _bc, uint32_t& _ptr, uint32_t& _size) const {
                ::memcpy(_bc + _ptr, ins, size);
                switch (ins->code) {
                case TI_FRAG:
                    {
                        ins_frag_t& i = *reinterpret_cast<ins_frag_t*>(_bc + _ptr);
                        if (code)
                            i.addr = code->link(_bc, _size);
                    }
                    break;
                };
                _ptr += size;
            }
        };

        typedef std::list<_ins_info_t> _ins_list_t;

        xc::pool_t& _pool;
        size_t _total;
        size_t _size;
        _ins_list_t _list;
    };


    class obj_t
    {
    public:
        obj_t() : _pool(), _main(_pool) {}

        void compile(scanner_t& in) {
            compile(in, _main);
        }

        xc::templ::page_t* link_page(const xc::string& name) const;
    protected:
        uint32_t add_data(const char* str, size_t size) {
            uint32_t ret = _dbuff.size();
            _dbuff.append(str, size);
            _dbuff.push_back('\0');
            return ret;
        }

        void compile(scanner_t& in, bc_t& out, const char* fragname = NULL);
        bc_t* compile_fragment(scanner_t& in, const char* fragname);

        unsigned char* make_code() const;
        char* make_data() const;
    private:
        xc::pool_t _pool;
        size_t _total;
        bc_t _main;
        xc::string _dbuff;
    };

    class mkpage_t : public xc::templ::page_t
    {
    public:
        mkpage_t(const xc::string& name, bytecode_t* code, char* data)
            : page_t(name), _code(code), _data(data) {}

        virtual ~mkpage_t() {
            if (_code)
                delete [] _code;
            if (_data)
                delete [] _data;
        }

        virtual unsigned char* code(uint32_t addr = 0) const {
            return _code + addr;
        }

        virtual const char* data(unsigned int ptr) const {
            return _data + ptr;
        }

    private:
        mkpage_t(const mkpage_t&);
        mkpage_t& operator=(const mkpage_t&);
    protected:
        bytecode_t* _code;
        char* _data;
    };

    uint32_t bc_t::link(bytecode_t* bc, uint32_t& size) const {
        uint32_t ret = size;
        uint32_t ptr = size;
        size += this->size();
        for (_ins_list_t::const_iterator i = _list.begin(); i != _list.end(); ++i) {
            i->write(bc, ptr, size);
        }
        return ret;
    }

    void obj_t::compile(scanner_t& in, bc_t& out, const char* fragname)
    {
        while (scanner_t::term_t t = in.read()) {
            XC_DBG("Instruction %s", t.name());

            switch (t.type) {
            case TI_DEBUG:
            case TI_RAW:
                out.add_data(t.type, this->add_data(t.text, t.length), t.length);
                break;
            case TI_PAGE:
            case TI_PAGE_LIST:
                out.add_page(t.type, t.sub(2,2));
                break;
            case TI_DICT:
                out.add_dict(t.sub(2,1));
                break;
            case TI_VALUE:
                out.add_value(t.sub(2,1));
                break;
            case TI_FRAG_BEGIN:
                {
                    xc::string name = t.sub(1,1);
                    bc_t* frag = compile_fragment(in, name.c_str());
                    LOG_INFO(1, "Compile fragment '%s' with size %zd", name.c_str(), frag->total_size());
                    out.add_fragment(name, frag);
                }
                break;
            case TI_FRAG_END:
                {
                    xc::string name = t.sub(1,1);
                    if (name != fragname)
                        ERROR(xc::error_t, "Cross fragment end '%s' over '%s'",
                            name.c_str(), fragname ? fragname:"-");
                    out.add_simple(TI_END);
                    return;
                }
                break;

            default:
                LOG_WARN(3, "Ignore template token %s. Not implemented", t.name());
            };
        };
        out.add_simple(TI_END);
    }
    
    bc_t* obj_t::compile_fragment(scanner_t& in, const char* fragname)
    {
        bc_t* frag = new bc_t(_pool);
        compile(in, *frag, fragname);
        return frag;
    }

    xc::templ::page_t* obj_t::link_page(const xc::string& name) const
    {
        std::auto_ptr<bytecode_t> code(make_code());
        std::auto_ptr<char> data(make_data());
        return new mkpage_t(name, code.release(), data.release());
    }

    bytecode_t* obj_t::make_code() const
    {
        size_t cs = _main.total_size();
        bytecode_t* cp = new bytecode_t[cs];
        uint32_t s = 0;
        _main.link(cp, s);
        LOG_INFO(1, "Compile %u bytes of code.", s);
        return cp;
    }

    char* obj_t::make_data() const
    {
        // malloc data, malloc code
        size_t ds = _dbuff.size();
        char* dp = new char[ds ? ds:1];
        _dbuff.copy(dp, ds);
        return dp;
    }
}

xc::templ::compiler_t::compiler_t(files_t& files)
    : _files(files)
{
}

xc::templ::compiler_t::~compiler_t()
{
}

xc::templ::page_t* xc::templ::compiler_t::compile(const char* name)
{
    preproc_t in(_files);
    in.set(name);
    obj_t obj;
    obj.compile(in);

    /*mkpage_t* page = new mkpage_t(name);

    ins_t e;
    e.code = TI_END;
    page->add(e);
    page->finish();
    return page;*/
    return obj.link_page(name);
}



