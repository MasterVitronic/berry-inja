/*

  be-inja.cpp - Berry bindings to inja template engine

  Copyright (c) 2021, Díaz Devera Víctor <mastervitronic@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/***
 * Berry bindings to inja template engine
 *
 * This documentation is partial, and doesn't cover all functionality yet.
 * @module inja
 * @author Díaz Devera Víctor (Máster Vitronic) <mastervitronic@gmail.com>
 */


#define BERRY_MODULE
#include <berry/berry.h>
#include "inja.hpp"
#include <unistd.h>
#define VERSION "1.0.0"

#if defined(_WIN32)
  #define MODULE_API        __declspec(dllexport)
#else
  #define MODULE_API        extern
#endif


using namespace inja;
using json = nlohmann::json;
Environment env;


/***
 * berry-inja binding version.
 * @function version
 * @treturn string version string "major.minor.revision"
 */
static int m_version(bvm *vm) {
	const char * version = VERSION;
	be_pushstring(vm,  version);
	be_return(vm);
}

/***
 * Read a template file
 * @function parse_template
 * @tparam string file the file template
 * @tparam string data the data in json format
 */
static int m_parse_template(bvm *vm) {
	std::string file = be_tostring(vm, 1);
	json data  	 = json::parse(be_tostring(vm, 2));

	Template tpl      = env.parse_template(file);
	std::string result = env.render(tpl, data);

	be_pushstring(vm, result.c_str());
	be_return(vm);
}


/***
 * Render template
 * @function render
 * @tparam string tpl the template
 * @tparam string data the data in json format
 */
static int m_render(bvm *vm) {
	std::string tpl = be_tostring(vm, 1);
	json data  	= json::parse(be_tostring(vm, 2));

	std::string result;
	result = env.render(tpl, data);

	be_pushstring(vm, result.c_str());
	be_return(vm);
}


/***
 * Render file
 * @function render_file
 * @tparam string file the tpl file
 * @tparam string data the data in json format
 */
static int m_render_file(bvm *vm) {
	std::string file = be_tostring(vm, 1);
	json data  	 = json::parse(be_tostring(vm, 2));

	std::string result;
	result = env.render_file(file, data);

	be_pushstring(vm, result.c_str());
	be_return(vm);
}


/***
 * Read the template file (and the json file) directly from the environment
 * @function render_file_with_json_file
 * @tparam string file the tpl
 * @tparam string data the data in json format
 */
static int m_render_file_with_json_file(bvm *vm) {
	std::string file = be_tostring(vm, 1);
	json data  	 = json::parse(be_tostring(vm, 2));

	std::string result;
	result = env.render_file_with_json_file(file, data);

	be_pushstring(vm, result.c_str());
	be_return(vm);
}


/***
 * Write a rendered template/or file to file
 * @function write
 * @tparam string tpl the tpl or filepath
 * @tparam string data the data in json format
 * @tparam string out  the file out
 */
static int m_write(bvm *vm) {
	std::string tpl  = be_tostring(vm, 1);
	json data        = json::parse(be_tostring(vm, 2));
	std::string out  = be_tostring(vm, 3);

	std::ofstream file(out);
	if ( access( tpl.c_str(), F_OK ) != -1 ) { /*is file*/
		file << env.render_file(tpl, data);
	} else {
		file << env.render(tpl, data);
	}
	file.close();

	be_return(vm);
}


/***
 * Write a rendered template file
 * @function write_with_json_file
 * @tparam string tpl the filepath
 * @tparam string data json filepath 
 * @tparam string out  the out filepath
 */
static int m_write_with_json_file(bvm *vm) {
	std::string file = be_tostring(vm, 1);
	std::string data = be_tostring(vm, 2);
	std::string out  = be_tostring(vm, 3);

	env.write_with_json_file(file, data, out);

	be_return(vm);
}


/***
 * Sets the opener and closer for template expressions.
 * @function set_expression
 * @tparam string opener opener expression
 * @tparam string close  close expression
 */
static int m_set_expression(bvm *vm) {
	std::string open  = be_tostring(vm, 1);
	std::string close = be_tostring(vm, 2);

	env.set_expression(open, close);

	be_return(vm);
}


/***
 * Sets the opener and closer for template comments.
 * @function set_comment
 * @tparam string opener opener comments
 * @tparam string close  close comments
 */
static int m_set_comment(bvm *vm) {
	std::string open  = be_tostring(vm, 1);
	std::string close = be_tostring(vm, 2);

	env.set_comment(open, close);

	be_return(vm);
}


/***
 * Sets the opener and closer for template statements.
 * @function set_statement
 * @tparam string opener opener statements
 * @tparam string close  close statements
 */
static int m_set_statement(bvm *vm) {
	std::string open  = be_tostring(vm, 1);
	std::string close = be_tostring(vm, 2);

	env.set_statement(open, close);

	be_return(vm);
}


/***
 * Sets the element notation syntax.
 * @function set_search_included_templates_in_files
 * @tparam boolean search_in_files 
 */
static int m_set_search_included_templates_in_files(bvm *vm) {
	bool search_in_files = be_tobool(vm, 1);
	env.set_search_included_templates_in_files(search_in_files);

	be_return_nil(vm);
	be_return(vm);
}


/***
 * Sets whether to remove the first newline after a block.
 * @function set_trim_blocks
 * @tparam boolean search_in_files 
 */
static int m_set_trim_blocks(bvm *vm) {
	bool trim_blocks = be_tobool(vm, 1);
	env.set_trim_blocks(trim_blocks);

	be_return_nil(vm);
	be_return(vm);
}


/***
 * Sets whether to remove the first newline after a block.
 * @function set_lstrip_blocks
 * @tparam boolean search_in_files 
 */
static int m_set_lstrip_blocks(bvm *vm) {
	bool lstrip_blocks = be_tobool(vm, 1);
	env.set_lstrip_blocks(lstrip_blocks);

	be_return_nil(vm);
	be_return(vm);
}


/***
 * Sets whether a missing include will throw an error.
 * @function set_throw_at_missing_includes
 * @tparam boolean will_throw 
 */
static int m_set_throw_at_missing_includes(bvm *vm) {
	bool will_throw = be_tobool(vm, 1);
	env.set_throw_at_missing_includes(will_throw);

	be_return_nil(vm);
	be_return(vm);
}


/***
 * Sets the opener for template line statements.
 * @function set_line_statement
 * @tparam string opener opener statements
 */
static int m_set_line_statement(bvm *vm) {
	std::string open = be_tostring(vm, 1);
	env.set_line_statement(open);

	be_return_nil(vm);
	be_return(vm);
}


/***
 * Include in-memory templates
 * @function include_template
 * @tparam string name the name to save in memory
 * @tparam string tmpl the template
 */
static int m_include_template(bvm *vm) {
	const std::string name = be_tostring(vm, 1);
	const std::string tmpl = be_tostring(vm, 2);

	inja::Template content = env.parse(tmpl);
	env.include_template(name,content);

	be_return_nil(vm);
	be_return(vm);
}



static void bind_member(bvm *vm, const char *attr, bntvfunc f){
    be_pushntvfunction(vm, f);
    be_setmember(vm, -2, attr);
    be_pop(vm, 1);
}

extern "C" {
MODULE_API int berry_export(bvm *vm) {
	be_newmodule(vm);
	be_setname(vm, -1, "inja");
	bind_member(vm, "version", m_version);
	bind_member(vm, "parse_template", m_parse_template);
	bind_member(vm, "render", m_render);
	bind_member(vm, "render_file", m_render_file);
	bind_member(vm, "render_file_with_json_file",
		m_render_file_with_json_file
	);
	bind_member(vm, "write",m_write);
	bind_member(vm, "write_with_json_file",
		m_write_with_json_file
	);
	bind_member(vm, "set_expression", m_set_expression);
	bind_member(vm, "set_comment", m_set_comment);
	bind_member(vm, "set_statement", m_set_statement);
	bind_member(vm, "set_line_statement", m_set_line_statement);
	bind_member(vm,"set_search_included_templates_in_files",
		m_set_search_included_templates_in_files
	);
	bind_member(vm, "set_trim_blocks", m_set_trim_blocks);
	bind_member(vm, "set_lstrip_blocks", m_set_lstrip_blocks);
	bind_member(vm, "set_throw_at_missing_includes",
		m_set_throw_at_missing_includes
	);
	bind_member(vm, "set_line_statement", m_set_line_statement);
	bind_member(vm, "include_template", m_include_template);
	be_return(vm);
}
}
