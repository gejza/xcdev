/*
   File name:  vm.cc
   Date:       2010/03/30 15:16
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>
#include <xc/text.h>


#include "xc/template/vm.h"
#include "xc/template/page.h"
#include "xc/template/bytecode.h"
#include "xc/template/env.h"

xc::templ::vm_t::vm_t()
{
}

xc::templ::vm_t::~vm_t()
{
}

void xc::templ::vm_t::process(const xc::templ::page_t& page,
        const xc::templ::frag_t& data, uint32_t ci)
{
    LOG_INFO(1, "Generating page '%s'", page.name());
    const bytecode_t* bc = page.code(0);
    while (1) {
        const ins_t& i = *reinterpret_cast<const ins_t*>(&bc[ci]);
        XC_TRACE("Running instruction %s", i.name());
        if (i.code == TI_END)
            break;

        switch (i.code) {
        case TI_RAW:
            {
                const ins_data_t& data = reinterpret_cast<const ins_data_t&>(i);
                ci += sizeof(ins_data_t);
                this->vm_output(page.data(data.data.ptr), data.data.size);
                break;
            }
        case TI_PAGE:
        case TI_PAGE_LIST:
            {
                const ins_page_t& page = reinterpret_cast<const ins_page_t&>(i);
                ci += sizeof(ins_page_t);
                xc::string name((const char*)&bc[ci], (size_t)page.name_size);
                LOG_INFO(2, "Run page %s", name.c_str());
                ci += page.name_size;
                this->vm_debug(xc::format("begin '%s'", name.c_str()));
                this->vm_page(name);
                this->vm_debug(xc::format("end '%s'", name.c_str()));
                //this->process(_env.get_page(name.c_str()));
                break;
            }
        case TI_DICT:
            {
                const ins_dict_t& page = reinterpret_cast<const ins_dict_t&>(i);
                ci += sizeof(ins_dict_t);
                xc::string name((const char*)&bc[ci], (size_t)page.name_size);
                ci += page.name_size;
                this->vm_dict(name);
                break;
            }
        case TI_VALUE:
            {
                const ins_value_t& page = reinterpret_cast<const ins_value_t&>(i);
                ci += sizeof(ins_value_t);
                xc::string name((const char*)&bc[ci], (size_t)page.name_size);
                ci += page.name_size;
                //this->vm_value(name);
                xc::string val(data.get_value(name).str);
                this->vm_output(val.data(), val.size());
                break;
            }
        case TI_DEBUG:
            {
                const ins_data_t& data = reinterpret_cast<const ins_data_t&>(i);
                ci += sizeof(ins_data_t);
                this->vm_debug(page.data(data.data.ptr), data.data.size);
                break;
            }
        case TI_FRAG:
            {
                const ins_frag_t& frag = reinterpret_cast<const ins_frag_t&>(i);
                ci += sizeof(ins_frag_t);
                xc::string name((const char*)&bc[ci], (size_t)frag.name_size);
                LOG_INFO(2, "Run fragment %s at %u", name.c_str(), frag.addr);
                ci += frag.name_size;
                this->process(page, data.get_frag(name), frag.addr);
                break;
            }
        default:
            ERROR(xc::error_t, "Not implemented instruction %u(%s)", i.code, i.name());
            break;
        };
        //
    }
}

void xc::templ::vm_t::flush()
{
}

