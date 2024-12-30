#!/usr/bin/env python3

import argparse
import io
import os
import sys

import roman
import jinja2
import yaml

class StoreValueAction(argparse.Action):
    # From python docs  
    def __init__(self, option_strings, dest, nargs=None, **kwargs):
        if nargs is not None:
            raise ValueError("nargs not allowed")
        super().__init__(option_strings, dest, **kwargs)

    def __call__(self, parser, namespace, values, option_string=None):
        if(option_string != "-V"):
            raise ValueError(f"Invalid invocation: {option_string}",)

        if "=" not in values:
            raise ValueError(f"invalid argument {values}")
        key, val = values.split("=", maxsplit=1)
        if "V" not in namespace or namespace.V == None:
            setattr(namespace, "V", dict())

        namespace.V[key] = val    

def print_e(*args):
    print(*args, file=sys.stderr)

def jinja_filter_liters_to_imperial_gallons(text):
    return float(text) * 0.2199692

def jinja_filter_liters_to_us_gallons(text):
    return float(text) * 0.264172

def jinja_filter_arabic2roman(text):
    try:
        num = int(text)
        assert num > 1
        return roman.toRoman(num)
    except:
        print_e(f"Warning: arabic2roman: unable to convert {text}.")
        return "NaN"


def get_jinja_environment(template_dir, config):
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(template_dir),
                             autoescape=jinja2.select_autoescape(['html', 'xml']),
                             extensions=['jinja2.ext.do'])
    env.filters['l2gal'] = jinja_filter_liters_to_us_gallons if config.use_us_gallons else jinja_filter_liters_to_imperial_gallons
    env.filters['arabic2roman'] = jinja_filter_arabic2roman
    return env

def read_file(file):
    with open(file, 'r') as f:
        
        header = ""
        content = ""

        line =  f.readline()
        if line.rstrip() == "---":
            for line in f:
                if line.rstrip() == "---":
                    break
                header += line
            print(header)
        else:
            content = line

        content += f.read()

        return header, content

def parse_header(header):
    try:
        return yaml.safe_load(header)
    except yaml.YAMLError as exc:
        print_e("Invalid header")


def run(argv):
    args = argparse.ArgumentParser(description='Templater')
    args.add_argument(
        '--template',
        dest='template',
        required=True,
        metavar='FILENAME.j2',
        help='Jinja2 template file')
    args.add_argument(
        '--input',
        dest='input',
        required=True,
        metavar='INPUT',
        help='Input filename'
    )
    args.add_argument(
        '--use-us-gallons',
        action="store_true",
        help='Use US gallons, else imperial gallons will be used'
    )
    args.add_argument(
        '-V',
        action=StoreValueAction,
        help='Add custom parameters: -V "test=My value"'
    )

    config = args.parse_args(argv)

    env = get_jinja_environment(os.path.dirname(config.template), config)
    template = env.get_template(config.template)

    header, content = read_file(config.input)
    parsed_header = parse_header(header) or {}

    custom_vars = config.V or {}

    variables = custom_vars | parsed_header | {
        'content': content,
        'TEMPLATE': config.template,
        'INPUT': config.input,
    } 

    # Use \n even on Windows
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, newline='\n')

    result = template.render(variables)

    print(result)



def main():
    run(sys.argv[1:])

if __name__ == '__main__':
    main()
