use std::io::{self};

fn ackermann(x: i32, y: i32) -> i32 {
    match (x, y) {
        (_, 0) => 0,
        (0, y) => 2 * y,
        (_, 1) => 2,
        (x, y) => ackermann(x - 1, ackermann(x, y - 1)),
    }
}

fn pascal_triangle(row: i32, col: i32) -> i32 {
    if col == 0 || col == row {
        1
    } else {
        pascal_triangle(row - 1, col - 1) + pascal_triangle(row - 1, col)
    }
}

pub fn main() {
    let mut stack: Vec<i32> = Vec::new();
    loop {

        let mut input = String::new();
        match io::stdin().read_line(&mut input) {
            Ok(0) => break, // End of input
            Ok(_) => {
                let input = input.trim();
                if input == "q" {
                    break;
                }

                match input {
                    "+" => {
                        if stack.len() < 2 {
                            eprintln!("Stack underflow");
                            continue;
                        }
                        let b = stack.pop().unwrap();
                        let a = stack.pop().unwrap();
                        stack.push(a + b);
                    }
                    "-" => {
                        if stack.len() < 2 {
                            eprintln!("Stack underflow");
                            continue;
                        }
                        let b = stack.pop().unwrap();
                        let a = stack.pop().unwrap();
                        stack.push(a - b);
                    }
                    "*" => {
                        if stack.len() < 2 {
                            eprintln!("Stack underflow");
                            continue;
                        }
                        let b = stack.pop().unwrap();
                        let a = stack.pop().unwrap();
                        stack.push(a * b);
                    }
                    "/" => {
                        if stack.len() < 2 {
                            eprintln!("Stack underflow");
                            continue;
                        }
                        let b = stack.pop().unwrap();
                        let a = stack.pop().unwrap();
                        if b == 0 {
                            eprintln!("Division by zero");
                            stack.push(a);
                            stack.push(b);
                            continue;
                        }
                        stack.push(a / b);
                    }
                    "c" => stack.clear(),
                    "p" => println!("{:?}", stack),
                    "A" => {
                        if stack.len() < 2 {
                            eprintln!("Stack underflow");
                            continue;
                        }
                        let y = stack.pop().unwrap();
                        let x = stack.pop().unwrap();
                        stack.push(ackermann(x, y));
                    }
                    "pascal-triangle" => {
                        if stack.len() < 2 {
                            eprintln!("Stack underflow");
                            continue;
                        }
                        let col = stack.pop().unwrap();
                        let row = stack.pop().unwrap();
                        stack.push(pascal_triangle(row, col));
                    }
                    num => match num.parse::<i32>() {
                        Ok(n) => stack.push(n),
                        Err(_) => eprintln!("Invalid input: {}", num),
                    },
                }
            }
            Err(_) => {
                eprintln!("Failed to read input");
                break;
            }
        }
    }
}



#[cfg(test)]
mod tests {
    use super::*;

    fn execute_rpn(input: Vec<&str>) -> Vec<i32> {
        let mut stack: Vec<i32> = Vec::new();

        for line in input {
            match line {
                "+" => {
                    if stack.len() < 2 {
                        panic!("Stack underflow");
                    }
                    let b = stack.pop().unwrap();
                    let a = stack.pop().unwrap();
                    stack.push(a + b);
                }
                "-" => {
                    if stack.len() < 2 {
                        panic!("Stack underflow");
                    }
                    let b = stack.pop().unwrap();
                    let a = stack.pop().unwrap();
                    stack.push(a - b);
                }
                "*" => {
                    if stack.len() < 2 {
                        panic!("Stack underflow");
                    }
                    let b = stack.pop().unwrap();
                    let a = stack.pop().unwrap();
                    stack.push(a * b);
                }
                "/" => {
                    if stack.len() < 2 {
                        panic!("Stack underflow");
                    }
                    let b = stack.pop().unwrap();
                    let a = stack.pop().unwrap();
                    if b == 0 {
                        panic!("Division by zero");
                    }
                    stack.push(a / b);
                }
                "c" => stack.clear(),
                "A" => {
                    if stack.len() < 2 {
                        panic!("Stack underflow");
                    }
                    let y = stack.pop().unwrap();
                    let x = stack.pop().unwrap();
                    stack.push(ackermann(x, y));
                }
                "pascal-triangle" => {
                    if stack.len() < 2 {
                        panic!("Stack underflow");
                    }
                    let col = stack.pop().unwrap();
                    let row = stack.pop().unwrap();
                    stack.push(pascal_triangle(row, col));
                }
                "p" => {}
                num => match num.parse::<i32>() {
                    Ok(n) => stack.push(n),
                    Err(_) => panic!("Invalid input: {}", num),
                },
            }
        }

        stack
    }

    #[test]
    fn test_addition() {
        let input = vec!["3", "4", "+", "p"];
        assert_eq!(execute_rpn(input), vec![7]);
    }

    #[test]
    fn test_pascal_triangle() {
        let input = vec!["3", "2", "pascal-triangle", "p"];
        assert_eq!(execute_rpn(input), vec![3]);
    }

    #[test]
    fn test_ackermann() {
        let input1 = vec!["1", "10", "A", "p"];
        assert_eq!(execute_rpn(input1), vec![1024]);

        let input2 = vec!["0", "4", "A", "p"];
        assert_eq!(execute_rpn(input2), vec![8]);

        let input3 = vec!["1", "0", "A", "p"];
        assert_eq!(execute_rpn(input3), vec![0]);

        let input4 = vec!["2", "4", "A", "p"];
        assert_eq!(execute_rpn(input4), vec![65536]);
    }
}
