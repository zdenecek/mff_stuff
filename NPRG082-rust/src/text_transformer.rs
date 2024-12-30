use std::env;

fn remove_accents(c: char) -> Option<char> {
    match c {
        'á' | 'à' | 'ä' | 'â' => None,
        'é' | 'è' | 'ë' | 'ê' => None,
        'í' | 'ì' | 'ï' | 'î' => None,
        'ó' | 'ò' | 'ö' | 'ô' => None,
        'ú' | 'ù' | 'ü' | 'û' => None,
        'ç' => None,
        'ñ' => None,
        'ý' | 'ÿ' => None,
        'Á' | 'À' | 'Ä' | 'Â' => None,
        'É' | 'È' | 'Ë' | 'Ê' => None,
        'Í' | 'Ì' | 'Ï' | 'Î' => None,
        'Ó' | 'Ò' | 'Ö' | 'Ô' => None,
        'Ú' | 'Ù' | 'Ü' | 'Û' => None,
        'Ç' => None,
        'Ñ' => None,
        'Ý' | 'Ÿ' => None,
        '\'' | '"' | '\\' => None,
        _ if c.is_ascii() => Some(c),
        _ => None, 
    }
}

fn slugify(text: &str) -> String {
    text.to_lowercase() 
        .replace(' ', "-") 
        .chars() 
        .filter_map(remove_accents) 
        .collect() 
}

pub fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 3 {
        eprintln!("Usage: <operation> <text>");
        return;
    }

    let operation = &args[1];
    let text = &args[2];

    let result = match operation.as_str() {
        "lowercase" => text.to_lowercase(),
        "uppercase" => text.to_uppercase(),
        "no-spaces" => text.replace(" ", ""),
        "slugify" => slugify(text),
        _ => {
            eprintln!("Unknown operation: {}", operation);
            return;
        }
    };

    println!("{}", result);
}
