## berry-inja: Berry bindings for inja template engine

berry-inja is a Berry binding library for the [{{ inja }}](https://github.com/pantor/inja) template engine.

It runs on GNU/Linux and requires [Berry](https://github.com/Skiars/berry/) (>=0.1.10) and
[nlohmann-json](https://github.com/nlohmann/json) (>=3.7.3)

_Authored by:_ _[Díaz Devera Víctor Diex Gamar (Máster Vitronic)](https://www.linkedin.com/in/Master-Vitronic)_

[![Berry logo](./docs/berry.svg)](https://github.com/Skiars/berry/)

### License

MIT license . See [LICENSE](./LICENSE).

### Documentation

See the [Reference Manual](https://vitronic.gitlab.io/berry-inja/).


### Motivation:

[Berry](https://github.com/Skiars/berry/) is an interpreted language with
a lot of potential, so I have set out to scale it with a series of general
purpose bindings, this binding is part of a toolkit to make Berry capable
of being a web development language.


#### Getting and installing

```sh
$ git clone https://gitlab.com/vitronic/berry-inja.git
$ cd berry-inja
berry-inja$ make
berry-inja$ make install # or 'sudo make install' (Ubuntu)
```

### Tutorial

This tutorial will give you an idea how to use inja. It will explain the
most important concepts and give practical advices using examples code.
Beside this tutorial, you may check out the [documentation](https://vitronic.gitlab.io/berry-inja/).

#### Template Rendering

The basic template rendering takes a template as a string and a json
object for all data. It returns the rendered template as an string.

```ruby
#- Render a string with json data -#
import inja
import json
var data = json.dump({"name":"world"})

var tpl  = "Hello {{ name }}!"
inja.render(tpl, data) # Returns string "Hello world!"

#Or directly read a template file
tpl = inja.parse_template("./templates/greeting.txt");
result = inja.render(tpl, data); # "Hello world!"


result = inja.render(tpl, json.dump({"name":"Inja"})); # "Hello Inja!"

#Or read the template file (and/or the json file) directly from the environment
result = inja.render_file("./templates/greeting.txt", data)
result = inja.render_file_with_json_file("./templates/greeting.txt", "./data.json")

#Or write a rendered template file
inja.write(tpl, data, "./result.txt");
inja.write_with_json_file("./templates/greeting.txt", "./data.json", "./result.txt");

```

The configured to your needs.

```ruby

# With other opening and closing strings (here the defaults)
inja.set_expression("{{", "}}"); #Expressions
inja.set_comment("{#", "#}"); #Comments
inja.set_statement("{%", "%}"); #Statements {% %} for many things, see below
inja.set_line_statement("##"); #Line statements ## (just an opener)

```


#### Variables

Variables are rendered within the {{ ... }} expressions.

```ruby
import inja
import json

data = json.dump({
	"guests" : [
		"Jeff", "Tom", "Patrick"
	],
	"time"  : {
		"start" : 16,
		"end" : 22
	}
})
# Indexing in array
result = inja.render("{{ guests.1 }}", data )
print(result)  #"Tom"

#Objects
result = inja.render("{{ time.start }} to {{ time.end + 1 }}pm", data)
print(result) #"16 to 23pm"

```

#### Statements

Statements can be written either with the {% ... %} syntax or the ##
syntax for entire lines. Note that ## needs to start the line without
indentation. The most important statements are loops, conditions and file
includes. All statements can be nested.

#### Loops

```ruby

# Combining loops and line statements
tpl = "Guest List:\n"+
"## for guest in guests\n"+
"	{{ loop.index1 }}: {{ guest }}\n"+
"## endfor\n"
result = inja.render(tpl, data)
print(result)

#- Guest List:
	1: Jeff
	2: Tom
	3: Patrick -#

```

In a loop, the special variables loop/index (number), loop/index1 (number),
loop/is_first (boolean) and loop/is_last (boolean) are defined. In nested loops,
the parent loop variables are available e.g. via loop/parent/index.
You can also iterate over objects like {% for key, value in time %}.

#### Conditions
Conditions support the typical if, else if and else statements.
Following conditions are for example possible:

```ruby
data = json.dump({
	"guests" : [
		"Jeff", "Tom", "Patrick", "neighbour"
	],
	"time"  : {
		"start" : 16,
		"end" : 22,
		"hour" : 20
	},
	"guest_count" : 2,
	"all_tired"   : true
})

#Standard comparisons with a variable
result = inja.render("{% if time.hour >= 20 %}Serve{% else if time.hour >= 18 %}Make{% endif %} dinner.", data);
print(result) #Serve dinner.

#Logical operations
result = inja.render("{% if guest_count < 3 and all_tired %}Sleepy...{% else %}Keep going...{% endif %}", data); 
print(result) # Sleepy...

#Negations
result = inja.render("{% if not guest_count %}The End{% endif %}", data);
print(result) #nil

```

#### Includes
You can either include other in-memory templates or from the file system.

```ruby

# To include in-memory templates, add them to the environment first
data = json.dump({"author": "Pantor"})
inja.include_template("content", "Hello {{ author }}!")
result = inja.render("Content: {% include \"content\" %}", data)
print( result )


# Other template files are included relative from the current file location
inja.render("{% include \"footer.html\" %}", data)

# You can disable to search for templates in the file system via
inja.set_search_included_templates_in_files(false)

```

~~Inja will throw an inja::RenderError if an included file is not found.
To disable this error, you can call
inja.set_throw_at_missing_includes(false).~~ this is currently not working


#### Assignments

Variables can also be defined within the template using the set statment.

```ruby
inja.render("{% set new_hour=23 %}{{ new_hour }}pm", data) # "23pm"
```


#### Functions
A few functions are implemented within the inja template syntax.
They can be called with

```ruby
import inja
import json
data = json.dump({
	"guests" : [
		"Jeff", "Tom", "Patrick"
	],
	"time"  : {
		"start" : 16,
		"end" : 22,
		"hour" : 20
	},
	"neighbour" : 'peter'
})

# Upper and lower function, for string cases
print(inja.render("Hello {{ upper(neighbour) }}!", data)) #  "Hello PETER!"
print(inja.render("Hello {{ lower(neighbour) }}!", data)) #  "Hello peter!"

# Range function, useful for loops
print(inja.render("{% for i in range(4) %}{{ loop.index1 }}{% endfor %}", data)) #  "1234"
print(inja.render("{% for i in range(3) %}{{ at(guests, i) }} {% endfor %}", data)) #  "Jeff Tom Patrick "

# Length function (please don't combine with range, use list directly...)
print(inja.render("I count {{ length(guests) }} guests.", data)) #  "I count 3 guests."

# Get first and last element in a list
print(inja.render("{{ first(guests) }} was first.", data)) #  "Jeff was first."
print(inja.render("{{ last(guests) }} was last.", data)) #  "Patrick was last."

# Sort a list
print(inja.render("{{ sort([3,2,1]) }}", data)) #  "[1,2,3]"
print(inja.render("{{ sort(guests) }}", data)) #  "["Jeff","Patrick","Tom"]"

# Round numbers to a given precision
print(inja.render("{{ round(3.1415, 0) }}", data)) #  3.0
print(inja.render("{{ round(3.1415, 3) }}", data)) #  3.142

# Check if a value is odd, even or divisible by a number
print(inja.render("{{ odd(42) }}", data)) #  false
print(inja.render("{{ even(42) }}", data)) #  true
print(inja.render("{{ divisibleBy(42, 7) }}", data)) #  true

# Maximum and minimum values from a list
print(inja.render("{{ max([1, 2, 3]) }}", data)) #  3
print(inja.render("{{ min([-2.4, -1.2, 4.5]) }}", data)) #  -2.4

# Convert strings to numbers
print(inja.render("{{ int(\"2\") == 2 }}", data)) #  true
print(inja.render("{{ float(\"1.8\") > 2 }}", data)) #  false

# Set default values if variables are not defined
print(inja.render("Hello {{ default(neighbour, \"my friend\") }}!", data)) #  "Hello peter!"
print(inja.render("Hello {{ default(colleague, \"my friend\") }}!", data)) #  "Hello my friend!"

# Check if a key exists in an object
print(inja.render("{{ exists(\"guests\") }}", data)) #  "true"
print(inja.render("{{ exists(\"city\") }}", data)) #  "false"
print(inja.render("{{ existsIn(time, \"start\") }}", data)) #  "true"
print(inja.render("{{ existsIn(time, neighbour) }}", data)) #  "false"

# Check if a key is a specific type
print(inja.render("{{ isString(neighbour) }}", data)) # "true"
print(inja.render("{{ isArray(guests) }}", data)) # "true"
# Implemented type checks: isArray, isBoolean, isFloat, isInteger, isNumber, isObject, isString,

```

#### Whitespace Control

In the default configuration, no whitespace is removed while rendering
the file. To support a more readable template style, you can configure
the environment to control whitespaces before and after a statement
automatically. While enabling set_trim_blocks removes the first newline
after a statement, set_lstrip_blocks strips tabs and spaces from the
beginning of a line to the start of a block.

```
inja.set_trim_blocks(true) #this currently has no effect
inja.set_lstrip_blocks(true) #this currently has no effect
```
With both trim_blocks and lstrip_blocks enabled, you can put statements
on their own lines. Furthermore, you can also strip whitespaces for both
statements and expressions by hand. If you add a minus sign (-) to the
start or end, the whitespaces before or after that block will be removed:

```ruby
inja.render("Hello       {{- name -}}     !", data) # "Hello Inja!"
inja.render("{% if neighbour in guests -%}   I was there{% endif -%}   !", data) # Renders without any whitespaces
```

Stripping behind a statement or expression also removes any newlines.

#### Comments

Comments can be written with the {# ... #} syntax.

```ruby
inja.render("Hello{# Todo #}!", data)  #"Hello!"
```

Other examples can be found in the **examples/** directory contained in the release package

### known

Variable in list does not work
 
```ruby
#Variable in list
inja.render("{% if neighbour in guests %}Turn up the music!{% endif %}", data) #Turn up the music!
# fail with exception "variable 'neighbour' not found"
```

This currently has no effect

```ruby
inja.set_trim_blocks(true) 
inja.set_lstrip_blocks(true)
```


### Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
